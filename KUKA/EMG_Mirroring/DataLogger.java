package EMG_Mirroring;


import java.util.concurrent.TimeUnit;
import com.kuka.common.ThreadUtil;

import com.kuka.roboticsAPI.applicationModel.RoboticsAPIApplication;
import static com.kuka.roboticsAPI.motionModel.BasicMotions.*;
import com.kuka.roboticsAPI.deviceModel.LBR;
import com.kuka.roboticsAPI.geometricModel.AbstractFrame;
import com.kuka.roboticsAPI.motionModel.PTP;
import com.kuka.roboticsAPI.sensorModel.DataRecorder;
import com.kuka.roboticsAPI.sensorModel.DataRecorder.AngleUnit;
import com.kuka.roboticsAPI.uiModel.ApplicationDialogType;

public class DataLogger extends RoboticsAPIApplication {
	private LBR lbr;
	private final static String askForExit =
         "Exit Program??"+ "\n"; 
	
	 
	public void initialize() {
		lbr = getContext().getDeviceFromType(LBR.class);
	}

	public void run() {

		getLogger().info("Move to Start position");
		PTP ptpStartPosition = ptp(Math.toRadians(90), Math.toRadians(0), 0, Math.toRadians(90), 0, Math.toRadians(90), 0);
		ptpStartPosition.setJointVelocityRel(0.55);
		lbr.move(ptpStartPosition);
		
		DataRecorder rec = new DataRecorder("DataLoggerTest5forcartforc.log", -1,	TimeUnit.MILLISECONDS, 1000);
		//rec.addCurrentJointPosition(lbr, AngleUnit.Radian);
		//rec.addExternalJointTorque(lbr);
		rec.addCartesianForce(lbr.getFlange(),lbr.getRootFrame());
		rec.enable();
		ThreadUtil.milliSleep(1000);
		getLogger().info("Start recording...");
    	rec.startRecording();
    	
    	int isCancel = getApplicationUI().displayModalDialog(ApplicationDialogType.QUESTION, askForExit, "OK", "Cancel");
        rec.stopRecording();
        
	}

}
