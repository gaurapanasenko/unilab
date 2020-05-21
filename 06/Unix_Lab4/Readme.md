# Лабораторна робота №4, Панасенко Єгор, ПА-17-2

## Постановка задачі

Написати на мові Perl або bash скрипт, який прочитає зміст усіх файлів, що передані йому в командній лінії та об'єднає їх в один файл. Якщо серед переданих файлів будуть файли в форматі HTML, то в них необхідно знищити всі теги.

## Вихідний текст розв'язку

### Скрипт: `lab4.sh`
```shell
#!/bin/sh

[ -z "$1" ] || [ -z "$2" ] && { echo "Немає потрібних аргументів"; exit 1; }

OUTPUT_FILE="${@:$#}"
FILES="${@:1:$(($#-1))}"

find $FILES >/dev/null || { echo "Файли перелічені вище не існують"; exit 1; }

sed -E "s/<[^>]*?>//g" $FILES >$OUTPUT_FILE
exit $?

```

## Опис розв'язку

Скрипт `lab4.sh` приймає масив файлів та об'єднує з першого файлу до перед останнього у файл з назвою позначеної у останньому аргументі.
Тому як мінімум повинно бути 2 аргументи.
Скрипт перевіряє наявність перелічених файлів і у випадку якщо якийсь файл відсутній, виводить назви відсутніх файлів та виходить з помилкою.
Це зроблено за допомогою програми `find`, причому стандартний потік виводу перенаправлений у `/dev/null`, тобто не виводиться нікуди,
у той же час стандартний потік помилок виводиться на екран.

## Опис тестових прикладів

### Тестові файли

Створимо якісь тестові файли.

#### Файл `table.html`

```html
<!DOCTYPE html>
<html>
<head>
<style>
table {
  font-family: arial, sans-serif;
  border-collapse: collapse;
  width: 100%;
}

td, th {
  border: 1px solid #dddddd;
  text-align: left;
  padding: 8px;
}

tr:nth-child(even) {
  background-color: #dddddd;
}
</style>
</head>
<body>

<h2>HTML Table</h2>

<table>
  <tr>
    <th>Company</th>
    <th>Contact</th>
    <th>Country</th>
  </tr>
  <tr>
    <td>Alfreds Futterkiste</td>
    <td>Maria Anders</td>
    <td>Germany</td>
  </tr>
  <tr>
    <td>Centro comercial Moctezuma</td>
    <td>Francisco Chang</td>
    <td>Mexico</td>
  </tr>
  <tr>
    <td>Ernst Handel</td>
    <td>Roland Mendel</td>
    <td>Austria</td>
  </tr>
  <tr>
    <td>Island Trading</td>
    <td>Helen Bennett</td>
    <td>UK</td>
  </tr>
  <tr>
    <td>Laughing Bacchus Winecellars</td>
    <td>Yoshi Tannamuri</td>
    <td>Canada</td>
  </tr>
  <tr>
    <td>Magazzini Alimentari Riuniti</td>
    <td>Giovanni Rovelli</td>
    <td>Italy</td>
  </tr>
</table>

</body>
</html>
```

#### Файл `lists.html`

```html
<!DOCTYPE html>
<html>
<body>

<h2>An Unordered HTML List</h2>

<ul>
  <li>Coffee</li>
  <li>Tea</li>
  <li>Milk</li>
</ul>

<h2>An Ordered HTML List</h2>

<ol>
  <li>Coffee</li>
  <li>Tea</li>
  <li>Milk</li>
</ol>

</body>
</html>
```

### Приклад

Запустимо команду:
```shell
./lab4.sh table.html lists.html out.txt
```

Тепер відкриємо `out.txt`:

```




table {
  font-family: arial, sans-serif;
  border-collapse: collapse;
  width: 100%;
}

td, th {
  border: 1px solid #dddddd;
  text-align: left;
  padding: 8px;
}

tr:nth-child(even) {
  background-color: #dddddd;
}




HTML Table



    Company
    Contact
    Country


    Alfreds Futterkiste
    Maria Anders
    Germany


    Centro comercial Moctezuma
    Francisco Chang
    Mexico


    Ernst Handel
    Roland Mendel
    Austria


    Island Trading
    Helen Bennett
    UK


    Laughing Bacchus Winecellars
    Yoshi Tannamuri
    Canada


    Magazzini Alimentari Riuniti
    Giovanni Rovelli
    Italy









An Unordered HTML List


  Coffee
  Tea
  Milk


An Ordered HTML List


  Coffee
  Tea
  Milk




```
