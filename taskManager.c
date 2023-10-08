#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/wait.h>
#include <stdio.h>

#define MAX_PASSWORD_LENGTH 20

size_t len = 0;

void GetPassword(char *password, size_t maxlen) 
{
    struct termios oldt, newt;
    int ch;

    // Получаем текущие настройки терминала
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    // Отключаем отображение вводимых символов
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); 

    printf("Enter password: ");

    // Читаем символы до Enter или до максимальной длины
    while ((ch = getchar()) != '\n' && len < maxlen - 1) {
        password[len++] = ch;
    }
    // Заканчиваем строку
    password[len] = '\0';

    // Возвращаем настройки терминала
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    printf("\n");
}

int main()
{
    char password[MAX_PASSWORD_LENGTH];
    char taskNumber;
    int status;
    GetPassword(password, sizeof(password));

    printf("'e' выйти из программы");
    printf("\nВыберите задани (1-5): ");

    while (scanf("%c", &taskNumber))
    {
        while ((getchar()) != '\n');  // Очистка буфера ввода

        switch (taskNumber)
        {
        case '1':

            printf("\nЗапускаю другую программу! (1)\n");

            if (fork() == 0)
            {
                execl("./compiled_tasks/task1","task1", password, NULL);
                exit(EXIT_FAILURE);
            }

            wait(&status);

            break;
        case '2':

            printf("\nЗапускаю другую программу! (2)\n");
            
            if (fork() == 0)
            {
                execl("./compiled_tasks/task2","task2", password, NULL);
                exit(EXIT_FAILURE);
            }
            
            wait(&status);

            break;
        case '3':

            printf("\nЗапускаю другую программу! (3)\n");            
            
            if (fork() == 0)
            {
                execl("./compiled_tasks/task3","task3", password, NULL);
                exit(EXIT_FAILURE);
            }
            
            wait(&status);

            break;
        case '4':

            printf("\nЗапускаю другую программу! (4)\n");
            
            if (fork() == 0)
            {
                execl("./compiled_tasks/task4","task4", password, NULL);
                exit(EXIT_FAILURE);
            }
            
            wait(&status);

            break;
        case '5':
            printf("\nЗапускаю другую программу! (5)\n");
                        
            if (fork() == 0)
            {
                execl("./compiled_tasks/task5","task5", password, NULL);
                exit(EXIT_FAILURE);
            }
            
            wait(&status);

            break;
        case 'e':
            return EXIT_SUCCESS;
        default:
            printf("Дебил нет? Сказали же от 1 до 5\n");
            break;
        }
            printf("\n'e' выйти из программы");
            printf("\nВыберите задани (1-5): ");
    }

    return EXIT_SUCCESS;
}