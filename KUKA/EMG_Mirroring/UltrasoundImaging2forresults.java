package EMG_Mirroring;


import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DecimalFormat;

import javax.inject.Inject;
import javax.inject.Named;

import com.kuka.common.ThreadUtil;
import com.kuka.connectivity.motionModel.directServo.DirectServo;
import com.kuka.connectivity.motionModel.directServo.IDirectServoRuntime;
import com.kuka.connectivity.motionModel.smartServo.ServoMotion;
import com.kuka.generated.ioAccess.MediaFlangeIOGroup;
import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;
import static com.kuka.roboticsAPI.motionModel.BasicMotions.*;

import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.geometricModel.CartDOF;
import com.kuka.roboticsAPI.geometricModel.Frame;
import com.kuka.roboticsAPI.geometricModel.Tool;
import com.kuka.roboticsAPI.geometricModel.World;
import com.kuka.roboticsAPI.motionModel.controlModeModel.CartesianImpedanceControlMode;
import com.kuka.roboticsAPI.sensorModel.ForceSensorData;

/**
 * Author :Siavash Esteki [[esteki.siavash@yahoo.com]]
 * This program enables force control in Impedance mode, 
 * while being compliant to external forces.
 * Robot excerts desired force along Z axis w.r.t world frame. 
 * In xy plane it is only compliant position controlled.
 * 
 * Desired force and desired pose of tool will be riceived via UDP connection from
 * another computer. EMG sensors worn by human operator is used to estimate elbow torque by artificial neural network.
 * position of tool and its orientation will be measured respectively from mouse and IMU sensor.
 * Run cppinterface1.cpp in order to send commands to robot 
 * (look for the app in folder 'appv1') 
 * 
 * @see UseRoboticsAPIContext
 * @see #initialize()
 * @see #run()
 * @see #dispose()
 */
public class UltrasoundImaging2forresults extends RoboticsAPIApplication {
	@Inject
	private LBR lBR_iiwa_14_R820_1;
	private final String otherMachineIP = "172.31.1.150";// windows 10 pc 
    private UDPConnection udp = new UDPConnection(30002, 50004, otherMachineIP,800) ;  
    double subjectForearmLen = 340.0;// in mm
    
    
	private static final int stiffnessZ = 500;
	private static final int stiffnessXY = 500;
	private LBR _lbr;
	double initialPosition[] = {20,550, 350};
	double initOrientation[] = {179,0,180};
	double desiredCartesianPose[] ={0,0,0};
	
	@Inject
	@Named("Cylinder")
	private Tool tool;//page 371 pdf
	
	private String command; 
    
	@Inject
	 private MediaFlangeIOGroup mediaflange;
	File myLogfile = new File("UltrasoundImaging2resultsFFNN3azar1.txt");//**this file will be saved in \\172.31.1.147\krc\ApplicationServer
														//User : kukauser
														//pass : 68kuka1secpw59
	FileWriter myWriter;
	
	
	@Override
	public void initialize() {
		_lbr = getContext().getDeviceFromType(LBR.class);
		tool.attachTo(_lbr.getFlange());
		
		try
		{
			myLogfile.createNewFile();
			myWriter = new FileWriter("UltrasoundImaging2resultsFFNN3azar1.txt");
		    myWriter.write("fx fy fz tx ty tz \n\r");
		    //myWriter.close();//**remember to close the file at the end of program
		}
		catch (IOException e) {
			getLogger().error(e.toString());
		    }
		udp.setBufferSize(128);
	}
	
	@Override
	public void dispose(){
		try{
			udp.closeUDP(); //Close the UDP socket and free it's port 
		myWriter.close();
		}
		catch (IOException err){
			
		}
		
	}
	
