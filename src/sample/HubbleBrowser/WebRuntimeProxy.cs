using System;
using System.Xml;
using Cosmos;
using System.Windows.Forms;

namespace Sunny
{
    static class WebDelegate
    {
        private static int Init(string argument)
        {
            Hubble.OnBindCLRObjToWebPage += Cosmos_OnBindCLRObjToWebPage;
            Hubble.OnGetSubObjForWebPage += Tangram_OnGetSubObjForWebPage;
            Hubble.OnHubbleMsg += Cosmos_OnHubbleMsg;
            Hubble.OnCustomizedDOMElement += Cosmos_OnCustomizedDOMElement;
            Hubble.OnHubbleMsgReceived += Cosmos_OnHubbleMsgReceived;
            return 0;
        }

        private static void Cosmos_OnHubbleMsgReceived(Wormhole cloudWormhole)
        {
            string strMsgID = cloudWormhole.GetString("msgID");
            IntPtr nHandle = (IntPtr)cloudWormhole.GetInt64("nodehandle");
        }

        private static object Tangram_OnGetSubObjForWebPage(object SourceObj, string subObjName)
        {
            return null;
        }

        private static void Cosmos_OnHubbleMsg(IntPtr hWnd, string strType, string strParam1, string strParam2)
        {
        }

        private static void Cosmos_OnCustomizedDOMElement(IntPtr hWnd, string strRuleName, string strHTML)
        {
        }

        private static void Cosmos_OnBindCLRObjToWebPage(object SourceObj, Wormhole eventWormhole, string eventName)
        {
            switch (eventName)
            {
                case "SizeChanged":
                    {
                        Form thisForm = SourceObj as Form;
                        thisForm.SizeChanged += ThisForm_SizeChanged;
                    }
                    break;
                case "OnClick":
                    {
                        Button thisbtn = SourceObj as Button;
                        thisbtn.Click += Thisbtn_Click;
                    }
                    break;
                case "TextChanged":
                    {
                        TextBox textBox = SourceObj as TextBox;
                        if (textBox != null)
                        {
                            textBox.TextChanged += TextBox_TextChanged;
                        }
                    }
                    break;
                case "OnMyCustomClick":
                    {
                        DemoComponents.ucButtonsAndTextBoxes ucButtonsAndTextBoxes = SourceObj as DemoComponents.ucButtonsAndTextBoxes;
                        if (ucButtonsAndTextBoxes != null)
                        {
                            ucButtonsAndTextBoxes.MyCustomClick += UcButtonsAndTextBoxes_MyCustomClick;
                        }
                    }
                    break;
                case "OnAfterSelect":
                    {
                        TreeView thisTreeview = SourceObj as TreeView;
                        thisTreeview.AfterSelect += ThisTreeview_AfterSelect;
                    }
                    break;
            }
        }

        private static void UcButtonsAndTextBoxes_MyCustomClick(object sender, DemoComponents.ucButtonsAndTextBoxes.MyEventArgs e)
        {
            if (Cosmos.Hubble.Wormholes.TryGetValue(sender, out Wormhole thisWormhole))
            {
                Control ctrl = sender as Control;
                if (ctrl != null)
                {
                    thisWormhole.InsertString("msgID", "FIRE_EVENT");
                    thisWormhole.InsertString("currentsubobj", ctrl.Name);
                    thisWormhole.InsertString("currentevent", "OnMyCustomClick");
                    thisWormhole.InsertString("text1", e.Text1);
                    thisWormhole.InsertString("text2", e.Text2);
                    thisWormhole.InsertString("text3", e.Text3);
                    thisWormhole.SendMessage();
                }
            }
        }

        private static void TextBox_TextChanged(object sender, EventArgs e)
        {
            if (Cosmos.Hubble.Wormholes.TryGetValue(sender, out Wormhole thisSession))
            {
                TextBox ctrl = sender as TextBox;
                if (ctrl != null)
                {
                    thisSession.InsertString("msgID", "FIRE_EVENT");
                    thisSession.InsertString("currentsubobj", ctrl.Name);
                    thisSession.InsertString("currentevent", "TextChanged@" + ctrl.Name);
                    thisSession.InsertString("text", ctrl.Text);
                    thisSession.SendMessage();
                }
            }
        }

        private static void ThisTreeview_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (Hubble.Wormholes.TryGetValue(sender, out Wormhole thisSession))
            {
                if (e.Node.Tag != null)
                {
                    string strTag = e.Node.Tag.ToString();
                    XmlDocument xml = new XmlDocument();
                    xml.LoadXml(strTag);
                    foreach (XmlAttribute attribute in xml.ChildNodes[0].Attributes)
                    {
                        thisSession.InsertString(attribute.Name, attribute.Value);
                    }
                }
                TreeView treeview = sender as TreeView;
                thisSession.InsertString("msgID", "FIRE_EVENT");
                thisSession.InsertInt64("subobjhandle", treeview.Handle.ToInt64());
                thisSession.InsertString("currentevent", "OnAfterSelect@" + treeview.Name);
                thisSession.InsertString("currentsubobj", treeview.Name);
                Control ctrl = sender as Control;
                if (ctrl != null && ctrl.Tag != null)
                {
                    string strTag = ctrl.Tag.ToString();
                    XmlDocument xml = new XmlDocument();
                    xml.LoadXml(strTag);
                    foreach (XmlAttribute attribute in xml.ChildNodes[0].Attributes)
                    {
                        thisSession.InsertString(attribute.Name, attribute.Value);
                    }
                }
                thisSession.SendMessage();
            }
        }

        private static void Thisbtn_Click(object sender, EventArgs e)
        {
            if (Cosmos.Hubble.Wormholes.TryGetValue(sender, out Wormhole thisSession))
            {
                Control ctrl = sender as Control;
                thisSession.InsertString("msgID", "FIRE_EVENT");
                thisSession.InsertString("currentsubobj", ctrl.Name);
                thisSession.InsertString("currentevent", "OnClick@" + ctrl.Name);
                if (ctrl != null && ctrl.Tag != null)
                {
                    string strTag = ctrl.Tag.ToString();
                    XmlDocument xml = new XmlDocument();
                    xml.LoadXml(strTag);
                    foreach (XmlAttribute attribute in xml.ChildNodes[0].Attributes)
                    {
                        thisSession.InsertString(attribute.Name, attribute.Value);
                    }
                }
                thisSession.SendMessage();
            }
        }

        private static void ThisForm_SizeChanged(object sender, EventArgs e)
        {
            if (Hubble.Wormholes.TryGetValue(sender, out Wormhole thisSession))
            {
                Form xform = sender as Form;
                thisSession.InsertString("msgID", "FIRE_EVENT");
                thisSession.InsertString("currentevent", "SizeChanged");
                thisSession.InsertString("currentsubobj", "SizeChanged");
                thisSession.InsertLong("width", xform.Width);
                thisSession.InsertLong("height", xform.Height);
                thisSession.SendMessage();
            }
        }
    }
}
