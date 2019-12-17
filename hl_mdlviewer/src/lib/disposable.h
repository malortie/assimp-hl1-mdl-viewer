/**
* \file disposable.h
* \brief Interface for a model viewer view class.
*/

#ifndef HLMDLVIEWER_DISPOSABLE_H_
#define HLMDLVIEWER_DISPOSABLE_H_

namespace hl_mdlviewer {

/** \brief An interface for classes with disposable behavior. */
class Disposable
{
public:
    virtual void dispose() = 0;
};

}

#endif // HLMDLVIEWER_DISPOSABLE_H_
