#include "modekeeper.h"

Modekeeper::Modekeeper(Mode default_mode) : _default_mode(default_mode), _current_mode(default_mode) {}

void Modekeeper::setMode(Mode mode) {
    if (_current_mode != mode) {
        emitEvent(Modekeeper::Event::RESET);
    }
    _current_mode = mode;
}

void Modekeeper::emitEvent(Event event) { _latest_event = event; }

void Modekeeper::stamp() { _last_stamp = millis() + ONE_MINUTE_MS; }

Modekeeper::Mode Modekeeper::getMode() {
    if (_last_stamp != 0 && millis() > _last_stamp) {
        _current_mode = _default_mode;
        _last_stamp = 0;
    }

    return _current_mode;
}

void Modekeeper::nextMode() {
    int nextModeInt = ((int)_current_mode) + 1;
    nextModeInt %= 4;
    _current_mode = (Mode)nextModeInt;
}

Modekeeper::Event Modekeeper::getEvent() {
    auto event = _latest_event;
    _latest_event = Modekeeper::Event::NONE;
    return event;
}
