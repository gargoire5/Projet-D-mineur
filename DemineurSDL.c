#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINSOW_WIDTH 940
#define WINSOW_HEIGHT 940

void createBomb(int** tab, int** tabHidden, int* dimension, int* bomb);

void SDL_ExitWithError(const char* message);
void pause(int** tabHidden);


int main(int argc, char* argv[])
{
    SDL_Window* fenetre = NULL;
    SDL_Renderer* renderer = NULL;
    int x, y;

    // Lancement du SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        SDL_ExitWithError("Initialisation de la SDL");

    if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur initialisation SDL_mixer : %s", Mix_GetError());
        SDL_Quit();
        return -1;
    }

    Mix_Music* music = Mix_LoadMUS("src/palette.mp3");

    if (music == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur chargement de la musique : %s", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return -1;
    }

    Mix_PlayMusic(music, -1);
    Uint8 volume = 10;
    Mix_VolumeMusic(volume); // Mets le volume a 0


    // Creation de la fenêtre
    fenetre = SDL_CreateWindow("Pokebomb", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINSOW_WIDTH, WINSOW_HEIGHT, 0);

    if (fenetre == NULL)
        SDL_ExitWithError("Création fenetre echouee");

    // Création du rendu
    renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_SOFTWARE);

    if (renderer == NULL)
        SDL_ExitWithError("Creation rendu echouee");


    /*-------------------------------------------------------------------------------*/
    int dimension = 8, column, line, bomb = 10;
    column = dimension;
    line = dimension;

    int i, j;
    int** tab = malloc(line * sizeof(int*));    //malloc le tableau tab
    for (i = 0; i < line; i++)
    {
        tab[i] = malloc(column * sizeof(int));
        for (int j = 0; j < column; j++)
        {
            tab[i][j] = 0;
        }
    }

    int** tabHidden = malloc(line * sizeof(int*));  //malloc le teableau tabHidden
    for (i = 0; i < line; i++)
    {
        tabHidden[i] = malloc(column * sizeof(int));
        for (int j = 0; j < column; j++)
        {
            tabHidden[i][j] = 0;
        }

    }

    createBomb(tab, tabHidden, &dimension, &bomb);

    /*-------------------------------------------------------------------------------*/


    i = 0;
    j = 0;
    for (x = 50; x < dimension * 100 + 50; x += 105) {
        for (y = 50; y < dimension * 100 + 50; y += 105) {
            SDL_Surface* carre = SDL_LoadBMP("src/map.bmp");
            SDL_Texture* textf = NULL;

            if (carre == NULL) {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(fenetre);
                SDL_ExitWithError("Création surface echouee");
            }

            textf = SDL_CreateTextureFromSurface(renderer, carre);
            SDL_FreeSurface(carre);

            SDL_Rect a = { x,y,100,100 };
            if (SDL_QueryTexture(textf, NULL, NULL, &a.w, &a.h) != 0) {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(fenetre);
                SDL_ExitWithError("Création text echouee");
            }
            if (SDL_RenderCopy(renderer, textf, NULL, &a) != 0) {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(fenetre);
                SDL_ExitWithError("Création text1 echouee");
            }
            SDL_DestroyTexture(textf);
            SDL_RenderPresent(renderer);


            i++;
            if (i == 7) {
                i = 0;
            }
        }
        j++;
    }


    SDL_bool program_lauchned = SDL_TRUE;
    SDL_Event event;

    while (program_lauchned)
    {

        while (SDL_WaitEvent(&event))
        {

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p)
                Mix_PauseMusic(); // Mets en pause la musique 
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r)
                Mix_ResumeMusic(); // Reprend la lecture 
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s)
                Mix_RewindMusic(); // Revient au début de la musique
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP && volume < MIX_MAX_VOLUME)
                volume++; // Augmente le volume jusqu'a MIX_MAX_VOLUME
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN && volume > 0)
                volume--; // Réduit le volume jusqu'a 0
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                Mix_HaltMusic(); // Arreter la musique

            if (event.type == SDL_KEYDOWN)
                Mix_VolumeMusic(volume); // Applique le volume desirer

            switch (event.type)
            {

            case SDL_MOUSEBUTTONDOWN:

                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    printf("Click gauche en %dx %dy \n", event.button.x, event.button.y);
                    int caseX = ((event.button.x - 50) / 105);
                    int caseY = ((event.button.y - 50) / 105);
                    int coordCaseX = ((caseX) * 105) + 50;
                    int coordCaseY = ((caseY) * 105) + 50;

                    /*-----------------------------------------------------------*/
                    if (tabHidden[caseY][caseX] != 1)
                    {
                        tabHidden[caseY][caseX] = 2;
                        printf("case X = %d\ncase Y = %d\ntabHidden[caseX][caseY] = %d\n", caseX, caseY, tabHidden[caseY][caseX]);
                        for (x = coordCaseX; x < coordCaseX + 100; x += 100) {
                            for (y = coordCaseY; y < coordCaseY + 100; y += 100) {
                                SDL_Surface* drap = SDL_LoadBMP("src/drap.bmp");
                                SDL_Texture* textf = NULL;

                                if (drap == NULL) {
                                    SDL_DestroyRenderer(renderer);
                                    SDL_DestroyWindow(fenetre);
                                    SDL_ExitWithError("Création surface echouee");
                                }

                                textf = SDL_CreateTextureFromSurface(renderer, drap);
                                SDL_FreeSurface(drap);

                                SDL_Rect a = { x,y,100,100 };
                                if (SDL_QueryTexture(textf, NULL, NULL, &a.w, &a.h) != 0) {
                                    SDL_DestroyRenderer(renderer);
                                    SDL_DestroyWindow(fenetre);
                                    SDL_ExitWithError("Création text echouee");
                                }
                                if (SDL_RenderCopy(renderer, textf, NULL, &a) != 0) {
                                    SDL_DestroyRenderer(renderer);
                                    SDL_DestroyWindow(fenetre);
                                    SDL_ExitWithError("Création text1 echouee");
                                }
                                SDL_DestroyTexture(textf);
                                SDL_RenderPresent(renderer);

                            }
                        }
                    }

                    /*-----------------------------------------------------------*/



                }


                else if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int caseX = ((event.button.x - 50) / 105);
                    int caseY = ((event.button.y - 50) / 105);
                    int coordCaseX = ((caseX) * 105) + 50;
                    int coordCaseY = ((caseY) * 105) + 50;

                    /*-----------------------------------------------------------*/
                    if (tab[caseY][caseX] < 9)
                    {
                        tabHidden[caseY][caseX] = 1;
                        printf("case X = %d\ncase Y = %d\ntabHidden[caseX][caseY] = %d\n", caseX, caseY, tabHidden[caseY][caseX]);
                        for (x = coordCaseX; x < coordCaseX + 100; x += 100) {
                            for (y = coordCaseY; y < coordCaseY + 100; y += 100) {
                                SDL_Surface* drap = SDL_LoadBMP("src/8.bmp");
                                SDL_Texture* textf = NULL;

                                if (drap == NULL) {
                                    SDL_DestroyRenderer(renderer);
                                    SDL_DestroyWindow(fenetre);
                                    SDL_ExitWithError("Création surface echouee");
                                }

                                textf = SDL_CreateTextureFromSurface(renderer, drap);
                                SDL_FreeSurface(drap);

                                SDL_Rect a = { x,y,100,100 };
                                if (SDL_QueryTexture(textf, NULL, NULL, &a.w, &a.h) != 0) {
                                    SDL_DestroyRenderer(renderer);
                                    SDL_DestroyWindow(fenetre);
                                    SDL_ExitWithError("Création text echouee");
                                }
                                if (SDL_RenderCopy(renderer, textf, NULL, &a) != 0) {
                                    SDL_DestroyRenderer(renderer);
                                    SDL_DestroyWindow(fenetre);
                                    SDL_ExitWithError("Création text1 echouee");
                                }
                                SDL_DestroyTexture(textf);
                                SDL_RenderPresent(renderer);

                            }
                        }
                    }
                    else {
                        i = 0;
                        j = 0;
                        for (x = 50; x < dimension * 100 + 50; x += 105) {
                            for (y = 50; y < dimension * 100 + 50; y += 105) {
                                int xTemp = ((x - 50) / 105);
                                int yTemp = ((y - 50) / 105);
                                if (tab[yTemp][xTemp] != 9) {
                                    SDL_Surface* carre = SDL_LoadBMP("src/map.bmp");
                                    SDL_Texture* textf = NULL;

                                    if (carre == NULL) {
                                        SDL_DestroyRenderer(renderer);
                                        SDL_DestroyWindow(fenetre);
                                        SDL_ExitWithError("Création surface echouee");
                                    }

                                    textf = SDL_CreateTextureFromSurface(renderer, carre);
                                    SDL_FreeSurface(carre);

                                    SDL_Rect a = { x,y,100,100 };
                                    if (SDL_QueryTexture(textf, NULL, NULL, &a.w, &a.h) != 0) {
                                        SDL_DestroyRenderer(renderer);
                                        SDL_DestroyWindow(fenetre);
                                        SDL_ExitWithError("Création text echouee");
                                    }
                                    if (SDL_RenderCopy(renderer, textf, NULL, &a) != 0) {
                                        SDL_DestroyRenderer(renderer);
                                        SDL_DestroyWindow(fenetre);
                                        SDL_ExitWithError("Création text1 echouee");
                                    }
                                    SDL_DestroyTexture(textf);
                                    SDL_RenderPresent(renderer);
                                }
                                else {
                                    SDL_Surface* bomb = SDL_LoadBMP("src/bomb.bmp");
                                    SDL_Texture* textf = NULL;

                                    if (bomb == NULL) {
                                        SDL_DestroyRenderer(renderer);
                                        SDL_DestroyWindow(fenetre);
                                        SDL_ExitWithError("Création surface echouee");
                                    }

                                    textf = SDL_CreateTextureFromSurface(renderer, bomb);
                                    SDL_FreeSurface(bomb);

                                    SDL_Rect a = { x,y,100,100 };
                                    if (SDL_QueryTexture(textf, NULL, NULL, &a.w, &a.h) != 0) {
                                        SDL_DestroyRenderer(renderer);
                                        SDL_DestroyWindow(fenetre);
                                        SDL_ExitWithError("Création text echouee");
                                    }
                                    if (SDL_RenderCopy(renderer, textf, NULL, &a) != 0) {
                                        SDL_DestroyRenderer(renderer);
                                        SDL_DestroyWindow(fenetre);
                                        SDL_ExitWithError("Création text1 echouee");
                                    }
                                    SDL_DestroyTexture(textf);
                                    SDL_RenderPresent(renderer);

                                }



                                i++;
                                if (i == 7) {
                                    i = 0;
                                }
                            }
                            j++;
                        }
                    }


                    /*-----------------------------------------------------------*/
                }
                break;


            case SDL_QUIT:
                program_lauchned = SDL_FALSE;
                break;

            default:
                break;
            }
        }
    }

    SDL_RenderPresent(renderer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();

    return EXIT_FAILURE;
}

void  SDL_ExitWithError(const char* message)
{
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}


void pause(int** tabHidden) {
    SDL_bool program_lauchned = SDL_TRUE;
    SDL_Event event;

    while (program_lauchned)
    {

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    printf("Click gauche en %dx %dy \n", event.button.x, event.button.y);
                int caseX = ((event.button.x - 50) / 105);
                int caseY = ((event.button.y - 50) / 105);
                tabHidden[caseY][caseX] = 1;
                printf("case X = %d\ncase Y = %d\ntabHidden[caseX][caseY] = %d\n", caseX, caseY, tabHidden[caseY][caseX]);
                if (event.button.button == SDL_BUTTON_RIGHT)
                    printf("Click droit en %dx %dy \n", event.button.x, event.button.y);
                break;


            case SDL_QUIT:
                program_lauchned = SDL_FALSE;
                break;

            default:
                break;
            }
        }
    }
}





void createBomb(int** tab, int** tabHidden, int* dimension, int* bomb)
{
    srand(time(NULL));
    for (int i = 0; i < *bomb; i++)
    {
        printf("%d", i);
        int j = rand() % 8;
        int k = rand() % 8;
        if (tab[j][k] != 9) {
            tab[j][k] = 9;
        }
    }
}
