package EMG_Mirroring;



import com.kuka.common.ThreadUtil;
import java.io.FileOutputStream;
import java.io.RandomAccessFile;
import java.util.concurrent.TimeUnit;

import javax.inject.Inject;

import com.kuka.common.StatisticTimer;
import com.kuka.common.ThreadUtil;
import com.kuka.common.StatisticTimer.OneTimeStep;
import com.kuka.connectivity.motionModel.smartServo.ISmartServoRuntime;
import com.kuka.connectivity.motionModel.smartServo.SmartServo;
import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;
import static com.kuka.roboticsAPI.motionModel.BasicMotions.*;

import com.kuka.roboticsAPI.deviceModel.JointPosition;
import com.kuka.roboticsAPI.deviceModel.LBR;
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
public class SimpleSineWave extends RoboticsAPIApplication {
	@Inject
	private LBR lBR;
	private ISmartServoRuntime _theSmartServoRuntime = null;
	
	 private int _count = 0;

	    private static final int MILLI_SLEEP_TO_EMULATE_COMPUTATIONAL_EFFORT = 70;
	    private static final int NUM_RUNS = 300;
	    private static final double AMPLITUDE = 1.0;
	    private static final double FREQENCY = 0.2;
	    private int _steps = 0;
	@Override
	public void initialize() {
		lBR = getContext().getDeviceFromType(LBR.class);
		moveToStartPosition();
	}

	@Override
	public void run() {
			
		
        DataRecorder rec=new DataRecorder("SimpleSineWave.log", -1,	TimeUnit.MILLISECONDS, 70);
       
        rec.addCurrentJointPosition(lBR, AngleUnit.Degree);
        rec.addCommandedJointPosition(lBR, AngleUnit.Degree);
        //rec.addExternalJointTorque(lBR);
        //rec.addInternalJointTorque(lBR);
        rec.enable();
       
			
		JointPosition initialPosition = new JointPosition(
                lBR.getCurrentJointPosition());
        SmartServo aSmartServoMotion = new SmartServo(initialPosition);
        aSmartServoMotion.setJointAccelerationRel(0.9);
        aSmartServoMotion.setJointVelocityRel(0.9);

        aSmartServoMotion.setMinimumTrajectoryExecutionTime(20e-3);
        getLogger().info("Starting SmartServo motion in position control mode");
        lBR.moveAsync(aSmartServoMotion);
        
        getLogger().info("Get the runtime of the SmartServo motion");
        _theSmartServoRuntime = aSmartServoMotion.getRuntime();
        JointPosition destination = initialPosition;
        getLogger().info("Start loop");
        
     // For Roundtrip time measurement...
        StatisticTimer timing = new StatisticTimer();
        rec.startRecording();
        try
        {
     // do a cyclic loop
        // Refer to some timing...
        // in nanosec
        double omega = FREQENCY * 2 * Math.PI * 1e-9;
        long startTimeStamp = System.nanoTime();
        for (_steps = 0; _steps < NUM_RUNS; ++_steps)
        {
            // Timing - draw one step
            OneTimeStep aStep = timing.newTimeStep();
            // ///////////////////////////////////////////////////////
            // Insert your code here
            // e.g Visual Servoing or the like
            // emulate some computational effort - or waiting for external
            // stuff
            

            
           ThreadUtil.milliSleep(MILLI_SLEEP_TO_EMULATE_COMPUTATIONAL_EFFORT);
            //_theSmartServoRuntime.updateWithRealtimeSystem();
            // Get the measured position
            JointPosition curMsrJntPose = _theSmartServoRuntime
                    .getAxisQMsrOnController();

            double curTime = System.nanoTime() - startTimeStamp;
            double sinArgument = omega * curTime;

            /*/for (int k = 0; k < destination.getAxisCount() ; ++k)
           // {
                destination.set(k, Math.sin(sinArgument)
                        * AMPLITUDE + initialPosition.get(k));
                if (k > 4)
                {
                    destination.set(k, initialPosition.get(k));
                }
            }*/
            double angle =  Math.sin(sinArgument) * AMPLITUDE;// + initialPosition.get(3);
            destination.set(3,angle);
            //getLogger().info(String.valueOf(angle));
            _theSmartServoRuntime.setDestination(destination);
            
            
            //getLogger().info(String.valueOf(initialPosition.get(3)));
            // ////////////////////////////////////////////////////////////
            aStep.end();

        }
    }
    catch (Exception e)
    {
        getLogger().info(e.getLocalizedMessage());
        e.printStackTrace();
    }
        rec.stopRecording();
    ThreadUtil.milliSleep(500);
		
	}
	
	public void moveToStartPosition(){
    	getLogger().info("Move to the mechanical zero position!");
		getLogger().info("Move to the Start position position!");
		PTP ptpPosition = ptp(Math.toRadians(-90), Math.toRadians(0), 0, Math.toRadians(0), 0, 0, 0);
		ptpPosition.setJointVelocityRel(0.3);
		lBR.move(ptpPosition);
    }
}