#include "jukebox.hpp"
#include "raylib.h"
#include "log.hpp"

Jukebox::Jukebox(std::string music_path, Settings *settings)
    : _music_path{music_path}, _settings{settings} {
    srand(std::time(nullptr));
    _update_thread = std::thread([this]() {
			InitAudioDevice();
			SetMasterVolume(_settings->get_master_volume());
			std::string app_directory = GetApplicationDirectory();
			FilePathList files = LoadDirectoryFilesEx(
					(app_directory + _music_path).c_str(), ".ogg", false);
			for (std::size_t i = 0; i < files.count; i++) {
				_musics.push_back(LoadMusicStream(files.paths[i]));
				_music_names.push_back(std::string(files.paths[i]));
				SetMusicVolume(_musics.back(), _settings->get_music_volume());
			}
			UnloadDirectoryFiles(files);
			_current_music = rand() % _musics.size();
			PlayMusicStream(_musics[_current_music]);
			Log::Info.log("Playing music %s. Duration : %0.fs",
				_music_names[_current_music].c_str(),
				GetMusicTimeLength(_musics[_current_music])
			);
			auto sound_directory = "assets/sounds/";
			_sounds.resize(NUMBER);
			_sounds[CLICK] = LoadSound((app_directory + sound_directory + "button-click.ogg").c_str());
			_sounds[RAIL] = LoadSound((app_directory + sound_directory + "rail.ogg").c_str());
			_sounds[RAIL_DESTRUCTION] = LoadSound((app_directory + sound_directory + "rail_destruction.ogg").c_str());
			for (auto sound : _sounds) {
			SetSoundVolume(sound, _settings->get_effect_volume());
			}

			update();
	});
}

void Jukebox::update() {
    while (_settings->get_state() != State::Quit) {
		// Update the music
        UpdateMusicStream(_musics[_current_music]);
        std::this_thread::sleep_for(std::chrono::milliseconds(6));
        if (GetMusicTimeLength(_musics[_current_music]) -
                GetMusicTimePlayed(_musics[_current_music]) <=
            0.05) {
            StopMusicStream(_musics[_current_music]);
            _current_music = rand() % _musics.size();
            PlayMusicStream(_musics[_current_music]);
			Log::Info.log("Playing music %s. Duration : %0.fs",
				_music_names[_current_music].c_str(),
				GetMusicTimeLength(_musics[_current_music])
			);
        }

		// Play the requested sounds
		_to_play_lock.lock();
		if (!_to_play.empty()) {
			while (!_to_play.empty()) {
				auto sound = _sounds[_to_play.back()];
				PlaySound(sound);
				_to_play.pop_back();
			}
		}
		_to_play_lock.unlock();
    }
}

void Jukebox::play_sound(SoundName sound) {
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


Jukebox::~Jukebox() {
	_settings->set_state(State::Quit);
    _update_thread.join();
    StopMusicStream(_musics[_current_music]);
    for (Music music : _musics) {
        UnloadMusicStream(music);
    }
	for (auto sound : _sounds) {
		UnloadSound(sound);
	}
    CloseAudioDevice();
}
