#include "modekeeper.h"

Modekeeper::Modekeeper(Mode defaultMode) : defaultMode(defaultMode), currentMode(defaultMode) {}

void Modekeeper::SetMode(Mode mode) {
    currentMode = mode;
}

Modekeeper::Mode Modekeeper::GetMode() {
    return currentMode;
}

void Modekeeper::NextMode() {
    int nextModeInt = ((int)currentMode) + 1;
    nextModeInt %= 4;
    currentMode = (Mode)nextModeInt;
}
