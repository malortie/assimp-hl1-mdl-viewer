#include "pch.h"

#if defined (_WIN32) && defined (_DEBUG)
#include "enable_memory_leak_detection.h"
static hl_mdlviewer::EnableMemoryLeakDetection memory_leak_detection;
#endif

#if defined (HLMVIEWER_NANOGUI_ENABLED)
#include "hl1_nanogui_view.h"
#else
#include "hl1_null_view.h"
#endif
#include "hl1_mdlviewer_presenter.h"
#include "mdlviewer_application.h"

int main()
{
#if defined HLMVIEWER_NANOGUI_ENABLED
    hl_mdlviewer::hl1::HL1NanoGUIView view;
#else
    hl_mdlviewer::hl1::HL1MDLViewerViewNull view;
#endif
    hl_mdlviewer::hl1::HL1MDLViewerPresenter presenter(&view);
    hl_mdlviewer::MDLViewerApplication app(
        &presenter);
    int code = app.run();
    return code;
}
