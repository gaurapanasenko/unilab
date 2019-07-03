using System;

namespace ANKETA
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			Console.Write ("Фамилия?= ");
			string sname = Console.ReadLine ();
			Console.Write ("Имя?= ");
			string fname = Console.ReadLine ();
			Console.Write ("Отчество?= ");
			string tname = Console.ReadLine ();
			Console.Write ("Пол?= ");
			string gender = Console.ReadLine ();
			Console.Write ("Дата рождения?= ");
			string bday = Console.ReadLine ();
			Console.Write ("Адрес?= ");
			string address = Console.ReadLine ();
			Console.Write ("Номер телефона?= ");
			string phone = Console.ReadLine ();
			Console.Write ("Место учебы?= ");
			string school = Console.ReadLine ();
			Console.Write ("Класс?= ");
			string grade = Console.ReadLine ();
			Console.Write ("Хобби?= ");
			string bestdoing = Console.ReadLine ();
			int[] firstcolumn = {7,sname.Length,3,grade.Length,11,school.Length};
			int[] secondcolumn = {3,fname.Length,13,bday.Length,5,grade.Length};
			int[] thirdcolumn = {8,tname.Length,14,phone.Length};
			int fcmax = 0;
			int scmax = 0;
			int tcmax = 0;
			foreach (int i in firstcolumn) {
				if (i > fcmax)
					fcmax = i;
			}
			foreach (int i in secondcolumn) {
				if (i > scmax)
					scmax = i;
			}
			foreach (int i in thirdcolumn) {
				if (i > tcmax)
					tcmax = i;
			}
			fcmax += 5;scmax += 5;tcmax += 5;
			Console.WriteLine (new String (' ', (fcmax + scmax + tcmax - 11) / 2) + "А Н К Е Т А");
			Console.WriteLine ("Фамилия" + new String (' ', fcmax - 7) + "Имя" + new String (' ', scmax - 3) + "Отчество");
			Console.WriteLine ("-------" + new String (' ', fcmax - 7) + "---" + new String (' ', scmax - 3) + "--------");
			Console.WriteLine (sname + new String (' ', fcmax - sname.Length) + fname + new String (' ', scmax - fname.Length) + tname);
			Console.WriteLine ();
			Console.WriteLine ("Пол" + new String (' ', fcmax - 3) + "Дата рождения" + new String (' ', scmax - 13) + "Номер телефона");
			Console.WriteLine ("---" + new String (' ', fcmax - 3) + "-------------" + new String (' ', scmax - 13) + "--------------");
			Console.WriteLine (gender + new String (' ', fcmax - gender.Length) + bday + new String (' ', scmax - bday.Length) + phone);
			Console.WriteLine ();
			Console.WriteLine ("Адрес");
			Console.WriteLine ("-----");
			Console.WriteLine (address);
			Console.WriteLine ();
			Console.WriteLine ("Место учебы" + new String (' ', fcmax - 11) + "Класс");
			Console.WriteLine ("-----------" + new String (' ', fcmax - 11) + "-----");
			Console.WriteLine (school + new String (' ', fcmax - grade.Length) + grade);
			Console.WriteLine ();
			Console.WriteLine ("Хобби");
			Console.WriteLine ("-----");
			Console.WriteLine (bestdoing);
		}
	}
}
