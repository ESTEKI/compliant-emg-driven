
//in barname dadehaye record shode EMG marboot be yek mefsal ra az lab pc migirad va dar joint4 piade mikonad
package EMG_Mirroring;


import java.util.concurrent.TimeUnit;

import javax.inject.Inject;

import com.kuka.common.ThreadUtil;
import com.kuka.connectivity.motionModel.smartServo.ISmartServoRuntime;
import com.kuka.connectivity.motionModel.smartServo.SmartServo;
import com.kuka.generated.ioAccess.MediaFlangeIOGroup;
import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;
import static com.kuka.roboticsAPI.motionModel.BasicMotions.*;

import com.kuka.roboticsAPI.deviceModel.JointPosition;
import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.geometricModel.LoadData;
import com.kuka.roboticsAPI.geometricModel.Tool;
import com.kuka.roboticsAPI.motionModel.PTP;
import com.kuka.roboticsAPI.sensorModel.DataRecorder;
import com.kuka.roboticsAPI.sensorModel.DataRecorder.AngleUnit;

/**
 * Implementation of a robot application.
 * <p>
 * The application provides a {@link RoboticsAPITask#initialize()} and a 
 * {@link RoboticsAPITask#run()} method, which will be called successively in 
 * the application lifecycle. The application will terminate automatically after 
 * the {@link RoboticsAPITask#run()} method has finished or after stopping the 
 * task. The {@link RoboticsAPITask#dispose()} method will be called, even if an 
 * exception is thrown during initialization or run. 
 * <p>
 * <b>It is imperative to call <code>super.dispose()</code> when overriding the 
 * {@link RoboticsAPITask#dispose()} method.</b> 
 * 
 * @see UseRoboticsAPIContext
 * @see #initialize()
 * @see #run()
 * @see #dispose()
 */
public class EMG1 extends RoboticsAPIApplication {
	@Inject
	private LBR lBR;
	private ISmartServoRuntime _theSmartServoRuntime = null;
	
	private final String otherMachineIP = "172.31.1.150";// windows 10 pc 
    private UDPConnection udp = new UDPConnection(30002, 40003, otherMachineIP,60) ;   
    
    private static final int MILLI_SLEEP_TO_EMULATE_COMPUTATIONAL_EFFORT = 30;

    private double jposition = 0.0;
    private MediaFlangeIOGroup mediaFlangeIO;// = new MediaFlangeIOGroup(controller);
    
	@Override
	public void initialize() {
		moveToStartPosition();
	}
	
	@Override
	public void dispose(){
		udp.closeUDP(); //Close the UDP socket and free it's port  
	}
	
	@Override
	public void run() {
		DataRecorder rec=new DataRecorder("EMG1.log", -1,	TimeUnit.SECONDS, 5);
	       
        rec.addCurrentJointPosition(lBR, AngleUnit.Degree);
        rec.addCommandedJointPosition(lBR, AngleUnit.Degree);
        //rec.addExternalJointTorque(lBR);
        //rec.addInternalJointTorque(lBR);
        rec.enable();
		
		JointPosition initialPosition = new JointPosition(
				lBR.getCurrentJointPosition());
		
        SmartServo aSmartServoMotion = new SmartServo(initialPosition);
        aSmartServoMotion.setJointAccelerationRel(0.6);
        aSmartServoMotion.setJointVelocityRel(0.6);

        aSmartServoMotion.setMinimumTrajectoryExecutionTime(20e-3);
        getLogger().info("Starting SmartServo motion in position control mode");
        lBR.moveAsync(aSmartServoMotion);
        
        getLogger().info("Get the runtime of the SmartServo motion");
        _theSmartServoRuntime = aSmartServoMotion.getRuntime();
        JointPosition destination = initialPosition;
        getLogger().info("Start loop");
        
        	
        	String inputData = udp.readRawDatastr();
        	long startLoopTime = System.currentTimeMillis() ;
        	long currentLoopTime = 0;
        	long totalLoopTime =0;
        
        	udp.writeDatastr("SENDME");
        rec.startRecording();	
        while (!inputData.equals("end") && totalLoopTime < 10) //if something goes wrong 
        {
        	if (!inputData.equals(""))
        	{
        		try{
        			jposition = Double.parseDouble(inputData);
        			//getLogger().info( String.valueOf(jposition));
        			startLoopTime = System.currentTimeMillis() ;
        			destination.set(3, Math.toRadians(jposition));
        			_theSmartServoRuntime.setDestination(destination);//this command updates the joint desired position
        			
        		/////to measure and record actual positions or vel...
                	_theSmartServoRuntime.updateWithRealtimeSystem();
                	// Get the measured position
                	JointPosition curMsrJntPose = _theSmartServoRuntime
                			.getAxisQMsrOnController();
                	double[] jointTorque =  _theSmartServoRuntime.getAxisTauExtMsr();
                	
                	getLogger().info( String.valueOf(jointTorque[3]));
                //end
        		}
        		catch (NumberFormatException ne){
        			 getLogger().info("end of file !!!");
        			 
        		}
        	}
        	currentLoopTime = System.currentTimeMillis();
        	totalLoopTime = (currentLoopTime - startLoopTime)/1000;
        	inputData = udp.readRawDatastr();
        }
        getLogger().info(String.valueOf(totalLoopTime));
        getLogger().info("End loop");
       
      rec.stopRecording();
        //ThreadUtil.milliSleep(1000);
	}
	
	public void moveToStartPosition(){
    	getLogger().info("Move to the mechanical zero position!");
		PTP ptpPosition = ptp(0,0,0,0,0,0,0);
		ptpPosition.setJointVelocityRel(0.3);
		//lBR.move(ptpPosition);
		//ThreadUtil.milliSleep(500);
		getLogger().info("Move to the Start position position!");
		ptpPosition = ptp(Math.toRadians(-90), Math.toRadians(-15), 0, Math.toRadians(105), 0, 0, 0);
		lBR.move(ptpPosition);
    }
}