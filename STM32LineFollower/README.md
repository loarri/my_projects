
![logo](https://user-images.githubusercontent.com/13197186/217768057-796b55cd-327f-4c9a-a399-e0bea59943c1.PNG)

            STM32 Line Follower: a PID example 

![LineFollower_gif](https://user-images.githubusercontent.com/13197186/217767981-41ac1049-0579-415e-8168-93bd208dd49d.gif)

Hi all, here you can find the STM32LineFollower code: a PID example.
The platform used is the STM32duino and the aim of this project is to test different robot control algorithms. Through a custom application (done by to App Inventor) you can decide which of three algorithms to run and also control its parameters. The first demo has no PID control implemented so the machine will try to stay on the black line by continuing to turn left and right; in the second demo (my favorite) the STM32LineFollower will try to stay on the trajectory by varying the speed of the motors (simple PID, with only the KP parameter) while in the last test a full PID controller has been implemented with the KP parameters, KI and KD to tune.

More details can be found here:

while a short video here:

Good fun!!
