#include <sqlca.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_PASSWORD_LENGTH 20

error_msg(char *desc)
{
    printf("\n%s\nКод: %d\nОписание: %s\n", desc, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc);
    // откат транзакции
    EXEC SQL ROLLBACK WORK;
    // rollback сбрасывает search_path
    EXEC SQL SET SEARCH_PATH TO pmib0409;
}

int main(int numberOfArguments, char *arguments[])
{
    if (numberOfArguments < 2)
    {
        fprintf(stderr, "Arguments too few (The directory name is missing)\n");
        return EXIT_FAILURE;
    }

   // Секции обьявления главных переменных
    EXEC SQL BEGIN DECLARE SECTION;
        char password[MAX_PASSWORD_LENGTH];
    EXEC SQL END DECLARE SECTION;

    strncpy(password, arguments[1], MAX_PASSWORD_LENGTH - 1);
    password[MAX_PASSWORD_LENGTH - 1] = '\0';
    
    // Подключение к БД
    EXEC SQL CONNECT TO students@fpm2.ami.nstu.ru USER "pmi-b0409" USING :password;
   
    // Перезаписываем массив, чтобы уменьшить время нахождения пароля в памяти
    memset(password, 0, strlen(password));

    if (sqlca.sqlcode < 0)
    {
        printf("Ошибка при подключении: %s\n", sqlca.sqlerrm.sqlerrmc);
        exit(1);
    }

    EXEC SQL SET SEARCH_PATH TO pmib0409;

    if (sqlca.sqlcode < 0)
    {
        printf("Ошибка при установке схемы: %s\n", sqlca.sqlerrm.sqlerrmc);
        EXEC SQL DISCONNECT students;
        exit(1);
    }

    EXEC SQL BEGIN WORK;

    printf("Задание 2: \nПоменять местами цвета деталей из Рима и из Лондона, т. е. деталям из Рима установить цвет детали из Лондона, а деталям из Лондона установить цвет детали из Рима. Если цветов несколько, брать первый по алфавиту из этих цветов.\n");

    EXEC SQL UPDATE p set cvet = (
    CASE WHEN p.town = 'Лондон'
        THEN (-- Цвет из рима
            SELECT rp.cvet
            FROM p AS rp
            WHERE rp.town = 'Рим'
            ORDER BY rp.cvet 
            LIMIT 1
        )
        ELSE (-- Цвет из Лондона
            SELECT lp.cvet
            FROM p AS lp
            WHERE lp.town = 'Лондон'
            ORDER BY lp.cvet 
            LIMIT 1
        )
    END
    )
    WHERE p.town = 'Рим'
        OR p.town = 'Лондон';

    if (sqlca.sqlcode < 0)
    {
        error_msg("Ошибка при изменении данных (UPDATE).");
        return EXIT_FAILURE;
    }
    if (sqlca.sqlcode == 0)
    {
        printf("Количество обработанных записей = %d\n", sqlca.sqlerrd[2]);
    }

    EXEC SQL COMMIT WORK;
    
    EXEC SQL DISCONNECT students;

    if (sqlca.sqlcode == 0)
    {
        printf("Я отключился 2\n");
    }

    return EXIT_SUCCESS;
}