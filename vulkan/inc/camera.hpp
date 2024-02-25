#ifndef CAMERA_HPP_
#define CAMERA_HPP_
#include <iostream>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

namespace Vulkan
{

class Camera 
{
    private:
        float speed = 1;

        // glm::mat4 projectionMatrix{1.f};
        // glm::mat4 viewMatrix{1.f};
        // glm::mat4 inverseViewMatrix{1.f};

    public:
        double xpos;
        double ypos;

        glm::vec3 determine_move(/*const std::array<bool, NUMBER_OF_KEYBOARD_KEYS>& keyboard_keys*/);
        glm::vec3 determine_rotate(/*const std::array<bool, NUMBER_OF_MOUSE_BUTTONS>& mouse_buttons,*/ double prev_x, double prev_y);

        glm::vec3 viewer_position = glm::vec3(0.0f, 0.0f, 0.0f);
        // glm::vec3 view_position =  glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 camera_direction = glm::normalize (glm::vec3 {1.0f, 1.0f, 1.0f});
        glm::vec3 camera_up = glm::vec3(0.0f, 0.0f, 1.0f);

        // const glm::mat4& get_projection() const { return projectionMatrix; }
        // const glm::mat4& get_view() const { return viewMatrix; }
        // const glm::mat4& get_inverse_view() const { return inverseViewMatrix; }


        // glm::vec3 get_viewer_position()  const { return viewer_position; }
        // glm::vec3 get_view_position()    const { return view_position; }
        // glm::vec3 get_camera_direction() const { return camera_direction; }
        // glm::vec3 get_camera_up()        const { return camera_up; }
};

} // end of Vulkan namespcae

#endif