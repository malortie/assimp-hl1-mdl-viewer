/**
* \file hl1_animation_event_handler.h
* \brief Declaration for the HL1 animation event handler class.
*/

#ifndef HLMDLVIEWER_HL1_ANIMATION_EVENT_HANDLER_H_
#define HLMDLVIEWER_HL1_ANIMATION_EVENT_HANDLER_H_

#include "hl1_studiomodel.h"
#include "hl1_sequence_listener.h"
#include "sound_system.h"
#include "file_system.h"

namespace hl_mdlviewer {
namespace hl1 {

/** \brief A class that handles animation events. */
class AnimationEventHandler : public SequenceListener
{
public:
    AnimationEventHandler(SoundSystem* sound_system = nullptr);
    AnimationEventHandler(const AnimationEventHandler&) = delete;

    /** \brief Check sequence for animation events.
    * \param[in] sequence The sequence to be processed.
    * \param[in] frame The frame at which to check for animation events.
    */
    void process_events(const Sequence* sequence, int frame);

    // See SequenceListener interface for more info.
    virtual void on_change_sequence(const Sequence* old_sequence, const Sequence* new_sequence);
    
    // See SequenceListener interface for more info.
    virtual void on_sequence_finished(const Sequence* sequence);

protected:

    /** \brief Called when an animation event is processed. 
    * \param[in] event The animation event.
    */
    void handle_event(const AnimationEvent* event);

private:

    int last_event_frame_;
    SoundSystem* sound_system_;
};

}
}

#endif // HLMDLVIEWER_HL1_ANIMATION_EVENT_HANDLER_H_
