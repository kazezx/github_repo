using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Linq;

namespace ConsoleApp1
{
    class Program
    {
        static void Main(string[] args)
        {
            List<string> Licenses = new List<string>();
            List<int> counter = new List<int>();
            Console.Write("Enter Your Domain : ");
            string domain = Console.ReadLine().ToUpper();
            string query = "SELECT [License],[Counter] FROM ["+domain+"] WHERE ";
            Console.Write("Enter Your Query : ");
            string[] services = Console.ReadLine().Split(';');
            int query_size = services.Length - 1;
            for (int i = 0; i < services.Length; i++)
            {
                services[i] = (services[i].Replace(" ", String.Empty))+"=" + "'" + "Yes" + "'";
                if (i < query_size)     
                {
                    services[i] = services[i] + " AND ";
                }
                query = query + services[i];

            }
            query = query + "ORDER BY Counter ASC" + ";";
            Console.WriteLine(query);
            Console.ReadLine(); 
            string connectionString = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\Kaze\Documents\GRDF.mdb";
            string strSQL = query; 
            using (OleDbConnection connection = new OleDbConnection(connectionString))
            { 
                OleDbCommand command = new OleDbCommand(strSQL, connection);
                try
                { 
                    connection.Open();
                    using (OleDbDataReader reader = command.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            Licenses.Add(reader.GetValue(0).ToString());
                            counter.Add(Convert.ToInt32(reader.GetValue(1)));
                        }
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.Message);
                }
            }
            try
            {
                int index = counter.Min();
                string lic = Licenses[counter.IndexOf(index)];
                Console.WriteLine(lic);
            }
            catch (Exception) { Console.WriteLine("No such Licenses available"); }
            Console.ReadKey();

        }
    }
}
