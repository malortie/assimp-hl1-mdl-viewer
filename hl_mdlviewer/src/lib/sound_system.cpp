/**
* \file sound_system.cpp
* \brief Implementation for the sound system class.
*/

#include "pch.h"
#include "sound_system.h"
#ifdef _WIN32
#include "sound_system_windows.h"
#endif

namespace hl_mdlviewer {

SoundSystem::SoundSystem(FileSystem* file_system) :
    file_system_(file_system),
    impl_(nullptr)
{
#ifdef _WIN32
    impl_.reset(new SoundSystemImplWindows());
#endif
}

SoundSystem::~SoundSystem()
{
    if (impl_)
        impl_.reset(nullptr);
}

void SoundSystem::play_sound(const char* file_path)
{
    impl_->play_sound(file_path, file_system_);
}

}
