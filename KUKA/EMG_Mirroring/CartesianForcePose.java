package EMG_Mirroring;


import javax.inject.Inject;
import javax.inject.Named;

import sun.security.action.GetLongAction;

import com.kuka.generated.ioAccess.MediaFlangeIOGroup;
import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;
import static com.kuka.roboticsAPI.motionModel.BasicMotions.*;

import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.geometricModel.Frame;
import com.kuka.roboticsAPI.geometricModel.LoadData;
import com.kuka.roboticsAPI.geometricModel.ObjectFrame;
import com.kuka.roboticsAPI.geometricModel.Tool;
import com.kuka.roboticsAPI.geometricModel.World;
import com.kuka.roboticsAPI.geometricModel.math.IRotation;
import com.kuka.roboticsAPI.geometricModel.math.Rotation;
import com.kuka.roboticsAPI.geometricModel.math.Transformation;
import com.kuka.roboticsAPI.geometricModel.math.Vector;
import com.kuka.roboticsAPI.geometricModel.math.XyzAbcTransformation;
import com.kuka.roboticsAPI.sensorModel.ForceSensorData;
import com.kuka.common.ThreadUtil;
import com.kuka.connectivity.motionModel.smartServo.ISmartServoRuntime;
import com.kuka.connectivity.motionModel.smartServo.SmartServo;
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
public class CartesianForcePose extends RoboticsAPIApplication {
	@Inject
	private LBR _lbr;
	private LoadData _loadData;
    @Inject
    @Named("Cylinder")
    private Tool tool;
    @Inject
    private MediaFlangeIOGroup MediaHS;
    double initialPosition[] = {84,495, 300};
    //double[] desiredCartesianPose = new double[3];
    
    
   
    private int steps = 0;
    
	@Override
	public void initialize() {
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
        
	}

