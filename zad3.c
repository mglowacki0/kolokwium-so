#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        perror("Nie można otworzyć pliku");
        exit(1);
    }

    // Proces nadrzędny zapisujacyb 1 wpis
    fprintf(file, "Wpis inicjalizujący od procesu nadrzędnego\n");
    fflush(file);  // Zapewnia, że dane zostaną zapisane na dysk

    // fork - dziecko i rodzic
    pid_t pid = fork();
    if (pid < 0) {
        perror("Błąd przy tworzeniu procesu potomnego");
        fclose(file);
        exit(1);
    }

    if (pid == 0) {
        // zapisanie do pliku przez dziecko / potomka
        fprintf(file, "Wpis przetwórczy od procesu podrzędnego\n");
        fflush(file);  // od razu zapisze dane do pliku a nie z opóźnieniem np po zamknięciu programu - zmusi - bufor wyjściowy
        exit(0);
    } else {
        wait(NULL);  // Czeka na zakończenie procesu potomka


        fprintf(file, "Wpis kończący od procesu nadrzędnego\n");
        fflush(file);  // od razu zapisze dane do pliku a nie z opóźnieniem np po zamknięciu programu - zmusi - bufor wyjściowy
    }

    fclose(file);

    // wyświetlanie
    file = fopen("output.txt", "r");
    if (file == NULL) {
        perror("Nie można otworzyć pliku do odczytu");
        exit(1);
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) { //// Odczytuje linie z pliku, aż do końca w wielkości buffera
        printf("%s", buffer);
    }

    fclose(file);

    return 0;
}
