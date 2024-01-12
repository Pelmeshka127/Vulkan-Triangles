#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace Vulkan
{

class Camera
{

    public:

        void SetOrthographProjection(float left, float right, float top, float bottom, float near, float far);

        void SetPerspectiveProjection(float fovy, float aspect, float near, float far);

        const glm::mat4& GetProjection() const { return projection_matrix_; }

    private:

        glm::mat4 projection_matrix_{1.f};

};

} // end of Vulkan namespace

#endif