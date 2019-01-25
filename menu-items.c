#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include "game.h"
#include "logic.h"
#include "menu-items.h"
#include "rendering.h"

// Root user initialisation
user_t root = {
    .id = "root",
    .password = "root",
    .admin = 1
};

// Current user
user_t current_user; 

node head = {&root, NULL};
node* list_root = &head;

// User logic

node* user_search(node* head, char* id){
    // Base case 
    if (head == NULL) 
        return NULL;      
    // If id is in node
    if (!strcmp(head->user->id, id))
        return head;  
    // Recur for remaining list 
    return user_search(head->next, id);
}

int compare(node *one , node *two){
    return strcmp(one->user->id, two->user->id);
}

int user_add(node **pp, char* id, char* passwd, int admin) {
    node* new = malloc(sizeof(node));
    user_t* user = malloc(sizeof(user_t));
    new->user = user;
    strcpy(new->user->id,id);
    strcpy(new->user->password,passwd);
    new->user->admin = admin;

    for ( ; *pp != NULL; pp = &(*pp)->next) {
        if (!strcmp((*pp)->user->id,id))
            return 1;
    }

    new->next = *pp;
    *pp = new;
    return 0;
}

void user_remove(node** head, char* id){
    int present = 0;
    node* old;
    node** tracer = head;
    
    while((*tracer) && !(present = (strcmp(id,(*tracer)->user->id) == 0 )  ))
        tracer = &(*tracer)->next;

    if(present){
        old = *tracer;
        *tracer = (*tracer)->next;
        free(old->user); // free off space used by adherent struct
        free(old); // free up remainder of node 
    }
}

void fetch_users(node* head){
    FILE* user_file = fopen("records/users", "r");
    char id[20];
    char password[20];
    int admin;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, user_file)) != -1) {
        sscanf(line,"%s %s %d",id,password,&admin);
        user_add(&head,id,password,admin);
    }

    fclose(user_file);
    if (line)
        free(line);
}

void record_users(node* head){
    node* current = head;
    FILE* user_file = fopen("records/users", "w");
    while(current != NULL){
        if (strcmp(current->user->id,"root"))
            fprintf(user_file, "%s %s %d\n",current->user->id,current->user->password,current->user->admin);
        current = current->next;
    }
    fclose(user_file);
}

void passwd_input(SDL_Renderer* renderer, menu_t* menu, user_t* username, char* id, char* passwd, int* success){
    int done = 0;
    int level = 1;
    SDL_StartTextInput();
    while (!done) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    /* Quit */
                    done = 1;
                    menu->state = QUIT;
                    break;
                case SDL_TEXTINPUT:
                    /* Add new id onto the end of our id */
                    if (strlen(passwd) < 20)
                        strcat(passwd, e.text.text);
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.scancode){ 
                    case SDL_SCANCODE_ESCAPE:
                        //passwd = calloc(1,21);
                        passwd[0] = 0;
                        done = 1;
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        passwd[strlen(passwd)-1] = 0;
                        break;
                    case SDL_SCANCODE_RETURN:
                        if (!strcmp(username->password, passwd)){
                            // Successful login!
                            if (username->admin)
                                menu->type = ADMIN_MENU;
                            else
                                menu->type = USER_MENU;
                            strcpy(menu->user_id,username->id);
                            menu->selection = NO_SELECTION;
                            *success = 1;
                            done = 1;
                        }
                        else{
                            passwd[0] = 0;
                        }
                            
                        break;
                    }
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_text_input(renderer,id,passwd,level);
        SDL_RenderPresent(renderer);
    
        SDL_Delay(1000/60);
    }
}

void game_loop(SDL_Renderer* renderer, game_t* game, menu_t* menu){
	// Initializing some parameters for logic functions
    int init = SDL_GetTicks();
    int x,y;
    int animate = 1;
    int walk_away = 0;

    // Event handling
    SDL_Event e;
    while (game->state != QUIT_STATE) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
            // Exit the program entirely
                game->state = QUIT_STATE;
                menu->state = QUIT;
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.scancode){ 
                case SDL_SCANCODE_ESCAPE:
                // Go back to menu 
                    walk_away = 1;
                    game->state = GAME_OVER_STATE;
                    //menu->state = RUNNING;
                    //menu->selection = NO_SELECTION;
                    break;
                case SDL_SCANCODE_1:
                    use_lifeline_50(game);
                    break;
                case SDL_SCANCODE_2:
                    use_lifeline_25(game);
                    break;
                case SDL_SCANCODE_3:
                    use_lifeline_switch(game,&animate);
                    break;

                case SDL_SCANCODE_A:
                    // Can only select in running state and before confirming
                    if (game->state == RUNNING_STATE){
                        if (game->selection < 5 && game->A_available)
                            game->selection = A_SELECTED;
                    }
                    break;
                case SDL_SCANCODE_B:
                    if (game->state == RUNNING_STATE){
                        if (game->selection < 5 && game->B_available)
                            game->selection = B_SELECTED;
                    }
                    break;
                case SDL_SCANCODE_C:
                    if (game->state == RUNNING_STATE){
                        if (game->selection < 5 && game->C_available)
                            game->selection = C_SELECTED;
                    }
                    break;
                case SDL_SCANCODE_D:
                    if (game->state == RUNNING_STATE){
                        if (game->selection < 5 && game->D_available)
                            game->selection = D_SELECTED;
                    }
                    break;
                case SDL_SCANCODE_RETURN:
                case SDL_SCANCODE_KP_ENTER:
                case SDL_SCANCODE_SPACE:
                    answer_confirm(game);
                    break;
                case SDL_SCANCODE_BACKSPACE:
                    if (game->state == RUNNING_STATE)
                        if (game->selection < 5)
                            game->selection = NO_SELECTION;
                    break;
                }
                break;
            // Mouse selection 
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&x,&y);
                hover_select(game, x, y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                answer_confirm(game);
                break;

            default: {}
            }
        }
        // Done with event handling for the frame

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        decrement_timer(game, &init);
        render_game(renderer, game, &animate, &walk_away);
        SDL_RenderPresent(renderer);
        
        check_game_over_state(game, menu, &animate, &walk_away);
        // FPS
        SDL_Delay(1000/60);
        SDL_FlushEvent(SDL_KEYDOWN);
        SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
        SDL_FlushEvent(SDL_MOUSEMOTION);
    }
}

