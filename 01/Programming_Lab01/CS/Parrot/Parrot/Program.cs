using System;

namespace Parrot
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			string str = "";
			while (true) {
				Console.WriteLine ("Введите строку и нажмите ENTER или нажмите просто ENTER для выхода");
				str = Console.ReadLine ();
				if (str == "")
					break;
				Console.WriteLine (str);
			}
		}
	}
}
