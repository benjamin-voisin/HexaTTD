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

  public:
    State state;
    Settings();
    void toggle_debug();
    bool is_debug();
};
