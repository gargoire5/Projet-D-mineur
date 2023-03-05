#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Spell {
    char* name;
    int price;
    int own;
} Spell;

void inputPlayer(int* column, int* line);
void createBomb(int** tab, int** tabHidden, int* column, int* line);
void spellExplosion(int** tab, int** tabHidden, int* lineChoose, int* columnChoose, int* column, int* line);
void initBombAround(int** tab, int** tabHidden, int* column, int* line, Spell* explosion, Spell* revive, Spell* destroyLine);
void initSpell(Spell* explosion, Spell* revive, Spell* destroyLine);
void shop(int* coin, Spell* explosion, Spell* revive, Spell* destroyLine);
void spellDestroyLine(int** tab, int** tabHidden, int* columnChoose, int* line);
void spellRevive(int** tab, int** tabHidden, int* columnChoose, int* lineChoose);


int main()
{
    int choix = 1;
    int coin = 100;
    Spell explosion;
    Spell revive;
    Spell destroyLine;
    initSpell(&explosion, &revive, &destroyLine);


    do {
        int dimension;
        int column = 0;
        int line = 0;

        do {
            printf("quelle dimension pour le tableau (5 minimum) ? :");
            scanf_s("%d", &dimension);
            column = dimension;
            line = dimension;
            if (column < 5 || line < 5)
            {
                printf("colonne ou ligne trop petit!\n");
            }
        } while (column < 5 || line < 5);


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

        printf("coin : %d\n", coin);
        initBombAround(tab, tabHidden, &column, &line, &explosion, &revive, &destroyLine);
        printf("\n\n\n");

        int columnChoose, lineChoose;
        do {
            inputPlayer(&columnChoose, &lineChoose);
        } while (columnChoose > column || lineChoose > line);
        columnChoose--;
        lineChoose--;
        printf("spell explosion : activee");
        spellExplosion(tab, tabHidden, &columnChoose, &lineChoose, &column, &line);         // premiere explosion
        createBomb(tab, tabHidden, &column, &line);
        int  iTemp, jTemp;

        initBombAround(tab, tabHidden, &column, &line, &explosion, &revive, &destroyLine);

        int isFinish = 0;

        while (isFinish == 0)                        // début du jeu
        {
            int cellOrFlag;
            printf("1 : decouvrir une case\n2 : poser un drapeau\nchoix : ");
            scanf_s("%d", &cellOrFlag);
            if (cellOrFlag == 2)
            {
                do {
                    inputPlayer(&columnChoose, &lineChoose);
                } while (columnChoose > column || lineChoose > line);
                columnChoose--;
                lineChoose--;
                tabHidden[columnChoose][lineChoose] = 2; 
                initBombAround(tab, tabHidden, &column, &line, &explosion, &revive, &destroyLine);
            }
            else {
                do {
                    inputPlayer(&columnChoose, &lineChoose);
                } while (columnChoose > column || lineChoose > line);
                columnChoose--;
                lineChoose--;

                if (columnChoose == -1 && lineChoose == -1)
                {
                    int playerChoise = 0;
                    printf("\n1 : %s : %d\n2 : %s : %d\n\n(si tu veux quitter tape 0)\n\nquelle spell tu veux utiliser ? ", explosion.name, explosion.own, destroyLine.name, destroyLine.own);
                    scanf_s("%d", &playerChoise);
                    switch (playerChoise) {
                    case 1:
                        if (explosion.own == 0)
                        {
                            printf("\n\nvous n'avez pas cette spell !");
                        }
                        else {
                            initBombAround(tab, tabHidden, &column, &line, &explosion, &revive, &destroyLine);
                            explosion.own--; do {
                                printf("\n\nspell explosion : activee\n\n");
                                inputPlayer(&columnChoose, &lineChoose);
                            } while (columnChoose > column || lineChoose > line);
                            columnChoose--;
                            lineChoose--;

                            spellExplosion(tab, tabHidden, &columnChoose, &lineChoose, &column, &line);
                            initBombAround(tab, tabHidden, &column, &line, &explosion, &revive, &destroyLine);
                        }
                        break;
                    case 2:
                        if (destroyLine.own == 0)
                        {
                            printf("\n\nvous n'avez pas cette spell !");
                        }
                        else {

                            destroyLine.own--;
                            initBombAround(tab, tabHidden, &column, &line, &explosion, &revive, &destroyLine);
                            printf("\n\nspell destroyLine  : activée\n\nligne ?");
                            scanf_s("%d", &columnChoose);
                            columnChoose--;
                            spellDestroyLine(tab, tabHidden, &columnChoose, &line);
                            initBombAround(tab, tabHidden, &column, &line, &explosion, &revive, &destroyLine);
                        }
                        break;

                    }
                }

                else if (tabHidden[columnChoose][lineChoose] == 1) {

                    printf("case deja choisi!\n\n");
                    continue;
                }
                else if (tabHidden[columnChoose][lineChoose] == 0 || tabHidden[columnChoose][lineChoose] == 2) {
                    tabHidden[columnChoose][lineChoose] = 1;
                }
                if (columnChoose != -1 && lineChoose != -1 && tab[columnChoose][lineChoose] == 1) // bombe choisi = défaite + affichage de tout le tableau
                {
                    char* wantRevive = 0;
                    if (revive.own > 0)
                    {
                        printf("il y a une bombe ici !!!\n\nveux tu revivre ?\t\t(vous avez %d revive)\n(1:OUI ; 2:NON)\n", revive.own);
                        scanf_s("%d", &wantRevive);
                        if (wantRevive == 1)
                        {
                            revive.own--;
                            spellRevive(tab, tabHidden, &columnChoose, &lineChoose);
                        } 
                        else if (revive.own == 0 || wantRevive == 2) {
                            printf("\n");
                            for (i = 0; i <= column; i++)
                            {
                                for (j = 0; j <= line; j++)
                                {
                                    iTemp = i - 1;
                                    jTemp = j - 1;
                                    if (i == 0) {
                                        printf("  %d  ", j);
                                    }
                                    else if (j == 0) {
                                        printf("  %d  ", i);
                                    }
                                    else if (tab[iTemp][jTemp] < 1) {
                                        printf("  -  ");
                                    }
                                    else {
                                        printf("  B  ");
                                    }
                                }
                                printf("\n");
                            }
                            printf("Vous avez perdu!\n");
                            printf("\n\n\n");

                            printf("Voulez vous recommencer ? ( 1=OUI, 2=NON, 3=SHOP): ");
                            scanf_s("%d", &choix);
                            if (choix == 3) {
                                shop(&coin, &explosion, &revive, &destroyLine);
                                initBombAround(tab, tabHidden, &column, &line, &explosion, &revive, &destroyLine);
                                choix = 1;
                            }
                            if (choix == 1) {
                                for (i = 0; i < column; i++)
                                {
                                    free(tab[i]);
                                    free(tabHidden[i]);
                                }
                                free(tab);
                                free(tabHidden);
                                isFinish = 1;
                            }

                            break;
                        }
                        else {
                            printf("erreur");
                        }
                    }

                }
                else if (columnChoose != -1 && lineChoose != -1 && tab[columnChoose][lineChoose] <= 1) { // case acceptable
                    initBombAround(tab, tabHidden, &column, &line, &explosion, &revive, &destroyLine);
                }
                isFinish = 1;
                for (i = 0; i < column; i++)
                {
                    for (j = 0; j < line; j++)
                    {
                        iTemp = i - 1;
                        jTemp = j - 1;
                        if (j != 0 && i != 0 && tabHidden[iTemp][jTemp] == 0 && tab[iTemp][jTemp] == 0) {
                            isFinish = 0;
                        }
                    }
                }
                if (isFinish == 1) {
                    printf("vous avez gagné! Trop fort wesh !!");
                    srand(time(NULL));
                    coin = coin + dimension + rand() % dimension;
                    printf("coin = %d ", coin);
                    printf("\n\n\n");
                    free(tab);
                    printf("Voulez vous recommencer ? ( 1 = OUI, 2 = NON, 3 = SHOP ): ");
                    scanf_s("%d", &choix);
                    if (choix == 3) {
                        shop(&coin, &explosion, &revive, &destroyLine);
                        choix = 1;
                    }
                    break;
                }
            }
        }
    } while (choix == 1);
    printf("\n*************** Fin du jeu ****************\n");
    return 0;
}






