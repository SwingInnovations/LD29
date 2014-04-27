#include "GameWindow.h"

GameWindow::GameWindow() {
    
}

void GameWindow::Init(const std::string& title, const int WIDTH, const int HEIGHT) 
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        std::cerr << "Error 1: Failed to init SDL: " << SDL_GetError() << std::endl;
    }else{
        m_Window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
        if(m_Window == NULL)
        {
            std::cerr << "Error 2: Failed to load Window!" << SDL_GetError() << std::endl;
        }else{
            m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(m_Renderer == NULL)
            {
                std::cerr << "Error 3: Failed to load Renderer" << SDL_GetError() << std::endl;
            }
        }
    }
    //Initialize other features
    CloseRequested = false;
    CompleteSuccess = false;
    GameOver = false;
    GamePlay = false;
    MenuScreen = true;
    HelpScreen = false;
    
    MAX_REWARD = 0;
    rewardAmt = 0;
    
    player = new Entity(Vector2D( 64.0, 8.0), Vector2D(30, 48));
    player->SetHealth(80);
    playerFoot = new Entity(Vector2D(player->GetPositionVector().GetX(), player->GetPositionVector().GetY() + player->GetDimensionVector().GetY()-2), Vector2D(30, 2));
    playerFoot->SetOnGround(false);
}

void GameWindow::PopulateWorld(unsigned int TileAmtX, unsigned int TileAmtY)
{
    unsigned int time_ui = static_cast<unsigned int>(time(NULL));
    srand(time_ui);
    
    unsigned int Tx = TileAmtX / 64;
    unsigned int Ty = TileAmtY / 64;
    std::cout << "Tile X Amount: " << Tx << "Tile Y Amount: " << Ty << std::endl;
    
    BlocksY.clear();
    
    for(unsigned int i = 0; i < Ty; i++)
    {
        BlocksY.push_back(new Ground());
        for(unsigned int j = 0; j < Tx; j++)
        {
            if(BlocksY[i]->BlockX.empty()){
                BlocksY[i]->BlockX.push_back(new Entity(Vector2D( 0.0, 
                                                             72.0),
                                                              Vector2D(64, 64)));
            }else{
                BlocksY[i]->BlockX.push_back(new Entity(Vector2D(BlocksY[i]->BlockX[j-1]->GetPositionVector().GetX() + BlocksY[i]->BlockX[j-1]->GetDimensionVector().GetX(), 
                                                                  72.0), Vector2D(64, 64)));
            }
        }
    }
    for(unsigned int k = 0; k < Ty; k++){
        for(unsigned int l = 0; l < Tx; l++){
            BlocksY[k]->BlockX[l]->SetPassable(false);
            BlocksY[k]->BlockX[l]->SetValuable(false);
            unsigned int RandX = rand() % Tx;
            unsigned int RandY = rand() % Ty;
            BlocksY[RandY]->BlockX[RandX]->SetPassable(true);
            if(BlocksY[k]->BlockX[l]->GetPassable()){
                unsigned int Vx = rand() % Tx;
                unsigned int Vy = rand() % Ty;
                BlocksY[Vy]->BlockX[Vx]->SetValuable(true);
            }
            if(k > 0)
            {
                BlocksY[k]->BlockX[l]->SetPosY(BlocksY[k-1]->BlockX[l]->GetPositionVector().GetY() + BlocksY[k-1]->BlockX[l]->GetDimensionVector().GetY());
            }
        }    
    }
}

void GameWindow::PopulateEnemies(unsigned int num)
{
    for(unsigned int i = 0; i < num; i++)
    {
        Worms.push_back(new Entity(Vector2D(0.0, 0.0), Vector2D(96.0, 48.0)));
        LizardWorms.push_back(new Entity(Vector2D(0.0, 0.0), Vector2D(96.0, 48.0)));
    }
    //TODO: Random Position
}

void GameWindow::PopulateReward(unsigned int num){
    for(unsigned int i = 0; i < num; i++)
    {
        Reward.push_back(new Entity(Vector2D(0.0, 0.0), Vector2D(32, 32)));
    }
    //TODO: Random Position
}

