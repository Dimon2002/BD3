#include <sqlca.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_PASSWORD_LENGTH 20

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
      int countDetails;
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

   printf("Задание 1: \nВыдать число деталей, которые поставлялись поставщиками, выполнявшими поставки в Париж.\n");
   
   EXEC SQL SELECT COUNT(DISTINCT spj.n_det) kol_det
   into :countDetails
   FROM spj
   WHERE spj.n_post IN (
      SELECT DISTINCT spj.n_post
      FROM spj
      JOIN j ON j.n_izd = spj.n_izd
      WHERE j.town = 'Париж'
   );

   if (sqlca.sqlcode < 0)
   {
      printf("Ошибка при выборке из БД: %s\n", sqlca.sqlerrm.sqlerrmc);
      EXEC SQL DISCONNECT students;
      exit(1);
   }
   printf("Число деталей = %d\n", countDetails);

   EXEC SQL COMMIT WORK;

   EXEC SQL DISCONNECT students;

   if (sqlca.sqlcode == 0)
   {
      printf("Я отключился 1\n");
   }

   return EXIT_SUCCESS;
}