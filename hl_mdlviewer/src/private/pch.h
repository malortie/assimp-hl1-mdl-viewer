#ifndef HLMDLVIEWER_PCH_H_
#define HLMDLVIEWER_PCH_H_

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <memory>
#include <algorithm>
#include <utility>
#include <functional>
#include <numeric>
#include <stdexcept>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#include <math.h>
#endif

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#endif

#include "glad.h"

#include "math_functions.h"
#include "disposable.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "config.h"

#endif // HLMDLVIEWER_PCH_H_
