/**
* \file hl1_nanogui_view_glcanvas.h
* \brief Declaration for the HL1 nanogui OpenGL canvas.
*/

#ifndef HLMDLVIEWER_HL1_NANOGUI_VIEW_GLCANVAS_H_
#define HLMDLVIEWER_HL1_NANOGUI_VIEW_GLCANVAS_H_

#include <nanogui/nanogui.h>

namespace hl_mdlviewer {
namespace hl1 {

class HL1MDLViewerPresenter;

class MyGLCanvas : public nanogui::GLCanvas {
public:
    MyGLCanvas(HL1MDLViewerPresenter* presenter, nanogui::Widget* parent);

    virtual void drawGL() override;

    virtual bool mouseButtonEvent(
        const nanogui::Vector2i& p,
        int button,
        bool down,
        int modifiers);

    virtual bool mouseMotionEvent(
        const nanogui::Vector2i& p,
        const nanogui::Vector2i& rel,
        int button,
        int modifiers);

    virtual bool scrollEvent(
        const nanogui::Vector2i& p,
        const nanogui::Vector2f& rel);

private:
    HL1MDLViewerPresenter* presenter_;

    nanogui::Vector2i mouse_;
    int mouseButtonDown_;
    float last_time_;
};

}
}

#endif // HLMDLVIEWER_HL1_NANOGUI_VIEW_GLCANVAS_H_
