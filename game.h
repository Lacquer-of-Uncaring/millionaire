#ifndef GAME_H_
#define GAME_H_

// Screen dimensions
#define SCREEN_WIDTH 640.0
#define SCREEN_HEIGHT 480.0

// Answer selction possiblities
#define NO_SELECTION 0
#define A_SELECTED 1
#define B_SELECTED 2
#define C_SELECTED 3
#define D_SELECTED 4
#define A_CONFIRMED 5
#define B_CONFIRMED 6
#define C_CONFIRMED 7
#define D_CONFIRMED 8

// Gamestate
#define RUNNING_STATE 0
#define GAME_OVER_STATE 1
#define QUIT_STATE 2

typedef struct {
	// user* player; 
    // question questions[15]; 
    int selection;
    int state;
    // int le_50;
    // int le_25;
    // int q_switch;
    int question_number;
    // int timer;
} game_t;

#endif  // GAME_H_