	public Frame moveToInitialPosition()
    {
		
    Frame desiredFrame0  = _lbr.getFlange().copyWithRedundancy();
    desiredFrame0.setX(initialPosition[0]);
    desiredFrame0.setY(initialPosition[1]);
    desiredFrame0.setZ(initialPosition[2]);
    
    desiredFrame0.setAlphaRad(initOrientation[0]*3.14/180);
    desiredFrame0.setBetaRad(initOrientation[1]*3.14/180);
    desiredFrame0.setGammaRad(initOrientation[2]*3.14/180);
    
    getLogger().info(tool.getDefaultMotionFrame().toString()); 
    
    	try{
    		tool.move(ptp(desiredFrame0).setJointVelocityRel(0.2));
    	}
    	catch (Exception e) {
    		getLogger().error(e.getMessage());  
		}
    	return desiredFrame0;
    	
    }

	@SuppressWarnings("deprecation")
	@Override
	public void run() {
		// your application execution starts here
		mediaflange.setLEDBlue(false);
		DecimalFormat df = new DecimalFormat("0.000000");
		double emgForce = 0;
		Frame initialPoseOfTool=moveToInitialPosition();
		///-----------Impedance Controller	
				CartesianImpedanceControlMode cartImpCtrlMode;
		        cartImpCtrlMode = new CartesianImpedanceControlMode();
		        
		        cartImpCtrlMode.parametrize(CartDOF.X,
		        CartDOF.Y).setStiffness(stiffnessXY);
		        cartImpCtrlMode.parametrize(CartDOF.Z).setStiffness(stiffnessZ);
		        cartImpCtrlMode.parametrize(CartDOF.ROT).setStiffness(300.0);
		        cartImpCtrlMode.parametrize(CartDOF.ALL).setDamping(0.2);
		//----------end
		///-------Direct Servo Motion
		        DirectServo aDirectServoMotion = new DirectServo(
		                _lbr.getCurrentJointPosition());
		        aDirectServoMotion.setMinimumTrajectoryExecutionTime(40e-3);
		        ServoMotion.validateForImpedanceMode(tool);
		        aDirectServoMotion.setJointVelocityRel(0.4);
		        //aDirectServoMotion
		        getLogger().info("Starting DirectServo motion in position control mode");
		        tool.getDefaultMotionFrame().moveAsync(aDirectServoMotion.setMode(cartImpCtrlMode));//here binds move method to our SmartServo motion
		        getLogger().info("Get the runtime of the DirectServo motion");
		        IDirectServoRuntime theServoRuntime = aDirectServoMotion
		                .getRuntime();
		        
		///-------
		//measuring bias force in kuka sensor
		ForceSensorData initialExtForceTorques = _lbr.getExternalForceTorque(_lbr.getFlange(),World.Current.getRootFrame());
		
		getLogger().info("Bias forces: " + initialExtForceTorques.toString());
		
		Frame desiredFrame =_lbr.getFlange().copyWithRedundancy();// initialPoseOfTool.copyWithRedundancy();
        
		desiredFrame.setX(initialPoseOfTool.getX());
        desiredFrame.setY(initialPoseOfTool.getY());
        desiredFrame.setZ(initialPoseOfTool.getZ());
        desiredFrame.setAlphaRad(initOrientation[0]*3.14/180);
        desiredFrame.setBetaRad(initOrientation[1]*3.14/180);
        desiredFrame.setGammaRad(initOrientation[2]*3.14/180);
        
        command = udp.readRawDatastr();
        getLogger().info("Waiting for Start command...");  
        while (command == ""){
        	command = udp.readRawDatastr();
        }
        mediaflange.setLEDBlue(true);
        getLogger().info(String.valueOf(command));
        getLogger().info("Loop Started...");
        desiredCartesianPose[0] =  initialPoseOfTool.getX();
        desiredCartesianPose[1] =  initialPoseOfTool.getY();
        desiredCartesianPose[2] =  initialPoseOfTool.getZ();
        //--parameters used in detection and reaction to human guidance in Z direction
        double hz = 0;// h in z direction
        double Xdot =0;
        double Xdoth =0;
        double xddoth = 0;
        double previusXdot =0;
        double xddot =0;
        double Ez=0;
        final double D_va_z = 10; //best 10 
        final double M_va_z= 2;	// best 2
        final double E_t_z = 0.04;//threshold
        final double E_m_z = 3.5;// maximum energy in tank
        final double P_d_z = 0.45;//power disipation rate //best 0.5
        double Ftilda = 0;
        double Fimp = 0;
        double previusPositionz = initialPoseOfTool.getZ()/1000;//position in previus timestep
        double currentPositionZ =0;
        double desiredForceZ=0;
        //--------
        //--parameters for x direction 
        double hx = 0;
        final double D_va_x = 2; //best 10 
        final double M_va_x = 1;	// best 2
        final double E_t_x = 0.4;//threshold
        final double E_m_x = 4;// maximum energy in tank
        final double P_d_x = 0.40;//power disipation rate //best 0.5
        double Fhx=0;
        double currentPositionx =0;
        double Ex=0; //energy tank

        double x1[] ={0,0}; // x1[0] is previus x1 and x[1] is current x1
        double x2[] = {0,0};
        double previusPositionx = initialPoseOfTool.getX();
        //--------
      //--parameters for y direction 
        double hy = 0;
        final double D_va_y = 2; //best 10 
        final double M_va_y = 1;	// best 2
        final double E_t_y = 0.4;//threshold
        final double E_m_y = 4;// maximum energy in tank
        final double P_d_y = 0.40;//power disipation rate //best 0.5
        double Ey=0; //energy tank

        double Fhy=0;
        double currentPositiony =0;

        double y1[] ={0,0}; // x1[0] is previus x1 and x[1] is current x1
        double y2[] = {0,0};
        double previusPositiony = initialPoseOfTool.getY();
        //--------
        
        double k1,k2,k3,k4;//Rung-Kuta parameters
        double l1,l2,l3,l4;//Rung-Kuta parameters
        double Po=0;//output power
        double Pi =0;// input power.
        double Edot;
        double dt = 0.015;
    	//---recieved parameters over UDP
        double cmdDeltaX =0;
        double cmdDeltaY =0;
        double rollangle =0; 
      	double pitchangle=0;
      	double yawangle =0;
        String cmdMSG; 
        //-------------
        long startLoopTime = System.currentTimeMillis() ;
    	long currentLoopTime = 0;
    	long totalLoopTime = 0;
    	long iterationStartTime=0;
    	
    	double xTask = initialPoseOfTool.getX();
        double yTask =  initialPoseOfTool.getY();
        
        char transferData=0;
        
    	while (command != "STOP" && totalLoopTime < 10 )//everytime that a data is recieved over udp total loop time will be 0
        {
    		iterationStartTime = System.currentTimeMillis();
    		command = udp.readRawDatastr();
        	String[] cmdvalues = command.split("#");
        	/*
        	 * cmdvalues[0] : x cartesian
        	 * cmdvalues[1] : y cartesian
        	 * cmdvalues[2] : torque
        	 * cmdvalues[3] :roll
        	 * cmdvalues[4] : pitch
        	 * cmdvalues[5] : yaw
        	 * cmdvalues[6] : errmsg
        	 */
        	if (cmdvalues.length == 7)//only when data is received over udp, emgForce will get  updated
        	{
        		 transferData = 1;
        		startLoopTime = System.currentTimeMillis() ;
        		emgForce = Double.parseDouble(cmdvalues[2]);
        		cmdDeltaX =Math.signum(Double.parseDouble(cmdvalues[0]))*0.5; //Double.parseDouble(cmdvalues[0])*0.1;//
        		cmdDeltaY = Math.signum(  Double.parseDouble(cmdvalues[1]))*0.5;//Double.parseDouble(cmdvalues[1])*0.1;//
        		 rollangle =Double.parseDouble(cmdvalues[3]); 
              	 pitchangle = Double.parseDouble(cmdvalues[4]);
              	 yawangle = Double.parseDouble(cmdvalues[5]);
              	 
              	 if ( Math.abs(cmdDeltaX) > 10)
              	 {
              		cmdDeltaX = Math.signum(cmdDeltaX) *  10;
              	 }
              	 
              	 if ( Math.abs(cmdDeltaY) > 10)
              	 {
              		cmdDeltaY = Math.signum(cmdDeltaY) *  10;
              	 }
              	 
        	}
        	else{
        		cmdDeltaX =  0;
        		cmdDeltaY =  0;
        	}
           	    theServoRuntime.updateWithRealtimeSystem();
                Frame currentCartesanPosition =  theServoRuntime.getCurrentCartesianPosition(tool.getDefaultMotionFrame());//we can have TCP coordinate using only this method
                ForceSensorData currentExtForceTorques = _lbr.getExternalForceTorque(_lbr.getFlange(),World.Current.getRootFrame());
                
                
                //getLogger().error(command);
              
      ////////////////////x direction compliance ctrl
                
                currentPositionx = currentCartesanPosition.getX();
                double Fe_x = currentExtForceTorques.getForce().getX();
               
                k1 = dt*x2[0];
                l1 = dt* (-D_va_x*x1[0]/M_va_x+Fe_x/M_va_x );
                k2 = dt*(x2[0]+l1/2);
                l2 = dt*((-D_va_x*(x1[0]+k1/2))/M_va_x+Fe_x/M_va_x );
                k3 = dt*(x2[0]+l2/2);
                l3 = dt* ((-D_va_x*(x1[0]+k2/2))/M_va_x+Fe_x/M_va_x );
                k4 = dt*(x2[0]+l3);
                l4 = dt* ((-D_va_x*(x1[0]+k3))/M_va_x+Fe_x/M_va_x);
                
                x1[1] = (x1[0] + (1.0/6.0)*(k1+2*k2+2*k3+k4));
                x2[1] = x2[0] + (1.0/6.0)*(l1+2*l2+2*l3+l4); 
                
                double xtildadot = x2[1];
                x1[0] = x1[1] ;
                x2[0] = x2[1];
                
                Pi = xtildadot/1000*Fe_x;
                Po = xtildadot/1000*Fhx;
                
                Edot = Pi - Po -(1-hx)*P_d_x;
                
                Ex = Ex + Edot;
                
         	   if (Ex<0)
         	   {
         		  Ex = 0;
         	   }
         	   else if (Ex>E_m_x)
         	   {
         		   Ex= E_m_x;
         	   }
         	   
         	   if (Ex<= E_t_x)
         	   {
         		   hx =0;
         	   }
         	   else 
         	   {
         		   hx = (Ex-E_t_x)/(E_m_x-E_t_x);
         	   }
         	   
         	   if (hx<0)
         	   {
         		   hx=0;
         	   }
         	   
         	   Fhx = Fe_x*hx;
         	   
         	   
         	    xTask = xTask + cmdDeltaX;//would be the x coordinate of mouse in final application
         	    if(xTask > 170){
         	    	xTask = 170;
         	    }
         	    else if (xTask<-175){
         	    	xTask=-175;
         	    }
         	   double xdesired;
         		   xdesired = xTask + (currentPositionx - xTask)*hx ;
         		  if(Math.abs(previusPositionx - xTask) > Math.abs(currentPositionx - xTask) && Math.abs(currentPositionx - xTask) > 10 )// && aDirectServoMotion.getJointVelocityRel() > 0.1)// meaning robot is moving towards xTask
	   		  	   {//this keeps the speed of robot natural when going back to its desired position comanded bu operator
	   		  		   //setJointVelocityRel did not work!
	   		  		//aDirectServoMotion.setJointVelocityRel(0.1);
	   		  	   // mediaflange.setLEDBlue(false);
	   		  		//xdesired =  -1*Math.signum(currentPositionx - xdesired) *18 + currentPositionx; 
	   		  	   }
	   		  	   else 
	   		  	   {
	   		  		   //aDirectServoMotion.setJointVelocityRel(0.4);
	   		  		   //mediaflange.setLEDBlue(true);
	   		  	   }
	   	      	 previusPositionx = currentPositionx;
         	   desiredFrame.setX(xdesired) ;
         	   ////////////////////////////////////////////end x

       /////////////////////////////////////                 Y direction 
          currentPositiony = currentCartesanPosition.getY();
          double Fe_y = currentExtForceTorques.getForce().getY();
         
          k1 = dt*y2[0];
          l1 = dt* (-D_va_y*y1[0]/M_va_y+Fe_y/M_va_y );
          k2 = dt*(y2[0]+l1/2);
          l2 = dt*((-D_va_y*(y1[0]+k1/2))/M_va_y+Fe_y/M_va_y );
          k3 = dt*(y2[0]+l2/2);
          l3 = dt* ((-D_va_y*(y1[0]+k2/2))/M_va_y+Fe_y/M_va_y );
          k4 = dt*(y2[0]+l3);
          l4 = dt* ((-D_va_y*(y1[0]+k3))/M_va_y+Fe_y/M_va_y);
          
          y1[1] = (y1[0] + (1.0/6.0)*(k1+2*k2+2*k3+k4));
          y2[1] = y2[0] + (1.0/6.0)*(l1+2*l2+2*l3+l4); 
          
          double ytildadot = y2[1];
          y1[0] = y1[1] ;
          y2[0] = y2[1];
          
          Pi = ytildadot/1000*Fe_y;
          Po = ytildadot/1000*Fhy;
          
          Edot = Pi - Po -(1-hy)*P_d_y;
          
          Ey = Ey + Edot;
          
      	   if (Ey<0)
      	   {
      		  Ey = 0;
      	   }
      	   else if (Ey>E_m_y)
      	   {
      		   Ey= E_m_y;
      	   }
      	   
      	   if (Ey<= E_t_y)
      	   {
      		   hy =0;
      	   }
      	   else 
      	   {
      		   hy = (Ey-E_t_y)/(E_m_y-E_t_y);
      	   }
      	   
      	   if (hy<0)
      	   {
      		   hy=0;
      	   }
      	   
      	   Fhy = Fe_y*hy;
      	   
      	    yTask = yTask +cmdDeltaY;//would be the x coordinate of mouse
      	  if(yTask > 710){
      		yTask = 710;
	   	    }
	   	    else if (yTask<400){
	   	    	yTask=400;
	   	    }
      	   double ydesired = yTask + (currentPositiony - yTask)*hy ; 
      	   
	      	 if(Math.abs(previusPositiony - yTask) > Math.abs(currentPositiony - yTask) && Math.abs(currentPositiony - yTask) > 10 )// && aDirectServoMotion.getJointVelocityRel() > 0.1)// meaning robot is moving towards xTask
		  	   {//this keeps the speed of robot natural
		  		   //setJointVelocityRel did not work!
		  		
		  	    //mediaflange.setLEDBlue(false);
		  		//ydesired =  -1*Math.signum(currentPositiony - ydesired) *18 + currentPositiony; 
		  	   }
		  	   else 
		  	   {
		  		  // mediaflange.setLEDBlue(true);
		  	   }
	      	 previusPositiony = currentPositiony;
	      	 
	         	desiredFrame.setY(ydesired) ;
         	   
       ///////////////////////////////////////end y 
         	   
         	   //////////////////////////////////////////////////Z direction
           currentPositionZ = currentCartesanPosition.getZ()/1000;//meter
      	   Xdot = (currentPositionZ- previusPositionz)/dt;
      	   xddot = (Xdot - previusXdot)/dt;
      	   previusXdot = Xdot;
      	   previusPositionz = currentPositionZ;
      	   
      	   Ftilda = M_va_z*xddot + D_va_z*Xdot;

      	   if (Math.signum(Ftilda) == Math.signum(emgForce))
      	   {
      		  Ftilda=0;
      	   }
      	   
      	   Pi = Xdot*Ftilda;
      	   Po = Xdoth*Ftilda;
      	   
      	   Edot = Pi - Po -(1-hz)*P_d_z-0.0001;
      	   
      	   Ez = Ez + Edot;
      	   if (Ez<0)
      	   {
      		  Ez =0;
      	   }
      	   else if (Ez>E_m_z)
      	   {
      		   Ez= E_m_z;
      	   }
      	   
      	   if (Ez<= E_t_z)
      	   {
      		   hz =0;
      	   }
      	   else 
      	   {
      		   hz = (Ez-E_t_z)/(E_m_z-E_t_z);
      	   }
      	   
      	   if (hz<0)
      	   {
      		   hz=0;
      	   }
      	   
      	   Xdoth = hz* Xdot;
      	   xddoth = hz* xddot;
      	   
      	   Fimp = M_va_z*xddoth + D_va_z*Xdoth; //addmittance in paper
      	   
      //----------------control over Force with constant impedance    	   
      	  
      	   desiredForceZ = ((1-hz))*emgForce+Fimp ;
         	  	  
      	   	   if(Xdot < 0)//causes a hard collision if we keep velocity high
      	   {
      		   aDirectServoMotion.setJointVelocityRel(0.1);
      	   }
      	   else{
      		   aDirectServoMotion.setJointVelocityRel(0.4);
      	   }
      	   
          desiredCartesianPose[1] = (desiredForceZ ) / stiffnessZ *1000 + currentCartesanPosition.getZ() ; //
          desiredFrame.setZ( desiredCartesianPose[1]);//   
         	   ////////////////////////////////////////////////////
        
      	
      	desiredFrame.setAlphaRad((initOrientation[0]+yawangle)*3.14/180);
     	desiredFrame.setBetaRad((-pitchangle+initOrientation[1])*3.14/180);
     	desiredFrame.setGammaRad( (rollangle+initOrientation[2])*3.14/180);
     	
         	try{
      			theServoRuntime.setDestination(desiredFrame); 			
      		}
      		catch (Exception e) {
      			getLogger().error("Out of reach:  " + String.valueOf(desiredFrame) );
      		}
	         	
         	if (transferData == 1)
         	{
         		ForceSensorData extForceTorquestr = _lbr.getExternalForceTorque(_lbr.getFlange(),World.Current.getRootFrame());
	    		//ForceSensorData extForceTorquestr2 = _lbr.getExternalForceTorque(_toolAttachedToLBR.getDefaultMotionFrame(),World.Current.getRootFrame());
	    		//getLogger().info(String.valueOf(extForceTorquestr));
	    		//getLogger().error(String.valueOf(extForceTorquestr2));
	    		String recordStr = 
	    		  String.valueOf( df.format(extForceTorquestr.getForce().getX()) ) + " " 
	    		+ String.valueOf( df.format(extForceTorquestr.getForce().getY()) ) + " " 
	    		+ String.valueOf( df.format(extForceTorquestr.getForce().getZ() - initialExtForceTorques.getForce().getZ()) ) + " " 
	    		+ String.valueOf( df.format(extForceTorquestr.getTorque().getX()) ) + " " 
	    		+ String.valueOf( df.format(extForceTorquestr.getTorque().getY()) ) + " " 
	    		+ String.valueOf( df.format(extForceTorquestr.getTorque().getZ()) ) + "\n\r";
	    		
	    		//startLoopTime = System.currentTimeMillis() ;
	    		try{
	    			myWriter.write(recordStr);
	    		}
	    		catch (Exception err)
	    		{
	    			getLogger().error(err.toString());
	    		}
	    		
	    		udp.writeDatastr(recordStr);
	    		transferData=0;
         	}
    	
      		 //ThreadUtil.milliSleep(2);
      		 currentLoopTime =  System.currentTimeMillis() ;
      		 dt = (double)(currentLoopTime-iterationStartTime)/1000;
     		 totalLoopTime = (currentLoopTime-startLoopTime)/1000;	
        	}//end while
    	 getLogger().info("END loop");
         mediaflange.setLEDBlue(false);
         ThreadUtil.milliSleep(100);
        }//end run
	
	}
