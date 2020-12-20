using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;

namespace Cas
{
    public class WebAgent
    {
        public static int Load(string args)
        {
            Cosmos.Cosmos.OnBindCLRObjToWebPage += Tangram_OnBindCLRObjToWebPage;
            Cosmos.Cosmos.OnHubbleMsgReceived += Tangram_OnTangramCloudMsgReceived;
            return 0;
        }

        private static void Tangram_OnTangramCloudMsgReceived(Cosmos.Wormhole cloudWormhole)
        {
            Console.WriteLine("msgID: " + cloudWormhole.GetString("msgID"));
        }

        private static void Tangram_OnBindCLRObjToWebPage(object SourceObj, Cosmos.Wormhole eventWormhole, string eventName)
        {
            switch (eventName)
            {
                case "OnClick":
                    Button button = SourceObj as Button;
                    if (button != null)
                    {
                        button.Click += Button_Click;
                    }
                    break;
                case "OnAfterSelect":
                    TreeView treeView = SourceObj as TreeView;
                    if (treeView != null)
                    {
                        treeView.AfterSelect += TreeView_AfterSelect;
                    }
                    break;
                default:
                    break;
            }
        }

        private static void TreeView_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (Cosmos.Cosmos.Wormholes.TryGetValue(sender, out Cosmos.Wormhole cloudWormhole))
            {
                if (e.Node.Tag != null)
                {
                    XmlDocument xml = new XmlDocument();
                    xml.LoadXml(e.Node.Tag as String);
                    foreach (XmlAttribute attribute in xml.ChildNodes[0].Attributes)
                    {
                        cloudWormhole.InsertString(attribute.Name, attribute.Value);
                    }
                }
                TreeView treeView = sender as TreeView;
                cloudWormhole.InsertString("msgID", "FIRE_EVENT");
                cloudWormhole.InsertLong("message", 20201204);
                cloudWormhole.InsertInt64("subobjhandle", treeView.Handle.ToInt64());
                cloudWormhole.InsertString("currentevent", "OnAfterSelect@" + treeView.Name);
                cloudWormhole.InsertString("currentsubobj", treeView.Name);
                cloudWormhole.SendMessage();
            }
        }

        private static void Button_Click(object sender, EventArgs e)
        {
            if (Cosmos.Cosmos.Wormholes.TryGetValue(sender, out Cosmos.Wormhole cloudWormhole))
            {
                Button button = sender as Button;
                if (button != null)
                {
                    cloudWormhole.InsertString("msgID", "FIRE_EVENT");
                    cloudWormhole.InsertLong("message", 20201204);
                    cloudWormhole.InsertString("currentsubobj", button.Name);
                    cloudWormhole.InsertString("currentevent", "OnClick@" + button.Name);
                    cloudWormhole.SendMessage();
                }
            }
        }
    }
}
