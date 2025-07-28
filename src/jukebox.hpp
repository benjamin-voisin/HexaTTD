#pragma once

#include <string>
#include <thread>
#include <vector>
#include <mutex>

#include "raylib.h"
#include "settings.hpp"

class Jukebox {
  public:
	enum SoundName {
		CLICK,
		RAIL,
		RAIL_DESTRUCTION,
		NUMBER,
	};
    Jukebox(std::string music_path, Settings *settings);
    ~Jukebox();
    void play_random_music();
	void play_sound(SoundName sound);

	void set_master_volume(float volume);
	void set_music_volume(float volume);
	void set_effect_volume(float volume);

    // void stop();
  private:
    void update();
    std::vector<Music> _musics;
	std::vector<std::string> _music_names;
    int _current_music;
    std::thread _update_thread;
    std::string _music_path;
    Settings *_settings;
	std::vector<Sound> _sounds;
	std::vector<SoundName> _to_play;
	std::mutex _to_play_lock;

};
