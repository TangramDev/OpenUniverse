using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DemoComponents
{
    public partial class ucButtonsAndTextBoxes : UserControl
    {
        public class MyEventArgs : EventArgs
        {
            public string Text1 { get; set; }
            public string Text2 { get; set; }
            public string Text3 { get; set; }

            public MyEventArgs(string text1, string text2, string text3)
            {
                Text1 = text1;
                Text2 = text2;
                Text3 = text3;
            }
        }

        public event EventHandler<MyEventArgs> MyCustomClick;

        public ucButtonsAndTextBoxes()
        {
            InitializeComponent();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (MyCustomClick != null)
            {
                MyCustomClick(this, new MyEventArgs(textBox1.Text, textBox2.Text, textBox3.Text));
            }
        }
    }
}
