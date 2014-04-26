#ifndef GAMEWINDOW_H
#define	GAMEWINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <time.h>

#include "src/Entity.h"


class Ground : public Entity{
public:
    Ground(){;}
    virtual ~Ground(){;}
    std::vector<Entity*> BlockX;
};

class GameWindow {
public:
    GameWindow();
    void Init(const std::string &title, const int WIDTH, const int HEIGHT);
    void PopulateWorld(unsigned int TileAmtX, unsigned int TileAmtY);
    void Update(SDL_Event e);
    void Render();
    void Quit();
    void CalcDelta();
    
    void ApplySurface(int x, int y, SDL_Texture* tex, SDL_Renderer* rend);
    
    bool GetCloseRequested(){return CloseRequested;}
    bool GetCompleteSuccess(){return CompleteSuccess;}
    SDL_Renderer* GetRenderer(){return m_Renderer;}
    
    unsigned int GetDelta(){return delta;}
    
    static SDL_Texture* LoadImage(const std::string &filePath,SDL_Renderer* renderer){
        SDL_Texture *texture = NULL;
        texture = IMG_LoadTexture(renderer, filePath.c_str());
        if(texture == NULL)
        {
            std::cerr << "Error 5: Failed to load Texture" << std::endl;
        }
        return texture;
    }
    
    virtual ~GameWindow();
private:
    SDL_Window *m_Window = NULL;
    SDL_Renderer *m_Renderer = NULL;
    
    unsigned int delta;
    
    bool CloseRequested, CompleteSuccess, GameOver, GamePlay, MenuScreen, HelpScreen;
    std::vector<Ground*> BlocksY;
    std::vector<Entity*> Worms;
    std::vector<Entity*> LizardWorms;
    std::vector<Entity*> Reward;
    Entity* player; //Player
    

};

#endif	/* GAMEWINDOW_H */

