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
  
**Program dibuat di `kasir_muthu.c`**  
Setiap proses dijalankan oleh child process menggunakan `exec()`, dan parent akan menunggu menggunakan `waitpid()` agar berjalan secara berurutan.  
  
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
Child process menjalankan perintah `mkdir` untuk membuat folder `brankas_kedai`, sedangkan parent menunggu proses selesai dan memastikan tidak terjadi error sebelum lanjut.  
  
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
Child process menyalin file `buku_hutang.csv` ke dalam folder `brankas_kedai`, lalu parent memastikan proses berhasil sebelum melanjutkan ke tahap berikutnya.  
  
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
Child process menjalankan `grep` melalui `bash -c` untuk mengambil data dengan status "Belum Lunas" dan menyimpannya ke file baru, sementara parent memastikan proses berhasil.  
  
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
Child process mengompres folder `brankas_kedai` menjadi file `rahasia_muthu.zip`, dan setelah semua proses berhasil, program menampilkan pesan sukses.  

    
**Test flow**  
  
*Output terminal*  
  
<img width="1724" height="177" alt="image" src="https://github.com/user-attachments/assets/65adf37a-8290-49f8-9235-e9936982536b" />
  
*Hasil `grep` pada `daftar_penunggak.txt`*  
  
<img width="1723" height="315" alt="image" src="https://github.com/user-attachments/assets/8abdc9f8-110b-4dc2-bb0d-85b7fd849562" />
  
*Isi zip*  
  
<img width="1724" height="290" alt="image" src="https://github.com/user-attachments/assets/43bc14ff-ebff-4931-bd13-7131e5a1c557" />
  
*Test error*  
  
<img width="1720" height="59" alt="image" src="https://github.com/user-attachments/assets/1d3b28cb-a1b1-4ace-be62-2445e8a4edf9" />

<img width="1722" height="117" alt="image" src="https://github.com/user-attachments/assets/cb99e0e7-7c06-4ed4-9ffd-4742228c256f" />
   
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
  
  
  
---  
## Revisi  
Soal yang tidak sempat terselesaikan.  
### Soal 3 - One letter for destiny  
Program ini mengimplementasikan daemon process yang secara otomatis menulis pesan cinta acak ke dalam file `LoveLetter.txt`, kemudian mengenkripsinya menggunakan base64, serta mencatat aktivitas ke dalam `ethereal.log`.  

Program memiliki 3 fitur utama:  
```bash
./angel
Penggunaan:
./angel -daemon
: jalankan sebagai daemon
./angel -decrypt
: decrypt LoveLetter.txt
./angel -kill
: kill proses
```

**`angel.c`**
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/prctl.h>

char *quotes[] = {
    "aku akan fokus pada diriku sendiri",
    "aku mencintaimu dari sekarang hingga selamanya",
    "aku akan menjauh darimu, hingga takdir mempertemukan kita di versi kita yang terbaik.",
    "kalau aku dilahirkan kembali, aku tetap akan terus menyayangimu"
};

void write_log(char *process, char *status) {
    FILE *log = fopen("ethereal.log", "a");

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(log, "[%02d:%02d:%04d]-[%02d:%02d:%02d]_%s_%s\n",
        tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900,
        tm.tm_hour, tm.tm_min, tm.tm_sec,
        process, status);

    fclose(log);
}

void encode_base64() {
    write_log("surprise", "RUNNING");
    system("base64 LoveLetter.txt > temp.txt && mv temp.txt LoveLetter.txt");
    write_log("surprise", "SUCCESS");
}

void write_random_quote() {
    write_log("secret", "RUNNING");

    FILE *fp = fopen("LoveLetter.txt", "w");
    if (!fp) {
        write_log("secret", "ERROR");
        return;
    }

    int idx = rand() % 4;
    fprintf(fp, "%s\n", quotes[idx]);
    fclose(fp);

    write_log("secret", "SUCCESS");
}

void decrypt_file() {
    write_log("decrypt", "RUNNING");

    if (access("LoveLetter.txt", F_OK) == -1) {
        printf("File tidak ditemukan!\n");
        write_log("decrypt", "ERROR");
        return;
    }

    system("base64 -d LoveLetter.txt > temp.txt && mv temp.txt LoveLetter.txt");

    write_log("decrypt", "SUCCESS");
}

void run_daemon(char *argv[]) {
    pid_t pid = fork();

    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    umask(0);
    setsid();
    prctl(PR_SET_NAME, "maya", 0, 0, 0);
    strcpy(argv[0], "maya");

    while (1) {
        write_random_quote();
        encode_base64();
        sleep(10);
    }
}

