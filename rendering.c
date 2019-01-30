#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "game.h"
#include "rendering.h"
#include "logic.h"

const SDL_Rect rect_a = { .x = 0, .y = 300, .w = 320, .h = 90};
const SDL_Rect rect_b = { .x = 320, .y = 300, .w = 320, .h = 90};
const SDL_Rect rect_c = { .x = 0, .y = 390, .w = 320, .h = 90};
const SDL_Rect rect_d = { .x = 320, .y = 390, .w = 320, .h = 90};

int lifeline_x = 267;
int lifeline_y = 5;
int lifeline_w = 35;
int question_x = 100;
int question_y = 80;
int ans_a_x = 65;
int ans_a_y = 337;
int ans_b_x = 347;
int ans_b_y = 337;
int ans_c_x = 65;
int ans_c_y = 406;
int ans_d_x = 347;
int ans_d_y = 406;
int timer_x = 303;
int timer_y = 290;

const SDL_Color green = { .r = 34, .g = 170, .b = 34};
const SDL_Color red = { .r = 230, .g = 0, .b = 0};
const SDL_Color yellow = { .r = 250, .g = 154, .b = 20};
const SDL_Color grey = { .r = 100, .g = 100, .b = 100};
const SDL_Color white = { .r = 255, .g = 255, .b = 255};
const SDL_Color orange = { .r = 255, .g = 72, .b = 0};


SDL_Texture* load_texture(SDL_Renderer* renderer, const char* path){
    //The final texture
    SDL_Texture* new_texture = NULL;

    //Load image at specified path
    SDL_Surface* loaded_surface = IMG_Load(path);
    if(loaded_surface == NULL){
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
    } else {
        //Create texture from surface pixels
        new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
        if(new_texture == NULL){
            printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loaded_surface);
    }

    return new_texture;
}

void render_screen(SDL_Renderer* renderer, const char* path){
    // simply show the chosen end screen
	SDL_Texture* texture = load_texture(renderer, path);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_DestroyTexture(texture);

}

void render_game_over_state(SDL_Renderer* renderer, game_t* game, int* walk_away){
    if (*walk_away){    
        switch (game->question_number) {
        case 2:
            render_screen(renderer, H_SCREEN);
            break;

        case 3:
            render_screen(renderer, TWOH_SCREEN);
            break;

        case 4:
            render_screen(renderer, THREEH_SCREEN);
            break;

        case 5:
            render_screen(renderer, FIVEH_SCREEN);
            break;

        case 6:
            render_screen(renderer, ONEK_SCREEN);
            break;
        
        case 7:
            render_screen(renderer, TWOK_SCREEN);
            break;
        
        case 8:
            render_screen(renderer, FOURK_SCREEN);
            break;
        
        case 9:
            render_screen(renderer, EIGHTK_SCREEN);
            break;
        
        case 10:
            render_screen(renderer, SIXTEENK_SCREEN);
            break;
        
        case 11:
            render_screen(renderer, THIRTYTWOK_SCREEN);
            break;
        
        case 12:
            render_screen(renderer, SIXTYFOURK_SCREEN);
            break;
        
        case 13:
            render_screen(renderer, ONETWENTYFIVEK_SCREEN);
            break;
        
        case 14:
            render_screen(renderer, TWOFIFTYK_SCREEN);
            break;
        
        case 15:
            render_screen(renderer, FIVEHK_SCREEN);
            break;
           
        default: {}
        }
    } else {
        if (game->question_number < CHECKPOINT_1){
            //
        } else if (game->question_number < CHECKPOINT_2){
            render_screen(renderer, H_SCREEN);
        } else if (game->question_number < CHECKPOINT_3){
            render_screen(renderer, ONEK_SCREEN);
        } else {
            render_screen(renderer, THIRTYTWOK_SCREEN);
        }
        if (game->question_number == 15 && check_answer(game))
            render_screen(renderer, WIN_SCREEN);    
    }
}

