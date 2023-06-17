# PIC16F877A_Bipolar_Stepper
# Created on 06/16/2023
  As we know, step motor is one of the most importatnt actuator in various applications, due to their ease and high precision in control. There are two main kind of step motors: unipolar (e.g BYJ-48) and bipolar, where unipolar stepper is mostly used for domestic applications such as disk recorder or camera rotation, meanwhile, bipolar stepper is used in many industrial tool machines that require high precision in position control such as 3D printers or CNC machines.
  
  In this project, I will use MCU PIC16F877A, a very common 8-bit MCU to control the KV4234-F2B009 step motor, which according to Nidec manufacturer, has step resolution of 1.8 degree, and frequency of pulse ranges from 1500-10000 pps. Besides, to control any kind of motors using MCU, we will need a motor driver, in this case is the A4988 stepper driver, which is used widely in the industry. Last but not least, an UART-TTL IC FT232RL is used for the MCU to communicate with PC, so that I can send command and receive feedback from the MCU.
  
  The aim of this project is only to reasearch the ability of controlling the speed, as well as the position of a bipolar step motor, without any application purpose. During the project, I will use my computer to send the command through UART module to control the speed and position of the step motor. Besides, I use the external interrupt module with a press button to stop the motor immediately.
  
  Last but not least, I have found out that we can not use PWM to control the Step pin of the A4988 driver, but we have to use GPIO, and we must pay huge attention to the UART connection as it is the biggest source of problem.
  
![Step motor circuit](https://github.com/HarryNguyen2023/PIC16F877A_Bipolar_Stepper/assets/136590151/b844a209-e317-4e69-93b4-f9ef4129d30d)
