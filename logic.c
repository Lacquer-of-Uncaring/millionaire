#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include "game.h"
#include "logic.h"
#include "menu-items.h"
#include "rendering.h"

int box_a_x = 65;
int box_a_y = 325;
int box_b_x = 350;
int box_b_y = 325;
int box_c_x = 65;
int box_c_y = 395;
int box_d_x = 350;
int box_d_y = 395;
int box_w = 215;
int box_h = 55;

// Shuffling an array of size n
void shuffle(int *array, size_t n){
    if (n > 1){
        size_t i;
        for (i = 0; i < n - 1; i++){
            srand( time(0)+clock()+rand()+getpid() );
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

// Fills an array with counting numbers
void count(int *array, int n){
    for (int i = 0; i < n; i++){
        array[i] = i+1;
    }
}

// Checks the currently confirmed answer
int check_answer(game_t *game){
    question current_q = game->questions[game->question_number-1];
    if (current_q.correct == game->selection)
        return 1;
    return 0;
}

// Returns the game score
int game_score(game_t* game, int* walk_away){
    if (*walk_away){    
        switch (game->question_number) {
        case 1:
            return 0;
            break;
        case 2:
            return 100;
            break;

        case 3:
            return 200;
            break;

        case 4:
            return 300;
            break;

        case 5:
            return 500;
            break;

        case 6:
            return 1000;
            break;
        
        case 7:
            return 2000;
            break;
        
        case 8:
            return 4000;
            break;
        
        case 9:
            return 8000;
            break;
        
        case 10:
            return 16000;
            break;
        
        case 11:
            return 32000;
            break;
        
        case 12:
            return 64000;
            break;
        
        case 13:
            return 125000;
            break;
        
        case 14:
            return 250000;
            break;
        
        case 15:
            return 500000;
            break;
           
        default: {}
        }
    } else {
        if (game->question_number < CHECKPOINT_1){
            return 0;
        } else if (game->question_number < CHECKPOINT_2){
            return 100;
        } else if (game->question_number < CHECKPOINT_3){
            return 1000;
        } else if (game->question_number < 15){
            return 32000;
        } else if (game->question_number == 15 && !check_answer(game)){
            return 32000;
        } else {
            return 1000000;
        }
    }
}

// Returns a pointer to stats fetched from the records file
stats* fetch_stats(){
    stats* global_stats = malloc(sizeof(stats));
    FILE* stat_file = fopen("records/stats", "r");
    char top_score[20];
    char top_score_holder[20];
    char number_of_users[20];
    char correct_answers[20];
    char lifeline_50_uses[20];
    char lifeline_25_uses[20];
    char lifeline_switch_uses[20];
    
    fscanf(stat_file,"%s",top_score);
    fscanf(stat_file,"%s",top_score_holder);
    fscanf(stat_file,"%s",number_of_users);
    fscanf(stat_file,"%s",correct_answers);
    fscanf(stat_file,"%s",lifeline_50_uses);
    fscanf(stat_file,"%s",lifeline_25_uses);
    fscanf(stat_file,"%s",lifeline_switch_uses);

    global_stats->top_score = atoi(top_score);
    strcpy(global_stats->top_score_holder, top_score_holder);
    global_stats->number_of_users = atoi(number_of_users);
    global_stats->correct_answers = atoi(correct_answers);
    global_stats->lifeline_50_uses = atoi(lifeline_50_uses);
    global_stats->lifeline_25_uses = atoi(lifeline_25_uses);
    global_stats->lifeline_switch_uses = atoi(lifeline_switch_uses);

    fclose(stat_file);
    return global_stats;
}

// Updates the stats file
void record_game_stats(game_t* game){
    stats* global_stats = fetch_stats();
    // Changes the top score if it has changed
    if (game->top_score_changed){
        global_stats->top_score = game->top_score;
        strcpy(global_stats->top_score_holder, game->player_id);
    }
    // Updates the number of correct answer
    if (game->question_number < 15 || (game->question_number == 15 && !check_answer(game)))
        global_stats->correct_answers += game->question_number-1;
    else
        global_stats->correct_answers += game->question_number; // += 15
    // Update lifeline uses
    if (!game->lifeline_50)
        global_stats->lifeline_50_uses++;
    if (!game->lifeline_25)
        global_stats->lifeline_25_uses++;
    if (!game->lifeline_switch)
        global_stats->lifeline_switch_uses++;
    // Write everything to the file
    FILE* stat_file = fopen("records/stats", "w");
    fprintf(stat_file,"%d %s %d %d %d %d %d",global_stats->top_score
                                            ,global_stats->top_score_holder
                                            ,global_stats->number_of_users
                                            ,global_stats->correct_answers
                                            ,global_stats->lifeline_50_uses
                                            ,global_stats->lifeline_25_uses
                                            ,global_stats->lifeline_switch_uses);
    fclose(stat_file);
}

// Used when accounts are created or deleted
void increment_user_number(){
    stats* global_stats = fetch_stats();
    global_stats->number_of_users++;
    FILE* stat_file = fopen("records/stats", "w");
    fprintf(stat_file,"%d %s %d %d %d %d %d",global_stats->top_score
                                            ,global_stats->top_score_holder
                                            ,global_stats->number_of_users
                                            ,global_stats->correct_answers
                                            ,global_stats->lifeline_50_uses
                                            ,global_stats->lifeline_25_uses
                                            ,global_stats->lifeline_switch_uses);
    fclose(stat_file);
}

void decrement_user_number(){
    stats* global_stats = fetch_stats();
    global_stats->number_of_users--;
    FILE* stat_file = fopen("records/stats", "w");
    fprintf(stat_file,"%d %s %d %d %d %d %d",global_stats->top_score
                                            ,global_stats->top_score_holder
                                            ,global_stats->number_of_users
                                            ,global_stats->correct_answers
                                            ,global_stats->lifeline_50_uses
                                            ,global_stats->lifeline_25_uses
                                            ,global_stats->lifeline_switch_uses);
    fclose(stat_file);
}

// Counts the number of questions in a file
int count_questions(FILE* fp){
  int lines=0;
  int ch = 0;
  while(!feof(fp)){
    ch = fgetc(fp);
    if(ch == '\n'){
      lines++;
    }
  }
  return lines/5;
}

// Retruns an array of 6 random different numbers between 1 and the number of questions in a file
int* random_different_numbers(const char* path){
    FILE* fp = fopen(path,"r");
    int n = count_questions(fp);
    fclose(fp);
    int array[n];
    count(array,n);
    shuffle(array,n);
    // We only need 6 questions for each diff so we only return the first 6 random numbers
    int* ret = calloc(6,sizeof(int));
    for (int i=0; i<6; i++)
        ret[i] = array[i];
    return ret;
}

// Returns the nth question from a specified file
question* nth_question(int n, const char* path){
    FILE* fp = fopen(path,"r");
    question* ret = malloc(sizeof(question));
    int count = 1;
    ret->text = malloc(100);
    ret->ans_a = malloc(30);
    ret->ans_b = malloc(30);
    ret->ans_c = malloc(30);
    ret->ans_d = malloc(30);
    ret->correct = A_CORRECT;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int target = (n-1)*5 + 1;

    while ((read = getline(&line, &len, fp)) != -1) {
        if (count == target){  
          sscanf(line,"%[^\n]s",ret->text);
          getline(&line, &len, fp);
          sscanf(line,"%[^\n]s",ret->ans_a);
          getline(&line, &len, fp);
          sscanf(line,"%[^\n]s",ret->ans_b);
          getline(&line, &len, fp);
          sscanf(line,"%[^\n]s",ret->ans_c);
          getline(&line, &len, fp);
          sscanf(line,"%[^\n]s",ret->ans_d);
          return ret;
        }
        else{
          count++;
        }
    }
    fclose(fp);
    if (line)
        free(line);
}

// Shuffles the order of the answers (A is supposed the correct answer in org)
question* scramble_answers(question* org){
  int trans[4] = {0,1,2,3};
  srand( time(0)+clock()+rand() );
  shuffle(trans,4);
  question* ret = malloc(sizeof(question));
  ret->text = malloc(100);
  ret->ans_a = malloc(30);
  ret->ans_b = malloc(30);
  ret->ans_c = malloc(30);
  ret->ans_d = malloc(30);
  ret->text = org->text
;  for (int i=0; i<4; i++){
    switch (i){
      case 0:
        switch (trans[i]){
          case 0:
            ret->ans_a = org->ans_a;
            ret->correct = A_CORRECT;
            break;
          case 1:
            ret->ans_a = org->ans_b;
            break;
          case 2:
            ret->ans_a = org->ans_c;
            break;
          case 3:
            ret->ans_a = org->ans_d;
            break;
          default : {}
        }
        break;
      case 1:
        switch (trans[i]){
          case 0:
            ret->ans_b = org->ans_a;
            ret->correct = B_CORRECT;
            break;
          case 1:
            ret->ans_b = org->ans_b;
            break;
          case 2:
            ret->ans_b = org->ans_c;
            break;
          case 3:
            ret->ans_b = org->ans_d;
            break;
          default : {}
        }
        break;
      case 2:
        switch (trans[i]){
          case 0:
            ret->ans_c = org->ans_a;
            ret->correct = C_CORRECT;
            break;
          case 1:
            ret->ans_c = org->ans_b;
            break;
          case 2:
            ret->ans_c = org->ans_c;
            break;
          case 3:
            ret->ans_c = org->ans_d;
            break;
          default : {}
        }
        break;
      case 3:
        switch (trans[i]){
          case 0:
            ret->ans_d = org->ans_a;
            ret->correct = D_CORRECT;
            break;
          case 1:
            ret->ans_d = org->ans_b;
            break;
          case 2:
            ret->ans_d = org->ans_c;
            break;
          case 3:
            ret->ans_d = org->ans_d;
            break;
          default : {}
        }
        break;
      default : {}
    }
  }
}

// Initialises a new game
game_t* game_init(menu_t* menu){

    // Seeding rand for the function calls
    srand( time(0)+clock()+rand() );
    question* org;
    question* ret;
    game_t* game = malloc(sizeof(game_t));
    // Getting 5 random questions of diff 1
    int* r1 = random_different_numbers("records/lvl1");
    for (int i=0; i<5; i++){
        org = nth_question(r1[i],"records/lvl1");
        ret = scramble_answers(org);
        game->questions[i] = *ret;
        free(org);
        free(ret);
    }
    // Extra diff 1 question for switch
    org = nth_question(r1[5],"records/lvl1");
    ret = scramble_answers(org);
    game->switch_questions[0] = *ret;
    free(org);
    free(ret);

    int* r2 = random_different_numbers("records/lvl2");
    for (int i=0; i<5; i++){
        org = nth_question(r2[i],"records/lvl2");
        ret = scramble_answers(org);
        game->questions[i+5] = *ret;
        free(org);
        free(ret);
    }
    org = nth_question(r2[5],"records/lvl2");
    ret = scramble_answers(org);
    game->switch_questions[1] = *ret;
    free(org);
    free(ret);

    int* r3 = random_different_numbers("records/lvl3");
    for (int i=0; i<5; i++){
        org = nth_question(r3[i],"records/lvl3");
        ret = scramble_answers(org);
        game->questions[i+10] = *ret;
        free(org);
        free(ret);
    }
    org = nth_question(r3[5],"records/lvl3");
    ret = scramble_answers(org);
    game->switch_questions[2] = *ret;
    free(org);
    free(ret);
    free(r1);
    free(r2);
    free(r3);
    // Resetting the game_t fields
    stats* global_stats = fetch_stats();
    strcpy(game->player_id, menu->user_id);
    game->selection = NO_SELECTION;
    game->A_available = 1;
    game->B_available = 1;
    game->C_available = 1;
    game->D_available = 1;
    game->lifeline_50 = 1;
    game->lifeline_25 = 1;
    game->lifeline_switch = 1;
    game->state = RUNNING_STATE;
    game->question_number = 1;
    game->timer = FIRST_COUNTDOWN;
    game->score = 0;
    game->top_score = global_stats->top_score;
    game->top_score_changed = 0;

    return game;   
} 

// Confirming selected answer
void answer_confirm(game_t *game){
    if (game->state == RUNNING_STATE){
        if (game->selection < 5 && game->selection > 0)
            game->selection += 4;
    }
}

// Resets the game state for the next question
void next_question(game_t* game, int* animate){
    game->question_number += 1;
    game->selection = NO_SELECTION;
    game->state = RUNNING_STATE;
    game->A_available = 1;
    game->B_available = 1;
    game->C_available = 1;
    game->D_available = 1;
    if (game->question_number < CHECKPOINT_2)
        game->timer = FIRST_COUNTDOWN + 1;
    else if (game->question_number < CHECKPOINT_3)
        game->timer = SECOND_COUNTDOWN + 1;
    else
        game->timer = THIRD_COUNTDOWN + 1;
    *animate = 1;
}

// Checks if the game should end
void check_game_over_state(game_t *game, menu_t* menu, int* animate, int* walk_away){
    // Go back to menu when a key or a mouse button is pressed
    if (game->state == GAME_OVER_STATE){
        game->score = game_score(game,walk_away);
        // Update top score
        if (game->score > game->top_score){
            game->top_score = game->score;
            game->top_score_changed = 1;
        }
        // Wait for user input to continue
        SDL_Event e;
        do{
            SDL_WaitEvent(&e);
        } while(e.type != SDL_KEYDOWN && e.type != SDL_MOUSEBUTTONDOWN);
        game->state = QUIT_STATE;
        menu->state = RUNNING;
        menu->selection = NO_SELECTION;
        record_game_stats(game);
        srand( time(0)+clock()+rand() );
        //game = game_init(menu);
    }   
    // Check answer
    if (game->state == CHECKING_STATE){
        SDL_Delay(1000);
        if ((game->question_number == 15 && check_answer(game)) || !check_answer(game) || game->timer <= 0 )
            game->state = GAME_OVER_STATE;
        else
            next_question(game,animate);
    }
    // Go into checking state if the player confirmed his answer or ran out of time
    if (game->state == RUNNING_STATE){
        if (game->selection > 4){
            SDL_Delay(1000);
            game->state = CHECKING_STATE;
        }
        if (game->timer <= 0){
            SDL_Delay(1000);
            game->state = CHECKING_STATE;
        }
    }     
}   

// Mouse hover detection
void hover_select(game_t* game, int x, int y){
    if (game->state == RUNNING_STATE){
        if (game->selection < 5){
            game->selection = NO_SELECTION;        
            if (x > box_a_x && x < box_a_x + box_w && y > box_a_y && y < box_a_y + box_h && game->A_available)
                game->selection = A_SELECTED;
            if (x > box_b_x && x < box_b_x + box_w && y > box_b_y && y < box_b_y + box_h && game->B_available)
                game->selection = B_SELECTED;
            if (x > box_c_x && x < box_c_x + box_w && y > box_c_y && y < box_c_y + box_h && game->C_available)
                game->selection = C_SELECTED;
            if (x > box_d_x && x < box_d_x + box_w && y > box_d_y && y < box_d_y + box_h && game->D_available)
                game->selection = D_SELECTED;
        }
    }

}

// Updates the countdown on the current question
void decrement_timer(game_t* game, int* init){
    int current = SDL_GetTicks();
    // Decrement the countdown timer every second
    if (current - *init > 1000){
        if (game->state == RUNNING_STATE){
            game->timer--;
            *init = SDL_GetTicks();
        }        
    }
}

// Returns a random wrong answer to delete
int random_wrong_answer(game_t* game){
    question current_q = game->questions[game->question_number-1];
    int r;
    int available;
    do{
        r = rand() % 4;
        switch(r){
            case 0:
            available = game->A_available;
            break;
            case 1:
            available = game->B_available;
            break;
            case 2:
            available = game->C_available;
            break;
            case 3:
            available = game->D_available;
            break;
        }
    } while (r == current_q.correct - 5 || !available);
    
    return r;
}

// Makes anwer unselectable
int delete_wrong_answer(game_t* game){
    int r = random_wrong_answer(game);
    switch (r){
        case 0:
            game->A_available = 0;
            break;
        case 1:
            game->B_available = 0;
            break;
        case 2:
            game->C_available = 0;
            break;
        case 3:
            game->D_available = 0;
            break;    
        
        default : {}
    }
    return r;
}

// Deletes two wrong answers
void use_lifeline_50(game_t* game){
    if (game->state == RUNNING_STATE){    
        if (game->selection < 5){
            if (game->lifeline_50){
                int r1 = delete_wrong_answer(game);
                int r2;
                do{
                    r2 = delete_wrong_answer(game);
                } while (r1 == r2);
                game->lifeline_50 = 0;
                SDL_Delay(1000);
            }
        }   
    }
}

// Deletes one wrong answer
void use_lifeline_25(game_t* game){
    if (game->state == RUNNING_STATE){    
        if (game->selection < 5){
            if (game->lifeline_25){
                delete_wrong_answer(game);
                game->lifeline_25 = 0;
                SDL_Delay(1000);
            }
        }
    }
}

// Changes the current question to the switch question
void use_lifeline_switch(game_t* game, int* animate){
    if (game->lifeline_switch && game->state == RUNNING_STATE && game->selection < 5){
        question *current_q = &game->questions[game->question_number-1];
        question *switch_q;
        if (game->question_number < 6)
            switch_q = &game->switch_questions[0];
        else if (game->question_number < 11)
            switch_q = &game->switch_questions[1];
        else
            switch_q = &game->switch_questions[2];
        current_q->text = malloc(strlen(switch_q->text) + 1);
        strcpy(current_q->text,switch_q->text);
        current_q->ans_a = malloc(strlen(switch_q->ans_a) + 1);
        strcpy(current_q->ans_a,switch_q->ans_a);
        current_q->ans_b = malloc(strlen(switch_q->ans_b) + 1);
        strcpy(current_q->ans_b,switch_q->ans_b);
        current_q->ans_c = malloc(strlen(switch_q->ans_c) + 1);
        strcpy(current_q->ans_c,switch_q->ans_c);
        current_q->ans_d = malloc(strlen(switch_q->ans_d) + 1);
        strcpy(current_q->ans_d,switch_q->ans_d);
        current_q->correct = switch_q->correct;
        // reset the game state for a new question
        game->lifeline_switch = 0;
        game->selection = NO_SELECTION;
        game->A_available = 1;
        game->B_available = 1;
        game->C_available = 1;
        game->D_available = 1;
        if (game->question_number < CHECKPOINT_2)
            game->timer = FIRST_COUNTDOWN;
        else if (game->question_number < CHECKPOINT_3)
            game->timer = SECOND_COUNTDOWN;
        else
            game->timer = THIRD_COUNTDOWN;
        *animate = 1;
    }
}


// Menu logic

int m_box_a_x = 65;
int m_box_a_y = 295;
int m_box_b_x = 350;
int m_box_b_y = 295;
int m_box_c_x = 65;
int m_box_c_y = 365;
int m_box_d_x = 350;
int m_box_d_y = 365;
int m_box_w = 215;
int m_box_h = 55;

// Mouse hover detection
void menu_hover_select(menu_t* menu, int x, int y){
    menu->selection = NO_SELECTION;        
    if (x > m_box_a_x && x < m_box_a_x + m_box_w && y > m_box_a_y && y < m_box_a_y + m_box_h)
        menu->selection = A_SELECTED;
    if (x > m_box_b_x && x < m_box_b_x + m_box_w && y > m_box_b_y && y < m_box_b_y + m_box_h)
        menu->selection = B_SELECTED;
    if (x > m_box_c_x && x < m_box_c_x + m_box_w && y > m_box_c_y && y < m_box_c_y + m_box_h)
        menu->selection = C_SELECTED;
    if (x > m_box_d_x && x < m_box_d_x + m_box_w && y > m_box_d_y && y < m_box_d_y + m_box_h)
        menu->selection = D_SELECTED;
}

// Main Menu
void check_menu_selection(SDL_Renderer* renderer,game_t* game, menu_t* menu){
    // Resetting the game without the questions before the start button is pressed so it's responsive
    stats* global_stats = fetch_stats();
    game->selection = NO_SELECTION;
    game->A_available = 1;
    game->B_available = 1;
    game->C_available = 1;
    game->D_available = 1;
    game->lifeline_50 = 1;
    game->lifeline_25 = 1;
    game->lifeline_switch = 1;
    game->question_number = 1;
    game->timer = FIRST_COUNTDOWN;
    game->score = 0;
    game->top_score = global_stats->top_score;
    game->top_score_changed = 0;
    // Variables for text input
    char* id = calloc(1,21); 
    char* passwd = calloc(1,21);
    char* confirm = calloc(1,21);
    char* text = calloc(1,151);
    char* ans_a = calloc(1,31);
    char* ans_b = calloc(1,31);
    char* ans_c = calloc(1,31);
    char* ans_d = calloc(1,31);
    question* new_q = malloc(sizeof(question));
    new_q->text = text;
    new_q->ans_a = ans_a;
    new_q->ans_b = ans_b;
    new_q->ans_c = ans_c;
    new_q->ans_d = ans_d;
    // Menu Buttons
    switch (menu->type){  
        case INIT_MENU:
        switch (menu->selection){
            case A_CONFIRMED: // Login
                username_input(renderer,menu,id,passwd);
                break;
            case B_CONFIRMED: // Signup
                signup_input(renderer,menu,id,passwd);
                break;
            case C_CONFIRMED: // Show instructions
                SDL_RenderClear(renderer);
                render_instructions(renderer);
                SDL_RenderPresent(renderer);
                SDL_Event o;
                do{
                    SDL_WaitEvent(&o);
                } while(o.type != SDL_KEYDOWN && o.type != SDL_MOUSEBUTTONDOWN);
                menu->state = RUNNING;
                menu->selection = NO_SELECTION;
                break;
            case D_CONFIRMED: // Show stats
                SDL_RenderClear(renderer);
                stats* global_stats = fetch_stats();
                render_stats(renderer,global_stats);
                SDL_RenderPresent(renderer);
                SDL_Event e;
                do{
                    SDL_WaitEvent(&e);
                } while(e.type != SDL_KEYDOWN && e.type != SDL_MOUSEBUTTONDOWN);
                menu->state = RUNNING;
                menu->selection = NO_SELECTION;
                break;
            }
            break;

        case USER_MENU:
        switch (menu->selection){
            case A_CONFIRMED: // Start game
                game = game_init(menu);
                game_loop(renderer, game, menu);
                //game = game_init(menu);
                //record_stats(game);
                break;
            case B_CONFIRMED: // Logout
                menu->user_id[0] = 0;
                menu->type = INIT_MENU;
                menu->selection = NO_SELECTION;
                break;
            case C_CONFIRMED: // Show instructions
                SDL_RenderClear(renderer);
                render_instructions(renderer);
                SDL_RenderPresent(renderer);
                SDL_Event o;
                do{
                    SDL_WaitEvent(&o);
                } while(o.type != SDL_KEYDOWN && o.type != SDL_MOUSEBUTTONDOWN);
                menu->state = RUNNING;
                menu->selection = NO_SELECTION;
                break;
            case D_CONFIRMED: // Show stats
                SDL_RenderClear(renderer);
                stats* global_stats = fetch_stats();
                render_stats(renderer,global_stats);
                SDL_RenderPresent(renderer);
                SDL_Event e;
                do{
                    SDL_WaitEvent(&e);
                } while(e.type != SDL_KEYDOWN && e.type != SDL_MOUSEBUTTONDOWN);
                menu->state = RUNNING;
                menu->selection = NO_SELECTION;
                break;
                break;
            }

            break;

        case ADMIN_MENU:
        switch (menu->selection){
            case A_CONFIRMED: // Start game
                game = game_init(menu);     
                game_loop(renderer, game, menu);
                //game = game_init(menu);
                break;
            case B_CONFIRMED: // Logout
                menu->user_id[0] = 0;
                menu->type = INIT_MENU;
                menu->selection = NO_SELECTION;
                break;
            case C_CONFIRMED: // Show instructions
                SDL_RenderClear(renderer);
                render_instructions(renderer);
                SDL_RenderPresent(renderer);
                SDL_Event o;
                do{
                    SDL_WaitEvent(&o);
                } while(o.type != SDL_KEYDOWN && o.type != SDL_MOUSEBUTTONDOWN);
                menu->state = RUNNING;
                menu->selection = NO_SELECTION;
                break;
            case D_CONFIRMED: // Show stats
                SDL_RenderClear(renderer);
                stats* global_stats = fetch_stats();
                render_stats(renderer,global_stats);
                SDL_RenderPresent(renderer);
                SDL_Event e;
                do{
                    SDL_WaitEvent(&e);
                } while(e.type != SDL_KEYDOWN && e.type != SDL_MOUSEBUTTONDOWN);
                menu->state = RUNNING;
                menu->selection = NO_SELECTION;
                break;
                break;
            }
            break;

        case ADMIN_OPS:
        switch (menu->selection){
            case A_CONFIRMED: // Delete an account
                admin_input(renderer,menu,id,confirm);
                break;
            case B_CONFIRMED: // Grant admin rights
                rights_input(renderer,menu,id,confirm);
                break;
            case C_CONFIRMED: // Add a question
                add_question_input(renderer,menu,new_q);
                break;
            case D_CONFIRMED: // Go Back
                menu->type = ADMIN_MENU;
                menu->selection = NO_SELECTION;
                break;
            }
            break;

        default : {}
    }
}