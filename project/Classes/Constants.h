//
//  Constants.h
//  ColorSort-desktop
//
//  Created by hoangbui-fe on 8/8/26.
//

#ifndef Constants_h
#define Constants_h

namespace ColorSort
{
    constexpr float DESIGN_WIDTH = 800.0f;
    constexpr float DESIGN_HEIGHT = 600.0f;

    constexpr float BIN_WIDTH = 200.0f;
    constexpr float BIN_HEIGHT = 100.0f;

    constexpr float RED_BIN_X = 50.0f;
    constexpr float GREEN_BIN_X = 300.0f;
    constexpr float BLUE_BIN_X = 550.0f;
    constexpr float BIN_Y = 0.0f;

    constexpr float BLOCK_SIZE = 40.0f;
    constexpr float BLOCK_HALF_SIZE = BLOCK_SIZE * 0.5f;
    constexpr float BLOCK_FALL_SPEED = 100.0f;

    constexpr float SPAWN_INTERVAL = 2.0f;
    constexpr float SPAWN_Y = DESIGN_HEIGHT;
    constexpr float SPAWN_MIN_X = BLOCK_HALF_SIZE;
    constexpr float SPAWN_MAX_X = DESIGN_WIDTH - BLOCK_HALF_SIZE;

    constexpr int WIN_SCORE = 15;

    constexpr int SCORE_FONT_SIZE = 28;
    constexpr int MESSAGE_FONT_SIZE = 48;
}
#endif /* Constants_h */
