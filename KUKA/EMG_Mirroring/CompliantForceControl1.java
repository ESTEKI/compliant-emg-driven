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
import com.kuka.roboticsAPI.uiModel.ApplicationDialogType;

/**
 * Author :Siavash Esteki [[esteki.siavash@yahoo.com]]
 * This program enables controlling force in Impedance mode, 
 * while being compliant to external forces.
 * Robot excerts desired force along Z axis w.r.t world frame. 
 * In xy plane it is only compliant position controlled.
 * 
 * @see UseRoboticsAPIContext
 * @see #initialize()
 * @see #run()
 * @see #dispose()
 */
public class CompliantForceControl1 extends RoboticsAPIApplication {
	@Inject
	private LBR lBR_iiwa_14_R820_1;
	
	private static final int stiffnessZ = 500;
	private static final int stiffnessXY = 500;
	private LBR _lbr;
	double initialPosition[] = {20,550, 350};
	double initOrientation[] = {179,0,180};
	double desiredCartesianPose[] ={0,0,0};
	final int iterations=3000;
	@Inject
	@Named("Cylinder")
	private Tool tool;//page 371 pdf
	
	@Inject
	 private MediaFlangeIOGroup mediaflange;
	File myLogfile = new File("compliantForceControl1_1.txt");//**this file will be saved in \\172.31.1.147\krc\ApplicationServer
	FileWriter myWriter;
	
	
	@Override
	public void dispose(){
		try{
		myWriter.close();
		}
		catch (IOException err){
			
		}
		
	}
	
