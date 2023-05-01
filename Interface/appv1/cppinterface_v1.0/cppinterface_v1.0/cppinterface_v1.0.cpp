// cppinterface_v1.0.cpp : Defines the entry point for the console application.
//
/*

// in case of Linker errors: goto Project-> Properties -> linker->input ->Additional library directory add(<edit>):ws2_32.lib || for UDP connection
// beware of Warnings! set to x64 platform unless using x86
*ERROR cannot open source file "myo/myo.hpp"
for compiler settings refer to Start Here page>>myo sdk ... in myo sdk folder

also copy myo64.dll in system32 folder and your project folder if that didn't work
i have disabled error number 4996 in settings c++ advanced... to work with getch()


**remember to change build tool for different versions of visual studio 
project>Properties>platform toolset >>change it to the installed version
then,right click on Solution>retarget...>ok



/matlab harnesses Intel math kernel Library for deep learning  MKL-DNN
//first install cmake on your PC then follow the link bellow
//follow second answer related to matlab version 2021
//https://www.mathworks.com/matlabcentral/answers/447387-matlab-coder-how-do-i-build-the-intel-mkl-dnn-library-for-deep-learning-c-code-generation-and-dep
//regarding any error :
//project -> properties -> C/C++ -> additional include library (may require modification when moving project folder )
//project -> properties -> Linker ->general ==> modify folder
//project -> properties -> Linker -> input : add mkldnn.lib
//dnnl.lib
//mclmcrrt.lib
//mclmcr.lib
//add #include "stdafx.h" to every .cpp file matlab generated!
copy and paste tmwtypes.h to prj folder and to Header files
//enjoy!
copy c:program files/mkl-dnn both lib and include to project directory! 
*/

#include "stdafx.h"
#include <array>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <fstream>
//#include <windows.h>
#include <cstdlib>
#include <cmath>
#include <conio.h>
#include <chrono>

#include <Eigen/Dense>

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <myo/myo.hpp>
#include <myo/libmyo.h>
#include <mouseAPI_SE.hpp>

#include <objbase.h>	// Needed for COM functionality
#include "xsens_cmt.h"
#include "LSTMnet_DatasetF.h"
#include "LSTMnet_DatasetF_terminate.h"

using namespace std;

#define BUFFERSIZE 128



//#define MAXFORCE 31.27//lstm data 1shahrivar
//#define MINFORCE -54.3
#define MAXFORCE 50.0//lstm data 9shahrivar
#define MINFORCE -31.0

#define EXIT_ON_ERROR(res,comment) if (res != XRV_OK) { printf("Error %d occurred in " comment ": %s\n",res,cmtGetResultText(res)); exit(1); }
#define KEY "17150-A7209-54C77-39EF0"



string dateofrun = "9shahrivar1401_results3_FFNN";
double netInputs[3];
float mintorque = 0.06;// used for calibration max min network
float maxtorque = 0.25;
float meantorque = 0;
//LSTM standardize data Use matlab for calculation
double input_mean[3] = { 10.5447,8.5231,7.4366 };
double output_mean = -16.4213;
double input_std[3] = { 14.3966,8.2539,7.5604 };
double out_std = 26.3591;

SOCKET socketSend;
SOCKET socketRec;

//IMU------
long instance = -1;
// used to signal that the user initiated the exit, so we do not wait for an extra keypress-
int userQuit = 0;
CmtOutputMode mode;
CmtOutputSettings settings;
unsigned short mtCount = 0;
int screenSensorOffset = 0;
int temperatureOffset = 0;
CmtDeviceId deviceIds[256];
CmtVector positionLLA;


void doHardwareScan();
void doMtSettings(void);
void exitFunc(void);
//----

struct sockaddr_in local;//udp send 
struct sockaddr_in otherMachine;
struct sockaddr_in localrec;//udp reciever socket different PORT number 
string fn;
int otherMachinelen = sizeof(otherMachine);
fd_set set;//this is for time out setting
MouseAPI Mouse;
/*
variables for butFilt function
*///-------
float xfc[4] = { 0,0,0,0 };//first channel { x(n-2) x(n-1) x(n)}
float xsc[4] = { 0,0,0,0 };//second channel
array<float, 4> xtc = { 0,0,0,0 };//third channel

double yfc[4] = { 0,0,0,0 };//first channel
double ysc[4] = { 0,0,0,0 };//second channel
double ytc[4] = { 0,0,0,0 };//third channel
//--------

float mavg[4] = { 0,0,0,0 };

class DataCollector : public myo::DeviceListener {

	std::fstream logFile;
public:
	DataCollector()
		: emgSamples()
	{
		logFile.open("logFile" + dateofrun + ".txt", std::fstream::out);//warning this will rewrite text file and erase previous recordings
		logFile << "EMG1  EMG2  EMG3  EMG4  EMG5  EMG6  EMG7  EMG8   ActualForce  EstimatedForce\n";
	}

	// onUnpair() is called whenever the Myo is disconnected from Myo Connect by the user.
	void onUnpair(myo::Myo* myo, uint64_t timestamp)
	{
		// We've lost a Myo.
		// Let's clean up some leftover state.
		emgSamples.fill(0);
	}

	
	// onEmgData() is called whenever a paired Myo has provided new EMG data, and EMG streaming is enabled.
	void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg)
	{
		for (int i = 0; i < 8; i++) {
			emgSamples[i] = emg[i];
		}
		
	}

	// There are other virtual functions in DeviceListener that we could override here, like onAccelerometerData().
	// For this example, the functions overridden above are sufficient.

	// We define this function to print the current values that were updated by the on...() functions above.
	void log(string estimatedTorque,string actualforce)
	{

		// Clear the current line
		//std::cout << '\r';
		std::string EMGs;
		// Print out the EMG data.
		for (int8_t i = 0; i < emgSamples.size(); i++) {
			std::ostringstream oss;
			oss << static_cast<int>(emgSamples[i]);
			std::string emgString = oss.str();

			//std::cout << ' ' << emgString << std::string(4 - emgString.size(), ' ') << ' ';

			EMGs = EMGs + ' ' + emgString + std::string(4 - emgString.size(), ' ') + ' ';

		}
		//std::cout   << angle;
		EMGs = EMGs +  actualforce + "  " + estimatedTorque + '\n';
		logFile << EMGs;
		//cout << EMGs;

		//std::cout << std::flush;
	}

	// The values of this array is set by onEmgData() above.
	 std::array<int8_t, 8> emgSamples;
	 
	
};


void InitWinsock()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	otherMachine.sin_family = AF_INET;
	otherMachine.sin_port = htons(30002);
	//otherMachine.sin_addr.s_addr = inet_addr("172.31.1.147");//kuka ip
	InetPton(AF_INET, _T("172.31.1.147"), &otherMachine.sin_addr.s_addr);//kuka ip


	int otherMashinelen = sizeof(otherMachine);

	local.sin_family = AF_INET;
	local.sin_port = htons(50003);
	//local.sin_addr.s_addr = inet_addr("172.31.1.150");// pc 
	InetPton(AF_INET, _T("172.31.1.150"), &local.sin_addr.s_addr);

	localrec.sin_family = AF_INET;
	localrec.sin_port = htons(50004);
	//local.sin_addr.s_addr = inet_addr("172.31.1.150");// pc 
	InetPton(AF_INET, _T("172.31.1.150"), &localrec.sin_addr.s_addr);

	socketSend = socket(AF_INET, SOCK_DGRAM, 0);
	bind(socketSend, (sockaddr*)&local, sizeof(local));

	socketRec = socket(AF_INET, SOCK_DGRAM, 0);
	bind(socketRec, (sockaddr*)&localrec, sizeof(localrec));


	FD_ZERO(&set); /* clear the set *///this is for time out setting 
	FD_SET(socketSend, &set); /* add our file descriptor to the set *///this is for time out setting 
	DWORD recTimeOut = 500;
	setsockopt(socketRec, SOL_SOCKET, SO_RCVTIMEO, (const char*)&recTimeOut, sizeof(recTimeOut));
}

