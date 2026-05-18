#include "network.h"
#include <WiFi.h>
#include <WebServer.h>
#include "face.h"
#include "servo.h"
#include <M5Unified.h>

WebServer server(80);

void handleFace() {
    if (server.hasArg("mode")) {
        String mode = server.arg("mode");
        if (mode == "normal") setFaceMode(FACE_NORMAL);
        else if (mode == "happy") setFaceMode(FACE_HAPPY);
        else if (mode == "blink") setFaceMode(FACE_BLINK);
        else if (mode == "left") setFaceMode(FACE_LOOK_LEFT);
        else if (mode == "right") setFaceMode(FACE_LOOK_RIGHT);

        server.send(200, "text/plain", "Face updated");
    } else {
        server.send(400, "text/plain", "Missing mode param");
    }
}

void handleServo() {
    if (server.hasArg("action")) {
        String action = server.arg("action");
        if (action == "reset") setServoAction(SERVO_ACTION_RESET);
        else if (action == "nod") setServoAction(SERVO_ACTION_NOD);
        else if (action == "shake") setServoAction(SERVO_ACTION_SHAKE);

        server.send(200, "text/plain", "Servo updated");
    } else {
        server.send(400, "text/plain", "Missing action param");
    }
}

void initNetwork() {
    server.on("/face", HTTP_GET, handleFace);
    server.on("/servo", HTTP_GET, handleServo);
    server.begin();
}

void updateNetwork() {
    server.handleClient();
}

