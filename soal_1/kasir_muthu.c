#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
pid_t pid = fork();
int status;
if (pid == 0) {
  execlp("mkdir", "mkdir", "brankas_kedai", NULL);
  exit(EXIT_FAILURE);
} else {
  waitpid(pid, &status, 0);
  if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
    printf("[ERROR] Aiyaa! Proses gagal, file atau folder tidak ditemukan.\n");
    exit(EXIT_FAILURE);
  }
}
pid = fork();

if (pid == 0) {
    execlp("cp", "cp", "buku_hutang.csv", "brankas_kedai/", NULL);
    exit(EXIT_FAILURE);
} else {
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        printf("[ERROR] Aiyaa! Proses gagal, file atau folder tidak ditemukan.\n");
        exit(EXIT_FAILURE);
    }
}
pid = fork();

if (pid == 0) {
  execlp("bash", "bash", "-c", "grep 'Belum Lunas' brankas_kedai/buku_hutang.csv > brankas_kedai/daftar_penunggak.txt", NULL);
  exit(EXIT_FAILURE);
} else {
  waitpid(pid, &status, 0);
  if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
    printf("[ERROR] Aiyaa! Proses gagal, file atau folder tidak ditemukan.\n");
    exit(EXIT_FAILURE);
  }
}
pid = fork();

if (pid == 0) {
  execlp("zip", "zip", "-r", "rahasia_muthu.zip", "brankas_kedai", NULL);
  exit(EXIT_FAILURE);
} else {
  waitpid(pid, &status, 0);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
    printf("[ERROR] Aiyaa! Proses gagal, file atau folder tidak ditemukan.\n");
    exit(EXIT_FAILURE);
    }
}
printf("[INFO] Fuhh, selamat! Buku hutang dan daftar penagihan berhasil diamankan.\n");
return 0;
}



