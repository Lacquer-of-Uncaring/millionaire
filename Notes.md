# Plan

## Renderer (functions that render a given gamestate)

[x] Gameover state
[ ] Running state
[ ] Anwser checking state

## Logic (functions that change the gamestate)

* bool check_answer(game_t* game)  // check if answer correct (used to quit gameloop in case of wrong answer) 
* void answer_selected(game_t* game, coordinates)  // change the game->selection field
* void answer_confirmed(game_t* game)  // changes the game->selection field
* void check_gameover_state(game_t* game) // changes the game->state field
