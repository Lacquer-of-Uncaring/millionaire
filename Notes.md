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

## Menus

Init Menu : 
* Login
* Sign up
* Stats
* Instructions

User Menu :
* Start
* Instructions
* Stats
* Logout

Admin Menu :
* Start
* Instructions
* Stats
* Logout
* Admin options
    * Delete account
    * Grant admin rights
    * Add question

## Stats

* Number of registered users
* Number of correct answers
* BTS Score

## Files

* 3 question files one for each diff (ascii)
* file for user data (bin)
* file for stats (?)



## Instrutions
This is who wants to be a millionaire! Test your knowledge by attempting to answer all 15 answers
