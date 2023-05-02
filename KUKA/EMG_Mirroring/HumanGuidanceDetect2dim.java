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
/**
 * @author Siavash Esteki
 * 
 *
 */
public class HumanGuidanceDetect2dim extends RoboticsAPIApplication {

private static final int stiffnessXY = 500;
	
	@Inject
	private LBR _lbr;
	
	double initialPosition[] = {20,550, 200};
	double initOrientation[] = {179,0,180};
	double desiredCartesianPose[] ={0,0,0};
	final int iterations=3000;
	@Inject
	@Named("USprobe")
	private Tool tool;//page 371 pdf
	
	 @Inject
	 private MediaFlangeIOGroup mediaflange;
	 
	 File myLogfile = new File("logHumanGuidanceDetect2dim.txt");//**this file will be saved in \\172.31.1.147\krc\ApplicationServer
	 FileWriter myWriter;

	@Override
	public void initialize() {
		_lbr = getContext().getDeviceFromType(LBR.class);
		tool.attachTo(_lbr.getFlange());
		
		try
		{
			myLogfile.createNewFile();
			myWriter = new FileWriter("Humanguidancedetect2dimxy_h.txt");
		    myWriter.write("ExtForceX ExtForceY  h  xdesired  dx  x  y    stiffnessXY  \n\r");
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
    
	@Override
	public void run() {
		DecimalFormat df = new DecimalFormat("0.000000");
		double desiredForceZ[] = {0,0,0};//N
		
		Frame initialPoseOfTool=moveToInitialPosition();
	///-----------Impedance Controller	
		CartesianImpedanceControlMode cartImpCtrlMode;
        cartImpCtrlMode = new CartesianImpedanceControlMode();
        
        cartImpCtrlMode.parametrize(CartDOF.X,
        CartDOF.Y).setStiffness(stiffnessXY);
        cartImpCtrlMode.parametrize(CartDOF.Z).setStiffness(4000);
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
        getLogger().info(initialExtForceTorques.toString());
		
        
      
        Frame desiredFrame =_lbr.getFlange().copyWithRedundancy();// initialPoseOfTool.copyWithRedundancy();
        desiredFrame.setX(initialPoseOfTool.getX());
        desiredFrame.setY(initialPoseOfTool.getY());
        desiredFrame.setZ(initialPoseOfTool.getZ());
        
        desiredFrame.setAlphaRad(initOrientation[0]*3.14/180);
        desiredFrame.setBetaRad(initOrientation[1]*3.14/180);
        desiredFrame.setGammaRad(initOrientation[2]*3.14/180);
        
        double h = 0;
        double currentPosition =0;
        double previusPosition = initialPoseOfTool.getZ()/1000;//position in previus timestep
        final double D_va = 2; //best 10 
        final double M_va = 1;	// best 2
        final double E_t = 0.4;//threshold
        final double E_m = 4;// maximum energy in tank
        final double P_d = 0.40;//power disipation rate //best 0.5
         
        double Po=0;//output power
        double Pi =0;// input power.
        double Edot;
        double E=0; //energy tank
        double dt = 0.015;
        double Fh=0;
		long startLoopTime; 
		long currentLoopTime = 0;
    	long totalLoopTime =0;
    	
    	startLoopTime = System.currentTimeMillis() ;
		mediaflange.setLEDBlue(false);
        getLogger().info("Loop started");
        
        double x1[] ={0,0}; // x1[0] is previus x1 and x[1] is current x1
        double x2[] = {0,0};
        double k1,k2,k3,k4;
        double l1,l2,l3,l4;
        
        double x1va[] ={0,0}; // x1[0] is previus x1 and x[1] is current x1
        double x2va[] = {0,0};
        
        for(int i=0;i<iterations;i++)
        {
           startLoopTime = System.currentTimeMillis() ;
       	   theServoRuntime.updateWithRealtimeSystem();
           Frame currentCartesanPosition =  theServoRuntime.getCurrentCartesianPosition(tool.getDefaultMotionFrame());//we can have TCP coordinate using only this method
           ForceSensorData currentExtForceTorques = _lbr.getExternalForceTorque(_lbr.getFlange(),World.Current.getRootFrame());
           currentPosition = currentCartesanPosition.getX();
           double Fe = currentExtForceTorques.getForce().getX();
          
           k1 = dt*x2[0];
           l1 = dt* (-D_va*x1[0]/M_va+Fe/M_va );
           k2 = dt*(x2[0]+l1/2);
           l2 = dt*((-D_va*(x1[0]+k1/2))/M_va+Fe/M_va );
           k3 = dt*(x2[0]+l2/2);
           l3 = dt* ((-D_va*(x1[0]+k2/2))/M_va+Fe/M_va );
           k4 = dt*(x2[0]+l3);
           l4 = dt* ((-D_va*(x1[0]+k3))/M_va+Fe/M_va);
           
           x1[1] = (x1[0] + (1.0/6.0)*(k1+2*k2+2*k3+k4));
           x2[1] = x2[0] + (1.0/6.0)*(l1+2*l2+2*l3+l4); 
           
           double xtildadot = x2[1];
           x1[0] = x1[1] ;
           x2[0] = x2[1];
           
           Pi = xtildadot/1000*Fe;
           Po = xtildadot/1000*Fh;
           
           Edot = Pi - Po -(1-h)*P_d;
           
           E = E + Edot;
           
    	   if (E<0)
    	   {
    		  E = 0;
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
    	   
    	   Fh = Fe*h;
    	   
    	   /*k1 = dt*x2va[0];
           l1 = dt* (-D_va*x1va[0]/M_va+Fh/M_va );
           k2 = dt*(x2va[0]+l1/2);
           l2 = dt*((-D_va*(x1va[0]+k1/2))/M_va+Fh/M_va );
           k3 = dt*(x2va[0]+l2/2);
           l3 = dt* ((-D_va*(x1va[0]+k2/2))/M_va+Fh/M_va );
           k4 = dt*(x2va[0]+l3);
           l4 = dt* ((-D_va*(x1va[0]+k3))/M_va+Fh/M_va);
           
           x1va[1] = (x1va[0] + (1.0/6.0)*(k1+2*k2+2*k3+k4));
           x2va[1] = x2va[0] + (1.0/6.0)*(l1+2*l2+2*l3+l4);
    	   double xdotva = x2va[1];
           x1va[0] = x1va[1] ;
           x2va[0] = x2va[1];
           */
    	   double xTask = initialPoseOfTool.getX();//would be the x coordinate of mouse
    	   double xdesired = xTask  + (currentPosition - xTask)*h ; 
    	   
    	   
    	   
    	   if(Math.abs(previusPosition - xTask) > Math.abs(currentPosition - xTask) && Math.abs(currentPosition - xTask) > 10 && aDirectServoMotion.getJointVelocityRel() !=0.1)// && aDirectServoMotion.getJointVelocityRel() > 0.1)// meaning robot is moving towards xTask
    	   {
    		   //setJointVelocityRel did not work!
    		aDirectServoMotion.setJointVelocityRel(0.1);
    	    mediaflange.setLEDBlue(false);
    		xdesired =  -1*Math.signum(currentPosition - xTask) *18 + currentPosition; 
    	   }
    	   else if (aDirectServoMotion.getJointVelocityRel() !=0.4)
    	   {
    		   aDirectServoMotion.setJointVelocityRel(0.4);
    		   mediaflange.setLEDBlue(true);
    	   }
           
    	   desiredFrame.setX(xdesired) ; 
    	   previusPosition = currentPosition;
    	   //desiredFrame.setX(x)
           try
    		  {
         	  //		"ExtForceX ExtForceY  h  xdesired  dx  x  y    stiffnessXY  \n\r"
         	  String recordStr = 
             		  String.valueOf( df.format(Fe) ) + " " 
             		+ String.valueOf( df.format(currentExtForceTorques.getForce().getY()) ) + " " 
             		+ String.valueOf( df.format(h)) + " "
             		+ String.valueOf( df.format(xdesired )) + " "
             		+ String.valueOf( df.format(currentPosition - xTask )) + " "
             		+ String.valueOf( df.format(currentPosition )) + " "
             		+ String.valueOf( df.format(currentCartesanPosition.getY() ))+ " "
             		+ String.valueOf( df.format(stiffnessXY )) +" \n\r";
             		
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

	}
}