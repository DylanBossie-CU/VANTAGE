using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace SimpleLive
{
    static class Program
    {
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new uEye_DotNet_Simple_Live());
        }
    }
}
