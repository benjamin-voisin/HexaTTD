#pragma once

class Settings {
  private:
    bool _is_debug;

  public:
    Settings();
    void toggle_debug();
    bool is_debug();
};
