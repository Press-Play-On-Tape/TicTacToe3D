#pragma once

enum class GameState : uint8_t { 
    Splash, 
    Title, 
    Game, 
    GameOver,
    GameOver_View_1,
    GameOver_View_2,
    GameOver_View_3,
    EndState,
};

enum Players { 
    None = 0,
    Noughts = 1, 
    Crosses = 2, 
    Draw = 3,
};