/**
* \file hl1_sequence_listener.h
* \brief Interface for sequence listeners.
*/

#ifndef HLMDLVIEWER_HL1_SEQUENCE_LISTENER_H_
#define HLMDLVIEWER_HL1_SEQUENCE_LISTENER_H_

#include "hl1_studiomodel.h"

namespace hl_mdlviewer {
namespace hl1 {

class SequenceListener
{
public:

    /** \brief Called when a sequence has changed.
    * \param[in] old_sequence The previous sequence.
    * \param[in] new_sequence The new sequence.
    */
    virtual void on_change_sequence(const Sequence* old_sequence, const Sequence* new_sequence) = 0;
    
    /** \brief Called when a sequence has finished.
    * \param[in] old_sequence The sequence that has finished.
    */
    virtual void on_sequence_finished(const Sequence* sequence) = 0;
};

}
}

#endif // HLMDLVIEWER_HL1_SEQUENCE_LISTENER_H_
