#ifndef MENU_ITEMS_H_
#define MENU_ITEMS_H_

#define LOGIN 0
#define STATS 1
#define INSTRUCTIONS 2
#define NEW_ACCOUNT 3
#define LOGOUT 4

typedef struct {
	int type;
	char* text;
}button;

typedef struct {
	int option_number;
	button* options;
}menu_t;


void game_loop(SDL_Renderer* renderer, game_t* game);

#endif // MENU_ITEMS_H_