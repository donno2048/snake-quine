#include <unistd.h>
#include <pty.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#define delay() usleep(1000000)

// generated on my system using:
// echo | bash --rcfile <(echo "PS1='$PS1'") -i 2>&1 | head -n1 | sed -n l | sed 's/\$$//'
#define PS1 "\033[01;32mroot\033[00m@\033[01;34m/root/snake-quine\033[00m$ "

int fd;

char last_screen[6000];

int _shell(const char *command, int fake) {
    fflush(stdout);
    delay();
    printf("%s", command);
    fflush(stdout);
    delay();
    puts("");
    int status = fake || system(command);
    printf(PS1);
    return status || !WIFEXITED(status) || WEXITSTATUS(status);
}

int shell(const char *command) {
    return _shell(command, 0);
}

double now() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

void write_halt(const char *w, double seconds) {
    static char buffer[7000];
    static size_t buffer_size = 0;

    write(fd, w, strlen(w));

    double end = now() + seconds;
    while (now() < end) {
        size_t r_count = read(fd, buffer + buffer_size, 1024);
        if (r_count <= 0) continue;
        buffer_size += r_count;
        buffer[buffer_size] = '\0';

        char *eof = strstr(buffer + 1, "\x1b[H");
        if (eof) {
            size_t screen_size = eof - buffer;
            memcpy(last_screen, buffer, screen_size);
            last_screen[screen_size] = '\0';
            puts(last_screen);
            buffer_size -= screen_size;
            memmove(buffer, eof, buffer_size);
        }
    }
}

int main() {
    printf(PS1);
    shell("gcc main.c");
    for (int i = 0; i < 3; i++) {
        int pid = forkpty(&fd, NULL, NULL, NULL);
        if (pid == 0)
            execvp("./a.out", (char*[]){NULL});
        else {
            fflush(stdout);
            delay();
            printf("./a.out");
            fflush(stdout);
            delay();
            write_halt("\x1b[A", 1);
            write_halt("\x1b[D", 1);
            write_halt("\x1b[B", .3);
            write_halt("\x1b[D", .5);
            write_halt("\x1b[B", 1.6);
            write_halt("\x1b[C", 1.5);
            write_halt("\x1b[A", 1.1);
            write_halt("\x1b[D", 1.3);
            write_halt("\x1b[B", 1);
            write_halt("\x1b[C", 1.3);
            write_halt("\x1b[A", .7);
            write(fd, "q", 1);
            waitpid(pid, NULL, 0);
            printf(PS1);
            _shell("copy_terminal _test.c", 1);
            FILE *out = fopen("_test.c", "w");
            fputs(last_screen + 11, out);
            fclose(out);
            shell("cat _test.c");
            if (shell("gcc _test.c")) return -1;
        }
    }
    return 0;
}
