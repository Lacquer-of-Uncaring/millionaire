#ifndef RENDERING_H_
#define RENDERING_H_

#define DEFAULT_FONT "resources/bahnschrift.ttf"

#define MENU_BG "resources/menu.png"
#define RUNNING_BG "resources/BG.png"
#define CRED_SCREEN "resources/login.png"

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

#define LIFELINE_50_ON "resources/50_1.png"
#define LIFELINE_50_OFF "resources/50_0.png"
#define LIFELINE_25_ON "resources/25_1.png"
#define LIFELINE_25_OFF "resources/25_0.png"
#define LIFELINE_SWITCH_ON "resources/switch_1.png"
#define LIFELINE_SWITCH_OFF "resources/switch_0.png"

void render_game(SDL_Renderer *renderer, game_t *game, int* animate, int* walk_away);
void render_menu(SDL_Renderer* renderer, menu_t *menu);
void render_stats(SDL_Renderer* renderer, stats* global_stats);
void render_text_input(SDL_Renderer* renderer, char* text, char* hidden, int level);
void render_answer(SDL_Renderer* renderer, int x, int y, const char* text, const char* font_name);

#endif  // RENDERING_H_
