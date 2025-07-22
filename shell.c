#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 200
#define MAX_TOKENS 50

char input_line[MAX_LINE];
char *commands[MAX_TOKENS];
char *args[MAX_TOKENS];

char input_file[MAX_LINE] = "";
char output_file[MAX_LINE] = "";
int num_pipes = 0;

// Mostrar prompt
void mostrarPrompt() {
    printf("\n\033[1;32musuario@usuario: ~$ \033[0m");
    fflush(stdout);
}

// Leer línea del usuario
void leerLinea() {
    fgets(input_line, MAX_LINE, stdin);
    input_line[strcspn(input_line, "\n")] = '\0';  // Quitar salto de línea
}

// Dividir comandos por '|'
void dividirPorPipes() {
    char *token = strtok(input_line, "|");
    int i = 0;
    while (token != NULL && i < MAX_TOKENS) {
        commands[i++] = token;
        token = strtok(NULL, "|");
    }
    commands[i] = NULL;
    num_pipes = i - 1;
}

// Detectar redirecciones < y >
void procesarRedirecciones(char *cmd) {
    input_file[0] = '\0';
    output_file[0] = '\0';

    char *input_redirect = strchr(cmd, '<');
    char *output_redirect = strchr(cmd, '>');

    if (input_redirect) {
        *input_redirect = '\0';
        sscanf(input_redirect + 1, "%s", input_file);
    }
    if (output_redirect) {
        *output_redirect = '\0';
        sscanf(output_redirect + 1, "%s", output_file);
    }
}

// Separar argumentos (tokens por espacio)
void separarArgumentos(char *cmd) {
    char *token = strtok(cmd, " ");
    int i = 0;
    while (token != NULL && i < MAX_TOKENS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

// Ejecutar comando individual
void ejecutarSimple(char *cmd) {
    procesarRedirecciones(cmd);
    separarArgumentos(cmd);

    if (args[0] == NULL) return;

    // Comando interno: cd
    if (strcmp(args[0], "cd") == 0) {
        if (args[1]) {
            if (chdir(args[1]) != 0)
                perror("cd");
        } else {
            fprintf(stderr, "cd: falta argumento\n");
        }
        return;
    }

    // Comando interno: exit
    if (strcmp(args[0], "exit") == 0) {
        printf("Saliendo del shell...\n");
        exit(0);
    }

    // Comando externo
    pid_t pid = fork();
    if (pid == 0) {
        // Redirección de entrada
        if (strlen(input_file) > 0) {
            int in = open(input_file, O_RDONLY);
            if (in < 0) { perror("Error archivo entrada"); exit(1); }
            dup2(in, STDIN_FILENO);
            close(in);
        }

        // Redirección de salida
        if (strlen(output_file) > 0) {
            int out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out < 0) { perror("Error archivo salida"); exit(1); }
            dup2(out, STDOUT_FILENO);
            close(out);
        }

        execvp(args[0], args);
        perror("Error al ejecutar");
        exit(1);
    } else {
        wait(NULL);
    }
}

// Ejecutar comandos conectados con pipes
void ejecutarPipes() {
    int i;
    int fd[2], prev_fd = -1;

    for (i = 0; commands[i] != NULL; i++) {
        pipe(fd);
        pid_t pid = fork();

        if (pid == 0) {
            if (prev_fd != -1) {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (commands[i + 1] != NULL) {
                dup2(fd[1], STDOUT_FILENO);
            }

            close(fd[0]);
            close(fd[1]);

            procesarRedirecciones(commands[i]);
            separarArgumentos(commands[i]);

            execvp(args[0], args);
            perror("Error al ejecutar");
            exit(1);
        } else {
            wait(NULL);
            close(fd[1]);
            if (prev_fd != -1) close(prev_fd);
            prev_fd = fd[0];
        }
    }
}

// Programa principal
int main() {
    system("clear");

    while (1) {
        mostrarPrompt();
        leerLinea();

        // Línea vacía
        if (strlen(input_line) == 0) continue;

        dividirPorPipes();

        if (num_pipes > 0) {
            ejecutarPipes();
        } else {
            ejecutarSimple(commands[0]);
        }
    }

    return 0;
}