	private void MoveToInitialPosition()
	{
		Frame flange = _lbr.getFlange().copyWithRedundancy();
		Vector desiredPosition = Vector.of(initialPosition[0],initialPosition[1],initialPosition[2]);
    	IRotation desiredOrientation = Rotation.ofRad(-90*3.14/180, 180*3.14/180, 0);
	
    	Frame desiredFrame = flange.transform(Transformation.ofTranslation(0,0, 175)); // adding a Frame on tool TCP.
     	desiredFrame.setParent(_lbr.getRootFrame());
        Transformation bTn = desiredFrame.transformationFromWorld();//tool center point coordinate(transformation) w.r.t. World
	
    	bTn = Transformation.of(bTn.withTranslation(desiredPosition).getTranslation(), 
    			bTn.withRotation(desiredOrientation).getRotationMatrix());
    
    	desiredFrame.setTransformationFromParent(bTn);
    
    	//getLogger().info(String.valueOf(deltaForceZ) + "  " + String.valueOf(pc));
    	//getLogger().error(desiredFrame.toString());
    	tool.move(ptp(desiredFrame).setJointVelocityRel(0.2));
	}
	
	
	@Override
	public void run() {
		MoveToInitialPosition();
		
		SmartServo aSmartServoMotion = new SmartServo(
	            _lbr.getCurrentJointPosition());
		aSmartServoMotion.setJointAccelerationRel(0.2);
	    aSmartServoMotion.setJointVelocityRel(0.2);
	    aSmartServoMotion.setMinimumTrajectoryExecutionTime(5e-3);
	    
	    
	    getLogger().info("Starting SmartServo motion in position control mode");
        tool.moveAsync(aSmartServoMotion);//here binds move method to our SmartServo motion
        getLogger().info("Get the runtime of the SmartServo motion");
        ISmartServoRuntime theServoRuntime = aSmartServoMotion//after binding we use this to command position in realtime
                .getRuntime();
		// your application execution starts here
		//_lbr.move(ptpHome());
		double kaf = 1;
		double kvf = 0.9;
		double vcDot = 0;// pcDotDot = vcDot;
		double vc = 0;//pcDot = vc;
		
		
		
		double pc =0;// position computed by force controller
		double pr = 300;// reference position by visual or encoder or ...
		double desiredCartesianPose0 = 0;//x
		double desiredCartesianPose1 = 0;//y
		double desiredCartesianPose2 = 0;//z
		
		Frame currentPoseOfTool = _lbr.getCurrentCartesianPosition(tool.getDefaultMotionFrame());// position of tool
    	getLogger().info(currentPoseOfTool.toString());
    	Frame flange = _lbr.getFlange().copyWithRedundancy();
     	
        ThreadUtil.milliSleep(500);
        ForceSensorData initialExtForceTorques = _lbr.getExternalForceTorque(_lbr.getFlange(),World.Current.getRootFrame());
        getLogger().info(initialExtForceTorques.toString());
        ThreadUtil.milliSleep(500);
        
        long starttime;
        long endtime;
        for (steps =0; steps< 1000 ; steps++)
        {
        	starttime = System.currentTimeMillis();
            //currentPoseOfTool = _lbr.getCurrentCartesianPosition(_lbr.getFlange().getChild(tool.getName()));// position of tool
        	theServoRuntime.updateWithRealtimeSystem();
        	
        	currentPoseOfTool = theServoRuntime.getCurrentCartesianPosition(tool.getDefaultMotionFrame());
            //currentPoseOfTool = _lbr.getCurrentCartesianPosition(tool.getDefaultMotionFrame());// position of tool
            //
            // currentPoseOfTool.getZ();
        	//forces = theServoRuntime.getExtForceMsr();
        	
            ForceSensorData currentExtForceTorques = _lbr.getExternalForceTorque(_lbr.getFlange(),World.Current.getRootFrame());
        	//getLogger().info(forces.toString()+"\n"+
        	//		String.valueOf(currentExtForceTorques));
            double desireForceZ = -20;
        	double deltaForceZ = -desireForceZ + (currentExtForceTorques.getForce().getZ() - initialExtForceTorques.getForce().getZ());
        	if (Math.abs(deltaForceZ) < 1)
        	{
        		//getLogger().error("done");
        		getLogger().info(String.valueOf(deltaForceZ));
        		//pc=0;
        		//break;
        	}
        	
        	double deltaT = 0.015;
        	vcDot = (deltaForceZ - kvf*vc)/kaf;
        	vc = vc + vcDot*deltaT;
        	
        	pc = pc + vc*deltaT;
        	
        	desiredCartesianPose0 = initialPosition[0];
        	desiredCartesianPose1 = initialPosition[1];
        	desiredCartesianPose2 =   pc + pr;
        	
    	//desired Transformation p and R
        	Vector desiredPosition = Vector.of(desiredCartesianPose0,desiredCartesianPose1, desiredCartesianPose2);
        	IRotation desiredOrientation = Rotation.ofRad(-90*3.14/180, 180*3.14/180, 0);
		
        	Frame desiredFrame = flange.transform(Transformation.ofTranslation(0,0, 175)); // adding a Frame on tool TCP.
         	desiredFrame.setParent(_lbr.getRootFrame());
            Transformation bTn = desiredFrame.transformationFromWorld();//tool center point coordinate(transformation) w.r.t. World
		
        	bTn = Transformation.of(bTn.withTranslation(desiredPosition).getTranslation(), 
        			bTn.withRotation(desiredOrientation).getRotationMatrix());
        
        	desiredFrame.setTransformationFromParent(bTn);
        
        	 
        	//getLogger().info(String.valueOf(deltaForceZ) + "  " + String.valueOf(pc));
        	//getLogger().error(desiredFrame.toString());
        	//tool.moveAsync(ptp(desiredFrame).setJointVelocityRel(0.5));
        	 theServoRuntime.setDestination(desiredFrame);
        	endtime = System.currentTimeMillis();
        	ThreadUtil.milliSleep((long)(deltaT*1000)- ( starttime - endtime));
        }
        getLogger().error("done");
	}
}