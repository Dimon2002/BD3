/* Processed by ecpg (4.9.0) */
/* These include files are added by the preprocessor */
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
/* End of automatic include section */

#line 1 "./tasks/task3.ec"
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
#line 12 "./tasks/task3.ec"

    // rollback сбрасывает search_path
    { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "set SEARCH_PATH to pmib0409", ECPGt_EOIT, ECPGt_EORT);}
#line 14 "./tasks/task3.ec"

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
       
       
       
       
   
#line 27 "./tasks/task3.ec"
 char password [ MAX_PASSWORD_LENGTH ] ;
 
#line 28 "./tasks/task3.ec"
 char n_post [ 7 ] ;
 
#line 29 "./tasks/task3.ec"
 int kol ;
 
#line 30 "./tasks/task3.ec"
 double average_volume ;
/* exec sql end declare section */
#line 31 "./tasks/task3.ec"


   strncpy(password, arguments[1], MAX_PASSWORD_LENGTH - 1);
   password[MAX_PASSWORD_LENGTH - 1] = '\0';

   // Подключение к БД
   { ECPGconnect(__LINE__, 0, "students@fpm2.ami.nstu.ru" , "pmi-b0409" , password , NULL, 0); }
#line 37 "./tasks/task3.ec"

   
   // Перезаписываем массив, чтобы уменьшить время нахождения пароля в памяти
   memset(password, 0, strlen(password));

   if (sqlca.sqlcode < 0)
   {
      printf("Ошибка при подключении: %s\n", sqlca.sqlerrm.sqlerrmc);
      exit(1);
   }

   { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "set SEARCH_PATH to pmib0409", ECPGt_EOIT, ECPGt_EORT);}
#line 48 "./tasks/task3.ec"


   if (sqlca.sqlcode < 0)
   {
      printf("Ошибка при установке схемы: %s\n", sqlca.sqlerrm.sqlerrmc);
      { ECPGdisconnect(__LINE__, "students");}
#line 53 "./tasks/task3.ec"

      exit(1);
   }

   { ECPGtrans(__LINE__, NULL, "begin work");}
#line 57 "./tasks/task3.ec"

   printf("===================================\n");
   printf("Задание 3: \nНайти поставщиков, имеющих поставки, объем которых не менее чем в 3 раза превышает средний объем поставки этого поставщика для изделий из Рима. Вывести номер поставщика, объем поставки, средний объем поставки поставщика для изделий из Рима.");
   ECPGset_var( 0, &( average_volume ), __LINE__);\
 ECPGset_var( 1, &( kol ), __LINE__);\
 ECPGset_var( 2, ( n_post ), __LINE__);\
 /* declare cursor3 cursor for select t . n_post , t . kol , res . average_volume from spj as t inner join ( select spj . n_post , AVG ( spj . kol ) as average_volume from spj join j on j . n_izd = spj . n_izd where j . town = 'Рим' group by spj . n_post ) as res on res . n_post = t . n_post where t . kol >= 3 * res . average_volume */
#line 72 "./tasks/task3.ec"

   
   { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "declare cursor3 cursor for select t . n_post , t . kol , res . average_volume from spj as t inner join ( select spj . n_post , AVG ( spj . kol ) as average_volume from spj join j on j . n_izd = spj . n_izd where j . town = 'Рим' group by spj . n_post ) as res on res . n_post = t . n_post where t . kol >= 3 * res . average_volume", ECPGt_EOIT, 
	ECPGt_char,(n_post),(long)7,(long)1,(7)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(kol),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_double,&(average_volume),(long)1,(long)1,sizeof(double), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 74 "./tasks/task3.ec"

   
   if (sqlca.sqlcode < 0)
   {
        error_msg("Ошибка при открытии курсора (OPEN).");
   }
   
   { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch cursor3", ECPGt_EOIT, 
	ECPGt_char,(n_post),(long)7,(long)1,(7)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(kol),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_double,&(average_volume),(long)1,(long)1,sizeof(double), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 81 "./tasks/task3.ec"


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
        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "fetch cursor3", ECPGt_EOIT, 
	ECPGt_char,(n_post),(long)7,(long)1,(7)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_int,&(kol),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_double,&(average_volume),(long)1,(long)1,sizeof(double), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 99 "./tasks/task3.ec"

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

    
   { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "close cursor3", ECPGt_EOIT, ECPGt_EORT);}
#line 115 "./tasks/task3.ec"

   { ECPGtrans(__LINE__, NULL, "commit work");}
#line 116 "./tasks/task3.ec"


   { ECPGdisconnect(__LINE__, "students");}
#line 118 "./tasks/task3.ec"


   if (sqlca.sqlcode == 0)
   {
      printf("Я отключился 5\n");
   }

   return EXIT_SUCCESS;
}