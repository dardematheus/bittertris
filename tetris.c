#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#define X_AXIS 13
#define Y_AXIS 22

typedef enum Tetromino{
    I, O, T, S, Z, J, L, COUNT
}Tetromino;

//RUNNING: 0xFF || LOCK: 0xF0 || LOST: 0x00
typedef struct Game {
    unsigned int points;
    uint8_t state;
    double gravity;
}Game;

typedef struct Piece {
    int coordX, coordY;
    uint8_t rot;
    uint8_t type;
}Piece;

Game initGame(uint8_t board[X_AXIS][Y_AXIS]);
void drawGame(Game game, uint8_t board[X_AXIS][Y_AXIS]);

static uint8_t board[X_AXIS][Y_AXIS]; // 0 eh vazio, >0 eh preenchido com id da peca

int main(void)
{
    char key;
    Game game = initGame(board);

    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    
    while(key != 'q'){
        drawGame(game, board);
        refresh();
        key = getch();
    }

    endwin();
    return 0;
}

Game initGame(uint8_t board[X_AXIS][Y_AXIS])
{
    int x, y;
    
    for(y = 0; y < Y_AXIS; ++y){
        for(x = 0; x < X_AXIS; ++x){
            if(y == 0 || y == Y_AXIS - 1) board[x][y] = 1;
            else if(x == 0 || x == X_AXIS - 1) board[x][y] = 1;
            else board[x][y] = 0; 
        }
    }
    Game game;
    game.points = 0;
    game.state = 0xFF;
    game.gravity = 0;
    return game;
}

void drawGame(Game game, uint8_t board[X_AXIS][Y_AXIS])
{
    if(game.state == 0x00){
        mvprintw(0, 0, "You Lost, press R to try again");
        return;
    }

    for(int y = 0; y < Y_AXIS; ++y){
        for(int x = 0; x < X_AXIS; ++x){
            if(board[x][y] == 1) mvaddch(y, x, '#');
            else if(board[x][y] == 0) mvaddch(y, x, ' ');
        }
    }

    mvprintw(0, X_AXIS + 1, "Points: %d", game.points);
}
