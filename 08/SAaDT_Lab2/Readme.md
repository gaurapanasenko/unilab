# Звіт з лабораторної роботи 2
за дисципліною "Системний аналіз та теорія прийняття рішень"  
студента групи ПА-17-2  
Панасенка Єгора Сергійовича  
Кафедра комп'ютерних технологій  
ФПМ, ДНУ, 2020-2021 навч.р.  
Варант 17  

Звіт у форматі PDF доступний за посиланням  
[https://gaurapanasenko.github.io/unilab_opt/SAaDT_Lab2.pdf](https://gaurapanasenko.github.io/unilab_opt/SAaDT_Lab2.pdf).  
Вихідний код доступний за посиланням  
[https://github.com/gaurapanasenko/unilab/tree/master/08/SAaDT_Lab2](https://github.com/gaurapanasenko/unilab/tree/master/08/SAaDT_Lab2)

## Постановка задачі

Потрібно побудувати імітаційну модель роботи каси з продажу квитків за таких параметрів роботи. Відвідувачі приходять до каси через 10 ... 30 сек. Оглядають приміщення – 0 ... 15 сек. і займають чергу. У касі працюють два касири. Кожен касир витрачає на обслуговування відвідувача однаковий час – приблизно 15 ... 25 сек. Каса працює 5 год.

## Перша ітерація

### GPSS Код

```
SIMULATE
GENERATE 20,10  ;new person every 20sec +-10sec
TRANSFER 0.5,,PROM  ;probability 1/2 for queue PROM 
ADVANCE 7.5,7.5 ;wait 7.5sec+-7.5sec or 0..15sec, I FOUND MISTAKE!!!!
QUEUE OCH
SEIZE KASS 
DEPART OCH 
ADVANCE 20,5    ;this kass processes person 20sec +-5sec
RELEASE KASS
TERMINATE 
PROM QUEUE OCH1
SEIZE KASS1 
DEPART OCH1 
ADVANCE 20,5    ;this kass processes person 20sec +-5sec
RELEASE KASS1 
TERMINATE 
GENERATE 18000  ;this is seconds!!! I know it!!!
TERMINATE 1
START 1
```

### GPSS World звіт

```
              GPSS World Simulation Report - Lab2.16.1


                   Thursday, April 01, 2021 01:30:53  

           START TIME           END TIME  BLOCKS  FACILITIES  STORAGES
                0.000          18000.000    17        2          0


              NAME                       VALUE  
          KASS                        10003.000
          KASS1                       10001.000
          OCH                         10002.000
          OCH1                        10000.000
          PROM                           10.000


 LABEL              LOC  BLOCK TYPE     ENTRY COUNT CURRENT COUNT RETRY
                    1    GENERATE           921             0       0
                    2    TRANSFER           921             0       0
                    3    ADVANCE            461             0       0
                    4    QUEUE              461             1       0
                    5    SEIZE              460             0       0
                    6    DEPART             460             0       0
                    7    ADVANCE            460             1       0
                    8    RELEASE            459             0       0
                    9    TERMINATE          459             0       0
PROM               10    QUEUE              460             0       0
                   11    SEIZE              460             0       0
                   12    DEPART             460             0       0
                   13    ADVANCE            460             1       0
                   14    RELEASE            459             0       0
                   15    TERMINATE          459             0       0
                   16    GENERATE             1             0       0
                   17    TERMINATE            1             0       0


FACILITY         ENTRIES  UTIL.   AVE. TIME AVAIL. OWNER PEND INTER RETRY DELAY
 KASS1              460    0.507      19.840  1      922    0    0     0      0
 KASS               460    0.508      19.871  1      920    0    0     0      1


QUEUE              MAX CONT. ENTRY ENTRY(0) AVE.CONT. AVE.TIME   AVE.(-0) RETRY
 OCH1                2    0    460    320     0.051      1.991      6.543   0
 OCH                 2    1    461    306     0.071      2.783      8.277   0


FEC XN   PRI         BDT      ASSEM  CURRENT  NEXT  PARAMETER    VALUE
   920    0       18004.732    920      7      8
   922    0       18016.003    922     13     14
   923    0       18020.509    923      0      1
   924    0       36000.000    924      0     16
```

### Аналіз

У попередьному навчальному семестрі я був ознайомлений з деякими командами програмнимного пакету GPSS, але у цій лабораторній роботі з'явилась нова незнайома команда `TRANSFER`.

> "Блок TRANSFER изменяет маршрут движения транзактов в зависимости от значения первого операнда (в основном или альтернативном направлении) при определенном состоянии оборудования: обслуживающих устройств, очереди, логических ключей и т.п.:
> TRANSFER [A],B,[C],[D]
> A – режим перехода;
> B – метка первого альтернативного блока;
> C – метка второго альтернативного блока;
> D – константа, используемая для относительной переадресации транзактов."
> - Имитационное моделирование на GPSS : учеб.-метод. пособие для студентов технических специальностей / Д. Н. Шевченко, И. Н. Кравченя; М-во образования Респ. Беларусь, Белорус. гос. ун-т трансп. – Гомель : БелГУТ, 2007. – 97 с. - [http://simulation.su/uploads/files/default/2007-uch-posob-snevchenko-kravchenko-1.pdf](http://simulation.su/uploads/files/default/2007-uch-posob-snevchenko-kravchenko-1.pdf)

Таким чином за допомогою `TRANSFER` ми задаємо, що із вирогідністю 0.5 ми будемо вибирати другу касу.

Також у постановці задачі, та прикладу на GPSS виявлено та виправлено помилки та розходження, зокрема виправлено, що огляд приміщеня повинен бути все ж таки у діапазоні 0...15 сек, що означає 7.5сек +-7.5сек.

У звіті бачимо що каси не навантажені, і у кожній черзі по 2 максимальних тракзакта і у середному знаходилось `0.061` транзакта, тому можна поэксперементувати із вирогідністю. Фізично це можливо зробити одну із кас на неповний робочий часом, хоча імовірність не зовсім прямий спосіб задання цієї нової моделі з неповний робочим часом.

## Друга ітерація

### GPSS Код

```
SIMULATE
GENERATE 20,10  ;new person every 20sec +-10sec
TRANSFER 0.94,,PROM ;probability 1/2 for queue PROM 
ADVANCE 7.5,7.5 ;wait 7.5sec+-7.5sec or 0..15sec, I FOUND MISTAKE!!!!
QUEUE OCH
SEIZE KASS 
DEPART OCH 
ADVANCE 20,5    ;this kass processes person 20sec +-5sec
RELEASE KASS
TERMINATE 
PROM QUEUE OCH1
SEIZE KASS1 
DEPART OCH1 
ADVANCE 20,5    ;this kass processes person 20sec +-5sec
RELEASE KASS1 
TERMINATE 
GENERATE 18000  ;this is seconds!!! I know it!!!
TERMINATE 1
START 1
```

### GPSS World звіт

```
              GPSS World Simulation Report - Lab2_Interesting.16.1


                   Thursday, April 01, 2021 01:45:58  

           START TIME           END TIME  BLOCKS  FACILITIES  STORAGES
                0.000          18000.000    17        2          0


              NAME                       VALUE  
          KASS                        10003.000
          KASS1                       10001.000
          OCH                         10002.000
          OCH1                        10000.000
          PROM                           10.000


 LABEL              LOC  BLOCK TYPE     ENTRY COUNT CURRENT COUNT RETRY
                    1    GENERATE           888             0       0
                    2    TRANSFER           888             0       0
                    3    ADVANCE             54             0       0
                    4    QUEUE               54             0       0
                    5    SEIZE               54             0       0
                    6    DEPART              54             0       0
                    7    ADVANCE             54             0       0
                    8    RELEASE             54             0       0
                    9    TERMINATE           54             0       0
PROM               10    QUEUE              834             0       0
                   11    SEIZE              834             0       0
                   12    DEPART             834             0       0
                   13    ADVANCE            834             1       0
                   14    RELEASE            833             0       0
                   15    TERMINATE          833             0       0
                   16    GENERATE             1             0       0
                   17    TERMINATE            1             0       0


FACILITY         ENTRIES  UTIL.   AVE. TIME AVAIL. OWNER PEND INTER RETRY DELAY
 KASS1              834    0.920      19.852  1      889    0    0     0      0
 KASS                54    0.060      20.010  1        0    0    0     0      0


QUEUE              MAX CONT. ENTRY ENTRY(0) AVE.CONT. AVE.TIME   AVE.(-0) RETRY
 OCH1                4    0    834    202     0.504     10.885     14.365   0
 OCH                 1    0     54     51     0.001      0.399      7.173   0


FEC XN   PRI         BDT      ASSEM  CURRENT  NEXT  PARAMETER    VALUE
   890    0       18012.617    890      0      1
   889    0       18018.335    889     13     14
   891    0       36000.000    891      0     16
```

### Аналіз

Було обрано імовірність 0.94, що тракзакти будуть оброблятися касою `KASS1`, це видно у стовпці `UTIL.`. Як бачимо система працює эфективно, на цій ітерації, але було виявлено, що, якщо підвищувати цю імовірність, то каса `KASS1` буде перевантажена і таким чином відмовитись від другої каси не можливо.

## Висновки

Модель працює стабільно, хоча ці каси простоюють, але спроби відмовитись від другої каси приводять до перевантаження.
