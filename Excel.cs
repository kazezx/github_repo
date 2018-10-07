using System;
using System.Diagnostics;
using Microsoft.Office.Interop.Excel;

namespace Excel_Manipulator
{
    class Program
    {
        static void Main(string[] args)
        {
            //Create COM Objects.
            Application excelApp = new Application();
            if (excelApp == null)
            {
                Console.WriteLine("Excel is not installed!!");
                return;
            }
            Workbook excelBook = excelApp.Workbooks.Open(@"C:\Users\Kaze\Desktop\test.xlsx");
            _Worksheet excelSheet = excelBook.Sheets[1];
            Range excelRange = excelSheet.UsedRange;
            int rows = excelRange.Rows.Count;
            int cols = excelRange.Columns.Count;
            //Store Monthly Data
            double Current_Specific_Sum = excelRange.Cells[2, 8].Value2;
            double Current_Standard_Sum = excelRange.Cells[3, 8].Value2;
            double Current_Total_Sum = excelRange.Cells[4, 8].Value2;
            double Current_Dumps_Sum = excelRange.Cells[6, 8].Value2;
            double Current_Users_Sum = excelRange.Cells[7, 8].Value2;
            double Current_Distinct_Sum = excelRange.Cells[8, 8].Value2;
            //Read Specific Days Data
            double J1_Specific = excelRange.Cells[2, 2].Value2;
            double J2_Specific = excelRange.Cells[2, 3].Value2;
            double J3_Specific = excelRange.Cells[2, 4].Value2;
            double J4_Specific = excelRange.Cells[2, 5].Value2;
            double J5_Specific = excelRange.Cells[2, 6].Value2;
            double J6_Specific = excelRange.Cells[2, 7].Value2;
            double Specific_Sum = J1_Specific + J2_Specific + J3_Specific + J4_Specific + J5_Specific + J6_Specific;
            Console.WriteLine(Specific_Sum);
            //Read Standard Days Data
            double J1_Standard = excelRange.Cells[3, 2].Value2;
            double J2_Standard = excelRange.Cells[3, 3].Value2;
            double J3_Standard = excelRange.Cells[3, 4].Value2;
            double J4_Standard = excelRange.Cells[3, 5].Value2;
            double J5_Standard = excelRange.Cells[3, 6].Value2;
            double J6_Standard = excelRange.Cells[3, 7].Value2;
            double Standard_Sum = J1_Standard + J2_Standard + J3_Standard + J4_Standard + J5_Standard + J6_Standard;
            Console.WriteLine(Standard_Sum);
            //Read Total Days Data
            double J1_Total = excelRange.Cells[4, 2].Value2;
            double J2_Total = excelRange.Cells[4, 3].Value2;
            double J3_Total = excelRange.Cells[4, 4].Value2;
            double J4_Total = excelRange.Cells[4, 5].Value2;
            double J5_Total = excelRange.Cells[4, 6].Value2;
            double J6_Total = excelRange.Cells[4, 7].Value2;
            double Total_Sum = J1_Total + J2_Total + J3_Total + J4_Total + J5_Total + J6_Total;
            Console.WriteLine(Total_Sum);
            //Read Dumps Days Data
            double J1_Dumps = excelRange.Cells[6, 2].Value2;
            double J2_Dumps = excelRange.Cells[6, 3].Value2;
            double J3_Dumps = excelRange.Cells[6, 4].Value2;
            double J4_Dumps = excelRange.Cells[6, 5].Value2;
            double J5_Dumps = excelRange.Cells[6, 6].Value2;
            double J6_Dumps = excelRange.Cells[6, 7].Value2;
            double Dumps_Sum = J1_Dumps + J2_Dumps + J3_Dumps + J4_Dumps + J5_Dumps + J6_Dumps;
            Console.WriteLine(Dumps_Sum);
            //Read Users Days Data
            double J1_Users = excelRange.Cells[7, 2].Value2;
            double J2_Users = excelRange.Cells[7, 3].Value2;
            double J3_Users = excelRange.Cells[7, 4].Value2;
            double J4_Users = excelRange.Cells[7, 5].Value2;
            double J5_Users = excelRange.Cells[7, 6].Value2;
            double J6_Users = excelRange.Cells[7, 7].Value2;
            double Users_Sum = J1_Users + J2_Users + J3_Users + J4_Users + J5_Users + J6_Users;
            Console.WriteLine(Total_Sum);
            //Read Distinct Days Data
            double J1_Distinct = excelRange.Cells[8, 2].Value2;
            double J2_Distinct = excelRange.Cells[8, 3].Value2;
            double J3_Distinct = excelRange.Cells[8, 4].Value2;
            double J4_Distinct = excelRange.Cells[8, 5].Value2;
            double J5_Distinct = excelRange.Cells[8, 6].Value2;
            double J6_Distinct = excelRange.Cells[8, 7].Value2;
            double Distinct_Sum = J1_Distinct + J2_Distinct + J3_Distinct + J4_Distinct + J5_Distinct + J6_Distinct;
            Console.WriteLine(Distinct_Sum);
            //after reading, release the excel project
            excelBook.Close(null, null, null);
            excelApp.Quit();

            System.Runtime.InteropServices.Marshal.ReleaseComObject(excelApp);
            excelApp = null;
            Console.ReadLine();
            double new_Specific = Current_Specific_Sum + Specific_Sum;
            double new_Standard = Current_Standard_Sum + Standard_Sum;
            double new_Total = Current_Total_Sum + Total_Sum;
            double new_Dumps = Current_Dumps_Sum + Dumps_Sum;
            double new_Users = Current_Users_Sum + Users_Sum;
            double new_Distinct = Current_Distinct_Sum + Distinct_Sum;
            Console.WriteLine();
            Console.WriteLine(new_Specific);
            Console.WriteLine(new_Standard);
            Console.WriteLine(new_Total);
            Console.WriteLine(new_Dumps);
            Console.WriteLine(new_Users);
            Console.WriteLine(new_Distinct);
            Console.ReadLine();
            Process[] process = Process.GetProcessesByName("Excel");
            foreach (Process p in process)
            {
                if (!string.IsNullOrEmpty(p.ProcessName) && p.StartTime.AddSeconds(+10) > DateTime.Now)
                {
                    try
                    {
                        p.Kill();
                    }
                    catch { }
                }
            }
        }
    }

}



