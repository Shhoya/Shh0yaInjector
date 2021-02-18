using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Shh0yaInjectorGUI;

namespace Shh0yaInjectorGUI
{
    public partial class Injector
    {

        [DllImport("Shh0yaInjector.dll")]
        public unsafe static extern void InjectMain(int pid, char[] path, int exec, int dll);
        [DllImport("Shh0yaInjector.dll")]
        public unsafe static extern void EjectMain(int pid, char[] path);

        public unsafe static void DllInjection(int pid, char[] path, bool bInject)
        {
            if (bInject)
            {
                InjectMain(pid, path, Form1.FormData.cmbExec.SelectedIndex, Form1.FormData.cmbInjection.SelectedIndex);
            }
            else
            {
                EjectMain(pid, path);
            }

        }
    }
}
