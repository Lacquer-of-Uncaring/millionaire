#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "game.h"
#include "rendering.h"

#define WIN_SCREEN "resources/WIN.png"

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

void render_win_state(SDL_Renderer* renderer){

	SDL_Texture* texture = load_texture(renderer, WIN_SCREEN);
	SDL_RenderCopy(renderer, texture, NULL, NULL);

}