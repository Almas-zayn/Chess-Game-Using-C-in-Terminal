#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include "chess.h"

CHESS black={"\033[38;5;214m\u2654","\033[38;5;214m\u2655","\033[38;5;214m\u2656","\033[38;5;214m\u2657","\033[38;5;214m\u2658","\033[38;5;214m\u2659"};
CHESS white={"\033[34m\u265A","\033[34m\u265B","\033[34m\u265C","\033[34m\u265D","\033[34m\u265E","\033[34m\u265F"};

void menu();
void loading_game();
int  resume_game();
char board[8][8][30];
void InitBoard(char board[8][8][30],CHESS white,CHESS black);
char str[30],option;

int main() {

    InitBoard(board,white,black);
    enableRawMode();
 MENU: system("clear");
    menu();
    option=getchar();
    if(option == '1')
      InitBoard(board,white,black);

    if(option == '2'){
     if( resume_game() == 0 )
             goto MENU;
    }

    else if(option == '3')
      exit(0);
    system("clear");
    printf("\n\n\n\n\n\n\t");

    loading_game();
    while (1){
        displayBoard(board);
        moveCursor(board);
    }
}
void loading_game(){
    char str[11]="Loading...";
    for(int i=0;i<11;i++){
        putchar(str[i]);
        fflush(stdout);
        usleep(500000);
    }
}
void menu(){
     printf("\n\n\n\n\n\n\t");
     printf("1.Start New game\n\t2.Resume\n\t3.Exit game");
}
int resume_game(){
     FILE *fp=NULL;
     fp=fopen("saved_game.txt","r");
     if(fp==NULL){
             printf("No saved data available\n");
             return 0;
     }
     for(int i=0;i<8;i++){
             for(int j=0;j<8;j++){
                 strcpy(board[i][j]," ");
             }
     }
     return 1;
}
void InitBoard(char board[8][8][30],CHESS white,CHESS black){

    strcpy(board[0][0],white.rook);
    strcpy(board[0][7],white.rook);
    strcpy(board[0][1],white.knight);
    strcpy(board[0][6],white.knight);
    strcpy(board[0][2],white.bishop);
    strcpy(board[0][5],white.bishop);
    strcpy(board[0][4],white.queen);
    strcpy(board[0][3],white.king);

    for (int i = 0; i < 8; i++) {
        strcpy(board[1][i],white.pawn);
        strcpy(board[6][i],black.pawn);
    }

    strcpy(board[7][0],black.rook);
    strcpy(board[7][7],black.rook);
    strcpy(board[7][1],black.knight);
    strcpy(board[7][6],black.knight);
    strcpy(board[7][2],black.bishop);
    strcpy(board[7][5],black.bishop);
    strcpy(board[7][4],black.queen);
    strcpy(board[7][3],black.king);

    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
           strcpy(board[i][j]," ");
        }
    }
}