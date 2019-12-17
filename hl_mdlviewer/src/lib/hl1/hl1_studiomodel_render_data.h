/**
* \file hl1_studiomodel_render_data.h
*/

#ifndef HLMDLVIEWER_HL1_STUDIOMODEL_RENDER_DATA_H_
#define HLMDLVIEWER_HL1_STUDIOMODEL_RENDER_DATA_H_

#include <memory>

namespace hl_mdlviewer {
namespace hl1 {

/** \brief A structure that contains data used by the renderer class. */
struct StudioModelRenderData
{
    StudioModelRenderData() :
        model(),
        skin(0)
    {
    }

    void clear()
    {
        skin = 0;
        model.clear();
    }

    void initialize(int skin, const size_t num_bodyparts)
    {
        clear();

        this->skin = skin;

        if (num_bodyparts)
            model.resize(num_bodyparts);
    }

    std::vector<int> model;
    int skin;
};

}
}

#endif // HLMDLVIEWER_HL1_STUDIOMODEL_RENDER_DATA_H_
