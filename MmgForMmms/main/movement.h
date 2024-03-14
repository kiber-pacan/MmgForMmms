
//Cam data
camData CameraData = camData();
//Mouse position
double xpos, ypos, lasty, lastx;
float pitch, yaw;
const float sensitivity = 0.1f;
bool firstMouse = true;

//GLFW mouse callback
void mouseCallback(GLFWwindow* window) {
    glfwGetCursorPos(window, &xpos, &ypos);

    if (firstMouse)
    {
        lastx = xpos;
        lasty = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastx;
    float yoffset = lasty - ypos; // reversed since y-coordinates range from bottom to top
    lastx = xpos;
    lasty = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.9f) {
        pitch = 89.9f;
    }
    if (pitch < -89.9f) {
        pitch = -89.9f;
    }
}

float speed = 0.0001f;
float theta = glm::radians(90.f);

glm::vec3 dir;
glm::vec3 dir1;

//GLFW key callback        
void keyCallback(GLFWwindow* window) {

    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    dir = glm::normalize(dir);

    dir1.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir1.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    float cs = cos(theta);
    float sn = sin(theta);

    dir1.x = dir.x * cs - dir.z * sn;
    dir1.z = dir.x * sn + dir.z * cs;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        CameraData.camPos[2] -= dir1[2] * speed;
        CameraData.camPos[0] -= dir1[0] * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        CameraData.camPos[2] += dir1[2] * speed;
        CameraData.camPos[0] += dir1[0] * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        CameraData.camPos[2] += dir[2] * speed;
        CameraData.camPos[0] += dir[0] * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        CameraData.camPos[2] -= dir[2] * speed;
        CameraData.camPos[0] -= dir[0] * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        CameraData.direction[1] -= speed;
        CameraData.camPos[1] -= speed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        CameraData.direction[1] += speed;
        CameraData.camPos[1] += speed;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}