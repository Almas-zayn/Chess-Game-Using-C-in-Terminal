#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chess.h"
#define VALID_MOV 1
#define INVALID_MOV 0
#define PLY_1 1
#define PLY_2 2
#define BISHOP  if( ( (a+i) == x) && ( (b+j) == y) ) \
                          return VALID_MOV; \
                         if(strcmp(board[a+i][b+j]," ")) \
                          return INVALID_MOV;
#define ROOK  if( ( (a+i) == x) && ( (b+j) == y) ) \
                          return VALID_MOV; \
                         if(strcmp(board[a+i][b+j]," ")) \
                          return INVALID_MOV;

void enableRawMode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
u16 cur_ply=1;//cur_ply==1->player 1(white) ...... cur_ply==2->player 2(black)
u16 a=9,b=9,n_of_moves=0,flag=0;
u16 x = 4, y = 4,temp;
s8 sel_p[30],opp_p[30];
s8 ply_1[12]=" PLAYER - 1 ";
s8 ply_2[12]=" PLAYER - 2 ";
void moveCursor(char board[8][8][30]) {
    char c;
    while (1) {
        c = getchar();
        if (c == '\033') {
            getchar();
            switch(getchar()) {
                case 'A': if (x > 0) (x)--; break;
                case 'B': if (x < 7) (x)++; break;
                case 'C': if (y < 7) (y)++; break;
                case 'D': if (y > 0) (y)--; break;
            }
            break;
        }
        if( (c=='\n') && (flag==0) ){
                memset(sel_p,'\0',sizeof(sel_p));
                strcpy(sel_p,board[x][y]);
                temp = current_Piece(sel_p,cur_ply);
                if( ((temp > 6)?2:( temp == 0)?0:1) != cur_ply)
                        break;
                a=x; b=y;
                flag=1;
             break;
        }
        if(c == 'u' || c == 'U' && flag == 1){
             flag=0;
             a=9,b=9;
             break;
        }
        if( (c=='\n') && (flag==1) )
        {
               memset(opp_p,'\0',sizeof(opp_p));
               strcpy(opp_p,board[x][y]);
             if( mov_piece() == INVALID_MOV ){
                break;
             }
               temp = current_Piece(opp_p,cur_ply);
               temp = ((temp>=1)&&(temp<=6))?1:((temp>=7)&&(temp<=12)?2:0);
             if( temp == cur_ply )
                break;
             cur_ply=((++n_of_moves)%2)+1;
             strcpy(board[a][b]," ");
             strcpy(board[x][y],sel_p);
             flag=0;
             a=9,b=9;
           break;
        }
    }
}

void displayBoard(char board[8][8][30]) {
    system("clear");
    printf("\n\n\t");
    for(int p=0;p<12;p++)
       (cur_ply==PLY_1)?printf("\033[42m %c\033[0m",ply_1[p]):printf(" %c",ply_1[p]);
    printf("\n\n\t");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (i == x && j == y)
                printf("\033[42m %s \033[0m",board[i][j]);//green for cursor
            else if (i == a && j == b )
                printf("\033[41m %s \033[0m",board[i][j]);//red Bg for selected piece
            else if ((i + j) % 2 == 0)
                printf("\033[47m %s \033[0m",board[i][j]);//white Bg
            else
                printf("\033[100m %s \033[0m",board[i][j]);//grey Bg
            }
       printf("\033[0m\n\t");
    }
    printf("\n\t");
    for(int p=0;p<12;p++)
       (cur_ply==PLY_2)?printf("\033[42m %c\033[0m",ply_2[p]):printf(" %c",ply_2[p]);
    printf("\n\n");
}