void render_rect(SDL_Renderer* renderer, const SDL_Rect* rect, const SDL_Color* color){
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);
    SDL_RenderFillRect(renderer, rect);
}

void render_question(SDL_Renderer* renderer, int x, int y, const char* text, const char* font_name){
    TTF_Init();
    TTF_Font * font = TTF_OpenFont(font_name, 25);
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    Uint32 wrap = 445;

    SDL_Surface * surface = TTF_RenderText_Blended_Wrapped(font, text, white, wrap);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);
    TTF_Quit();
}

void render_answer(SDL_Renderer* renderer, int x, int y, const char* text, const char* font_name, const SDL_Color color){
    TTF_Init();
    TTF_Font * font;
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    if(strlen(text) < 16){
        font = TTF_OpenFont(font_name, 25);
    }
    else if (strlen(text) < 22){
        font = TTF_OpenFont(font_name, 20);
        dest.y += 4;
    }
    else if (strlen(text) < 26){
        font = TTF_OpenFont(font_name, 17);
        dest.y += 4;
    }
    else{
        font = TTF_OpenFont(font_name, 15);
        dest.y += 4;
    }

    SDL_Surface * surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);
    TTF_Quit();
}

void render_money(SDL_Renderer* renderer, int x, int y, const char* text, const char* font_name){
    TTF_Init();
    TTF_Font * font = TTF_OpenFont(font_name, 17);

    
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    SDL_Surface * surface = TTF_RenderText_Blended(font, text, white);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);
    TTF_Quit();
}

void render_25(SDL_Renderer* renderer, int x, int y, const char* text, const char* font_name){
    TTF_Init();
    TTF_Font * font = TTF_OpenFont(font_name, 25);

    
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    SDL_Surface * surface = TTF_RenderText_Blended(font, text, white);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);
    TTF_Quit();
}

void render_20(SDL_Renderer* renderer, int x, int y, const char* text, const char* font_name){
    TTF_Init();
    TTF_Font * font = TTF_OpenFont(font_name, 25);

    
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;

    SDL_Surface * surface = TTF_RenderText_Blended(font, text, white);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);
    TTF_Quit();
}


void render_25_centered(SDL_Renderer* renderer, int x, int y, const char* text, const char* font_name){
    TTF_Init();
    TTF_Font * font = TTF_OpenFont(font_name, 25);

    SDL_Surface * surface = TTF_RenderText_Blended(font, text, white);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);    
    SDL_Rect dest;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    dest.x = x - dest.w/2;
    dest.y = y;

    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);
    TTF_Quit();
}

