/**
* \file hl1_animation_event_handler.cpp
* \brief Implementation for the HL1 animation event handler class.
*/

#include "pch.h"
#include "hl1_animation_event_handler.h"

namespace hl_mdlviewer {
namespace hl1 {

AnimationEventHandler::AnimationEventHandler(SoundSystem* sound_system) :
    last_event_frame_(-1),
    sound_system_(sound_system)
{
}

void AnimationEventHandler::on_change_sequence(const Sequence* old_sequence, const Sequence* new_sequence)
{
    last_event_frame_ = -1;
}

void AnimationEventHandler::on_sequence_finished(const Sequence* sequence)
{
    last_event_frame_ = -1;
}

void AnimationEventHandler::handle_event(const AnimationEvent* event)
{
    if (event->event == SCRIPT_EVENT_SOUND)
    {
        if (sound_system_)
        {
            char* pc = const_cast<char*>(event->options.c_str());
            if (*pc != '\0')
            {
                while (*pc == '*' && *pc != '\0')
                    pc++;

                // Play the sound if we have a sound system.
                sound_system_->play_sound(pc);
            }
        }
    }
}

void AnimationEventHandler::process_events(const Sequence* sequence, int frame)
{
    if (last_event_frame_ < frame)
    {
        const AnimationEvent* event = nullptr;

        for (size_t i = 0; i < sequence->events.size(); ++i)
        {
            event = &sequence->events[i];
            if (event->frame == frame)
            {
                last_event_frame_ = frame;

                handle_event(event);
            }
        }
    }
}

}
}
