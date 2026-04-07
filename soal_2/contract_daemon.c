#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>

int running = 1;

void handle_stop(int sig) {
  FILE *log = fopen("work.log", "a");
  fprintf(log, "We really weren't meant to be together\n");
  fclose(log);
  running = 0;
}

void daemonize() {
  pid_t pid = fork();
  if (pid < 0) exit(EXIT_FAILURE);
  if (pid > 0) exit(EXIT_SUCCESS);

  umask(0);
  setsid();

  fclose(stdin);
  fclose(stdout);
  fclose(stderr);
}

void get_time(char *buf) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  strftime(buf, 100, "%Y-%m-%d %H:%M:%S", t);
}

void create_contract(int restored) {
    FILE *f = fopen("contract.txt", "w");
    char timebuf[100];
    get_time(timebuf);

    fprintf(f, "A promise to keep going, even when unseen.\n");

    if (restored)
        fprintf(f, "restored at: %s\n", timebuf);
    else
        fprintf(f, "created at: %s\n", timebuf);

    fclose(f);
}

int is_valid() {
    FILE *f = fopen("contract.txt", "r");
    if (!f) return 0;
    char line[256];
    fgets(line, sizeof(line), f);
    fclose(f);
    line[strcspn(line, "\n")] = 0;
    if (strcmp(line, "A promise to keep going, even when unseen.") == 0)
        return 1;
    else
        return 0;
}

void write_log() {
    FILE *log = fopen("work.log", "a");

    char *status[] = {"[awake]", "[drifting]", "[numbness]"};
    int r = rand() % 3;

    fprintf(log, "still working... %s\n", status[r]);
    fclose(log);
}

int main() {
    signal(SIGTERM, handle_stop);
    signal(SIGINT, handle_stop);

    daemonize();
    srand(time(NULL));
    create_contract(0);

    while (running) {
        struct stat st;
        if (stat("contract.txt", &st) != 0) {
            sleep(1);
            create_contract(1);
        } else {
            if (!is_valid()) {
                FILE *log = fopen("work.log", "a");
                fprintf(log, "contract violated.\n");
                fclose(log);
                create_contract(1);
            }
        }
        write_log();
        sleep(5);
    }
    return 0;
}
