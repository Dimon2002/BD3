/* Processed by ecpg (4.9.0) */
/* These include files are added by the preprocessor */
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
/* End of automatic include section */

#line 1 "./tasks/task5.ec"
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
#line 12 "./tasks/task5.ec"

    // rollback сбрасывает search_path
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "set SEARCH_PATH to pmib0409", ECPGt_EOIT, ECPGt_EORT);}
#line 14 "./tasks/task5.ec"

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
       
         
   
#line 27 "./tasks/task5.ec"
 char password [ MAX_PASSWORD_LENGTH ] ;
 
#line 28 "./tasks/task5.ec"
 char n_izd [ 7 ] , name [ 21 ] , town [ 21 ] ;
/* exec sql end declare section */
#line 29 "./tasks/task5.ec"


   strncpy(password, arguments[1], MAX_PASSWORD_LENGTH - 1);
   password[MAX_PASSWORD_LENGTH - 1] = '\0';

   // Подключение к БД
   { ECPGconnect(__LINE__, 0, "students@fpm2.ami.nstu.ru" , "pmi-b0409" , password , NULL, 0); }
#line 35 "./tasks/task5.ec"

   
   // Перезаписываем массив, чтобы уменьшить время нахождения пароля в памяти
   memset(password, 0, strlen(password));

   if (sqlca.sqlcode < 0)
   {
      printf("Ошибка при подключении: %s\n", sqlca.sqlerrm.sqlerrmc);
      exit(1);
   }

   { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "set SEARCH_PATH to pmib0409", ECPGt_EOIT, ECPGt_EORT);}
#line 46 "./tasks/task5.ec"


   if (sqlca.sqlcode < 0)
   {
      printf("Ошибка при установке схемы: %s\n", sqlca.sqlerrm.sqlerrmc);
      { ECPGdisconnect(__LINE__, "students");}
#line 51 "./tasks/task5.ec"

      exit(1);
   }

   { ECPGtrans(__LINE__, NULL, "begin work");}
#line 55 "./tasks/task5.ec"

   printf("===================================\n");
   printf("Задание 5: \nВыдать полную информацию об изделиях, для которых поставлялись ТОЛЬКО детали с весом больше 12.");

   ECPGset_var( 0, ( town ), __LINE__);\
 ECPGset_var( 1, ( name ), __LINE__);\
 ECPGset_var( 2, ( n_izd ), __LINE__);\
 /* declare cursor5 cursor for select distinct j . * from spj join j on j . n_izd = spj . n_izd where spj . n_det in ( select p . n_det from p where p . ves > 12 ) except select distinct j . * from spj join j on j . n_izd = spj . n_izd where not spj . n_det in ( select p . n_det from p where p . ves > 12 ) */
#line 77 "./tasks/task5.ec"
   

   { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "declare cursor5 cursor for select distinct j . * from spj join j on j . n_izd = spj . n_izd where spj . n_det in ( select p . n_det from p where p . ves > 12 ) except select distinct j . * from spj join j on j . n_izd = spj . n_izd where not spj . n_det in ( select p . n_det from p where p . ves > 12 )", ECPGt_EOIT, 
	ECPGt_char,(n_izd),(long)7,(long)1,(7)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(name),(long)21,(long)1,(21)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(town),(long)21,(long)1,(21)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 79 "./tasks/task5.ec"

   
   if (sqlca.sqlcode < 0)
   {
        error_msg("Ошибка при открытии курсора (OPEN).");
   }
   
   { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch cursor5", ECPGt_EOIT, 
	ECPGt_char,(n_izd),(long)7,(long)1,(7)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(name),(long)21,(long)1,(21)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(town),(long)21,(long)1,(21)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 86 "./tasks/task5.ec"


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
        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch cursor5", ECPGt_EOIT, 
	ECPGt_char,(n_izd),(long)7,(long)1,(7)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(name),(long)21,(long)1,(21)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(town),(long)21,(long)1,(21)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 104 "./tasks/task5.ec"

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

    
   { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close cursor5", ECPGt_EOIT, ECPGt_EORT);}
#line 120 "./tasks/task5.ec"

   { ECPGtrans(__LINE__, NULL, "commit work");}
#line 121 "./tasks/task5.ec"


   { ECPGdisconnect(__LINE__, "students");}
#line 123 "./tasks/task5.ec"


   if (sqlca.sqlcode == 0)
   {
      printf("Я отключился 5\n");
   }

   return EXIT_SUCCESS;
}