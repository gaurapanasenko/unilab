using System;

namespace HELLO
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			Console.Write ("Как Вас зовут? = ");
			string name = Console.ReadLine ();
			Console.Write ("Сколько Вам лет? = ");
			int age = 0;
			if (int.TryParse (Console.ReadLine (), out age) == false || age < 1) {
				Console.WriteLine ("Вы вписали неправильный формат возраста, он должен быть натуральным числом");
				return;
			}
			Console.Write ("Где Вы учитесь? = "); // хотя наверное правильно было сказать учились)
			string school = Console.ReadLine (); // todo: сделать FirstUppercase

			string end = " года";
			if ((age % 100 >= 5 && age % 100 <= 20) || (age % 10 >= 5 && age % 10 <= 9) || age % 10 == 0)
				end = " лет"; // а то "7 года" не звучит
			else if (age % 10 == 1)
				end = " год"; // "1 года" тоже не звучит
			int tmp = 0;
			if (int.TryParse (school, out tmp))
				school = "Школа №" + school; // на случай ввода только цыфр
			Console.WriteLine ();
			Console.WriteLine ("Здравствуйте, " + name + "!");
			Console.WriteLine ("Поздравляем Вас, Вам всего " + age + end + ", а Вы уже ведёте диалог с компьютером!");
			Console.WriteLine (school + " будет гордиться Вами!");
			string end = Console.ReadLine (); // чтобы некоторые могли увидеть результат
		}
	}
}
