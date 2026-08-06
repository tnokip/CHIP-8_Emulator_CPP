#include "../include/chip8.h"
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

chip8 myemu;

SDL_Window *window = NULL;
SDL_Renderer *renderer;

const float cycleDelay = 1000.0f / 500.0f;  //500Hz
const float timerDelay = 1000.0f / 60.0f ;  //60Hz

float cycleTimer = 0.0f;
float timerTimer = 0.0f;
U32 lastTimer = SDL_GetTicks();

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout<<"Couldn't initialize SDL: " << SDL_GetError();
        return SDL_APP_FAILURE; 
    }
    

    window = SDL_CreateWindow(
            "CHIP-8 EMULATOR", 
            1280,640, 
            SDL_WINDOW_MAXIMIZED );
    if(!window)
    {
        std::cout<<"Couldn't create window: " << SDL_GetError();
        return SDL_APP_FAILURE; 
            
    }
    

    renderer = SDL_CreateRenderer(window, nullptr);
    if(!renderer)
    {
        std::cout<<"Couldn't create renderer: " << SDL_GetError();
        return SDL_APP_FAILURE; 
    }


    myemu.LoadROM();


   return SDL_APP_CONTINUE; 
}



void draw()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //black pixels
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);   //white pixels
    const U32* disp = myemu.getDisplay();

    for(int y = 0; y< height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if(disp[y*width + x])
            {
                SDL_FRect rect {x*scale, y*scale, scale , scale}; 
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}



void render()
{
    
   
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
  

}




void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}




SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    switch(event->type)
    {
        case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
        break;

        case SDL_EVENT_KEY_DOWN:
        switch(event->key.key)
        {
            case SDLK_1: myemu.setKey(0x1,true); break;
            case SDLK_2: myemu.setKey(0x2,true); break;
            case SDLK_3: myemu.setKey(0x3,true); break;
            case SDLK_4: myemu.setKey(0xC,true); break;
            case SDLK_Q: myemu.setKey(0x4,true); break;
            case SDLK_W: myemu.setKey(0x5,true); break;
            case SDLK_E: myemu.setKey(0x6,true); break;
            case SDLK_R: myemu.setKey(0xD,true); break;
            case SDLK_A: myemu.setKey(0x7,true); break;
            case SDLK_S: myemu.setKey(0x8,true); break;
            case SDLK_D: myemu.setKey(0x9,true); break;
            case SDLK_F: myemu.setKey(0xE,true); break;
            case SDLK_Z: myemu.setKey(0xA,true); break;
            case SDLK_X: myemu.setKey(0x0,true); break;
            case SDLK_C: myemu.setKey(0xB,true); break;
            case SDLK_V: myemu.setKey(0xF,true); break;

        }
            break;

        case SDL_EVENT_KEY_UP:
        switch(event->key.key)
        {
            case SDLK_1: myemu.setKey(0x1,false); break;
            case SDLK_2: myemu.setKey(0x2,false); break;
            case SDLK_3: myemu.setKey(0x3,false); break;
            case SDLK_4: myemu.setKey(0xC,false); break;
            case SDLK_Q: myemu.setKey(0x4,false); break;
            case SDLK_W: myemu.setKey(0x5,false); break;
            case SDLK_E: myemu.setKey(0x6,false); break;
            case SDLK_R: myemu.setKey(0xD,false); break;
            case SDLK_A: myemu.setKey(0x7,false); break;
            case SDLK_S: myemu.setKey(0x8,false); break;
            case SDLK_D: myemu.setKey(0x9,false); break;
            case SDLK_F: myemu.setKey(0xE,false); break;
            case SDLK_Z: myemu.setKey(0xA,false); break;
            case SDLK_X: myemu.setKey(0x0,false); break;
            case SDLK_C: myemu.setKey(0xB,false); break;
            case SDLK_V: myemu.setKey(0xF,false); break;

        }
        break;
    }   
    return SDL_APP_CONTINUE;

}



SDL_AppResult SDL_AppIterate(void *appstate)
{
    U32 currentTime = SDL_GetTicks();
    float delta = currentTime - lastTimer;
    lastTimer = currentTime;

    cycleTimer += delta;
    timerTimer += delta;

    while(cycleTimer >= cycleDelay)
    {
        myemu.cycle();
        cycleTimer -= cycleDelay;

    

    if(drawflag)
    {
        draw();
        drawflag = false;
    }

    }
    //myemu.setTimer();

    return SDL_APP_CONTINUE;
}
