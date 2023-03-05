#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void inputPlayer(int* column, int* line);
void verifyBombNumber(int* inputPlayer, int* dimension);
void verifyDimension(int* inputPlayer);
void initTab(int** tab, int* dimension);
void createBomb(int** tab, int** tabHidden, int* dimension, int* bomb);
void firstBreak(int** tab, int** tabHidden, int* columnChoose, int* lineChoose);
void printfTableau(int** tab, int** tabHidden, int* column, int* line);
void lose(int** tab, int* column, int* line);
void inputRemap(int** tab, int** tabHidden, int* column, int* choix, int* isFinish);
void verifyInput(int* inputPlayer);
void countBombAround(int** tab, int* column, int* line, int* dimension, int* count);

int main()
{
    int choix = 1;
    do {
        int dimension, column, line, bomb;

        printf("quelle dimension pour le tableau (5 minimum) ? :");
        verifyDimension(&dimension);
        printf("combien de bombe ? (%d max)", (dimension * dimension) / 3);
        verifyBombNumber(&bomb, &dimension);
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

        printfTableau(tab, tabHidden, &column, &line);
        printf("\n\n\n");

        int columnChoose, lineChoose;
        do {
            inputPlayer(&columnChoose, &lineChoose);
        } while (columnChoose > column || lineChoose > line);

        columnChoose--;
        lineChoose--;

        firstBreak(tab, tabHidden, &columnChoose, &lineChoose);         // premiere explosion
        createBomb(tab, tabHidden, &dimension, &bomb);
        int  iTemp, jTemp;

        printfTableau(tab, tabHidden, &column, &line);

        int isFinish = 0;

        while (isFinish == 0)                        // début du jeu
        {
            do {
                inputPlayer(&columnChoose, &lineChoose);
            } while (columnChoose > column || lineChoose > line);
            columnChoose--;
            lineChoose--;

            if (tabHidden[columnChoose][lineChoose] == 1) {

                printf("case deja choisi!\n\n");
                continue;
            }
            else if (tabHidden[columnChoose][lineChoose] == 0 || tabHidden[columnChoose][lineChoose] == 2) {
                tabHidden[columnChoose][lineChoose] = 1;
            }
            if (columnChoose != -1 && lineChoose != -1 && tab[columnChoose][lineChoose] == 9) // bombe choisi = défaite + affichage de tout le tableau
            {
                lose(tab, &column, &line);
                inputRemap(tab, tabHidden, &column, &choix, &isFinish);
                break;
            }
            else if (columnChoose != -1 && lineChoose != -1 && tab[columnChoose][lineChoose] <= 8 && tab[columnChoose][lineChoose] >= 0) { // case acceptable
                printfTableau(tab, tabHidden, &column, &line);
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
                printf("vous avez gagne!!!");
                inputRemap(tab, tabHidden, &column, &choix, &isFinish);
                break;
            }
        }
    } while (choix == 1);
    printf("\n*************** Fin du jeu ****************\n");
    return 0;
}

void inputPlayer(int* column, int* line)
{
    printf("ligne : ");
    verifyInput(column);
    printf("colonne : ");
    verifyInput(line);
    printf("\n\n");
}

void verifyInput(int * inputPlayer)
{
    char character; 
    while (scanf_s("%d", inputPlayer) != 1) {
        
        scanf_s("%c", &character); 
        if (!isdigit(character)) { 
            printf("Entree invalide. veuillez reessayer : ");
            continue;
        }
        ungetc(character, stdin);
    }
}

void verifyBombNumber(int * inputPlayer, int * dimension)
{
    while (scanf_s("%d", inputPlayer) != 1 || *inputPlayer < 1 || *inputPlayer > (*dimension * *dimension) / 3)
    {
        printf("Valeur invalide. Veuillez saisir a nouveau un entier entre 1 et %d : ", (*dimension * *dimension) /3);
        while (getchar() != '\n') {} 
    }
}

void verifyDimension(int* inputPlayer)
{
    while (scanf_s("%d", inputPlayer) != 1 || *inputPlayer < 5)
    {
        printf("Valeur invalide. Veuillez saisir a nouveau un entier au dessus de 5 : ");
        while (getchar() != '\n') {}
    }
}

