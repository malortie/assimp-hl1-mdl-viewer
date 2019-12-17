/**
* \file mdlviewer_presenter.h
* \brief Interface for a model viewer presenter class.
*/

#ifndef HLMDLVIEWER_PRESENTER_H_
#define HLMDLVIEWER_PRESENTER_H_

#include "disposable.h"

namespace hl_mdlviewer {

class MDLViewerPresenter : public Disposable
{
public:
    virtual void initialize() = 0;
    virtual void run() = 0;
    virtual void load_model(const std::string& file_path) = 0;
};
    
}

#endif // HLMDLVIEWER_PRESENTER_H_