void GameWindow::Update(SDL_Event e)
{
    int x, y;
    SDL_GetMouseState(&x, &y); // get the Mouse position
    
    CalcDelta(); //Will help with maintaining stable framerate;
    
    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
        {
            CloseRequested = true;
        }
        
        if(!GameOver){
            if(!GamePlay){
                if(!MenuScreen && HelpScreen == true){
                    //handle Help Screen Actions
                    
                }else{
                    //handle Menu Actions
                    if(e.key.keysym.sym == SDLK_RETURN)
                    {
                        GamePlay = true;
                        MenuScreen = false;
                    }
                }
            }else{
                /*-Handle Gameplay Events-*/
                
                /*--Player Movement--*/
                if(e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_LEFT)
                {
                    float _x = player->GetPositionVector().GetX();
                    _x -= 0.25f * delta;
                    player->SetPosX(_x);
                    playerFoot->SetPosX(_x);
                }else{
                    float _x = player->GetPositionVector().GetX();
                    _x += 0.0;
                    player->SetPosX(_x);
                    playerFoot->SetPosX(_x);
                }
                if(e.key.keysym.sym == SDLK_d || e.key.keysym.sym == SDLK_RIGHT)
                {
                    float _x = player->GetPositionVector().GetX();
                    _x += 0.25f * delta;
                    player->SetPosX(_x);
                    playerFoot->SetPosX(_x);
                    std::cout << "Key Pressed" << std::endl;
                }else{
                    float _x = player->GetPositionVector().GetX();
                    _x += 0.0;
                    player->SetPosX(_x);
                    playerFoot->SetPosX(_x);
                }


                /*End of Handling events in Gameplay*/
            }
        }else{
            //Handle what happens if game is over
            
        }
        
    }
        if(!GameOver){
            if(!GamePlay){
                if(!MenuScreen && HelpScreen == true){
                    //handle Help Screen Actions
                    
                }else{

                }
            }else{
                /*-AI Behaviour-*/
                /*-Enemy Detection-*/
                if(!Worms.empty() && !LizardWorms.empty()){
                    for(unsigned int a = 0; a < Worms.size(); a++)
                    {
                        for(unsigned int b = 0; b < LizardWorms.size(); b++)
                        {
                            if(player->Intersected(*Worms[a]) || player->Intersected(*LizardWorms[b]))
                            {
                                player->SetHealth(0.0);
                                //GameOver = true;
                                //CloseRequested = true;
                            }
                        }
                    }
                }
                
                /*-Falling Behaviour-*/
                for(unsigned int i = 0; i < BlocksY.size(); i++){
                    for(unsigned int j = 0; j < BlocksY[0]->BlockX.size(); j++){
                        if(!playerFoot->GetGroundStatus()){
                            float _y = player->GetPositionVector().GetY();
                            _y += 0.0001 * delta;
                            player->SetPosY(_y);
                            playerFoot->SetPosY(_y + player->GetDimensionVector().GetY());
                        }else{
                            float _y = player->GetPositionVector().GetY();
                            _y += 0.0;
                            player->SetPosY(_y);
                            playerFoot->SetPosY(_y + player->GetDimensionVector().GetY());
                            //std::cout << "Player Y Position" << _y - playerFoot->GetDimensionVector().GetY() << std::endl;
                        }
                        if(BlocksY[i]->BlockX[j]->GetPassable())
                        {
                            if(playerFoot->Intersected(*BlocksY[i]->BlockX[j]))
                            {
                                playerFoot->SetOnGround(true);
                            }
                        } 
                    }
                }
                
                if(!Reward.empty())
                {
                    for(unsigned int i = 0; i < Reward.size(); i++){
                        if(player->Intersected(*Reward[i])){
                            Reward[i]->SetPosX(-50.0);
                            rewardAmt = rewardAmt + 1;
                        }
                        if(Reward.empty()){
                            GameOver = true;
                            CompleteSuccess = true;
                        }
                    }
                }
                /*End of Handling events in Gameplay*/
            }
        }else{
            //Handle what happens if game is over
            
        }
}

