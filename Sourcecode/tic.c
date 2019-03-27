# include <stdio.h>
//for ease of storing moves in csv
struct move
{
    char player;
    int row;
    int col;
};


int main(void);
void start();
void drawBoard();
void twoPlayers();
struct move getMove();
int checkEmpty(int i, int j);
void setMove(struct move moveToSet);
void saveMove(struct move currentMove, int round);
int checkWin();
void endGame();
void saveGameToCSV();


//easier to evaluate valid & win positions tho it may be slower than just a 1d char array from 1-9
//i'm choosing to use this because i don't Like picking a move by a numbered grid rather than a row and column
//global because i'd be passing it to so many lil functions i think it's worth it
char board[3][3];
char currentPlayer;
struct move moves[9]; //max amount of moves in game
char outputPath[40] = "D:\\Uni\\ADS\\Scripts\\Output\\";

int main(void)
{
    printf("Welcome to Tic Tac Toe!\n");

    printf("What would you like to do?\n");

    printf("1: Start a New Game\n");
    printf("2: Replay a previous game\n");

    int menuSelect;
    scanf("%d", &menuSelect);

    while(menuSelect != 1 && menuSelect != 2)
    {
        printf("Invalid menu option %d entered, please pick 1 or 2\n", menuSelect);

        scanf("%d", &menuSelect);
    }

    start();

    return 0;
}

void start()
{
    //clears board
    memset(board, 0, sizeof(board[0][0]) * 3 * 3);
    //clears player
    currentPlayer = ' ';
    printf("Are you playing with one or two players? (1/2)\n");

    int players;
    scanf("%d", &players);
    printf("\n");

    while(players != 1 && players != 2)
    {
        printf("Invalid option %d entered, please pick 1 or 2\n", players);

        scanf("%d", &players);
    }

    if (players == 1)
    {
        printf("One player selected \n");

        //1player
    }
    else
    {
        printf("Two players selected \n");
        twoPlayers();
    }
}


void drawBoard()
{
    printf("\n------\n");

    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            printf("%c ", board[i][j]  );
        }
        putchar('\n');
    }
    printf("------\n\n");
}

void twoPlayers()
{
    //didn't use a bool since char takes up the same amt of space in practice and i'd have to have an if operation to put the char in anyway
    //also c doesn't have bOOLS????

    while(currentPlayer != 'X' && currentPlayer != 'O')
    {
        printf("Who is playing first? (X/O)\n");
        scanf(" %c", &currentPlayer);
    }

    for (int round = 0; round < 9; ++round)
    {
        if (checkWin() == 1)
        {
            break;
        }

        struct move currentMove;

        currentMove = getMove();

        setMove(currentMove);

        saveMove(currentMove, round);

        //switches players after turn ends
        if (currentPlayer == 'X')
        {
            currentPlayer = 'O';

        }
        else
        {
            currentPlayer = 'X';
        }
        drawBoard();

    }

    endGame(currentPlayer);
}

struct move getMove()
{
    int i;
    int j;

    printf("%c's turn\n", currentPlayer);

    printf("Enter the row and column number you want to occupy separated by a space or enter: \n");
    scanf("%d %d", &i, &j);
    //check if in range
    while (checkEmpty(i - 1, j - 1) == 0)
    {
        printf("That space is already occupied! Please pick a different one. \n");
        printf("Enter the row and column number you want to occupy separated by a space or enter: \n");
        scanf("%d %d", &i, &j);
    }

    while(i < 1 || i > 3 || j < 1 || j > 3)
    {
        printf("Row or column value out of range, please pick values from 1 to 3 \n");
        printf("Enter the row and column number you want to occupy separated by a space or enter: \n");
        scanf("%d %d", &i, &j);
    }

    struct move m;

    m.row = i - 1;
    m.col = j - 1;
    m.player = currentPlayer;

    return m;
}

//checks if space is free
int checkEmpty(int i, int j)
{
    if (board[i][j] == 0)
    {
        return 1;
    }
    return 0;
}

void setMove(struct move moveToSet)
{
    board[moveToSet.row][moveToSet.col] = currentPlayer;
}

