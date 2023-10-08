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
      char n_izd[7], name[21], town[21];
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
   printf("===================================\n");
   printf("Задание 5: \nВыдать полную информацию об изделиях, для которых поставлялись ТОЛЬКО детали с весом больше 12.");

   EXEC SQL Declare cursor5 cursor for
   SELECT DISTINCT j.*
   into :n_izd, :name, :town
   FROM spj
   JOIN j ON j.n_izd = spj.n_izd
   WHERE spj.n_det IN (
        SELECT p.n_det
        FROM p
        WHERE p.ves > 12
   )
   EXCEPT
   SELECT DISTINCT j.*
   FROM spj
   JOIN j ON j.n_izd = spj.n_izd
   WHERE NOT spj.n_det IN (
       SELECT p.n_det
       FROM p
       WHERE p.ves > 12
   );   

   EXEC SQL OPEN cursor5;
   
   if (sqlca.sqlcode < 0)
   {
        error_msg("Ошибка при открытии курсора (OPEN).");
   }
   
   EXEC SQL FETCH cursor5;

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
        printf("\nНомер изделия\tИмя\t\tГород\n%s\t%s\t\t%s\n", n_izd, name, town);
   }

   while(sqlca.sqlcode == 0)
   {
        EXEC SQL FETCH cursor5;
        if (sqlca.sqlcode < 0)
        {
            error_msg("Ошибка при чтении курсора (FETCH)");
            break;
        }

        if (sqlca.sqlcode == 0)
        {
            printf("%s\t%s\t\t%s\n", n_izd, name, town);
        }
   }

   printf("===================================\n");

    
   EXEC SQL CLOSE cursor5;
   EXEC SQL COMMIT WORK;

   EXEC SQL DISCONNECT students;

   if (sqlca.sqlcode == 0)
   {
      printf("Я отключился 5\n");
   }

   return EXIT_SUCCESS;
}