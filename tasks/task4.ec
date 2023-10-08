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
      char itemIds[7];
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
   printf("===================\n");
   printf("Задание 4: \nВыбрать изделия, для которых не делал поставок ни один из поставщиков, поставлявших красные детали.\n");
   
   EXEC SQL Declare cursor4 cursor for
   SELECT n_izd
   into :itemIds
   FROM j
   EXCEPT
   SELECT DISTINCT spj.n_izd
   FROM spj
   WHERE spj.n_post IN (
           -- Поставщики, поставлявшие красные изделия
           SELECT DISTINCT a.n_post
           FROM spj AS a
           JOIN p ON p.n_det = a.n_det
           WHERE p.cvet = 'Красный'
   );

   EXEC SQL OPEN cursor4;
   
   if (sqlca.sqlcode < 0)
   {
        error_msg("Ошибка при открытии курсора (OPEN).");
   }
   
   EXEC SQL FETCH cursor4;
   if (sqlca.sqlcode < 0)
   {
        error_msg("Ошибка при чтении курсора (FETCH)");
   }

   if (sqlca.sqlcode == 100)
   {
        printf("\nДанных не найдено\n");
   }
   else
   {
        printf("\nСписок изделий\n%s\n", itemIds);
   }

   while(sqlca.sqlcode == 0)
   {
        EXEC SQL FETCH cursor4;
        if (sqlca.sqlcode < 0)
        {
            error_msg("Ошибка при чтении курсора (FETCH)");
            break;
        }

        if (sqlca.sqlcode == 0)
        {
            printf("%s\n", itemIds);
        }
   }

   printf("===================\n");
    
   EXEC SQL CLOSE cursor4;
   EXEC SQL COMMIT WORK;

   EXEC SQL DISCONNECT students;

   if (sqlca.sqlcode == 0)
   {
      printf("Я отключился 4\n");
   }

   return EXIT_SUCCESS;
}