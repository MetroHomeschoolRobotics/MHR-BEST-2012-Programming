#pragma config(Motor,  port2,           climber,       tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port3,           turret,        tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port4,           servturret,    tmotorServoStandard, openLoop)
#pragma config(Motor,  port5,           claw1,         tmotorServoStandard, openLoop)
#pragma config(Motor,  port6,           claw2,         tmotorServoStandard, openLoop)
#pragma config(Motor,  port8,           slidestop,     tmotorServoStandard, openLoop)
#pragma config(Motor,  port9,           arm,           tmotorServoContinuousRotation, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*                                             *\
| Controls:
|   Climb: Up/Down on Right Joystick
|   Cruise Control™: 7L
|   Brake Slider: 7D in/out
\*                                             */

bool autoClimb = false;
int rightX = 0;
int rightY = 0;
int leftX = 0;
int leftY = 0;
int climbSpeed;
int climbShift;
int clawPos;



task lift() {
	while(true) {
		climbSpeed = rightY + climbShift;
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
		if(vexRT[Btn8L]) {
			clawPos++;
			wait1Msec(2);
			} else	if(vexRT[Btn8R]) {
			clawPos = -127;
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
