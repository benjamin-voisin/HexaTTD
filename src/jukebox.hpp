#pragma once

#include <string>
#include <thread>
#include <vector>

#include "raylib.h"
#include "settings.hpp"

class Jukebox {
  private:
    void update();
    std::vector<Music> _musics;
    int _current_music;
    std::thread _update_thread;
    std::string _music_path;
    Settings *_settings;

  public:
    Jukebox(std::string music_path, Settings *settings);
    ~Jukebox();
    void play_random_music();
    // void stop();
};
