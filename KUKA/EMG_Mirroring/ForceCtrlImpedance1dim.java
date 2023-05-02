package EMG_Mirroring;
/**
 * Program used to examine posibility of comliant control of robot in pure force control mode.
 * Robot will be run in Impedance mode and desired command is Force in Z direction
 * sorce:"Detection and reaction to human guidance" Aude Billard
 */
import java.text.DecimalFormat;
import java.util.concurrent.TimeUnit;
import java.lang.*;
import javax.inject.Inject;
import javax.inject.Named;

import com.kuka.common.ThreadUtil;
import com.kuka.connectivity.motionModel.directServo.DirectServo;
import com.kuka.connectivity.motionModel.smartServo.ISmartServoRuntime;
import com.kuka.connectivity.motionModel.smartServo.ServoMotion;
import com.kuka.connectivity.motionModel.smartServo.SmartServo;
import com.kuka.connectivity.motionModel.smartServoLIN.ISmartServoLINRuntime;
import com.kuka.connectivity.motionModel.smartServoLIN.SmartServoLIN;
import com.kuka.generated.ioAccess.MediaFlangeIOGroup;
import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;
import static com.kuka.roboticsAPI.motionModel.BasicMotions.*;

import com.kuka.roboticsAPI.deviceModel.JointPosition;
import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.geometricModel.AbstractFrame;
import com.kuka.roboticsAPI.geometricModel.CartDOF;
import com.kuka.roboticsAPI.geometricModel.Frame;
import com.kuka.roboticsAPI.geometricModel.Tool;
import com.kuka.roboticsAPI.geometricModel.World;
import com.kuka.roboticsAPI.geometricModel.math.Transformation;
import com.kuka.roboticsAPI.motionModel.IMotionContainer;
import com.kuka.roboticsAPI.motionModel.PTP;
import com.kuka.roboticsAPI.motionModel.PositionHold;
import com.kuka.roboticsAPI.motionModel.controlModeModel.CartesianImpedanceControlMode;
import com.kuka.roboticsAPI.sensorModel.DataRecorder;
import com.kuka.roboticsAPI.sensorModel.ForceSensorData;
import com.kuka.roboticsAPI.uiModel.ApplicationDialogType;
import com.kuka.connectivity.motionModel.directServo.DirectServo;
import com.kuka.connectivity.motionModel.directServo.IDirectServoRuntime;
import com.kuka.connectivity.motionModel.smartServo.ServoMotion;
import java.io.File;  // Import the File class
import java.io.FileWriter;
import java.io.IOException;  // Import the IOException class to handle errors

public class ForceCtrlImpedance1dim extends RoboticsAPIApplication {

	private static final int stiffnessZ = 300;
	
	private LBR _lbr;
	double initialPosition[] = {20,550, 200};
	double initOrientation[] = {179,0,180};
	double desiredCartesianPose[] ={0,0,0};
	final int iterations=5000;
	@Inject
	@Named("simpleCylender_EMG")
	private Tool tool;//page 371 pdf
	
