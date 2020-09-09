using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Cosmos;

namespace HubbleBrowser
{
    public partial class NtpCtrl : UserControl
    {
        Star thisNode = null;
        public NtpCtrl()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //if (thisNode.ParentNode != null)
            //{
            //    string xml = "<content1 target=\"mainworkclient\">" +
            //        "<window>" +
            //            "<node startype='splitter' id='properties' rows='2' cols='2' height='300,250,100,' width='300,200,100,' middlecolor='RGB(255,224,192)'>" +
            //                "<node id='tangramtabctrl_Page2' startype="nucleus" caption=''></node>" +
            //                "<node id='test1'></node>" +
            //                "<node id='test2'></node>" +
            //                "<node id='test3'></node>" +
            //            "</node>" +
            //        "</window>" +
            //      "</content1>";

            //    thisNode.ParentNode.OpenChild(0, 0, button1.Name, xml);
            //}
        }

        private void button2_Click(object sender, EventArgs e)
        {
            //if (thisNode.ParentNode != null)
            //{
            //    string xml = "<content1>" +
            //        "<window>" +
            //            "<node startype=\"TangramTabCtrl\" objid=\"TangramTabbedWnd.TabbedComponent.1\" style=\"18\">" +
            //                "<node id='tangramtabctrl_Page2' startype="nucleus" caption='Page 1'></node>" +
            //                "<node id='test1' caption='Page 2'></node>" +
            //                "<node id='test2' caption='Page 3'></node>" +
            //                "<node id='test3' caption='Page 4'></node>" +
            //            "</node>" +
            //        "</window>" +
            //      "</content1>";

            //    thisNode.ParentNode.OpenChild(0, 0, button2.Name, xml);
            //}
        }

        private void NtpCtrl_Load(object sender, EventArgs e)
        {
            thisNode = Hubble.CreatingStar;
        }
    }

}
