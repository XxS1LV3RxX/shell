# Mi Shell Personalizada para Linux en C

Este es un proyecto que simula el comportamiento de una shell de comandos de Linux, desarrollado para demostrar el entendimiento de conceptos clave de Sistemas Operativos.

**Importante:** Este proyecto está diseñado para ser compilado y ejecutado exclusivamente en sistemas operativos tipo UNIX (Linux, macOS).

## Tecnologías
*   **Lenguaje:** C
*   **APIs Principales:** POSIX (`fork`, `execvp`, `pipe`, `dup2`, `wait`)
*   **Compilador:** GCC
*   **Build System:** Make

## Funcionalidades Implementadas
*   Ejecución de comandos externos con sus argumentos (ej. `ls -l`).
*   Comandos internos personalizados: `cd` y `exit`.
*   Redirección de salida estándar (`>`).
*   Redirección de entrada estándar (`<`).
*   Comunicación inter-proceso mediante pipes (`|`).

## ¿Cómo Compilar y Ejecutar?

Sigue estos pasos en tu terminal:

1.  **Clona el repositorio:**
    ```sh
    git clone https://github.com/XxS1LV3RxX/Shell.git
    ```
2.  **Navega a la carpeta del proyecto:**
    ```sh
    cd shell
    ```
3.  **Compila el proyecto usando el Makefile:**
    ```sh
    make
    ```
4.  **Ejecuta la shell:**
    ```sh
    ./shell
    ```