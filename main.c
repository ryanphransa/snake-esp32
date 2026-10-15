#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#define screen_height 500
#define screen_width 500

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window =
        SDL_CreateWindow("My Game", screen_height, screen_width, 0);

    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, NULL);

    bool running = true;

    float player_x = 10;
    float player_y = 300;

    char player_dir = 'r';
    char player_dir_pendent;

    float player_x_tick = 0;
    float player_y_tick = 0;

    float apple_x = arc4random_uniform(471);
    float apple_y = arc4random_uniform(471);

    float tail_x[screen_width*screen_height];
    float tail_y[screen_width*screen_height];

    float tail_size = 2;

    int i = 0;
    size_t tick = 0;

    bool death = false;

    while (running)
    {
        SDL_Event event;

        SDL_FRect player = {
            player_x,
            player_y,
            30,
            30
        };

        SDL_FRect tail[screen_height*screen_width];

        SDL_FRect apple = {
            apple_x,
            apple_y,
            30,
            30
        };

        /* INPUT */
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        /* UPDATE */
        const bool *keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_LEFT]) {
            player_dir_pendent = 'l';
        }

        if (keys[SDL_SCANCODE_RIGHT]) {
            player_dir_pendent = 'r';
        }

        if (keys[SDL_SCANCODE_UP]) {
            player_dir_pendent = 'u';
        }

        if (keys[SDL_SCANCODE_DOWN]) {
            player_dir_pendent = 'd';
        }

        //if ((int)player_x % 30 == 0 && (int)player_y % 30 == 0)
            player_dir = player_dir_pendent;

        if (SDL_HasRectIntersectionFloat(&player, &apple)) {
            ++tail_size;
            apple_x = 30 * arc4random_uniform(16);
            apple_y = 30 * arc4random_uniform(16);
        }

        for(size_t m = 2; m < tail_size; m++) {
            if (SDL_HasRectIntersectionFloat(&player, &tail[m])) {
                death = true;
            }
        }

        /* RENDER */
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        if (player_x+32 > screen_width ||
            player_x < 0 ||
            player_y+32 > screen_height ||
            player_y < 0) {
            break;
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &apple);

        if (tick == 1300) {
            switch (player_dir) {
                case 'l':
                    player_x -= 30.0;
                    break;
                case 'r':
                    player_x += 30.0;
                    break;
                case 'u':
                    player_y -= 30.0;
                    break;
                case 'd':
                    player_y += 30.0;
                    break;
            }
        }

        while (tick == 1300) {
            for (size_t i = tail_size; i > 0; i--) {
                tail_x[i] = tail_x[i-1];
                tail_y[i] = tail_y[i-1];
                tail[i] = (SDL_FRect){tail_x[i], tail_y[i], 30, 30};
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &tail[i]);
            }
            /*tail_x[1] = tail_x[0];
            tail_y[1] = tail_y[0];
            tail[1] = (SDL_FRect){tail_x[1], tail_y[1], 30, 30};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &tail[1]);*/

            tail_x[0] = player_x_tick;
            tail_y[0] = player_y_tick;
            tail[0] = (SDL_FRect){tail_x[0], tail_y[0], 30, 30};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &tail[0]);

            player_y_tick = player_y;
            player_x_tick = player_x;

            tick = 0;
        }

        for (size_t i = 0; i < tail_size; i++) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &tail[i]);
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &player);

        SDL_RenderPresent(renderer);
        ++tick;
        if (death)
            break;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
