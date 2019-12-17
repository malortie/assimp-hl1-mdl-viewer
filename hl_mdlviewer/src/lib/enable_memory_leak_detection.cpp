/**
* \file enable_memory_leak_detection.cpp
* \brief Implementation for the memory leak detection enabler class.
*/

#include "pch.h"
#include "enable_memory_leak_detection.h"

#ifdef _DEBUG
#include <crtdbg.h>
#endif

namespace hl_mdlviewer {

EnableMemoryLeakDetection::EnableMemoryLeakDetection() {
    enable_memory_leak_detection();
}

void EnableMemoryLeakDetection::enable_memory_leak_detection() {
#ifdef _DEBUG
    _CrtSetDbgFlag(
        _CRTDBG_ALLOC_MEM_DF |
        _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_WNDW);
#endif
}

}
