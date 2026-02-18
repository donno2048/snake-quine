#include <unistd.h>
#include <pty.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>

int fd;

char last_screen[6000];

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
    system("gcc main.c");
    for (int i = 0; i < 5; i++) {
        int pid = forkpty(&fd, NULL, NULL, NULL);
        if (pid == 0)
            execvp("./a.out", (char*[]){NULL});
        else {
            sleep(1);
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
            FILE *out = fopen("_test.c", "w");
            fputs(last_screen + 11, out);
            fclose(out);
            waitpid(pid, NULL, 0);
            int status = system("gcc _test.c");
            if (status == -1 || !WIFEXITED(status) || WEXITSTATUS(status))
                return -1;
        }
    }
    return 0;
}