void sendViaUDP(string msg)
{
	char sbuffer[BUFFERSIZE];//u have to use same buffer size on both sides (pc and kuka)!!!!!!!!!!!
	strcpy(sbuffer, msg.c_str());
	sendto(socketSend, sbuffer, sizeof(sbuffer), 0, (sockaddr*)&otherMachine, otherMachinelen);
	ZeroMemory(sbuffer, sizeof(sbuffer));
}

string recieveViaUDP()
{
	char rbuffer[BUFFERSIZE];
	ZeroMemory(rbuffer, sizeof(rbuffer));

	int rv = 1;// select(socketS, &set, NULL, NULL, &timeout);//this will wait until timeout occures...
			   //select(...) checks if there is any data ready BUUUUUTT it makes the connection vulnerable so i comented it ans use setsockop(..) instead
	if (rv == SOCKET_ERROR)
	{
		printf("select error");
	}
	else if (rv == 0)
	{
		printf("timeout, socket does not have anything to read");
		// timeout, socket does not have anything to read
	}
	else
	{
		// socket has something to read
		int recv_size = recv(socketRec, rbuffer, sizeof(rbuffer), 0);
		if (recv_size == SOCKET_ERROR)
		{
			// read failed...
			printf("read from UDP connection failed\n");
		}
		else if (recv_size == 0)
		{
			// peer disconnected...
			printf("peer disconnected");
		}
		else
		{
			// read successful...

			//printf("t");
			//printf("\n");

		}
	}

	return rbuffer;
}


array<float, 3> butterWorthFilt(int emgdata0, int emgdata1, int emgdata2)
{
	/*ButterWorth Filter
	https://www.mathworks.com/help/matlab/ref/filter.html
	Y = filter(B,A,X) filters the data in vector X with the
	filter described by vectors A and B to create the filtered
	data Y.  The filter is a "Direct Form II Transposed"
	implementation of the standard difference equation:

	a(1)*y(n) = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
	- a(2)*y(n-1) - ... - a(na+1)*y(n-na)

	If a(1) is not equal to 1, filter normalizes the filter
	coefficients by a(1).

	*/
	//const int max = 150;
	//const double a_buttFilt[] = { 1.0, -1.900236937281604, 0.904982200030122 };//2nd order cutoff=2,fs=178
	//const double b_buttFilt[] = { 0.001186315687129 , 0.002372631374259 , 0.001186315687129 };
	//3nd order cutoff=2,fs=178
	//const double a_buttFilt[] = { 1.000000000000000, - 2.858834018362215  , 2.727456626752148, - 0.868294328440301 };
	//const double b_buttFilt[] = { 0.000041034993703948 ,  0.000123104981111843  , 0.000123104981111843 ,  0.000041034993703948 };
	
	//3nd order cutoff=3,fs=178
	const double a_buttFilt[] = { 1.000000000000000 ,-2.788305265051546  , 2.598428482198820 ,-0.809052067991894 };
	const double b_buttFilt[] = { 0.000133893644422484 ,  0.000401680933267452 ,  0.000401680933267452  , 0.000133893644422484 };

	array<float, 3> rectEmg = { 0,0,0 };
	array<float, 3> normEmg = { 0,0,0 };
	/*
	out of 8 channels of emg I only use 3 of them, here: 1 2 5(2 3 6 in matlab)
	*/
	rectEmg[0] = std::abs( emgdata0);
	rectEmg[1] = abs(emgdata1);
	rectEmg[2] = abs(emgdata2);
	//normEmg[0] = static_cast<double>((ymax - ymin)*(static_cast<float>(rectEmg[0]) - xmin[0]) / (xmax[0] - xmin[0]) + ymin);
	//normEmg[1] = static_cast<double>((ymax - ymin)*(static_cast<float>(rectEmg[1]) - xmin[1]) / (xmax[1] - xmin[1]) + ymin);
	//normEmg[2] = static_cast<double>((ymax - ymin)*(static_cast<float>(rectEmg[2]) - xmin[2]) / (xmax[2] - xmin[2]) + ymin);
	/*
	shift left ...
	bc new value for emg has arrived
	*/
	xfc[0] = xfc[1];
	xfc[1] = xfc[2];
	xfc[2] = xfc[3];
	xfc[3] = rectEmg[0];
	yfc[0] = yfc[1];
	yfc[1] = yfc[2];
	yfc[2] = yfc[3];

	xsc[0] = xsc[1];
	xsc[1] = xsc[2];
	xsc[2] = xsc[3];
	xsc[3] = rectEmg[1];
	ysc[0] = ysc[1];
	ysc[1] = ysc[2];
	ysc[2] = ysc[3];

	xtc[0] = xtc[1];
	xtc[1] = xtc[2];
	xtc[2] = xtc[3];
	xtc[3] = rectEmg[2];
	ytc[0] = ytc[1];
	ytc[1] = ytc[2];
	ytc[2] = ytc[3];

	/*

	a(1)*y(n) = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
	- a(2)*y(n-1) - ... - a(na+1)*y(n-na)
	*/
	/*3nd Order*/
	yfc[3] = b_buttFilt[0] * xfc[3] + b_buttFilt[1] * xfc[2] + b_buttFilt[2] * xfc[1] + b_buttFilt[3] * xfc[0]
		- a_buttFilt[1] * yfc[2] - a_buttFilt[2] * yfc[1] - a_buttFilt[3] * yfc[0];

	ysc[3] = b_buttFilt[0] * xsc[3] + b_buttFilt[1] * xsc[2] + b_buttFilt[2] * xsc[1] + b_buttFilt[2] * xsc[0]
		- a_buttFilt[1] * ysc[2] - a_buttFilt[2] * ysc[1] - a_buttFilt[3] * ysc[0];

	ytc[3] = b_buttFilt[0] * xtc[3] + b_buttFilt[1] * xtc[2] + b_buttFilt[2] * xtc[1] + b_buttFilt[2] * xtc[0]
		- a_buttFilt[1] * ytc[2] - a_buttFilt[2] * ytc[1] - a_buttFilt[3] * ytc[0];


	array<float, 3>y = { yfc[3],ysc[3],ytc[3] };

	return y;

}

float xmax[3] = { 53.193819679303552,
42.808350361864861,
52.277099517673072 };
float xmin[3] = { 0.000133893644422,
-1.029854817830196,
-1.370730774949031 };

float ymax = 1.0, ymin = -1.0;

