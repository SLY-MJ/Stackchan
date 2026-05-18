#pragma once

enum ServoAction {
    SERVO_ACTION_RESET,
    SERVO_ACTION_NOD,
    SERVO_ACTION_SHAKE
};

void initServo();
void setServoAction(ServoAction action);
void updateServo();

