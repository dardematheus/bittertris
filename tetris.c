#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#define X_AXIS 4
#define Y_AXIS 6

typedef enum Tetromino{
    I, O, T, S, Z, J, L, COUNT
}Tetromino;

//uint16_t matrice used to hold the bitmasks
//TYPE is Y AXIS, ROTATION is X AXIS
static const uint16_t type[COUNT][4] = {
    //Square O
    { 0xCC00, 0xCC00, 0xCC00, 0xCC00 },
    //T invertido sla
    { 0x4E00, 0x8C80, 0xE400, 0x4C40 },
    //I
    { 0xF000, 0x8888, 0xF000, 0x8888 },
    //L
    { 0x8E00, 0xC880, 0xE200, 0x44C0 },
    //Outro L
    { 0x2E00, 0x88C0, 0xE800, 0xC440 },
    //Z
    { 0x6C00, 0x8C40, 0x6C00, 0x8C40 },
    //Outro Z
    { 0xC600, 0x4C80, 0xC600, 0x4C80 },
}; 

//RUNNING: 0xFF || LOCK: 0xF0 || LOST: 0x00
//RUNNING: JOGADOR CONTROLANDO PECA
//LOCKED: PECA TRANCADA, PRECISA GERAR UMA NOVA
typedef struct Game {
    unsigned int points;
    uint8_t state;
    double gravity;
}Game;

typedef struct Piece {
    unsigned int coordX, coordY;
    uint16_t type;
    uint16_t rot;
}Piece;

Game initGame(void);
void drawBoard(Game game);

static uint16_t board[X_AXIS][Y_AXIS]; // 0 eh vazio, 1 nao

int main(void)
{
    char key;
    Game game = initGame();

    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    
    while(key != 'q'){
        drawBoard(game);
        refresh();
        key = getch();
        if(game.state == 0xF0){
            game.state = 0xFF;
        }
    }

    endwin();
    return 0;
}


//reescrever tambem
Game initGame(void)
{  
    int x, y;
    for(y = 0; y < Y_AXIS; ++y){
        for(x = 0; x < X_AXIS; ++x){
            board[x][y] = board[x][y] | 0x0000;
        }
    }

    Game game;
    game.points = 0;
    game.state = 0xFF;
    game.gravity = 0;
    return game;
}

//abc
void drawBoard(Game game)
{
    if(game.state == 0x00){
        mvprintw(0,0, "You lost, press R to try again");
        return;
    }

    int x, y, xcount;

    for(int i = 0; i < Y_AXIS; i++){
        for(int j = 0; j < X_AXIS; j++){
            x = 4 * j;
            y = 4 * i;
            for(int bit = 15; bit >= 0; bit--){
                if(xcount != 0 && ((xcount % 4) == 0)){
                    y++;
                    x = 4 * j;
                }
                if(board[j][i] & (1 << bit)) mvaddch(y, x, '1');
                else mvaddch(y, x, '0');
                xcount++;
                x++;
            }
        }
    }
    mvprintw(1, X_AXIS + 15, "Points: %d", game.points);
}
