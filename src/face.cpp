#include "face.h"
#include <M5Unified.h>
#include "config.h"

static FaceMode currentMode = FACE_NORMAL;
static unsigned long lastUpdate = 0;
static bool blinkState = false;

void initFace() {
    M5.Lcd.setBrightness(255); // Max brightness for better visibility
    M5.Lcd.fillScreen(COLOR_BG);
    setFaceMode(FACE_NORMAL);
}

void drawEye(int x, int y, int w, int h) {
    M5.Lcd.fillRoundRect(x - w/2, y - h/2, w, h, min(w, h)/4, COLOR_FG);
}

void drawMouth(int x, int y, int w, int h) {
    M5.Lcd.fillRoundRect(x - w/2, y - h/2, w, h, h/2, COLOR_FG);
}

void renderFace() {
    M5.Lcd.fillScreen(COLOR_BG);
    int cx = M5.Lcd.width() / 2;
    int cy = M5.Lcd.height() / 2;

    int eyeY = cy - 30;
    int mouthY = cy + 40;

    switch (currentMode) {
        case FACE_NORMAL:
            drawEye(cx - 50, eyeY, 30, 40);
            drawEye(cx + 50, eyeY, 30, 40);
            drawMouth(cx, mouthY, 60, 10);
            break;
        case FACE_HAPPY:
            // Arc or smaller eyes
            drawEye(cx - 50, eyeY, 30, 10);
            drawEye(cx + 50, eyeY, 30, 10);
            M5.Lcd.fillCircle(cx, mouthY, 20, COLOR_FG);
            M5.Lcd.fillCircle(cx, mouthY - 10, 20, COLOR_BG); // Cutout to make smile
            break;
        case FACE_BLINK:
            if (blinkState) {
                drawEye(cx - 50, eyeY, 30, 5);
                drawEye(cx + 50, eyeY, 30, 5);
            } else {
                drawEye(cx - 50, eyeY, 30, 40);
                drawEye(cx + 50, eyeY, 30, 40);
            }
            drawMouth(cx, mouthY, 60, 10);
            break;
        case FACE_LOOK_LEFT:
            drawEye(cx - 60, eyeY, 20, 40);
            drawEye(cx + 40, eyeY, 20, 40);
            drawMouth(cx - 10, mouthY, 50, 10);
            break;
        case FACE_LOOK_RIGHT:
            drawEye(cx - 40, eyeY, 20, 40);
            drawEye(cx + 60, eyeY, 20, 40);
            drawMouth(cx + 10, mouthY, 50, 10);
            break;
    }
}

void setFaceMode(FaceMode mode) {
    currentMode = mode;
    renderFace();
}

void updateFace() {
    if (currentMode == FACE_BLINK) {
        if (millis() - lastUpdate > (blinkState ? 200 : 3000)) {
            blinkState = !blinkState;
            renderFace();
            lastUpdate = millis();
        }
    }
}
