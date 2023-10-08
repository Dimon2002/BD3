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
      char n_post[7];
      int kol;
      double average_volume;
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
   printf("Задание 3: \nНайти поставщиков, имеющих поставки, объем которых не менее чем в 3 раза превышает средний объем поставки этого поставщика для изделий из Рима. Вывести номер поставщика, объем поставки, средний объем поставки поставщика для изделий из Рима.");
   EXEC SQL Declare cursor3 cursor for
   SELECT t.n_post, t.kol, res.average_volume
   into :n_post, :kol, :average_volume
   FROM spj AS t
   INNER JOIN (
        -- Детали поставок для изделия из Рима
        SELECT spj.n_post, AVG(spj.kol) AS average_volume
        FROM spj
        JOIN j ON j.n_izd = spj.n_izd
        WHERE j.town = 'Рим'
        GROUP BY spj.n_post
   ) AS res ON res.n_post = t.n_post
   WHERE t.kol >= 3 * res.average_volume;
   
   EXEC SQL OPEN cursor3;
   
   if (sqlca.sqlcode < 0)
   {
        error_msg("Ошибка при открытии курсора (OPEN).");
   }
   
   EXEC SQL FETCH cursor3;

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
        printf("\nНомер поставщика\tОбъем поставки\t\tСредний объем поставки\n%s\t\t%d\t\t%lf\n", n_post, kol, average_volume);
   }

   while(sqlca.sqlcode == 0)
   {
        EXEC SQL FETCH cursor3;
        if (sqlca.sqlcode < 0)
        {
            error_msg("Ошибка при чтении курсора (FETCH)");
            break;
        }

        if (sqlca.sqlcode == 0)
        {
            printf("%s\t\t%d\t\t%lf\n", n_post, kol, average_volume);
        }
   }

   printf("===================================\n");

    
   EXEC SQL CLOSE cursor3;
   EXEC SQL COMMIT WORK;

   EXEC SQL DISCONNECT students;

   if (sqlca.sqlcode == 0)
   {
      printf("Я отключился 5\n");
   }

   return EXIT_SUCCESS;
}