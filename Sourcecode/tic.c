# include <stdio.h>
#include <stdlib.h>
#include <time.h>

//for ease of storing moves in csv
struct move
{
    char player;
    int row;
    int col;
};

int main(void);
void start();

void onePlayer();
struct move autoMove();

void twoPlayers();

struct move getMove();
int xDo(int round);
void undo(int round);
void redo(int round);

void drawBoard();
void switchPlayer();
int checkEmpty(int i, int j);
void setMove(int round);
void saveMove(struct move currentMove, int round);
int checkWin();

void draw();
void win();
void endGame();

void saveGameToCSV();
// move* loadGame();
void loadGame();
void replay();


//easier to evaluate valid & win positions tho it may be slower than just a 1d char array from 1-9
//i'm choosing to use this because i don't Like picking a move by a numbered grid rather than a row and column
//global because i'd be passing it to so many lil functions i think it's worth it
char board[3][3];
char currentPlayer;
//could have just saved all of this to the struct?? i dont know it's 5 am
struct move moves[10]; //max amount of moves in game
struct move undoneMoves[9]; //max amount of moves in game
char outputPath[40] = "D:\\Uni\\ADS\\Scripts\\Output\\";

int main(void)
{
    //seeds random generator with current time
    srand(time(0));

    printf("Welcome to Tic Tac Toe!\n\n");

    printf("What would you like to do?\n\n");

    printf("1: Start a New Game\n");
    printf("2: Replay a previous game\n");

    int menuSelect;
    scanf("%d", &menuSelect);

    while(menuSelect != 1 && menuSelect != 2)
    {
        printf("Invalid menu option %d entered, please pick 1 or 2\n", menuSelect);

        scanf("%d", &menuSelect);
    }

    if (menuSelect == 1)
    {
        start();
    }
    else if(menuSelect == 2)
    {
        // struct move* loadedMoves;

        // loadedMoves = loadGame();
        loadGame();
        // for (int i = 0; i < 9; ++i)
        // {
        //     moves[i] = loadedMoves[i] ;
        // }
        printf("Loaded\n\n");

        replay();
    }

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

    players = 1;
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
        onePlayer();
    }
    else
    {
        printf("Two players selected \n");
        twoPlayers();
    }
    return;
}


void onePlayer()
{
    while(currentPlayer != 'X' && currentPlayer != 'O')
    {
        printf("Which piece would you like to play? (X/O)\n");
        scanf(" %c", &currentPlayer);
    }

    char player = currentPlayer;

    printf("Would you like to enable undo/redos? (y/n)\n");
    char enableUndo;
    scanf(" %c", &enableUndo);


    for (int round = 0; round < 9; ++round)
    {
        drawBoard();

        //only if enabled so we don't get spammed with constant asks to undo
        if (enableUndo == 'y')
        {
            int action = xDo(round);
            if (action == 1)
            {
                printf("%d\n", round );
                //rewinds back to undone round
                round = round - 2;
                printf("%d\n", round );
                continue;
            }
            else if(action == 2)
            {
                continue;
            }
        }

        if (currentPlayer == player)
        {
            saveMove(getMove(), round);
        }
        else
        {
            saveMove(autoMove(), round);
        }

        setMove(round);

        if (checkWin() == 1)
        {
            drawBoard();
            win();
            return;
        }
        switchPlayer();
    }
    draw();
    return;
}

struct move autoMove()
{
    printf("Computer's turn\n");
    struct move randomMove;
    randomMove.player = currentPlayer;

    while(1)
    {
        int upper = 2;
        int lower = 0;


        int row;
        int col;

        row = (rand() % (upper - lower + 1)) + lower;
        col = (rand() % (upper - lower + 1)) + lower;

        // printf("row: %d\n", row );
        // printf("col: %d\n", col );
        if(checkEmpty(row, col) == 1)
        {
            randomMove.row = row;
            randomMove.col = col;

            return randomMove;
        }
        // printf("regenerating move \n");

        // randomMove.row = (rand() % (upper - lower + 1)) + lower;
        // randomMove.col = (rand() % (upper - lower + 1)) + lower;

        // printf("row: %d\n", randomMove.row );
        // printf("col: %d\n", randomMove.col );
        // getchar();

    }
}


void twoPlayers()
{
    while(currentPlayer != 'X' && currentPlayer != 'O')
    {
        printf("Who is playing first? (X/O)\n");
        scanf(" %c", &currentPlayer);
    }

    printf("Would you like to enable undo/redos? (y/n)\n");
    char enableUndo;
    scanf(" %c", &enableUndo);


    for (int round = 0; round < 9; ++round)
    {
        drawBoard();

        //only if enabled so we don't get spammed with constant asks to undo
        if (enableUndo == 'y')
        {
            int action = xDo(round);
            if (action == 1)
            {
                printf("%d\n", round );
                //rewinds back to undone round
                round = round - 2;
                printf("%d\n", round );
                continue;
            }
            else if(action == 2)
            {
                continue;
            }
        }

        if (checkWin() == 1)
        {
            win();
            return;
        }

        saveMove(getMove(), round);

        setMove(round);

        switchPlayer();
    }
    draw();
    return;
}

