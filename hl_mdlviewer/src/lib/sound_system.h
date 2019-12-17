/**
* \file sound_system.h
* \brief Declaration for the sound system class.
*/

#ifndef HLMDLVIEWER_SOUND_SYSTEM_H_
#define HLMDLVIEWER_SOUND_SYSTEM_H_

#include "disposable.h"
#include "file_system.h"
#include "sound_system_impl.h"

namespace hl_mdlviewer {

class SoundSystem
{
public:
    SoundSystem(FileSystem* file_system);
    SoundSystem(const SoundSystem&) = delete;
    ~SoundSystem();

    void play_sound(const char* file_path);
private:
    FileSystem* file_system_;

    /** The bridge implementation. */
    std::unique_ptr<SoundSystemImpl> impl_;
};

}

#endif // HLMDLVIEWER_SOUND_SYSTEM_H_
