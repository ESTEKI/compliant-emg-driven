// this program were used to sent cartesian force and torque data over UDP connection
package EMG_Mirroring;

import static com.kuka.roboticsAPI.motionModel.BasicMotions.lin;
//import static com.kuka.roboticsAPI.motionModel.BasicMotions.linRel;
import static com.kuka.roboticsAPI.motionModel.BasicMotions.ptp;
//import static com.kuka.roboticsAPI.motionModel.BasicMotions.spl;

import java.text.DecimalFormat;

import javax.inject.Inject;
import javax.print.attribute.standard.Media;
//import java.util.Currency;
//import java.util.concurrent.TimeUnit;

//import com.kuka.common.StatisticTimer;
//import com.kuka.common.StatisticTimer.OneTimeStep;
import com.kuka.common.DataLogger;
import com.kuka.common.ThreadUtil;
import com.kuka.connectivity.motionModel.smartServo.ISmartServoRuntime;
import com.kuka.connectivity.motionModel.smartServo.ServoMotion;
import com.kuka.connectivity.motionModel.smartServo.SmartServo;
import com.kuka.connectivity.motionModel.smartServoLIN.ISmartServoLINRuntime;
import com.kuka.connectivity.motionModel.smartServoLIN.SmartServoLIN;
import com.kuka.generated.ioAccess.MediaFlangeIOGroup;

import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;
import com.kuka.roboticsAPI.deviceModel.JointPosition;
import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.geometricModel.AbstractFrame;
import com.kuka.roboticsAPI.geometricModel.Frame;
import com.kuka.roboticsAPI.geometricModel.LoadData;
import com.kuka.roboticsAPI.geometricModel.ObjectFrame;
import com.kuka.roboticsAPI.geometricModel.Tool;
import com.kuka.roboticsAPI.geometricModel.World;
import com.kuka.roboticsAPI.geometricModel.math.IRotation;
import com.kuka.roboticsAPI.geometricModel.math.MatrixTransformation;
import com.kuka.roboticsAPI.geometricModel.math.Rotation;
import com.kuka.roboticsAPI.geometricModel.math.Transformation;
import com.kuka.roboticsAPI.geometricModel.math.Vector;
import com.kuka.roboticsAPI.geometricModel.math.XyzAbcTransformation;
import com.kuka.roboticsAPI.motionModel.IMotion;
import com.kuka.roboticsAPI.motionModel.controlModeModel.IMotionControlMode;
import com.kuka.roboticsAPI.sensorModel.DataRecorder;
import com.kuka.roboticsAPI.sensorModel.DataRecorder.AngleUnit;
import com.kuka.roboticsAPI.sensorModel.ForceSensorData;

/**
 * 
 * 
 */
public class appv1java extends RoboticsAPIApplication
{

    private LBR _lbr;
    //private ISmartServoRuntime _theSmartServoRuntime = null;
    
    private final String otherMachineIP = "172.31.1.150";// windows 10 pc 
    private UDPConnection udp = new UDPConnection(30002, 50004, otherMachineIP,800) ;  
    
    double subjectForearmLen = 340.0;// in mm
    // Tool Data
    private Tool tool;
    
    private LoadData _loadData;
    private static final String TOOL_FRAME = "toolFrame";
    private static final double[] TRANSLATION_OF_TOOL = { 0, 0, 175 };
    private static final double MASS = 0;
    private static final double[] CENTER_OF_MASS_IN_MILLIMETER = { 0, 0, 100 };

    double initialPosition[] = {20,600, -35};
    double initOrientation[] = {90,0,180};
    double desiredCartesianPose[] ={0,0,0};
    private String command;
    @Inject
    private MediaFlangeIOGroup mediaflange;
    
    
    @Override
    public void initialize()
    {
    	_lbr = getContext().getDeviceFromType(LBR.class);
		_loadData = new LoadData();
        _loadData.setMass(0.2);
        _loadData.setCenterOfMass(0,0,100);
        tool = new Tool("Tool", _loadData);

        XyzAbcTransformation trans = XyzAbcTransformation.ofTranslation(
                0, 0, 175);
        ObjectFrame aTransformation = tool.addChildFrame("TCP1", trans);
        tool.setDefaultMotionFrame(aTransformation);
        // Attach tool to the robot
         tool.attachTo(_lbr.getFlange());
        
        udp.setBufferSize(128);
        
    }
    
