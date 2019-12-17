/**
* \file mdlviewer_view.h
* \brief Interface for a model viewer view class.
*/

#ifndef HLMDLVIEWER_VIEW_H_
#define HLMDLVIEWER_VIEW_H_

#include "disposable.h"

namespace hl_mdlviewer {

template<typename Presenter>
class MDLViewerView : public Disposable
{
public:
    virtual void set_presenter(Presenter* presenter) = 0;
    virtual void initialize() = 0;
    virtual void run() = 0;
    virtual void invalidate() = 0;

    virtual void on_model_loading_success() = 0;
    virtual void on_model_loading_failure(const std::string& model_file_path) = 0;
    virtual void enable_model_interaction() = 0;
    virtual void disable_model_interaction() = 0;
};

}

#endif // HLMDLVIEWER_VIEW_H_
