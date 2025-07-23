#include "settings.hpp"

Settings::Settings() {
    _state = State::Menu;
	_master_volume = 1.;
	_music_volume = 0.5;
	_effect_volume = 1.;
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

float Settings::get_master_volume() { return _master_volume; };
float Settings::get_music_volume() { return _music_volume; };
float Settings::get_effect_volume() { return _effect_volume; };

void Settings::set_master_volume(float volume) { _master_volume = volume; };
void Settings::set_music_volume(float volume) { _music_volume = volume; };
void Settings::set_effect_volume(float volume) { _effect_volume = volume; };
