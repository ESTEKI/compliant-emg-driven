# compliant-emg-driven
An EMG-driven KUKA LBR iiwa14 robot for tele-operated Ultra Sound Imaging setup
Link to Video Demo: https://youtu.be/1YZYQPV0OIA
# Overview:
In this project, an operator controls a robot manipulator equipped with an Ultra-sound imaging probe from a remote location.
He/she wears a MyoArmband EMG sensor on their arm which is used as an input to the Neural Networks to estimate the force to be applied by the robot for better US examination. 
On the other hand, the robot exerts the estimated force of the human operator on the patient (here, a phantom) while being compliant. In US imaging sessions, there might be a sudden need for the patient under examination or the nurse present at the site to take control of the robot. This means that the robot has to be able to detect and react to human guidance and become compliant.

# disclaimer
* Codes are by no means optimized
* Must not be used in any form on any clinical setup without prior checks
  
# What is the Interface:
It is the main code to receive commands from humans on the master side and then command the robot on the remote side. Therefore, this code contains multiple parts as below:
1- Read MyoArmband raw EMG data.
2- Real-time filtering data and then running the LSTM/ANN to estimate force.
3- Read joystick data containing an xSense IMU for probe heading and operator PC mouse for US probe location on patient's body.
4- Transmit probe heading and position along with desired force over UDP to KUKA robot

# What is the KUKA/emg_mirroing:
This code is the JAVA code running on KUKA station. Its job is to execute force and probe pose commands recieved from master, and also detect human intervention and to become compliant. 

# What is the Training:
Matlab codes that were used to analyse datasets, train LSTM/ANN models and then generate c++ code.


