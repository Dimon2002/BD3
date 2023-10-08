/* Processed by ecpg (4.9.0) */
/* These include files are added by the preprocessor */
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
/* End of automatic include section */

#line 1 "./tasks/task4.ec"
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
#line 12 "./tasks/task4.ec"

    // rollback сбрасывает search_path
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "set SEARCH_PATH to pmib0409", ECPGt_EOIT, ECPGt_EORT);}
#line 14 "./tasks/task4.ec"

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
       
       
   
#line 27 "./tasks/task4.ec"
 char password [ MAX_PASSWORD_LENGTH ] ;
 
#line 28 "./tasks/task4.ec"
 char itemIds [ 7 ] ;
/* exec sql end declare section */
#line 29 "./tasks/task4.ec"


   strncpy(password, arguments[1], MAX_PASSWORD_LENGTH - 1);
   password[MAX_PASSWORD_LENGTH - 1] = '\0';

   // Подключение к БД
   { ECPGconnect(__LINE__, 0, "students@fpm2.ami.nstu.ru" , "pmi-b0409" , password , NULL, 0); }
#line 35 "./tasks/task4.ec"

   
   // Перезаписываем массив, чтобы уменьшить время нахождения пароля в памяти
   memset(password, 0, strlen(password));

   if (sqlca.sqlcode < 0)
   {
      printf("Ошибка при подключении: %s\n", sqlca.sqlerrm.sqlerrmc);
      exit(1);
   }

   { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "set SEARCH_PATH to pmib0409", ECPGt_EOIT, ECPGt_EORT);}
#line 46 "./tasks/task4.ec"


   if (sqlca.sqlcode < 0)
   {
      printf("Ошибка при установке схемы: %s\n", sqlca.sqlerrm.sqlerrmc);
      { ECPGdisconnect(__LINE__, "students");}
#line 51 "./tasks/task4.ec"

      exit(1);
   }

   { ECPGtrans(__LINE__, NULL, "begin work");}
#line 55 "./tasks/task4.ec"

   printf("===================\n");
   printf("Задание 4: \nВыбрать изделия, для которых не делал поставок ни один из поставщиков, поставлявших красные детали.\n");
   
   ECPGset_var( 0, ( itemIds ), __LINE__);\
 /* declare cursor4 cursor for select n_izd from j except select distinct spj . n_izd from spj where spj . n_post in ( select distinct a . n_post from spj as a join p on p . n_det = a . n_det where p . cvet = 'Красный' ) */
#line 72 "./tasks/task4.ec"


   { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "declare cursor4 cursor for select n_izd from j except select distinct spj . n_izd from spj where spj . n_post in ( select distinct a . n_post from spj as a join p on p . n_det = a . n_det where p . cvet = 'Красный' )", ECPGt_EOIT, 
	ECPGt_char,(itemIds),(long)7,(long)1,(7)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 74 "./tasks/task4.ec"

   
   if (sqlca.sqlcode < 0)
   {
        error_msg("Ошибка при открытии курсора (OPEN).");
   }
   
   { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch cursor4", ECPGt_EOIT, 
	ECPGt_char,(itemIds),(long)7,(long)1,(7)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 81 "./tasks/task4.ec"

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
        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch cursor4", ECPGt_EOIT, 
	ECPGt_char,(itemIds),(long)7,(long)1,(7)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 98 "./tasks/task4.ec"

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
    
   { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close cursor4", ECPGt_EOIT, ECPGt_EORT);}
#line 113 "./tasks/task4.ec"

   { ECPGtrans(__LINE__, NULL, "commit work");}
#line 114 "./tasks/task4.ec"


   { ECPGdisconnect(__LINE__, "students");}
#line 116 "./tasks/task4.ec"


   if (sqlca.sqlcode == 0)
   {
      printf("Я отключился 4\n");
   }

   return EXIT_SUCCESS;
}