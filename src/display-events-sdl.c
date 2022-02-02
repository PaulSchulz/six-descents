// display-events-sdl
//
// Quesry the SQLite3 database and display total events.
//
// Author: Paul Schulz <paul@mawsonlakes.org>

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

#include <sqlite3.h>

#define QUITKEY SDLK_ESCAPE

// Example default
// #define WIDTH  1024
// #define HEIGHT  768

// HDML Screen -
#define WIDTH  1920
#define HEIGHT 1080

// Small HDML Screen - will scale output
// #define WIDTH    960
// #define HEIGHT   540

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

// Database
#define EVENTSDB "/home/pi/Documents/git/six-descents-data/events.db"
sqlite3 *db;
int channel[6];

//////////////////////////////////////////////////////////////////////////////
/* returns a number between 1 and max */
int Random (int max) {
    return (rand() % max) + 1;
}

void LogError (char* msg) {
    printf("%s\n", msg);
    errorCount++;
}

/* Sets Window caption according to state - eg in debug mode or showing fps */
void SetCaption (char* msg) {
  SDL_SetWindowTitle(screen, msg);
}

/* Initialize all setup, set screen mode, load images etc */
int InitSetup (void) {

  // Database Initialisation
  int rc = sqlite3_open(EVENTSDB, &db);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);

    return 1;
  }

  // Data Initialisation
  for(int i=0; i<6; i++) {
    channel[i] = 0;
  }

  // SDL and Graphics Initialisation
  // Seed random number generator
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

  // Hide Mouse
  SDL_ShowCursor(SDL_DISABLE);

  // Opens a font style and sets a size
  // Sans = TTF_OpenFont(FONT, 100);
  Sans = TTF_OpenFont(FONT, 200);

  // Initialise count data
  for (int i=0; i<6; i++) {
    channel[i] = 0;
  }

  SetCaption("Example One");

  return 0;
}

/* Cleans up after game over */
void FinishOff() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(screen);

  //Quit SDL
  SDL_Quit();

  // Close Database
  sqlite3_close(db);

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
//////////////////////////////////////////////////////////////////////////////
void DrawString (char* buffer, int x, int y) {

  SDL_Color    White = {255, 255, 255};
  SDL_Surface* surfaceMessage = NULL;
  SDL_Texture* Message = NULL;
  SDL_Rect     Message_rect; //create a rect

  surfaceMessage = TTF_RenderText_Solid(Sans, buffer, White);
  Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
  Message_rect.x = x;
  Message_rect.y = y;
  Message_rect.w = surfaceMessage->w;
  Message_rect.h = surfaceMessage->h;
  SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);

}


void DrawScreen () {

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

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderFillRect(renderer, NULL);

  // Get Data
  sqlite3_stmt *res;
  int data_ok = false;
  int rc = sqlite3_prepare_v2(db,
                              "SELECT channel, count from totals",
                              -1, &res, NULL);

  if (rc == SQLITE_OK) {
      while (sqlite3_step(res) == SQLITE_ROW) {
          channel[sqlite3_column_int(res, 0) - 1] = sqlite3_column_int(res, 1);
      }

      data_ok = true;
  }
  sqlite3_finalize(res);

  // Display Data
  if (data_ok == true) {

      int total = 0;
      for(int i=0; i<6; i++) {
          total = total + channel[i];
      }

      char message_buff[100];
      SDL_Color    White = {255, 255, 255};
      SDL_Surface* surfaceMessage = NULL;
      SDL_Texture* Message = NULL;
      SDL_Rect     Message_rect; //create a rect

      snprintf(message_buff, sizeof(message_buff), "%s", "Total:");
      // DrawString (message_buff, 40, 20);
      DrawString (message_buff, 80, 40);

      snprintf(message_buff, sizeof(message_buff), "%08d", total);
      // DrawString (message_buff, 460, 20);
      DrawString (message_buff, 920, 40);

      for (int i=0; i<3; i++) {

          snprintf(message_buff, sizeof(message_buff), "%07d", channel[i]);
          // DrawString (message_buff, 40, 180 + i*100);
          DrawString (message_buff, 80, 360 + i*200);

          snprintf(message_buff, sizeof(message_buff), "%07d", channel[i+3]);
          // DrawString (message_buff, 520, 180 + i*100);
          DrawString (message_buff, 1040, 360 + i*200);
      }

      SDL_RenderPresent(renderer);
  }
}


/* main game loop. Handles demo mode, high score and game play */
void GameLoop() {
  int gameRunning = 1;
  startTimer(&s);
  while (gameRunning) {
    DrawScreen();

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
	keypressed = event.key.keysym.sym;
	if (keypressed == QUITKEY) {
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

int main(int argc, char* args[]) {
  InitSetup();
  GameLoop();
  FinishOff();
  return 0;
}