    @Override
	public void dispose(){
		udp.closeUDP(); //Close the UDP socket and free it's port  
	}

    /**
     * Move to an initial Position WARNING: MAKE SURE, THAT the pose is collision free.
     */
    public Frame moveToInitialPosition()
    {
    	
    	Frame flange = _lbr.getFlange().copyWithRedundancy();
		
    	Frame desiredFrame = flange.transform(Transformation.ofTranslation(0,0, 175)); // adding a Frame on tool TCP.
     	desiredFrame.setParent(_lbr.getRootFrame());
        //Transformation bTn = desiredFrame.transformationFromWorld();//tool center point transformation(pos+orinetation) w.r.t. World
	
    	//bTn = Transformation.of(bTn.withTranslation(desiredPosition).getTranslation(), 
    			//bTn.withRotation(desiredOrientation).getRotationMatrix());
    	
    	//desiredFrame.setTransformationFromParent(bTn);
    desiredFrame.setX(initialPosition[0]);
    desiredFrame.setY(initialPosition[1]);
    desiredFrame.setZ(initialPosition[2]);
    
    desiredFrame.setAlphaRad(initOrientation[0]*3.14/180);
    desiredFrame.setBetaRad(initOrientation[1]*3.14/180);
    desiredFrame.setGammaRad(initOrientation[2]*3.14/180);
    
    	//getLogger().info(String.valueOf(deltaForceZ) + "  " + String.valueOf(pc));
    	//getLogger().error(desiredFrame.toString());
    	try{
    		tool.move(ptp(desiredFrame).setJointVelocityRel(0.2));
    	}
    	catch (Exception e) {
    		getLogger().error(e.getMessage());  
		}
    	return desiredFrame;
    
    }