void render_text(SDL_Renderer* renderer, game_t* game, int* animate){
    question current_q = game->questions[game->question_number-1];
    char q_number[3]; 
    sprintf(q_number, "%d", game->question_number);
    render_answer(renderer, 440, 448, "Question number : ", DEFAULT_FONT, white);
    render_answer(renderer, 610, 450, q_number, DEFAULT_FONT, white);
    switch (game->question_number) {
        case 1:
            render_money(renderer, 5, 455, "0 DHS / 0 DHS", DEFAULT_FONT);
            break;

        case 2:
            render_money(renderer, 5, 455, "100 DHS / 100 DHS", DEFAULT_FONT);
            break;

        case 3:
            render_money(renderer, 5, 455, "200 DHS / 100 DHS", DEFAULT_FONT);
            break;

        case 4:
             render_money(renderer, 5, 455, "300 DHS / 100 DHS", DEFAULT_FONT);
            break;

        case 5:
             render_money(renderer, 5, 455, "500 DHS / 100 DHS", DEFAULT_FONT);
            break;

        case 6:
             render_money(renderer, 5, 455, "1k DHS / 1k DHS", DEFAULT_FONT);
            break;
        
        case 7:
             render_money(renderer, 5, 455, "2k DHS / 1k DHS", DEFAULT_FONT);
            break;
        
        case 8:
             render_money(renderer, 5, 455, "4k DHS / 1k DHS", DEFAULT_FONT);
            break;
        
        case 9:
             render_money(renderer, 5, 455, "8k DHS / 1k DHS", DEFAULT_FONT);
            break;
        
        case 10:
             render_money(renderer, 5, 455, "16k DHS / 1k DHS", DEFAULT_FONT);
            break;
        
        case 11:
             render_money(renderer, 5, 455, "32k DHS / 32k DHS", DEFAULT_FONT);
            break;
        
        case 12:
             render_money(renderer, 5, 455, "64k DHS / 32k DHS", DEFAULT_FONT);
            break;
        
        case 13:
             render_money(renderer, 5, 455, "125k DHS / 32k DHS", DEFAULT_FONT);
            break;
        
        case 14:
             render_money(renderer, 5, 455, "250k DHS / 32k DHS", DEFAULT_FONT);
            break;
        
        case 15:
             render_money(renderer, 5, 455, "500k DHS / 32k DHS", DEFAULT_FONT);
            break;
           
        default: {}
        }

    if (*animate){
        SDL_Delay(300);
        render_question(renderer, question_x, question_y, current_q.text, DEFAULT_FONT);
        SDL_RenderPresent(renderer);
        SDL_Delay(2000);    
        render_answer(renderer, ans_a_x+25, ans_a_y, current_q.ans_a, DEFAULT_FONT, white);
        render_answer(renderer, ans_a_x, ans_a_y, "A:", DEFAULT_FONT, orange);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
        render_answer(renderer, ans_b_x+25, ans_b_y, current_q.ans_b, DEFAULT_FONT, white);
        render_answer(renderer, ans_b_x, ans_b_y, "B:", DEFAULT_FONT, orange);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);    
        render_answer(renderer, ans_c_x+25, ans_c_y, current_q.ans_c, DEFAULT_FONT, white);
        render_answer(renderer, ans_c_x, ans_c_y, "C:", DEFAULT_FONT, orange);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
        render_answer(renderer, ans_d_x+25, ans_d_y, current_q.ans_d, DEFAULT_FONT, white);
        render_answer(renderer, ans_d_x, ans_d_y, "D:", DEFAULT_FONT, orange);
        SDL_RenderPresent(renderer);
        *animate = 0;
    }

    else{
        render_question(renderer, question_x, question_y, current_q.text, DEFAULT_FONT);
        if (game->A_available){
            render_answer(renderer, ans_a_x+25, ans_a_y, current_q.ans_a, DEFAULT_FONT, white);
            render_answer(renderer, ans_a_x, ans_a_y, "A:", DEFAULT_FONT, orange);
        }
        if (game->B_available){
            render_answer(renderer, ans_b_x+25, ans_b_y, current_q.ans_b, DEFAULT_FONT, white);
            render_answer(renderer, ans_b_x, ans_b_y, "B:", DEFAULT_FONT, orange);
        }
        if (game->C_available){   
            render_answer(renderer, ans_c_x+25, ans_c_y, current_q.ans_c, DEFAULT_FONT, white);
            render_answer(renderer, ans_c_x, ans_c_y, "C:", DEFAULT_FONT, orange);
        }
        if (game->D_available){
            render_answer(renderer, ans_d_x+25, ans_d_y, current_q.ans_d, DEFAULT_FONT, white);
            render_answer(renderer, ans_d_x, ans_d_y, "D:", DEFAULT_FONT, orange);
        }
    
    }
}

void render_timer(SDL_Renderer* renderer, game_t* game){
    TTF_Init();
    TTF_Font * font = TTF_OpenFont(DEFAULT_FONT, 25);
    
    SDL_Rect dest;
    dest.x = timer_x;
    dest.y = timer_y;
    char countdown[3];
    SDL_Color col = white;
    // Small adhoc tweaks
    if (game->timer < 10)
        dest.x += 5;
    if (game->timer < 6)
        col = red; 
    sprintf(countdown, "%d", game->timer);

    SDL_Surface * surface = TTF_RenderText_Blended(font, countdown, col);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);
    TTF_Quit();
}

