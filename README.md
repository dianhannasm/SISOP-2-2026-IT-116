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
Program ini mengimplementasikan daemon process yang berjalan di background untuk memonitor file `contract.txt` dan mencatat aktivitas ke `work.log`c.  
  
**Handle saat daemon dihentikan**  
```c
void handle_stop(int sig) {
  FILE *log = fopen("work.log", "a");
  fprintf(log, "We really weren't meant to be together\n");
  fclose(log);
  running = 0;
}
```
Fungsi ini menangani sinyal penghentian program dan menuliskan pesan terakhir ke work.log sebelum daemon berhenti.  
  
**Membuat daemon**  
```c
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
```
Fungsi ini digunakan untuk mengubah program menjadi daemon dengan memisahkan proses dari terminal menggunakan fork() dan setsid().  
  
**Timestamp**  
```c
void get_time(char *buf) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  strftime(buf, 100, "%Y-%m-%d %H:%M:%S", t);
}
```
Digunakan untuk mendapatkan waktu saat ini dan memformatnya menjadi string timestamp.  
  
**Restored `contract.txt`**  
```c
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
```
Fungsi ini membuat file contract.txt saat awal program berjalan juga saat file perlu direstore.  
  
**Validasi `contract.txt`**  
```c
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
```
Digunakan untuk mengecek apakah isi file masih sesuai dengan format yang benar atau telah dimodifikasi.  
  
**Menulis status acak pada `work.log`**  
```c
void write_log() {
    FILE *log = fopen("work.log", "a");

    char *status[] = {"[awake]", "[drifting]", "[numbness]"};
    int r = rand() % 3;

    fprintf(log, "still working... %s\n", status[r]);
    fclose(log);
}
```
Fungsi ini menuliskan log setiap 5 detik dengan status acak ke dalam `work.log`.  
  
**Main**  
```c
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
```
Menjalankan loop daemon untuk memonitor file, mendeteksi perubahan atau penghapusan, serta menuliskan log secara berkala.  
  
**Test Flow**  
*Clean*
```bash
pkill contract_daemon
rm work.log contract.txt
```
*Jalankan daemon*
```bash
./contract_daemon
```
*Cek file*
```bash
ls
```

<img width="1715" height="88" alt="image" src="https://github.com/user-attachments/assets/70f6a28b-3fa3-4438-b94e-7cd8e3fc61e7" />

*Isi `contract.txt`*  
```bash
cat contract.txt
```

<img width="1713" height="124" alt="image" src="https://github.com/user-attachments/assets/e4df1c1a-355e-4806-b502-9c56d85a630d" />

*Log berjalan*  
```bash
cat work.log
```

<img width="1710" height="510" alt="image" src="https://github.com/user-attachments/assets/e3b3c7e8-7920-4648-be8f-8da6788d9f18" />

*Delete file `contract.txt`*
```bash
rm contract.txt
```

<img width="1712" height="191" alt="image" src="https://github.com/user-attachments/assets/5c204867-61a2-4ab3-b628-4883850d18d3" />

File muncul kembali setelah 1-2 detik.  

<img width="1713" height="104" alt="image" src="https://github.com/user-attachments/assets/e1aa70c6-a76f-4528-afe9-c0bf0bd75648" />

```bash
cat contract.txt
```

<img width="1710" height="129" alt="image" src="https://github.com/user-attachments/assets/1a7c8e18-39fe-4209-b961-5f28d37af0e6" />
  
*Test edit file*
```bash
nano contract.txt
```

<img width="1698" height="95" alt="image" src="https://github.com/user-attachments/assets/d8317e6a-802c-492e-99c3-b12036878d50" />
  
Pada `work.log` akan muncul `contract violated`  
<img width="1723" height="55" alt="image" src="https://github.com/user-attachments/assets/aee31bdf-e6b1-4d10-a5f9-81dee2f3830a" />

Dan isi file `contract.txt` kembali semula  

<img width="1725" height="127" alt="image" src="https://github.com/user-attachments/assets/47458ce9-7f9c-4f3b-9afe-0c6f04d28c75" />

*Stop daemon*  
Cek PID  
<img width="1708" height="110" alt="image" src="https://github.com/user-attachments/assets/99794d40-1d5d-4bb1-95b5-27a35ac9bced" />

Kill daemon dan cek status pada `work.log`  

<img width="1724" height="148" alt="image" src="https://github.com/user-attachments/assets/d3641d41-fe21-4db2-941a-6bdc191d3f7a" />

<img width="1722" height="98" alt="image" src="https://github.com/user-attachments/assets/3ead6148-bb3a-4f14-bafd-bffa10268c2b" />