	 @Inject
	 private MediaFlangeIOGroup mediaflange;
	File myLogfile = new File("logForceCtrlImpedanceKuka6_complete.txt");//**this file will be saved in \\172.31.1.147\krc\ApplicationServer
	FileWriter myWriter;
	
	
	public void initialize() {
		_lbr = getContext().getDeviceFromType(LBR.class);
		tool.attachTo(_lbr.getFlange());
		
		try
		{
			myLogfile.createNewFile();
			myWriter = new FileWriter("logForceCtrlImpedanceKuka6_complete.txt");
		    myWriter.write("ExtForce stiffnessValue  h   x desiredFrameZ   xdot   xddot   E   commandesPos  emgForce Fimp DesiredForceZ desiredDeltaZ \n\r");
		    //myWriter.close();//**remember to close the file at the end of program
		}
		catch (IOException e) {
			getLogger().error(e.toString());
		    }
		
		
	}
	
	
	 /**
     * Move to an initial Position WARNING: MAKE SURE, THAT the pose is collision free.
     */
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
	//@SuppressWarnings("deprecation")
	public void run(){
		DecimalFormat df = new DecimalFormat("0.000000");
		double desiredForceZ[] = {0,0,0};//N
		
		Frame initialPoseOfTool=moveToInitialPosition();
		
		/*
        DataRecorder rec=new DataRecorder("ForceCtrlImpedance1dim_6.log", -1,	TimeUnit.MILLISECONDS,10);
        rec.addCurrentCartesianPositionXYZ(_lbr.getFlange(),_lbr.getRootFrame() );
        rec.addCartesianForce(_lbr.getFlange(),_lbr.getRootFrame() );
        rec.addCartesianTorque(_lbr.getFlange(),_lbr.getRootFrame() );
        rec.addCommandedCartesianPositionXYZ(_lbr.getFlange(),_lbr.getRootFrame());
        
        rec.enable();
        rec.startRecording();*/
		//-------Cartesian Impedance mode
        CartesianImpedanceControlMode cartImpCtrlMode;
        cartImpCtrlMode = new CartesianImpedanceControlMode();
        
        cartImpCtrlMode.parametrize(CartDOF.X,
        CartDOF.Y).setStiffness(4000.0);
        cartImpCtrlMode.parametrize(CartDOF.Z).setStiffness(stiffnessZ);
        cartImpCtrlMode.parametrize(CartDOF.ROT).setStiffness(300.0);
        cartImpCtrlMode.parametrize(CartDOF.ALL).setDamping(0.2);
        
        //cartImpCtrlMode.setAdditionalControlForce(0, 0, 0, 0, 0, 0);
        //getLogger().info(cartImpCtrlMode.getAdditionalControlForce().toString());
        
        //-----
        
		//-----SmartservoLIN motion definitiotions
		/*SmartServoLIN aSmartServoLINMotion  = new SmartServoLIN(initialPoseOfTool);
    	aSmartServoLINMotion .setMinimumTrajectoryExecutionTime(5e-3);
    	
	    ServoMotion.validateForImpedanceMode(tool);
	    getLogger().info("Starting SmartServoLIN motion in position control mode");
        tool.getDefaultMotionFrame().moveAsync(aSmartServoLINMotion.setMode(cartImpCtrlMode));//here binds move method to our SmartServo motion
       
        getLogger().info("Get the runtime of the SmartServo motion");
        ISmartServoLINRuntime theServoRuntime = aSmartServoLINMotion //after binding we use this to command position in realtime
                .getRuntime();
        theServoRuntime.updateWithRealtimeSystem();
        */
        ///-------end.
        ///=====Smart Servo Motin
        /*
        SmartServo aSmartServoMotion = new SmartServo(
                _lbr.getCurrentJointPosition());
        getLogger().info("Starting Smart Servo motion in position control mode");
        tool.getDefaultMotionFrame().moveAsync(aSmartServoMotion.setMode(cartImpCtrlMode));//here binds move method to our SmartServo motion
        aSmartServoMotion.setMinimumTrajectoryExecutionTime(1e-3);
        ISmartServoRuntime theServoRuntime = aSmartServoMotion
                .getRuntime();
        theServoRuntime.updateWithRealtimeSystem();
        */
        ///=====
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
        getLogger().info(initialExtForceTorques.toString());
		
        
      
        Frame desiredFrame =_lbr.getFlange().copyWithRedundancy();// initialPoseOfTool.copyWithRedundancy();
        desiredFrame.setX(initialPoseOfTool.getX());
        desiredFrame.setY(initialPoseOfTool.getY());
        desiredFrame.setZ(initialPoseOfTool.getZ());
        
        desiredFrame.setAlphaRad(initOrientation[0]*3.14/180);
        desiredFrame.setBetaRad(initOrientation[1]*3.14/180);
        desiredFrame.setGammaRad(initOrientation[2]*3.14/180);
        getLogger().info("Loop started");
        
        double h = 0;
        double Xdot =0;
        double Xdoth =0;
        double xddoth = 0;
        double previusXdot =0;
        double xddot =0;
        double currentPosition =0;
        double previusPosition = initialPoseOfTool.getZ()/1000;//position in previus timestep
        final double D_va = 10; //best 10 
        final double M_va = 2;	// best 2
        final double E_t = 0.04;//threshold
        final double E_m = 3.5;// maximum energy in tank
        final double P_d = 0.45;//power disipation rate //best 0.5
        double Ftilda = 0;
        double Po=0;//output power
        double Pi =0;// input power.
        double Edot;
        double E=0; //energy tank
        double dt = 0.01;
        double Fimp = 0;
		double emgForce = -35;

		long startLoopTime; 
		long currentLoopTime = 0;
    	long totalLoopTime =0;
		startLoopTime = System.currentTimeMillis() ;
		mediaflange.setLEDBlue(false);
       for(int i=0;i<iterations;i++)
       {
    	   startLoopTime = System.currentTimeMillis() ;
    	 //calculations for xdot and xddot
    	   //Frame currentCartesanPosition = _lbr.getCurrentCartesianPosition(tool.getDefaultMotionFrame());
    	   theServoRuntime.updateWithRealtimeSystem();
           Frame currentCartesanPosition =  theServoRuntime.getCurrentCartesianPosition(tool.getDefaultMotionFrame());//we can have TCP coordinate using only this method
          
    	   currentPosition = currentCartesanPosition.getZ()/1000;//meter
    	   Xdot = (currentPosition- previusPosition)/dt;
    	   xddot = (Xdot - previusXdot)/dt;
    	   previusXdot = Xdot;
    	   previusPosition = currentPosition;
    	   
    	   Ftilda = M_va*xddot + D_va*Xdot;
           ForceSensorData currentExtForceTorques = _lbr.getExternalForceTorque(_lbr.getFlange(),World.Current.getRootFrame());

    	   if (Math.signum(Ftilda) == Math.signum(emgForce))
    	   {
    		  Ftilda=0;
    	   }
    	   
    	   Pi = Xdot*Ftilda;
    	   Po = Xdoth*Ftilda;
    	   
    	   Edot = Pi - Po -(1-h)*P_d-0.0001;
    	   
    	   E = E + Edot;
    	   if (E<0)
    	   {
    		  E =0;
    	   }
    	   else if (E>E_m)
    	   {
    		   E= E_m;
    	   }
    	   
    	   if (E<= E_t)
    	   {
    		   h =0;
    	   }
    	   else 
    	   {
    		   h = (E-E_t)/(E_m-E_t);
    	   }
    	   
    	   if (h<0)
    	   {
    		   h=0;
    	   }
    	   
    	   Xdoth = h* Xdot;
    	   xddoth = h* xddot;
    	   
    	   Fimp = M_va*xddoth + D_va*Xdoth; //addmittance in paper
    	   
//----------------control over Force with constant impedance    	   
    	   final double alpha = 1;
    	   //desiredForceZ[1] = ((1-h))*emgForce+Fimp;//*1.9-0.9
    	   desiredForceZ[1] = emgForce;
    	   //desiredForceZ[2] = alpha*desiredForceZ[2] + (1-alpha)*desiredForceZ[1];
    	   
    	   //desiredForceZ[0]=desiredForceZ[2];
    	   //desiredForceZ[2] =-2;
    	  
    	   
    	   //theServoRuntime.updateWithRealtimeSystem();
           //currentCartesanPosition =  theServoRuntime.getCurrentCartesianPosition(tool.getDefaultMotionFrame());//we can have TCP coordinate using only this method
    	   if(Xdot < 0)//causes a hard collision if we keep velocity high
    	   {
    		   aDirectServoMotion.setJointVelocityRel(0.1);
    	   }
    	   else{
    		   aDirectServoMotion.setJointVelocityRel(0.4);
    	   }
    	   
           desiredCartesianPose[1] = ( desiredForceZ[1]) / stiffnessZ *1000 ; // desiredCartesianPose[2] is delta z its not posion
           
           //desiredCartesianPose[2] = alpha*desiredCartesianPose[0]+(1-alpha)*desiredCartesianPose[1];
           //desiredCartesianPose[0]= desiredCartesianPose[2]; 
          
           
           desiredFrame.setZ(currentCartesanPosition.getZ() + desiredCartesianPose[1]);//
           //desiredFrame.setZ(currentCartesanPosition.getZ() );
           
         Frame cmdPos = _lbr.getCommandedCartesianPosition(tool.getDefaultMotionFrame());
//-----------------control over impedance parameter with constant delta Z
    	  
//----------------------
           
          try
   		  {
        	  //		"ExtForce stiffnessValue  h   x desiredFrameZ   xdot   xddot   E   commandesPos  emgForce Fimp DesiredForceZ desiredDeltaZ \n\r"
        	  String recordStr = 
            		  String.valueOf( df.format(currentExtForceTorques.getForce().getZ()) ) + " " 
            		+ String.valueOf( df.format(stiffnessZ) )+ " " 
            		+ String.valueOf( df.format(h ) )+ " " 
            		+ String.valueOf( df.format(currentPosition )) + " "
            		+ String.valueOf( df.format(desiredFrame.getZ() )) + " "
            		+ String.valueOf( df.format(Xdot )) + " " 
            		+ String.valueOf( df.format(xddot))+ " "
            		+ String.valueOf( df.format(E))+ " "
            		+ String.valueOf( df.format(cmdPos.getZ()))+ " "
            		+ String.valueOf( df.format(emgForce))+ " "
            		+ String.valueOf( df.format(Fimp))+ " "
            		+ String.valueOf( df.format(desiredForceZ[1]))+ " "
        	        + String.valueOf( df.format(desiredCartesianPose[1]))+ " \n\r";
            		
        	  myWriter.write(recordStr);
   		    //myWriter.close();//**remember to close the file at the end of program
   		  }
   		  catch (IOException e) {
   			getLogger().error(e.toString());
   		    }
           
          try{
     			theServoRuntime.setDestination(desiredFrame); 			
     		}
     		catch (Exception e) {
     			getLogger().error("Out of reach");
     		}
           //theServoRuntime.updateWithRealtimeSystem();(int)(dt*1000)
    	   ThreadUtil.milliSleep(5);
           currentLoopTime =  System.currentTimeMillis() ;
           totalLoopTime = currentLoopTime-startLoopTime;
           dt = (double)totalLoopTime/1000;
           //getLogger().error(String.valueOf(totalLoopTime));    
       }
       
       
       try{
    	   myWriter.close();//**remember to close the file at the end of program
    	  // rec.stopRecording();
       }
       catch (IOException e){
    	   //
       }
       
	}

}