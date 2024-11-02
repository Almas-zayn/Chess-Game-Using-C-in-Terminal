typedef struct Chess{
    char king[30];
    char queen[30];
    char rook[30];
    char bishop[30];
    char knight[30];
    char pawn[30];
}CHESS;

typedef char s8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef short int s16;
typedef int s32;

extern CHESS white;
extern CHESS black;
extern char board[8][8][30];

void displayBoard(s8 board[8][8][30]);
void moveCursor(s8 board[8][8][30]);
void enableRawMode();

//mov_pieces
u16 mov_piece();
u16 current_Piece(s8 *sel_p,u16 cur_ply);
u16 mov_pawn(CHESS black,CHESS white);
u16 mov_knight(CHESS black,CHESS white);
u16 mov_bishop(CHESS black,CHESS white);
u16 mov_rook(CHESS black,CHESS white);
u16 mov_queen(CHESS black,CHESS white);
u16 mov_king(CHESS black,CHESS white);