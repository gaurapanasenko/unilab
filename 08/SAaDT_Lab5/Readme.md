# Звіт з лабораторної роботи 5
за дисципліною "Системний аналіз та теорія прийняття рішень"  
студента групи ПА-17-2  
Панасенка Єгора Сергійовича  
Кафедра комп'ютерних технологій  
ФПМ, ДНУ, 2020-2021 навч.р.  
Варант 17  

Звіт у форматі PDF доступний за посиланням  
[https://gaurapanasenko.github.io/unilab_opt/SAaDT_Lab5.pdf](https://gaurapanasenko.github.io/unilab_opt/SAaDT_Lab5.pdf).  
Вихідний код доступний за посиланням  
[https://github.com/gaurapanasenko/unilab/tree/master/08/SAaDT_Lab5](https://github.com/gaurapanasenko/unilab/tree/master/08/SAaDT_Lab5)

## Постановка задачі

У супермаркеті є M кас. Відвідувачі займають чергу до каси. Як правило, черги до різних кас приблизно однакові. Було помічено, що у разі довжині черги понад N покупець залишає супермаркет без покупки. Час приходу покупців і час їх обслуговування має експоненціальну залежність. Середній час приходу одного покупця – T, середній час обслуговування одного покупця – Z. Для залучення покупців перед супермаркетом будується парковка на X машин. Протягом доби супермаркет працює S годин.

Відповідно до варіанта завдання слід побудувати імітаційну модель роботи супермаркету і визначити статистичні характеристики системи згідно з вихідними даними, наведеними:

| № завдання | M, число кас, шт | N, довжина черги, чол. | T, час приходу покупця, хв. | Z, час обслуговування покупця, хв. | X, Місць наарковці, шт | S, час роботи супермаркету, хв. |
| --- | --- | --- | --- | --- | --- | --- |
| 17  |  5  |  9  | 11  |  7  |  6  | 120 |

## Перша ітерація

### GPSS Код

```
KRES STORAGE 5
GENERATE (EXPONENTIAL (1,0,11))
TRANSFER 0.25,NOCAR
CAR TEST L Q2,6,TOOFULLPARKING
QUEUE 2
TEST L Q1,9,TOOFULLKASS
QUEUE 1
ENTER KRES
DEPART 1
ADVANCE 7
LEAVE KRES
DEPART 2
TERMINATE
NOCAR TEST L Q1,9,TOOFULLKASS
QUEUE 1
ENTER KRES
DEPART 1
ADVANCE 7
LEAVE KRES
TERMINATE
TOOFULLPARKING TERMINATE
TOOFULLKASS TERMINATE
GENERATE 120
TERMINATE 1
START 1
```

### GPSS World звіт

```
              GPSS World Simulation Report - Lab5.23.1


                   Thursday, April 01, 2021 02:41:47  

           START TIME           END TIME  BLOCKS  FACILITIES  STORAGES
                0.000            120.000    23        0          1


              NAME                       VALUE  
          CAR                             3.000
          KRES                        10000.000
          NOCAR                          13.000
          TOOFULLKASS                    21.000
          TOOFULLPARKING                 20.000


 LABEL              LOC  BLOCK TYPE     ENTRY COUNT CURRENT COUNT RETRY
                    1    GENERATE             8             0       0
                    2    TRANSFER             8             0       0
CAR                 3    TEST                 1             0       0
                    4    QUEUE                1             0       0
                    5    TEST                 1             0       0
                    6    QUEUE                1             0       0
                    7    ENTER                1             0       0
                    8    DEPART               1             0       0
                    9    ADVANCE              1             0       0
                   10    LEAVE                1             0       0
                   11    DEPART               1             0       0
                   12    TERMINATE            1             0       0
NOCAR              13    TEST                 7             0       0
                   14    QUEUE                7             0       0
                   15    ENTER                7             0       0
                   16    DEPART               7             0       0
                   17    ADVANCE              7             0       0
                   18    LEAVE                7             0       0
                   19    TERMINATE            7             0       0
TOOFULLPARKING     20    TERMINATE            0             0       0
TOOFULLKASS        21    TERMINATE            0             0       0
                   22    GENERATE             1             0       0
                   23    TERMINATE            1             0       0


QUEUE              MAX CONT. ENTRY ENTRY(0) AVE.CONT. AVE.TIME   AVE.(-0) RETRY
 1                   1    0      8      8     0.000      0.000      0.000   0
 2                   1    0      1      0     0.058      7.000      7.000   0


STORAGE            CAP. REM. MIN. MAX.  ENTRIES AVL.  AVE.C. UTIL. RETRY DELAY
 KRES                5    5   0     2        8   1    0.467  0.093    0    0


FEC XN   PRI         BDT      ASSEM  CURRENT  NEXT  PARAMETER    VALUE
    10    0         131.607     10      0      1
    11    0         240.000     11      0     22
```

### Аналіз

Для моделювання було обрано, що імовірність що у покупця буде машина 25%, таким чином, якщо покупець має машину він іде по гілці `NOCAR`, яку видно в звіті, якщо же має машину, то він повинен припакруватися, а потім після покупки, звільнити місце паркування.

У звіті бачимо, що модель зовсім не навантажена, спробуємо три речі:
1. Перевіремо скільки нам потрібно кас для поточного навантаження,
2. Перевіремо яке навантаження витримає ця модель,
3. Перевантажимо модель, для того щоб перевірити, що усі умови задані в програмі працюють.

## Друга ітерація

### GPSS Код

```
KRES STORAGE 1
GENERATE (EXPONENTIAL (1,0,11))
TRANSFER 0.25,NOCAR
CAR TEST L Q2,6,TOOFULLPARKING
QUEUE 2
TEST L Q1,9,TOOFULLKASS
QUEUE 1
ENTER KRES
DEPART 1
ADVANCE 7
LEAVE KRES
DEPART 2
TERMINATE
NOCAR TEST L Q1,9,TOOFULLKASS
QUEUE 1
ENTER KRES
DEPART 1
ADVANCE 7
LEAVE KRES
TERMINATE
TOOFULLPARKING TERMINATE
TOOFULLKASS TERMINATE
GENERATE 120
TERMINATE 1
START 1
```

### GPSS World звіт

```
              GPSS World Simulation Report - Lab5.24.1


                   Thursday, April 01, 2021 02:50:57  

           START TIME           END TIME  BLOCKS  FACILITIES  STORAGES
                0.000            120.000    23        0          1


              NAME                       VALUE  
          CAR                             3.000
          KRES                        10000.000
          NOCAR                          13.000
          TOOFULLKASS                    21.000
          TOOFULLPARKING                 20.000


 LABEL              LOC  BLOCK TYPE     ENTRY COUNT CURRENT COUNT RETRY
                    1    GENERATE             8             0       0
                    2    TRANSFER             8             0       0
CAR                 3    TEST                 1             0       0
                    4    QUEUE                1             0       0
                    5    TEST                 1             0       0
                    6    QUEUE                1             0       0
                    7    ENTER                1             0       0
                    8    DEPART               1             0       0
                    9    ADVANCE              1             0       0
                   10    LEAVE                1             0       0
                   11    DEPART               1             0       0
                   12    TERMINATE            1             0       0
NOCAR              13    TEST                 7             0       0
                   14    QUEUE                7             0       0
                   15    ENTER                7             0       0
                   16    DEPART               7             0       0
                   17    ADVANCE              7             0       0
                   18    LEAVE                7             0       0
                   19    TERMINATE            7             0       0
TOOFULLPARKING     20    TERMINATE            0             0       0
TOOFULLKASS        21    TERMINATE            0             0       0
                   22    GENERATE             1             0       0
                   23    TERMINATE            1             0       0


QUEUE              MAX CONT. ENTRY ENTRY(0) AVE.CONT. AVE.TIME   AVE.(-0) RETRY
 1                   2    0      8      4     0.216      3.245      6.491   0
 2                   1    0      1      0     0.110     13.201     13.201   0


STORAGE            CAP. REM. MIN. MAX.  ENTRIES AVL.  AVE.C. UTIL. RETRY DELAY
 KRES                1    1   0     1        8   1    0.467  0.467    0    0


FEC XN   PRI         BDT      ASSEM  CURRENT  NEXT  PARAMETER    VALUE
    10    0         131.607     10      0      1
    11    0         240.000     11      0     22
```

### Аналіз

Як бачимо, що достатньо однієї каси для стабільної роботи моделі.

## Третя ітерація

### GPSS Код

```
KRES STORAGE 5
GENERATE (EXPONENTIAL (1,0,2))
TRANSFER 0.25,NOCAR
CAR TEST L Q2,6,TOOFULLPARKING
QUEUE 2
TEST L Q1,9,TOOFULLKASS
QUEUE 1
ENTER KRES
DEPART 1
ADVANCE 7
LEAVE KRES
DEPART 2
TERMINATE
NOCAR TEST L Q1,9,TOOFULLKASS
QUEUE 1
ENTER KRES
DEPART 1
ADVANCE 7
LEAVE KRES
TERMINATE
TOOFULLPARKING TERMINATE
TOOFULLKASS TERMINATE
GENERATE 120
TERMINATE 1
START 1
```

### GPSS World звіт

```
              GPSS World Simulation Report - Lab5.25.1


                   Thursday, April 01, 2021 02:52:33  

           START TIME           END TIME  BLOCKS  FACILITIES  STORAGES
                0.000            120.000    23        0          1


              NAME                       VALUE  
          CAR                             3.000
          KRES                        10000.000
          NOCAR                          13.000
          TOOFULLKASS                    21.000
          TOOFULLPARKING                 20.000


 LABEL              LOC  BLOCK TYPE     ENTRY COUNT CURRENT COUNT RETRY
                    1    GENERATE            54             0       0
                    2    TRANSFER            54             0       0
CAR                 3    TEST                19             0       0
                    4    QUEUE               19             0       0
                    5    TEST                19             0       0
                    6    QUEUE               19             0       0
                    7    ENTER               19             0       0
                    8    DEPART              19             0       0
                    9    ADVANCE             19             0       0
                   10    LEAVE               19             0       0
                   11    DEPART              19             0       0
                   12    TERMINATE           19             0       0
NOCAR              13    TEST                35             0       0
                   14    QUEUE               35             0       0
                   15    ENTER               35             0       0
                   16    DEPART              35             0       0
                   17    ADVANCE             35             0       0
                   18    LEAVE               35             0       0
                   19    TERMINATE           35             0       0
TOOFULLPARKING     20    TERMINATE            0             0       0
TOOFULLKASS        21    TERMINATE            0             0       0
                   22    GENERATE             1             0       0
                   23    TERMINATE            1             0       0


QUEUE              MAX CONT. ENTRY ENTRY(0) AVE.CONT. AVE.TIME   AVE.(-0) RETRY
 1                   3    0     54     39     0.225      0.500      1.800   0
 2                   4    0     19      0     1.159      7.320      7.320   0


STORAGE            CAP. REM. MIN. MAX.  ENTRIES AVL.  AVE.C. UTIL. RETRY DELAY
 KRES                5    5   0     5       54   1    3.150  0.630    0    0


FEC XN   PRI         BDT      ASSEM  CURRENT  NEXT  PARAMETER    VALUE
    56    0         123.665     56      0      1
    57    0         240.000     57      0     22
```

### Аналіз

Було виставлено для нормальної роботи системи математичне очікування 2 хвилини.

## Четверта ітерація

### GPSS Код

```
KRES STORAGE 5
GENERATE (EXPONENTIAL (1,0,1))
TRANSFER 0.25,NOCAR
CAR TEST L Q2,6,TOOFULLPARKING
QUEUE 2
TEST L Q1,9,TOOFULLKASS
QUEUE 1
ENTER KRES
DEPART 1
ADVANCE 7
LEAVE KRES
DEPART 2
TERMINATE
NOCAR TEST L Q1,9,TOOFULLKASS
QUEUE 1
ENTER KRES
DEPART 1
ADVANCE 7
LEAVE KRES
TERMINATE
TOOFULLPARKING TERMINATE
TOOFULLKASS TERMINATE
GENERATE 120
TERMINATE 1
START 1
```

### GPSS World звіт

```
              GPSS World Simulation Report - Lab5.26.1


                   Thursday, April 01, 2021 02:54:21  

           START TIME           END TIME  BLOCKS  FACILITIES  STORAGES
                0.000            120.000    23        0          1


              NAME                       VALUE  
          CAR                             3.000
          KRES                        10000.000
          NOCAR                          13.000
          TOOFULLKASS                    21.000
          TOOFULLPARKING                 20.000


 LABEL              LOC  BLOCK TYPE     ENTRY COUNT CURRENT COUNT RETRY
                    1    GENERATE           102             0       0
                    2    TRANSFER           102             0       0
CAR                 3    TEST                30             0       0
                    4    QUEUE               27             0       0
                    5    TEST                27             0       0
                    6    QUEUE               25             2       0
                    7    ENTER               23             0       0
                    8    DEPART              23             0       0
                    9    ADVANCE             23             2       0
                   10    LEAVE               21             0       0
                   11    DEPART              21             0       0
                   12    TERMINATE           21             0       0
NOCAR              13    TEST                72             0       0
                   14    QUEUE               65             2       0
                   15    ENTER               63             0       0
                   16    DEPART              63             0       0
                   17    ADVANCE             63             3       0
                   18    LEAVE               60             0       0
                   19    TERMINATE           60             0       0
TOOFULLPARKING     20    TERMINATE            3             0       0
TOOFULLKASS        21    TERMINATE            9             0       0
                   22    GENERATE             1             0       0
                   23    TERMINATE            1             0       0


QUEUE              MAX CONT. ENTRY ENTRY(0) AVE.CONT. AVE.TIME   AVE.(-0) RETRY
 1                   9    4     90     10     4.865      6.486      7.297   0
 2                   6    6     27      0     3.571     15.869     15.869   0


STORAGE            CAP. REM. MIN. MAX.  ENTRIES AVL.  AVE.C. UTIL. RETRY DELAY
 KRES                5    0   0     5       86   1    4.862  0.972    0    4


FEC XN   PRI         BDT      ASSEM  CURRENT  NEXT  PARAMETER    VALUE
   104    0         120.686    104      0      1
    95    0         121.243     95     17     18
    96    0         122.797     96      9     10
    97    0         123.964     97     17     18
    98    0         124.338     98     17     18
    99    0         126.172     99      9     10
   105    0         240.000    105      0     22
```

### Аналіз

Як бачимо, троє покупців відмовились від покупки із-за відсутності місць на парковці, а 9 покупців із за переповнення кас.
Також бачимо приблизне розподілення, що 25% мали машину. А точніше мали машину 30 покупців, а 72 її не мали.

## Висновки

Модель не навантажена, але дуще часто простоює і для поточного навантаження достатньо однієї каси. А ця модель здатна приймати навантаження, коли математичне очікування появи покупців 2 хвилини.