void saveMove(struct move currentMove, int round)
{
    moves[round] = currentMove;
}

int checkWin()
{
    //rows
    for (int i = 0; i < 3; i++)
    {
        if (board[i][1] == board[i][0] && board[i][1] == board[i][2] && board[i][1] != 0)
        {
            return 1;
        }
    }

    //columns
    for (int j = 0; j < 3; j++)
    {
        if (board[1][j] == board[0][j]
                && board[1][j] == board[2][j] && board[1][j] != 0)
        {
            return 1;
        }
    }

    //i could be implementing some check on whether there are any moves left but
    //diagonals
    for (int i = 0; i < 3; ++i)
    {
        //some for loop nonsense cld have been done here but it's too much hassle
        if (board[1][1] != 0 &&
                (
                    (board[1][1] == board[0][0] && board[1][1] == board[2][2]) ||
                    (board[1][1] == board[2][0] && board[1][1] == board[0][2])) )
        {
            return 1;
        }
    }

    return 0;
}

void endGame()
{
    printf("%c Wins!\n", currentPlayer);

    char save;
    printf("Would you like to save your game? (y/n)\n");
    scanf(" %c", &save);

    if (save == 'y')
    {
        saveGameToCSV();
    }

    char playAgain;

    while(playAgain != 'y' && playAgain != 'n')
    {
        printf("Play again? (y/n)\n");
        scanf(" %c", &playAgain);

        if (playAgain == 'y')
        {
            start();
        }
    }

    printf("Thanks for playing!\n");
    return;
}

void saveGameToCSV()
{
    FILE *game;
    //we're never gonna be saving anywhere Close to a thousand games so
    char gameName[7] = "game";

    char path[40];
    strncpy( path, outputPath, 40);

    for (int i = 0; i < 999; ++i)
    {
        char gameNameBuf[7];
        char pathBuf[40];

        strncpy(gameNameBuf, gameName, 7) ;
        strncpy(pathBuf, path, 40);

        char char_i[3];
        sprintf(char_i, "%d", i);
        printf("it: %s\n", char_i);

        strcat(gameNameBuf, char_i);

        strcat(pathBuf, gameNameBuf);
        strcat(pathBuf, ".csv");

        //if save with game name already exists
        if (fopen(pathBuf, "r"))
        {
            continue;
        }
        else
        {
            strncpy(gameName, gameNameBuf, 7) ;
            strcat(path, gameName);
            break;
        }
    }

    printf( "Saving game as: %s\n", gameName);

    strcat(path, ".csv");
    printf("path is:%s\n", path);

    game = fopen(path, "w");

    fprintf(game, "Player,Row,Column\n");

    for (int i = 0; i < 9; ++i)
    {
        if (moves[i].player == 0)
        {
            break;
        }
        fprintf(game, "%c,%d,%d\n", moves[i].player, moves[i].row, moves[i].col);
    }

    fclose(game);
}

void replay()
{
    FILE *game;

    char path[40];
    strncpy(path, outputPath, 40);

    printf( "Enter the filename of the game you'd like to replay:\n");
    char gameName[7];
    scanf(" %s", &gameName)

    strcat(path, gameName);

    while (!fopen(path, "r"))
    {
        printf( "Game file with path %s not found, try again. \n", path);
        scanf(" %s", &gameName)

        strncpy(path, outputPath, 40);
        strcat(path, gameName);
    }

    game=fopen(path, "r");
    printf( "Loaded game %s\n", gameName);

    printf("Would you like to autoplay or go through moves on enter?\n");

    printf("1: Autoplay\n");
    printf("2: On Enter\n");

    int menuSelect;
    scanf("%d", &menuSelect);

    while(menuSelect != 1 && menuSelect != 2)
    {
        printf("Invalid menu option %d entered, please pick 1 or 2\n", menuSelect);

        scanf("%d", &menuSelect);
    }

    for (int i = 0; i < 9; ++i)
    {
        /* code */
    }

    if (menuSelect == 1)
    {

    }
    else{

    }

}

void autoplay(FILE &game){

}