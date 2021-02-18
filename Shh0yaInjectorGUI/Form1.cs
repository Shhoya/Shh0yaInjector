using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Shh0yaInjectorGUI
{
    public partial class Form1 : Form
    {
        public static Form1 FormData;
        string FilePath;
        ComboBox tmpCmbBox;
        public Form1()
        {
            InitializeComponent();
            FormData = this;

            cmbExec.Items.Add("CreateRemoteThread");
            cmbExec.Items.Add("NtCreateEx");
            cmbExec.Items.Add("Thread Hijacking");
            cmbExec.Items.Add("NtQueueApcThread");

            cmbInjection.Items.Add("LoadLibrary");
            cmbInjection.Items.Add("LdrLoadDll");
            cmbInjection.Items.Add("ManualMapping");


            cmbExec.SelectedIndex = 0;
            cmbInjection.SelectedIndex = 0;


        }

        public void BtnInjection_Click(object sender, EventArgs e)
        {
            if(cmbProcess.SelectedItem == null)
            {
                MessageBox.Show("Choose the process to inject first");
                return;
            }
            int end = cmbProcess.SelectedItem.ToString().IndexOf("]");
            int start = cmbProcess.SelectedItem.ToString().IndexOf("[");
            int pid = Convert.ToInt32(cmbProcess.SelectedItem.ToString().Substring(start+1, end-start-1));
 
            Injector.DllInjection(pid,FilePath.ToCharArray(),true);
        }

        private void BtnRefresh_Click(object sender, EventArgs e)
        {
            cmbProcess.Items.Clear();
            Process[] Processes = Process.GetProcesses();
            foreach(Process p in Processes.OrderBy(m=>m.ProcessName))
            {
                cmbProcess.Items.Add(p.ProcessName+" [" + p.Id + "]" );
            }
        }

        private void BtnOpen_Click(object sender, EventArgs e)
        {
            FilePath = null;
            if(openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                FilePath = openFileDialog1.FileName;
                txtDll.Text = FilePath;
            }
        }

        private void BtnEjection_Click(object sender, EventArgs e)
        {
            if (cmbProcess.SelectedItem == null)
            {
                MessageBox.Show("Choose the process to inject first");
                return;
            }
            int end = cmbProcess.SelectedItem.ToString().IndexOf("]");
            int start = cmbProcess.SelectedItem.ToString().IndexOf("[");
            int pid = Convert.ToInt32(cmbProcess.SelectedItem.ToString().Substring(start + 1, end - start - 1));

            Injector.DllInjection(pid, FilePath.ToCharArray(),false);
        }
    }
}
