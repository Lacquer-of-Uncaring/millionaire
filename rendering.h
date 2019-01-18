#ifndef RENDERING_H_
#define RENDERING_H_

#define INTRO_SCREEN "resources/intro.png"
#define RUNNING_BG "resources/BG.png"

#define H_SCREEN "resources/100.png"
#define TWOH_SCREEN "resources/200.png"
#define THREEH_SCREEN "resources/300.png"
#define FIVEH_SCREEN "resources/500.png"
#define ONEK_SCREEN "resources/1k.png"
#define TWOK_SCREEN "resources/2k.png"
#define FOURK_SCREEN "resources/4k.png"
#define EIGHTK_SCREEN "resources/8k.png"
#define SIXTEENK_SCREEN "resources/16k.png"
#define THIRTYTWOK_SCREEN "resources/32k.png"
#define SIXTYFOURK_SCREEN "resources/64k.png"
#define ONETWENTYFIVEK_SCREEN "resources/125k.png"
#define TWOFIFTYK_SCREEN "resources/250k.png"
#define FIVEHK_SCREEN "resources/500k.png"
#define WIN_SCREEN "resources/WIN.png"


void render_game(SDL_Renderer *renderer, game_t *game);

#endif  // RENDERING_H_