void username_input(SDL_Renderer* renderer, menu_t* menu, char* id, char* passwd){
    fetch_users(&head);
    int done = 0;
    int level = 0;
    int success = 0;
    node* username;
    SDL_StartTextInput();
    while (!done && !success) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    /* Quit */
                    done = 1;
                    menu->state = QUIT;
                    break;
                case SDL_TEXTINPUT:
                    /* Add new id onto the end of our id */
                    if (strlen(id) < 20)
                        strcat(id, e.text.text);
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.scancode){ 
                    case SDL_SCANCODE_ESCAPE:
                        done = 1;
                        id = calloc(1,21);
                        menu->selection = NO_SELECTION;
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        id[strlen(id)-1] = 0;
                        break;
                    case SDL_SCANCODE_RETURN:
                        username = user_search(&head,id);
                        if (username == NULL){
                            id = calloc(1,21);
                        }
                        else
                            passwd_input(renderer,menu,username->user,id,passwd,&success);
                        break;
                    }
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_text_input(renderer,id,passwd,level);
        SDL_RenderPresent(renderer);
    
        SDL_Delay(1000/60);
    }
    menu->selection = NO_SELECTION;
}


void signup_passwd_input(SDL_Renderer* renderer, menu_t* menu, char* id, char* passwd, int* success){
    int done = 0;
    int level = 1;
    SDL_StartTextInput();
    while (!done) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    /* Quit */
                    done = 1;
                    menu->state = QUIT;
                    break;
                case SDL_TEXTINPUT:
                    /* Add new id onto the end of our id */
                    if (strlen(passwd) < 20)
                        strcat(passwd, e.text.text);
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.scancode){ 
                    case SDL_SCANCODE_ESCAPE:
                        //passwd = calloc(1,21);
                        passwd[0] = 0;
                        done = 1;
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        passwd[strlen(passwd)-1] = 0;
                        break;
                    case SDL_SCANCODE_RETURN:
                        if (strlen(passwd)){
                            // Successful signup!
                            user_add(&list_root,id,passwd,0);
                            record_users(&head);
                            increment_user_number();                          
                            menu->selection = NO_SELECTION;
                            *success = 1;
                            done = 1;
                        }
                        else{
                            passwd[0] = 0;
                        }
                            
                        break;
                    }
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_text_input(renderer,id,passwd,level);
        SDL_RenderPresent(renderer);
    
        SDL_Delay(1000/60);
    }
}

void signup_input(SDL_Renderer* renderer, menu_t* menu, char* id, char* passwd){
    fetch_users(&head);
    int done = 0;
    int level = 0;
    int success = 0;
    node* username;
    SDL_StartTextInput();
    while (!done && !success) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    /* Quit */
                    done = 1;
                    menu->state = QUIT;
                    break;
                case SDL_TEXTINPUT:
                    /* Add new id onto the end of our id */
                    if (strlen(id) < 20)
                        strcat(id, e.text.text);
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.scancode){ 
                    case SDL_SCANCODE_ESCAPE:
                        done = 1;
                        id = calloc(1,21);
                        menu->selection = NO_SELECTION;
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        id[strlen(id)-1] = 0;
                        break;
                    case SDL_SCANCODE_RETURN:
                        username = user_search(&head,id);
                        if (username != NULL){
                            id = calloc(1,21);
                            render_answer(renderer, 5, 450, "Username Taken :c", DEFAULT_FONT);
                            SDL_RenderPresent(renderer);
                            SDL_Delay(500);
                        }
                        else
                            signup_passwd_input(renderer,menu,id,passwd,&success);
                        break;
                    }
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_text_input(renderer,id,passwd,level);
        SDL_RenderPresent(renderer);
    
        SDL_Delay(1000/60);
    }
    menu->selection = NO_SELECTION;
}