void render_lifeline_50(SDL_Renderer* renderer, game_t* game){
    SDL_Texture* texture;
    if (game->lifeline_50)
        texture = load_texture(renderer, LIFELINE_50_ON);
    else
        texture = load_texture(renderer, LIFELINE_50_OFF);
    SDL_Rect dest = {.x = lifeline_x, .y = lifeline_y, .h = 25, .w = 25};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
}

void render_lifeline_25(SDL_Renderer* renderer, game_t* game){
    SDL_Texture* texture;
    if (game->lifeline_25)
        texture = load_texture(renderer, LIFELINE_25_ON);
    else
        texture = load_texture(renderer, LIFELINE_25_OFF);
    SDL_Rect dest = {.x = lifeline_x + lifeline_w - 1, .y = lifeline_y, .h = 25, .w = 25};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
}

void render_lifeline_switch(SDL_Renderer* renderer, game_t* game){
    SDL_Texture* texture;
    if (game->lifeline_switch)
        texture = load_texture(renderer, LIFELINE_SWITCH_ON);
    else
        texture = load_texture(renderer, LIFELINE_SWITCH_OFF);
    SDL_Rect dest = {.x = lifeline_x + 2*lifeline_w, .y = lifeline_y, .h = 25, .w = 25};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
}

void render_running_state(SDL_Renderer* renderer, game_t* game, int* animate){
    switch(game->selection){
        case A_SELECTED:
            render_rect(renderer, &rect_a, &grey);
            break;

        case B_SELECTED:
            render_rect(renderer, &rect_b, &grey);
            break;

        case C_SELECTED:
            render_rect(renderer, &rect_c, &grey);
            break;

        case D_SELECTED:
            render_rect(renderer, &rect_d, &grey);
            break;

        case A_CONFIRMED:
            render_rect(renderer, &rect_a, &yellow);
            break;

        case B_CONFIRMED:
            render_rect(renderer, &rect_b, &yellow);
            break;

        case C_CONFIRMED:
            render_rect(renderer, &rect_c, &yellow);
            break;

        case D_CONFIRMED:
            render_rect(renderer, &rect_d, &yellow);
            break;

        default : {}

    }

    render_screen(renderer, RUNNING_BG);
    render_timer(renderer,game);
    render_lifeline_50(renderer,game);
    render_lifeline_25(renderer,game);
    render_lifeline_switch(renderer,game);
    if (*animate){
        //SDL_RenderPresent(renderer);
    }
    render_text(renderer,game,animate);    
}

void render_checking_state(SDL_Renderer* renderer, game_t* game){
    
    switch(game->selection){
        case A_CONFIRMED:
            render_rect(renderer, &rect_a, &red);
            break;

        case B_CONFIRMED:
            render_rect(renderer, &rect_b, &red);
            break;

        case C_CONFIRMED:
            render_rect(renderer, &rect_c, &red);
            break;

        case D_CONFIRMED:
            render_rect(renderer, &rect_d, &red);
            break;

        default : {}
    }

    question current_q = game->questions[game->question_number-1];

    switch(current_q.correct){
        case A_CORRECT:
            render_rect(renderer, &rect_a, &green);
            break;

        case B_CORRECT:
            render_rect(renderer, &rect_b, &green);
            break;

        case C_CORRECT:
            render_rect(renderer, &rect_c, &green);
            break;

        case D_CORRECT:
            render_rect(renderer, &rect_d, &green);
            break;

        default : {}
    }
    int animate = 0;
    render_screen(renderer, RUNNING_BG);
    render_timer(renderer,game);
    render_lifeline_50(renderer,game);
    render_lifeline_25(renderer,game);
    render_lifeline_switch(renderer,game);
    render_text(renderer,game,&animate);
}

void render_game(SDL_Renderer* renderer, game_t* game, int* animate, int* walk_away){
    
    switch(game->state){
        case RUNNING_STATE:
            render_running_state(renderer,game,animate);
            break;

        case CHECKING_STATE:
            render_checking_state(renderer,game);
            break;

        case GAME_OVER_STATE:
            render_game_over_state(renderer,game,walk_away);
            render_answer(renderer, 440, 455, "Press any key to continue", DEFAULT_FONT, white);
            //render_screen(renderer,"resources/surprise.png");
            break;        

        default : {} 
    }
}

