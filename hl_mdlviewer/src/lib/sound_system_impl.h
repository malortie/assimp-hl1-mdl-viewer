/**
* \file sound_system_impl.h
* \brief Interface for the sound system implementation class.
*/

#ifndef HLMDLVIEWER_SOUND_SYSTEM_IMPL_H_
#define HLMDLVIEWER_SOUND_SYSTEM_IMPL_H_

#include "file_system.h"

namespace hl_mdlviewer {

class SoundSystemImpl {
public:
    virtual void play_sound(const char* file_path, 
        FileSystem* const file_system) = 0;
};

}

#endif // HLMDLVIEWER_SOUND_SYSTEM_IMPL_H_
