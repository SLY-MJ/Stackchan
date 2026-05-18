#pragma once

enum FaceMode {
    FACE_NORMAL,
    FACE_HAPPY,
    FACE_BLINK,
    FACE_LOOK_LEFT,
    FACE_LOOK_RIGHT
};

void initFace();
void setFaceMode(FaceMode mode);
void updateFace();

