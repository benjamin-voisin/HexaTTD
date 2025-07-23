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

  public:
    Settings();
	State get_state();
	void set_state(State state);
    void toggle_debug();
    bool is_debug();
};
