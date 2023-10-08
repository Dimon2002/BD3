-- Создание таблиц --
CREATE TABLE pmib0409.s( n_post character(6) PRIMARY KEY,
                         name character(20),
                         reiting integer,
                         town character(20)
) WITH OIDS;


CREATE TABLE pmib0409.p( n_post character(6) PRIMARY KEY,
                         name character(20),
                         cvet character(20),
                         ves integer,
                         town character(20)
) WITH OIDS;

CREATE TABLE pmib0409.j( n_izd character(6) PRIMARY KEY,
                         name character(20),
                         town character(20)
) WITH OIDS;

CREATE TABLE pmib0409.spj( n_post character(6),
                           n_det character(6),
                           n_izd character(6),
                           kol integer,
                           date_post date
) WITH OIDS;

-- Занесение данных в созданные таблицы --
INSERT INTO pmib0409.s
VALUES ('S1', 'Смит ', 20, 'Лондон'),
       ('S2', 'Джонс', 10, 'Париж '),
       ('S3', 'Блейк', 30, 'Париж '),
       ('S4', 'Кларк', 20, 'Лондон'),
       ('S5', 'Адамс', 30, 'Афины ');

INSERT INTO pmib0409.p
VALUES ('P1', 'Гайка', 'Красный', 12, 'Лондон'),
       ('P2', 'Болт', 'Зеленый', 17, 'Париж '),
       ('P3', 'Винт', 'Голубой', 17, 'Рим'),
       ('P4', 'Винт', 'Красный', 14, 'Лондон'),
       ('P5', 'Кулачок', 'Голубой', 12, 'Париж '),
       ('P6', 'Блюм', 'Красный', 19, 'Лондон');

INSERT INTO pmib0409.j
VALUES ('J1', 'Жесткий диск', 'Париж'),
       ('J2', 'Перфоратор', 'Рим'),
       ('J3', 'Считыватель', 'Афины'),
       ('J4', 'Принтер', 'Афины'),
       ('J5', 'Флоппи-диск', 'Лондон'),
       ('J6', 'Терминал', 'Осло'),
       ('J7', 'Лента', 'Лондон');

INSERT INTO pmib0409.spj
VALUES ('S1','P1','J1',200,to_date('09/01/23', 'mm/dd/yy')),
       ('S1','P1','J4',700,to_date('09/02/23', 'mm/dd/yy')),
       ('S2','P3','J1',400,to_date('09/03/23', 'mm/dd/yy')),
       ('S2','P3','J2',200,to_date('09/04/23', 'mm/dd/yy')),
       ('S2','P3','J3',200,to_date('09/05/23', 'mm/dd/yy')),
       ('S2','P3','J4',500,to_date('09/06/23', 'mm/dd/yy')),
       ('S2','P3','J5',600,to_date('09/07/23', 'mm/dd/yy')),
       ('S2','P3','J6',400,to_date('09/08/23', 'mm/dd/yy')),
       ('S2','P3','J7',800,to_date('09/09/23', 'mm/dd/yy')),
       ('S2','P5','J2',100,to_date('09/10/23', 'mm/dd/yy')),
       ('S3','P3','J1',200,to_date('09/11/23', 'mm/dd/yy')),
       ('S3','P4','J2',500,to_date('09/12/23', 'mm/dd/yy')),
       ('S4','P6','J3',300,to_date('10/01/23', 'mm/dd/yy')),
       ('S4','P6','J7',300,to_date('10/02/23', 'mm/dd/yy')),
       ('S5','P2','J2',200,to_date('10/03/23', 'mm/dd/yy')),
       ('S5','P2','J4',100,to_date('10/04/23', 'mm/dd/yy')),
       ('S5','P5','J5',500,to_date('10/05/23', 'mm/dd/yy')),
       ('S5','P5','J7',100,to_date('10/06/23', 'mm/dd/yy')),
       ('S5','P6','J2',200,to_date('10/07/23', 'mm/dd/yy')),
       ('S5','P1','J4',100,to_date('10/08/23', 'mm/dd/yy')),
       ('S5','P3','J4',200,to_date('10/09/23', 'mm/dd/yy')),
       ('S5','P4','J4',800,to_date('10/10/23', 'mm/dd/yy')),
       ('S5','P5','J4',400,to_date('10/11/23', 'mm/dd/yy')),
       ('S5','P6','J4',500,to_date('10/12/23', 'mm/dd/yy'));
