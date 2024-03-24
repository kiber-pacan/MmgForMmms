//
//Small file for controlling app with keys
//

//Cam data
camData CameraData = camData();

//Mouse position
double xpos, ypos, lasty, lastx;
float pitch, yaw;
const float sensitivity = 0.1f;
bool firstMouse = true;

//Movement
float runningBonus = 4;
float speed = 0.005f;

float theta = glm::radians(90.f);

glm::vec3 dir;
glm::vec3 dir1;

//Cursor
bool F1_released = true;
bool cursorEnabled = true;


class input {

public:
    input(GLFWwindow* window) {
        glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));
    }

    //key callback        
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
            speed *= runningBonus;
        }
        else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) {
            speed /= runningBonus;
        }

        if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
            if (cursorEnabled) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                cursorEnabled = false;
            }
            else if (!cursorEnabled) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                cursorEnabled = true;
            }
        }
    }
};



static void GLFWkeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    
    input data = input(window);
    data.keyCallback(window, key, scancode, action, mods);
}


void keyCallback(GLFWwindow* window, float deltaTime) {

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
        CameraData.camPos[2] -= dir1[2] * (speed * deltaTime);
        CameraData.camPos[0] -= dir1[0] * (speed * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        CameraData.camPos[2] += dir1[2] * (speed * deltaTime);
        CameraData.camPos[0] += dir1[0] * (speed * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        CameraData.camPos[2] += dir[2] * (speed * deltaTime);
        CameraData.camPos[0] += dir[0] * (speed * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        CameraData.camPos[2] -= dir[2] * (speed * deltaTime);
        CameraData.camPos[0] -= dir[0] * (speed * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        CameraData.direction[1] -= (speed * deltaTime);
        CameraData.camPos[1] -= (speed * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        CameraData.direction[1] += (speed * deltaTime);
        CameraData.camPos[1] += (speed * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

//GLFW mouse callback
void mouseCallback(GLFWwindow* window) {
    if (cursorEnabled == true) {
        firstMouse = true;
        return;
    }
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