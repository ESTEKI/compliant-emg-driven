// testLSTMcodegen1.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <iostream>
#include "LSTMnet.h"
#include "LSTMnet_terminate.h"

double dv[3];
float y;

static void main_LSTMnet()
{
	double dv[3];
	float y;
	// Initialize function 'LSTMnet' input arguments.
	// Initialize function input argument 'x'.
	// Call the entry-point 'LSTMnet'.
	for (int ii = 0; ii < 10; ii++)
	{
		std::cout << "Enter all 3 inputs to network consequently:\n\r";
		std::cin >> dv[0]; std::cin >> dv[1]; std::cin >> dv[2];
		y = LSTMnet(dv);
		std::cout << "y= " << y << "\n\r";
	}
	
}


int main()
{
	main_LSTMnet();
	LSTMnet_terminate();
    return 0;
}

