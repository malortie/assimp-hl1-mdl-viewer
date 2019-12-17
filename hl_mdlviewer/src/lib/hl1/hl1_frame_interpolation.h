/**
* \file hl1_frame_interpolation.h
* \brief Declaration for the HL1 frame interpolation class.
*/

#ifndef HLMDLVIEWER_HL1_FRAME_INTERPOLATION_H_
#define HLMDLVIEWER_HL1_FRAME_INTERPOLATION_H_

#include "hl1_studiomodel.h"
#include "hl1_sequence_listener.h"

namespace hl_mdlviewer {
namespace hl1 {

class FrameInterpolation
{
public:
    FrameInterpolation();
    FrameInterpolation(const FrameInterpolation&) = delete;

    /** \brief Advance the frame.
    * \param[in] sequence The sequence.
    * \param[in] frame The current frame.
    * \param[in] playback_rate A factor by which to scale the animation speed.
    * \param[in] delta_time The delta to apply to the frame.
    * \param[out] sequence_finished Whether or not the sequence 
    *             was finished after this frame.
    * \return The new frame.
    */
    float advance_frame(
        const Sequence* sequence,
        float frame,
        float playback_rate,
        float delta_time,
        bool* sequence_finished = nullptr);
};

}
}

#endif // HLMDLVIEWER_HL1_FRAME_INTERPOLATION_H_
