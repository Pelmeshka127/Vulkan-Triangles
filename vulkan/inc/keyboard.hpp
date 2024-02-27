#ifndef KEYBOARD_HPP_
#define KEYBOARD_HPP_

#include <iostream>
#include <array>

namespace Vulkan
{

//-------------------------------------------------------------------------------//

class Keyboard
{
    private:
        
        static const int NUMBER_OF_KEYBOARD_KEYS = 12; 

    public:

        static std::array<bool, NUMBER_OF_KEYBOARD_KEYS> keyboard_keys;

        static bool isKeyboardKey(int key)        { return keyboard_keys[key]; }
        
        static void pressKeyboardKey(int key)     { keyboard_keys[key] = true; }
        
        static void releaseKeyboardKey(int key)   { keyboard_keys[key] = false; }

};

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace

#endif