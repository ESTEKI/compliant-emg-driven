// this program were used to sent Cartesian force and torque data over UDP connection
package EMG_Mirroring;


import static com.kuka.roboticsAPI.motionModel.BasicMotions.ptp;


import java.text.DecimalFormat;


import javax.inject.Inject;

import com.kuka.common.ThreadUtil;

import com.kuka.generated.ioAccess.MediaFlangeIOGroup;
import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;

import com.kuka.roboticsAPI.deviceModel.LBR;

import com.kuka.roboticsAPI.geometricModel.Frame;
import com.kuka.roboticsAPI.geometricModel.LoadData;
import com.kuka.roboticsAPI.geometricModel.ObjectFrame;
import com.kuka.roboticsAPI.geometricModel.Tool;
import com.kuka.roboticsAPI.geometricModel.World;

import com.kuka.roboticsAPI.geometricModel.math.Transformation;
import com.kuka.roboticsAPI.geometricModel.math.XyzAbcTransformation;
import com.kuka.roboticsAPI.sensorModel.ForceSensorData;

/**
 * This example activates a SmartServo motion in position control mode, sends a sequence of joint specific set points,
 * describing a sine function and evaluates the statistic timing.
 */
public class ForCeTorqueMonitor extends RoboticsAPIApplication
{

    private LBR _lbr;
    //private ISmartServoRuntime _theSmartServoRuntime = null;
    
    private final String otherMachineIP = "172.31.1.150";// windows 10 pc 
    private UDPConnection udp = new UDPConnection(30002, 50004, otherMachineIP,800) ;  
    
    double subjectForearmLen = 340.0;// in mm
    // Tool Data
    private Tool _toolAttachedToLBR;
    
    private LoadData _loadData;
    private static final String TOOL_FRAME = "toolFrame";
    private static final double[] TRANSLATION_OF_TOOL = { 0, 0, 175 };
    private static final double MASS = 0;
    private static final double[] CENTER_OF_MASS_IN_MILLIMETER = { 0, 0, 100 };

    private byte[] command;
    @Inject
    private MediaFlangeIOGroup mediaflange;
    
    @Override
    public void initialize()
    {
        _lbr = getContext().getDeviceFromType(LBR.class);

        // Create a Tool by Hand this is the tool we want to move with some mass
        // properties and a TCP-Z-offset of 100.
        _loadData = new LoadData();
        _loadData.setMass(MASS);
        _loadData.setCenterOfMass(
                CENTER_OF_MASS_IN_MILLIMETER[0], CENTER_OF_MASS_IN_MILLIMETER[1],
                CENTER_OF_MASS_IN_MILLIMETER[2]);
       
        _toolAttachedToLBR = new Tool("Tool", _loadData);
       
        XyzAbcTransformation trans = XyzAbcTransformation.ofTranslation(
                TRANSLATION_OF_TOOL[0], TRANSLATION_OF_TOOL[1],
                TRANSLATION_OF_TOOL[2]);
        ObjectFrame aTransformation = _toolAttachedToLBR.addChildFrame(TOOL_FRAME
                + "(TCP)", trans);
        //ObjectFrame aTransformation = _toolAttachedToLBR.addChildFrame("fdfdf", _lbr.);
       // _toolAttachedToLBR.setDefaultMotionFrame(aTransformation);
        // Attach tool to the robot
       _toolAttachedToLBR.setDefaultMotionFrame(aTransformation);
        _toolAttachedToLBR.attachTo(_lbr.getFlange());
        
        udp.setBufferSize(128);
        
    }
    
    @Override
	public void dispose(){
		udp.closeUDP(); //Close the UDP socket and free it's port  
	}

