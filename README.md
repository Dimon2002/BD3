# BD3
1. Для начала работы необходимо скопилировать все задачи (ESQL/C запросы по заданию варианта 5)
с помощью bash-скрипта compile_tasks.sh следующим образом: ./compile_tasks.sh
3. Далее необходимо скопилировать taskManager.c с помощью makefile следующим образом: make.
Результатом будет исполняемый файл main, который следует запустить так: ./main
5. После запуска taskManager пользователю предлагается ввести номер задания котоырй он бы хотел выполнить (согласно варианту 5),
в зависимости от выбора пользователя будет запущена программа, выполняющяя i-ое задание, порожденная в дочернем процессе.
Пользователь может завершить работу программы, нажав 'e'.
---
### Вариант 5.
1. Выдать число деталей, которые поставлялись поставщиками, выполнявшими поставки в Париж. 
2. Поменять местами цвета деталей из Рима и из Лондона,
т. е. деталям из Рима установить цвет детали из Лондона, а деталям из Лондона установить цвет детали из Рима.
Если цветов несколько, брать первый по алфавиту из этих цветов. 
4. Найти поставщиков, имеющих поставки, объем которых не менее чем в 3 раза превышает средний объем поставки этого поставщика
для изделий из Рима. Вывести номер поставщика, объем поставки, средний объем поставки поставщика для изделий из Рима. 
5. Выбрать изделия, для которых не делал поставок ни один из поставщиков, поставлявших красные детали. 
6. Выдать полную информацию об изделиях, для которых поставлялись ТОЛЬКО детали с весом больше 12. 
