# compliant-emg-driven
An EMG-driven KUKA LBR iiwa14 robot for tele-operated Ultra Sound Imaging setup
# Overview:
In this project, an operator controls a robot manipulator equipped with an Ultra-sound imaging probe from a remote location.
He/she wears a MyoArmband EMG sensor on their arm which is used as an input to the Neural Networks to estimate the force to be applied by the robot for better US examination. 
On the other side, robot excerts the estimated force of the human operator on the patient (here, a phantom) while being compliant. In US imaging sessios, there might be a sudden need for the patient under examination, or the nurse present at site to take controll of the robot. Meaning, robot has to be able to detect and react ro human guidance and become compliant.

# disclaimer
* Codes are by no means optimized
* Must not be used as any form on any actual setup

# What is the Interface:
It is the main code to recieve commands from human at the master side and then command the robot on remote side. Therefore, this code contains multiple parts as bellow:
1- Read MyoArmband raw EMG data.
2- Realtime filtering data and then running the LSTM/ANN to estimate force.
3- Read joystick data containing an xSense IMU for probe heading and operator PC mouse for US probe location on patient body.
4- Transmit probe heading adn position along with desired force over UDP to KUKA robot

# What is the KUKA:
This code is the JAVA code running on KUKA station. Its job is to execute force and probe pose commands recieved from master, and also detect human intervention and to become compliant. 

# What is the Training:]
Matlab codes that were used to analyse datasets, train LSTM/ANN models and then generate c++ code.


