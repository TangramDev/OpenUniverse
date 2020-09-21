using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Cosmos;

namespace HubbleBrowser
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string xml = "<content1>" +
                "<layout>" +
                    "<grid objid=\"TangramTabbedWnd.TabbedComponent.1\" style=\"13\">" +
                        "<grid id='tangramtabctrl_Page2 www' gridtype=\"nucleus\" caption='Page 1'></grid>" +
                        "<grid id='test1' caption='Page 2 eee'></grid>" +
                        "<grid id='test2' caption='Page 3'></grid>" +
                        "<grid id='test3' caption='Page 4'></grid>" +
                    "</grid>" +
                "</layout>" +
              "</content1>";
            Grid node = Hubble.Observe(listView1, "test", xml);
        }
    }
}
