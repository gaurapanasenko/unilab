using System;

namespace Fibonacci
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			int n = 0, a = 1, b = 1, c = 0, i = 2;
			Console.Write ("N = ");
			if (int.TryParse (Console.ReadLine (), out n) && n >= 0) {
				if (n == 0)
					Console.WriteLine ("F(0) = 0");
				else if (n == 1)
					Console.WriteLine ("F(1) = 1");
				else {
					while (i < n) {
						c = a + b;a = b;b = c;i++;
					}
					Console.WriteLine ("F(" + n + ") = " + b);
				}
			} else
				Console.WriteLine ("Неправиьный формат, ввод должен быть натуральным числом.");
		}
	}
}
