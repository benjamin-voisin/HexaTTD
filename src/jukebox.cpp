#include "jukebox.hpp"
#include "raylib.h"

Jukebox::Jukebox(std::string music_path, Settings *settings)
    : _music_path{music_path}, _settings{settings} {
    srand(std::time(nullptr));
    _update_thread = std::thread([this]() { update(); });
}

void Jukebox::update() {
    InitAudioDevice();
	SetMasterVolume(_settings->get_master_volume());
    std::string app_directory = GetApplicationDirectory();
    FilePathList files = LoadDirectoryFilesEx(
        (app_directory + _music_path).c_str(), ".ogg", false);
    for (std::size_t i = 0; i < files.count; i++) {
        _musics.push_back(LoadMusicStream(files.paths[i]));
		SetMusicVolume(_musics.back(), _settings->get_music_volume());
    }
    UnloadDirectoryFiles(files);
    _current_music = rand() % _musics.size();
    PlayMusicStream(_musics[_current_music]);
	auto sound_directory = "assets/sounds/";
	_sounds["click"] = LoadSound((app_directory + sound_directory + "button-click.ogg").c_str());
	_sounds["rail"] = LoadSound((app_directory + sound_directory + "rail.ogg").c_str());
	_sounds["rail_destruction"] = LoadSound((app_directory + sound_directory + "rail_destruction.ogg").c_str());
	for (auto sound_map : _sounds) {
		SetSoundVolume(sound_map.second, _settings->get_effect_volume());
	}


    while (_settings->get_state() != State::Quit) {
		// Update the music
        UpdateMusicStream(_musics[_current_music]);
        std::this_thread::sleep_for(std::chrono::milliseconds(6));
        if (GetMusicTimeLength(_musics[_current_music]) -
                GetMusicTimePlayed(_musics[_current_music]) <=
            0.05) {
            StopMusicStream(_musics[_current_music]);
            _current_music = rand() % _musics.size();
            // SeekMusicStream(_musics[_current_music], 0.f);
            PlayMusicStream(_musics[_current_music]);
        }

		// Play the requested sounds
		if (!_to_play.empty()) {
			_to_play_lock.lock();
			while (!_to_play.empty()) {
				auto sound = _sounds[_to_play.back()];
				PlaySound(sound);
				_to_play.pop_back();
			}
			_to_play_lock.unlock();
		}
    }
}

void Jukebox::play_sound(std::string sound) {
	_to_play_lock.lock();
	_to_play.push_back(sound);
	_to_play_lock.unlock();
}

void Jukebox::set_master_volume(float volume) {
	SetMasterVolume(volume);
	_settings->set_master_volume(volume);
}

void Jukebox::set_music_volume(float volume) {
	_settings->set_music_volume(volume);
}

void Jukebox::set_effect_volume(float volume) {
	_settings->set_effect_volume(volume);
}


#include "log.hpp"

Jukebox::~Jukebox() {
	_settings->set_state(State::Quit);
    _update_thread.join();
    StopMusicStream(_musics[_current_music]);
    for (Music music : _musics) {
        UnloadMusicStream(music);
    }
	for (auto sound_map : _sounds) {
		UnloadSound(sound_map.second);
		Log::Debug.log("Unloaded sound %s", sound_map.first.c_str());
	}
    CloseAudioDevice();
}
