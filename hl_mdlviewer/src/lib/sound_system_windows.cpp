/** 
* \file sound_system_windows.cpp
* \brief Implementation for the Windows sound system implementation class.
*/

#include "pch.h"
#include "sound_system_windows.h"

#include <mmsystem.h>

namespace hl_mdlviewer {

void SoundSystemImplWindows::play_sound(const char* file_path, FileSystem* const file_system)
{
    std::string full_path_to_sound;
    if (file_system->find_file(file_path, full_path_to_sound))
    {
        PlaySoundA(full_path_to_sound.c_str(), NULL,
            SND_FILENAME
            | SND_NODEFAULT
            | SND_ASYNC
            | SND_NOSTOP
        );
    }
}

}
