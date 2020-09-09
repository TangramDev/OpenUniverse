using System;
using System.Windows.Forms;
using Cosmos;

namespace HubbleBrowser
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Hubble.Run(new Form1());
        }
    }
}