    /**
     * Move to an initial Position WARNING: MAKE SURE, THAT the pose is collision free.
     */
    public void moveToInitialPosition()
    {
    	
    	Frame currentPoseOfTool = _lbr.getCurrentCartesianPosition(_lbr.getFlange().getChild(_toolAttachedToLBR.getName()));// position of tool
    	getLogger().info(currentPoseOfTool.toString());
    	double startposeX = 0;
    	startposeX = currentPoseOfTool.getX();//
    	getLogger().info(Double.toString(startposeX));
    	
    	
    
    	double deltaY =  Math.sin(20*Math.PI/180)*subjectForearmLen;
    	double deltaZ = (1-Math.cos(20*Math.PI/180))*subjectForearmLen;
    	
    	
    	
    	//Frame destination = 
    	
    	//ObjectFrame flangeFrame = _lbr.getFlange().copyWithRedundancy();
    	
     //ObjectFrame desiredFrame = flangeFrame.transform(Transformation.ofTranslation(0,0, 175)); // adding a Frame on Needle TCP.
     	//desiredFrame.(World.Current.getRootFrame());//use this frame as destination point for path
     	//desiredFrame
    	//Frame desired1 = _lbr.getFlange().copyWithRedundancy();
    	//double rotDistance = desired1.rotationalDistanceTo(_lbr.getRootFrame());
    	//getLogger().error(String.valueOf(rotDistance));
    	//Frame desiredFrame = currentPoseOfTool;//.transform(World.Current.getRootFrame(),Transformation.ofRad(0, 0, 0, 0, 0, 0)); // adding a Frame on Needle TCP. ;
    	//desiredFrame.setParent(_lbr.getRootFrame());
    	//desiredFrame = currentPoseOfTool.transform(World.Current.getRootFrame(),Transformation.ofRad(0, 0, 0, 0, 0, 0)); // adding a Frame on Needle TCP.
     	//getLogger().info(desiredFrame.toString());
     	
     	
     	//getLogger().info(desiredFrame.toString());
     	//_toolAttachedToLBR.move(ptp(desiredFrame).setJointVelocityRel(0.1));
     	//ThreadUtil.milliSleep(1000);
     	// desiredFrame = currentPoseOfTool.transform(World.Current.getRootFrame(),Transformation.ofTranslation(100,0,0 )); // adding a Frame on Needle TCP.
     	//_toolAttachedToLBR.move(ptp(desiredFrame).setJointVelocityRel(0.1));
     	//ThreadUtil.milliSleep(1000);
     	//desiredFrame = currentPoseOfTool.transform(Transformation.ofTranslation(100,0,0 )); // adding a Frame on Needle TCP.
     	
    	Frame desiredFrame = _lbr.getFlange().copy();
     	Frame desired1 = desiredFrame.transform(Transformation.ofTranslation(0,0, 170)); // adding a Frame on Needle TCP.
        desired1.setParent(_lbr.getRootFrame());
        
        Transformation bTn = desired1.transformationFromWorld();
        bTn = Transformation.of(bTn.withTranslation(currentPoseOfTool.getX() + 0, currentPoseOfTool.getY(), currentPoseOfTool.getZ()+100));
        desired1.setTransformationFromParent(bTn);
    	getLogger().info(desired1.toString());
     	
     	
  
     	_toolAttachedToLBR.move(ptp(desired1).setJointVelocityRel(0.1));
     	
     	
      
        
        
      //  ThreadUtil.milliSleep(2000);
        //_toolAttachedToLBR.move(linRel(Transformation.ofDeg(0, 0, 500, 0, 0, 0),World.Current.getRootFrame()));
        /* Note: The Validation itself justifies, that in this very time instance, the load parameter setting was
         * sufficient. This does not mean by far, that the parameter setting is valid in the sequel or lifetime of this
         * program */
    
    }

    @Override
    public void run()
    {
    	//moveToInitialPosition();
    	DecimalFormat df=new DecimalFormat("0.000");
		ForceSensorData extForceTorquestr = _lbr.getExternalForceTorque(_lbr.getFlange(),World.Current.getRootFrame());
			getLogger().error(extForceTorquestr.toString());
        command = udp.readRawDatabyte();
        getLogger().info("Waiting for Start command...");
        while (command[0] == -1){//wait until recieve command '1' from PC
        	command = udp.readRawDatabyte();
        }
        
        String elbowAngle = "" ;
        for ( int i =0; i < command.length ; i++ )
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
        
        long startLoopTime = System.currentTimeMillis() ;
    	long currentLoopTime = 0;
    	long totalLoopTime =0;
    	getLogger().info("Loop Started...");
    	mediaflange.setLEDBlue(true);
    	command = udp.readRawDatabyte();
        while (command[0] != '3' && totalLoopTime < 15 )
        {
        	command = udp.readRawDatabyte(); 
        	
        	if (command[0] == '2')
        	{
        		startLoopTime = System.currentTimeMillis() ;
        		 extForceTorquestr = _lbr.getExternalForceTorque(_lbr.getFlange(),World.Current.getRootFrame());
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
        	}*/
        }
        mediaflange.setLEDBlue(false);
        getLogger().info("END loop");
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
        ForCeTorqueMonitor app = new
                ForCeTorqueMonitor();
        app.runApplication();
    }
}
