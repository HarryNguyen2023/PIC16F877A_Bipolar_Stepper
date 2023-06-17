# PIC16F877A_Bipolar_Stepper
# Created on 06/16/2023
  As we know, stepper motor is one of the most importatnt actuator in various applications, due to their ease and high precision in control. There are two main kind of stepper motors: unipolar (e.g BYJ-48) and bipolar, where unipolar stepper is mostly used for domestic applications such as disk recorder or camera rotation, meanwhile, bipolar stepper is used in many industrial tool machines that require high precision in position control such as 3D printers or CNC machines.
  
  In this project, I will use MCU PIC16F877A, a very common 8-bit MCU to control the KV4234-F2B009, which according to Nidec manufacturer, has step resolution of 1.8 degree, and frequency of pulse ranges from 1500-10000 pps. Besides, to control any kind of motors using MCU, we will need a motor driver, in this case is the A4988 stepper driver, which is used widely in the industry. Last but not least, an UART-TTL IC FT232RL is used for the MCU to communicate with PC, so that I can send command and receive feedback from the MCU.
  