void kill_daemon() {
    write_log("kill", "RUNNING");

    int result = system("pkill -x maya");

    if (result == 0) {
        printf("Daemon berhasil dihentikan.\n");
        write_log("kill", "SUCCESS");
    } else {
        printf("Daemon tidak ditemukan!\n");
        write_log("kill", "ERROR");
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (argc < 2) {
        printf("Penggunaan:\n");
        printf("./angel -daemon\n");
        printf("./angel -decrypt\n");
        printf("./angel -kill\n");
        return 1;
    }

    if (strcmp(argv[1], "-daemon") == 0) {
        run_daemon(argv);
    }
    else if (strcmp(argv[1], "-decrypt") == 0) {
        decrypt_file();
    }
    else if (strcmp(argv[1], "-kill") == 0) {
        kill_daemon();
    }
    else {
        printf("Argumen tidak valid!\n");
    }
    return 0;
}
```

**Penjelasan per fungsi**  
*Membuat daemon*  
```c
void write_log(char *process, char *status) {
    FILE *log = fopen("ethereal.log", "a");

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(log, "[%02d:%02d:%04d]-[%02d:%02d:%02d]_%s_%s\n",
        tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900,
        tm.tm_hour, tm.tm_min, tm.tm_sec,
        process, status);

    fclose(log);
}
```
Semua aktivitas dicatat ke `ethereal.log` dengan format:  
```
[dd:mm:yyyy]-[hh:mm:ss]_process_status
```
Contoh:
```
[08:04:2026]-[02:44:06]_secret_RUNNING
[08:04:2026]-[02:44:06]_secret_SUCCESS
```

*Enkripsi Base64*
```c
void encode_base64() {
    write_log("surprise", "RUNNING");
    system("base64 LoveLetter.txt > temp.txt && mv temp.txt LoveLetter.txt");
    write_log("surprise", "SUCCESS");
}
```  
Fungsi ini mengenkripsi isi file menggunakan base64 dan menggunakan file sementara (`temp.txt`).  

*Menulis Pesan Acak*  
```c
void write_random_quote() {
    write_log("secret", "RUNNING");

    FILE *fp = fopen("LoveLetter.txt", "w");
    if (!fp) {
        write_log("secret", "ERROR");
        return;
    }

    int idx = rand() % 4;
    fprintf(fp, "%s\n", quotes[idx]);
    fclose(fp);

    write_log("secret", "SUCCESS");
}
```  
Fungsi ini memilih quote secara acak, menulis ke `LoveLetter.txt`, file selalu di-overwrite ("`w`").  

*Dekripsi File*  
```c
void decrypt_file() {
    write_log("decrypt", "RUNNING");

    if (access("LoveLetter.txt", F_OK) == -1) {
        printf("File tidak ditemukan!\n");
        write_log("decrypt", "ERROR");
        return;
    }

    system("base64 -d LoveLetter.txt > temp.txt && mv temp.txt LoveLetter.txt");

    write_log("decrypt", "SUCCESS");
}
```
Fungsi ini digunakan untuk mengembalikan isi file ke bentuk asli, menggunakan command `base64 -d`.  

*Membuat Daemon*  
```c
void run_daemon(char *argv[]) {
    pid_t pid = fork();

    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    umask(0);
    setsid();
    prctl(PR_SET_NAME, "maya", 0, 0, 0);
    strcpy(argv[0], "maya");

    while (1) {
        write_random_quote();
        encode_base64();
        sleep(10);
    }
}
```
Fungsi ini mengubah program menjadi daemon:  
- `fork()` → memisahkan parent & child  
- `setsid()` → membuat session baru  
- `prctl` & `argv[0]` → mengubah nama proses menjadi maya  
- `chdir("/")` & `close fd` → detach dari terminal  
  
Daemon akan berjalan setiap 10 detik:  
- Menulis pesan random  
- Mengenkripsi dengan base64

*Menghentikan Daemon*
```c
void kill_daemon() {
    write_log("kill", "RUNNING");

    int result = system("pkill -x maya");

    if (result == 0) {
        printf("Daemon berhasil dihentikan.\n");
        write_log("kill", "SUCCESS");
    } else {
        printf("Daemon tidak ditemukan!\n");
        write_log("kill", "ERROR");
    }
}
```
`pkill -x maya` untuk menghentikan proses.  

*Main*  
```c
int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (argc < 2) {
        printf("Penggunaan:\n");
        printf("./angel -daemon\n");
        printf("./angel -decrypt\n");
        printf("./angel -kill\n");
        return 1;
    }

    if (strcmp(argv[1], "-daemon") == 0) {
        run_daemon(argv);
    }
    else if (strcmp(argv[1], "-decrypt") == 0) {
        decrypt_file();
    }
    else if (strcmp(argv[1], "-kill") == 0) {
        kill_daemon();
    }
    else {
        printf("Argumen tidak valid!\n");
    }
    return 0;
}
```

**Output**  
`./angel -daemon`  
<img width="1722" height="217" alt="image" src="https://github.com/user-attachments/assets/1a1c111c-6f05-4859-8e4b-e7bc9b2edc2f" />

*Cek file*  
<img width="1714" height="212" alt="image" src="https://github.com/user-attachments/assets/576a96b4-78c1-46b3-b7b4-9a480f15aea7" />

`./angel -decrypt`    
<img width="1712" height="99" alt="image" src="https://github.com/user-attachments/assets/2c13366a-934a-4944-bb33-9a8c36e9c1ee" />

*Cek log*  
<img width="1714" height="430" alt="image" src="https://github.com/user-attachments/assets/195f6f4b-e17b-4dc5-8a7d-740cb7b94351" />

`.angel -kill`  
<img width="1716" height="97" alt="image" src="https://github.com/user-attachments/assets/a7e48fa6-14e2-40c6-97f0-814f9cbfeef4" />