float TorqueEstimator(array<float, 3> filtemgdata)
{
	//for network architectue
	//refer to Case Study 1: Function Approximation,  in Neural Network Design by martin T.Hagan 2nd edition
	//
	//net1Trainedwith90deg3_normal_3channel_PPN
	
	


	Eigen::MatrixXd W1(10, 3);
	Eigen::MatrixXd P(3, 1);
	Eigen::MatrixXd W2(1, 10);
	Eigen::MatrixXd b1(10, 1);
	Eigen::MatrixXd b2(1, 1);

	W1 <<
		3.5356547842015628724 ,- 0.11876461415729849813 ,4.290913216753173387,
		0.40373806230584924348 ,- 8.1933299060338349307, - 0.51466765164597072513,
		-2.8486589548942746752 ,- 0.26389823837128550954, - 1.7736585918051894772,
		6.1995204590007899625 ,- 5.4516683087377417749 ,- 9.1683169764435721305,
		-3.0761185971627504898, 0.12217331953930245236 ,- 4.0291775909973228664,
		4.6537035633212111563 ,- 2.3316320541556314616 ,- 1.651847564885474906,
		0.50159137177410806352 ,1.5552713495258154719, 0.69201124087662946316,
		-7.0102799591164322024, - 0.29897803737570671778, 0.41343397097793677109,
		-0.1404534066484203203 ,- 0.78596544253536371194, 5.9053203206607962628,
		0.087356962869227089419 ,0.51570679733325297533, - 4.6482509626354939769;

	//std::cout << W1;

	for (int j = 0; j < 3; j++) {
		if (filtemgdata[j] < xmin[j])
			xmin[j] = filtemgdata[j];

		if (filtemgdata[j] > xmax[j])
			xmax[j] = filtemgdata[j];
	}
	//y = (ymax - ymin)*(x - xmin) / (xmax - xmin) + ymin;//both x and y are related to net input y max is 1 ymin is -1 
	P << (ymax - ymin)*(filtemgdata[0] - xmin[0]) / (xmax[0] - xmin[0]) + ymin,
		(ymax - ymin)*(filtemgdata[1] - xmin[1]) / (xmax[1] - xmin[1]) + ymin,
		(ymax - ymin)*(filtemgdata[2] - xmin[2]) / (xmax[2] - xmin[2]) + ymin;



	W2 << 1.1470749525004604319 ,- 1.1021215137971729359 ,- 0.74291951564710922984, 0.48988500987217764582 ,1.321821141670223243 ,0.2886600732724604601, - 0.7470515185008987924 ,1.9354664228790807723 ,2.2009330079731945773, - 0.14806553066028230181;

	b1 << 1.7574914407635018332,
		-8.2264973327316344154,
		2.3612207561863618466,
		-7.4107860989592131418,
		-1.3527555850187240605,
		-1.8664136156940602795,
		2.629425534547977783,
		-8.234210914243073276,
		5.8216707438054173807,
		2.8963967431783572692;

	b2 << 0.092720378350795398803;


	Eigen::MatrixXd n1;
	n1 = W1 * P + b1;
	Eigen::MatrixXd a1;
	n1 = -2 * n1;
	a1 = 2 / (1 + n1.array().exp()) - 1;//a = tansig(n) = 2/(1+exp(-2*n))-1
	Eigen::MatrixXd a2;
	a2 = W2*a1 + b2;

	return static_cast<float>((a2(0)+0.2));
}

float movinavrg(float input)
{

	float output = 0.0;
	mavg[0] = mavg[1];
	mavg[1] = mavg[2];
	mavg[2] = mavg[3];
	mavg[3] = input;

	for (int ii = 0; ii < 4; ii++)
	{
		output += mavg[ii];
	}
	
	 output /= 4;
	 
		 return output;
	
}
std::wstring s2ws(const std::string& s) // function to convert string into lpcw...
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

