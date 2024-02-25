#include "keyboard.hpp"

namespace Vulkan
{

// constexpr int Keyboard::NUMBER_OF_KEYBOARD_KEYS = 348; 
// constexpr int Keyboard::NUMBER_OF_MOUSE_BUTTONS = 7;

std::array<bool, Keyboard::NUMBER_OF_KEYBOARD_KEYS> Keyboard::keyboard_keys{};
std::array<bool, Keyboard::NUMBER_OF_MOUSE_BUTTONS> Keyboard::mouse_buttons{};

} // end of Vulkan namespace