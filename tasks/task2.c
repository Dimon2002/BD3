/* Processed by ecpg (4.9.0) */
/* These include files are added by the preprocessor */
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
/* End of automatic include section */

#line 1 "./tasks/task2.ec"
#include <sqlca.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_PASSWORD_LENGTH 20

error_msg(char *desc)
{
    printf("\n%s\nКод: %d\nОписание: %s\n", desc, sqlca.sqlcode, sqlca.sqlerrm.sqlerrmc);
    // откат транзакции
    { ECPGtrans(__LINE__, NULL, "rollback work");}
#line 12 "./tasks/task2.ec"

    // rollback сбрасывает search_path
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "set SEARCH_PATH to pmib0409", ECPGt_EOIT, ECPGt_EORT);}
#line 14 "./tasks/task2.ec"

}

int main(int numberOfArguments, char *arguments[])
{
    if (numberOfArguments < 2)
    {
        fprintf(stderr, "Arguments too few (The directory name is missing)\n");
        return EXIT_FAILURE;
    }

   // Секции обьявления главных переменных
    /* exec sql begin declare section */
         
    
#line 27 "./tasks/task2.ec"
 char password [ MAX_PASSWORD_LENGTH ] ;
/* exec sql end declare section */
#line 28 "./tasks/task2.ec"


    strncpy(password, arguments[1], MAX_PASSWORD_LENGTH - 1);
    password[MAX_PASSWORD_LENGTH - 1] = '\0';
    
    // Подключение к БД
    { ECPGconnect(__LINE__, 0, "students@fpm2.ami.nstu.ru" , "pmi-b0409" , password , NULL, 0); }
#line 34 "./tasks/task2.ec"

   
    // Перезаписываем массив, чтобы уменьшить время нахождения пароля в памяти
    memset(password, 0, strlen(password));

    if (sqlca.sqlcode < 0)
    {
        printf("Ошибка при подключении: %s\n", sqlca.sqlerrm.sqlerrmc);
        exit(1);
    }

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "set SEARCH_PATH to pmib0409", ECPGt_EOIT, ECPGt_EORT);}
#line 45 "./tasks/task2.ec"


    if (sqlca.sqlcode < 0)
    {
        printf("Ошибка при установке схемы: %s\n", sqlca.sqlerrm.sqlerrmc);
        { ECPGdisconnect(__LINE__, "students");}
#line 50 "./tasks/task2.ec"

        exit(1);
    }

    { ECPGtrans(__LINE__, NULL, "begin work");}
#line 54 "./tasks/task2.ec"


    printf("Задание 2: \nПоменять местами цвета деталей из Рима и из Лондона, т. е. деталям из Рима установить цвет детали из Лондона, а деталям из Лондона установить цвет детали из Рима. Если цветов несколько, брать первый по алфавиту из этих цветов.\n");

    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "update p set cvet = ( case when p . town = 'Лондон' then ( select rp . cvet from p as rp where rp . town = 'Рим' order by rp . cvet limit 1 ) else ( select lp . cvet from p as lp where lp . town = 'Лондон' order by lp . cvet limit 1 ) end ) where p . town = 'Рим' or p . town = 'Лондон'", ECPGt_EOIT, ECPGt_EORT);}
#line 77 "./tasks/task2.ec"


    if (sqlca.sqlcode < 0)
    {
        error_msg("Ошибка при изменении данных (UPDATE).");
        return EXIT_FAILURE;
    }
    if (sqlca.sqlcode == 0)
    {
        printf("Количество обработанных записей = %d\n", sqlca.sqlerrd[2]);
    }

    { ECPGtrans(__LINE__, NULL, "commit work");}
#line 89 "./tasks/task2.ec"

    
    { ECPGdisconnect(__LINE__, "students");}
#line 91 "./tasks/task2.ec"


    if (sqlca.sqlcode == 0)
    {
        printf("Я отключился 2\n");
    }

    return EXIT_SUCCESS;
}