	@Override
	public void initialize() {
		_lbr = getContext().getDeviceFromType(LBR.class);
		tool.attachTo(_lbr.getFlange());
		
		try
		{
			myLogfile.createNewFile();
			myWriter = new FileWriter("compliantForceControl1_1.txt");
		    myWriter.write(" currentPositionx	currentPositiony	currentPositionZ    Fe_x  Fe_y  Fe_z  hx  hy  hz  desiredForceZ xTask yTask  emgForce \n\r");
		    //myWriter.close();//**remember to close the file at the end of program
		}
		catch (IOException e) {
			getLogger().error(e.toString());
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

	@Override
	public void run() {
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
        
        int isCompliantAlongZ = getApplicationUI().displayModalDialog(ApplicationDialogType.QUESTION, "Robot become compliant in Z direction?", "NO","Yes" );
	    int isCompliantAlongY = getApplicationUI().displayModalDialog(ApplicationDialogType.QUESTION, "Robot become compliant in Y direction?", "NO" ,"Yes");
        int isCompliantAlongX = getApplicationUI().displayModalDialog(ApplicationDialogType.QUESTION, "Robot become compliant in X direction?", "NO" ,"Yes");
        
        int forceval = getApplicationUI().displayModalDialog(ApplicationDialogType.QUESTION, "Choose simulated EMG force to excert in Z direction", "0 N" ,"-25 N","-45 N");
        if (forceval == 0){
            emgForce = 0;
        }
        else if (forceval == 1){
        	emgForce = -25;
        }
        else {
        	emgForce = -45;
        }
        
       // currentPositionx	currentPositionZ currentPositiony Fe_x Fe_y Fe_z desiredForceZ 
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
        double previusPositionx = initialPoseOfTool.getX();
        double x1[] ={0,0}; // x1[0] is previus x1 and x[1] is current x1
        double x2[] = {0,0};
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
        double previusPositiony = 0;
        double y1[] ={0,0}; // x1[0] is previus x1 and x[1] is current x1
        double y2[] = {0,0};
        //--------
        
        double k1,k2,k3,k4;//Rung-Kuta parameters
        double l1,l2,l3,l4;//Rung-Kuta parameters
        double Po=0;//output power
        double Pi =0;// input power.
        double Edot;
        double dt = 0.015;
        long startLoopTime; 
		long currentLoopTime = 0;
    	long totalLoopTime =0;
    	
    	startLoopTime = System.currentTimeMillis() ;
		mediaflange.setLEDBlue(true);
        getLogger().info("Loop started");
        for(int i=0;i<iterations;i++)
        {
     	  startLoopTime = System.currentTimeMillis() ;
     	  theServoRuntime.updateWithRealtimeSystem();
          Frame currentCartesanPosition =  theServoRuntime.getCurrentCartesianPosition(tool.getDefaultMotionFrame());//we can have TCP coordinate using only this method
          ForceSensorData currentExtForceTorques = _lbr.getExternalForceTorque(_lbr.getFlange(),World.Current.getRootFrame());
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
   	   
   	   
   	   double xTask = initialPoseOfTool.getX();//would be the x coordinate of mouse in final application
   	   double xdesired;
   	   if (isCompliantAlongX==1){
   		   xdesired = xTask + (currentPositionx - xTask)*hx ;
   	   }
   	   else{
   		   xdesired = xTask;
   	   }
   	   
   	  if(Math.abs(previusPositionx - xTask) > Math.abs(currentPositionx - xTask) && Math.abs(currentPositionx - xTask) > 10  )// meaning robot is moving towards xTask
	  	   {//this keeps the speed of robot natural when going back to its desired position comanded bu operator
	  		//setJointVelocityRel did not work!
	  		//aDirectServoMotion.setJointVelocityRel(0.1);
	  	    //mediaflange.setLEDBlue(false);
	  		xdesired =  -1*Math.signum(currentPositionx - xdesired) *18 + currentPositionx; 
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
	   
	   double yTask = initialPoseOfTool.getY();//would be the x coordinate of mouse
	   double ydesired = yTask + (currentPositiony - yTask)*hy ; 
	   
	   if (isCompliantAlongY==1){
   		   ydesired = yTask + (currentPositiony - yTask)*hy ;
   	   }
   	   else{
   		   ydesired = yTask;
   	   }
	   
	   if(Math.abs(previusPositiony - yTask) > Math.abs(currentPositiony - yTask) && Math.abs(currentPositiony - yTask) > 10 )// && aDirectServoMotion.getJointVelocityRel() > 0.1)// meaning robot is moving towards xTask
  	   {//this keeps the speed of robot natural
  		   //setJointVelocityRel did not work!
  		
  	   // mediaflange.setLEDBlue(false);
  		ydesired =  -1*Math.signum(currentPositiony - ydesired) *18 + currentPositiony; 
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
	   double desiredForceZ=0;
	   
	   if (isCompliantAlongZ==1){
		   desiredForceZ = ((1-hz))*emgForce+Fimp ;
   	   }
   	   else{
   		   desiredForceZ = emgForce;
   	   }
	    
	   	  
	   	   if(Xdot < 0)//causes a hard collision if we keep velocity high
	   {
		   aDirectServoMotion.setJointVelocityRel(0.1);
	   }
	   else{
		   aDirectServoMotion.setJointVelocityRel(0.4);
	   }
	   
    desiredCartesianPose[1] = (desiredForceZ ) / stiffnessZ *1000 ; // 
   
    
    desiredFrame.setZ(currentCartesanPosition.getZ() + desiredCartesianPose[1]);//
   	   
   	   
   	   ////////////////////////////////////////////////////
   	 try{
			theServoRuntime.setDestination(desiredFrame); 			
		}
		catch (Exception e) {
			getLogger().error("Out of reach");
		}
   	 try{
   		//currentPositionx	currentPositiony	currentPositionZ    Fe_x  Fe_y  Fe_z  hx  hy  hz  desiredForceZ xTask yTask  emgForce 
   	  String recordStr = 
      		  String.valueOf( df.format(currentPositionx) ) + " " 
      		+ String.valueOf( df.format(currentPositiony) )+ " " 
      		+ String.valueOf( df.format(currentPositionZ ) )+ " " 
      		+ String.valueOf( df.format(Fe_x )) + " "
      		+ String.valueOf( df.format(Fe_y )) + " "
      		+ String.valueOf( df.format(currentExtForceTorques.getForce().getZ())) + " " 
      		+ String.valueOf( df.format(hx))+ " "
      		+ String.valueOf( df.format(hy))+ " "
      		+ String.valueOf( df.format(hz))+ " "
      		+ String.valueOf( df.format(desiredForceZ))+ " "
      		+ String.valueOf( df.format(xTask))+ " "
      		+ String.valueOf( df.format(yTask))+ " "
   	        + String.valueOf( df.format(emgForce))+ " \n\r";
      		
   	  myWriter.write(recordStr);
   	 }
   	 catch(Exception err){
   		 
   	 }

		 //theServoRuntime.updateWithRealtimeSystem();(int)(dt*1000)
		 ThreadUtil.milliSleep(5);
		 currentLoopTime =  System.currentTimeMillis() ;
		 totalLoopTime = currentLoopTime-startLoopTime;
		 dt = (double)totalLoopTime/1000;
	  }
	}
}