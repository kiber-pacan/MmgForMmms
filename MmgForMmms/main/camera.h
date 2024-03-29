#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES

class camData {
public:
    glm::vec3 camPos, direction, upVector;

    camData() : camPos{0, 2, 0}, direction{0, 0, 0}, upVector{0, 1, 0} { }
};

