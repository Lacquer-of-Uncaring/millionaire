#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "game.h"
#include "rendering.h"


SDL_Texture* load_texture(SDL_Renderer* renderer,const char* path){
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

void render_end_screen(SDL_Renderer* renderer, const char* path){
    // simply show the chosen end screen
	SDL_Texture* texture = load_texture(renderer, path);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
}

void render_game_over_state(SDL_Renderer* renderer, game_t* game){
    switch (game->question_number) {
    case 1:
        render_end_screen(renderer, H_SCREEN);
        break;

    case 2:
        render_end_screen(renderer, TWOH_SCREEN);
        break;

    case 3:
        render_end_screen(renderer, THREEH_SCREEN);
        break;

    case 4:
        render_end_screen(renderer, FIVEH_SCREEN);
        break;

    case 5:
        render_end_screen(renderer, ONEK_SCREEN);
        break;
    
    case 6:
        render_end_screen(renderer, TWOK_SCREEN);
        break;
    
    case 7:
        render_end_screen(renderer, FOURK_SCREEN);
        break;
    
    case 8:
        render_end_screen(renderer, EIGHTK_SCREEN);
        break;
    
    case 9:
        render_end_screen(renderer, SIXTEENK_SCREEN);
        break;
    
    case 10:
        render_end_screen(renderer, THIRTYTWOK_SCREEN);
        break;
    
    case 11:
        render_end_screen(renderer, SIXTYFOURK_SCREEN);
        break;
    
    case 12:
        render_end_screen(renderer, ONETWENTYFIVEK_SCREEN);
        break;
    
    case 13:
        render_end_screen(renderer, TWOFIFTYK_SCREEN);
        break;
    
    case 14:
        render_end_screen(renderer, FIVEHK_SCREEN);
        break;
    
    case 15:
        render_end_screen(renderer, WIN_SCREEN);
        break;
    
    default: {}
    }
}


void render_game(SDL_Renderer *renderer, game_t *game){
    render_game_over_state(renderer, game);
}