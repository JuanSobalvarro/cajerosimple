# Cajero Automático en C

Este proyecto implementa un simulador de cajero automático en lenguaje C. A continuación se explican las distintas partes del código.

Todos los pines son 1234.

## Librerías Importadas
```c
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
```
- stdio.h: Provee funciones para realizar operaciones de entrada y salida (printf, scanf, etc.).
- stdlib.h: Provee funciones para la gestión de memoria dinámica (malloc, free, realloc) y otras utilidades.
- conio.h: Provee funciones para la entrada y salida en la consola (getch).
- time.h: Provee funciones para trabajar con fechas y horas.

## Definiciones y Variables Globales

```c
#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

typedef const struct tm local;

const int BILLVALUES[] = {1000, 500, 200, 100};
int CUENTAS[] = {1000, 2000, 3000};
```
- CLEAR: Define el comando para limpiar la consola dependiendo del sistema operativo.
- local: Define una estructura constante para manejar el tiempo local.
- BILLVALUES: Array de valores de los billetes disponibles.
- CUENTAS: Array con los saldos iniciales de tres cuentas diferentes.

## Prototipos de Funciones
```c
void key2Exit();
int initCajero();
int printMenu();
void saveMovement(int cuentaSelected, int amount, char **movimientos, int *movNumber, char *movementType);
void consulta(int cuentaSelect, char **movimientos, int *movnum, int *billQuantity);
void deposito(int cuentaSelect, char **movimientos, int *movnum, int *billQuantity);
void retiro(int cuentaSelect, char **movimientos, int *movnum, int *billQuantity);
void movimientos(int cuentaSelect, char **movimientos, int *movnum, int *billQuantity);
void callAction(int selection, int cuentaSelect, char **mov, int *movnum, int *billQuantity);
```
Aquí se declaran las funciones utilizadas en el programa.
## Función Principal
```c
Copy code
int main()
{
    int billQuantity[] = {5, 5, 5, 5};
    char **movimientos;
    int movnum = 0;
    int cuentaSelect = -1;

    movimientos = realloc(NULL, sizeof(*movimientos) * movnum);
    if (!movimientos)
    {
        printf("COULDNT REALLOCATE MEMORY UWUNYA\n");
        return -1;
    }

    if((cuentaSelect = initCajero()) == -1)
    {
        return 0;
    }

    int selection;
    while ((selection = printMenu()) != 5)
    {   
        callAction(selection, cuentaSelect, movimientos, &movnum, billQuantity);
    }

    while (movimientos)
    {
        free(*(movimientos++));
    }
    free(movimientos);

    return 0;
}
```
- main:
Inicializa las cantidades de billetes, los movimientos y otras variables.
Realiza la inicialización del cajero (initCajero).
Muestra un menú en un bucle mientras no se seleccione salir.
Libera la memoria al final.
Otras Funciones
### key2Exit
```c
void key2Exit()
{
    printf("\nPresione cualquier tecla para continuar\n");
    getchar();
    getchar();
}
```
Espera a que el usuario presione una tecla para continuar.
### initCajero
```c
int initCajero()
{
    int cuentaSelected;
    printf("===================Cajero Automatico===================\n");
    printf("Ingrese su numero de cuenta(1-3): ");
    scanf("%d", &cuentaSelected);
    if (cuentaSelected < 1 || cuentaSelected > 3)
    {
        printf("Cuenta no valida\n");
        return -1;
    }
    cuentaSelected--;
    getchar();

    int equal;
    int countpass = 0;
    do
    {
        equal = 1;
        char pin[5] = "1234";
        char check[5];
        printf("\nIngrese pin de la cuenta: ");
        for (int i = 0; i < 4; i++)
        {  
            check[i] = getch();
            putchar('*');
            if (check[i] != pin[i])
            {
                equal = 0;
            }
        }
        check[4] = '\0';
        if(!equal)
        {
            countpass++;
            if (countpass < 3)
            {
                printf("\nPin incorrecto. Intente otra vez.\n");
            }
            else
            {
                printf("\nPor seguridad la cuenta ha sido bloqueada\n");
                return -1;
            }
        }
        printf("\n");
    } while (!equal);

    return cuentaSelected;
}
```
- initCajero:
Solicita al usuario el número de cuenta.
Si el número de cuenta es inválido, retorna -1.
Pide al usuario ingresar un PIN. Si el PIN es incorrecto tres veces, bloquea la cuenta y retorna -1.
Si el PIN es correcto, retorna el número de cuenta seleccionada.
Implementación del PIN con *
La parte del código donde se implementa el ingreso del PIN y se muestra * en lugar de los caracteres reales es esta:

```c
do
{
    equal = 1;
    char pin[5] = "1234";
    char check[5];
    printf("\nIngrese pin de la cuenta: ");
    for (int i = 0; i < 4; i++)
    {  
        check[i] = getch();
        putchar('*');
        if (check[i] != pin[i])
        {
            equal = 0;
        }
    }
    check[4] = '\0';
    if(!equal)
    {
        countpass++;
        if (countpass < 3)
        {
            printf("\nPin incorrecto. Intente otra vez.\n");
        }
        else
        {
            printf("\nPor seguridad la cuenta ha sido bloqueada\n");
            return -1;
        }
    }
    printf("\n");
} while (!equal);
```
- getch(): Función de conio.h que captura un carácter del teclado sin mostrarlo en la consola.
- putchar('*'): Imprime un asterisco * en lugar del carácter ingresado.
= equal: Variable que indica si el PIN ingresado es correcto.
- countpass: Cuenta la cantidad de intentos fallidos. Después de tres intentos fallidos, bloquea la cuenta.
- Este código solicita al usuario ingresar un PIN de cuatro dígitos, mostrando * por cada carácter ingresado, y compara el PIN ingresado con el PIN almacenado (en este caso, "1234"). Si el PIN es incorrecto, permite hasta tres intentos antes de bloquear la cuenta por seguridad.