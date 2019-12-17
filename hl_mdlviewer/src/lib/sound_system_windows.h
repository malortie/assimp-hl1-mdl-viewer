/**
* \file sound_system_windows.h
* \brief Declaration for the Windows sound system implementation class.
*/

#ifndef HLMDLVIEWER_SOUND_SYSTEM_WINDOWS_H_
#define HLMDLVIEWER_SOUND_SYSTEM_WINDOWS_H_

#include "sound_system_impl.h"

namespace hl_mdlviewer {

class SoundSystemImplWindows : public SoundSystemImpl
{
public:
    virtual void play_sound(const char* file_path, FileSystem* const file_system);
};

}

#endif // HLMDLVIEWER_SOUND_SYSTEM_WINDOWS_H_
