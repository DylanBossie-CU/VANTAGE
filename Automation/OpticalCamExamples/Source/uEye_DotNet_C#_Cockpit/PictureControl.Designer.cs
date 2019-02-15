namespace DotnetCockpit
{
    partial class PictureControl
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

        #region Vom Komponenten-Designer generierter Code

        /// <summary> 
        /// Erforderliche Methode für die Designerunterstützung. 
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.groupBoxWhitebalance = new System.Windows.Forms.GroupBox();
            this.radioButtonWhitebalanceOnce = new System.Windows.Forms.RadioButton();
            this.radioButtonWhitebalanceAuto = new System.Windows.Forms.RadioButton();
            this.radioButtonWhitebalanceDisabled = new System.Windows.Forms.RadioButton();
            this.labelGainBlueMax = new System.Windows.Forms.Label();
            this.labelGainBlueMin = new System.Windows.Forms.Label();
            this.labelGainGreenMax = new System.Windows.Forms.Label();
            this.labelGainGreenMin = new System.Windows.Forms.Label();
            this.labelGainRedMax = new System.Windows.Forms.Label();
            this.labelGainRedMin = new System.Windows.Forms.Label();
            this.trackBarGainBlue = new System.Windows.Forms.TrackBar();
            this.numericUpDownGainBlue = new System.Windows.Forms.NumericUpDown();
            this.labelGainBlue = new System.Windows.Forms.Label();
            this.trackBarGainGreen = new System.Windows.Forms.TrackBar();
            this.numericUpDownGainGreen = new System.Windows.Forms.NumericUpDown();
            this.labelGainGreen = new System.Windows.Forms.Label();
            this.trackBarGainRed = new System.Windows.Forms.TrackBar();
            this.numericUpDownGainRed = new System.Windows.Forms.NumericUpDown();
            this.labelGainRed = new System.Windows.Forms.Label();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.checkBoxMasterGainBoost = new System.Windows.Forms.CheckBox();
            this.checkBoxMasterGainAuto = new System.Windows.Forms.CheckBox();
            this.labelGainMasterMax = new System.Windows.Forms.Label();
            this.labelGainMasterMin = new System.Windows.Forms.Label();
            this.trackBarGainMaster = new System.Windows.Forms.TrackBar();
            this.numericUpDownGainMaster = new System.Windows.Forms.NumericUpDown();
            this.labelGainMaster = new System.Windows.Forms.Label();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.groupBoxGamma = new System.Windows.Forms.GroupBox();
            this.checkBoxHardwareGamma = new System.Windows.Forms.CheckBox();
            this.checkBoxGammaEnable = new System.Windows.Forms.CheckBox();
            this.labelGammaMax = new System.Windows.Forms.Label();
            this.labelGammaMin = new System.Windows.Forms.Label();
            this.trackBarGamma = new System.Windows.Forms.TrackBar();
            this.numericUpDownGamma = new System.Windows.Forms.NumericUpDown();
            this.labelGamma = new System.Windows.Forms.Label();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.groupBoxWhitebalance.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarGainBlue)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownGainBlue)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarGainGreen)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownGainGreen)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarGainRed)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownGainRed)).BeginInit();
            this.tabPage2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarGainMaster)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownGainMaster)).BeginInit();
            this.tabPage3.SuspendLayout();
            this.groupBoxGamma.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarGamma)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownGamma)).BeginInit();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(642, 405);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.groupBoxWhitebalance);
            this.tabPage1.Controls.Add(this.labelGainBlueMax);
            this.tabPage1.Controls.Add(this.labelGainBlueMin);
            this.tabPage1.Controls.Add(this.labelGainGreenMax);
            this.tabPage1.Controls.Add(this.labelGainGreenMin);
            this.tabPage1.Controls.Add(this.labelGainRedMax);
            this.tabPage1.Controls.Add(this.labelGainRedMin);
            this.tabPage1.Controls.Add(this.trackBarGainBlue);
            this.tabPage1.Controls.Add(this.numericUpDownGainBlue);
            this.tabPage1.Controls.Add(this.labelGainBlue);
            this.tabPage1.Controls.Add(this.trackBarGainGreen);
            this.tabPage1.Controls.Add(this.numericUpDownGainGreen);
            this.tabPage1.Controls.Add(this.labelGainGreen);
            this.tabPage1.Controls.Add(this.trackBarGainRed);
            this.tabPage1.Controls.Add(this.numericUpDownGainRed);
            this.tabPage1.Controls.Add(this.labelGainRed);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(634, 379);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Color Gain";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // groupBoxWhitebalance
            // 
            this.groupBoxWhitebalance.Controls.Add(this.radioButtonWhitebalanceOnce);
            this.groupBoxWhitebalance.Controls.Add(this.radioButtonWhitebalanceAuto);
            this.groupBoxWhitebalance.Controls.Add(this.radioButtonWhitebalanceDisabled);
            this.groupBoxWhitebalance.Location = new System.Drawing.Point(9, 213);
            this.groupBoxWhitebalance.Name = "groupBoxWhitebalance";
            this.groupBoxWhitebalance.Size = new System.Drawing.Size(453, 108);
            this.groupBoxWhitebalance.TabIndex = 52;
            this.groupBoxWhitebalance.TabStop = false;
            this.groupBoxWhitebalance.Text = "Whitebalance";
            // 
            // radioButtonWhitebalanceOnce
            // 
            this.radioButtonWhitebalanceOnce.AutoSize = true;
            this.radioButtonWhitebalanceOnce.Location = new System.Drawing.Point(7, 66);
            this.radioButtonWhitebalanceOnce.Name = "radioButtonWhitebalanceOnce";
            this.radioButtonWhitebalanceOnce.Size = new System.Drawing.Size(51, 17);
            this.radioButtonWhitebalanceOnce.TabIndex = 2;
            this.radioButtonWhitebalanceOnce.TabStop = true;
            this.radioButtonWhitebalanceOnce.Text = "Once";
            this.radioButtonWhitebalanceOnce.UseVisualStyleBackColor = true;
            this.radioButtonWhitebalanceOnce.CheckedChanged += new System.EventHandler(this.radioButtonWhitebalanceOnce_CheckedChanged);
            // 
            // radioButtonWhitebalanceAuto
            // 
            this.radioButtonWhitebalanceAuto.AutoSize = true;
            this.radioButtonWhitebalanceAuto.Location = new System.Drawing.Point(6, 43);
            this.radioButtonWhitebalanceAuto.Name = "radioButtonWhitebalanceAuto";
            this.radioButtonWhitebalanceAuto.Size = new System.Drawing.Size(47, 17);
            this.radioButtonWhitebalanceAuto.TabIndex = 1;
            this.radioButtonWhitebalanceAuto.TabStop = true;
            this.radioButtonWhitebalanceAuto.Text = "Auto";
            this.radioButtonWhitebalanceAuto.UseVisualStyleBackColor = true;
            this.radioButtonWhitebalanceAuto.CheckedChanged += new System.EventHandler(this.radioButtonWhitebalanceAuto_CheckedChanged);
            // 
            // radioButtonWhitebalanceDisabled
            // 
            this.radioButtonWhitebalanceDisabled.AutoSize = true;
            this.radioButtonWhitebalanceDisabled.Location = new System.Drawing.Point(7, 20);
            this.radioButtonWhitebalanceDisabled.Name = "radioButtonWhitebalanceDisabled";
            this.radioButtonWhitebalanceDisabled.Size = new System.Drawing.Size(66, 17);
            this.radioButtonWhitebalanceDisabled.TabIndex = 0;
            this.radioButtonWhitebalanceDisabled.TabStop = true;
            this.radioButtonWhitebalanceDisabled.Text = "Disabled";
            this.radioButtonWhitebalanceDisabled.UseVisualStyleBackColor = true;
            this.radioButtonWhitebalanceDisabled.CheckedChanged += new System.EventHandler(this.radioButtonWhitebalanceDisabled_CheckedChanged);
            // 
            // labelGainBlueMax
            // 
            this.labelGainBlueMax.AutoSize = true;
            this.labelGainBlueMax.Location = new System.Drawing.Point(363, 174);
            this.labelGainBlueMax.Name = "labelGainBlueMax";
            this.labelGainBlueMax.Size = new System.Drawing.Size(26, 13);
            this.labelGainBlueMax.TabIndex = 48;
            this.labelGainBlueMax.Text = "max";
            // 
            // labelGainBlueMin
            // 
            this.labelGainBlueMin.AutoSize = true;
            this.labelGainBlueMin.Location = new System.Drawing.Point(111, 174);
            this.labelGainBlueMin.Name = "labelGainBlueMin";
            this.labelGainBlueMin.Size = new System.Drawing.Size(23, 13);
            this.labelGainBlueMin.TabIndex = 47;
            this.labelGainBlueMin.Text = "min";
            // 
            // labelGainGreenMax
            // 
            this.labelGainGreenMax.AutoSize = true;
            this.labelGainGreenMax.Location = new System.Drawing.Point(363, 110);
            this.labelGainGreenMax.Name = "labelGainGreenMax";
            this.labelGainGreenMax.Size = new System.Drawing.Size(26, 13);
            this.labelGainGreenMax.TabIndex = 43;
            this.labelGainGreenMax.Text = "max";
            // 
            // labelGainGreenMin
            // 
            this.labelGainGreenMin.AutoSize = true;
            this.labelGainGreenMin.Location = new System.Drawing.Point(110, 110);
            this.labelGainGreenMin.Name = "labelGainGreenMin";
            this.labelGainGreenMin.Size = new System.Drawing.Size(23, 13);
            this.labelGainGreenMin.TabIndex = 42;
            this.labelGainGreenMin.Text = "min";
            // 
            // labelGainRedMax
            // 
            this.labelGainRedMax.AutoSize = true;
            this.labelGainRedMax.Location = new System.Drawing.Point(363, 46);
            this.labelGainRedMax.Name = "labelGainRedMax";
            this.labelGainRedMax.Size = new System.Drawing.Size(26, 13);
            this.labelGainRedMax.TabIndex = 38;
            this.labelGainRedMax.Text = "max";
            // 
            // labelGainRedMin
            // 
            this.labelGainRedMin.AutoSize = true;
            this.labelGainRedMin.Location = new System.Drawing.Point(111, 46);
            this.labelGainRedMin.Name = "labelGainRedMin";
            this.labelGainRedMin.Size = new System.Drawing.Size(23, 13);
            this.labelGainRedMin.TabIndex = 37;
            this.labelGainRedMin.Text = "min";
            // 
            // trackBarGainBlue
            // 
            this.trackBarGainBlue.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.trackBarGainBlue.Location = new System.Drawing.Point(95, 142);
            this.trackBarGainBlue.Name = "trackBarGainBlue";
            this.trackBarGainBlue.Size = new System.Drawing.Size(294, 45);
            this.trackBarGainBlue.TabIndex = 50;
            this.trackBarGainBlue.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBarGainBlue.Scroll += new System.EventHandler(this.trackBarGainBlue_Scroll);
            // 
            // numericUpDownGainBlue
            // 
            this.numericUpDownGainBlue.Location = new System.Drawing.Point(395, 140);
            this.numericUpDownGainBlue.Name = "numericUpDownGainBlue";
            this.numericUpDownGainBlue.Size = new System.Drawing.Size(67, 20);
            this.numericUpDownGainBlue.TabIndex = 49;
            this.numericUpDownGainBlue.ValueChanged += new System.EventHandler(this.numericUpDownGainBlue_ValueChanged);
            // 
            // labelGainBlue
            // 
            this.labelGainBlue.AutoSize = true;
            this.labelGainBlue.Location = new System.Drawing.Point(6, 142);
            this.labelGainBlue.Name = "labelGainBlue";
            this.labelGainBlue.Size = new System.Drawing.Size(51, 13);
            this.labelGainBlue.TabIndex = 46;
            this.labelGainBlue.Text = "Blue gain";
            // 
            // trackBarGainGreen
            // 
            this.trackBarGainGreen.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.trackBarGainGreen.Location = new System.Drawing.Point(95, 78);
            this.trackBarGainGreen.Name = "trackBarGainGreen";
            this.trackBarGainGreen.Size = new System.Drawing.Size(294, 45);
            this.trackBarGainGreen.TabIndex = 45;
            this.trackBarGainGreen.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBarGainGreen.Scroll += new System.EventHandler(this.trackBarGainGreen_Scroll);
            // 
            // numericUpDownGainGreen
            // 
            this.numericUpDownGainGreen.Location = new System.Drawing.Point(395, 76);
            this.numericUpDownGainGreen.Name = "numericUpDownGainGreen";
            this.numericUpDownGainGreen.Size = new System.Drawing.Size(67, 20);
            this.numericUpDownGainGreen.TabIndex = 44;
            this.numericUpDownGainGreen.ValueChanged += new System.EventHandler(this.numericUpDownGainGreen_ValueChanged);
            // 
            // labelGainGreen
            // 
            this.labelGainGreen.AutoSize = true;
            this.labelGainGreen.Location = new System.Drawing.Point(6, 78);
            this.labelGainGreen.Name = "labelGainGreen";
            this.labelGainGreen.Size = new System.Drawing.Size(59, 13);
            this.labelGainGreen.TabIndex = 41;
            this.labelGainGreen.Text = "Green gain";
            // 
            // trackBarGainRed
            // 
            this.trackBarGainRed.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.trackBarGainRed.Location = new System.Drawing.Point(95, 14);
            this.trackBarGainRed.Name = "trackBarGainRed";
            this.trackBarGainRed.Size = new System.Drawing.Size(294, 45);
            this.trackBarGainRed.TabIndex = 40;
            this.trackBarGainRed.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBarGainRed.Scroll += new System.EventHandler(this.trackBarGainRed_Scroll);
            // 
            // numericUpDownGainRed
            // 
            this.numericUpDownGainRed.Location = new System.Drawing.Point(395, 12);
            this.numericUpDownGainRed.Name = "numericUpDownGainRed";
            this.numericUpDownGainRed.Size = new System.Drawing.Size(67, 20);
            this.numericUpDownGainRed.TabIndex = 39;
            this.numericUpDownGainRed.ValueChanged += new System.EventHandler(this.numericUpDownGainRed_ValueChanged);
            // 
            // labelGainRed
            // 
            this.labelGainRed.AutoSize = true;
            this.labelGainRed.Location = new System.Drawing.Point(6, 14);
            this.labelGainRed.Name = "labelGainRed";
            this.labelGainRed.Size = new System.Drawing.Size(50, 13);
            this.labelGainRed.TabIndex = 36;
            this.labelGainRed.Text = "Red gain";
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.checkBoxMasterGainBoost);
            this.tabPage2.Controls.Add(this.checkBoxMasterGainAuto);
            this.tabPage2.Controls.Add(this.labelGainMasterMax);
            this.tabPage2.Controls.Add(this.labelGainMasterMin);
            this.tabPage2.Controls.Add(this.trackBarGainMaster);
            this.tabPage2.Controls.Add(this.numericUpDownGainMaster);
            this.tabPage2.Controls.Add(this.labelGainMaster);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(634, 379);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Master gain";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // checkBoxMasterGainBoost
            // 
            this.checkBoxMasterGainBoost.AutoSize = true;
            this.checkBoxMasterGainBoost.Location = new System.Drawing.Point(9, 98);
            this.checkBoxMasterGainBoost.Name = "checkBoxMasterGainBoost";
            this.checkBoxMasterGainBoost.Size = new System.Drawing.Size(77, 17);
            this.checkBoxMasterGainBoost.TabIndex = 26;
            this.checkBoxMasterGainBoost.Text = "Gain boost";
            this.checkBoxMasterGainBoost.UseVisualStyleBackColor = true;
            this.checkBoxMasterGainBoost.CheckedChanged += new System.EventHandler(this.checkBoxMasterGainBoost_CheckedChanged);
            // 
            // checkBoxMasterGainAuto
            // 
            this.checkBoxMasterGainAuto.AutoSize = true;
            this.checkBoxMasterGainAuto.Location = new System.Drawing.Point(9, 75);
            this.checkBoxMasterGainAuto.Name = "checkBoxMasterGainAuto";
            this.checkBoxMasterGainAuto.Size = new System.Drawing.Size(48, 17);
            this.checkBoxMasterGainAuto.TabIndex = 25;
            this.checkBoxMasterGainAuto.Text = "Auto";
            this.checkBoxMasterGainAuto.UseVisualStyleBackColor = true;
            this.checkBoxMasterGainAuto.CheckedChanged += new System.EventHandler(this.checkBoxMasterGainAuto_CheckedChanged);
            // 
            // labelGainMasterMax
            // 
            this.labelGainMasterMax.AutoSize = true;
            this.labelGainMasterMax.Location = new System.Drawing.Point(363, 47);
            this.labelGainMasterMax.Name = "labelGainMasterMax";
            this.labelGainMasterMax.Size = new System.Drawing.Size(26, 13);
            this.labelGainMasterMax.TabIndex = 22;
            this.labelGainMasterMax.Text = "max";
            // 
            // labelGainMasterMin
            // 
            this.labelGainMasterMin.AutoSize = true;
            this.labelGainMasterMin.Location = new System.Drawing.Point(111, 47);
            this.labelGainMasterMin.Name = "labelGainMasterMin";
            this.labelGainMasterMin.Size = new System.Drawing.Size(23, 13);
            this.labelGainMasterMin.TabIndex = 21;
            this.labelGainMasterMin.Text = "min";
            // 
            // trackBarGainMaster
            // 
            this.trackBarGainMaster.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.trackBarGainMaster.Location = new System.Drawing.Point(95, 15);
            this.trackBarGainMaster.Name = "trackBarGainMaster";
            this.trackBarGainMaster.Size = new System.Drawing.Size(294, 45);
            this.trackBarGainMaster.TabIndex = 24;
            this.trackBarGainMaster.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBarGainMaster.Scroll += new System.EventHandler(this.trackBarGainMaster_Scroll);
            // 
            // numericUpDownGainMaster
            // 
            this.numericUpDownGainMaster.Location = new System.Drawing.Point(395, 15);
            this.numericUpDownGainMaster.Name = "numericUpDownGainMaster";
            this.numericUpDownGainMaster.Size = new System.Drawing.Size(67, 20);
            this.numericUpDownGainMaster.TabIndex = 23;
            this.numericUpDownGainMaster.ValueChanged += new System.EventHandler(this.numericUpDownGainMaster_ValueChanged);
            // 
            // labelGainMaster
            // 
            this.labelGainMaster.AutoSize = true;
            this.labelGainMaster.Location = new System.Drawing.Point(6, 15);
            this.labelGainMaster.Name = "labelGainMaster";
            this.labelGainMaster.Size = new System.Drawing.Size(62, 13);
            this.labelGainMaster.TabIndex = 20;
            this.labelGainMaster.Text = "Master gain";
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.groupBoxGamma);
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(634, 379);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Gamma";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // groupBoxGamma
            // 
            this.groupBoxGamma.Controls.Add(this.checkBoxHardwareGamma);
            this.groupBoxGamma.Controls.Add(this.checkBoxGammaEnable);
            this.groupBoxGamma.Controls.Add(this.labelGammaMax);
            this.groupBoxGamma.Controls.Add(this.labelGammaMin);
            this.groupBoxGamma.Controls.Add(this.trackBarGamma);
            this.groupBoxGamma.Controls.Add(this.numericUpDownGamma);
            this.groupBoxGamma.Controls.Add(this.labelGamma);
            this.groupBoxGamma.Location = new System.Drawing.Point(6, 6);
            this.groupBoxGamma.Name = "groupBoxGamma";
            this.groupBoxGamma.Size = new System.Drawing.Size(480, 93);
            this.groupBoxGamma.TabIndex = 33;
            this.groupBoxGamma.TabStop = false;
            // 
            // checkBoxHardwareGamma
            // 
            this.checkBoxHardwareGamma.AutoSize = true;
            this.checkBoxHardwareGamma.Location = new System.Drawing.Point(13, 70);
            this.checkBoxHardwareGamma.Name = "checkBoxHardwareGamma";
            this.checkBoxHardwareGamma.Size = new System.Drawing.Size(143, 17);
            this.checkBoxHardwareGamma.TabIndex = 31;
            this.checkBoxHardwareGamma.Text = "Hardware sensor gamma";
            this.checkBoxHardwareGamma.UseVisualStyleBackColor = true;
            this.checkBoxHardwareGamma.CheckedChanged += new System.EventHandler(this.checkBoxHardwareGamma_CheckedChanged);
            // 
            // checkBoxGammaEnable
            // 
            this.checkBoxGammaEnable.AutoSize = true;
            this.checkBoxGammaEnable.Location = new System.Drawing.Point(13, 0);
            this.checkBoxGammaEnable.Name = "checkBoxGammaEnable";
            this.checkBoxGammaEnable.Size = new System.Drawing.Size(62, 17);
            this.checkBoxGammaEnable.TabIndex = 30;
            this.checkBoxGammaEnable.Text = "Gamma";
            this.checkBoxGammaEnable.UseVisualStyleBackColor = true;
            this.checkBoxGammaEnable.CheckedChanged += new System.EventHandler(this.checkBoxGammaEnable_CheckedChanged);
            // 
            // labelGammaMax
            // 
            this.labelGammaMax.AutoSize = true;
            this.labelGammaMax.Location = new System.Drawing.Point(338, 45);
            this.labelGammaMax.Name = "labelGammaMax";
            this.labelGammaMax.Size = new System.Drawing.Size(26, 13);
            this.labelGammaMax.TabIndex = 29;
            this.labelGammaMax.Text = "max";
            // 
            // labelGammaMin
            // 
            this.labelGammaMin.AutoSize = true;
            this.labelGammaMin.Location = new System.Drawing.Point(116, 45);
            this.labelGammaMin.Name = "labelGammaMin";
            this.labelGammaMin.Size = new System.Drawing.Size(23, 13);
            this.labelGammaMin.TabIndex = 28;
            this.labelGammaMin.Text = "min";
            // 
            // trackBarGamma
            // 
            this.trackBarGamma.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.trackBarGamma.Enabled = false;
            this.trackBarGamma.Location = new System.Drawing.Point(102, 24);
            this.trackBarGamma.Name = "trackBarGamma";
            this.trackBarGamma.Size = new System.Drawing.Size(284, 45);
            this.trackBarGamma.TabIndex = 27;
            this.trackBarGamma.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBarGamma.Scroll += new System.EventHandler(this.trackBarGamma_Scroll);
            // 
            // numericUpDownGamma
            // 
            this.numericUpDownGamma.DecimalPlaces = 1;
            this.numericUpDownGamma.Enabled = false;
            this.numericUpDownGamma.Location = new System.Drawing.Point(392, 24);
            this.numericUpDownGamma.Name = "numericUpDownGamma";
            this.numericUpDownGamma.Size = new System.Drawing.Size(67, 20);
            this.numericUpDownGamma.TabIndex = 26;
            this.numericUpDownGamma.ValueChanged += new System.EventHandler(this.numericUpDownGamma_ValueChanged);
            // 
            // labelGamma
            // 
            this.labelGamma.AutoSize = true;
            this.labelGamma.Location = new System.Drawing.Point(14, 24);
            this.labelGamma.Name = "labelGamma";
            this.labelGamma.Size = new System.Drawing.Size(43, 13);
            this.labelGamma.TabIndex = 25;
            this.labelGamma.Text = "Gamma";
            // 
            // PictureControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tabControl1);
            this.Name = "PictureControl";
            this.Size = new System.Drawing.Size(642, 405);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.groupBoxWhitebalance.ResumeLayout(false);
            this.groupBoxWhitebalance.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarGainBlue)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownGainBlue)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarGainGreen)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownGainGreen)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarGainRed)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownGainRed)).EndInit();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarGainMaster)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownGainMaster)).EndInit();
            this.tabPage3.ResumeLayout(false);
            this.groupBoxGamma.ResumeLayout(false);
            this.groupBoxGamma.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarGamma)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownGamma)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Label labelGainBlueMax;
        private System.Windows.Forms.Label labelGainBlueMin;
        private System.Windows.Forms.Label labelGainGreenMax;
        private System.Windows.Forms.Label labelGainGreenMin;
        private System.Windows.Forms.Label labelGainRedMax;
        private System.Windows.Forms.Label labelGainRedMin;
        private System.Windows.Forms.TrackBar trackBarGainBlue;
        private System.Windows.Forms.NumericUpDown numericUpDownGainBlue;
        private System.Windows.Forms.Label labelGainBlue;
        private System.Windows.Forms.TrackBar trackBarGainGreen;
        private System.Windows.Forms.NumericUpDown numericUpDownGainGreen;
        private System.Windows.Forms.Label labelGainGreen;
        private System.Windows.Forms.TrackBar trackBarGainRed;
        private System.Windows.Forms.NumericUpDown numericUpDownGainRed;
        private System.Windows.Forms.Label labelGainRed;
        private System.Windows.Forms.Label labelGainMasterMax;
        private System.Windows.Forms.Label labelGainMasterMin;
        private System.Windows.Forms.TrackBar trackBarGainMaster;
        private System.Windows.Forms.NumericUpDown numericUpDownGainMaster;
        private System.Windows.Forms.Label labelGainMaster;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.GroupBox groupBoxGamma;
        private System.Windows.Forms.CheckBox checkBoxHardwareGamma;
        private System.Windows.Forms.CheckBox checkBoxGammaEnable;
        private System.Windows.Forms.Label labelGammaMax;
        private System.Windows.Forms.Label labelGammaMin;
        private System.Windows.Forms.TrackBar trackBarGamma;
        private System.Windows.Forms.NumericUpDown numericUpDownGamma;
        private System.Windows.Forms.Label labelGamma;
        private System.Windows.Forms.CheckBox checkBoxMasterGainBoost;
        private System.Windows.Forms.CheckBox checkBoxMasterGainAuto;
        private System.Windows.Forms.GroupBox groupBoxWhitebalance;
        private System.Windows.Forms.RadioButton radioButtonWhitebalanceOnce;
        private System.Windows.Forms.RadioButton radioButtonWhitebalanceAuto;
        private System.Windows.Forms.RadioButton radioButtonWhitebalanceDisabled;
    }
}
