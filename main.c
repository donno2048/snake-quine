#define WIDTH 25
#define HEIGHT 15
#define FOOD 'o'
#define PLAYER 'x'
#define WALL 'X'
#define EMPTY ' '

#define X(x) #x
#define STR(x) X(x)

char lines[HEIGHT][WIDTH + 1];
#define MAX_LOCATIONS ((WIDTH - 2) * (HEIGHT - 2) * 2)
char locations[MAX_LOCATIONS] = {0};
#define NEXT_LOCATION(x) locations[x = (x + 1) % MAX_LOCATIONS]

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

char *f = "};int m=%d,h=%d,w=%d;%c#include <stdio.h>%c#include <unistd.h>%c"
"#include <stdlib.h>%c#include <fcntl.h>%c#include <termios.h>%c#define "
"NEXT_LOCATION(x) locations [x=(x+1)%cm]%cchar *f=%c%s%c;int main(){struct "
"termios oldt,newt;tcgetattr(STDIN_FILENO,&oldt);newt=oldt;newt.c_lflag&=~"
"(ICANON|ECHO);tcsetattr(STDIN_FILENO,TCSANOW,&newt);fcntl(STDIN_FILENO,F_SETFL"
",fcntl(STDIN_FILENO,F_GETFL,0)|O_NONBLOCK);int pX=%d,pY=%d,head=%d,tail=%d,dirX"
"=%d,dirY=%d;char key=0;while(1){_sleep:usleep(200000L);for(char c;read(STDIN_FILENO"
",&c,1)>0||!key;key=c);switch(key&0xdf){case 0x41:dirX=0;dirY=-1;break;case 0x42:dirX"
"=0;dirY=1;break;case 0x43:dirX=1;dirY=0;break;case 0x44:dirX=-1;dirY=0;break;"
"case 'Q':goto _exit;case 'P': goto _sleep;}NEXT_LOCATION(head)=pY;NEXT_LOCATION"
"(head)=pX;pX+=dirX;pY+=dirY;switch(lines[pY][pX]){case "STR(PLAYER)":case "STR(WALL)":goto _exit;"
"case "STR(FOOD)":{char rX,rY;do{rX=(rand()%c(w-2))+1;rY=(rand()%c(h-2))+1;}while(lines"
"[rY][rX]!="STR(EMPTY)");lines[rY][rX]="STR(FOOD)";break;}case "STR(EMPTY)":lines[NEXT_LOCATION(tail)]"
"[NEXT_LOCATION(tail)]="STR(EMPTY)";}lines[pY][pX]="STR(PLAYER)";printf(%c%cc[2J%cc[Hchar "
"lines[%d][%d]={%c,27,27);for(char line=0;line<h;line++)printf(%c%cc%cc"
"%cs%cc,%c,10,34,lines[line],34);printf(%c};char locations[%d]={%c);for(int "
"loc=0;loc<m;loc++)printf(%c%cd,%c,locations[loc]);printf(f,m,h,w,10,10,10,10"
",10,10,37,10,34,f,34,pX,pY,head,tail,dirX,dirY,37,37,34,37,37,h,w+1,34,34"
",37,37,37,37,34,34,m,34,34,37,34,10);}_exit:tcsetattr(STDIN_FILENO,TCSANOW,"
"&oldt);return 0;}%c";

int main() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);

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
        usleep(200000L);
        for (char c; read(STDIN_FILENO, &c, 1) > 0 || !key; key = c);
        switch (key & 0xDF) {
            case 0x41: dirX = 0; dirY = -1; break;
            case 0x42: dirX = 0; dirY = 1; break;
            case 0x43: dirX = 1; dirY = 0; break;
            case 0x44: dirX = -1; dirY = 0; break;
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
        printf("%c[2J%c[Hchar lines[%d][%d] = {", 27, 27, HEIGHT, WIDTH + 1);
        for (char line = 0; line < HEIGHT; line++)
            printf("%c%c%s%c,", 10, 34, lines[line], 34);
        printf("};char locations[%d] = {", MAX_LOCATIONS);
        for (int loc = 0; loc < MAX_LOCATIONS; loc++)
            printf("%d,", locations[loc]);
        printf(f, MAX_LOCATIONS, HEIGHT, WIDTH, 10, 10, 10, 10, 10, 10, 37, 10, 34, f, 34, pX,
               pY, head, tail, dirX, dirY, 37, 37, 34, 37, 37, HEIGHT, WIDTH + 1, 34, 34, 37, 37,
               37, 37, 34, 34, MAX_LOCATIONS, 34, 34, 37, 34, 10);
    }
_exit:
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 0;
}