void inputPlayer(int* column, int* line)
{
    printf("ligne : ");
    scanf_s("%d", column);
    printf("colonne : ");
    scanf_s("%d", line);
    printf("\n\n");

}





void createBomb(int** tab, int** tabHidden, int* column, int* line)
{
    srand(time(NULL));
    int i, j, squareCount, n;
    n = 0;
    squareCount = (*column) * (*line);
    if (squareCount <= 30) {
        while (n < 7) {
            i = rand() % (*column);
            j = rand() % (*line);
            if (tab[i][j] != 1 || tabHidden[i][j] == 1) {
                tab[i][j] = 1;
                n++;
            }
        }
    }
    else if (squareCount <= 50) {
        while (n < 11) {
            i = rand() % (*column);
            j = rand() % (*line);
            tab[i][j] = 1;
            n++;
        }
    }
    else if (squareCount <= 75) {
        while (n < 20) {
            i = rand() % (*column);
            j = rand() % (*line);
            if (tab[i][j] != 1 || tabHidden[i][j] == 1) {
                tab[i][j] = 1;
                n++;
            }
        }
    }
    else if (squareCount <= 100) {
        while (n < 26) {
            i = rand() % (*column);
            j = rand() % (*line);
            if (tab[i][j] != 1 || tabHidden[i][j] == 1) {
                tab[i][j] = 1;
                n++;
            }
        }
    }
    else {
        while (n < 32) {
            i = rand() % (*column);
            j = rand() % (*line);
            if (tab[i][j] != 1 || tabHidden[i][j] == 1) {
                tab[i][j] = 1;
                n++;
            }
        }
    }
}






