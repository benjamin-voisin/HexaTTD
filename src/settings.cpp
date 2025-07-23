#include "settings.hpp"

Settings::Settings() {
    _state = State::Menu;
#ifdef NDEBUG
    _is_debug = false;
#else
    _is_debug = true;
#endif
}

void Settings::toggle_debug() { _is_debug = !_is_debug; }

bool Settings::is_debug() { return _is_debug; };

State Settings::get_state() { return _state; };

void Settings::set_state(State state) { _state = state; };