//undo/redo interface
int xDo(int round)
{
    char action = ' ';
    while(action != 'u' && action != 'r' && action != 'n')
    {
        printf("Would you like to undo the last move or redo an undone move? (u/r/n)\n");
        scanf(" %c", &action);
    }
    if (action == 'u')
    {
        if(round > 0)
        {
            undo(round - 1);

            switchPlayer();
            return 1;
        }
        printf("No moves to undo! Make one first please :)\n");
    }
    else if (action == 'r')
    {
        if(round < 9 && undoneMoves[round].player != 0)
        {
            redo(round);
            switchPlayer();
            return 2;
        }
        printf("No moves available to redo!\n");
    }
    return 0;
}

void undo(int round)
{
    //could add an iterator here to check for the first empty value and store then undoes sequentially But i think a better bet is to actually assign it to the index of the move undone
    undoneMoves[round] = moves[round];

    moves[round].player = NULL;

    setMove(round);

    moves[round].row = NULL;
    moves[round].col = NULL;

    printf("Last move undone.\n");
    return;
}

void redo(int round)
{
    moves[round] = undoneMoves[round];

    setMove(round);

    printf("Last undone move redone.\n");
}


void switchPlayer()
{
    if (currentPlayer == 'X')
    {
        currentPlayer = 'O';
    }
    else
    {
        currentPlayer = 'X';
    }
    return;
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
    //could technically pre-populate the moves with the player as that's decided on picking the first player but this leaves less room for the moves becoming unsynced from player
    m.player = currentPlayer;

    return m;
}

//checks if space is free
int checkEmpty(int i, int j)
{
    if (board[i][j] == 0)
    {
        // printf("space empty \n");
        return 1;
    }
    // printf("space occupied \n");

    return 0;
}

//initially was passing the move itself but it hink it's better to ref it from the move array so it doesn't get disconnected with the undos and redos
void setMove(int round)
{
    board[moves[round].row][moves[round].col] = moves[round].player;
    return;
}

void saveMove(struct move currentMove, int round)
{
    moves[round] = currentMove;
    return;
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


void win()
{
    printf("%c Wins!\n", currentPlayer);
    endGame();
    return;
}

void draw()
{
    printf("It's a draw! Congratulations, you've Both Won.\n");

    endGame();
    return;
}


void endGame()
{
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
        break;
    }
    printf("Thanks for playing!\n");
    return;
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
    return;
}


void saveGameToCSV()
{
    FILE *game = NULL;
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
    return;
}

// move* loadGame()
void loadGame()
{
    FILE *game;

    char path[40];
    strncpy(path, outputPath, 40);

    printf( "Enter the filename of the game you'd like to replay (must end with .csv):\n");
    char gameName[7];
    scanf(" %s", &gameName);

    strcat(path, gameName);

    while (!fopen(path, "r"))
    {
        printf( "Game file with path %s not found, try again. \n", path);
        scanf(" %s", &gameName);

        strncpy(path, outputPath, 40);
        strcat(path, gameName);
    }

    game = fopen(path, "r");
    printf( "Loaded game file %s\n", gameName);

    //left a bit of breating space
    char gameBuffer[100];
    //clda used a for here
    int i = 0;

    // struct move loadedMoves[20]; //max amount of moves in game

    while(fgets(gameBuffer, 100, game))
    {
        //skips first line
        if (i < 1)
        {
            i++;
            continue;
        }
        if (i > 9)
        {
            break;
        }
        printf("reading line %d\n", i);

        char *playerstr;
        playerstr = strtok(gameBuffer, ",");
        moves[i - 1].player = playerstr[0];
        printf("%c\n", moves[i - 1].player);

        moves[i - 1].row = atoi(strtok(NULL, ","));
        printf("%d\n", moves[i - 1].row);

        moves[i - 1].col = atoi(strtok(NULL, ","));
        printf("%d\n", moves[i - 1].col);


        i++;
    }

    //crashes without error after this point, can't for the life of me figure out why
    int j;
    while(moves[j].player != 0)
    {
        j++;
    }
    printf("%d elements in moves\n", j);


    printf("Loaded moves from %s\n", gameName);

    return;
}

void replay()
{
    printf("Would you like to autoplay or go through moves on keypress?\n");

    printf("1: Autoplay\n");
    printf("2: On Enter\n");

    int menuSelect;
    scanf("%d", &menuSelect);

    while(menuSelect != 1 && menuSelect != 2)
    {
        printf("Invalid menu option %d entered, please pick 1 or 2\n", menuSelect);

        scanf("%d", &menuSelect);
    }

    if (menuSelect == 1)
    {
        int i = 0;
        while(moves[i].player != 0)
        {
            setMove(i);
        }
        endGame();
    }
    else if (menuSelect == 2)
    {
        int i = 0;
        while(moves[i].player != 0)
        {
            setMove(i);
            getchar();
        }
        endGame();
    }
    return;
}