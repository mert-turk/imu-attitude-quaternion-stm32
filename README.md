\^# MY YOUTUBE CHANNEL LINK:https://www.youtube.com/@MertTurkk

\# imu_attitude_quaternion_stm32

A simple and efficient implementation of quaternion-based orientation
tracking using the \*\*MPU6050\*\* IMU sensor and \*\*STM32F4\*\*
microcontroller. This project also includes a real-time visualization
tool using \*\*MATLAB\*\* to plot the resulting Euler angles (roll,
pitch, yaw) derived from the quaternion.

\## Features

\- Read raw gyro and accelerometer data from MPU6050 - Compute
orientation using quaternion integration - Convert quaternion to Euler
angles - Visualize roll, pitch, and yaw in real-time using MATLAB

\## Repository Structure
imu_attitude_quaternion_stm32/ 
├── .vscode/ # VSCode config files
├── Core/
├── Drivers/ 
├── matlabplot/ # MATLAB script for real-time plotting 
    └── realtime_plot.m 
├── quaternion/ # Quaternion calculation source files 
├── LICENSE # MIT License 
├── Makefile # Makefile for building
├── quaternion_dsp.ioc # STM32CubeMX project file 
├── README.md  
├── startup_stm32f411xe.s # Startup assembly file 
├── stm32f411retx_flash.ld # Linker script

\## How It Works

\- \*\*Gyroscope\*\* data is used to incrementally update the
orientation quaternion. - Accelerometer data may be used in the future
for sensor fusion (e.g. with complementary or Kalman filter). - The
quaternion is converted to Euler angles using standard rotation
mathematics. - MATLAB reads the serial output from STM32 and displays
the roll, pitch, and yaw angles in real-time.

\## MATLAB Visualization

The script \`matlabplot/realtime_attitude_plot.m\` connects to the STM32
over a serial port and plots the orientation live.

\## Requirements

\- STM32F4 Board (e.g. STM32F411RE) - MPU6050 IMU sensor - MATLAB
(tested on R2023b) - USB-to-Serial connection (for MATLAB to read STM32
output)

\## License

This project is licensed under the \[MIT License\](./LICENSE). You are
free to use, modify, and distribute it for personal or commercial use.
# imu-attitude-quaternion-stm32

