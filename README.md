# ECEN2440_Final_Project

For the final project, we designed a bluetooth controlled battle robot using two MSP432 Launchpad’s, with one acting as a controller
and the other acting as a receiver/interpreter. The controller utilizes the Launchpad’s booster pack which has an onboard accelerometer
and two buttons along with many other peripherals, while the receiver was comprised of a single Launchpad fitted with a bluetooth module.
In order to provide some versatility, the robot was designed with two separate functionalities: an interactive control mode and a high
precision command mode. In control mode, the user tilts the controller to drive the robot and uses the buttons to activate the robot’s
three attacks. Each direction of motion has both a slow and fast speed which is determined based on how far the user is tilting the
controller; less tilt corresponds with slower motion while a larger tilt corresponds with faster motion. The 9 motions are stopped,
forward slow, forward fast, right slow, right fast, left slow, left fast, backwards slow, and backwards fast. The control mode attacks
are a melee slash, a ranged nerf gun fire, and a special spin attack move. In command mode, the user inputs a series of commands into a
terminal on the computer, which are queued on the robot until a start command is sent. At that point, the robot carries out the entire
sequence. Commands can either be for motion - a direction coupled with the desired distance for forward and backward motion or the desired
angle for turning, or attacking - swinging the sword and shooting the nerf gun. Our robot also implements a health system, in which robot
health is indicated by an LED on the handheld MSP that decrements with health from green to yellow, from yellow to red, and from red to
off (at which point the robot stops all motion and becomes unresponsive). Every 15 seconds, one health is regenerated. The special attack
in control mode costs 1 health, and every bumper hit costs 1 health. Each bumper required hardware and software debouncing. We also
utilized various voltage regulators to manage large amounts of current (up to 2A) at different voltages (3.3, 5, and 10V). Lastly, we
designed our own PCB with all previous courses circuitry (except for the H-bridges, which were too large to include with their heat sinks).
