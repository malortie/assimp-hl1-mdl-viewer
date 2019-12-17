/**
* \file enable_memory_leak_detection.h
* \brief Declaration for the memory leak detection enabler class.
*/

#ifndef HLMDLVIEWER_ENABLE_MEMORY_LEAK_DETECTION_H_
#define HLMDLVIEWER_ENABLE_MEMORY_LEAK_DETECTION_H_

namespace hl_mdlviewer {

/** \brief A helper class to enable/disable memory leak detection. */
class EnableMemoryLeakDetection
{
public:
    EnableMemoryLeakDetection();
private:
    void enable_memory_leak_detection();
};

}

#endif // HLMDLVIEWER_ENABLE_MEMORY_LEAK_DETECTION_H_
