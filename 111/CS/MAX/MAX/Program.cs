using System;

namespace MAX
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			Console.WriteLine ("Введите последовательность целых чисел. Ноль - признак конца.");
			int x = 0, tmp = 0, i = 0;
			while (true) {
				Console.Write ("a[" + (i + 1) + "]=");
				if (!int.TryParse (Console.ReadLine (), out tmp)) {
					Console.WriteLine ("Попробуй ещё");
					continue;
				} else if (tmp == 0)
					break;
				else if (tmp > x) {
					x = tmp;
				}
				i++;
			}
			Console.WriteLine ("MAX=" + x);
		}
	}
}