// Menu rendering

int m_ans_a_x = 65;
int m_ans_a_y = 307;
int m_ans_b_x = 347;
int m_ans_b_y = 307;
int m_ans_c_x = 65;
int m_ans_c_y = 376;
int m_ans_d_x = 347;
int m_ans_d_y = 376;

void render_opts(SDL_Renderer* renderer, menu_t* menu){
    m_question current_q = menu->question[menu->type];
    render_answer(renderer, m_ans_a_x, m_ans_a_y, current_q.m_ans_a, DEFAULT_FONT, white);
    render_answer(renderer, m_ans_b_x, m_ans_b_y, current_q.m_ans_b, DEFAULT_FONT, white);
    render_answer(renderer, m_ans_c_x, m_ans_c_y, current_q.m_ans_c, DEFAULT_FONT, white);
    render_answer(renderer, m_ans_d_x, m_ans_d_y, current_q.m_ans_d, DEFAULT_FONT, white);
}

const SDL_Rect m_rect_a = { .x = 0, .y = 270, .w = 320, .h = 90};
const SDL_Rect m_rect_b = { .x = 320, .y = 270, .w = 320, .h = 90};
const SDL_Rect m_rect_c = { .x = 0, .y = 360, .w = 320, .h = 90};
const SDL_Rect m_rect_d = { .x = 320, .y = 360, .w = 320, .h = 90};

void render_menu(SDL_Renderer* renderer, menu_t* menu){
switch(menu->selection){
        case A_SELECTED:
            render_rect(renderer, &m_rect_a, &grey);
            break;

        case B_SELECTED:
            render_rect(renderer, &m_rect_b, &grey);
            break;

        case C_SELECTED:
            render_rect(renderer, &m_rect_c, &grey);
            break;

        case D_SELECTED:
            render_rect(renderer, &m_rect_d, &grey);
            break;

        default : {}

    }
    render_screen(renderer, MENU_BG);
    render_money(renderer, 10, 5, menu->user_id, DEFAULT_FONT);
    if (menu->type == ADMIN_MENU)
        render_answer(renderer, 440, 455, "Press F for admin options", DEFAULT_FONT, white);
    if (menu->type == INIT_MENU)
        render_answer(renderer, 5, 453, "By Saad El Jebbari & Dalia El Aiche", DEFAULT_FONT, white);
    render_opts(renderer, menu);
}

void render_stats(SDL_Renderer* renderer, stats* global_stats){
    render_answer(renderer, 450, 455, "Press any key to go back", DEFAULT_FONT, white);
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
    fclose(stat_file);

    render_25(renderer, 150, 100 - 25, "Top Score : ", DEFAULT_FONT);
    render_25(renderer, 400, 100 - 25, top_score, DEFAULT_FONT);

    render_25(renderer, 150, 150 - 25, "Held By : ", DEFAULT_FONT);
    render_25(renderer, 400, 150 - 25, top_score_holder, DEFAULT_FONT);
    
    render_25(renderer, 150, 200 - 25, "Number of users : ", DEFAULT_FONT);
    render_25(renderer, 400, 200 - 25, number_of_users, DEFAULT_FONT);
    
    render_25(renderer, 150, 250 - 25, "Correct answers : ", DEFAULT_FONT);
    render_25(renderer, 400, 250 - 25, correct_answers, DEFAULT_FONT);
    
    render_25(renderer, 150, 300 - 25, "50s used : ", DEFAULT_FONT);
    render_25(renderer, 400, 300 - 25, lifeline_50_uses, DEFAULT_FONT);
    
    render_25(renderer, 150, 350 - 25, "25s used : ", DEFAULT_FONT);
    render_25(renderer, 400, 350 - 25, lifeline_25_uses, DEFAULT_FONT);
    
    render_25(renderer, 150, 400 - 25, "Switches used: ", DEFAULT_FONT);
    render_25(renderer, 400, 400 - 25, lifeline_switch_uses, DEFAULT_FONT);
}

