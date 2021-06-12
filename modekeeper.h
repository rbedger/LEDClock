#include <Arduino.h>

#ifndef __MODEKEEPER_H__
#define __MODEKEEPER_H__

#define ONE_MINUTE_MS 60000

class Modekeeper {
public:
    enum class Mode {
        Clock,
        SingleColor,
        Noise,
        Swirl,
    };

    Modekeeper(Mode defaultMode);
    void SetMode(Mode mode);
    void NextMode();
    Mode GetMode();

private:
    Mode defaultMode;
    Mode currentMode;
};

#endif //__MODEKEEPER_H__

