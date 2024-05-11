#include "game/init.h"

#include "game/drawing.h"
#include "game/game_data.h"
#include "game/load_game.h"
#include "game/move_trains.h"
#include "game/records.h"
#include "game/resources/train_glyph.h"
#include "graphics/color.h"
#include "graphics/drawing.h"
#include "graphics/driver.h"
#include "graphics/glyph.h"
#include "graphics/text.h"

#include <cstdlib>
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>

using namespace resl;

namespace {

std::function<void()> frameFunction;

void testRecordsScreen(int, const char*[])
{
    showRecordsScreen();
}

void testDrawText(int, const char*[])
{
    char buf[148];
    unsigned c = 0;
    for (int y = 0; y < 10; ++y) {
        char buf[20];
        int x = 0;
        for (x = 0; x < 15 && c <= 146; ++x) {
            buf[x] = c;
            ++c;
        }
        drawText(10, 40 + y * 20, buf, Red);

        if (c > 146)
            break;
    }
}

void testLoadGame(int argc, const char* argv[])
{
    const char* fname;
    if (argc != 3) [[unlikely]] {
        std::cout << "The game save file name is not set explicitly - loading DEMO_A file" << std::endl;
        std::cout << "You can pass the file name through the additional command line argument" << std::endl;
        fname = "DEMO_A";
    } else
        fname = argv[2];

    if (!std::filesystem::is_regular_file(fname)) [[unlikely]] {
        std::cerr << "specified file \"" << fname << "\" doesn't exist" << std::endl;
        return;
    }
    if (!std::filesystem::is_regular_file("PLAY.7")) {
        std::cerr
            << "!!!WARNING!!! The PLAY.7 file was not found!\n"
               "Please, copy PLAY.7 file from the original game folder to the application folder. "
               "Otherwise you will see a mess in the game header area"
            << std::endl;
    }

    initGameData();

    fillGameFieldBackground(0);

    char switchStatesBuf[100];
    char buf2[100];

    resetGameData();
    loadSavedGame(fname);

    drawWorld();

    frameFunction = [lastFrame = std::clock()]() mutable {
        std::clock_t curTime = std::clock();
        if (lastFrame != std::clock_t() && 60 * (curTime - lastFrame) < CLOCKS_PER_SEC)
            return;
        std::int16_t dTime = (80 * (curTime - lastFrame)) / CLOCKS_PER_SEC;
        std::cout << "FPS: " << (CLOCKS_PER_SEC / (curTime - lastFrame)) << std::endl
                  << "dtime: " << dTime << std::endl;
        lastFrame = curTime;

        for (Train& train : trains) {
            if (!train.isFreeSlot)
                moveTrain(train, dTime);
        }
        drawWorld();
    };
}

void testDrawTrains(int, const char*[])
{
    static Color colors[5][2] = {
        { Blue,       DarkBlue  },
        { Red,        DarkRed   },
        { White,      Gray      },
        { Gray,       DarkGray  },
        { LightGreen, DarkGreen }
    };

    frameFunction = [currentAngle = 0, currentDirection = 0, lastFrame = std::clock()]() mutable {
        std::clock_t curTime = std::clock();
        if (lastFrame != std::clock_t() && curTime - lastFrame < CLOCKS_PER_SEC / 5)
            return;
        lastFrame = curTime;

        drawing::filledRectangle(0, 0, 640, 480, 0xFF, DarkGreen);

        int x = 80;
        int y = 30;
        for (int i = 0; i < 15; ++i) {
            const TrainGlyph* g = &g_trainGlyphs[i][currentAngle][currentDirection];
            Color* c = colors[i % std::size(colors)];
            drawGlyph(g->glyph1, x - g->width / 2, y - g->height / 2, Black);
            drawGlyph(g->glyph2, x - g->width / 2, y - g->height / 2, c[0]);
            drawGlyph(g->glyph3, x - g->width / 2, y - g->height / 2, c[1]);
            char buf[100];
            snprintf(buf, sizeof(buf), "%d", i);
            drawText(x, y + 20, buf, Red);
            x += 80;
            if (x > 590) {
                x = 80;
                y += 60;
            }
        }

        if (++currentAngle >= 5) {
            currentDirection = 1 - currentDirection;
            currentAngle = 0;
        }
    };
}

const std::map<std::string, std::function<void(int, const char*[])>> commands = {
    { "records",        testRecordsScreen },
    { "testDrawText",   testDrawText      },
    { "testLoadGame",   testLoadGame      },
    { "testDrawTrains", testDrawTrains    }
};

int usage(const char* prog, const char* unknownArg = nullptr)
{
    if (unknownArg)
        std::cerr << "unknown argument: " << unknownArg << std::endl;

    std::cout << "usage: " << prog
              << " <mode> <arguments>\n"
                 "available modes:"
              << std::endl;
    for (auto& [cmd, _] : commands)
        std::cout << '\t' << cmd << std::endl;

    return EXIT_FAILURE;
}

} // namespace

int main(int argc, const char* argv[])
{
    graphics_init();

    if (argc < 2)
        return usage(*argv);

    auto iter = commands.find(argv[1]);
    if (iter == commands.end())
        return usage(argv[0], argv[1]);

    iter->second(argc, argv);

    while (poll_event()) {
        if (frameFunction)
            frameFunction();
        graphics_update();
    }

    graphics_close();

    return EXIT_SUCCESS;
}
