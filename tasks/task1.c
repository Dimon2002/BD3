/* Processed by ecpg (4.9.0) */
/* These include files are added by the preprocessor */
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
/* End of automatic include section */

#line 1 "./tasks/task1.ec"
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
   /* exec sql begin declare section */
       
       
   
#line 18 "./tasks/task1.ec"
 char password [ MAX_PASSWORD_LENGTH ] ;
 
#line 19 "./tasks/task1.ec"
 int countDetails ;
/* exec sql end declare section */
#line 20 "./tasks/task1.ec"


   strncpy(password, arguments[1], MAX_PASSWORD_LENGTH - 1);
   password[MAX_PASSWORD_LENGTH - 1] = '\0';

   // Подключение к БД
   { ECPGconnect(__LINE__, 0, "students@fpm2.ami.nstu.ru" , "pmi-b0409" , password , NULL, 0); }
#line 26 "./tasks/task1.ec"

   
   // Перезаписываем массив, чтобы уменьшить время нахождения пароля в памяти
   memset(password, 0, strlen(password));

   if (sqlca.sqlcode < 0)
   {
      printf("Ошибка при подключении: %s\n", sqlca.sqlerrm.sqlerrmc);
      exit(1);
   }

   { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "set SEARCH_PATH to pmib0409", ECPGt_EOIT, ECPGt_EORT);}
#line 37 "./tasks/task1.ec"


   if (sqlca.sqlcode < 0)
   {
      printf("Ошибка при установке схемы: %s\n", sqlca.sqlerrm.sqlerrmc);
      { ECPGdisconnect(__LINE__, "students");}
#line 42 "./tasks/task1.ec"

      exit(1);
   }

   { ECPGtrans(__LINE__, NULL, "begin work");}
#line 46 "./tasks/task1.ec"


   printf("Задание 1: \nВыдать число деталей, которые поставлялись поставщиками, выполнявшими поставки в Париж.\n");
   
   { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "select count ( distinct spj . n_det ) kol_det from spj where spj . n_post in ( select distinct spj . n_post from spj join j on j . n_izd = spj . n_izd where j . town = 'Париж' )", ECPGt_EOIT, 
	ECPGt_int,&(countDetails),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 58 "./tasks/task1.ec"


   if (sqlca.sqlcode < 0)
   {
      printf("Ошибка при выборке из БД: %s\n", sqlca.sqlerrm.sqlerrmc);
      { ECPGdisconnect(__LINE__, "students");}
#line 63 "./tasks/task1.ec"

      exit(1);
   }
   printf("Число деталей = %d\n", countDetails);

   { ECPGtrans(__LINE__, NULL, "commit work");}
#line 68 "./tasks/task1.ec"


   { ECPGdisconnect(__LINE__, "students");}
#line 70 "./tasks/task1.ec"


   if (sqlca.sqlcode == 0)
   {
      printf("Я отключился 1\n");
   }

   return EXIT_SUCCESS;
}