void render_text_input(SDL_Renderer* renderer, char* text, char* hidden, int level){
    char passwd[20] = "";
    // hiding password
    for (int i=0; i<strlen(hidden); i++)
        strcat(passwd,"*");
    render_25_centered(renderer, 320, 270, passwd, DEFAULT_FONT);
    render_25_centered(renderer, 320, 173, text, DEFAULT_FONT);
    render_screen(renderer, CRED_SCREEN);
    render_money(renderer, 275, 130, "Username : ", DEFAULT_FONT);
    render_money(renderer, 275, 225, "Password : ", DEFAULT_FONT);
    // Indicators
    if (level == 0){
        render_25(renderer, 150, 173, "O", DEFAULT_FONT);
        render_25(renderer, 470, 173, "O", DEFAULT_FONT);
    }

    else{
        render_25(renderer, 150, 270, "O", DEFAULT_FONT);
        render_25(renderer, 470, 270, "O", DEFAULT_FONT);
    }
    render_answer(renderer, 355, 455, "Press Enter to confirm and ESC to go back", DEFAULT_FONT, white);

}

void render_confirm_input(SDL_Renderer* renderer, char* text, char* confirm, int level){
    render_25_centered(renderer, 320, 270, confirm, DEFAULT_FONT);
    render_25_centered(renderer, 320, 173, text, DEFAULT_FONT);
    render_screen(renderer, CRED_SCREEN);
    render_money(renderer, 275, 130, "Username : ", DEFAULT_FONT);
    render_money(renderer, 285, 225, "Confirm : ", DEFAULT_FONT);
    // Indicators
    if (level == 0){
        render_25(renderer, 150, 173, "O", DEFAULT_FONT);
        render_25(renderer, 470, 173, "O", DEFAULT_FONT);
    }

    else{
        render_25(renderer, 150, 270, "O", DEFAULT_FONT);
        render_25(renderer, 470, 270, "O", DEFAULT_FONT);
    }
    render_answer(renderer, 355, 455, "Press Enter to confirm and ESC to go back", DEFAULT_FONT, white);

}

void render_question_input(SDL_Renderer* renderer, question* new_q, int level){
    render_answer(renderer, ans_a_x+25, ans_a_y, new_q->ans_a, DEFAULT_FONT, white);
    render_answer(renderer, ans_a_x, ans_a_y, "A:", DEFAULT_FONT, orange);

    render_answer(renderer, ans_b_x+25, ans_b_y, new_q->ans_b, DEFAULT_FONT, white);
    render_answer(renderer, ans_b_x, ans_b_y, "B:", DEFAULT_FONT, orange);

    render_answer(renderer, ans_c_x+25, ans_c_y, new_q->ans_c, DEFAULT_FONT, white);
    render_answer(renderer, ans_c_x, ans_c_y, "C:", DEFAULT_FONT, orange);

    render_answer(renderer, ans_d_x+25, ans_d_y, new_q->ans_d, DEFAULT_FONT, white);   
    render_answer(renderer, ans_d_x, ans_d_y, "D:", DEFAULT_FONT, orange);
        
    render_screen(renderer, RUNNING_BG);
    render_question(renderer, question_x, question_y, new_q->text, DEFAULT_FONT);

    render_20(renderer, 35, 280, "Be careful! Answer A will be taken as the correct one", DEFAULT_FONT);
    render_answer(renderer, 355, 455, "Press Enter to confirm and ESC to go back", DEFAULT_FONT, white);
}

void render_instructions(SDL_Renderer* renderer){
    render_screen(renderer,"resources/instructs.png");
    render_20(renderer, 75, 130, "50           25      switch", DEFAULT_FONT);
    render_20(renderer, 120, 310, "A             B             C             D", DEFAULT_FONT); 
    render_20(renderer, 450, 260, "confirm", DEFAULT_FONT);    
}