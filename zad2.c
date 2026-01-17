#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    // Liczba procesów
    int N = 10;

    pid_t pid;
    int status;

    // petla do tworzenia N procesow potomnych
    for (int i = 0; i < N; i++) {
        pid = fork();  // tworzenie dziecka i rodzica - proces potomny

        if (pid == -1) {
            perror("Fork failed");
            exit(1);
        }

        if (pid == 0) {
            printf("Potomek PID: %d, Rodzic PID: %d, Kod statusu: %d\n", getpid(), getppid(), i + 1);
            //exit z unikalnym kodem / id
            exit(i + 1);
        }
    }

    //wait - program czeka na zakonczenie potomkow z forka
    for (int i = 0; i < N; i++) {
        pid = wait(&status);

        if (pid == -1) {
            perror("Wait failed");
            exit(1);
        }

        // sprawdzenie statusu 1/0 czy poprawnie sie zakonczyl
        if (WIFEXITED(status)) {
            printf("Proces potomny %d zakończył się z kodem: %d\n", pid, WEXITSTATUS(status));
        } else {
            printf("Proces potomny %d zakończył się z błędem\n", pid);
        }
    }

    return 0;
}
