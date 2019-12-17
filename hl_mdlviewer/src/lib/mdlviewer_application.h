/**
* \file mdlviewer_application.h
* \brief Declaration for the model viewer application class.
*/

#ifndef HLMDLVIEWER_APPLICATION_H_
#define HLMDLVIEWER_APPLICATION_H_

#include "mdlviewer_presenter.h"
#include "file_system.h"
#include "sound_system.h"

namespace hl_mdlviewer {

class MDLViewerApplication
{
public:
    MDLViewerApplication(MDLViewerPresenter* presenter);
    MDLViewerApplication(const MDLViewerApplication&) = delete;
    ~MDLViewerApplication();

    int run();

protected:

    bool initialized_;
    MDLViewerPresenter* presenter_;
};

}

#endif // HLMDLVIEWER_APPLICATION_H_
