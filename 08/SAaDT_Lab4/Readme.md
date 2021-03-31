# Звіт з лабораторної роботи 4
за дисципліною "Системний аналіз та теорія прийняття рішень"  
студента групи ПА-17-2  
Панасенка Єгора Сергійовича  
Кафедра комп'ютерних технологій  
ФПМ, ДНУ, 2020-2021 навч.р.  
Варант 17  

Звіт у форматі PDF доступний за посиланням  
[https://gaurapanasenko.github.io/unilab_opt/SAaDT_Lab4.pdf](https://gaurapanasenko.github.io/unilab_opt/SAaDT_Lab4.pdf).  
Вихідний код доступний за посиланням  
[https://github.com/gaurapanasenko/unilab/tree/master/08/SAaDT_Lab4](https://github.com/gaurapanasenko/unilab/tree/master/08/SAaDT_Lab4)

## Постановка задачі

Потрібно побудувати імітаційну модель роботи перукарні за таких параметрів роботи. До обслуговуючої організації приходять клієнти за експонентним законом у середньому 2 людини на годину (математичне очікування 30 хв). Обслуговування клієнтів здійснюють чотири майстри.Час обслуговування клієнта – дискретний і становить від 14 до 19 хв. Клієнт відмовляється від обслуговування, якщо в черзі більше двох осіб.

Необхідно побудувати імітаційну модель за часом її роботи 480 хв і визначити статистичні характеристики системи.

## Перша ітерація

### GPSS код

```
KRES STORAGE 4
GENERATE (EXPONENTIAL (1,0,30))
TEST L Q1,3,OTKAZ
QUEUE 1
ENTER KRES
DEPART 1
ADVANCE 16.5,2.5
LEAVE KRES
TERMINATE
OTKAZ TERMINATE
GENERATE 480
TERMINATE 1
START 1
```

### GPSS World звіт

```
              GPSS World Simulation Report - Lab4.16.1


                   Thursday, April 01, 2021 00:53:50  

           START TIME           END TIME  BLOCKS  FACILITIES  STORAGES
                0.000            480.000    11        0          1


              NAME                       VALUE  
          KRES                        10000.000
          OTKAZ                           9.000


 LABEL              LOC  BLOCK TYPE     ENTRY COUNT CURRENT COUNT RETRY
                    1    GENERATE            18             0       0
                    2    TEST                18             0       0
                    3    QUEUE               18             0       0
                    4    ENTER               18             0       0
                    5    DEPART              18             0       0
                    6    ADVANCE             18             0       0
                    7    LEAVE               18             0       0
                    8    TERMINATE           18             0       0
OTKAZ               9    TERMINATE            0             0       0
                   10    GENERATE             1             0       0
                   11    TERMINATE            1             0       0


QUEUE              MAX CONT. ENTRY ENTRY(0) AVE.CONT. AVE.TIME   AVE.(-0) RETRY
 1                   1    0     18     18     0.000      0.000      0.000   0


STORAGE            CAP. REM. MIN. MAX.  ENTRIES AVL.  AVE.C. UTIL. RETRY DELAY
 KRES                4    4   0     3       18   1    0.623  0.156    0    0


FEC XN   PRI         BDT      ASSEM  CURRENT  NEXT  PARAMETER    VALUE
    20    0         501.120     20      0      1
    21    0         960.000     21      0     10
```

### Аналіз

Відносно попередньої лабораторної роботи було використано нову команду `TEST`

> "TEST XX A,B,[C]
> XX – знак логической операции: L – меньше, G – больше, E – равно, LE – меньше или равно, GE – больше или равно, NE – не равно; A, B – сравниваемые значения или СЧА; C – метка блока, куда перемещается транзакт в случае невыполнения заданного условия.
> При выполнении условия, записанного в блоке TEST, транзакт переходит
в следующий блок. В противном случае он направляется в блок с меткой,
содержащейся в операнде C. Если операнд C не задан, то транзакт задерживается в блоке TEST до выполнения условия."
> - Имитационное моделирование на GPSS : учеб.-метод. пособие для студентов технических специальностей / Д. Н. Шевченко, И. Н. Кравченя; М-во образования Респ. Беларусь, Белорус. гос. ун-т трансп. – Гомель : БелГУТ, 2007. – 97 с. - [http://simulation.su/uploads/files/default/2007-uch-posob-snevchenko-kravchenko-1.pdf](http://simulation.su/uploads/files/default/2007-uch-posob-snevchenko-kravchenko-1.pdf)

У звіті бачимо, що, у нас з'явилося 18 транзактів і жоден не відмовився від послуги, завдяки великої черги. Біль того у нас черги і не було, усі 18 транзактів виконувалися зразу. Що наводить на думку, що нам не потрібні усі 4 майстри. Спробуємо узяти одного майстра.

## Друга ітерація

### GPSS код

```
KRES STORAGE 1
GENERATE (EXPONENTIAL (1,0,30))
TEST L Q1,3,OTKAZ
QUEUE 1
ENTER KRES
DEPART 1
ADVANCE 16.5,2.5
LEAVE KRES
TERMINATE
OTKAZ TERMINATE
GENERATE 480
TERMINATE 1
START 1
```

### GPSS World звіт

```
              GPSS World Simulation Report - Lab4.18.1


                   Thursday, April 01, 2021 01:02:34  

           START TIME           END TIME  BLOCKS  FACILITIES  STORAGES
                0.000            480.000    11        0          1


              NAME                       VALUE  
          KRES                        10000.000
          OTKAZ                           9.000


 LABEL              LOC  BLOCK TYPE     ENTRY COUNT CURRENT COUNT RETRY
                    1    GENERATE            19             0       0
                    2    TEST                19             0       0
                    3    QUEUE               19             0       0
                    4    ENTER               19             0       0
                    5    DEPART              19             0       0
                    6    ADVANCE             19             1       0
                    7    LEAVE               18             0       0
                    8    TERMINATE           18             0       0
OTKAZ               9    TERMINATE            0             0       0
                   10    GENERATE             1             0       0
                   11    TERMINATE            1             0       0


QUEUE              MAX CONT. ENTRY ENTRY(0) AVE.CONT. AVE.TIME   AVE.(-0) RETRY
 1                   3    0     19      6     0.691     17.463     25.523   0


STORAGE            CAP. REM. MIN. MAX.  ENTRIES AVL.  AVE.C. UTIL. RETRY DELAY
 KRES                1    0   0     1       19   1    0.629  0.629    0    0


FEC XN   PRI         BDT      ASSEM  CURRENT  NEXT  PARAMETER    VALUE
    21    0         481.028     21      0      1
    20    0         488.956     20      6      7
    22    0         960.000     22      0     10
```

### Аналіз

У цій ітерації ми отримали вже чергу, у середньому у черзі `0.691` знаходяться транзакта і три клієнта очікували у черзі, тому можемо також зробити висновок, що навіть майстер легко обслуговує клієнтів. Тепер спробуємо визначити, коли у середньому у черзі буде приблизно один клієнт у черзі, наприклад задавши, що 3 клієнти можуть поступати у середньому на годину (математичне очікування 20 хв).


### GPSS код

```
KRES STORAGE 1
GENERATE (EXPONENTIAL (1,0,20))
TEST L Q1,3,OTKAZ
QUEUE 1
ENTER KRES
DEPART 1
ADVANCE 16.5,2.5
LEAVE KRES
TERMINATE
OTKAZ TERMINATE
GENERATE 480
TERMINATE 1
START 1
```

### GPSS World звіт

```
              GPSS World Simulation Report - Lab4.19.1


                   Thursday, April 01, 2021 01:09:41  

           START TIME           END TIME  BLOCKS  FACILITIES  STORAGES
                0.000            480.000    11        0          1


              NAME                       VALUE  
          KRES                        10000.000
          OTKAZ                           9.000


 LABEL              LOC  BLOCK TYPE     ENTRY COUNT CURRENT COUNT RETRY
                    1    GENERATE            27             0       0
                    2    TEST                27             0       0
                    3    QUEUE               25             1       0
                    4    ENTER               24             0       0
                    5    DEPART              24             0       0
                    6    ADVANCE             24             1       0
                    7    LEAVE               23             0       0
                    8    TERMINATE           23             0       0
OTKAZ               9    TERMINATE            2             0       0
                   10    GENERATE             1             0       0
                   11    TERMINATE            1             0       0


QUEUE              MAX CONT. ENTRY ENTRY(0) AVE.CONT. AVE.TIME   AVE.(-0) RETRY
 1                   3    1     25      5     1.089     20.902     26.127   0


STORAGE            CAP. REM. MIN. MAX.  ENTRIES AVL.  AVE.C. UTIL. RETRY DELAY
 KRES                1    0   0     1       24   1    0.800  0.800    0    1


FEC XN   PRI         BDT      ASSEM  CURRENT  NEXT  PARAMETER    VALUE
    27    0         483.403     27      6      7
    29    0         496.360     29      0      1
    30    0         960.000     30      0     10
```

### Аналіз

На цьому звіті ми бачимо, що у середьному оброблюєтсья `1.089` транзакти, тому система все трохи перевантажена. Але бачимо ще одну особливість, що два клієнти відмовились від обслуговування, тому що побачили довгу чергу. Таким чином робимо висновок, що у цій ітерації майстер перевантажений.

## Висновки

Завдяки трьом ітераціям моделювання процесу, виявлено, що для найбільш оптимального обслуговування, так щоб і не простоювали майстри та встигались оброблюватись, потрібно залишити одного майстра. Але якщо буде поступати у середньому 3 клієнта за годину за экспонентним законом, то клієнти будуть відлятися від обслуговування.
