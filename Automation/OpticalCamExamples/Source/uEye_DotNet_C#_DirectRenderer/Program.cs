using System;
using System.Windows.Forms;

namespace uEye_DotNet_Direct_Renderer
{
    static class Program
    {
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new uEye_DotNet_Direct_Renderer());
        }
    }
}
