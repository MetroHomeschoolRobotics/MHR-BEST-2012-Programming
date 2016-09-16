#pragma config(Motor,  port2,           lift,          tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port3,           armSwivel,     tmotorServoStandard, openLoop)
#pragma config(Motor,  port5,           claw1,         tmotorServoStandard, openLoop)
#pragma config(Motor,  port6,           claw2,         tmotorServoStandard, openLoop)
#pragma config(Motor,  port8,           lock,          tmotorServoStandard, openLoop)
#pragma config(Motor,  port9,           arm,           tmotorServoContinuousRotation, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task lift() {
	int rightY;
	int rightX;
	while(true) {
		rightY = vexRT[Ch2];
		rightX = vexRT[Ch1];
		motor[lift] = rightY;
	}
}

task manipulator() {
	int leftY;
	int leftX;
	while(true) {
		if(vexRT[Btn6U]) {
			motor[arm] = leftY;
		} else {
			motor[arm] = 0;
		}
	}
}

task main() {
	StartTask(lift);
	StartTask(manipulator);
	while(true) {}


}
