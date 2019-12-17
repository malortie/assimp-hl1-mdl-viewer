/**
* \file mdlviewer_application.cpp
* \brief Implementation for the model viewer application class.
*/

#include "pch.h"
#include "mdlviewer_application.h"
#include <iostream>

namespace hl_mdlviewer {

MDLViewerApplication::MDLViewerApplication(
    MDLViewerPresenter* presenter) :
    presenter_(presenter),
    initialized_(false)
{
}

MDLViewerApplication::~MDLViewerApplication()
{
}

int MDLViewerApplication::run()
{
    try
    {
        if (!initialized_)
        {
            presenter_->initialize();
            initialized_ = true;
        }

        presenter_->run();
    
        presenter_->dispose();

        return EXIT_SUCCESS;
    }
    catch (std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        
        presenter_->dispose();

        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Caught an unknown exception." << std::endl;
        
        presenter_->dispose();

        return EXIT_FAILURE;
    }
}

}
