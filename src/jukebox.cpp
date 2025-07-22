#include "jukebox.hpp"
#include "raylib.h"

Jukebox::Jukebox(std::string music_path, Settings *settings)
    : _music_path{music_path}, _settings{settings} {
    srand(std::time(nullptr));
    _update_thread = std::thread([this]() { update(); });
}

void Jukebox::update() {
    InitAudioDevice();
    std::string app_directory = GetApplicationDirectory();
    FilePathList files = LoadDirectoryFilesEx(
        (app_directory + _music_path).c_str(), ".ogg", false);
    for (std::size_t i = 0; i < files.count; i++) {
        _musics.push_back(LoadMusicStream(files.paths[i]));
    }
    UnloadDirectoryFiles(files);
    _current_music = rand() % _musics.size();
    PlayMusicStream(_musics[_current_music]);

    while (_settings->state != State::Quit) {
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
    }
}

Jukebox::~Jukebox() {
    _settings->state = State::Quit;
    _update_thread.join();
    StopMusicStream(_musics[_current_music]);
    for (Music music : _musics) {
        UnloadMusicStream(music);
    }
    CloseAudioDevice();
}
