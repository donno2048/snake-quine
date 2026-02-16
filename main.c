#define WIDTH 20
#define HEIGHT 20
char lines[HEIGHT][WIDTH + 1] = {
    "XXXXXXXXXXXXXXXXXXXX",
    "X                  X",
    "X                  X",
    "X                  X",
    "X                  X",
    "X    o             X",
    "X                  X",
    "X                  X",
    "X                  X",
    "X                  X",
    "X                  X",
    "X                  X",
    "X                  X",
    "X                  X",
    "X             x    X",
    "X                  X",
    "X                  X",
    "X                  X",
    "X                  X",
    "XXXXXXXXXXXXXXXXXXXX",
};
#define MAX_LOCATIONS ((WIDTH - 2) * (HEIGHT - 2) * 2)
char locations[MAX_LOCATIONS] = {0};

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#define NEXT_LOCATION(x) locations[x = (x + 1) % MAX_LOCATIONS]

char *f = "};int m=%d,h=%d,w=%d;%c#include <stdio.h>%c#include <unistd.h>%c"
"#include <stdlib.h>%c#include <fcntl.h>%c#include <termios.h>%c#define "
"NEXT_LOCATION(x) locations [x=(x+1)%cm]%cchar *f=%c%s%c;int main(){struct "
"termios oldt,newt;tcgetattr(STDIN_FILENO,&oldt);newt=oldt;newt.c_lflag&=~"
"(ICANON|ECHO);tcsetattr(STDIN_FILENO,TCSANOW,&newt);fcntl(STDIN_FILENO,F_SETFL"
",fcntl(STDIN_FILENO,F_GETFL,0)|O_NONBLOCK);int pX=%d,pY=%d,head=%d,tail=%d,dirX"
"=%d,dirY=%d;char key=0;while(1){usleep(200000L);for(char c;read(STDIN_FILENO"
",&c,1)>0||!key;key=c);switch(key){case 0x41:dirX=0;dirY=-1;break;case 0x42:dirX"
"=0;dirY=1;break;case 0x43:dirX=1;dirY=0;break;case 0x44:dirX=-1;dirY=0;break;"
"case 'q':goto _exit;}NEXT_LOCATION(head)=pY;NEXT_LOCATION(head)=pX;pX+=dirX;"
"pY+=dirY;switch(lines[pY][pX]){case 'x':case 'X':goto _exit;case 'o':{char rX"
",rY;do{rX=(rand()%c(w-2))+1;rY=(rand()%c(h-2))+1;}while(lines[rY][rX]"
"!=' ');lines[rY][rX]='o';break;}case ' ':lines[NEXT_LOCATION(tail)]"
"[NEXT_LOCATION(tail)]=' ';}lines[pY][pX]='x';printf(%c%ccchar lines[%d][%d]"
"={%c,10);for(char line=0;line<h;line++)printf(%c%cc%cc%cs%cc,%c,10,34,lines"
"[line],34);printf(%c};char locations[%d]={%c);for(int loc=0;loc<m;loc++)"
"printf(%c%cd,%c,locations[loc]);printf(f,m,h,w,10,10,10,10,10,10,37,10,34,f,34"
",pX,pY,head,tail,dirX,dirY,37,37,34,37,h,w+1,34,34,37,37,37,37,34,34,m,34,34,37"
",34,10);}_exit:tcsetattr(STDIN_FILENO,TCSANOW,&oldt);return 0;}%c";
int main() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);

    int pX = 14, pY = 14;
    int head = 0, tail = 0;
    int dirX = 0, dirY = 0;

    char key = 0;
    while (1) {
        usleep(200000L);
        for (char c; read(STDIN_FILENO, &c, 1) > 0 || !key; key = c);
        switch (key) {
            case 0x41: dirX = 0; dirY = -1; break;
            case 0x42: dirX = 0; dirY = 1; break;
            case 0x43: dirX = 1; dirY = 0; break;
            case 0x44: dirX = -1; dirY = 0; break;
            case 'q': goto _exit;
        }
        NEXT_LOCATION(head) = pY;
        NEXT_LOCATION(head) = pX;
        pX += dirX;
        pY += dirY;
        switch (lines[pY][pX]) {
            case 'x': case 'X': goto _exit;
            case 'o':
                {
                     char rX, rY;
                     do {
                         rX = (rand() % (WIDTH - 2)) + 1;
                         rY = (rand() % (HEIGHT - 2)) + 1;
                     } while (lines[rY][rX] != ' ');
                     lines[rY][rX] = 'o';
                     break;
                 }
            case ' ': lines[NEXT_LOCATION(tail)][NEXT_LOCATION(tail)] = ' ';
        }
        lines[pY][pX] = 'x';
        printf("%cchar lines[%d][%d] = {", 10, HEIGHT, WIDTH + 1);
        for (char line = 0; line < HEIGHT; line++)
            printf("%c%c%s%c,", 10, 34, lines[line], 34);
        printf("};char locations[%d] = {", MAX_LOCATIONS);
        for (int loc = 0; loc < MAX_LOCATIONS; loc++)
            printf("%d,", locations[loc]);
        printf(f, MAX_LOCATIONS, HEIGHT, WIDTH, 10, 10, 10, 10, 10, 10, 37, 10, 34, f, 34, pX,
               pY, head, tail, dirX, dirY, 37, 37, 34, 37, HEIGHT, WIDTH + 1, 34, 34, 37, 37,
               37, 37, 34, 34, MAX_LOCATIONS, 34, 34, 37, 34, 10);
    }
_exit:
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 0;
}

