using System;

namespace Factor
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			int a = 0; double b = 0;
			Console.Write ("N = ");
			string input = Console.ReadLine ();
			if (int.TryParse (input, out a)) {
				if (a < 1)
					Console.WriteLine ("Error: N must be natural, not negative!");
			} else if (double.TryParse (input, out b))
				Console.WriteLine ("Error: N must be natural, not real!");
			else Console.WriteLine ("Error: N must be number, not string!");
		}
	}
}
