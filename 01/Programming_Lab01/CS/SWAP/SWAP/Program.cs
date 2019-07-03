using System;

namespace SWAP
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			string A = Console.ReadLine ();
			string B = Console.ReadLine ();
			string C = Console.ReadLine ();
			string D = Console.ReadLine ();
			Console.WriteLine ("A=" + A + " B=" + B + " C=" + C + " D=" + D);
			string tmp = D;D = C;C = B;B = A;A = tmp;
			Console.WriteLine ("A=" + A + " B=" + B + " C=" + C + " D=" + D);
		}
	}
}
