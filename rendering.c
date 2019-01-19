#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "game.h"
#include "rendering.h"

const SDL_Rect rect_a = { .x = 0, .y = 300, .w = 320, .h = 90};
const SDL_Rect rect_b = { .x = 320, .y = 300, .w = 320, .h = 90};
const SDL_Rect rect_c = { .x = 0, .y = 390, .w = 320, .h = 90};
const SDL_Rect rect_d = { .x = 320, .y = 390, .w = 320, .h = 90};

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

const SDL_Color green = { .r = 43, .g = 198, .b = 135};
const SDL_Color red = { .r = 180, .g = 0, .b = 0};
const SDL_Color yellow = { .r = 250, .g = 154, .b = 20};
const SDL_Color grey = { .r = 100, .g = 100, .b = 100};
const SDL_Color white = { .r = 255, .g = 255, .b = 255};

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

void render_game_over_state(SDL_Renderer* renderer, game_t* game){
    switch (game->question_number) {
    case 1:
        render_screen(renderer, H_SCREEN);
        break;

    case 2:
        render_screen(renderer, TWOH_SCREEN);
        break;

    case 3:
        render_screen(renderer, THREEH_SCREEN);
        break;

    case 4:
        render_screen(renderer, FIVEH_SCREEN);
        break;

    case 5:
        render_screen(renderer, ONEK_SCREEN);
        break;
    
    case 6:
        render_screen(renderer, TWOK_SCREEN);
        break;
    
    case 7:
        render_screen(renderer, FOURK_SCREEN);
        break;
    
    case 8:
        render_screen(renderer, EIGHTK_SCREEN);
        break;
    
    case 9:
        render_screen(renderer, SIXTEENK_SCREEN);
        break;
    
    case 10:
        render_screen(renderer, THIRTYTWOK_SCREEN);
        break;
    
    case 11:
        render_screen(renderer, SIXTYFOURK_SCREEN);
        break;
    
    case 12:
        render_screen(renderer, ONETWENTYFIVEK_SCREEN);
        break;
    
    case 13:
        render_screen(renderer, TWOFIFTYK_SCREEN);
        break;
    
    case 14:
        render_screen(renderer, FIVEHK_SCREEN);
        break;
    
    case 15:
        render_screen(renderer, WIN_SCREEN);
        break;
    
    default: {}
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
    Uint32 wrap = 460;

    SDL_Surface * surface = TTF_RenderText_Blended_Wrapped(font, text, white, wrap);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);
    TTF_Quit();
}

void render_answer(SDL_Renderer* renderer, int x, int y, const char* text, const char* font_name){
    TTF_Init();
    TTF_Font * font;
    if(strlen(text) < 16)
        font = TTF_OpenFont(font_name, 25);
    else if (strlen(text) < 22)
        font = TTF_OpenFont(font_name, 20);
    else
        font = TTF_OpenFont(font_name, 17);
    
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

void render_running_state(SDL_Renderer* renderer, game_t* game){
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

    const char *SAMPLETEXT = "This is an example of my problem, for most lines it works fine, albeit it looks a bit tight. But for any letters that \"hang\" below the line.";
    render_screen(renderer, RUNNING_BG);
    render_question(renderer, question_x, question_y, SAMPLETEXT, DEFAULT_FONT);
    render_answer(renderer, ans_a_x, ans_a_y, "A:Shadenfreude", DEFAULT_FONT);
    render_answer(renderer, ans_b_x, ans_b_y, "B:Ireland", DEFAULT_FONT);
    render_answer(renderer, ans_c_x, ans_c_y, "C:Greenland", DEFAULT_FONT);
    render_answer(renderer, ans_d_x, ans_d_y, "D:New Zealand", DEFAULT_FONT);
    
}


void render_game(SDL_Renderer *renderer, game_t *game){
    render_running_state(renderer,game);
}
