namespace Shh0yaInjectorGUI
{
    partial class Form1
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.BtnInjection = new System.Windows.Forms.Button();
            this.cmbProcess = new System.Windows.Forms.ComboBox();
            this.cmbExec = new System.Windows.Forms.ComboBox();
            this.cmbInjection = new System.Windows.Forms.ComboBox();
            this.BtnRefresh = new System.Windows.Forms.Button();
            this.BtnOpen = new System.Windows.Forms.Button();
            this.txtDll = new System.Windows.Forms.TextBox();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.gbAdvanced = new System.Windows.Forms.GroupBox();
            this.cbReflective = new System.Windows.Forms.CheckBox();
            this.BtnEjection = new System.Windows.Forms.Button();
            this.gbAdvanced.SuspendLayout();
            this.SuspendLayout();
            // 
            // BtnInjection
            // 
            this.BtnInjection.FlatAppearance.BorderSize = 0;
            this.BtnInjection.Location = new System.Drawing.Point(287, 171);
            this.BtnInjection.Name = "BtnInjection";
            this.BtnInjection.Size = new System.Drawing.Size(73, 31);
            this.BtnInjection.TabIndex = 0;
            this.BtnInjection.Text = "Injection";
            this.BtnInjection.UseVisualStyleBackColor = true;
            this.BtnInjection.Click += new System.EventHandler(this.BtnInjection_Click);
            // 
            // cmbProcess
            // 
            this.cmbProcess.FormattingEnabled = true;
            this.cmbProcess.Location = new System.Drawing.Point(15, 96);
            this.cmbProcess.Name = "cmbProcess";
            this.cmbProcess.Size = new System.Drawing.Size(234, 20);
            this.cmbProcess.TabIndex = 1;
            // 
            // cmbExec
            // 
            this.cmbExec.FormattingEnabled = true;
            this.cmbExec.Location = new System.Drawing.Point(12, 182);
            this.cmbExec.Name = "cmbExec";
            this.cmbExec.Size = new System.Drawing.Size(237, 20);
            this.cmbExec.TabIndex = 2;
            // 
            // cmbInjection
            // 
            this.cmbInjection.FormattingEnabled = true;
            this.cmbInjection.Location = new System.Drawing.Point(12, 221);
            this.cmbInjection.Name = "cmbInjection";
            this.cmbInjection.Size = new System.Drawing.Size(237, 20);
            this.cmbInjection.TabIndex = 3;
            // 
            // BtnRefresh
            // 
            this.BtnRefresh.FlatAppearance.BorderSize = 0;
            this.BtnRefresh.Location = new System.Drawing.Point(287, 90);
            this.BtnRefresh.Name = "BtnRefresh";
            this.BtnRefresh.Size = new System.Drawing.Size(73, 31);
            this.BtnRefresh.TabIndex = 4;
            this.BtnRefresh.Text = "Process";
            this.BtnRefresh.UseVisualStyleBackColor = true;
            this.BtnRefresh.Click += new System.EventHandler(this.BtnRefresh_Click);
            // 
            // BtnOpen
            // 
            this.BtnOpen.FlatAppearance.BorderSize = 0;
            this.BtnOpen.Location = new System.Drawing.Point(287, 6);
            this.BtnOpen.Name = "BtnOpen";
            this.BtnOpen.Size = new System.Drawing.Size(73, 21);
            this.BtnOpen.TabIndex = 5;
            this.BtnOpen.Text = "Open";
            this.BtnOpen.UseVisualStyleBackColor = true;
            this.BtnOpen.Click += new System.EventHandler(this.BtnOpen_Click);
            // 
            // txtDll
            // 
            this.txtDll.Location = new System.Drawing.Point(15, 33);
            this.txtDll.Name = "txtDll";
            this.txtDll.ReadOnly = true;
            this.txtDll.Size = new System.Drawing.Size(345, 21);
            this.txtDll.TabIndex = 6;
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.Filter = "DLL |*.dll";
            this.openFileDialog1.Title = "File Open";
            // 
            // gbAdvanced
            // 
            this.gbAdvanced.Controls.Add(this.cbReflective);
            this.gbAdvanced.Location = new System.Drawing.Point(380, 33);
            this.gbAdvanced.Name = "gbAdvanced";
            this.gbAdvanced.Size = new System.Drawing.Size(445, 208);
            this.gbAdvanced.TabIndex = 7;
            this.gbAdvanced.TabStop = false;
            this.gbAdvanced.Text = "Advanced";
            // 
            // cbReflective
            // 
            this.cbReflective.AutoSize = true;
            this.cbReflective.Location = new System.Drawing.Point(6, 31);
            this.cbReflective.Name = "cbReflective";
            this.cbReflective.Size = new System.Drawing.Size(129, 16);
            this.cbReflective.TabIndex = 9;
            this.cbReflective.Text = "Reflective Injection";
            this.cbReflective.UseVisualStyleBackColor = true;
            this.cbReflective.Visible = false;
            // 
            // BtnEjection
            // 
            this.BtnEjection.FlatAppearance.BorderSize = 0;
            this.BtnEjection.Location = new System.Drawing.Point(287, 208);
            this.BtnEjection.Name = "BtnEjection";
            this.BtnEjection.Size = new System.Drawing.Size(73, 31);
            this.BtnEjection.TabIndex = 8;
            this.BtnEjection.Text = "Ejection";
            this.BtnEjection.UseVisualStyleBackColor = true;
            this.BtnEjection.Click += new System.EventHandler(this.BtnEjection_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(837, 253);
            this.Controls.Add(this.BtnEjection);
            this.Controls.Add(this.gbAdvanced);
            this.Controls.Add(this.txtDll);
            this.Controls.Add(this.BtnOpen);
            this.Controls.Add(this.BtnRefresh);
            this.Controls.Add(this.cmbInjection);
            this.Controls.Add(this.cmbExec);
            this.Controls.Add(this.cmbProcess);
            this.Controls.Add(this.BtnInjection);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "Shh0ya Injector";
            this.gbAdvanced.ResumeLayout(false);
            this.gbAdvanced.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button BtnInjection;
        public System.Windows.Forms.ComboBox cmbExec;
        public System.Windows.Forms.ComboBox cmbInjection;
        private System.Windows.Forms.Button BtnRefresh;
        public System.Windows.Forms.ComboBox cmbProcess;
        private System.Windows.Forms.Button BtnOpen;
        public System.Windows.Forms.TextBox txtDll;
        public System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.GroupBox gbAdvanced;
        private System.Windows.Forms.Button BtnEjection;
        public System.Windows.Forms.CheckBox cbReflective;
    }
}