int main()
{
	InitWinsock();
	//************Serial setup for arduino scale
	wstring com = s2ws("COM5");
	LPCWSTR COMport = com.c_str();
	HANDLE hSerial;
	hSerial = CreateFile(COMport,
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);
	if (hSerial == INVALID_HANDLE_VALUE) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			cout << "COM PORT ERORR";//serial port does not exist. Inform user.
		}
		//some other error occurred. Inform user.
	}

	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	if (!GetCommState(hSerial, &dcbSerialParams)) {
		//error getting state
	}
	dcbSerialParams.BaudRate = CBR_115200;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	if (!SetCommState(hSerial, &dcbSerialParams)) {
		//error setting serial port state
	}

	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 1;
	timeouts.ReadTotalTimeoutConstant = 5;
	timeouts.ReadTotalTimeoutMultiplier = 1;
	timeouts.WriteTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	if (!SetCommTimeouts(hSerial, &timeouts)) {
		//error occureed. Inform user
	}
	const int n = 127;
	//************End serial setup
	//int actualForce = 0;
	char actualForce[n + 1] = { 0 };
	DWORD dwBytesRead = 0;

	//xsens-----
	/*
	XsensResultValue res = XRV_OK;
	short screenSkipFactor = 10;
	short screenSkipFactorCnt = screenSkipFactor;

	// Set exit function
	atexit(exitFunc);
	printf("Creating an XsensCMT instance\n");
	char serialNumber[] = KEY;
	instance = cmtCreateInstance(serialNumber);
	if (instance != -1)
		printf("CMT instance created\n\n");
	else {
		printf("Creation of CMT instance failed, probably because of an invalid serial number\n");
		exit(1);
	}
	// lets create the Xsens CMT instance to handle the sensor(s)
	// Perform hardware scan
	if (instance != -1)
	 doHardwareScan();
	// Give user a (short) chance to see hardware scan results
	Sleep(2000);

	mode = CMT_OUTPUTMODE_CALIB | CMT_OUTPUTMODE_ORIENT;

	//settings = CMT_OUTPUTSETTINGS_ORIENTMODE_QUATERNION;
	//settings = CMT_OUTPUTSETTINGS_ORIENTMODE_EULER;
	settings = CMT_OUTPUTSETTINGS_ORIENTMODE_MATRIX;

	settings |= CMT_OUTPUTSETTINGS_TIMESTAMP_SAMPLECNT;
	if (instance != -1)
	 doMtSettings();
	// Wait for first data item(s) to arrive. In production code, you would use a callback function instead (see cmtRegisterCallback function)
	Sleep(20);
	// vars for sample counter & temp.
	unsigned short sdata;
	double tdata;
	//structs to hold data.
	CmtCalData caldata;
	CmtQuat qat_data;
	CmtEuler euler_data;
	CmtMatrix matrix_data;
	//-----
	*/
	bool hooked = false;

	float estimatedTorque = 0;
	float alpha = 0 , beta = 0;
	
	MouseAPI::MouseData mousedata;
	bool pick3 = false;
	array<int, 8> emgmeans;
	array<int, 3> pickedChannels = {0,3,4 };//3,7,8
	array<float, 3> filteredEMG = { 0,0,0 };
	
	string command;
	string errormsg = "OK";
		string kukaData;
		fstream timeFile;
		timeFile.open("timeFile" + dateofrun + ".txt", std::fstream::out);
		timeFile << "TotalElapsedTime_ms\n";
		
		myo::Hub hub("com.example.emg-data-sample");
		std::cout << "Attempting to find a Myo..." << std::endl;
		myo::Myo* myo1 = hub.waitForMyo(400);
		myo::Myo::UnlockType unlocktype = myo::Myo::UnlockType::unlockHold;

		// Next we construct an instance of our DeviceListener, so that we can register it with the Hub.
		DataCollector collector;
		// If waitForMyo() returned a null pointer, we failed to find a Myo, so exit with an error message.
		if (myo1) {
			//throw std::runtime_error("Unable to find a Myo!");
			// We've found a Myo.
			std::cout << "Connected to a Myo armband!" << std::endl << std::endl;

			// Next we enable EMG streaming on the found Myo.
			myo1->setStreamEmg(myo::Myo::streamEmgEnabled);
			hub.addListener(&collector);
			
			myo1->vibrate(myo1->vibrationShort);
			//myo1->unlock(unlocktype);
			myo1->lock();
			
			//myo1->lock();
		}		

		
		//cout << key;
		//SHORT keyState = GetKeyState(VK_RETURN);//enter key
		bool rec = false;
		

		Sleep(50);
		SHORT keyState = GetKeyState(VK_ESCAPE);//there was a bug reading this so here we read escape toggle to see if it stuck somwhere in the buffer or smt ... never mind

		Sleep(500);
		for (int i = 0; i <= 7; ++i)
			emgmeans[i] = 0;
		int cnt = 0;

		//std::cout << "\nCalibrate EMG channels??(y/n)\n";
		char key = ' ' ;// = getch();
		if (myo1 &&  key == 'y')//choosing which channel to feed into neural network
		{
			
				std::cout << "\n\nFor calibration purposes, poush your wrist towards the desk for 3 seconds:\n";
				Sleep(2000);
				std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
				std::chrono::steady_clock::time_point end;
				long long elapsedtime = 0;
				while (elapsedtime < 3000)
				{
					++cnt;
					hub.run(10);
					for (int i = 0; i <= 7;++i)
				      emgmeans[i] += abs(collector.emgSamples[i] ) ;
					end = std::chrono::steady_clock::now();
					elapsedtime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
					std::cout << "\r" << static_cast<int>(elapsedtime / 1000);
				}
				std::cout << "\n";
				for (int ii = 0; ii <= 7;++ii)
				{
					std::cout << emgmeans[ii]/cnt << "   ";
				}

				std::cout << "\n\nnow do the opposite for 3 seconds:\n";
				Sleep(2000);

				for (int i = 0; i <= 7; ++i)
					emgmeans[i] = 0;
				
			    begin = std::chrono::steady_clock::now();
				
				elapsedtime = 0;
				cnt = 0;
				while (elapsedtime < 3000)
				{
					hub.run(10);
					for (int i = 0; i <= 7; ++i)
						emgmeans[i] += abs(collector.emgSamples[i]);
					end = std::chrono::steady_clock::now();
					elapsedtime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
					std::cout << "\r" << static_cast<int>(elapsedtime / 1000);
					cnt++;
				}
				std::cout << "\n";
				for (int ii = 0; ii <= 7; ++ii)
				{
					std::cout << emgmeans[ii] / cnt << "   ";
				}
				std::cout << "\n\nEnter third channel ( CW or push or extension) number:\n";
				pickedChannels[2] = getch() - '0';
				std::cout << pickedChannels[2] <<  "\n" ;

				std::cout << "\n\nEnter first channel (CCW or lift or flexion) number:\n";
				pickedChannels[0] = getch() - '0';
				std::cout << pickedChannels[0] ;

				std::cout << "\n\nEnter second channel (also CCW or lift or flexion) number:\n";
				pickedChannels[1] = getch() - '0';
				std::cout << pickedChannels[1] ;

				

				std::cout << "\n\nAgain, poush your wrist towards the desk for 3 seconds:\n";// to mesure network maximum and minmum torque output
				Sleep(2000);
				elapsedtime = 0;
				cnt = 0;
				begin = std::chrono::steady_clock::now();
				
				while (elapsedtime < 3000)
				{
					hub.run(10);
					filteredEMG = butterWorthFilt(static_cast<int> (collector.emgSamples[pickedChannels[0]]),
						static_cast<int>(collector.emgSamples[pickedChannels[1]]),
						static_cast<int>(collector.emgSamples[pickedChannels[2]]));//filteremg data
					estimatedTorque = movinavrg(TorqueEstimator(filteredEMG));//calling neural net
					
					if (estimatedTorque < mintorque)
						mintorque = estimatedTorque;

					end = std::chrono::steady_clock::now();
					elapsedtime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
					std::cout << "\r" << static_cast<int>(elapsedtime / 1000);
					cnt++;
				}
				std::cout << "   " <<to_string( mintorque);
				
				std::cout << "\n\nNext, lift for 3 seconds:\n";
				Sleep(2000);
				elapsedtime = 0;
				cnt = 0;
				begin = std::chrono::steady_clock::now();
				
				while (elapsedtime < 3000)
				{
					hub.run(10);
					filteredEMG = butterWorthFilt(static_cast<int> (collector.emgSamples[pickedChannels[0]]),
						static_cast<int>(collector.emgSamples[pickedChannels[1]]),
						static_cast<int>(collector.emgSamples[pickedChannels[2]]));//filteremg data
					estimatedTorque = movinavrg(TorqueEstimator(filteredEMG));//calling neural net

					if (estimatedTorque > maxtorque)
						maxtorque = estimatedTorque;

					end = std::chrono::steady_clock::now();
					elapsedtime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
					std::cout << "\r" << static_cast<int>(elapsedtime / 1000);
					cnt++;
				}
				std::cout << "   " << to_string(maxtorque);


				std::cout << "\n\nLastly, just relax your arm in resting position:\n";
				Sleep(2000);
				elapsedtime = 0;
				cnt = 0;
				begin = std::chrono::steady_clock::now();

				while (elapsedtime < 3000)
				{
					hub.run(10);
					filteredEMG = butterWorthFilt(static_cast<int> (collector.emgSamples[pickedChannels[0]]),
						static_cast<int>(collector.emgSamples[pickedChannels[1]]),
						static_cast<int>(collector.emgSamples[pickedChannels[2]]));//filteremg data
					estimatedTorque = movinavrg(TorqueEstimator(filteredEMG));//calling neural net

					meantorque += estimatedTorque;

					end = std::chrono::steady_clock::now();
					elapsedtime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
					std::cout << "\r" << static_cast<int>(elapsedtime / 1000);
					cnt++;
				}
				meantorque /= 3000;
				std::cout << "   " << to_string(meantorque);
		}
		
		std::cout << "\nCalibrate Probe Heading??(y/n)\n";
		 key = getch();
		if (key == 'y')
		{
			std::cout << "\nHold Probe in appropriate config.\n";
			Sleep(2000);
			//res = cmtResetOrientation(instance, CmtResetMethod::CMT_RESETORIENTATION_ALIGN, deviceIds[0]);
			std::cout << "\nDone.\n";
		}

		Mouse.InitMouseAPI(); //**call this first
		cout << "Press 'S' to start the program\npress any key to exit.\n\n";
		key = getch();
		if (key == 's') {

			for (int c = 1; c > 0; c--)
			{
				cout << "program  will start in " << c << " seconds\n";
				Sleep(1000);
			}
			
			rec = true;
			sendViaUDP("START");
			cout << "\n\nStart .....!!!!!\npress and hold ESC to exit loop \n.\n.\n.\n.\n.\n.\n\n";//LED of robot should turn on
		}
		//std::cout << "\nNo\nEntering main loop...";
		
		
		
		// Finally we enter our main loop.
		 cnt = 0;
		 Eigen::MatrixXd rot(3, 3);
		 Eigen::MatrixXd accvector(3,1);
		 Eigen::MatrixXd rotuniv(3, 3);
		 Eigen::MatrixXd accvectorUniv(3, 1);
		 long long deltaT = 0.006;
		while (rec == true) {
			
			if (Mouse.IsMouseOverWin())
			{
				key = getch();
				std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

				while (key == 'm')// routin loop
				{
					keyState = GetKeyState(VK_ESCAPE);
					bool isDown = keyState & 0x8000;
					if (isDown) {// check if escape key has pressed
						rec = false;
						key = 'esc';
					}
					std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
					if (myo1)//if armband is available
					{
						
						hub.run(5);
						filteredEMG = butterWorthFilt(static_cast<int> (collector.emgSamples[pickedChannels[0]]), 
														static_cast<int>(collector.emgSamples[pickedChannels[1]]),
														 static_cast<int>( collector.emgSamples[pickedChannels[2]]) );//filteremg data
						estimatedTorque = movinavrg( TorqueEstimator(filteredEMG) );//calling neural net
						//y = (ymax - ymin)*(x - xmin) / (xmax - xmin) + ymin; 
						estimatedTorque = (MAXFORCE - MINFORCE) * (estimatedTorque - (-1.0)) / (1.0 - (-1.00)) + MINFORCE;
						//if (estimatedTorque >= 0 )
						//   estimatedTorque = (MAXFORCE - 0) * (estimatedTorque - 0) / (maxtorque - 0) + 0;
						//else 
						//	estimatedTorque = (0 - (-MAXFORCE)) * (estimatedTorque - mintorque) / (0 - mintorque) + -MAXFORCE;
					}
					else {
						estimatedTorque = 0;
					}
					/*
					if (instance != -1) {
						res = cmtGetNextDataBundle(instance);
						if ((mode & CMT_OUTPUTMODE_CALIB) != 0)
							res = cmtDataGetCalData(instance, &caldata, deviceIds[0]);

						if ((mode & CMT_OUTPUTMODE_ORIENT) != 0) {
							res = cmtDataGetOriEuler(instance, &euler_data, deviceIds[0]);
							res = cmtDataGetOriMatrix(instance, &matrix_data, deviceIds[0], NULL);
						}
					}
					*/
					//collector.log(kukaForces, to_string(estimatedTorque));
					mousedata = Mouse.Run();
					//std::cout << "vx:" << mousedata.vx << "  vy: " << mousedata.vy << "                      ";
					//std::cout << "\rroll:  " << euler_data.m_roll << "     " << euler_data.m_pitch << "     " << euler_data.m_yaw << "     ";
					/*
					rot << matrix_data.m_data[0][0], matrix_data.m_data[0][1], matrix_data.m_data[0][2],
						matrix_data.m_data[1][0], matrix_data.m_data[1][1], matrix_data.m_data[1][2],
						matrix_data.m_data[2][0], matrix_data.m_data[2][1], matrix_data.m_data[2][2];

					Eigen::MatrixXd t_rot = rot.array().transpose();
					rotuniv << 0, 1, 0,
						-1, 0, 0,
						0, 0, 1;//rotation matrix of imy sensor wrt robot base
						      
					accvector << caldata.m_acc.m_data[0], caldata.m_acc.m_data[1], caldata.m_acc.m_data[2];
					accvectorUniv =  t_rot * accvector ;//acceleration vector in universal coordinate system. note that z element is -9.81 
					//	matrix_data.m_data[1][0]<< matrix_data.m_data[1][1]<< matrix_data.m_data[1][2]<<
					//	matrix_data.m_data[2][0], matrix_data.m_data[2][1], matrix_data.m_data[2][2];
					//std::cout << "    " << to_string(accvectorUniv(0)*1000)<< "        ";
*/
				  // UNCOMMENT command = to_string(mousedata.vx) + "#" + to_string(mousedata.vy*-1) + "#" + to_string(-estimatedTorque / 10) + "#" + to_string(-euler_data.m_pitch) + "#" + to_string(-euler_data.m_roll) + "#" + to_string(euler_data.m_yaw) + "#" + errormsg;//
				  sendViaUDP(command);
				  collector.log(to_string(estimatedTorque), " 0 ");
				  Sleep(3);//emulate some computational effort
				  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
				  deltaT = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
					std::cout << "\r  Force(N):  " << to_string(-estimatedTorque/10) <<  "                      ";
				 
				}
				
				//while (Mouse.NkeyIsDown())// routin loop option 2 : where can be modified. Feed Forward ANN.
				while(key == 'n')
				{
					keyState = GetKeyState(VK_ESCAPE);
					bool isDown = keyState & 0x8000;
					if (isDown) {// check if escape key has pressed
						rec = false;
						key = 'esc';
					}

					std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
					if (myo1)//if armband is available
					{
						hub.run(5);
						filteredEMG = butterWorthFilt(static_cast<int> (collector.emgSamples[pickedChannels[0]]),
							static_cast<int>(collector.emgSamples[pickedChannels[1]]),
							static_cast<int>(collector.emgSamples[pickedChannels[2]]));//filteremg data
						estimatedTorque = movinavrg(TorqueEstimator(filteredEMG));//calling neural net
																				  //y = (ymax - ymin)*(x - xmin) / (xmax - xmin) + ymin; 
						if (estimatedTorque >= 0)
							estimatedTorque = (MAXFORCE - 0) * (estimatedTorque - 0) / (maxtorque - 0) + 0;
						else
							estimatedTorque = (0 - (-MAXFORCE)) * (estimatedTorque - mintorque) / (0 - mintorque) + -MAXFORCE;
					}
					else {
						estimatedTorque = 0;
					}
					/*
					res = cmtGetNextDataBundle(instance);
					if ((mode & CMT_OUTPUTMODE_CALIB) != 0)
						res = cmtDataGetCalData(instance, &caldata, deviceIds[0]);

					if ((mode & CMT_OUTPUTMODE_ORIENT) != 0) {
						res = cmtDataGetOriEuler(instance, &euler_data, deviceIds[0]);
						res = cmtDataGetOriMatrix(instance, &matrix_data, deviceIds[0], NULL);
					}*/





					//collector.log(kukaForces, to_string(estimatedTorque));
					mousedata = Mouse.Run();
					//std::cout << "\r vx:" << mousedata.vx << "  vy: " << mousedata.vy << "                      ";
					//std::cout << "\rroll:  " << euler_data.m_roll << "     " << euler_data.m_pitch << "     " << euler_data.m_yaw << "     ";
/*
					rot << matrix_data.m_data[0][0], matrix_data.m_data[0][1], matrix_data.m_data[0][2],
						matrix_data.m_data[1][0], matrix_data.m_data[1][1], matrix_data.m_data[1][2],
						matrix_data.m_data[2][0], matrix_data.m_data[2][1], matrix_data.m_data[2][2];

					Eigen::MatrixXd t_rot = rot.array().transpose();
					rotuniv << 0, 1, 0,
						-1, 0, 0,
						0, 0, 1;//rotation matrix of imu sensor wrt robot base

					accvector << caldata.m_acc.m_data[0], caldata.m_acc.m_data[1], caldata.m_acc.m_data[2];
					accvectorUniv = t_rot * accvector;//acceleration vector in universal coordinate system. note that z element is -9.81 
													  //	matrix_data.m_data[1][0]<< matrix_data.m_data[1][1]<< matrix_data.m_data[1][2]<<
													  //	matrix_data.m_data[2][0], matrix_data.m_data[2][1], matrix_data.m_data[2][2];
													  //std::cout << "    " << to_string(accvectorUniv(0)*1000)<< "        ";
					*/
					//estimatedTorque
					estimatedTorque = mousedata.pitchAngle;
					// uncommeeeent command = to_string(mousedata.vx) + "#" + to_string(mousedata.vy*-1) + "#" + to_string(estimatedTorque) + "#" + to_string(-euler_data.m_pitch) + "#" + to_string(-euler_data.m_roll) + "#" + to_string(euler_data.m_yaw) + "#" + errormsg;//
					sendViaUDP(command);
					collector.log( to_string(estimatedTorque)," 0 ");
					Sleep(3);//emulate some computational effort
					std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
					deltaT = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
					
					std::cout << "\r  Force(N):  " << to_string(estimatedTorque) << "                      ";

				}
				
				while (key == 'o')// this code is for Final test no end-effector change in pose. records weigh scale value and estimated force.
				{
					keyState = GetKeyState(VK_ESCAPE);
					bool isDown = keyState & 0x8000;
					if (isDown) {// check if escape key has pressed
						rec = false;
						key = 'esc';
					}

					std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
					string s = "s";

					strcpy(actualForce, s.c_str());
					int toWrite = strlen(actualForce);
					if (!WriteFile(hSerial, actualForce, toWrite, &dwBytesRead, NULL)) {//send command to arduino board to get actual force
						cout << "send error to Arduino error";
					}

					if (myo1)//if armband is available
					{
						hub.run(5);
						filteredEMG = butterWorthFilt(static_cast<int> (collector.emgSamples[pickedChannels[0]]),
							static_cast<int>(collector.emgSamples[pickedChannels[1]]),
							static_cast<int>(collector.emgSamples[pickedChannels[2]]));//filteremg data
						//estimatedTorque = movinavrg(-1*TorqueEstimator(filteredEMG));//calling neural net
						estimatedTorque = TorqueEstimator(filteredEMG);//calling neural net

						estimatedTorque = ((MAXFORCE - MINFORCE) * (estimatedTorque - (-1.0)) / (1.0 - (-1.00)) + MINFORCE )-11.0;														 //y = (ymax - ymin)*(x - xmin) / (xmax - xmin) + ymin; 
						
						//if (estimatedTorque >= 0)
						//	estimatedTorque = (MAXFORCE - 0) * (estimatedTorque - 0) / (0.7 - 0) + 0;
						//else
							//estimatedTorque = (0 - (MINFORCE)) * (estimatedTorque - (-0.7)) / (0 - (-0.7)) + MINFORCE;
						//if (estimatedTorque >= 0)
							//estimatedTorque = (MAXFORCE - MINFORCE) * (estimatedTorque - (-0.65)) / (0.7- (-0.65)) + MINFORCE;
						//else
						//	esmmmmmmmmmmmmmmmmmmmmmtimatedTorque = (0 - (-MAXFORCE)) * (estimatedTorque - mintorque) / (0 - mintorque) + -MAXFORCE;
					}
					else {
						estimatedTorque = 0;
						Sleep(5);
					}
					actualForce[0] = '\0';
					
					if (!ReadFile(hSerial, actualForce, n, &dwBytesRead, NULL)) {///receive data from arduino
						cout << "Force receive error from scale";
					}

					if (dwBytesRead > 0) {
						actualForce[dwBytesRead] = '\0';
					}
					else {
						cout << "No data from COM port\nCheck the COM port in the program with your device manager\n";
					}
					/*
					res = cmtGetNextDataBundle(instance);
					if ((mode & CMT_OUTPUTMODE_CALIB) != 0)
						res = cmtDataGetCalData(instance, &caldata, deviceIds[0]);

					if ((mode & CMT_OUTPUTMODE_ORIENT) != 0) {
						res = cmtDataGetOriEuler(instance, &euler_data, deviceIds[0]);
						res = cmtDataGetOriMatrix(instance, &matrix_data, deviceIds[0], NULL);
					}*/





					//collector.log(kukaForces, to_string(estimatedTorque));
					mousedata = Mouse.Run();
					//std::cout << "\r vx:" << mousedata.vx << "  vy: " << mousedata.vy << "                      ";
					//std::cout << "\rroll:  " << euler_data.m_roll << "     " << euler_data.m_pitch << "     " << euler_data.m_yaw << "     ";
/*
					rot << matrix_data.m_data[0][0], matrix_data.m_data[0][1], matrix_data.m_data[0][2],
						matrix_data.m_data[1][0], matrix_data.m_data[1][1], matrix_data.m_data[1][2],
						matrix_data.m_data[2][0], matrix_data.m_data[2][1], matrix_data.m_data[2][2];

					Eigen::MatrixXd t_rot = rot.array().transpose();
					rotuniv << 0, 1, 0,
						-1, 0, 0,
						0, 0, 1;//rotation matrix of imy sensor wrt robot base

					accvector << caldata.m_acc.m_data[0], caldata.m_acc.m_data[1], caldata.m_acc.m_data[2];
					accvectorUniv = t_rot * accvector;//acceleration vector in universal coordinate system. note that z element is -9.81 
													  //	matrix_data.m_data[1][0]<< matrix_data.m_data[1][1]<< matrix_data.m_data[1][2]<<
													  //	matrix_data.m_data[2][0], matrix_data.m_data[2][1], matrix_data.m_data[2][2];
													  //std::cout << "    " << to_string(accvectorUniv(0)*1000)<< "        ";
					*/								 

					
					//string kukaForcesViaUDP = recieveViaUDP();
					command = to_string(0) + "#" + to_string(0) + "#" + to_string(-estimatedTorque) + "#" + to_string(0) + "#" + to_string(0) + "#" + to_string(0) + "#" + errormsg;//
					sendViaUDP(command);
					
					collector.log( to_string(-estimatedTorque), actualForce);
					Sleep(3);//emulate some computational effort
					std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
					deltaT = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

					std::cout << "\r  EMG Force(N):  " << to_string(estimatedTorque) << "   Actuel Force: " << (actualForce) << "                      " ;

				}
	    		
				while (key == 'l')// this code is for Final test no end-effector change in pose. records weigh scale value and estimated force. using LSTM network
				{
					keyState = GetKeyState(VK_ESCAPE);
					bool isDown = keyState & 0x8000;
					if (isDown) {// check if escape key has pressed
						rec = false;
						key = 'esc';
					}

					std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
					string s = "s";

					strcpy(actualForce, s.c_str());
					int toWrite = strlen(actualForce);
					if (!WriteFile(hSerial, actualForce, toWrite, &dwBytesRead, NULL)) {//send command to arduino board to get actual force
						//cout << "send error to Arduino error";
					}

					if (myo1)//if armband is available
					{
						hub.run(5);


						filteredEMG = butterWorthFilt(static_cast<int> (collector.emgSamples[pickedChannels[0]]),static_cast<int>(collector.emgSamples[pickedChannels[1]]),static_cast<int>(collector.emgSamples[pickedChannels[2]]));//filteremg data
						//netInputs[0] = static_cast<double>(filteredEMG[0]);
						//netInputs[1] = static_cast<double>(filteredEMG[1]);
						//netInputs[2] = static_cast<double>(filteredEMG[2]);
						// y = (ymax - ymin)*(x - xmin) / (xmax - xmin) + ymin;
						netInputs[0] = static_cast<double>((ymax - ymin)*(static_cast<float>(filteredEMG[0]) - xmin[0]) / (xmax[0] - xmin[0]) +ymin);
						netInputs[1] = static_cast<double>( (ymax - ymin)*(static_cast<float>(filteredEMG[1]) - xmin[1]) / (xmax[1] - xmin[1]) + ymin);
						netInputs[2] = static_cast<double>( (ymax - ymin)*(static_cast<float>(filteredEMG[2]) - xmin[2]) / (xmax[2] - xmin[2]) + ymin);
						//** Normalization moved to butterwoth filter function.
						//netInputs[0] = (static_cast<double>(filteredEMG[0])- input_mean[0])/input_std[0];
						//netInputs[1] = (static_cast<double>(filteredEMG[1])- input_mean[1])/input_std[1];
						//netInputs[2] = (static_cast<double>(filteredEMG[2])- input_mean[2])/input_std[2];
						//netInputs[0] = static_cast<double>(filteredEMG[0]);
						//netInputs[1] = static_cast<double>(filteredEMG[1]);
						//netInputs[2] = static_cast<double>(filteredEMG[2]);

						estimatedTorque = -LSTMnet_DatasetF(netInputs) - 0.162;

						if (estimatedTorque >= 0 )
						   estimatedTorque = (MAXFORCE - 0) * (estimatedTorque - 0) / (maxtorque - 0) + 0;
						else 
							estimatedTorque = (0 - (MINFORCE)) * (estimatedTorque - mintorque) / (0 - mintorque) + MINFORCE;
						//estimatedTorque = (MAXFORCE - MINFORCE) * (estimatedTorque - (-1.0)) / (1.0 - (-1.00)) + MINFORCE;

						//estimatedTorque = (MAXFORCE - MINFORCE) * (estimatedTorque - (0.062)) / (0.25 - (0.062)) + MINFORCE;
					}
					else {
						estimatedTorque = 0;
						Sleep(5);
					}
					actualForce[0] = '\0';

					if (!ReadFile(hSerial, actualForce, n, &dwBytesRead, NULL)) {///receive data from arduino
						//cout << "Force receive error from scale";
					}

					if (dwBytesRead > 0) {
						actualForce[dwBytesRead] = '\0';
					}
					else {
						//cout << "No data from COM port\nCheck the COM port in the program with your device manager\n";
					}
					/*
					res = cmtGetNextDataBundle(instance);
					if ((mode & CMT_OUTPUTMODE_CALIB) != 0)
						res = cmtDataGetCalData(instance, &caldata, deviceIds[0]);

					if ((mode & CMT_OUTPUTMODE_ORIENT) != 0) {
						res = cmtDataGetOriEuler(instance, &euler_data, deviceIds[0]);
						res = cmtDataGetOriMatrix(instance, &matrix_data, deviceIds[0], NULL);
					}*/





					//collector.log(kukaForces, to_string(estimatedTorque));
					mousedata = Mouse.Run();
					//std::cout << "\r vx:" << mousedata.vx << "  vy: " << mousedata.vy << "                      ";
					//std::cout << "\rroll:  " << euler_data.m_roll << "     " << euler_data.m_pitch << "     " << euler_data.m_yaw << "     ";
					/*
					rot << matrix_data.m_data[0][0], matrix_data.m_data[0][1], matrix_data.m_data[0][2],
						matrix_data.m_data[1][0], matrix_data.m_data[1][1], matrix_data.m_data[1][2],
						matrix_data.m_data[2][0], matrix_data.m_data[2][1], matrix_data.m_data[2][2];

					Eigen::MatrixXd t_rot = rot.array().transpose();
					rotuniv << 0, 1, 0,
						-1, 0, 0,
						0, 0, 1;//rotation matrix of imy sensor wrt robot base

					accvector << caldata.m_acc.m_data[0], caldata.m_acc.m_data[1], caldata.m_acc.m_data[2];
					accvectorUniv = t_rot * accvector;//acceleration vector in universal coordinate system. note that z element is -9.81 
													  //	matrix_data.m_data[1][0]<< matrix_data.m_data[1][1]<< matrix_data.m_data[1][2]<<
													  //	matrix_data.m_data[2][0], matrix_data.m_data[2][1], matrix_data.m_data[2][2];
													  //std::cout << "    " << to_string(accvectorUniv(0)*1000)<< "        ";

					*/

													  //string kukaForcesViaUDP = recieveViaUDP();
					command = to_string(0) + "#" + to_string(0) + "#" + to_string(-estimatedTorque) + "#" + to_string(0) + "#" + to_string(0) + "#" + to_string(0) + "#" + errormsg;//
					sendViaUDP(command);

					collector.log(to_string(-estimatedTorque), actualForce);
					Sleep(3);//emulate some computational effort
					std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
					deltaT = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

					std::cout << "\r  EMG Force(N):  " << to_string(estimatedTorque) << "   Actuel Force: " << actualForce << "                      ";

				}
				while (key == 'u')// this code is for Final setup With end-effector change in pose. no weight scale.estimated force using LSTM network.
				{
					keyState = GetKeyState(VK_ESCAPE);
					bool isDown = keyState & 0x8000;
					if (isDown) {// check if escape key has pressed
						rec = false;
						key = 'esc';
					}

					std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();


					if (myo1)//if armband is available
					{
						hub.run(5);

						filteredEMG = butterWorthFilt(static_cast<int> (collector.emgSamples[pickedChannels[0]]),
													static_cast<int>(collector.emgSamples[pickedChannels[1]]),
													static_cast<int>(collector.emgSamples[pickedChannels[2]]));//filteremg data
										// y = (ymax - ymin)*(x - xmin) / (xmax - xmin) + ymin;
						netInputs[0] = static_cast<double>((ymax - ymin)*(static_cast<float>(filteredEMG[0]) - xmin[0]) / (xmax[0] - xmin[0]) + ymin);
						netInputs[1] = static_cast<double>((ymax - ymin)*(static_cast<float>(filteredEMG[1]) - xmin[1]) / (xmax[1] - xmin[1]) + ymin);
						netInputs[2] = static_cast<double>((ymax - ymin)*(static_cast<float>(filteredEMG[2]) - xmin[2]) / (xmax[2] - xmin[2]) + ymin);

						estimatedTorque = LSTMnet_DatasetF(netInputs) - 0.066;

						if (estimatedTorque >= 0)
							estimatedTorque = (MAXFORCE - 0) * (estimatedTorque - 0) / (maxtorque - 0) + 0;
						else
							estimatedTorque = (0 - (MINFORCE)) * (estimatedTorque - mintorque) / (0 - mintorque) + MINFORCE;
						//estimatedTorque = (MAXFORCE - MINFORCE) * (estimatedTorque - (-0.15)) / (0.2 - (-0.15)) + MINFORCE;
					}
					else {
						estimatedTorque = 0;
						Sleep(3);
					}
					actualForce[0] = '\0';
					/*
					res = cmtGetNextDataBundle(instance);
					if ((mode & CMT_OUTPUTMODE_CALIB) != 0)
						res = cmtDataGetCalData(instance, &caldata, deviceIds[0]);

					if ((mode & CMT_OUTPUTMODE_ORIENT) != 0) {
						res = cmtDataGetOriEuler(instance, &euler_data, deviceIds[0]);
						res = cmtDataGetOriMatrix(instance, &matrix_data, deviceIds[0], NULL);
					}*/


					//collector.log(kukaForces, to_string(estimatedTorque));
					mousedata = Mouse.Run();
					//std::cout << "\r vx:" << mousedata.vx << "  vy: " << mousedata.vy << "                      ";
					//std::cout << "\rroll:  " << euler_data.m_roll << "     " << euler_data.m_pitch << "     " << euler_data.m_yaw << "     ";
					/*
					rot << matrix_data.m_data[0][0], matrix_data.m_data[0][1], matrix_data.m_data[0][2],
						  matrix_data.m_data[1][0], matrix_data.m_data[1][1], matrix_data.m_data[1][2],
						  matrix_data.m_data[2][0], matrix_data.m_data[2][1], matrix_data.m_data[2][2];

					Eigen::MatrixXd t_rot = rot.array().transpose();
					rotuniv << 0, 1, 0,
						-1, 0, 0,
						0, 0, 1;//rotation matrix of imy sensor wrt robot base

					accvector << caldata.m_acc.m_data[0], caldata.m_acc.m_data[1], caldata.m_acc.m_data[2];
					accvectorUniv = t_rot * accvector;//acceleration vector in universal coordinate system. note that z element is -9.81 
													  //	matrix_data.m_data[1][0]<< matrix_data.m_data[1][1]<< matrix_data.m_data[1][2]<<
													  //	matrix_data.m_data[2][0], matrix_data.m_data[2][1], matrix_data.m_data[2][2];
													  //std::cout << "    " << to_string(accvectorUniv(0)*1000)<< "        ";
					*/



					//string kukaForcesViaUDP = recieveViaUDP();\
					//command = to_string(mousedata.vx) + "#" + to_string(mousedata.vy*-1) + "#" + to_string(estimatedTorque) + "#" + to_string(-euler_data.m_pitch) + "#" + to_string(-euler_data.m_roll) + "#" + to_string(euler_data.m_yaw) + "#" + errormsg;//

					//uncomment command = to_string(mousedata.vx) + "#" + to_string(mousedata.vy*-1) + "#" + to_string(estimatedTorque) + "#" + to_string(-euler_data.m_pitch) + "#" + to_string(-euler_data.m_roll) + "#" + to_string(euler_data.m_yaw) + "#" + errormsg;//
					sendViaUDP(command);
					collector.log(to_string(estimatedTorque), actualForce);
					std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
					deltaT = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
					std::cout << "\r  EMG Force(N):  " << to_string(estimatedTorque) << "   Actuel Force: " << mousedata.vx << "                      ";

				}
			}
			Mouse.UnHookMouse();//**important call this method after calling Mouse.Run() or U must call Run method consistently in a loop or the mouse wont work even in other programs

			
			keyState = GetKeyState(VK_ESCAPE);
			bool isDown = keyState & 0x8000;
			if (isDown) {// check if escape key has pressed
				rec = false;
			}

			
		}
		LSTMnet_DatasetF_terminate();
		sendViaUDP("STOP");
		closesocket(socketSend);
		closesocket(socketRec);
	
	
    return 0;
}

