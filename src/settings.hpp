#pragma once

enum class State {
    Menu,
    Settings,
    NewGame,
    Game,
    Quit,
};

class Settings {
  private:
    bool _is_debug;
    State _state;
	float _master_volume;
	float _music_volume;
	float _effect_volume;

  public:
    Settings();
	State get_state();
	void set_state(State state);
	float get_master_volume();
	void set_master_volume(float volume);
	float get_music_volume();
	void set_music_volume(float volume);
	float get_effect_volume();
	void set_effect_volume(float volume);
    void toggle_debug();
    bool is_debug();
};
