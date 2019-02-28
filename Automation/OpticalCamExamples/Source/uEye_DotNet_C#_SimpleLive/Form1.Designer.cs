namespace SimpleLive
{
    partial class uEye_DotNet_Simple_Live
    {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(uEye_DotNet_Simple_Live));
            this.DisplayWindow = new System.Windows.Forms.PictureBox();
            this.Button_Live_Video = new System.Windows.Forms.Button();
            this.Button_Stop_Video = new System.Windows.Forms.Button();
            this.Button_Exit_Prog = new System.Windows.Forms.Button();
            this.Button_Load_Parameter = new System.Windows.Forms.Button();
            this.Button_Freeze_Video = new System.Windows.Forms.Button();
            this.CB_Auto_White_Balance = new System.Windows.Forms.CheckBox();
            this.CB_Auto_Gain_Balance = new System.Windows.Forms.CheckBox();
            this.button1 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.DisplayWindow)).BeginInit();
            this.SuspendLayout();
            // 
            // DisplayWindow
            // 
            this.DisplayWindow.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.DisplayWindow.Image = ((System.Drawing.Image)(resources.GetObject("DisplayWindow.Image")));
            this.DisplayWindow.Location = new System.Drawing.Point(12, 12);
            this.DisplayWindow.Name = "DisplayWindow";
            this.DisplayWindow.Size = new System.Drawing.Size(464, 362);
            this.DisplayWindow.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.DisplayWindow.TabIndex = 0;
            this.DisplayWindow.TabStop = false;
            // 
            // Button_Live_Video
            // 
            this.Button_Live_Video.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.Button_Live_Video.Location = new System.Drawing.Point(503, 12);
            this.Button_Live_Video.Name = "Button_Live_Video";
            this.Button_Live_Video.Size = new System.Drawing.Size(117, 29);
            this.Button_Live_Video.TabIndex = 1;
            this.Button_Live_Video.Text = "Start Live";
            this.Button_Live_Video.UseVisualStyleBackColor = true;
            this.Button_Live_Video.Click += new System.EventHandler(this.Button_Live_Video_Click);
            // 
            // Button_Stop_Video
            // 
            this.Button_Stop_Video.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.Button_Stop_Video.Location = new System.Drawing.Point(503, 48);
            this.Button_Stop_Video.Name = "Button_Stop_Video";
            this.Button_Stop_Video.Size = new System.Drawing.Size(117, 29);
            this.Button_Stop_Video.TabIndex = 2;
            this.Button_Stop_Video.Text = "Stop Live";
            this.Button_Stop_Video.UseVisualStyleBackColor = true;
            this.Button_Stop_Video.Click += new System.EventHandler(this.Button_Stop_Video_Click);
            // 
            // Button_Exit_Prog
            // 
            this.Button_Exit_Prog.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.Button_Exit_Prog.Location = new System.Drawing.Point(503, 345);
            this.Button_Exit_Prog.Name = "Button_Exit_Prog";
            this.Button_Exit_Prog.Size = new System.Drawing.Size(117, 29);
            this.Button_Exit_Prog.TabIndex = 3;
            this.Button_Exit_Prog.Text = "Exit";
            this.Button_Exit_Prog.UseVisualStyleBackColor = true;
            this.Button_Exit_Prog.Click += new System.EventHandler(this.Button_Exit_Prog_Click);
            // 
            // Button_Load_Parameter
            // 
            this.Button_Load_Parameter.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.Button_Load_Parameter.Location = new System.Drawing.Point(503, 119);
            this.Button_Load_Parameter.Name = "Button_Load_Parameter";
            this.Button_Load_Parameter.Size = new System.Drawing.Size(117, 29);
            this.Button_Load_Parameter.TabIndex = 4;
            this.Button_Load_Parameter.Text = "Load Parameter";
            this.Button_Load_Parameter.UseVisualStyleBackColor = true;
            this.Button_Load_Parameter.Click += new System.EventHandler(this.Button_Load_Parameter_Click);
            // 
            // Button_Freeze_Video
            // 
            this.Button_Freeze_Video.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.Button_Freeze_Video.Location = new System.Drawing.Point(503, 84);
            this.Button_Freeze_Video.Name = "Button_Freeze_Video";
            this.Button_Freeze_Video.Size = new System.Drawing.Size(117, 29);
            this.Button_Freeze_Video.TabIndex = 5;
            this.Button_Freeze_Video.Text = "Freeze Video";
            this.Button_Freeze_Video.UseVisualStyleBackColor = true;
            this.Button_Freeze_Video.Click += new System.EventHandler(this.Button_Freeze_Video_Click);
            // 
            // CB_Auto_White_Balance
            // 
            this.CB_Auto_White_Balance.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.CB_Auto_White_Balance.AutoSize = true;
            this.CB_Auto_White_Balance.Location = new System.Drawing.Point(503, 174);
            this.CB_Auto_White_Balance.Name = "CB_Auto_White_Balance";
            this.CB_Auto_White_Balance.Size = new System.Drawing.Size(121, 17);
            this.CB_Auto_White_Balance.TabIndex = 6;
            this.CB_Auto_White_Balance.Text = "Auto White Balance";
            this.CB_Auto_White_Balance.UseVisualStyleBackColor = true;
            this.CB_Auto_White_Balance.CheckedChanged += new System.EventHandler(this.CB_Auto_White_Balance_CheckedChanged);
            // 
            // CB_Auto_Gain_Balance
            // 
            this.CB_Auto_Gain_Balance.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.CB_Auto_Gain_Balance.AutoSize = true;
            this.CB_Auto_Gain_Balance.Location = new System.Drawing.Point(503, 198);
            this.CB_Auto_Gain_Balance.Name = "CB_Auto_Gain_Balance";
            this.CB_Auto_Gain_Balance.Size = new System.Drawing.Size(115, 17);
            this.CB_Auto_Gain_Balance.TabIndex = 7;
            this.CB_Auto_Gain_Balance.Text = "Auto Gain Balance";
            this.CB_Auto_Gain_Balance.UseVisualStyleBackColor = true;
            this.CB_Auto_Gain_Balance.CheckedChanged += new System.EventHandler(this.CB_Auto_Gain_Balance_CheckedChanged);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(508, 243);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(112, 27);
            this.button1.TabIndex = 8;
            this.button1.Text = "Auto shutter";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // uEye_DotNet_Simple_Live
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(641, 386);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.CB_Auto_Gain_Balance);
            this.Controls.Add(this.CB_Auto_White_Balance);
            this.Controls.Add(this.Button_Freeze_Video);
            this.Controls.Add(this.Button_Load_Parameter);
            this.Controls.Add(this.Button_Exit_Prog);
            this.Controls.Add(this.Button_Stop_Video);
            this.Controls.Add(this.Button_Live_Video);
            this.Controls.Add(this.DisplayWindow);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(649, 420);
            this.Name = "uEye_DotNet_Simple_Live";
            this.Text = "uEye C# Simple Live";
            ((System.ComponentModel.ISupportInitialize)(this.DisplayWindow)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox DisplayWindow;
        private System.Windows.Forms.Button Button_Live_Video;
        private System.Windows.Forms.Button Button_Stop_Video;
        private System.Windows.Forms.Button Button_Exit_Prog;
        private System.Windows.Forms.Button Button_Load_Parameter;
        private System.Windows.Forms.Button Button_Freeze_Video;
        private System.Windows.Forms.CheckBox CB_Auto_White_Balance;
        private System.Windows.Forms.CheckBox CB_Auto_Gain_Balance;
        private System.Windows.Forms.Button button1;
    }
}