    @SuppressWarnings("deprecation")
	@Override
    public void run()
    {
    	mediaflange.setLEDBlue(false);
    	double kaf = 1;
		double kvf = 0.9;
		double vcDot = 0;// pcDotDot = vcDot;
		double vc = 0;//pcDot = vc;
		double pc =0;// position computed by force controller
		double pr = -20;// reference position by visual or encoder or ...
		
        
    	Frame initCommandedpos = moveToInitialPosition();
    	DecimalFormat df = new DecimalFormat("0.000");
    	Frame currentPoseOfTool;
    	Frame initialPoseOfTool;
    	
        
    	//-----Smartservo motion definitiotions
    	Frame flange = _lbr.getFlange().copyWithRedundancy();
    	Frame desiredFrame = flange.transform(Transformation.ofTranslation(0,0, 175)); // adding a Frame on tool TCP.
     	desiredFrame.setParent(_lbr.getRootFrame());
     	
    	SmartServoLIN aSmartServoLINMotion  = new SmartServoLIN(initCommandedpos);
    	aSmartServoLINMotion .setMinimumTrajectoryExecutionTime(5e-3);
	    
	    
	    getLogger().info("Starting SmartServoLIN motion in position control mode");
        tool.moveAsync(aSmartServoLINMotion );//here binds move method to our SmartServo motion
        
        getLogger().info("Get the runtime of the SmartServo motion");
        ISmartServoLINRuntime theServoRuntime = aSmartServoLINMotion //after binding we use this to command position in realtime
                .getRuntime();
        ///-------end.
        
        theServoRuntime.updateWithRealtimeSystem();
        initialPoseOfTool = theServoRuntime.getCurrentCartesianPosition(tool.getDefaultMotionFrame());//we can have TCP coordinate using only this method
    	getLogger().info(initialPoseOfTool.toString());
    	ForceSensorData initialExtForceTorques = _lbr.getExternalForceTorque(_lbr.getFlange(),World.Current.getRootFrame());
        getLogger().info(initialExtForceTorques.toString());
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
        long startLoopTime = System.currentTimeMillis() ;
    	long currentLoopTime = 0;
    	long totalLoopTime = 0;
    	while (command != "STOP" && totalLoopTime < 30 )
        {
    		command = udp.readRawDatastr();
        	String[] cmdvalues = command.split("#");
        	if (cmdvalues.length == 7)
        	{
        		theServoRuntime.updateWithRealtimeSystem();
                currentPoseOfTool = theServoRuntime.getCurrentCartesianPosition(tool.getDefaultMotionFrame());//we can have TCP coordinate using only this method
            	/*
            	 * cmdvalues[0] : x cartesian
            	 * cmdvalues[1] : y cartesian
            	 * cmdvalues[2] : torque
            	 * cmdvalues[3] :roll
            	 * cmdvalues[4] : pitch
            	 * cmdvalues[5] : yaw
            	 * cmdvalues[6] : errmsg
            	 */
               
        		desiredCartesianPose[0] =  currentPoseOfTool.getX() + Double.parseDouble(cmdvalues[0])*0.8;
                desiredCartesianPose[1] =  currentPoseOfTool.getY() + Double.parseDouble(cmdvalues[1])*0.8;
                
        		
                ForceSensorData currentExtForceTorques = _lbr.getExternalForceTorque(_lbr.getFlange(),World.Current.getRootFrame());
            	//getLogger().info(forces.toString()+"\n"+
            	//		String.valueOf(currentExtForceTorques));
                
                double desireForceZ = Double.parseDouble(cmdvalues[2]);
                if (desireForceZ > 25) desireForceZ=25;
                else if (desireForceZ < -25) desireForceZ=-25;
            	double deltaForceZ = -desireForceZ + (currentExtForceTorques.getForce().getZ() - initialExtForceTorques.getForce().getZ());
            	double deltaT = 0.015;
            	vcDot = (deltaForceZ - kvf*vc)/kaf;
            	vc = vc + vcDot*deltaT;
            	pc = pc + vc*deltaT;
            	desiredCartesianPose[2] =  initialPoseOfTool.getZ() + pc;
            	
            	double rollangle = Double.parseDouble(cmdvalues[3]);
            	double pitchangle = Double.parseDouble(cmdvalues[4]);
            	double yawangle = Double.parseDouble(cmdvalues[5]);
              //desired Transformation p and R
            	//Vector desiredPosition = Vector.of(desiredCartesianPose[0],desiredCartesianPose[1], desiredCartesianPose[2]);
            	//IRotation desiredOrientation = Rotation.ofRad((-90)*3.14/180, (-pitchangle+180)*3.14/180, -rollangle);//z y x
    		
            	/*
             	Transformation bTn = desiredFrame.transformationFromWorld();//tool center point coordinate(transformation) w.r.t. World
    		
            	bTn = Transformation.of(bTn.withTranslation(desiredPosition).getTranslation(), 
            			bTn.withRotation(desiredOrientation).getRotationMatrix());
            
            	desiredFrame.setTransformationFromParent(bTn);
            	*/

             	desiredFrame.setX(desiredCartesianPose[0]);
             	desiredFrame.setY(desiredCartesianPose[1]);
             	desiredFrame.setZ(desiredCartesianPose[2]);
				 
             	desiredFrame.setAlphaRad((initOrientation[0]+yawangle)*3.14/180);
             	desiredFrame.setBetaRad((-pitchangle+initOrientation[1])*3.14/180);
             	desiredFrame.setGammaRad( (rollangle+initOrientation[2])*3.14/180);
                
            	try{
            		theServoRuntime.setDestination(desiredFrame);
            	}
            	catch (Exception e) {
					getLogger().error("Out of reach");
            		
				}
            	
        		ForceSensorData extForceTorquestr = _lbr.getExternalForceTorque(_lbr.getFlange(),World.Current.getRootFrame());
        		//ForceSensorData extForceTorquestr2 = _lbr.getExternalForceTorque(_toolAttachedToLBR.getDefaultMotionFrame(),World.Current.getRootFrame());
        		//getLogger().info(String.valueOf(extForceTorquestr));
        		//getLogger().error(String.valueOf(extForceTorquestr2));
        		String recordStr = 
        		  String.valueOf( df.format(extForceTorquestr.getForce().getX()) ) + " " 
        		+ String.valueOf( df.format(extForceTorquestr.getForce().getY()) ) + " " 
        		+ String.valueOf( df.format(extForceTorquestr.getForce().getZ()) ) + " " 
        		+ String.valueOf( df.format(extForceTorquestr.getTorque().getX()) ) + " " 
        		+ String.valueOf( df.format(extForceTorquestr.getTorque().getY()) ) + " " 
        		+ String.valueOf( df.format(extForceTorquestr.getTorque().getZ()) ) ;
        		
        		startLoopTime = System.currentTimeMillis() ;
        		udp.writeDatastr(recordStr+ "  " + String.valueOf(pc)+ "  " + String.valueOf(desireForceZ));
        		
        	}
        	
        	currentLoopTime = System.currentTimeMillis();
        	totalLoopTime = (currentLoopTime - startLoopTime)/1000;
        	
        
        }
      /*  for ( int i =0; i < command.length ; i++ )
        {
        	if(command[i] != '\n')
        	{	
        		if(command[i]=='.')
        		{
        			elbowAngle = elbowAngle + ".";
        		}
        		else{
        			elbowAngle = elbowAngle + String.valueOf(command[i]-'0');	
        		}
        		 
        	}
        	else {
        		break;
        	}
        }
        
        getLogger().info(elbowAngle);
        
        
    	getLogger().info("Loop Started...");
    	command = udp.readRawDatabyte();
        while (command[0] != '3' && totalLoopTime < 15 )
        {
        	command = udp.readRawDatabyte(); 
        	
        	if (command[0] == '2')
        	{
        		startLoopTime = System.currentTimeMillis() ;
        		ForceSensorData extForceTorquestr = _lbr.getExternalForceTorque(_lbr.getFlange(),World.Current.getRootFrame());
        		//ForceSensorData extForceTorquestr2 = _lbr.getExternalForceTorque(_toolAttachedToLBR.getDefaultMotionFrame(),World.Current.getRootFrame());
        		//getLogger().info(String.valueOf(extForceTorquestr));
        		//getLogger().error(String.valueOf(extForceTorquestr2));
        		String recordStr = 
        		  String.valueOf( df.format(extForceTorquestr.getForce().getX()) ) + " " 
        		+ String.valueOf( df.format(extForceTorquestr.getForce().getY()) ) + " " 
        		+ String.valueOf( df.format(extForceTorquestr.getForce().getZ()) ) + " " 
        		+ String.valueOf( df.format(extForceTorquestr.getTorque().getX()) ) + " " 
        		+ String.valueOf( df.format(extForceTorquestr.getTorque().getY()) ) + " " 
        		+ String.valueOf( df.format(extForceTorquestr.getTorque().getZ()) ) ;
        		
        		udp.writeDatastr(recordStr);
        		
        	}
        	
        	currentLoopTime = System.currentTimeMillis();
        	totalLoopTime = (currentLoopTime - startLoopTime)/1000;
        	
            //String s = "";
            /*if(command[i] != '\n')
        	{	
        		if(command[i]=='.')
        		{
        			s = s + ".";
        		}
        		else{
        			s = s + String.valueOf(command[i]-'0');	
        		}
        		 
        	}
        	else {
        		break;
        	}*
        }
        */
    	
        getLogger().info("END loop");
        mediaflange.setLEDBlue(false);
        ThreadUtil.milliSleep(100);
        
    }

    /**
     * Main routine, which starts the application.
     * 
     * @param args
     *            arguments
     */
    public static void main(String[] args)
    {
    	appv1java app = new
    			appv1java();
        app.runApplication();
    }
}
