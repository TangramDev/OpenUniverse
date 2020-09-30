using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Cas
{
    public class WebAgent
    {
        public static int Load(string args)
        {
            Cosmos.Hubble.OnBindCLRObjToWebPage += Tangram_OnBindCLRObjToWebPage;
            return 0;
        }

        private static void Tangram_OnBindCLRObjToWebPage(object SourceObj, Cosmos.Wormhole eventSession, string eventName)
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
                default:
                    break;
            }
        }

        private static void Button_Click(object sender, EventArgs e)
        {
            Cosmos.Wormhole session = null;
            if (Cosmos.Hubble.Wormholes.TryGetValue(sender, out session))
            {
                Button button = sender as Button;
                if (button != null)
                {
                    session.InsertString("msgID", "FIRE_EVENT");
                    session.InsertString("currentsubobj", button.Name);
                    session.InsertString("currentevent", "OnClick@" + button.Name);
                    session.SendMessage();
                }
            }
        }
    }
}
