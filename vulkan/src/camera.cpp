#include <GLFW/glfw3.h>
#include "camera.hpp"
#include "keyboard.hpp"

namespace Vulkan
{

//-------------------------------------------------------------------------------//

void Camera::determineMove(const float dt) 
{
    glm::vec3 movement{0.0f, 0.0f, 0.0f};

    if (Keyboard::isKeyboardKey(GLFW_KEY_W))
        movement += glm::normalize(direction);

    if (Keyboard::isKeyboardKey(GLFW_KEY_S))
        movement -= glm::normalize(direction);

    if (Keyboard::isKeyboardKey(GLFW_KEY_A))
        movement -= glm::normalize(glm::cross (direction, up));

    if (Keyboard::isKeyboardKey(GLFW_KEY_D))
        movement += glm::normalize(glm::cross (direction, up));

    if (Keyboard::isKeyboardKey(GLFW_KEY_Q))
        movement -= glm::normalize(up);

    if (Keyboard::isKeyboardKey(GLFW_KEY_E))
        movement += glm::normalize(up);

    if (glm::dot(movement, movement) > std::numeric_limits<float>::epsilon())
        position += movement * speed * dt;

    if (Keyboard::isKeyboardKey(GLFW_KEY_P))
        speed += 1.f;

    if (Keyboard::isKeyboardKey(GLFW_KEY_O))
        speed -= 1.f;

    if (speed <= 0)
        speed = 0;
}

//-------------------------------------------------------------------------------//

void Camera::determineRotate(const float dt) 
{
    glm::vec3 rotation_delta{0.0f, 0.0f, 0.0f};

    if (Keyboard::isKeyboardKey(GLFW_KEY_UP))
        rotation_delta.y -= 1.f;
    
    if (Keyboard::isKeyboardKey(GLFW_KEY_DOWN))
        rotation_delta.y += 1.f;

    if (Keyboard::isKeyboardKey(GLFW_KEY_RIGHT))
        rotation_delta.z -= 1.f;
    
    if (Keyboard::isKeyboardKey(GLFW_KEY_LEFT))
        rotation_delta.z += 1.f;

    if (glm::dot(rotation_delta, rotation_delta) > std::numeric_limits<float>::epsilon())
        rotation += look_speed * dt * glm::normalize(rotation_delta);
}

//-------------------------------------------------------------------------------//

void Camera::updateViewMatrix()
{
    const float roll_in_radians  = glm::radians(rotation.x);
    const float pitch_in_radians = glm::radians(rotation.y);
    const float yaw_in_radians   = glm::radians(rotation.z);

    const glm::mat3 rotate_matrix_x(
        1, 0, 0,
        0,  std::cos(roll_in_radians), std::sin(roll_in_radians),
        0, -std::sin(roll_in_radians), std::cos(roll_in_radians)
    );

    const glm::mat3 rotate_matrix_y(
        std::cos(pitch_in_radians), 0, -std::sin(pitch_in_radians),
        0, 1, 0,
        std::sin(pitch_in_radians), 0, std::cos(pitch_in_radians)
    );

    const glm::mat3 rotate_matrix_z(
        std::cos(yaw_in_radians), std::sin(yaw_in_radians), 0,
        -std::sin(yaw_in_radians), std::cos(yaw_in_radians), 0,
        0, 0, 1
    );

    const glm::mat3 euler_rotate_matrix = rotate_matrix_z * rotate_matrix_y * rotate_matrix_x;
    
    direction = glm::normalize(euler_rotate_matrix * s_world_forward);
    
    right = glm::normalize(euler_rotate_matrix * s_world_right);
    
    up = glm::cross(right, direction);

    view_matrix = glm::lookAt(position, position + direction, up);
}

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace