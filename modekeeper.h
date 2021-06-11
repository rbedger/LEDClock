#include <Arduino.h>

#ifndef __MODEKEEPER_H__
#define __MODEKEEPER_H__

#define ONE_MINUTE_MS 60000

class Modekeeper {
public:
    enum class Mode {
        CLOCK,
        SINGLECOLOR,
        NOISE,
        SWIRL,
    };

    Modekeeper(Mode default_mode);
    void setMode(Mode mode);
    void nextMode();
    void stamp();
    Mode getMode();

private:
    Mode _default_mode;
    Mode _current_mode;
    unsigned long _last_stamp = 0;
};

#endif //__MODEKEEPER_H__

