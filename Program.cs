using System;

namespace ConsoleApp2
{
    class Program
    {
        static void Main(string[] args)
        {
            string x_date = "";

            DateTime today = DateTime.Today;
            DateTime yesterday = DateTime.Now.AddDays(-1);

            string today_date = today.ToString("dd/MM/yyyy");
            string yesterday_date = yesterday.ToString("dd/MM/yyyy");

            TimeSpan start = new TimeSpan(9, 0, 0);
            TimeSpan end = new TimeSpan(24, 0, 0);
            TimeSpan now = DateTime.Now.TimeOfDay;

            if ((now > start) && (now < end))
            {
                x_date = today_date.Replace('/','.');
            }
            else
            {  
                x_date = yesterday_date.Replace('/', '.');
            }
            Console.WriteLine(x_date);
            Console.Read();

        }
    }
}
