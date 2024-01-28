#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include "model.hpp"

#include <memory>
#include <glm/gtc/matrix_transform.hpp>

namespace Vulkan 
{

struct TransformComponent {
    glm::vec3 translation{};
    glm::vec3 scale{1.f, 1.f, 1.f};
    glm::vec3 rotation{};

    glm::mat4 mat4()
    {
        auto transform = glm::translate(glm::mat4(1.f), translation);

        transform = glm::rotate(transform, rotation.y, {1.f, 0.f, 0.f});

        transform = glm::rotate(transform, rotation.x, {0.f, 0.f, 1.f});

        transform = glm::rotate(transform, rotation.z, {1.f, 0.f, 0.f});

        transform = glm::scale(transform, scale);

        return transform;
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
        
        TransformComponent transform{};
};

} // end of vulkan namespace

#endif