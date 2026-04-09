# SISOP-2-2026-IT-116
Dian Hanna Simanjuntak (5027251116)
## Reporting

### Soal 1 - Kasbon Warga Kampung Durian Runtuh
Uncle Muthu mengalami masalah karena komputer kasirnya terkena virus. Di dalamnya terdapat file `buku_hutang.csv` yang berisi daftar utang pelanggan.

Program diminta untuk:
1. Membuat folder `brankas_kedai`
2. Menyalin file `buku_hutang.csv` ke dalam folder tersebut
3. Mengambil data dengan status **"Belum Lunas"**
4. Menyimpannya ke file `daftar_penunggak.txt`
5. Mengompres folder menjadi `rahasia_muthu.zip`

Program menggunakan konsep **parent-child process**:

- Parent (Upin) membuat child (Ipin)
- Child menjalankan perintah menggunakan `exec()`
- Parent menunggu dengan `waitpid()`
- Jika gagal → program berhenti

**Download file** `buku_hutang.csv`  
```bash
  wget -O buku_hutang.csv "https://drive.google.com/uc?export=download&id=144hsFJGoAM5lbvBQjkTfZaD3GOil4yEw"
```
**Apa ya**  `kasir_muthu.c`  
**Membuat folder** `brankas_kedai`
```c
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
```
**Menyalin file** `buku_hutang.csv`  
```c
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
```
**Filtering data** `Belum Lunas` **dan simpan data ke** `daftar_penunggak.txt`  
```c
pid = fork();

if (pid == 0) {
  execlp("bash", "bash", "-c", "grep 'Belum Lunas' brankas_kedai/buku_hutang.csv > brankas_kedai/daftar_penunggak.txt",>
  exit(EXIT_FAILURE);
} else {
  waitpid(pid, &status, 0);
  if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
    printf("[ERROR] Aiyaa! Proses gagal, file atau folder tidak ditemukan.\n");
    exit(EXIT_FAILURE);
  }
}
```
**Kompress folder** `rahasia_muthu.zip`
```c
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
```
---  
### Soal 2 - The world never stops, even when you feel tired.  
