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
#define UP 38
#define DOWN 40
#define RIGHT 39
#define LEFT 37
#define SETUP_SCREEN() SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),5)
#define RESET_SCREEN()
#define SLEEP() Sleep(200)
#define GET_KEY(key) {\
                         HANDLE in=GetStdHandle(STD_INPUT_HANDLE);\
                         DWORD read,events;\
                         INPUT_RECORD rec;\
                         GetNumberOfConsoleInputEvents(in,&events);\
                         while(events){\
                             ReadConsoleInput(in,&rec,1,&read);\
                             if(rec.EventType==KEY_EVENT)\
                                 key=rec.Event.KeyEvent.wVirtualKeyCode;\
                             GetNumberOfConsoleInputEvents(in,&events);\
                         }\
                     }
#define INCLUDES "#include<windows.h>%c"
#define INCLUDES_FORMAT 10
#else
#include <unistd.h>
#include <termios.h>
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68
#define SETUP_SCREEN() struct termios oldt,newt;\
                       tcgetattr(STDIN_FILENO,&oldt);\
                       newt=oldt;\
                       newt.c_lflag&=~(ICANON|ECHO);\
                       newt.c_cc[VMIN]=0;\
                       tcsetattr(STDIN_FILENO,TCSANOW,&newt)
#define RESET_SCREEN() tcsetattr(STDIN_FILENO, TCSANOW, &oldt)
#define SLEEP() usleep(200000L)
#define GET_KEY(key) for(char c;read(STDIN_FILENO,&c,1)>0;key=c)
#define INCLUDES "#include<unistd.h>%c#include<termios.h>%c"
#define INCLUDES_FORMAT 10,10
#endif

#define X(...) #__VA_ARGS__
#define S(x) X(x)

char lines[HEIGHT][WIDTH + 1];
#define MAX_LOCATIONS ((WIDTH - 2) * (HEIGHT - 2) * 2)
char locations[MAX_LOCATIONS] = {0};
#define NEXT_LOCATION(x) locations[x = (x + 1) % MAX_LOCATIONS]

#define MAIN(x1,x2,x3,x4,x5,w,h,n,payloadS,playloadE) \
int main(){\
    payloadS;SETUP_SCREEN();int pX=x1,pY=x2,head=x3,tail=x4,dirX=0,dirY=0;char key=0;\
    while(1){\
        SLEEP();GET_KEY(key);\
        switch(key){\
            case UP:dirX=0;dirY=-1;break;\
            case DOWN:dirX=0;dirY=1;break;\
            case RIGHT:dirX=1;dirY=0;break;\
            case LEFT:dirX=-1;dirY=0;break;\
            case'Q':case'q':goto _exit;\
            case'P':case'p':case 0:continue;\
    }n(head)=pY;n(head)=pX;pX+=dirX;pY+=dirY;\
    switch(lines[pY][pX]){\
        case PLAYER:case WALL:goto _exit;\
        case FOOD:{\
            int rX,rY;do{rX=(rand()x5(w-2))+1;rY=(rand()x5(h-2))+1;}while(lines[rY][rX]!=EMPTY);\
            lines[rY][rX]=FOOD;break;}\
        case EMPTY:{char*line=lines[n(tail)];line[n(tail)]=EMPTY;}\
     }lines[pY][pX]=PLAYER;playloadE
#define INIT \
    memset(lines[0], WALL, WIDTH);\
    lines[0][WIDTH] = 0;\
    memset(lines[HEIGHT - 1], WALL, WIDTH);\
    lines[HEIGHT - 1][WIDTH] = 0;\
    for (int line = 1; line < HEIGHT - 1; line++) {\
        lines[line][0] = WALL;\
        memset(lines[line] + 1, EMPTY, WIDTH - 2);\
        lines[line][WIDTH - 1] = WALL;\
        lines[line][WIDTH] = 0;\
    }\
    lines[HEIGHT / 4][WIDTH / 4] = FOOD
#define END \
        lines[pY][pX] = PLAYER;\
        printf("%c[H%c[2J%c[3Jchar lines[%d][%d] = {", 27, 27, 27, HEIGHT, WIDTH + 1);\
        for (int line = 0; line < HEIGHT; line++)\
            printf("%c%c%s%c,", 10, 34, lines[line], 34);\
        int end = MAX_LOCATIONS;\
        while(!locations[--end]);\
        printf("};char locations[%d]={", MAX_LOCATIONS);\
        for (int loc = 0; loc <= end; loc++)\
            printf("%d,", locations[loc]);\
        char *pr = strstr(f, ";printf");\
        printf(f, MAX_LOCATIONS, 10, 10, 10, INCLUDES_FORMAT, 10, 10, 10, 10, 34, strstr(f,\
              "%c#define MAIN") - f, f, 34, 34, strstr(pr, "END") - pr, pr, 34, 34, 34, 10, 10, pX,\
              pY, head, tail, 34, HEIGHT, WIDTH + 1, 34, 34, 34, 34, MAX_LOCATIONS, 34, 34, 34, 10);\
    }\
_exit:\
    RESET_SCREEN();\
    return 0;\
}

char *f = "};int m=%d,h="S(HEIGHT)",w="S(WIDTH)";%c#include<stdio.h>%c#include<stdlib.h>%c" INCLUDES
"#define MAIN(x1,x2,x3,x4,x5) "S(MAIN(x1,x2,x3,x4,x5,w,h,NEXT,,))"%c#define X(...) #__VA_ARGS__%c#de"
"fine S(x) X(x)%c#define END printf(f,m,10,10,10,"S(INCLUDES_FORMAT)",10,34,f,34,pX,pY,head,tail,34"
",h,w+1,34,34,34,34,m,34,34,34,10);}_exit:"S(RESET_SCREEN())";return 0;}%cchar*f=%c%.*s%%c#define N"
"EXT(x) locations[x=(x+1)%%%%m]%%cchar*f=%%c%%s%%c;%cS(MAIN(%%d,%%d,%%d,%%d,%%%%))%c%.*s%cS(END)%c%"
"%c%c;%c#define NEXT(x) locations[x=(x+1)%%m]%cMAIN(%d,%d,%d,%d,%%);printf(%c%%c[H%%c[2J%%c[3Jchar "
"lines[%d][%d] = {%c,27,27,27);for(int line=0;line<h;line++)printf(%c%%c%%c%%s%%c,%c,10,34,lines[li"
"ne],34);printf(%c};char locations[%d]={%c);for(int loc=0;loc<m;loc++)printf(%c%%d,%c,locations[loc"
"]);END%c";

MAIN(WIDTH / 2, HEIGHT / 2, 0, 0, %, WIDTH, HEIGHT, NEXT_LOCATION, INIT, END)

