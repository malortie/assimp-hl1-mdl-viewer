/**
* \file hl1_nanogui_view_glcanvas.cpp
* \brief Implementation for the HL1 nanogui OpenGL canvas.
*/

#include "pch.h"
#include "hl1_nanogui_view_glcanvas.h"
#include "hl1_mdlviewer_presenter.h"
#include <glm/glm.hpp>

namespace hl_mdlviewer {
namespace hl1 {

MyGLCanvas::MyGLCanvas(HL1MDLViewerPresenter* presenter, nanogui::Widget* parent) :
    presenter_(presenter),
    mouse_(),
    mouseButtonDown_(-1),
    last_time_(0),
    nanogui::GLCanvas(parent)
{
    last_time_ = static_cast<float>(glfwGetTime());
}

void MyGLCanvas::drawGL() {
    using namespace nanogui;

    float current_time = static_cast<float>(glfwGetTime());
    float delta_time = current_time - last_time_;
    if (delta_time > 0.1f)
        delta_time = 0.1f;
    presenter_->draw_model(delta_time);
    last_time_ = current_time;
}

bool MyGLCanvas::mouseButtonEvent(
    const nanogui::Vector2i& p,
    int button,
    bool down, 
    int modifiers)
{
    mouse_ = p;

    if (down)
    {
        mouseButtonDown_ = button;
    }
    else
    {
        mouseButtonDown_ = -1;
    }

    return true;
}

bool MyGLCanvas::mouseMotionEvent(
    const nanogui::Vector2i& p, 
    const nanogui::Vector2i& rel, 
    int button, 
    int modifiers)
{
    using namespace nanogui;

    Vector2i delta = p - mouse_;
    mouse_ = p;

    glm::vec2 delta2 = glm::vec2(delta.x(), delta.y());

    if (mouseButtonDown_ == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (modifiers == 0)
        {
            presenter_->update_angles(delta2);
        }
        else if (modifiers == GLFW_MOD_SHIFT)
        {
            presenter_->update_pan(delta2);
        }
    }
    else if (mouseButtonDown_ == GLFW_MOUSE_BUTTON_RIGHT)
    {
        presenter_->update_camera_distance(
            // Rotate by the greatest value.
            std::abs(delta2.x) > std::abs(delta2.y)
            ? delta2.x : delta2.y);
    }

    return true;
}

bool MyGLCanvas::scrollEvent(
    const nanogui::Vector2i& p, 
    const nanogui::Vector2f& rel)
{
    glm::vec2 delta(rel.col(0)[0], rel.col(0)[1]);
    float distance = delta.y != 0 ? delta.y : delta.x;

    // A positive distance tells us to move toward the model.
    presenter_->update_camera_distance_zoom_step(distance > 0);
    return true;
}

}
}
