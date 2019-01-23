#ifndef GAME_H_
#define GAME_H_

// Screen dimensions
#define SCREEN_WIDTH 640.0
#define SCREEN_HEIGHT 480.0

// correct field in question
#define A_CORRECT 5
#define B_CORRECT 6 
#define C_CORRECT 7
#define D_CORRECT 8

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
#define CHECKING_STATE 2
#define QUIT_STATE 3

#define FIRST_COUNTDOWN 15
#define SECOND_COUNTDOWN 30
#define THIRD_COUNTDOWN 45

#define CHECKPOINT_1 2
#define CHECKPOINT_2 6
#define CHECKPOINT_3 11

// Menu states
#define RUNNING 0
#define QUIT 1

// Menus
#define INIT_MENU 0
#define USER_MENU 1
#define ADMIN_MENU 2
#define ADMIN_OPS 3

typedef struct {
    char id[20];
    char password[20];
    int admin;
} user_t;

typedef struct _n{
    user_t* user;
    struct _n* next;
} node;

typedef struct {
    char* m_ans_a;
    char* m_ans_b;
    char* m_ans_c;
    char* m_ans_d;
} m_question;

typedef struct {
    m_question question[4];
    int state;
    int selection;
    int type;
} menu_t;

typedef struct {
	char* text;
	char* ans_a;
	char* ans_b;
	char* ans_c;
	char* ans_d;
	// int diff;
	int correct;
} question;

typedef struct {
	// user* player; 
    question questions[15]; 
    question switch_questions[15];
    int selection;
    int A_available;
    int B_available;
    int C_available;
    int D_available;
    int state;
    int lifeline_50;
    int lifeline_25;
    int lifeline_switch;
    int question_number;
    int timer;
} game_t;

#endif  // GAME_H_
