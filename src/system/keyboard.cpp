#include "keyboard.h"

#include <cstdint>

namespace resl {

/* 262d:6ef9 : 1 byte */
std::uint8_t g_lastKeyPressed;

/* 262d:6f11 : 1 byte */
std::uint8_t g_lastKeyCode;

//-----------------------------------------------------------------------------

/* 14af:06e7 */
void keyboardInterruptionHandler(std::uint8_t keycode)
{
    // The original implementation reads the data from the ports 0x60, 0x61
    // http://www.techhelpmanual.com/106-int_09h__keyboard_interrupt.html

    if (!(keycode & g_keyReleasedFlag))
        g_lastKeyPressed = keycode;
    g_lastKeyCode = keycode;
}

/* 1c71:000f */
void updateKeyboardLeds(std::int16_t)
{
    // In the original game, this function updates the state of keyboard LEDs.
    // This feature is non-portable and is not important at all
    // => reSL doesn't implement this
}

} // namespace resl
