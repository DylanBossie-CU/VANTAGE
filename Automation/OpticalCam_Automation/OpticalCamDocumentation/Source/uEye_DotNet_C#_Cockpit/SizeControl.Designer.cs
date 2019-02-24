namespace DotnetCockpit
{
    partial class SizeControl
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
            this.tabControlSize = new System.Windows.Forms.TabControl();
            this.tabPageAOI = new System.Windows.Forms.TabPage();
            this.labelAoiWidthMax = new System.Windows.Forms.Label();
            this.labelAoiWidthMin = new System.Windows.Forms.Label();
            this.labelAoiHeightMax = new System.Windows.Forms.Label();
            this.labelAoiHeightMin = new System.Windows.Forms.Label();
            this.labelAoiWidth = new System.Windows.Forms.Label();
            this.labelAoiTopMax = new System.Windows.Forms.Label();
            this.numericUpDownAoiWidth = new System.Windows.Forms.NumericUpDown();
            this.labelAoiTopMin = new System.Windows.Forms.Label();
            this.trackBarAoiWidth = new System.Windows.Forms.TrackBar();
            this.trackBarAoiTop = new System.Windows.Forms.TrackBar();
            this.numericUpDownAoiTop = new System.Windows.Forms.NumericUpDown();
            this.labelAoiTop = new System.Windows.Forms.Label();
            this.labelAoiHeight = new System.Windows.Forms.Label();
            this.labelAoiLeftMax = new System.Windows.Forms.Label();
            this.numericUpDownAoiHeight = new System.Windows.Forms.NumericUpDown();
            this.labelAoiLeftMin = new System.Windows.Forms.Label();
            this.trackBarAoiHeight = new System.Windows.Forms.TrackBar();
            this.trackBarAoiLeft = new System.Windows.Forms.TrackBar();
            this.numericUpDownAoiLeft = new System.Windows.Forms.NumericUpDown();
            this.labelAoiLeft = new System.Windows.Forms.Label();
            this.tabPageBinningSubsampling = new System.Windows.Forms.TabPage();
            this.checkBoxFormatMirrorUpDown = new System.Windows.Forms.CheckBox();
            this.checkBoxFormatMirrorLeftRight = new System.Windows.Forms.CheckBox();
            this.labelFormatMirror = new System.Windows.Forms.Label();
            this.comboBoxFormatSubsamplingVertical = new System.Windows.Forms.ComboBox();
            this.comboBoxFormatBinningVertical = new System.Windows.Forms.ComboBox();
            this.comboBoxFormatSubsamplingHorizontal = new System.Windows.Forms.ComboBox();
            this.comboBoxFormatBinningHorizontal = new System.Windows.Forms.ComboBox();
            this.labelFormatVertical2 = new System.Windows.Forms.Label();
            this.labelFormatVertical1 = new System.Windows.Forms.Label();
            this.labelFormatHorizontal2 = new System.Windows.Forms.Label();
            this.labelFormatHorizontal1 = new System.Windows.Forms.Label();
            this.labelFormatSubsampling = new System.Windows.Forms.Label();
            this.labelFormatBinning = new System.Windows.Forms.Label();
            this.tabControlSize.SuspendLayout();
            this.tabPageAOI.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownAoiWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAoiWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAoiTop)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownAoiTop)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownAoiHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAoiHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAoiLeft)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownAoiLeft)).BeginInit();
            this.tabPageBinningSubsampling.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControlSize
            // 
            this.tabControlSize.Controls.Add(this.tabPageAOI);
            this.tabControlSize.Controls.Add(this.tabPageBinningSubsampling);
            this.tabControlSize.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlSize.Location = new System.Drawing.Point(0, 0);
            this.tabControlSize.Name = "tabControlSize";
            this.tabControlSize.SelectedIndex = 0;
            this.tabControlSize.Size = new System.Drawing.Size(612, 364);
            this.tabControlSize.TabIndex = 0;
            this.tabControlSize.SelectedIndexChanged += new System.EventHandler(this.tabControlSize_SelectedIndexChanged);
            // 
            // tabPageAOI
            // 
            this.tabPageAOI.Controls.Add(this.labelAoiWidthMax);
            this.tabPageAOI.Controls.Add(this.labelAoiWidthMin);
            this.tabPageAOI.Controls.Add(this.labelAoiHeightMax);
            this.tabPageAOI.Controls.Add(this.labelAoiHeightMin);
            this.tabPageAOI.Controls.Add(this.labelAoiWidth);
            this.tabPageAOI.Controls.Add(this.labelAoiTopMax);
            this.tabPageAOI.Controls.Add(this.numericUpDownAoiWidth);
            this.tabPageAOI.Controls.Add(this.labelAoiTopMin);
            this.tabPageAOI.Controls.Add(this.trackBarAoiWidth);
            this.tabPageAOI.Controls.Add(this.trackBarAoiTop);
            this.tabPageAOI.Controls.Add(this.numericUpDownAoiTop);
            this.tabPageAOI.Controls.Add(this.labelAoiTop);
            this.tabPageAOI.Controls.Add(this.labelAoiHeight);
            this.tabPageAOI.Controls.Add(this.labelAoiLeftMax);
            this.tabPageAOI.Controls.Add(this.numericUpDownAoiHeight);
            this.tabPageAOI.Controls.Add(this.labelAoiLeftMin);
            this.tabPageAOI.Controls.Add(this.trackBarAoiHeight);
            this.tabPageAOI.Controls.Add(this.trackBarAoiLeft);
            this.tabPageAOI.Controls.Add(this.numericUpDownAoiLeft);
            this.tabPageAOI.Controls.Add(this.labelAoiLeft);
            this.tabPageAOI.Location = new System.Drawing.Point(4, 22);
            this.tabPageAOI.Name = "tabPageAOI";
            this.tabPageAOI.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageAOI.Size = new System.Drawing.Size(604, 338);
            this.tabPageAOI.TabIndex = 0;
            this.tabPageAOI.Text = "AOI";
            this.tabPageAOI.UseVisualStyleBackColor = true;
            // 
            // labelAoiWidthMax
            // 
            this.labelAoiWidthMax.AutoSize = true;
            this.labelAoiWidthMax.Location = new System.Drawing.Point(353, 32);
            this.labelAoiWidthMax.Name = "labelAoiWidthMax";
            this.labelAoiWidthMax.Size = new System.Drawing.Size(26, 13);
            this.labelAoiWidthMax.TabIndex = 53;
            this.labelAoiWidthMax.Text = "max";
            // 
            // labelAoiWidthMin
            // 
            this.labelAoiWidthMin.AutoSize = true;
            this.labelAoiWidthMin.Location = new System.Drawing.Point(111, 32);
            this.labelAoiWidthMin.Name = "labelAoiWidthMin";
            this.labelAoiWidthMin.Size = new System.Drawing.Size(23, 13);
            this.labelAoiWidthMin.TabIndex = 52;
            this.labelAoiWidthMin.Text = "min";
            // 
            // labelAoiHeightMax
            // 
            this.labelAoiHeightMax.AutoSize = true;
            this.labelAoiHeightMax.Location = new System.Drawing.Point(353, 83);
            this.labelAoiHeightMax.Name = "labelAoiHeightMax";
            this.labelAoiHeightMax.Size = new System.Drawing.Size(26, 13);
            this.labelAoiHeightMax.TabIndex = 51;
            this.labelAoiHeightMax.Text = "max";
            // 
            // labelAoiHeightMin
            // 
            this.labelAoiHeightMin.AutoSize = true;
            this.labelAoiHeightMin.Location = new System.Drawing.Point(108, 83);
            this.labelAoiHeightMin.Name = "labelAoiHeightMin";
            this.labelAoiHeightMin.Size = new System.Drawing.Size(23, 13);
            this.labelAoiHeightMin.TabIndex = 50;
            this.labelAoiHeightMin.Text = "min";
            // 
            // labelAoiWidth
            // 
            this.labelAoiWidth.AutoSize = true;
            this.labelAoiWidth.Location = new System.Drawing.Point(6, 15);
            this.labelAoiWidth.Name = "labelAoiWidth";
            this.labelAoiWidth.Size = new System.Drawing.Size(35, 13);
            this.labelAoiWidth.TabIndex = 34;
            this.labelAoiWidth.Text = "Width";
            // 
            // labelAoiTopMax
            // 
            this.labelAoiTopMax.AutoSize = true;
            this.labelAoiTopMax.Location = new System.Drawing.Point(353, 199);
            this.labelAoiTopMax.Name = "labelAoiTopMax";
            this.labelAoiTopMax.Size = new System.Drawing.Size(26, 13);
            this.labelAoiTopMax.TabIndex = 49;
            this.labelAoiTopMax.Text = "max";
            // 
            // numericUpDownAoiWidth
            // 
            this.numericUpDownAoiWidth.Location = new System.Drawing.Point(395, 15);
            this.numericUpDownAoiWidth.Name = "numericUpDownAoiWidth";
            this.numericUpDownAoiWidth.Size = new System.Drawing.Size(67, 20);
            this.numericUpDownAoiWidth.TabIndex = 35;
            this.numericUpDownAoiWidth.ValueChanged += new System.EventHandler(this.numericUpDownAoiWidth_ValueChanged);
            // 
            // labelAoiTopMin
            // 
            this.labelAoiTopMin.AutoSize = true;
            this.labelAoiTopMin.Location = new System.Drawing.Point(108, 199);
            this.labelAoiTopMin.Name = "labelAoiTopMin";
            this.labelAoiTopMin.Size = new System.Drawing.Size(23, 13);
            this.labelAoiTopMin.TabIndex = 48;
            this.labelAoiTopMin.Text = "min";
            // 
            // trackBarAoiWidth
            // 
            this.trackBarAoiWidth.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.trackBarAoiWidth.Location = new System.Drawing.Point(95, 11);
            this.trackBarAoiWidth.Name = "trackBarAoiWidth";
            this.trackBarAoiWidth.Size = new System.Drawing.Size(294, 45);
            this.trackBarAoiWidth.TabIndex = 36;
            this.trackBarAoiWidth.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBarAoiWidth.Scroll += new System.EventHandler(this.trackBarAoiWidth_Scroll);
            // 
            // trackBarAoiTop
            // 
            this.trackBarAoiTop.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.trackBarAoiTop.Location = new System.Drawing.Point(95, 168);
            this.trackBarAoiTop.Name = "trackBarAoiTop";
            this.trackBarAoiTop.Size = new System.Drawing.Size(294, 45);
            this.trackBarAoiTop.TabIndex = 47;
            this.trackBarAoiTop.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBarAoiTop.Scroll += new System.EventHandler(this.trackBarAoiTop_Scroll);
            // 
            // numericUpDownAoiTop
            // 
            this.numericUpDownAoiTop.Location = new System.Drawing.Point(395, 168);
            this.numericUpDownAoiTop.Name = "numericUpDownAoiTop";
            this.numericUpDownAoiTop.Size = new System.Drawing.Size(67, 20);
            this.numericUpDownAoiTop.TabIndex = 46;
            this.numericUpDownAoiTop.ValueChanged += new System.EventHandler(this.numericUpDownAoiTop_ValueChanged);
            // 
            // labelAoiTop
            // 
            this.labelAoiTop.AutoSize = true;
            this.labelAoiTop.Location = new System.Drawing.Point(6, 168);
            this.labelAoiTop.Name = "labelAoiTop";
            this.labelAoiTop.Size = new System.Drawing.Size(26, 13);
            this.labelAoiTop.TabIndex = 45;
            this.labelAoiTop.Text = "Top";
            // 
            // labelAoiHeight
            // 
            this.labelAoiHeight.AutoSize = true;
            this.labelAoiHeight.Location = new System.Drawing.Point(6, 66);
            this.labelAoiHeight.Name = "labelAoiHeight";
            this.labelAoiHeight.Size = new System.Drawing.Size(38, 13);
            this.labelAoiHeight.TabIndex = 37;
            this.labelAoiHeight.Text = "Height";
            // 
            // labelAoiLeftMax
            // 
            this.labelAoiLeftMax.AutoSize = true;
            this.labelAoiLeftMax.Location = new System.Drawing.Point(353, 148);
            this.labelAoiLeftMax.Name = "labelAoiLeftMax";
            this.labelAoiLeftMax.Size = new System.Drawing.Size(26, 13);
            this.labelAoiLeftMax.TabIndex = 44;
            this.labelAoiLeftMax.Text = "max";
            // 
            // numericUpDownAoiHeight
            // 
            this.numericUpDownAoiHeight.Location = new System.Drawing.Point(395, 66);
            this.numericUpDownAoiHeight.Name = "numericUpDownAoiHeight";
            this.numericUpDownAoiHeight.Size = new System.Drawing.Size(67, 20);
            this.numericUpDownAoiHeight.TabIndex = 38;
            this.numericUpDownAoiHeight.ValueChanged += new System.EventHandler(this.numericUpDownAoiHeight_ValueChanged);
            // 
            // labelAoiLeftMin
            // 
            this.labelAoiLeftMin.AutoSize = true;
            this.labelAoiLeftMin.Location = new System.Drawing.Point(108, 148);
            this.labelAoiLeftMin.Name = "labelAoiLeftMin";
            this.labelAoiLeftMin.Size = new System.Drawing.Size(23, 13);
            this.labelAoiLeftMin.TabIndex = 43;
            this.labelAoiLeftMin.Text = "min";
            // 
            // trackBarAoiHeight
            // 
            this.trackBarAoiHeight.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.trackBarAoiHeight.Location = new System.Drawing.Point(99, 62);
            this.trackBarAoiHeight.Name = "trackBarAoiHeight";
            this.trackBarAoiHeight.Size = new System.Drawing.Size(294, 45);
            this.trackBarAoiHeight.TabIndex = 39;
            this.trackBarAoiHeight.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBarAoiHeight.Scroll += new System.EventHandler(this.trackBarAoiHeight_Scroll);
            // 
            // trackBarAoiLeft
            // 
            this.trackBarAoiLeft.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.trackBarAoiLeft.Location = new System.Drawing.Point(95, 117);
            this.trackBarAoiLeft.Name = "trackBarAoiLeft";
            this.trackBarAoiLeft.Size = new System.Drawing.Size(294, 45);
            this.trackBarAoiLeft.TabIndex = 42;
            this.trackBarAoiLeft.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBarAoiLeft.Scroll += new System.EventHandler(this.trackBarAoiLeft_Scroll);
            // 
            // numericUpDownAoiLeft
            // 
            this.numericUpDownAoiLeft.Location = new System.Drawing.Point(395, 117);
            this.numericUpDownAoiLeft.Name = "numericUpDownAoiLeft";
            this.numericUpDownAoiLeft.Size = new System.Drawing.Size(67, 20);
            this.numericUpDownAoiLeft.TabIndex = 41;
            this.numericUpDownAoiLeft.ValueChanged += new System.EventHandler(this.numericUpDownAoiLeft_ValueChanged);
            // 
            // labelAoiLeft
            // 
            this.labelAoiLeft.AutoSize = true;
            this.labelAoiLeft.Location = new System.Drawing.Point(6, 117);
            this.labelAoiLeft.Name = "labelAoiLeft";
            this.labelAoiLeft.Size = new System.Drawing.Size(25, 13);
            this.labelAoiLeft.TabIndex = 40;
            this.labelAoiLeft.Text = "Left";
            // 
            // tabPageBinningSubsampling
            // 
            this.tabPageBinningSubsampling.Controls.Add(this.checkBoxFormatMirrorUpDown);
            this.tabPageBinningSubsampling.Controls.Add(this.checkBoxFormatMirrorLeftRight);
            this.tabPageBinningSubsampling.Controls.Add(this.labelFormatMirror);
            this.tabPageBinningSubsampling.Controls.Add(this.comboBoxFormatSubsamplingVertical);
            this.tabPageBinningSubsampling.Controls.Add(this.comboBoxFormatBinningVertical);
            this.tabPageBinningSubsampling.Controls.Add(this.comboBoxFormatSubsamplingHorizontal);
            this.tabPageBinningSubsampling.Controls.Add(this.comboBoxFormatBinningHorizontal);
            this.tabPageBinningSubsampling.Controls.Add(this.labelFormatVertical2);
            this.tabPageBinningSubsampling.Controls.Add(this.labelFormatVertical1);
            this.tabPageBinningSubsampling.Controls.Add(this.labelFormatHorizontal2);
            this.tabPageBinningSubsampling.Controls.Add(this.labelFormatHorizontal1);
            this.tabPageBinningSubsampling.Controls.Add(this.labelFormatSubsampling);
            this.tabPageBinningSubsampling.Controls.Add(this.labelFormatBinning);
            this.tabPageBinningSubsampling.Location = new System.Drawing.Point(4, 22);
            this.tabPageBinningSubsampling.Name = "tabPageBinningSubsampling";
            this.tabPageBinningSubsampling.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageBinningSubsampling.Size = new System.Drawing.Size(604, 338);
            this.tabPageBinningSubsampling.TabIndex = 1;
            this.tabPageBinningSubsampling.Text = "Binning/Subsampling";
            this.tabPageBinningSubsampling.UseVisualStyleBackColor = true;
            // 
            // checkBoxFormatMirrorUpDown
            // 
            this.checkBoxFormatMirrorUpDown.AutoSize = true;
            this.checkBoxFormatMirrorUpDown.Location = new System.Drawing.Point(206, 90);
            this.checkBoxFormatMirrorUpDown.Name = "checkBoxFormatMirrorUpDown";
            this.checkBoxFormatMirrorUpDown.Size = new System.Drawing.Size(73, 17);
            this.checkBoxFormatMirrorUpDown.TabIndex = 25;
            this.checkBoxFormatMirrorUpDown.Text = "Up/Down";
            this.checkBoxFormatMirrorUpDown.UseVisualStyleBackColor = true;
            this.checkBoxFormatMirrorUpDown.CheckedChanged += new System.EventHandler(this.checkBoxFormatMirrorUpDown_CheckedChanged);
            // 
            // checkBoxFormatMirrorLeftRight
            // 
            this.checkBoxFormatMirrorLeftRight.AutoSize = true;
            this.checkBoxFormatMirrorLeftRight.Location = new System.Drawing.Point(102, 90);
            this.checkBoxFormatMirrorLeftRight.Name = "checkBoxFormatMirrorLeftRight";
            this.checkBoxFormatMirrorLeftRight.Size = new System.Drawing.Size(74, 17);
            this.checkBoxFormatMirrorLeftRight.TabIndex = 24;
            this.checkBoxFormatMirrorLeftRight.Text = "Left/Right";
            this.checkBoxFormatMirrorLeftRight.UseVisualStyleBackColor = true;
            this.checkBoxFormatMirrorLeftRight.CheckedChanged += new System.EventHandler(this.checkBoxFormatMirrorLeftRight_CheckedChanged);
            // 
            // labelFormatMirror
            // 
            this.labelFormatMirror.AutoSize = true;
            this.labelFormatMirror.Location = new System.Drawing.Point(17, 90);
            this.labelFormatMirror.Name = "labelFormatMirror";
            this.labelFormatMirror.Size = new System.Drawing.Size(33, 13);
            this.labelFormatMirror.TabIndex = 23;
            this.labelFormatMirror.Text = "Mirror";
            // 
            // comboBoxFormatSubsamplingVertical
            // 
            this.comboBoxFormatSubsamplingVertical.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxFormatSubsamplingVertical.FormattingEnabled = true;
            this.comboBoxFormatSubsamplingVertical.Location = new System.Drawing.Point(389, 46);
            this.comboBoxFormatSubsamplingVertical.Name = "comboBoxFormatSubsamplingVertical";
            this.comboBoxFormatSubsamplingVertical.Size = new System.Drawing.Size(80, 21);
            this.comboBoxFormatSubsamplingVertical.TabIndex = 22;
            this.comboBoxFormatSubsamplingVertical.SelectedIndexChanged += new System.EventHandler(this.comboBoxFormatSubsamplingVertical_SelectedIndexChanged);
            // 
            // comboBoxFormatBinningVertical
            // 
            this.comboBoxFormatBinningVertical.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxFormatBinningVertical.FormattingEnabled = true;
            this.comboBoxFormatBinningVertical.Location = new System.Drawing.Point(389, 18);
            this.comboBoxFormatBinningVertical.Name = "comboBoxFormatBinningVertical";
            this.comboBoxFormatBinningVertical.Size = new System.Drawing.Size(80, 21);
            this.comboBoxFormatBinningVertical.TabIndex = 21;
            this.comboBoxFormatBinningVertical.SelectedIndexChanged += new System.EventHandler(this.comboBoxFormatBinningVertical_SelectedIndexChanged);
            // 
            // comboBoxFormatSubsamplingHorizontal
            // 
            this.comboBoxFormatSubsamplingHorizontal.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxFormatSubsamplingHorizontal.FormattingEnabled = true;
            this.comboBoxFormatSubsamplingHorizontal.Location = new System.Drawing.Point(163, 46);
            this.comboBoxFormatSubsamplingHorizontal.Name = "comboBoxFormatSubsamplingHorizontal";
            this.comboBoxFormatSubsamplingHorizontal.Size = new System.Drawing.Size(80, 21);
            this.comboBoxFormatSubsamplingHorizontal.TabIndex = 20;
            this.comboBoxFormatSubsamplingHorizontal.SelectedIndexChanged += new System.EventHandler(this.comboBoxFormatSubsamplingHorizontal_SelectedIndexChanged);
            // 
            // comboBoxFormatBinningHorizontal
            // 
            this.comboBoxFormatBinningHorizontal.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxFormatBinningHorizontal.FormattingEnabled = true;
            this.comboBoxFormatBinningHorizontal.Location = new System.Drawing.Point(163, 18);
            this.comboBoxFormatBinningHorizontal.Name = "comboBoxFormatBinningHorizontal";
            this.comboBoxFormatBinningHorizontal.Size = new System.Drawing.Size(80, 21);
            this.comboBoxFormatBinningHorizontal.TabIndex = 19;
            this.comboBoxFormatBinningHorizontal.SelectedIndexChanged += new System.EventHandler(this.comboBoxFormatBinningHorizontal_SelectedIndexChanged);
            // 
            // labelFormatVertical2
            // 
            this.labelFormatVertical2.AutoSize = true;
            this.labelFormatVertical2.Location = new System.Drawing.Point(340, 46);
            this.labelFormatVertical2.Name = "labelFormatVertical2";
            this.labelFormatVertical2.Size = new System.Drawing.Size(42, 13);
            this.labelFormatVertical2.TabIndex = 18;
            this.labelFormatVertical2.Text = "Vertical";
            // 
            // labelFormatVertical1
            // 
            this.labelFormatVertical1.AutoSize = true;
            this.labelFormatVertical1.Location = new System.Drawing.Point(340, 21);
            this.labelFormatVertical1.Name = "labelFormatVertical1";
            this.labelFormatVertical1.Size = new System.Drawing.Size(42, 13);
            this.labelFormatVertical1.TabIndex = 17;
            this.labelFormatVertical1.Text = "Vertical";
            // 
            // labelFormatHorizontal2
            // 
            this.labelFormatHorizontal2.AutoSize = true;
            this.labelFormatHorizontal2.Location = new System.Drawing.Point(103, 46);
            this.labelFormatHorizontal2.Name = "labelFormatHorizontal2";
            this.labelFormatHorizontal2.Size = new System.Drawing.Size(54, 13);
            this.labelFormatHorizontal2.TabIndex = 16;
            this.labelFormatHorizontal2.Text = "Horizontal";
            // 
            // labelFormatHorizontal1
            // 
            this.labelFormatHorizontal1.AutoSize = true;
            this.labelFormatHorizontal1.Location = new System.Drawing.Point(103, 21);
            this.labelFormatHorizontal1.Name = "labelFormatHorizontal1";
            this.labelFormatHorizontal1.Size = new System.Drawing.Size(54, 13);
            this.labelFormatHorizontal1.TabIndex = 15;
            this.labelFormatHorizontal1.Text = "Horizontal";
            // 
            // labelFormatSubsampling
            // 
            this.labelFormatSubsampling.AutoSize = true;
            this.labelFormatSubsampling.Location = new System.Drawing.Point(14, 46);
            this.labelFormatSubsampling.Name = "labelFormatSubsampling";
            this.labelFormatSubsampling.Size = new System.Drawing.Size(67, 13);
            this.labelFormatSubsampling.TabIndex = 14;
            this.labelFormatSubsampling.Text = "Subsampling";
            // 
            // labelFormatBinning
            // 
            this.labelFormatBinning.AutoSize = true;
            this.labelFormatBinning.Location = new System.Drawing.Point(14, 21);
            this.labelFormatBinning.Name = "labelFormatBinning";
            this.labelFormatBinning.Size = new System.Drawing.Size(42, 13);
            this.labelFormatBinning.TabIndex = 13;
            this.labelFormatBinning.Text = "Binning";
            // 
            // SizeControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tabControlSize);
            this.Name = "SizeControl";
            this.Size = new System.Drawing.Size(612, 364);
            this.tabControlSize.ResumeLayout(false);
            this.tabPageAOI.ResumeLayout(false);
            this.tabPageAOI.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownAoiWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAoiWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAoiTop)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownAoiTop)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownAoiHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAoiHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAoiLeft)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownAoiLeft)).EndInit();
            this.tabPageBinningSubsampling.ResumeLayout(false);
            this.tabPageBinningSubsampling.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControlSize;
        private System.Windows.Forms.TabPage tabPageAOI;
        private System.Windows.Forms.TabPage tabPageBinningSubsampling;
        private System.Windows.Forms.Label labelAoiWidthMax;
        private System.Windows.Forms.Label labelAoiWidthMin;
        private System.Windows.Forms.Label labelAoiHeightMax;
        private System.Windows.Forms.Label labelAoiHeightMin;
        private System.Windows.Forms.Label labelAoiWidth;
        private System.Windows.Forms.Label labelAoiTopMax;
        private System.Windows.Forms.NumericUpDown numericUpDownAoiWidth;
        private System.Windows.Forms.Label labelAoiTopMin;
        private System.Windows.Forms.TrackBar trackBarAoiWidth;
        private System.Windows.Forms.TrackBar trackBarAoiTop;
        private System.Windows.Forms.NumericUpDown numericUpDownAoiTop;
        private System.Windows.Forms.Label labelAoiTop;
        private System.Windows.Forms.Label labelAoiHeight;
        private System.Windows.Forms.Label labelAoiLeftMax;
        private System.Windows.Forms.NumericUpDown numericUpDownAoiHeight;
        private System.Windows.Forms.Label labelAoiLeftMin;
        private System.Windows.Forms.TrackBar trackBarAoiHeight;
        private System.Windows.Forms.TrackBar trackBarAoiLeft;
        private System.Windows.Forms.NumericUpDown numericUpDownAoiLeft;
        private System.Windows.Forms.Label labelAoiLeft;
        private System.Windows.Forms.CheckBox checkBoxFormatMirrorUpDown;
        private System.Windows.Forms.CheckBox checkBoxFormatMirrorLeftRight;
        private System.Windows.Forms.Label labelFormatMirror;
        private System.Windows.Forms.ComboBox comboBoxFormatSubsamplingVertical;
        private System.Windows.Forms.ComboBox comboBoxFormatBinningVertical;
        private System.Windows.Forms.ComboBox comboBoxFormatSubsamplingHorizontal;
        private System.Windows.Forms.ComboBox comboBoxFormatBinningHorizontal;
        private System.Windows.Forms.Label labelFormatVertical2;
        private System.Windows.Forms.Label labelFormatVertical1;
        private System.Windows.Forms.Label labelFormatHorizontal2;
        private System.Windows.Forms.Label labelFormatHorizontal1;
        private System.Windows.Forms.Label labelFormatSubsampling;
        private System.Windows.Forms.Label labelFormatBinning;
    }
}