//////////////////////////////////////////////////////////////////////////
// doHardwareScan
//
// Checks available COM ports and scans for MotionTrackers
void doHardwareScan()
{
	XsensResultValue res;
	CmtPortInfo portInfo[256];
	uint32_t portCount = 0;

	printf("Scanning for connected Xsens devices...");
	res = cmtScanPorts(portInfo, &portCount, 0);
	EXIT_ON_ERROR(res, "cmtScanPorts");
	printf("done\n");

	if (portCount == 0) {
		printf("No MotionTrackers found\n\n");
		//exit(0);
	}
	else {
		for (int i = 0; i < (int)portCount; i++) {
			printf("Using COM port %d at %d baud\n\n",
				(long)portInfo[i].m_portNr, portInfo[i].m_baudrate);
		}

		printf("Opening ports...");
		//open the port which the device is connected to and connect at the device's baudrate.
		for (int p = 0; p < (int)portCount; p++) {
			res = cmtOpenPort(instance, portInfo[p].m_portNr, portInfo[p].m_baudrate);
			EXIT_ON_ERROR(res, "cmtOpenPort");
		}
		printf("done\n\n");

		//get the Mt sensor count.
		printf("Retrieving MotionTracker count (excluding attached Xbus Master(s))\n");
		res = cmtGetMtCount(instance, &mtCount);
		EXIT_ON_ERROR(res, "cmtGetMtCount");
		printf("MotionTracker count: %i\n\n", mtCount);

		// retrieve the device IDs 
		printf("Retrieving MotionTrackers device ID(s)\n");
		for (unsigned int j = 0; j < mtCount; j++) {
			res = cmtGetMtDeviceId(instance, &deviceIds[j], j);
			EXIT_ON_ERROR(res, "cmtGetDeviceId");
			printf("Device ID at index %i: %08x\n", j, (long)deviceIds[j]);
		}

		// make sure that we get the freshest data
		printf("\nSetting queue mode so that we always get the latest data\n\n");
		res = cmtSetQueueMode(instance, CMT_QM_LAST);
		EXIT_ON_ERROR(res, "cmtSetQueueMode");
	}
	
}

