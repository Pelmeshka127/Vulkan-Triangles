#include "keyboard.hpp"

#include <iostream>

namespace Vulkan
{

//-------------------------------------------------------------------------------//

void Keyboard::MoveInPlainXZ(GLFWwindow* window, float dt, Object& object)
{
    std::cout << "Moving" << std::endl;
    glm::vec3 rotate{0};

    if (glfwGetKey(window, keys.LookRight) == GLFW_PRESS)   rotate.y += 1.f;
    if (glfwGetKey(window, keys.LookLeft) == GLFW_PRESS)    rotate.y -= 1.f;
    if (glfwGetKey(window, keys.LookUp) == GLFW_PRESS)      rotate.x += 1.f;
    if (glfwGetKey(window, keys.LookDown) == GLFW_PRESS)    rotate.x -= 1.f;

    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
        object.transform.rotation += look_spped_ * dt * glm::normalize(rotate);

    object.transform.rotation.x = glm::clamp(object.transform.rotation.x, -1.5f, 1.5f);

    object.transform.rotation.y = glm::mod(object.transform.rotation.y, glm::two_pi<float>());

    float yaw = object.transform.rotation.y;
    
    const glm::vec3 ForwardDir{sin(yaw), 0.f, cos(yaw)};
    
    const glm::vec3 RightDir{ForwardDir.z, 0.f, -ForwardDir.x};
    
    const glm::vec3 UpDir{0.f, -1.f, 0.f};

    glm::vec3 MoveDir{0.f};

    if (glfwGetKey(window, keys.MoveForward) == GLFW_PRESS)     MoveDir += ForwardDir;
    if (glfwGetKey(window, keys.MoveBackward) == GLFW_PRESS)    MoveDir -= ForwardDir;
    if (glfwGetKey(window, keys.MoveRight) == GLFW_PRESS)       MoveDir += RightDir;
    if (glfwGetKey(window, keys.MoveLeft) == GLFW_PRESS)        MoveDir -= RightDir;
    if (glfwGetKey(window, keys.MoveUp) == GLFW_PRESS)          MoveDir += UpDir;
    if (glfwGetKey(window, keys.MoveDown) == GLFW_PRESS)        MoveDir -= UpDir;

    if (glm::dot(MoveDir, MoveDir) > std::numeric_limits<float>::epsilon())
        object.transform.translation += move_speed_ * dt * glm::normalize(MoveDir);

    camera.AddMovementAndRotation(MoveDir, rotate, ForwardDir, RightDir, UpDir);
}

//-------------------------------------------------------------------------------//

void Keyboard::OnUpdate(GLFWwindow* window, float dt, Object& object)
{
    glm::vec3 movement_delta{ 0, 0, 0 };
    glm::vec3 rotation_delta{ 0, 0, 0 };

    if (glfwGetKey(window, keys.MoveForward) == GLFW_PRESS)
        movement_delta.x += dt;
    
    if (glfwGetKey(window, keys.MoveDown) == GLFW_PRESS)
        movement_delta.x -= dt;
    
    if (glfwGetKey(window, keys.MoveLeft) == GLFW_PRESS)
        movement_delta.y -= dt;
    
    if (glfwGetKey(window, keys.MoveRight) == GLFW_PRESS)
        movement_delta.y += dt;
    
    if (glfwGetKey(window, keys.MoveUp) == GLFW_PRESS)
        movement_delta.z += dt;
    
    if (glfwGetKey(window, keys.MoveDown) == GLFW_PRESS)
        movement_delta.z -= dt;

    if (glfwGetKey(window, keys.LookUp))
        rotation_delta.y -= 0.5f;
    
    if (glfwGetKey(window, keys.LookDown))
        rotation_delta.y += 0.5f;
    
    if (glfwGetKey(window, keys.LookRight))
        rotation_delta.z -= 0.5f;
    
    if (glfwGetKey(window, keys.LookLeft))
        rotation_delta.z += 0.5f;

    if (glm::dot(rotation_delta, rotation_delta) > std::numeric_limits<float>::epsilon())
        object.transform.rotation += look_spped_ * dt * glm::normalize(rotation_delta);

    object.transform.rotation.x = glm::clamp(object.transform.rotation.x, -1.5f, 1.5f);

    object.transform.rotation.y = glm::mod(object.transform.rotation.y, glm::two_pi<float>());

    if (glm::dot(movement_delta, movement_delta) > std::numeric_limits<float>::epsilon())
        object.transform.translation += move_speed_ * dt * glm::normalize(movement_delta);

    // camera.AddMovementAndRotation(movement_delta, rotation_delta);
}

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace