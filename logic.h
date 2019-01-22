#ifndef LOGIC_H_
#define LOGIC_H_

game_t* game_init();
void answer_confirm(game_t *game);
void check_game_over_state(game_t *game, menu_t* menu, int* animate);
void hover_select(game_t* game, int x, int y);
void decrement_timer(game_t* game, int* init);
void use_lifeline_50(game_t* game);
void use_lifeline_25(game_t* game);
void use_lifeline_switch(game_t* game, int* animate);

void menu_hover_select(menu_t* menu, int x, int y);
void check_menu_selection(SDL_Renderer* renderer, menu_t* menu);

#endif  // LOGIC_H_