void confirm_admin_input(SDL_Renderer* renderer, menu_t* menu, user_t* username, char* id, char* confirm, int* success){
    int done = 0;
    int level = 1;
    SDL_StartTextInput();
    while (!done) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    /* Quit */
                    done = 1;
                    menu->state = QUIT;
                    break;
                case SDL_TEXTINPUT:
                    /* Add new id onto the end of our id */
                    if (strlen(confirm) < 20)
                        strcat(confirm, e.text.text);
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.scancode){ 
                    case SDL_SCANCODE_ESCAPE:
                        //confirm = calloc(1,21);
                        confirm[0] = 0;
                        done = 1;
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        confirm[strlen(confirm)-1] = 0;
                        break;
                    case SDL_SCANCODE_RETURN:
                        if (!strcmp(username->id, confirm)){
                            // Successful deletion!
                            user_remove(&list_root,confirm);
                            record_users(&head);
                            menu->selection = NO_SELECTION;
                            *success = 1;
                            done = 1;
                        }
                        else{
                            confirm[0] = 0;
                        }
                            
                        break;
                    }
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_confirm_input(renderer,id,confirm,level);
        SDL_RenderPresent(renderer);
    
        SDL_Delay(1000/60);
    }
}

void admin_input(SDL_Renderer* renderer, menu_t* menu, char* id, char* confirm){
    fetch_users(&head);
    int done = 0;
    int level = 0;
    int success = 0;
    node* username;
    SDL_StartTextInput();
    while (!done && !success) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    /* Quit */
                    done = 1;
                    menu->state = QUIT;
                    break;
                case SDL_TEXTINPUT:
                    /* Add new id onto the end of our id */
                    if (strlen(id) < 20)
                        strcat(id, e.text.text);
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.scancode){ 
                    case SDL_SCANCODE_ESCAPE:
                        done = 1;
                        id = calloc(1,21);
                        menu->selection = NO_SELECTION;
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        id[strlen(id)-1] = 0;
                        break;
                    case SDL_SCANCODE_RETURN:
                        username = user_search(&head,id);
                        // can't delete root or your account
                        if (username == NULL || !strcmp(username->user->id,"root") || !strcmp(username->user->id,menu->user_id)){
                            id = calloc(1,21);
                        }
                        else
                            confirm_admin_input(renderer,menu,username->user,id,confirm,&success);
                        break;
                    }
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_confirm_input(renderer,id,confirm,level);
        SDL_RenderPresent(renderer);
    
        SDL_Delay(1000/60);
    }
    menu->selection = NO_SELECTION;
}

void confirm_rights_input(SDL_Renderer* renderer, menu_t* menu, user_t* username, char* id, char* confirm, int* success){
    int done = 0;
    int level = 1;
    SDL_StartTextInput();
    while (!done) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    /* Quit */
                    done = 1;
                    menu->state = QUIT;
                    break;
                case SDL_TEXTINPUT:
                    /* Add new id onto the end of our id */
                    if (strlen(confirm) < 20)
                        strcat(confirm, e.text.text);
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.scancode){ 
                    case SDL_SCANCODE_ESCAPE:
                        //confirm = calloc(1,21);
                        confirm[0] = 0;
                        done = 1;
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        confirm[strlen(confirm)-1] = 0;
                        break;
                    case SDL_SCANCODE_RETURN:
                        if (!strcmp(username->id, confirm)){
                            // Successful confirm!
                            if (username->admin)
                                username->admin = 0;
                            else
                                username->admin = 1;
                            record_users(&head);
                            menu->selection = NO_SELECTION;
                            *success = 1;
                            done = 1;
                        }
                        else{
                            confirm[0] = 0;
                        }
                            
                        break;
                    }
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_confirm_input(renderer,id,confirm,level);
        SDL_RenderPresent(renderer);
    
        SDL_Delay(1000/60);
    }
}

void rights_input(SDL_Renderer* renderer, menu_t* menu, char* id, char* confirm){
    fetch_users(&head);
    int done = 0;
    int level = 0;
    int success = 0;
    node* username;
    SDL_StartTextInput();
    while (!done && !success) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    /* Quit */
                    done = 1;
                    menu->state = QUIT;
                    break;
                case SDL_TEXTINPUT:
                    /* Add new id onto the end of our id */
                    if (strlen(id) < 20)
                        strcat(id, e.text.text);
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.scancode){ 
                    case SDL_SCANCODE_ESCAPE:
                        done = 1;
                        id = calloc(1,21);
                        menu->selection = NO_SELECTION;
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        id[strlen(id)-1] = 0;
                        break;
                    case SDL_SCANCODE_RETURN:
                        username = user_search(&head,id);
                        // can't delete root or your account
                        if (username == NULL || !strcmp(username->user->id,"root") || !strcmp(username->user->id,menu->user_id)){
                            id = calloc(1,21);
                        }
                        else
                            confirm_rights_input(renderer,menu,username->user,id,confirm,&success);
                        break;
                    }
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_confirm_input(renderer,id,confirm,level);
        SDL_RenderPresent(renderer);
    
        SDL_Delay(1000/60);
    }
    menu->selection = NO_SELECTION;
}
