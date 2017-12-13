using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CSserial
{
    static class Program
    {
        public static main FormMain;
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            
            FormMain = new main();           
            Application.Run(FormMain);
             /*
            try
            {
                FormMain = new main();
                Application.Run(new Form1());
            }
            catch (Exception err)
            {
            MessageBox.Show(err.Data.ToString());
            }
              * */
        }
    }
}
