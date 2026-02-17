#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define WIDTH 25
#define HEIGHT 15
#define FOOD 'o'
#define PLAYER 'x'
#define WALL 'X'
#define EMPTY ' '

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75
#define SETUP_SCREEN() SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),ENABLE_PROCESSED_OUTPUT|ENABLE_WRAP_AT_EOL_OUTPUT|ENABLE_VIRTUAL_TERMINAL_PROCESSING)
#define RESET_SCREEN()
#define SLEEP() Sleep(200)
#define GET_KEY(key) do{if(kbhit())key=getch()}while(!key)
#define INCLUDES "#include<windows.h>%c#include<conio.h>"
#define INCLUDES_FORMAT 10
#else
#include <unistd.h>
#include <termios.h>
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68
#define SETUP_SCREEN() struct termios oldt,newt;tcgetattr(STDIN_FILENO,&oldt);newt=oldt;newt.c_lflag&=~(ICANON|ECHO);newt.c_cc[VMIN]=0;tcsetattr(STDIN_FILENO,TCSANOW,&newt)
#define RESET_SCREEN() tcsetattr(STDIN_FILENO, TCSANOW, &oldt)
#define SLEEP() usleep(200000L)
#define GET_KEY(key) for(char c;read(STDIN_FILENO,&c,1)>0||!key;key=c)
#define INCLUDES "#include<unistd.h>%c#include<termios.h>"
#define INCLUDES_FORMAT 10
#endif

#define X(...) #__VA_ARGS__
#define STR(x) X(x)

char lines[HEIGHT][WIDTH + 1];
#define MAX_LOCATIONS ((WIDTH - 2) * (HEIGHT - 2) * 2)
char locations[MAX_LOCATIONS] = {0};
#define NEXT_LOCATION(x) locations[x = (x + 1) % MAX_LOCATIONS]


char *f = "};int m=%d,h="STR(HEIGHT)",w="STR(WIDTH)";%c#include<stdio.h>%c#include<stdlib.h>%c"INCLUDES
"%c#define NEXT_LOCATION(x) locations[x=(x+1)%%m]%cchar*f=%c"
"%s%c;int main(){"STR(SETUP_SCREEN())
";int pX=%d,pY=%d,head=%d,tail=%d,dirX=%d,dirY=%d;char key=0;while(1){_sleep:"STR(SLEEP())
";"STR(GET_KEY(key))";switch(key&0xdf){case "STR(UP)":dirX=0;"
"dirY=-1;break;case "STR(DOWN)":dirX=0;dirY=1;break;case "STR(RIGHT)":dirX=1;dirY=0;break;case "STR(LEFT)":dirX=-1;dirY=0"
";break;case'Q':goto _exit;case'P':goto _sleep;}NEXT_LOCATION(head)=pY;NEXT_LOCATION(head)=pX;pX"
"+=dirX;pY+=dirY;switch(lines[pY][pX]){case"STR(PLAYER)":case"STR(WALL)":goto _exit;case"STR(FOOD
)":{char rX,rY;do{rX=(rand()%%(w-2))+1;rY=(rand()%%(h-2))+1;}while(lines[rY][rX]!="STR(EMPTY)");lin"
"es[rY][rX]="STR(FOOD)";break;}case"STR(EMPTY)":lines[NEXT_LOCATION(tail)][NEXT_LOCATION(tail)]="S\
TR(EMPTY)";}lines[pY][pX]="STR(PLAYER)";printf(%c%%c[H%%c[2J%%c[3Jchar lines[%d][%d] = {%c,27,27,27"
");for(char line=0;line<h;line++)printf(%c%%c%%c%%s%%c,%c,10,34,lines[line],34);printf(%c};char loc"
"ations[%d]={%c);for(int loc=0;loc<m;loc++)printf(%c%%d,%c,locations[loc]);printf(f,m,10,10,"
"10,"STR(INCLUDES_FORMAT)",10,10,34,f,34,pX,pY,head,tail,dirX,dirY,34,h,w+1,34,34,34,34,m,34,34,34,10);}_exit:"
STR(RESET_SCREEN())";return 0;}%c";

int main() {
    SETUP_SCREEN();

    memset(lines[0], WALL, WIDTH);
    lines[0][WIDTH] = 0;
    memset(lines[HEIGHT - 1], WALL, WIDTH);
    lines[HEIGHT - 1][WIDTH] = 0;
    for (int line = 1; line < HEIGHT - 1; line++) {
        lines[line][0] = WALL;
        memset(lines[line] + 1, EMPTY, WIDTH - 2);
        lines[line][WIDTH - 1] = WALL;
        lines[line][WIDTH] = 0;
    }
    lines[HEIGHT / 4][WIDTH / 4] = FOOD;

    int pX = WIDTH / 2, pY = HEIGHT / 2;
    int head = 0, tail = 0;
    int dirX = 0, dirY = 0;

    char key = 0;

    while (1) {
_sleep:
        SLEEP();
        GET_KEY(key);
        switch (key & 0xDF) {
            case UP: dirX = 0; dirY = -1; break;
            case DOWN: dirX = 0; dirY = 1; break;
            case RIGHT: dirX = 1; dirY = 0; break;
            case LEFT: dirX = -1; dirY = 0; break;
            case 'Q': goto _exit;
            case 'P': goto _sleep;
        }
        NEXT_LOCATION(head) = pY;
        NEXT_LOCATION(head) = pX;
        pX += dirX;
        pY += dirY;
        switch (lines[pY][pX]) {
            case PLAYER: case WALL: goto _exit;
            case FOOD:
                {
                     char rX, rY;
                     do {
                         rX = (rand() % (WIDTH - 2)) + 1;
                         rY = (rand() % (HEIGHT - 2)) + 1;
                     } while (lines[rY][rX] != EMPTY);
                     lines[rY][rX] = FOOD;
                     break;
                 }
            case EMPTY: lines[NEXT_LOCATION(tail)][NEXT_LOCATION(tail)] = EMPTY;
        }
        lines[pY][pX] = PLAYER;
        printf("%c[H%c[2J%c[3Jchar lines[%d][%d] = {", 27, 27, 27, HEIGHT, WIDTH + 1);
        for (char line = 0; line < HEIGHT; line++)
            printf("%c%c%s%c,", 10, 34, lines[line], 34);
        printf("};char locations[%d]={", MAX_LOCATIONS);
        for (int loc = 0; loc < MAX_LOCATIONS; loc++)
            printf("%d,", locations[loc]);
        printf(f, MAX_LOCATIONS, 10, 10, 10, INCLUDES_FORMAT, 10, 10, 34, f, 34, pX, pY, head,
               tail, dirX, dirY, 34, HEIGHT, WIDTH + 1, 34, 34, 34, 34, MAX_LOCATIONS, 34, 34, 34, 10);
    }
_exit:
    RESET_SCREEN();
    return 0;
}

