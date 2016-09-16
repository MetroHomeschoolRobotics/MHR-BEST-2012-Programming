#pragma config(Motor,  port2,           climber,       tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port3,           stretch,       tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port4,           slidestop,     tmotorServoStandard, openLoop)
#pragma config(Motor,  port5,           turret,        tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port7,           claw1,         tmotorServoStandard, openLoop)
#pragma config(Motor,  port8,           claw2,         tmotorServoStandard, openLoop)
#pragma config(Motor,  port9,           arm,           tmotorServoContinuousRotation, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*                                             *\
| Controls:
|   Climb: Up/Down on Right Joystick
|   Cruise Control™: 7L - Sets speed adjustment to position of right joystick, waits 400ms and then sets motor speed to cruise + joystick position
|   Reset Cruise: 7R
|   6U + left joystick up/down controls arm motor (up/dn)
|   6D + left joystick left/right controls arm swing (servo)
|   8R opens claw (claw1, claw2) fully, 8L slowly closes it while being held, holds position when you release 8L
\*                                             */

bool autoClimb = false;
int rightX = 0;
int rightY = 0;
int leftX = 0;
int leftY = 0;
int climbSpeed;
int climbShift;
int clawPos = -90;
int turretSpeed = 0;
int turretSpeedSq = 0;
int armSpeed = 0;
int armSpeedSq = 0;
int armSpeedAdj = 7;



task lift() {
	while(true) {
		if(abs(rightY) > 10)
			climbSpeed = rightY + climbShift;
		else
			climbSpeed = climbShift;
		if(!autoClimb) {
				motor[climber] = climbSpeed;
		}
		if(vexRT[Btn7R]) {
			climbShift = 0;
		}
		if(vexRT[Btn7L]) {
			while(vexRT[Btn7L]) {
				climbShift = rightY;
			}
			climbSpeed = climbShift;
			motor[climber] = climbSpeed;
			wait1Msec(400);
		}
	}
}

task manipulation() {
	while(true) {
		if(vexRT[Btn6D]) { //Arm swivel
			turretSpeed = leftX;
		} else {
			turretSpeed = 0;
		}
		turretSpeedSq = (((turretSpeed)*(turretSpeed))/127);
		if(leftX < 0)
			turretSpeedSq = -turretSpeedSq;

		if(vexRT[Btn6U]) { // Arm up/down
			armSpeed = leftY;
		} else {
			armSpeed = 0;
		}
		armSpeedSq = (((armSpeed)*(armSpeed))/127);
		if(leftY < 0)
			armSpeedSq = -armSpeedSq;


		if(vexRT[Btn8L]) {  // Slow Claw Close
			if(clawPos < 127) {
				clawPos++;
				wait1Msec(1);
			}
		} else if(vexRT[Btn8R]) {  // Claw Open
			if(clawPos > -90) {
				clawPos = (clawPos - 1);
				wait1Msec(1);
			}
		}
		if(vexRT[Btn5U]) {
			motor[slidestop] = 127;
		} else if(vexRT[Btn5D]) {
			motor[slidestop] = -127;
		}
		if(vexRT[Btn5U]) {
			motor[stretch] = 127;
		} else if(vexRT[Btn5D]) {
			motor[stretch] = -127;
		} else{
			motor[stretch] = 0;
		}
		motor[turret] = ((turretSpeedSq)*0.75);
		motor[arm] = armSpeedSq + armSpeedAdj;
		motor[claw1] = clawPos;
		motor[claw2] = -clawPos;
	}
}


task main() {
	StartTask(lift);
	StartTask(manipulation);
	bool running = true;
	while(running) {
		rightX = vexRT[Ch1];
		rightY = vexRT[Ch2];
		leftX = -vexRT[Ch4];
		leftY = vexRT[Ch3];
	}
}
