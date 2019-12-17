/**
* \file hl1_studiomodel_animation.h
* \brief Declaration for the HL1 Studio model animation class.
*/

#ifndef HLMDLVIEWER_HL1_STUDIOMODEL_ANIMATION_H_
#define HLMDLVIEWER_HL1_STUDIOMODEL_ANIMATION_H_

#include "disposable.h"
#include "hl1_studiomodel.h"
#include "hl1_animation_event_handler.h"
#include "hl1_frame_interpolation.h"
#include "hl1_studiomodel_animation_data.h"

namespace hl_mdlviewer {
namespace hl1 {

/** \brief The HL1 model animation class. */
class StudioModelAnimation : public Disposable
{
public:
    StudioModelAnimation(
        StudioModel* studio_model,
        AnimationEventHandler* animation_event_handler,
        FrameInterpolation* frame_interpolation);
    StudioModelAnimation(const StudioModelAnimation&) = delete;

    void initialize();
    virtual void dispose();

    void reset();
    void update(const float frame_time);

    inline const std::vector<glm::mat4>& get_bone_transforms() const { return bones_transform_; }

    const StudioModelAnimationData* animation_data() const { return &animation_data_; }
    void set_sequence(int value);
    void set_playback_rate(float value);
    void set_frame(float value);
    void set_blending(int index, uint8_t value);
    void set_bone_controller(int index, float value);

    void add_listener(SequenceListener* listener);

    void on_model_changed();

protected:

    /** \brief Interpolate bone between two consecutive frames.
    * \param[in] bone The bone to interpolate.
    * \param[in] frame The frame to interpolate with the next one.
    * \param[in] s The interpolation factor where 0 is \p frame and 1 is \p frame + 1.
    * \param[in] blend The sequence blend index.
    * \param[out] result_position The interpolated bone position.
    * \param[out] result_orientation The interpolated bone orientation.
    */
    void interpolate_bone_at_frame(
        const Bone* bone,
        int frame,
        float s,
        int blend,
        glm::vec3& result_position,
        glm::quat& result_orientation);

    /** \brief Setup the bind pose bone matrix in local space.
    * \param[in] bone The bone.
    * \param[out] transform The bone bind pose transform in local space.
    */
    void setup_bind_pose_bone_transform(const Bone* bone, glm::mat4& transform);
    
    /** \brief Setup the bone matrix in local space.
    * \param[in] bone The bone to setup.
    * \param[in] sequence The sequence.
    * \param[in] frame The frame.
    * \param[in] s The interpolation factor where 0 is \p frame and 1 is \p frame + 1.
    * \param[out] transform The bone transform in local space.
    */
    void setup_animated_bone_transform(const Bone* bone,
        const Sequence* sequence, int frame, float s, glm::mat4& transform);

    /** \brief Apply bone parent transformation to \p transform.
    * \param[in] bone The bone.
    * \param[out] transform The transformation matrix to apply
    *             parent transform to.
    */
    void apply_bone_parent_transform(const Bone* bone, glm::mat4& transform);
    
    /** \brief Apply a single bone controller transformation to 
    *          \p result_position and \p result_orientation.
    * \param[in] bone_controller The bone controller.
    * \param[in, out] result_position The result position.
    * \param[in, out] result_orientation The result orientation.
    */
    void apply_bone_controller_transform(
        const BoneController* bone_controller,
        glm::vec3& result_position, 
        glm::quat& result_orientation);

    void advance_frame(const Sequence* sequence, const float frame_time);
    
    void process_animation_events(const Sequence* sequence, int frame);
    
    bool model_has_sequences() const;

private:

    /** \brief A pointer to the Studiomodel. */
    StudioModel* studio_model_;

    /** \brief The animation data. */
    StudioModelAnimationData animation_data_;

    /** \brief A pointer to an animation event handler. */
    AnimationEventHandler* animation_event_handler_;
    
    /** \brief A pointer to a frame interpolator. */
    FrameInterpolation* frame_interpolation_;

    /** \brief The bone transforms in absolute space. */
    std::vector<glm::mat4> bones_transform_;

    /** \brief A list of sequence listeners. */
    std::list<SequenceListener*> listeners_;
};

}
}

#endif // HLMDLVIEWER_HL1_STUDIOMODEL_ANIMATION_H_