//////////////////////////////////////////////////////////////////////////
// doMTSettings
//
// Set user settings in MTi/MTx
// Assumes initialized global MTComm class
void doMtSettings(void)
{
	XsensResultValue res;

	// set sensor to config sate
	res = cmtGotoConfig(instance);
	EXIT_ON_ERROR(res, "cmtGotoConfig");

	unsigned short sampleFreq;
	res = cmtGetSampleFrequency(instance, &sampleFreq, deviceIds[0]);

	// set the device output mode for the device(s)
	printf("Configuring your mode selection");
	for (int i = 0; i < mtCount; i++) {
		if (cmtIdIsMtig(deviceIds[i])) {
			res = cmtSetDeviceMode(instance, mode, settings, sampleFreq, deviceIds[i]);
		}
		else {
			res = cmtSetDeviceMode(instance, mode & 0xFF0F, settings, sampleFreq, deviceIds[i]);
		}
		EXIT_ON_ERROR(res, "setDeviceMode");
	}

	// start receiving data
	res = cmtGotoMeasurement(instance);
	EXIT_ON_ERROR(res, "cmtGotoMeasurement");
}
//////////////////////////////////////////////////////////////////////////
// exitFunc
//
// Closes cmt nicely
void exitFunc(void)
{
	// if we have a valid instance, we should get rid of it at the end of the program
	if (instance != -1) {
		// Close any open COM ports
		cmtClose(instance);
		cmtDestroyInstance(instance);
	}

	// get rid of keystrokes before we post our message
	while (_kbhit()) _getch();

	// wait for a keypress
	if (!userQuit)
	{
		//printf("Press a key to exit\n");
		//_getch();
	}
}
