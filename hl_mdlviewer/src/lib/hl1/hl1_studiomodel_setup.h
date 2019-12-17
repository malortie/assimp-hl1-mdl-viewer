/**
* \file hl1_studiomodel_setup.h
* \brief Declaration for the HL1 Studio model setup class.
*/

#ifndef HLMDLVIEWER_HL1_STUDIOMODEL_SETUP_H_
#define HLMDLVIEWER_HL1_STUDIOMODEL_SETUP_H_

#include "hl1_studiomodel.h"
#include "hl1_studiomodel_buffer.h"
#include "buffer_builder.h"
#include <assimp/scene.h>
#include <assimp/types.h>

namespace hl_mdlviewer {
namespace hl1 {

/** \brief This class converts information from an Assimp scene,
* to a Studiomodel. */
class StudioModelSetup
{
public:
    StudioModelSetup();
    StudioModelSetup(const StudioModelSetup&) = delete;

    /** \brief Convert the Assimp \p scene to a Studiomodel \p studio_model.
    * \param[in] scene The scene to be converted.
    * \param[in, out] studio_model The output Studiomodel.
    * \param[in, out] studio_model_buffer The output Studiomodel buffer.
    */
    void setup_model(const aiScene* scene, 
        StudioModel* studio_model, 
        StudioModelBuffer* studio_model_buffer,
        glm::mat4& scene_transform);

protected:
    void setup_model_data();
    void setup_model_buffers();

    void setup_bones();
    void setup_bone_controllers();
    void setup_sequences();
    void setup_textures();
    void setup_skins();
    void setup_attachments();
    void setup_hitboxes();
    void setup_meshes();

    void setup_model_stats();

    void setup_buffer_meshes();
    void setup_buffer_bones();
    void build_bone_segments(std::vector<unsigned int>& bone_segments);

    void setup_buffer_attachments();
    void setup_buffer_hitboxes();
    void setup_buffer_sequence_bbox();
    void setup_buffer_gltextures();

    /** \brief Read scene metadata.
    * \param[in] metadata_key The metadata key.
    * \param[in] value The output value.
    * \return true if the key could be read; false otherwise.
    */
    template<typename MetaDataType, typename TargetType = MetaDataType>
    inline bool get_global_info_value(const char* metadata_key, TargetType & value) {
        MetaDataType temp;
        bool result = scene_global_info_->mMetaData->Get<MetaDataType>(metadata_key, temp);
        value = static_cast<TargetType>(temp);
        return result;
    };

private:
    /** A pointer to the output Studiomodel. */
    StudioModel* studio_model_;

    /** A pointer to the output Studiomodel buffer. */
    StudioModelBuffer* studio_model_buffer_;

    /** A pointer to the loaded Assimp scene. */
    const aiScene* scene_;

    /** Bone names to their id. */
    std::map<std::string, int> bone_map_;

    /** \brief A pointer to the scene bones node.
    * Used to avoid having to constantly find the node. */
    aiNode* scene_bones_;

    /** \brief A pointer to the scene global info node.
    * Used to avoid having to constantly find the node. */
    aiNode* scene_global_info_;

    /** \brief Used to combine all vertices and indices
    * into one buffer. */
    BufferBuilder buffer_builder_;
};

}
}

#endif // HLMDLVIEWER_HL1_STUDIOMODEL_SETUP_H_
