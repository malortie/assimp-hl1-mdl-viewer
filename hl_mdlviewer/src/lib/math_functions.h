/**
* \file math_functions.h
*/

#ifndef HLMDLVIEWER_MATH_FUNCTIONS_H_
#define HLMDLVIEWER_MATH_FUNCTIONS_H_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/types.h>

namespace hl_mdlviewer {

/** A constant compile time float version of PI. */
const float M_PI_F = static_cast<float>(M_PI);

/** A constant compile time float version of PI / 2. */
const float HALF_PI = static_cast<float>(M_PI / 2);

/** A constant compile time float version of 2 * PI. */
const float TWO_PI = static_cast<float>(2 * M_PI);

/** \brief Clamp \p value between \p min and \p max, inclusively.
* \param[in] value The value to clamp.
* \param[in] min The minimum value.
* \param[in] max The maximum value.
* \return The clamped value.
*/
template<typename T>
inline T clamp(T value, T min, T max)
{
    return value < min ? min : value > max ? max : value;
}

/** \brief Convert an Assimp 2D vector to a GLM 2D vector.
* \param[in] v An assimp 2D vector.
* \return The Assimp 2D vector converted to a glm 2D vector.
*/
inline glm::vec2 to_glm_vec2(const aiVector3D& v)
{
    return glm::vec2(v.x, v.y);
}

/** \brief Convert an Assimp 3D vector to a GLM 3D vector. 
* \param[in] v An assimp 3D vector.
* \return The Assimp 3D vector converted to a glm 3D vector.
*/
inline glm::vec3 to_glm_vec3(const aiVector3D& v)
{
    return glm::vec3(v.x, v.y, v.z);
}

/** \brief Convert an Assimp 3D color to a glm 3D vector.
* \param[in] c An Assimp 3D color.
* \return The Assimp 3D color converted to a glm 3D vector.
*/
inline glm::vec3 to_glm_vec3(const aiColor3D& c)
{
    return glm::vec3(c.r, c.g, c.b);
}

/** \brief Convert an Assimp quaternion to a glm quaternion.
* \param[in] q An Assimp quaternion.
* \return The Assimp quaternion converted to a glm quaternion.
*/
inline glm::quat to_glm_quat(const aiQuaternion& q)
{
    return glm::quat(q.w, q.x, q.y, q.z);
}

/** \brief Convert an Assimp 4x4 matrix to a glm 4x4 matrix.
* \param[in] m An Assimp 4x4 matrix.
* \return The Assimp 4x4 matrix converted to a glm 4x4 matrix.
*/
inline glm::mat4 to_glm_mat4(const aiMatrix4x4& m)
{
    return glm::mat4(
        glm::vec4(m.a1, m.b1, m.c1, m.d1),
        glm::vec4(m.a2, m.b2, m.c2, m.d2),
        glm::vec4(m.a3, m.b3, m.c3, m.d3),
        glm::vec4(m.a4, m.b4, m.c4, m.d4)
    );
}

}

#endif // HLMDLVIEWER_MATH_FUNCTIONS_H_
