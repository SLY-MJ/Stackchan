#include "servo.h"
#include <ESP32Servo.h>
#include "config.h"

Servo panServo;
Servo tiltServo;

static ServoAction currentAction = SERVO_ACTION_RESET;
static unsigned long actionStartTime = 0;
static bool actionInProgress = false;

void initServo() {
    // CoreS3 might need specific timer allocation, ESP32Servo handles this mostly
    panServo.attach(SERVO_PAN_PIN, 500, 2400);
    tiltServo.attach(SERVO_TILT_PIN, 500, 2400);

    panServo.write(90);
    tiltServo.write(90);
}

void setServoAction(ServoAction action) {
    currentAction = action;
    actionStartTime = millis();
    actionInProgress = true;
}

void updateServo() {
    if (!actionInProgress) return;

    unsigned long elapsed = millis() - actionStartTime;

    switch (currentAction) {
        case SERVO_ACTION_RESET:
            panServo.write(90);
            tiltServo.write(90);
            actionInProgress = false;
            break;

        case SERVO_ACTION_NOD:
            if (elapsed < 300) {
                tiltServo.write(110);
            } else if (elapsed < 600) {
                tiltServo.write(70);
            } else if (elapsed < 900) {
                tiltServo.write(90);
            } else {
                actionInProgress = false;
            }
            break;

        case SERVO_ACTION_SHAKE:
            if (elapsed < 300) {
                panServo.write(110);
            } else if (elapsed < 600) {
                panServo.write(70);
            } else if (elapsed < 900) {
                panServo.write(90);
            } else {
                actionInProgress = false;
            }
            break;
    }
}