void GameWindow::Render()
{
    if(!GameOver){
        if(!GamePlay){
            if(!MenuScreen && HelpScreen == true)
            {
                //Render Help Screen 
                SDL_SetRenderDrawColor(m_Renderer, 255, 0, 0, 255);
                SDL_RenderClear(m_Renderer);
                    
                SDL_RenderPresent(m_Renderer);
            }else{
                //Render Menu Screen
                SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
                SDL_RenderClear(m_Renderer);
                    
                SDL_RenderPresent(m_Renderer);
            }
        }else{
            //Render GamePlayer Elements here
            SDL_SetRenderDrawColor(m_Renderer, 0, 0, 128, 255);
            SDL_RenderClear(m_Renderer);
            
            for(unsigned int i = 0, BlockYSize = BlocksY.size(); i < BlockYSize; i++){
                for(unsigned int j = 0, BlockXSize = BlocksY[0]->BlockX.size(); j < BlockXSize; j++)
                {
                    if(BlocksY[i]->BlockX[j]->GetPassable() == true)
                    {
                        SDL_SetRenderDrawColor(m_Renderer, 255, 0, 0, 255);
                        SDL_Rect rect = BlocksY[i]->BlockX[j]->GetRect();
                        SDL_RenderDrawRect(m_Renderer, &rect);
                    }else{
                        
                    }
                    
                    if(BlocksY[i]->BlockX[j]->GetValuable() == true)
                    {
                        SDL_SetRenderDrawColor(m_Renderer, 224, 169, 0, 255 );
                        SDL_Rect rect = BlocksY[i]->BlockX[j]->GetRect();
                        SDL_RenderDrawRect(m_Renderer, &rect);
                    }
                }
            }
            
            for(unsigned int r = 0; r < Reward.size(); r++)
            {
                SDL_SetRenderDrawColor(m_Renderer, 224, 169, 0, 255);
                SDL_Rect rect = Reward[r]->GetRect();
                SDL_RenderDrawRect(m_Renderer, &rect);
            }
            
            SDL_SetRenderDrawColor(m_Renderer, 0, 255, 0, 255);
            SDL_Rect playerRect = player->GetRect();
            SDL_RenderDrawRect(m_Renderer, &playerRect);
            SDL_SetRenderDrawColor(m_Renderer, 255, 255, 0, 255);
            SDL_Rect playerFootRect = playerFoot->GetRect();
            SDL_RenderDrawRect(m_Renderer, &playerFootRect);
            
            SDL_RenderPresent(m_Renderer);
        }
    }else{
        //Render Game Over Screen Here
    }
}

void GameWindow::Quit()
{
    for(unsigned int i = 0; i < BlocksY.size(); i++)
    {
        BlocksY[i]->BlockX.clear();
    }
    BlocksY.clear();
    Worms.clear();
    LizardWorms.clear();
    powerDrill = 0;
    pipeBomb.clear();
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    player = 0;
    playerFoot = 0;
    m_Renderer = 0;
    m_Window = 0;
    SDL_Quit();
}

void GameWindow::CalcDelta()
{
    delta = 0;
    Uint32 lastFrame, currentFrame;
    lastFrame = currentFrame;
    currentFrame = SDL_GetTicks();
    unsigned int tempDelta = (lastFrame - currentFrame)/100000000 - 12;
    std::cout << "Temp Delta: " << tempDelta << std::endl;
    if(tempDelta <= 30)
    {
        delta = tempDelta;
        //std::cout << "SDL_GetTicks: " << SDL_GetTicks() << std::endl;
        std::cout << delta << std::endl;
    }
}


bool GameWindow::CheckEnvironment( Entity& ent1,  Entity& ent2)
{
    SDL_Rect rect1 = ent1.GetRect();
    SDL_Rect rect2 = ent2.GetRect();
    
    if(SDL_HasIntersection(&rect1, &rect2) && ent2.GetPassable())
    {
        return true;
    }else{
        return false;
    }
}

void GameWindow::ApplySurface(int x, int y, SDL_Texture* tex, SDL_Renderer* rend)
{
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    
    SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
    SDL_RenderCopy(m_Renderer, tex, NULL, &pos);
}

GameWindow::~GameWindow() {
    
}

