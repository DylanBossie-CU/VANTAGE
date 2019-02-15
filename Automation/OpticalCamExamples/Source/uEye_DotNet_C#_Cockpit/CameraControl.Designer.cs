namespace DotnetCockpit
{
    partial class CameraControl
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
            this.tabControlTiming = new System.Windows.Forms.TabControl();
            this.tabPage = new System.Windows.Forms.TabPage();
            this.checkBoxFramerateAuto = new System.Windows.Forms.CheckBox();
            this.checkBoxFramerateMax = new System.Windows.Forms.CheckBox();
            this.checkBoxExposureMax = new System.Windows.Forms.CheckBox();
            this.checkBoxFineIncrement = new System.Windows.Forms.CheckBox();
            this.checkBoxExposureAuto = new System.Windows.Forms.CheckBox();
            this.labelExposureMax = new System.Windows.Forms.Label();
            this.labelExposureMin = new System.Windows.Forms.Label();
            this.labelFramerateMax = new System.Windows.Forms.Label();
            this.labelFramerateMin = new System.Windows.Forms.Label();
            this.labelPixelclockMax = new System.Windows.Forms.Label();
            this.labelPixelclockMin = new System.Windows.Forms.Label();
            this.trackBarExposure = new System.Windows.Forms.TrackBar();
            this.numericUpDownExposure = new System.Windows.Forms.NumericUpDown();
            this.labelExposureTime = new System.Windows.Forms.Label();
            this.trackBarFramerate = new System.Windows.Forms.TrackBar();
            this.numericUpDownFramerate = new System.Windows.Forms.NumericUpDown();
            this.labelFramerateText = new System.Windows.Forms.Label();
            this.trackBarPixelclock = new System.Windows.Forms.TrackBar();
            this.numericUpDownPixelclock = new System.Windows.Forms.NumericUpDown();
            this.labelPixelclockText = new System.Windows.Forms.Label();
            this.tabPageTiming = new System.Windows.Forms.TabPage();
            this.labelSensorBandwidth = new System.Windows.Forms.Label();
            this.labelCameraAvg = new System.Windows.Forms.Label();
            this.labelCameraPeak = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.tabControlTiming.SuspendLayout();
            this.tabPage.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarExposure)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownExposure)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarFramerate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownFramerate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarPixelclock)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPixelclock)).BeginInit();
            this.tabPageTiming.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControlTiming
            // 
            this.tabControlTiming.Controls.Add(this.tabPage);
            this.tabControlTiming.Controls.Add(this.tabPageTiming);
            this.tabControlTiming.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlTiming.Location = new System.Drawing.Point(0, 0);
            this.tabControlTiming.Name = "tabControlTiming";
            this.tabControlTiming.SelectedIndex = 0;
            this.tabControlTiming.Size = new System.Drawing.Size(687, 428);
            this.tabControlTiming.TabIndex = 0;
            this.tabControlTiming.SelectedIndexChanged += new System.EventHandler(this.tabControlTiming_SelectedIndexChanged);
            // 
            // tabPage
            // 
            this.tabPage.Controls.Add(this.checkBoxFramerateAuto);
            this.tabPage.Controls.Add(this.checkBoxFramerateMax);
            this.tabPage.Controls.Add(this.checkBoxExposureMax);
            this.tabPage.Controls.Add(this.checkBoxFineIncrement);
            this.tabPage.Controls.Add(this.checkBoxExposureAuto);
            this.tabPage.Controls.Add(this.labelExposureMax);
            this.tabPage.Controls.Add(this.labelExposureMin);
            this.tabPage.Controls.Add(this.labelFramerateMax);
            this.tabPage.Controls.Add(this.labelFramerateMin);
            this.tabPage.Controls.Add(this.labelPixelclockMax);
            this.tabPage.Controls.Add(this.labelPixelclockMin);
            this.tabPage.Controls.Add(this.trackBarExposure);
            this.tabPage.Controls.Add(this.numericUpDownExposure);
            this.tabPage.Controls.Add(this.labelExposureTime);
            this.tabPage.Controls.Add(this.trackBarFramerate);
            this.tabPage.Controls.Add(this.numericUpDownFramerate);
            this.tabPage.Controls.Add(this.labelFramerateText);
            this.tabPage.Controls.Add(this.trackBarPixelclock);
            this.tabPage.Controls.Add(this.numericUpDownPixelclock);
            this.tabPage.Controls.Add(this.labelPixelclockText);
            this.tabPage.Location = new System.Drawing.Point(4, 22);
            this.tabPage.Name = "tabPage";
            this.tabPage.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage.Size = new System.Drawing.Size(679, 402);
            this.tabPage.TabIndex = 0;
            this.tabPage.Text = "Timing";
            this.tabPage.UseVisualStyleBackColor = true;
            // 
            // checkBoxFramerateAuto
            // 
            this.checkBoxFramerateAuto.AutoSize = true;
            this.checkBoxFramerateAuto.Location = new System.Drawing.Point(16, 140);
            this.checkBoxFramerateAuto.Name = "checkBoxFramerateAuto";
            this.checkBoxFramerateAuto.Size = new System.Drawing.Size(48, 17);
            this.checkBoxFramerateAuto.TabIndex = 49;
            this.checkBoxFramerateAuto.Text = "Auto";
            this.checkBoxFramerateAuto.UseVisualStyleBackColor = true;
            this.checkBoxFramerateAuto.CheckedChanged += new System.EventHandler(this.checkBoxFramerateAuto_CheckedChanged);
            // 
            // checkBoxFramerateMax
            // 
            this.checkBoxFramerateMax.AutoSize = true;
            this.checkBoxFramerateMax.Location = new System.Drawing.Point(16, 163);
            this.checkBoxFramerateMax.Name = "checkBoxFramerateMax";
            this.checkBoxFramerateMax.Size = new System.Drawing.Size(46, 17);
            this.checkBoxFramerateMax.TabIndex = 48;
            this.checkBoxFramerateMax.Text = "Max";
            this.checkBoxFramerateMax.UseVisualStyleBackColor = true;
            this.checkBoxFramerateMax.CheckedChanged += new System.EventHandler(this.checkBoxFramerateMax_CheckedChanged);
            // 
            // checkBoxExposureMax
            // 
            this.checkBoxExposureMax.AutoSize = true;
            this.checkBoxExposureMax.Location = new System.Drawing.Point(16, 282);
            this.checkBoxExposureMax.Name = "checkBoxExposureMax";
            this.checkBoxExposureMax.Size = new System.Drawing.Size(46, 17);
            this.checkBoxExposureMax.TabIndex = 47;
            this.checkBoxExposureMax.Text = "Max";
            this.checkBoxExposureMax.UseVisualStyleBackColor = true;
            this.checkBoxExposureMax.CheckedChanged += new System.EventHandler(this.checkBoxExposureMax_CheckedChanged);
            // 
            // checkBoxFineIncrement
            // 
            this.checkBoxFineIncrement.AutoSize = true;
            this.checkBoxFineIncrement.Location = new System.Drawing.Point(16, 305);
            this.checkBoxFineIncrement.Name = "checkBoxFineIncrement";
            this.checkBoxFineIncrement.Size = new System.Drawing.Size(95, 17);
            this.checkBoxFineIncrement.TabIndex = 46;
            this.checkBoxFineIncrement.Text = "Fine increment";
            this.checkBoxFineIncrement.UseVisualStyleBackColor = true;
            this.checkBoxFineIncrement.CheckedChanged += new System.EventHandler(this.checkBoxFineIncrement_CheckedChanged);
            // 
            // checkBoxExposureAuto
            // 
            this.checkBoxExposureAuto.AutoSize = true;
            this.checkBoxExposureAuto.Location = new System.Drawing.Point(16, 259);
            this.checkBoxExposureAuto.Name = "checkBoxExposureAuto";
            this.checkBoxExposureAuto.Size = new System.Drawing.Size(48, 17);
            this.checkBoxExposureAuto.TabIndex = 45;
            this.checkBoxExposureAuto.Text = "Auto";
            this.checkBoxExposureAuto.UseVisualStyleBackColor = true;
            this.checkBoxExposureAuto.CheckedChanged += new System.EventHandler(this.checkBoxExposureAuto_CheckedChanged);
            // 
            // labelExposureMax
            // 
            this.labelExposureMax.AutoSize = true;
            this.labelExposureMax.Location = new System.Drawing.Point(370, 233);
            this.labelExposureMax.Name = "labelExposureMax";
            this.labelExposureMax.Size = new System.Drawing.Size(26, 13);
            this.labelExposureMax.TabIndex = 42;
            this.labelExposureMax.Text = "max";
            // 
            // labelExposureMin
            // 
            this.labelExposureMin.AutoSize = true;
            this.labelExposureMin.Location = new System.Drawing.Point(117, 233);
            this.labelExposureMin.Name = "labelExposureMin";
            this.labelExposureMin.Size = new System.Drawing.Size(23, 13);
            this.labelExposureMin.TabIndex = 41;
            this.labelExposureMin.Text = "min";
            // 
            // labelFramerateMax
            // 
            this.labelFramerateMax.AutoSize = true;
            this.labelFramerateMax.Location = new System.Drawing.Point(370, 114);
            this.labelFramerateMax.Name = "labelFramerateMax";
            this.labelFramerateMax.Size = new System.Drawing.Size(26, 13);
            this.labelFramerateMax.TabIndex = 37;
            this.labelFramerateMax.Text = "max";
            // 
            // labelFramerateMin
            // 
            this.labelFramerateMin.AutoSize = true;
            this.labelFramerateMin.Location = new System.Drawing.Point(117, 114);
            this.labelFramerateMin.Name = "labelFramerateMin";
            this.labelFramerateMin.Size = new System.Drawing.Size(23, 13);
            this.labelFramerateMin.TabIndex = 36;
            this.labelFramerateMin.Text = "min";
            // 
            // labelPixelclockMax
            // 
            this.labelPixelclockMax.AutoSize = true;
            this.labelPixelclockMax.Location = new System.Drawing.Point(370, 50);
            this.labelPixelclockMax.Name = "labelPixelclockMax";
            this.labelPixelclockMax.Size = new System.Drawing.Size(26, 13);
            this.labelPixelclockMax.TabIndex = 32;
            this.labelPixelclockMax.Text = "max";
            // 
            // labelPixelclockMin
            // 
            this.labelPixelclockMin.AutoSize = true;
            this.labelPixelclockMin.Location = new System.Drawing.Point(117, 50);
            this.labelPixelclockMin.Name = "labelPixelclockMin";
            this.labelPixelclockMin.Size = new System.Drawing.Size(23, 13);
            this.labelPixelclockMin.TabIndex = 31;
            this.labelPixelclockMin.Text = "min";
            // 
            // trackBarExposure
            // 
            this.trackBarExposure.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.trackBarExposure.Location = new System.Drawing.Point(102, 201);
            this.trackBarExposure.Name = "trackBarExposure";
            this.trackBarExposure.Size = new System.Drawing.Size(294, 45);
            this.trackBarExposure.TabIndex = 44;
            this.trackBarExposure.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBarExposure.Scroll += new System.EventHandler(this.trackBarExposure_Scroll);
            // 
            // numericUpDownExposure
            // 
            this.numericUpDownExposure.DecimalPlaces = 2;
            this.numericUpDownExposure.Location = new System.Drawing.Point(402, 201);
            this.numericUpDownExposure.Name = "numericUpDownExposure";
            this.numericUpDownExposure.Size = new System.Drawing.Size(67, 20);
            this.numericUpDownExposure.TabIndex = 43;
            this.numericUpDownExposure.ValueChanged += new System.EventHandler(this.numericUpDownExposure_ValueChanged);
            // 
            // labelExposureTime
            // 
            this.labelExposureTime.AutoSize = true;
            this.labelExposureTime.Location = new System.Drawing.Point(13, 201);
            this.labelExposureTime.Name = "labelExposureTime";
            this.labelExposureTime.Size = new System.Drawing.Size(51, 13);
            this.labelExposureTime.TabIndex = 40;
            this.labelExposureTime.Text = "Exposure";
            // 
            // trackBarFramerate
            // 
            this.trackBarFramerate.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.trackBarFramerate.Location = new System.Drawing.Point(102, 82);
            this.trackBarFramerate.Name = "trackBarFramerate";
            this.trackBarFramerate.Size = new System.Drawing.Size(294, 45);
            this.trackBarFramerate.TabIndex = 39;
            this.trackBarFramerate.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBarFramerate.Scroll += new System.EventHandler(this.trackBarFramerate_Scroll);
            // 
            // numericUpDownFramerate
            // 
            this.numericUpDownFramerate.DecimalPlaces = 2;
            this.numericUpDownFramerate.Location = new System.Drawing.Point(402, 82);
            this.numericUpDownFramerate.Name = "numericUpDownFramerate";
            this.numericUpDownFramerate.Size = new System.Drawing.Size(67, 20);
            this.numericUpDownFramerate.TabIndex = 38;
            this.numericUpDownFramerate.ValueChanged += new System.EventHandler(this.numericUpDownFramerate_ValueChanged);
            // 
            // labelFramerateText
            // 
            this.labelFramerateText.AutoSize = true;
            this.labelFramerateText.Location = new System.Drawing.Point(13, 82);
            this.labelFramerateText.Name = "labelFramerateText";
            this.labelFramerateText.Size = new System.Drawing.Size(54, 13);
            this.labelFramerateText.TabIndex = 35;
            this.labelFramerateText.Text = "Framerate";
            // 
            // trackBarPixelclock
            // 
            this.trackBarPixelclock.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.trackBarPixelclock.Location = new System.Drawing.Point(102, 18);
            this.trackBarPixelclock.Name = "trackBarPixelclock";
            this.trackBarPixelclock.Size = new System.Drawing.Size(294, 45);
            this.trackBarPixelclock.TabIndex = 34;
            this.trackBarPixelclock.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBarPixelclock.Scroll += new System.EventHandler(this.trackBarPixelclock_Scroll);
            // 
            // numericUpDownPixelclock
            // 
            this.numericUpDownPixelclock.Location = new System.Drawing.Point(402, 18);
            this.numericUpDownPixelclock.Name = "numericUpDownPixelclock";
            this.numericUpDownPixelclock.Size = new System.Drawing.Size(67, 20);
            this.numericUpDownPixelclock.TabIndex = 33;
            this.numericUpDownPixelclock.ValueChanged += new System.EventHandler(this.numericUpDownPixelclock_ValueChanged);
            // 
            // labelPixelclockText
            // 
            this.labelPixelclockText.AutoSize = true;
            this.labelPixelclockText.Location = new System.Drawing.Point(13, 18);
            this.labelPixelclockText.Name = "labelPixelclockText";
            this.labelPixelclockText.Size = new System.Drawing.Size(58, 13);
            this.labelPixelclockText.TabIndex = 30;
            this.labelPixelclockText.Text = "Pixel clock";
            // 
            // tabPageTiming
            // 
            this.tabPageTiming.Controls.Add(this.labelSensorBandwidth);
            this.tabPageTiming.Controls.Add(this.labelCameraAvg);
            this.tabPageTiming.Controls.Add(this.labelCameraPeak);
            this.tabPageTiming.Controls.Add(this.label3);
            this.tabPageTiming.Controls.Add(this.label2);
            this.tabPageTiming.Controls.Add(this.label1);
            this.tabPageTiming.Location = new System.Drawing.Point(4, 22);
            this.tabPageTiming.Name = "tabPageTiming";
            this.tabPageTiming.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageTiming.Size = new System.Drawing.Size(679, 402);
            this.tabPageTiming.TabIndex = 1;
            this.tabPageTiming.Text = "Bandwidth";
            this.tabPageTiming.UseVisualStyleBackColor = true;
            // 
            // labelSensorBandwidth
            // 
            this.labelSensorBandwidth.AutoSize = true;
            this.labelSensorBandwidth.Location = new System.Drawing.Point(166, 54);
            this.labelSensorBandwidth.Name = "labelSensorBandwidth";
            this.labelSensorBandwidth.Size = new System.Drawing.Size(35, 13);
            this.labelSensorBandwidth.TabIndex = 5;
            this.labelSensorBandwidth.Text = "label6";
            // 
            // labelCameraAvg
            // 
            this.labelCameraAvg.AutoSize = true;
            this.labelCameraAvg.Location = new System.Drawing.Point(166, 31);
            this.labelCameraAvg.Name = "labelCameraAvg";
            this.labelCameraAvg.Size = new System.Drawing.Size(35, 13);
            this.labelCameraAvg.TabIndex = 4;
            this.labelCameraAvg.Text = "label5";
            // 
            // labelCameraPeak
            // 
            this.labelCameraPeak.AutoSize = true;
            this.labelCameraPeak.Location = new System.Drawing.Point(166, 7);
            this.labelCameraPeak.Name = "labelCameraPeak";
            this.labelCameraPeak.Size = new System.Drawing.Size(35, 13);
            this.labelCameraPeak.TabIndex = 3;
            this.labelCameraPeak.Text = "label4";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(7, 54);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(95, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "Sensor bandwidth:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 31);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(140, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Camera average bandwidth:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 7);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(125, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Camera peak bandwidth:";
            // 
            // CameraControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tabControlTiming);
            this.Name = "CameraControl";
            this.Size = new System.Drawing.Size(687, 428);
            this.tabControlTiming.ResumeLayout(false);
            this.tabPage.ResumeLayout(false);
            this.tabPage.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarExposure)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownExposure)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarFramerate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownFramerate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarPixelclock)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPixelclock)).EndInit();
            this.tabPageTiming.ResumeLayout(false);
            this.tabPageTiming.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControlTiming;
        private System.Windows.Forms.TabPage tabPage;
        private System.Windows.Forms.Label labelExposureMax;
        private System.Windows.Forms.Label labelExposureMin;
        private System.Windows.Forms.Label labelFramerateMax;
        private System.Windows.Forms.Label labelFramerateMin;
        private System.Windows.Forms.Label labelPixelclockMax;
        private System.Windows.Forms.Label labelPixelclockMin;
        private System.Windows.Forms.TrackBar trackBarExposure;
        private System.Windows.Forms.NumericUpDown numericUpDownExposure;
        private System.Windows.Forms.Label labelExposureTime;
        private System.Windows.Forms.TrackBar trackBarFramerate;
        private System.Windows.Forms.NumericUpDown numericUpDownFramerate;
        private System.Windows.Forms.Label labelFramerateText;
        private System.Windows.Forms.TrackBar trackBarPixelclock;
        private System.Windows.Forms.NumericUpDown numericUpDownPixelclock;
        private System.Windows.Forms.Label labelPixelclockText;
        private System.Windows.Forms.TabPage tabPageTiming;
        private System.Windows.Forms.CheckBox checkBoxFramerateAuto;
        private System.Windows.Forms.CheckBox checkBoxFramerateMax;
        private System.Windows.Forms.CheckBox checkBoxExposureMax;
        private System.Windows.Forms.CheckBox checkBoxFineIncrement;
        private System.Windows.Forms.CheckBox checkBoxExposureAuto;
        private System.Windows.Forms.Label labelSensorBandwidth;
        private System.Windows.Forms.Label labelCameraAvg;
        private System.Windows.Forms.Label labelCameraPeak;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;

    }
}
