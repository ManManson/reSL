#pragma once

#include "types/chunk.h"
#include "types/rail_info.h"
#include <game/resources/semaphore_glyph.h>

#include <cstdint>

namespace resl {

enum class SemaphoreType : std::int16_t {
    RightUp = 0,
    RightDown = 1,
    LeftUp = 2,
    LeftDown = 3,
    None = -1
};

struct Semaphore {
    Chunk* chunk;
    SemaphoreType type;
    bool isRed;
    bool isRightDirection;
    std::int16_t pixelX;
    std::int16_t pixelY;
    const SemaphoreGlyph* glyph;
};

//-----------------------------------------------------------------------------

/* 262d:6f9c : 48 bytes */
extern Semaphore x_newSemaphores[4];

/* 262d:6f60 : 48 bytes */
extern Semaphore x_erasedSemaphores[4];

/* 262d:6f90 : 2 bytes */
extern std::int16_t x_newSemaphoreCount;

/* 262d:6f92 : 2 bytes */
extern std::int16_t x_erasedSemaphoreCount;

/* 262d:58aa : 600 bytes */
extern Semaphore g_semaphores[50];

/* 262d:21d6 : 2 bytes */
extern std::uint16_t g_semaphoreCount;

//-----------------------------------------------------------------------------

/* 17bf:07dd */
void createSemaphores(const RailInfo&);

/* 137c:0135 */
void drawSemaphore(const Semaphore&, std::int16_t yOffset);

/* 19de:0414 */
void toggleSemaphore(Semaphore&);

/* 19de:038e */
Semaphore* findClosestSemaphore(std::int16_t x, std::int16_t y);

} // namespace resl
