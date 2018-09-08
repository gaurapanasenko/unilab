using System;

namespace HIT2
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			Random rnd = new Random();
			int ox = rnd.Next (-1000, 1000), oy = rnd.Next (-1000, 1000),
			or = rnd.Next (1, 100), isx = 0, isy = 0, ifm = 0, ism = 0, ord, dx = 0, dy = 0;
			ord = or * or;
			bool frun = true;
			string[] istr;
			while (true) {
				Console.WriteLine ("Введите x, y через пробел и нажмите ENTER.");
				istr = Console.ReadLine ().Split();
				if (istr.Length != 2)
					Console.WriteLine ("Неверный формат");
				else if (!int.TryParse (istr [0], out isx))
					Console.WriteLine ("Неверный формат x");
				else if (!int.TryParse (istr [1], out isy))
					Console.WriteLine ("Неверный формат y");
				else {
					dx = isx - ox;
					dy = isy - oy;
					ism = dx * dx + dy * dy;
					Console.WriteLine (dx + " " + dy + " " + ism + " " + ord);
					if (ism <= ord) {
						Console.WriteLine ("Точка (" + isx + "," + isy + ") попала в круг с центром в точке (" + ox + "," + oy + ") радиуса " + or + "");
						break;
					}
					if (frun) {
						Console.WriteLine ("Попробуй ещё");
						frun = false;
					} else {
						if (ism < ifm)
							Console.WriteLine ("Теплее");
						else if (ism == ifm)
							Console.WriteLine ("Одинаково");
						else if (ism > ifm)
							Console.WriteLine ("Холоднее");
					}
					ifm = ism;
				}
			}
		}
	}
}
