/*
 *
 * -- Ludum Dare 29 entry, courtesy of DanqueDynasty at Swing Innovations --
 *                      -- @DanqueDynasty --
 * Protected under the DoWhatEverTheHellYouWantIDon'tCare License
 * 
 */

#include <iostream>
#include "GameWindow.h"

const std::string TITLE = "LD29::Beneath";
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

int main(int argc, char** argv) {
    bool IsRunning = true;
    GameWindow *game = new GameWindow();
    std::cout << "Hello World!" << std::endl;
    game->Init(TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
    game->PopulateWorld((int)(SCREEN_WIDTH * 3), (int)(SCREEN_HEIGHT * 3));
    game->PopulateEnemies(5);
    game->PopulateReward(7);
    while(IsRunning)
    {
        SDL_Event e;
        game->Update(e);
        if(game->GetCloseRequested())
        {
            IsRunning = false;
        }
        game->Render();
    }
    game->Quit();
    
    std::cout << SDL_GetError() << std::endl;
    
    return 0;
}

