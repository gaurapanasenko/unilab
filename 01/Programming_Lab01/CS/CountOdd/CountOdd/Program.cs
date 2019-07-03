using System;

namespace CountOdd
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			Console.WriteLine ("Введите последовательность целых чисел. Ноль - признак конца.");
			int x = 0, tmp = 0, i = 0;
			while (true) {
				Console.Write ("Введите число= ");
				if (!int.TryParse (Console.ReadLine (), out tmp)) {
					Console.WriteLine ("Попробуй ещё");
					continue;
				} else if (tmp == 0)
					break;
				else if (tmp % 2 == 1) {
					x++;
				}
				i++;
			}
			Console.WriteLine ("Встретилось " + x + " нечетных числа.");
		}
	}
}