void createBomb(int** tab, int** tabHidden, int * dimension, int* bomb)
{
    int n = 0, count = 0;
    int* allCase = malloc((*dimension) * (*dimension) * sizeof(int*) - 8);
    for (int i = 0; i < *dimension; i++)
    {
        for (int j = 0; j < *dimension; j++)
        {
            if (tabHidden[i][j] == 0)
            {
                allCase[count] = &tab[i][j];
                count++;
            }
        }
    }
    int* p;
    srand(time(NULL));
    for (int i = 0; i < *bomb; i++)
    {
        int random = rand() % ((*dimension) * (*dimension) - 8 - n);
        p = allCase[random];
        *p = 9;
        for (int j = random; j < (*dimension) * (*dimension) - 8 -n; j++)
        {
            allCase[j] = allCase[j + 1];
        }
        n++;
        //free(allCase);
        //newTab = malloc((*dimension) * (*dimension) * sizeof(int*) - n);
        //allCase = newTab;
    }
    initTab(tab, dimension);
}

void firstBreak(int** tab, int** tabHidden, int* columnChoose, int* lineChoose) {

    int count = 0;
    int n = 0;
    int* caseAroundMoreOne = malloc(9 * sizeof(int*));
    
    for (int i = *columnChoose - 1; i <= *columnChoose + 2; i++)
    {
        for (int j = *lineChoose - 1; j <= *lineChoose + 1; j++)
        {
            caseAroundMoreOne[count] = &tabHidden[i][j];
            count++;
        }
    }
    int* p;
    srand(time(NULL));
    for (int i = 0; i < 8; i++)
    {
        int random = rand() % (8 - n);
        p = caseAroundMoreOne[random];
        *p = 1;
        for (int j = random; j < 8 - n; j++)
        {
            caseAroundMoreOne[j] = caseAroundMoreOne[j + 1];
        }
        n++;
    }
}




void countBombAround(int** tab, int* column, int* line, int* dimension, int* count)
{
    int i, j;
    printf("column :%d\nline :%d\n", *column, *line);
    for (i = *column - 1; i < *column + 1; i++)
    {
        for (j = *line - 1; j < *line + 1; j++)
        {
            if(i >= 0 && j >= 0 &&  tab[i][j] == 9 && (i != *column && j != *line) && i < *dimension && j < *dimension ) {
                (*count)++;
                printf("%d\n", *count);
            }
        }
    }
}

void initTab(int** tab, int* dimension)
{
    int count = 0;
    for (int i = 0; i < *dimension; i++)
    {
        for (int j = 0; j < *dimension; j++)
        {
            countBombAround(tab, &i, &j, dimension, &count);
            printf("\n\ncount : %d\n\n", count);
            if (count < 9)
            {
                tab[i][j] = count;
            }
        }
    }
}





void printfTableau(int** tab, int** tabHidden, int* column, int* line)
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
            else if (tabHidden[iTemp][jTemp] == 1) { //case visible
                if (tab[iTemp][jTemp] == 0 ) {
                    printf("[ - ]");
                }
                else {
                    printf("[ %d ]", tab[iTemp][jTemp]);
                }
            }
            else if (tabHidden[iTemp][jTemp] == 0) { //case cachée
                printf("[   ]");
            }
            else if (tabHidden[iTemp][jTemp] == 2) { // un drapeau ou bien
                printf("[ |>]");
            }


        }
        printf("\n");
    }
}

void lose(int ** tab, int * column, int * line)
{
    printf("\n");
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

}

void inputRemap(int ** tab, int ** tabHidden, int * column, int * choix, int * isFinish)
{
    printf("Voulez vous recommencer ? ( 1=OUI, 2=NON): ");
    scanf_s("%d", choix);
    if (*choix == 1) {
        for (int i = 0; i < *column; i++)
        {
            free(tab[i]);
            free(tabHidden[i]);
        }
        free(tab);
        free(tabHidden);
        *isFinish = 1;
    }
}