void spellExplosion(int** tab, int** tabHidden, int* lineChoose, int* columnChoose, int* column, int* line)
{
    printf("\n\nspell explosion : activée\n\n");
    tabHidden[*columnChoose][*lineChoose] = 1;
    tab[*columnChoose][*lineChoose] = 0;

    srand(time(NULL));
    int i, j, squareCount, n;
    squareCount = (*column) * (*line);
    if (squareCount <= 36) {
        n = 0;
        while (n < 5) {
            i = rand() % 3 - 1;
            j = rand() % 3 - 1;
            if (*columnChoose + i >= 0 && *lineChoose + j >= 0 && tab[*columnChoose + i][*lineChoose + j] != 1 || *columnChoose + i >= 0 && *lineChoose + j >= 0 && tabHidden[*columnChoose + i][*lineChoose + j] == 1) {
                tab[*columnChoose + i][*lineChoose + j] = 0;
                tabHidden[*columnChoose + i][*lineChoose + j] = 1;
                n++;
            }
        }
    }
    else if (squareCount <= 75) {
        n = 0;
        while (n < 5) {
            i = rand() % 3 - 1;
            j = rand() % 3 - 1;
            if (*columnChoose + i >= 0 && *lineChoose + j >= 0 && tab[*columnChoose + i][*lineChoose + j] != 1 || *columnChoose + i >= 0 && *lineChoose + j >= 0 && tabHidden[*columnChoose + i][*lineChoose + j] == 1) {
                tab[*columnChoose + i][*lineChoose + j] = 0;
                tabHidden[*columnChoose + i][*lineChoose + j] = 1;
                n++;
            }
        }
        n = 0;
        n = 0;
        while (n < 5) {
            i = rand() % 5 - 2;
            j = rand() % 5 - 2;
            if (*columnChoose + i >= 0 && *lineChoose + j >= 0 && tab[*columnChoose + i][*lineChoose + j] != 1 || *columnChoose + i >= 0 && *lineChoose + j >= 0 && tabHidden[*columnChoose + i][*lineChoose + j] == 1) {
                tab[*columnChoose + i][*lineChoose + j] = 0;
                tabHidden[*columnChoose + i][*lineChoose + j] = 1;
                n++;
            }
        }
    }

}






