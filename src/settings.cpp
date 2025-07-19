#include "settings.hpp"

Settings::Settings() {
    state = State::Menu;
#ifdef NDEBUG
    _is_debug = false;
#else
    _is_debug = true;
#endif
}

void Settings::toggle_debug() { _is_debug = !_is_debug; }

bool Settings::is_debug() { return _is_debug; };
