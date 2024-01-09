#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include "model.hpp"

#include <memory>

namespace Vulkan 
{

struct Transform2dComponent {
    glm::vec2 translation{};
    glm::vec2 scale{1.f, 1.f};
    float rotation;

    glm::mat2 mat2()
    {
        const float sinus   = glm::sin(rotation);

        const float cosinus = glm::cos(rotation);

        glm::mat2 RotMatrix{{cosinus, sinus}, {sinus, cosinus}};

        glm::mat2 ScaleMatrix{{scale.x, .0f}, {.0f, scale.y}};

        return RotMatrix * ScaleMatrix;
    }
};

class Object
{
    using id_t = unsigned int;

    private:

        id_t id;

        Object(id_t obj_id) : id{obj_id} {}

    public:

        static Object CreateObject()
        {
            static id_t current_id = 0;

            return Object(current_id++);
        }

        id_t GetId() const { return id; }

        std::shared_ptr<Model> model{};
        
        glm::vec3 color{};
        
        Transform2dComponent transform2d{};
};

} // end of vulkan namespace

#endif