void initBombAround(int** tab, int** tabHidden, int* column, int* line, Spell* explosion, Spell* revive, Spell* destroyLine)
{
    system("cls");
    for (int i = 0; i <= *column; i++)
    {
        for (int j = 0; j <= *line; j++)
        {
            int iTemp = i - 1;
            int jTemp = j - 1;
            if (i == 0) {
                printf("  %d  ", j);
            }
            else if (j == 0) {
                printf("  %d  ", i);
            }
            else if (tabHidden[iTemp][jTemp] == 1) { //case déjà choisi
                int bombsAround = 0;
                if (iTemp + 1 < *line && tab[iTemp + 1][jTemp] == 1)
                {
                    bombsAround++;
                }
                if (jTemp + 1 < *column && tab[iTemp][jTemp + 1] == 1)
                {
                    bombsAround++;
                }
                if (iTemp - 1 >= 0 && tab[iTemp - 1][jTemp] == 1)
                {
                    bombsAround++;
                }
                if (jTemp - 1 >= 0 && tab[iTemp][jTemp - 1] == 1)
                {
                    bombsAround++;
                }
                if (iTemp + 1 < *line && jTemp + 1 < *column && tab[iTemp + 1][jTemp + 1] == 1)
                {
                    bombsAround++;
                }
                if (iTemp - 1 >= 0 && jTemp - 1 >= 0 && tab[iTemp - 1][jTemp - 1] == 1)
                {
                    bombsAround++;
                }
                if (iTemp + 1 < *line && jTemp - 1 >= 0 && tab[iTemp + 1][jTemp - 1] == 1)
                {
                    bombsAround++;
                }
                if (jTemp + 1 < *column && iTemp - 1 >= 0 && tab[iTemp - 1][jTemp + 1] == 1)
                {
                    bombsAround++;
                }
                if (bombsAround > 0) {
                    printf("[ %d ]", bombsAround);
                }
                else if (tabHidden[iTemp][jTemp] == 1 && bombsAround == 0) {
                    printf("[ - ]");
                }
                
            }
            else if (tabHidden[iTemp][jTemp] == 0) { //case cachée
                printf("[   ]");
            }
            else if(tabHidden[iTemp][jTemp] == 2) {
                printf("[ |>]");
            }
 
            
        }
        printf("\n");
    }


    printf("\n\nfaire 0 / 0 pour utiliser une spell\n");
}


void initSpell(Spell* explosion, Spell* revive, Spell* destroyLine)
{
    (*explosion).name = "explosion";
    (*explosion).price = 20;
    (*explosion).own = 1;

    (*revive).name = "revive";
    (*revive).price = 30;
    (*revive).own = 1;

    (*destroyLine).name = "destroyLine";
    (*destroyLine).price = 20;
    (*destroyLine).own = 1;
}


void shop(int* coin, Spell* explosion, Spell* revive, Spell* destroyLine)
{

    system("cls");
    int inputPlayer = 9;
    while (inputPlayer != 0)
    {
        printf("\n\n********SHOP********\n\nvous avez %d coins!\n\n 1 : %s : %d coins\t\tvous en avez %d\n 2 : %s : %d coins\t\tvous en avez %d\n 3 : %s : %d coins\t\tvous en avez %d\n\n\n(si vous voulez quitter la boutique, tapez 0)\n\n", *coin, (*explosion).name, (*explosion).price, (*explosion).own, (*revive).name, (*revive).price, (*revive).own, (*destroyLine).name, (*destroyLine).price, (*destroyLine).own);
        printf("voulez acheter quelque chose ?");
        scanf_s("%d", &inputPlayer);
        switch (inputPlayer) {
        case 1:
            if (*coin < (*explosion).price)
            {
                system("cls");
                printf("vous n'avez pas assez de coins !");
            }
            else {

                *coin -= (*explosion).price;
                (*explosion).own++;
                system("cls");
            }
            break;
        case 2:
            if (*coin < (*explosion).price)
            {

                system("cls");
                printf("vous n'avez pas assez de coins !");
            }
            else {
                *coin -= (*revive).price;
                (*revive).own++;
                system("cls");
            }
            break;
        case 3:
            if (*coin < (*explosion).price)
            {

                system("cls");
                printf("vous n'avez pas assez de coins !");
            }
            else {
                *coin -= (*destroyLine).price;
                (*destroyLine).own++;
                system("cls");
            }
            break;

        }
    }
}

void spellDestroyLine(int** tab, int** tabHidden, int* columnChoose, int* line)
{
    for (int i = 0; i < *line; i++) {
        tab[*columnChoose][i] = 0;
        tabHidden[*columnChoose][i] = 1;
    }
}


void spellRevive(int** tab, int** tabHidden, int* columnChoose, int* lineChoose)
{
    tab[*columnChoose][*lineChoose] = 0;
    tabHidden[*columnChoose][*lineChoose] = 1;
    printf("bon courage pour votre seconde chance !\n\n");
}
