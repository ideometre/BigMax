//DinoSmasher

#include <Arduboy2.h>
Arduboy2 arduboy;

#define GAME_TITLE  0
#define GAME_PLAY 1
#define GAME_OVER 2
#define GAME_HIGH 3
int gamestate = GAME_TITLE;

#define PLAYER_SIZE   16
int mapx = 0;
int mapy = 0;

const unsigned char tiles[4][32] PROGMEM  = {
  { 0xff, 0x7f, 0xfb, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xf7, 0xff, 0xfd, 0xff, 0xff, 0xf7, 0x7f, 0xff, 0xdf, 0xff, 0xff, 0xfb, 0x7f, 0xff, 0xff, 0xff, 0xef, 0xfe, 0xff, 0xff, 0xfb, 0xff, 0x7f, 0xff },
  { 0x08, 0x10, 0x10, 0x08, 0x10, 0x08, 0x10, 0x10, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x40, 0x20, 0x00, 0x01, 0x02, 0x02, 0x01, 0x02, 0x02, 0x01, 0x02, 0x21, 0x40, 0x40 },
  { 0xff, 0x1f, 0x5b, 0x3f, 0xeb, 0xdd, 0xff, 0xf7, 0xbb, 0xef, 0xfd, 0x7f, 0xe3, 0xcb, 0xe3, 0xff, 0xff, 0xc7, 0x96, 0xc7, 0xff, 0xff, 0xef, 0xfd, 0xff, 0xe3, 0xcb, 0xe3, 0xff, 0xff, 0x7b, 0xff },
  { 0xff, 0xdf, 0x7b, 0x3f, 0x9f, 0x6f, 0x77, 0xab, 0xdb, 0xd7, 0xcd, 0x5f, 0xbf, 0x77, 0xff, 0xff, 0xff, 0xc1, 0xdc, 0xd3, 0xaf, 0x9f, 0xae, 0xb0, 0xbb, 0xbd, 0xbd, 0xba, 0xd7, 0xcc, 0x63, 0xff }
};

#define WORLD_WIDTH     14
#define WORLD_HEIGHT    7
#define GRASS       0
#define WATER       1
#define TREES       2
#define STONE       3
int world[WORLD_HEIGHT][WORLD_WIDTH] = {
  { TREES, GRASS, GRASS, WATER, GRASS, GRASS, GRASS, TREES, GRASS, GRASS, GRASS, GRASS, GRASS, TREES },
  { GRASS, WATER, WATER, WATER, GRASS, WATER, GRASS, GRASS, GRASS, GRASS, GRASS, STONE, GRASS, GRASS },
  { GRASS, GRASS, GRASS, GRASS, GRASS, WATER, STONE, GRASS, GRASS, GRASS, TREES, GRASS, GRASS, GRASS },
  { STONE, GRASS, GRASS, STONE, TREES, WATER, WATER, WATER, GRASS, WATER, WATER, GRASS, TREES, GRASS },
  { GRASS, GRASS, GRASS, GRASS, TREES, GRASS, GRASS, GRASS, TREES, WATER, GRASS, GRASS, STONE, TREES },
  { GRASS, GRASS, GRASS, WATER, STONE, GRASS, GRASS, TREES, TREES, TREES, GRASS, GRASS, WATER, WATER },
  { GRASS, WATER, WATER, TREES, GRASS, WATER, WATER, TREES, TREES, GRASS, GRASS, GRASS, GRASS, STONE }
};

#define PLAYER_SIZE     16
#define PLAYER_X_OFFSET   WIDTH / 2 - PLAYER_SIZE / 2
#define PLAYER_Y_OFFSET   HEIGHT / 2 - PLAYER_SIZE / 2
void drawplayer() {
  arduboy.fillRect(PLAYER_X_OFFSET, PLAYER_Y_OFFSET, PLAYER_SIZE, PLAYER_SIZE, BLACK);
}

#define TILE_SIZE     16
void drawworld() {
  const int tileswide = WIDTH / TILE_SIZE + 1;
  const int tilestall = HEIGHT / TILE_SIZE + 1;

  for(int y = 0; y < tilestall; y++) {
    for(int x = 0; x < tileswide; x++) {
      const int tilex = x - mapx / TILE_SIZE;
      const int tiley = y - mapy / TILE_SIZE;
      if(tilex >= 0 && tiley >= 0 && tilex < WORLD_WIDTH && tiley < WORLD_HEIGHT) {
        arduboy.drawBitmap(x * TILE_SIZE + mapx % TILE_SIZE, y * TILE_SIZE + mapy % TILE_SIZE, tiles[world[tiley][tilex]], TILE_SIZE, TILE_SIZE, WHITE);
      }   
    }
  }

  arduboy.fillRect(0, 0, 48, 8, BLACK);
  arduboy.setCursor(0, 0);
  arduboy.print(0 - mapx / TILE_SIZE);
  arduboy.print(",");
  arduboy.print(0 - mapy / TILE_SIZE);
}

void titlescreen() {
  arduboy.setCursor(0, 0);
  arduboy.print("Title Screen\n");
  if(arduboy.justPressed(A_BUTTON)) {
    gamestate = GAME_PLAY;
  }
}

void playerinput() {
  if(arduboy.pressed(UP_BUTTON)) {
    if(mapy < PLAYER_Y_OFFSET) {
      mapy += 1;
    }
  }
  if(arduboy.pressed(DOWN_BUTTON)) {
    if(PLAYER_Y_OFFSET + PLAYER_SIZE < mapy + TILE_SIZE * WORLD_HEIGHT) {
      mapy -= 1;
    }
  }
  if(arduboy.pressed(LEFT_BUTTON)) {
    if(mapx < PLAYER_X_OFFSET) {
      mapx += 1;
    }
  }
  if(arduboy.pressed(RIGHT_BUTTON)) {
    if(PLAYER_X_OFFSET + PLAYER_SIZE < mapx + TILE_SIZE * WORLD_WIDTH) {
      mapx -= 1;
    }
  }
}

void gameplay() {
  playerinput();
  drawworld();
  drawplayer();

  if(arduboy.justPressed(A_BUTTON)) {
    gamestate = GAME_OVER;
  }
}

void gameoverscreen() {
  arduboy.setCursor(0, 0);
  arduboy.print("Game Over Screen\n");
  if(arduboy.justPressed(A_BUTTON)) {
    gamestate = GAME_HIGH;
  }
}

void highscorescreen() {
  arduboy.setCursor(0, 0);
  arduboy.print("High Score Screen\n");
  if(arduboy.justPressed(A_BUTTON)) {
    gamestate = GAME_TITLE;
  }
}

void gameloop() {
  switch(gamestate) {
    case GAME_TITLE:
      titlescreen();
      break;

    case GAME_PLAY:
      gameplay();
      break;

    case GAME_OVER:
      gameoverscreen();
      break;

    case GAME_HIGH:
      highscorescreen();
      break;
  }
}


void setup() {
  arduboy.begin();
  arduboy.setFrameRate(45);
  arduboy.display();

  arduboy.initRandomSeed();
  
  arduboy.clear();
}

void loop() {
  if(!(arduboy.nextFrame())) {
    return;
  }

  arduboy.pollButtons();

  arduboy.clear();

  gameloop();

  arduboy.display();
}
