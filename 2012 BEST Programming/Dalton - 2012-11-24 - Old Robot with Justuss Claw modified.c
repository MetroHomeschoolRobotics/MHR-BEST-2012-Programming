#pragma config(Motor,  port2,           climber,       tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port4,           slidestop,     tmotorServoStandard, openLoop)
#pragma config(Motor,  port5,           turret,        tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port6,           claw,          tmotorServoStandard, openLoop)
#pragma config(Motor,  port7,           clawTwist,     tmotorServoStandard, openLoop)
#pragma config(Motor,  port8,           wrist,         tmotorServoStandard, openLoop)
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

bool rightPole = true;
bool autoClimb;
int rightX = 0;
int rightY = 0;
int leftX = 0;
int leftY = 0;
int climbSpeed;
int climbShift;
int clawPos = -127;
int twistPos = 127;
int wristPos = -127;
bool wristPosl = false;



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
			motor[turret] = leftX;
		} /*else {
		motor[turret] = 0;
		}*/
		motor[arm] = leftY;  // Arm up/down
		if(vexRT[Btn8L]) {  // Slow Claw Close
			if(clawPos < 127) {
				clawPos++;
				wait1Msec(1);
			}
		} else if(vexRT[Btn8R]) {  // Claw Open
			if(clawPos > -127) {
				clawPos = (clawPos - 1);
				wait1Msec(1);
			}
		}

		if(vexRT[Btn8U]) {  // Wrist twist
			if(twistPos < 127) {
				twistPos++;
				wait1Msec(1);
			}
		} else if(vexRT[Btn8D]) {  // Wrist twist the other way
			if(twistPos > -127) {
				twistPos = (twistPos - 1);
				wait1Msec(1);
			}
		}

		if(vexRT[Btn6U]) {
			if(wristPos > 0 && !wristPosl) {
				wristPos = -64;
				wristPosl =  true;
			}
			else if(wristPos < 0 && !wristPosl) {
				wristPos = 127;
				wristPosl =  true;
			}
		} else {
			wristPosl = false;
		}
		motor[claw] = clawPos;
		motor[clawTwist] = twistPos;
		motor[wrist] = wristPos;
	}
}
task main() {
	while(!(vexRT[Btn5U] || vexRT[Btn5D])) {} // Requires driver to set which pole the robot is on before allowing control.
	if(vexRT[Btn5U]) { // Sets controls for right pole (from driver's perspective)
		rightPole = true;
		climbShift = 0;
		}	else if(vexRT[Btn5D]) { // Sets controls for left pole (from driver's perspective)
		rightPole = false;
		climbShift = 0;
	}
	StartTask(lift);
	StartTask(manipulation);
	bool running = true;
	while(running) {
		if(rightPole) { // Right pole input flip
			rightX = vexRT[Ch1];
			rightY = vexRT[Ch2];
			leftX = -vexRT[Ch4];
			leftY = vexRT[Ch3];
			} else { // Left pole input flip
			rightX = -vexRT[Ch1];
			rightY = -vexRT[Ch2];
			leftX = vexRT[Ch4];
			leftY = -vexRT[Ch3];
		}
	}
}