u16 current_Piece(s8 *sel_p,u16 cur_ply){
  if(cur_ply == PLY_1 ){
    if(strcmp(sel_p,white.pawn)==0)
            return 6;
    else if(strcmp(sel_p,white.rook)==0)
            return 5;
    else if(strcmp(sel_p,white.knight)==0)
            return 4;
    else if(strcmp(sel_p,white.bishop)==0)
            return 3;
    else if(strcmp(sel_p,white.queen)==0)
            return 2;
    else if(strcmp(sel_p,white.king)==0)
            return 1;
    else
            return 0;
   }
  else if(cur_ply == PLY_2 ){
    if(strcmp(sel_p,black.pawn)==0)
            return 12;
    else if(strcmp(sel_p,black.rook)==0)
            return 11;
    else if(strcmp(sel_p,black.knight)==0)
            return 10;
    else if(strcmp(sel_p,black.bishop)==0)
            return 9;
    else if(strcmp(sel_p,black.queen)==0)
            return 8;
    else if(strcmp(sel_p,black.king)==0)
            return 7;
    else
            return 0;
   }
  return 0;
}
u16 mov_piece(){
        temp = current_Piece(sel_p,cur_ply);
        switch((temp>6)? temp - 6 : temp){
                case 1:  if((mov_king(black,white))==1)
                              return VALID_MOV;  break;
                case 2:  if((mov_queen(black,white))==1)
                              return VALID_MOV;  break;
                case 3:  if((mov_bishop(black,white))==1)
                              return VALID_MOV;  break;
                case 4:  if((mov_knight(black,white))==1)
                              return VALID_MOV;  break;
                case 5:  if((mov_rook(black,white))==1)
                              return VALID_MOV;  break;
                case 6:  if((mov_pawn(black,white))==1)
                              return VALID_MOV;  break;
        }
    return INVALID_MOV;
}

u16 mov_pawn(CHESS black,CHESS white){
     if((strcmp( sel_p, white.pawn ) == 0 ) && ( cur_ply == PLY_1 )){
        if((a == 1) && (x <= 3) && (y == b))
                return VALID_MOV;
        if(( a > 1 ) && (( a+1 ) == x ) && (y == b) && (strcmp(board[x][y]," ") == 0))
                return VALID_MOV;
        if(a+1==x && ((b+1 == y) || (b-1 == y)) && !( strcmp(board[x][y]," ") == 0) )
                return VALID_MOV;
     }
     else if( (strcmp(sel_p,black.pawn) == 0) && (cur_ply == PLY_2 ) ){
        if (a == 6 && y == b && (x == 4 || x == 5))
                return VALID_MOV;
        if( (a<6) && (x == (a-1)) && (y == b) && (strcmp(board[x][y]," ") == 0))
                return VALID_MOV;
        if(a-1==x && (b-1 == y || b+1 == y) && strcmp(board[x][y]," ") != 0 )
                return VALID_MOV;
     }
   return INVALID_MOV;
}
u16 mov_knight(CHESS black,CHESS white){

        if(( (b+1) == y) && (( (a+2) == x) || ( (a-2) == x)))
                return VALID_MOV;
        if(( (b-1) == y) && (( (a+2) == x) || ( (a-2) == x )))
                return VALID_MOV;
        if(( (a+1) == x) && (( (b+2) == y) || ( (b-2) == y)))
                return VALID_MOV;
        if(( (a-1) == x) && (( (b+2) == y) || ( (b-2) == y)))
                return VALID_MOV;

     return INVALID_MOV;
}

u16 mov_rook(CHESS black,CHESS white){
     short int i,j;
       if((a==x) && (b<y)){
         for(i=0,j=1;i<=y;j++){
             ROOK;
         }
       }
       if((a==x) && (b>y)){
         for(i=0,j=-1;i<=y;j--){
            ROOK;
         }
       }
       if((a>x) && (b==y)){
         for(i=-1,j=0;i<=x;i--){
            ROOK;
         }
       }
       if((a<x) && (b==y)){
         for(i=1,j=0;i<=x;i++){
            ROOK
         }
       }
       return INVALID_MOV;
}
u16 mov_king(CHESS black,CHESS white){
    if((x>(a+1)) || (y>(b+1)) || (x<(a-1)) || (y<(b-1)) )
            return INVALID_MOV;
    return VALID_MOV;
}
u16 mov_queen(CHESS black,CHESS white){
    if(mov_bishop(black,white) == VALID_MOV )
            return VALID_MOV;
    if(mov_rook(black,white) == VALID_MOV )
            return VALID_MOV;
    return INVALID_MOV;
}
u16 mov_bishop(CHESS black,CHESS white){
     short int i,j;
       if((a<x) && (b<y)){
         for(i=1,j=1;i<=x;i++,j++){
            BISHOP;
         }
       }
       if((a<x) && (b>y)){
         for(i=1,j=-1;i<=x;i++,j--){
            BISHOP;
         }
       }
       if((a>x) && (b<y)){
         for(i=-1,j=1;i<=x;i--,j++){
            BISHOP;
         }
       }
       if((a>x) && (b>y)){
         for(i=-1,j=-1;i<=x;i--,j--){
            BISHOP;
         }
       }
   return INVALID_MOV;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   ~                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        ~                  