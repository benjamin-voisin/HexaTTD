#pragma once

#include <string>
#include <thread>
#include <vector>
#include <unordered_map>
#include <mutex>

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
	std::unordered_map<std::string, Sound> _sounds;
	std::vector<std::string> _to_play;
	std::mutex _to_play_lock;

  public:
    Jukebox(std::string music_path, Settings *settings);
    ~Jukebox();
    void play_random_music();
	void play_sound(std::string sound);

	void set_master_volume(float volume);
	void set_music_volume(float volume);
	void set_effect_volume(float volume);
    // void stop();
};
