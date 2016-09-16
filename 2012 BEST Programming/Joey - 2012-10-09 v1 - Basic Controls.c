#pragma config(Motor,  port2,           climber,       tmotorServoContinuousRotation, openLoop)
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
int clawPos = -127;



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
				climbShift = vexRT[Ch2];
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
			motor[turret] = -vexRT[Ch4];
		} /*else {
		motor[turret] = 0;
		}*/
		if(vexRT[Btn6U]) { // Arm up/down
			motor[arm] = vexRT[Ch3];
			} else {
			motor[arm] = 0;
		}
		if(vexRT[Btn8L]) {  // Slow Claw Close
			if(clawPos < 127) {
				clawPos++;
				wait1Msec(1);
			}
		} else	if(vexRT[Btn8R]) {  // Claw Open
			if(clawPos > -127) {
				clawPos = (clawPos - 1);
				wait1Msec(1);
			}
		}
		if(vexRT[Btn5U]) {
			motor[slidestop] = 127;
			} else if(vexRT[Btn5D]) {
			motor[slidestop] = -127;
		}
		motor[claw1] = clawPos;
		motor[claw2] = -clawPos;
	}
}

task autoFunctions() {

}

task main() {
	motor[slidestop] = -127;
	StartTask(lift);
	StartTask(manipulation);
	StartTask(autoFunctions);
	bool running = true;
	while(running) {
		//Still Alive!
		rightX = vexRT[Ch1];
		rightY = vexRT[Ch2];
		leftX = vexRT[Ch4];
		leftY = vexRT[Ch3];
	}
}
