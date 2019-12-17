/**
* \file hl1_frame_interpolation.cpp
* \brief Implementation for the HL1 frame interpolation class.
*/

#include "pch.h"
#include "hl1_frame_interpolation.h"

namespace hl_mdlviewer {
namespace hl1 {

FrameInterpolation::FrameInterpolation()
{
}

/**
* @note The structure of this function has been adapted from the HL1 source code.
* source: file: studio_render.c
*         function(s): AdvanceFrame
*/
float FrameInterpolation::advance_frame(
    const Sequence* sequence,
    float frame,
    float playback_rate,
    float delta_time,
    bool* sequence_finished)
{
    if (sequence_finished)
        *sequence_finished = false;

    if (sequence->num_frames <= 1)
    {
        return 0;
    }
    else
    {
        if (delta_time > 0.1f)
            delta_time = (float)0.1f;

        frame += delta_time * sequence->fps * playback_rate;

        if (frame >= sequence->num_frames - 1)
        {
            frame -= (int)(frame / (sequence->num_frames - 1)) * (sequence->num_frames - 1);
            if (sequence_finished)
                *sequence_finished = true;
        }

        return frame;
    }
}

}
}
