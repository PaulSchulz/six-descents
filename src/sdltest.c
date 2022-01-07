// sdltest.c
#include<linux/time.h>
#define __timespec_defined 1
#define __timeval_defined 1
#define __itimerspec_defined 1
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "hr_time.h"


#define QUITKEY SDLK_ESCAPE

// Example default
// #define WIDTH  1024
// #define HEIGHT  768

// HDML Screen - 
// #define WIDTH  1920
// #define HEIGHT 1080

// HDML Screen - will scale output
#define WIDTH    960
#define HEIGHT   540

#define FONT  "fonts/NovaMono-Regular.ttf"

SDL_Window*   screen = NULL;
SDL_Renderer* renderer;
SDL_Event     event;
SDL_Rect      source, destination, dst;

int errorCount = 0;
int keypressed;
int rectCount = 0;
stopWatch s;

// Font Handling
TTF_Font* Sans = NULL;
TTF_Font* Font = NULL;

// Data
int channel[6];



///////////////////////////////////////////////////////////////////////////////////////////
/* returns a number between 1 and max */
int Random(int max) {
  return (rand() % max) + 1;
}

void LogError(char* msg) {
  printf("%s\n", msg);
  errorCount++;
}

/* Sets Window caption according to state - eg in debug mode or showing fps */
void SetCaption(char* msg) {
  SDL_SetWindowTitle(screen, msg);
}

/* Initialize all setup, set screen mode, load images etc */
void InitSetup() {
  srand((int)time(NULL));
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_SHOWN, &screen, &renderer);
  if (!screen) {
    LogError("InitSetup failed to create window");
  }

  if(TTF_Init()==-1) {
    printf("TTF_Init: %s\n", TTF_GetError());

    exit(2);
  }

  // Opens a font style and sets a size
  Sans = TTF_OpenFont(FONT, 100);

  // Initialise count data
  for (int i=0; i<6; i++) {
    channel[i] = 0;
  }

  SetCaption("Example One");
}

/* Cleans up after game over */
void FinishOff() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(screen);
  //Quit SDL
  SDL_Quit();
  exit(0);
}

/* read a character */
char getaChar() {
  int result = -1;
 
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN)
      {
	result = event.key.keysym.sym;
	break;
      }
  }
  return result;
}

void DrawText() {
  char buff[20];

  int background = false;
  if (background == true) {
    SDL_Rect rect;
    SDL_SetRenderDrawColor(renderer, Random(256) - 1, Random(256) - 1, Random(256) - 1, 255);
    rect.h = 120;// Random(100) + 20;
    rect.w = 120;// Random(100) + 20;
    rect.y = Random(HEIGHT - rect.h - 1);
    rect.x = Random(WIDTH - rect.w - 1);
    SDL_RenderFillRect(renderer, &rect);
  }
  
  char message_buff[100];
  //
  SDL_Color    White = {255, 255, 255};
  SDL_Surface* surfaceMessage = NULL;
  SDL_Texture* Message = NULL;
  SDL_Rect     Message_rect; //create a rect

  snprintf(message_buff, sizeof(message_buff), "%s", "Total:");
  surfaceMessage = TTF_RenderText_Solid(Sans, message_buff, White); 
  Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
  Message_rect.x = 20; 
  Message_rect.y = 20; 
  Message_rect.w = surfaceMessage->w;
  Message_rect.h = surfaceMessage->h;
  SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);

  snprintf(message_buff, sizeof(message_buff), "%08d", 0);
  surfaceMessage = TTF_RenderText_Solid(Sans, message_buff, White); 
  Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
  Message_rect.x = 400;
  Message_rect.y = 20; 
  Message_rect.w = surfaceMessage->w;
  Message_rect.h = surfaceMessage->h;
  SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);

  for (int i=0; i<3; i++) {
    
    snprintf(message_buff, sizeof(message_buff), "%07d", 0);
    surfaceMessage = TTF_RenderText_Solid(Sans, message_buff, White); 
    Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    Message_rect.x = 20; 
    Message_rect.y = 200 + i*100;
    Message_rect.w = surfaceMessage->w;
    Message_rect.h = surfaceMessage->h;
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);

    snprintf(message_buff, sizeof(message_buff), "%07d", 0);
    surfaceMessage = TTF_RenderText_Solid(Sans, message_buff, White); 
    Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    Message_rect.x = 520;   
    Message_rect.y = 200 + i*100;
    Message_rect.w = surfaceMessage->w;
    Message_rect.h = surfaceMessage->h;
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
    
  }
  
  rectCount++;
  if (rectCount % 100 == 0) {
    SDL_RenderPresent(renderer);
    stopTimer(&s);
    snprintf(buff, sizeof(buff),"%10.6f",diff(&s));
    SetCaption(buff);
    startTimer(&s);
  }
}

/* main game loop. Handles demo mode, high score and game play */
void GameLoop() {
  int gameRunning = 1;
  startTimer(&s);
  while (gameRunning)
    {
      DrawText();
      
      while (SDL_PollEvent(&event)) {
	switch (event.type) {
	case SDL_KEYDOWN:
	  keypressed = event.key.keysym.sym;
	  if (keypressed == QUITKEY)
	    {
	      gameRunning = 0;
	      break;
	    }
	  
	  break;
	case SDL_QUIT: /* if mouse click to close window */
	  {
	    gameRunning = 0;
	    break;
	  }
	case SDL_KEYUP: {
	  break;
	}
	} /* switch */
	
      } /* while SDL_PollEvent */
    }
}

int main(int argc, char* args[])
{
  InitSetup();
  GameLoop();
  FinishOff();
  return 0;
}
