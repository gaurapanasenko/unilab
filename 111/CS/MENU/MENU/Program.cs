using System;
using System.Net;

namespace MENU
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			string tmp = "";
			while (true) {
				Console.WriteLine ("Работает информационная система СШ23");
				Console.WriteLine ("1. Новости дня");
				Console.WriteLine ("2. Анекдот недели");
				Console.WriteLine ("3. Автора!!!");
				Console.WriteLine ("0. Выход");
				Console.WriteLine ("Введите соответсвующий номер и нажмите ENTER.");
				int a = 0;
				if (!int.TryParse (Console.ReadLine (), out a) || a < 0 || a > 3)
					Console.WriteLine ("Попробуйте ещё");
				else if (a == 0)
					break;
				else if (a == 1 || a == 2) {
					Console.WriteLine ("Новостей и Анекдотов нет");
					//string json;
					//using (var webpage = new WebClient())
					//{
						//webpage.Headers[HttpRequestHeader.UserAgent] = "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.2 (KHTML, like Gecko) Chrome/15.0.874.121 Safari/535.2";
						//json = webpage.DownloadString("https://ru.wikipedia.org/w/api.php?action=query&generator=random&grnnamespace=0&prop=extracts&format=json&explaintext=");
					//}
					//Console.WriteLine (json);
				} else if (a == 3)
					Console.WriteLine ("Данных нет");
				Console.WriteLine ("Нажмите ENTER для продолжения.");
				tmp = Console.ReadLine ();
				Console.Clear ();
			}
		}
	}
}
