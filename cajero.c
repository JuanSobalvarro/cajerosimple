#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

// define for different SO, the code needs an implementation for replace conio.h, so it cannot be used for linux
#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

typedef const struct tm local;

const int BILLVALUES[] = {1000, 500, 200, 100};
int CUENTAS[] = {1000, 2000, 3000};

void key2Exit();
int initCajero();
int printMenu();
void saveMovement(int cuentaSelected, int amount, char **movimientos, int *movNumber, char *movementType);
void consulta(int cuentaSelect, char **movimientos, int *movnum, int *billQuantity);
void deposito(int cuentaSelect, char **movimientos, int *movnum, int *billQuantity);
void retiro(int cuentaSelect, char **movimientos, int *movnum, int *billQuantity);
void movimientos(int cuentaSelect, char **movimientos, int *movnum, int *billQuantity);
void callAction(int selection, int cuentaSelect, char **mov, int *movnum, int *billQuantity);

int main()
{
    int billQuantity[] = {5, 5, 5, 5};
    char **movimientos; // format: DD.MM.YY HH.MM.SS IDCUENTA MOVEMENTTYPE QUANTITY
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

    // free MEMORY
    while (movimientos)
    {
        free(*(movimientos++));
    }
    free(movimientos);

    return 0;
}

void key2Exit()
{
    printf("\nPresione cualquier tecla para continuar\n");
    getchar();
    getchar();
}

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

int printMenu()
{
    system(CLEAR);
    int select;
    printf("===================Cajero Automatico===================\n");
    printf("1. Consulta\n2. Deposito\n3. Retiro\n4. Movimientos\n5. Salir\n");
    printf("=======================================================\n");

    scanf("%d", &select);

    return select - 1;
}

void saveMovement(int cuentaSelected, int amount, char **movimientos, int *movNumber, char *movementType)
{
    time_t raw = time(NULL);
    local *loc = localtime(&raw);
    char *mov;
    mov = malloc(sizeof(char) * 100);
    int check = snprintf(mov, 100, "%d.%d.%d %d:%d:%d AccountID: %d %s %d", 
                            loc->tm_mday, loc->tm_mon + 1, loc->tm_year + 1900, 
                            loc->tm_hour, loc->tm_min, loc->tm_sec, 
                            cuentaSelected, movementType, amount);
    
    if (check <= 0)
    {
        printf("ERROR creating string movement");
        return;
    }

    mov = realloc(mov, sizeof(char) * check);

    movimientos[*movNumber] = mov;
    *movNumber += 1;
    movimientos = realloc(movimientos, sizeof(*movimientos) * (*movNumber));


    if (!movimientos)
    {
        printf("COULDNT REALLOCATE MEMORY UWUNYA\n");
    }
}

void consulta(int cuentaSelect, char **movimientos, int *movnum, int *billQuantity)
{
    system(CLEAR);
    printf("===================Cajero Automatico===================\n");
    printf("CONSULTA\n");
    printf("Su saldo actual es de: %d\n", CUENTAS[cuentaSelect]);
    printf("=======================================================\n");

    key2Exit();
}

void deposito(int cuentaSelect, char **movimientos, int *movnum, int *billQuantity)
{
    system(CLEAR);
    int deposit;
    printf("===================Cajero Automatico===================\n");
    printf("DEPOSITO\n");
    printf("Cuanto desea depositar?\n");
    scanf("%d", &deposit);

    if (deposit%100 != 0)
    {
        printf("\nNumero ingresado no valido\n");
        key2Exit();
        return;
    }
    CUENTAS[cuentaSelect] += deposit;
    printf("Depositado correctamente %d\n", deposit);
    printf("Saldo actual: %d\n", CUENTAS[cuentaSelect]);

    saveMovement(cuentaSelect, deposit, movimientos, movnum, "Deposito");
    key2Exit();
}

void retiro(int cuentaSelect, char **movimientos, int *movnum, int *billQuantity)
{
    
    system(CLEAR);
    int retire;
    printf("===================Cajero Automatico===================\n");
    printf("RETIRO\n");
    printf("Cuanto desea retirar(multiplos de 100)?\n");
    scanf("%d", &retire);

    //printf("\ntotal%d\n", retire);

    //float total = 0;
    //int retire2 = retire;
    if (CUENTAS[cuentaSelect] - retire < 0)
    {
        printf("\nFONDOS NO SUFICIENTES\n");
        return; 
    }

    if (retire%100 != 0)
    {
        printf("\nNumero ingresado no valido\n");
        return;
    } 

    int retireBill[] = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++)
    {
        retireBill[i] = (int)retire/BILLVALUES[i];
        retire -= retireBill[i] * BILLVALUES[i];
        //printf("\nretirando:%d de %d", retireBill[i], BILLVALUES[i]);
        if (retireBill[i] > billQuantity[i])
        {
            printf("\nNo hay billetes suficientes en el cajero para la transaccion\n");
            return;
        }
    }
    int total_retirado = 0;
    for (int i = 0; i < 4; i++)
    {
        int sum = retireBill[i] * BILLVALUES[i];
        printf("Billetes de %d: %d\n", BILLVALUES[i], retireBill[i]);
        billQuantity[i] -= retireBill[i];
        //printf("\nVariable sum: %d de tama;o %d\n", sum, sizeof(int));
        total_retirado += sum;
    }
    CUENTAS[cuentaSelect] -= total_retirado;
    printf("\nSe retiraron exitosamente %d\n", total_retirado);
    printf("Saldo actual: %d\n", CUENTAS[cuentaSelect]);

    saveMovement(cuentaSelect, total_retirado, movimientos, movnum, "Retiro");

    key2Exit();
}

void movimientos(int cuentaSelect, char **movimientos, int *movnum, int *billQuantity)
{
    system(CLEAR);
    printf("===================Cajero Automatico===================\n");
    printf("===============HISTORIAL DE MOVIMIENTOS================\n\n");

    char **head = movimientos;
    for (int i = 0; i < *movnum; i++)
    {
        //printf("adress %p: ", head);
        printf("MOV %d %s\n", i+1, head[i]);
    }

    key2Exit();
}

void callAction(int selection, int cuentaSelect, char **mov, int *movnum, int *billQuantity)
{
    void (*functionsPtr[])(int, char**, int*, int*) = {consulta, deposito, retiro, movimientos}; 

    functionsPtr[selection](cuentaSelect, mov, movnum, billQuantity);
}