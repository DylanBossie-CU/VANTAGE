namespace uEye_DotNet_Direct_Renderer
{
    partial class uEye_DotNet_Direct_Renderer
    {
        private System.ComponentModel.IContainer components = null;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(uEye_DotNet_Direct_Renderer));
            this.DisplayWindow = new System.Windows.Forms.PictureBox();
            this.Button_Camera_Open_Freerun = new System.Windows.Forms.Button();
            this.Button_Camera_Open_Stop = new System.Windows.Forms.Button();
            this.Button_Exit = new System.Windows.Forms.Button();
            this.p1IDS_uEye = new System.Windows.Forms.PictureBox();
            this.rB_Direct3D = new System.Windows.Forms.RadioButton();
            this.rB_OpenGL = new System.Windows.Forms.RadioButton();
            this.rB_Vsync_off = new System.Windows.Forms.RadioButton();
            this.rB_Vsync_auto = new System.Windows.Forms.RadioButton();
            this.rB_Vsync_user = new System.Windows.Forms.RadioButton();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.tB_Image_Width = new System.Windows.Forms.TextBox();
            this.tB_Image_Height = new System.Windows.Forms.TextBox();
            this.tB_Overlay_Width = new System.Windows.Forms.TextBox();
            this.tB_Overlay_Height = new System.Windows.Forms.TextBox();
            this.cB_Mono = new System.Windows.Forms.CheckBox();
            this.cB_Show_overlay = new System.Windows.Forms.CheckBox();
            this.cB_Scale_image_and_overlay = new System.Windows.Forms.CheckBox();
            this.cB_Show_moving_overlay = new System.Windows.Forms.CheckBox();
            this.cB_Scale_image = new System.Windows.Forms.CheckBox();
            this.cB_Overlay_from_file = new System.Windows.Forms.CheckBox();
            this.cB_Semi_transparent = new System.Windows.Forms.CheckBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.Button_Key_Color = new System.Windows.Forms.Button();
            this.gBVsync = new System.Windows.Forms.GroupBox();
            this.nuSyncUpDown = new System.Windows.Forms.NumericUpDown();
            this.Button_Load_Param = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.DisplayWindow)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.p1IDS_uEye)).BeginInit();
            this.gBVsync.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nuSyncUpDown)).BeginInit();
            this.SuspendLayout();
            // 
            // DisplayWindow
            // 
            this.DisplayWindow.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.DisplayWindow.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.DisplayWindow.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.DisplayWindow.Location = new System.Drawing.Point(12, 12);
            this.DisplayWindow.Name = "DisplayWindow";
            this.DisplayWindow.Size = new System.Drawing.Size(739, 515);
            this.DisplayWindow.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.DisplayWindow.TabIndex = 0;
            this.DisplayWindow.TabStop = false;
            // 
            // Button_Camera_Open_Freerun
            // 
            this.Button_Camera_Open_Freerun.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.Button_Camera_Open_Freerun.Location = new System.Drawing.Point(12, 667);
            this.Button_Camera_Open_Freerun.Name = "Button_Camera_Open_Freerun";
            this.Button_Camera_Open_Freerun.Size = new System.Drawing.Size(124, 28);
            this.Button_Camera_Open_Freerun.TabIndex = 4;
            this.Button_Camera_Open_Freerun.Text = "Start Live";
            this.Button_Camera_Open_Freerun.UseVisualStyleBackColor = true;
            this.Button_Camera_Open_Freerun.Click += new System.EventHandler(this.Button_Camera_Open_Freerun_Click);
            // 
            // Button_Camera_Open_Stop
            // 
            this.Button_Camera_Open_Stop.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.Button_Camera_Open_Stop.Enabled = false;
            this.Button_Camera_Open_Stop.Location = new System.Drawing.Point(156, 667);
            this.Button_Camera_Open_Stop.Name = "Button_Camera_Open_Stop";
            this.Button_Camera_Open_Stop.Size = new System.Drawing.Size(124, 28);
            this.Button_Camera_Open_Stop.TabIndex = 5;
            this.Button_Camera_Open_Stop.Text = "Stop Live";
            this.Button_Camera_Open_Stop.UseVisualStyleBackColor = true;
            this.Button_Camera_Open_Stop.Click += new System.EventHandler(this.Button_Camera_Open_Stop_Click);
            // 
            // Button_Exit
            // 
            this.Button_Exit.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.Button_Exit.Location = new System.Drawing.Point(610, 667);
            this.Button_Exit.Name = "Button_Exit";
            this.Button_Exit.Size = new System.Drawing.Size(124, 28);
            this.Button_Exit.TabIndex = 7;
            this.Button_Exit.Text = "Exit";
            this.Button_Exit.UseVisualStyleBackColor = true;
            this.Button_Exit.Click += new System.EventHandler(this.Button_Exit_Click);
            // 
            // p1IDS_uEye
            // 
            this.p1IDS_uEye.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.p1IDS_uEye.Image = ((System.Drawing.Image)(resources.GetObject("p1IDS_uEye.Image")));
            this.p1IDS_uEye.Location = new System.Drawing.Point(12, 542);
            this.p1IDS_uEye.Name = "p1IDS_uEye";
            this.p1IDS_uEye.Size = new System.Drawing.Size(124, 59);
            this.p1IDS_uEye.TabIndex = 8;
            this.p1IDS_uEye.TabStop = false;
            // 
            // rB_Direct3D
            // 
            this.rB_Direct3D.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.rB_Direct3D.AutoSize = true;
            this.rB_Direct3D.Checked = true;
            this.rB_Direct3D.Location = new System.Drawing.Point(145, 542);
            this.rB_Direct3D.Name = "rB_Direct3D";
            this.rB_Direct3D.Size = new System.Drawing.Size(67, 17);
            this.rB_Direct3D.TabIndex = 9;
            this.rB_Direct3D.TabStop = true;
            this.rB_Direct3D.Text = "Direct3D";
            this.rB_Direct3D.UseVisualStyleBackColor = true;
            this.rB_Direct3D.CheckedChanged += new System.EventHandler(this.rB_Direct3D_CheckedChanged);
            // 
            // rB_OpenGL
            // 
            this.rB_OpenGL.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.rB_OpenGL.AutoSize = true;
            this.rB_OpenGL.Location = new System.Drawing.Point(268, 542);
            this.rB_OpenGL.Name = "rB_OpenGL";
            this.rB_OpenGL.Size = new System.Drawing.Size(65, 17);
            this.rB_OpenGL.TabIndex = 10;
            this.rB_OpenGL.Text = "OpenGL";
            this.rB_OpenGL.UseVisualStyleBackColor = true;
            this.rB_OpenGL.CheckedChanged += new System.EventHandler(this.rB_OpenGL_CheckedChanged);
            // 
            // rB_Vsync_off
            // 
            this.rB_Vsync_off.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.rB_Vsync_off.AutoSize = true;
            this.rB_Vsync_off.Location = new System.Drawing.Point(21, 17);
            this.rB_Vsync_off.Name = "rB_Vsync_off";
            this.rB_Vsync_off.Size = new System.Drawing.Size(69, 17);
            this.rB_Vsync_off.TabIndex = 11;
            this.rB_Vsync_off.Text = "Vsync off";
            this.rB_Vsync_off.UseVisualStyleBackColor = true;
            this.rB_Vsync_off.CheckedChanged += new System.EventHandler(this.rB_Vsync_off_CheckedChanged);
            // 
            // rB_Vsync_auto
            // 
            this.rB_Vsync_auto.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.rB_Vsync_auto.AutoSize = true;
            this.rB_Vsync_auto.Checked = true;
            this.rB_Vsync_auto.Location = new System.Drawing.Point(96, 17);
            this.rB_Vsync_auto.Name = "rB_Vsync_auto";
            this.rB_Vsync_auto.Size = new System.Drawing.Size(78, 17);
            this.rB_Vsync_auto.TabIndex = 12;
            this.rB_Vsync_auto.TabStop = true;
            this.rB_Vsync_auto.Text = "Vsync auto";
            this.rB_Vsync_auto.UseVisualStyleBackColor = true;
            this.rB_Vsync_auto.CheckedChanged += new System.EventHandler(this.rB_Vsync_auto_CheckedChanged);
            // 
            // rB_Vsync_user
            // 
            this.rB_Vsync_user.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.rB_Vsync_user.AutoSize = true;
            this.rB_Vsync_user.Location = new System.Drawing.Point(180, 17);
            this.rB_Vsync_user.Name = "rB_Vsync_user";
            this.rB_Vsync_user.Size = new System.Drawing.Size(77, 17);
            this.rB_Vsync_user.TabIndex = 13;
            this.rB_Vsync_user.Text = "Vsync user";
            this.rB_Vsync_user.UseVisualStyleBackColor = true;
            this.rB_Vsync_user.CheckedChanged += new System.EventHandler(this.rB_Vsync_user_CheckedChanged);
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(142, 592);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(67, 13);
            this.label1.TabIndex = 14;
            this.label1.Text = "Image width:";
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(142, 617);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(71, 13);
            this.label2.TabIndex = 15;
            this.label2.Text = "Image height:";
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(265, 592);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(74, 13);
            this.label3.TabIndex = 16;
            this.label3.Text = "Overlay width:";
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(265, 617);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(78, 13);
            this.label4.TabIndex = 17;
            this.label4.Text = "Overlay height:";
            // 
            // tB_Image_Width
            // 
            this.tB_Image_Width.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.tB_Image_Width.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tB_Image_Width.Location = new System.Drawing.Point(215, 593);
            this.tB_Image_Width.Multiline = true;
            this.tB_Image_Width.Name = "tB_Image_Width";
            this.tB_Image_Width.ReadOnly = true;
            this.tB_Image_Width.Size = new System.Drawing.Size(44, 16);
            this.tB_Image_Width.TabIndex = 18;
            // 
            // tB_Image_Height
            // 
            this.tB_Image_Height.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.tB_Image_Height.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tB_Image_Height.Location = new System.Drawing.Point(215, 617);
            this.tB_Image_Height.Multiline = true;
            this.tB_Image_Height.Name = "tB_Image_Height";
            this.tB_Image_Height.ReadOnly = true;
            this.tB_Image_Height.Size = new System.Drawing.Size(44, 16);
            this.tB_Image_Height.TabIndex = 19;
            // 
            // tB_Overlay_Width
            // 
            this.tB_Overlay_Width.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.tB_Overlay_Width.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tB_Overlay_Width.Location = new System.Drawing.Point(345, 591);
            this.tB_Overlay_Width.Multiline = true;
            this.tB_Overlay_Width.Name = "tB_Overlay_Width";
            this.tB_Overlay_Width.ReadOnly = true;
            this.tB_Overlay_Width.Size = new System.Drawing.Size(44, 16);
            this.tB_Overlay_Width.TabIndex = 20;
            // 
            // tB_Overlay_Height
            // 
            this.tB_Overlay_Height.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.tB_Overlay_Height.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tB_Overlay_Height.Location = new System.Drawing.Point(345, 617);
            this.tB_Overlay_Height.Multiline = true;
            this.tB_Overlay_Height.Name = "tB_Overlay_Height";
            this.tB_Overlay_Height.ReadOnly = true;
            this.tB_Overlay_Height.Size = new System.Drawing.Size(44, 16);
            this.tB_Overlay_Height.TabIndex = 21;
            // 
            // cB_Mono
            // 
            this.cB_Mono.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cB_Mono.AutoSize = true;
            this.cB_Mono.Location = new System.Drawing.Point(405, 543);
            this.cB_Mono.Name = "cB_Mono";
            this.cB_Mono.Size = new System.Drawing.Size(53, 17);
            this.cB_Mono.TabIndex = 22;
            this.cB_Mono.Text = "Mono";
            this.cB_Mono.UseVisualStyleBackColor = true;
            this.cB_Mono.CheckedChanged += new System.EventHandler(this.cB_Mono_CheckedChanged);
            // 
            // cB_Show_overlay
            // 
            this.cB_Show_overlay.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cB_Show_overlay.AutoSize = true;
            this.cB_Show_overlay.Location = new System.Drawing.Point(405, 569);
            this.cB_Show_overlay.Name = "cB_Show_overlay";
            this.cB_Show_overlay.Size = new System.Drawing.Size(90, 17);
            this.cB_Show_overlay.TabIndex = 23;
            this.cB_Show_overlay.Text = "Show overlay";
            this.cB_Show_overlay.UseVisualStyleBackColor = true;
            this.cB_Show_overlay.CheckedChanged += new System.EventHandler(this.cB_Show_overlay_CheckedChanged);
            // 
            // cB_Scale_image_and_overlay
            // 
            this.cB_Scale_image_and_overlay.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cB_Scale_image_and_overlay.AutoSize = true;
            this.cB_Scale_image_and_overlay.Location = new System.Drawing.Point(501, 543);
            this.cB_Scale_image_and_overlay.Name = "cB_Scale_image_and_overlay";
            this.cB_Scale_image_and_overlay.Size = new System.Drawing.Size(142, 17);
            this.cB_Scale_image_and_overlay.TabIndex = 24;
            this.cB_Scale_image_and_overlay.Text = "Scale image and overlay";
            this.cB_Scale_image_and_overlay.UseVisualStyleBackColor = true;
            this.cB_Scale_image_and_overlay.CheckedChanged += new System.EventHandler(this.cB_Scale_image_and_overlay_CheckedChanged);
            // 
            // cB_Show_moving_overlay
            // 
            this.cB_Show_moving_overlay.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cB_Show_moving_overlay.AutoSize = true;
            this.cB_Show_moving_overlay.Location = new System.Drawing.Point(501, 569);
            this.cB_Show_moving_overlay.Name = "cB_Show_moving_overlay";
            this.cB_Show_moving_overlay.Size = new System.Drawing.Size(127, 17);
            this.cB_Show_moving_overlay.TabIndex = 25;
            this.cB_Show_moving_overlay.Text = "Show moving overlay";
            this.cB_Show_moving_overlay.UseVisualStyleBackColor = true;
            this.cB_Show_moving_overlay.CheckedChanged += new System.EventHandler(this.cB_Show_moving_overlay_CheckedChanged);
            // 
            // cB_Scale_image
            // 
            this.cB_Scale_image.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cB_Scale_image.AutoSize = true;
            this.cB_Scale_image.Location = new System.Drawing.Point(650, 543);
            this.cB_Scale_image.Name = "cB_Scale_image";
            this.cB_Scale_image.Size = new System.Drawing.Size(84, 17);
            this.cB_Scale_image.TabIndex = 26;
            this.cB_Scale_image.Text = "Scale image";
            this.cB_Scale_image.UseVisualStyleBackColor = true;
            this.cB_Scale_image.CheckedChanged += new System.EventHandler(this.cB_Scale_image_CheckedChanged);
            // 
            // cB_Overlay_from_file
            // 
            this.cB_Overlay_from_file.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cB_Overlay_from_file.AutoSize = true;
            this.cB_Overlay_from_file.Location = new System.Drawing.Point(650, 569);
            this.cB_Overlay_from_file.Name = "cB_Overlay_from_file";
            this.cB_Overlay_from_file.Size = new System.Drawing.Size(101, 17);
            this.cB_Overlay_from_file.TabIndex = 27;
            this.cB_Overlay_from_file.Text = "Overlay from file";
            this.cB_Overlay_from_file.UseVisualStyleBackColor = true;
            this.cB_Overlay_from_file.CheckedChanged += new System.EventHandler(this.cB_Overlay_from_file_CheckedChanged);
            // 
            // cB_Semi_transparent
            // 
            this.cB_Semi_transparent.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cB_Semi_transparent.AutoSize = true;
            this.cB_Semi_transparent.Location = new System.Drawing.Point(650, 593);
            this.cB_Semi_transparent.Name = "cB_Semi_transparent";
            this.cB_Semi_transparent.Size = new System.Drawing.Size(105, 17);
            this.cB_Semi_transparent.TabIndex = 28;
            this.cB_Semi_transparent.Text = "Semi transparent";
            this.cB_Semi_transparent.UseVisualStyleBackColor = true;
            this.cB_Semi_transparent.CheckedChanged += new System.EventHandler(this.cB_Semi_transparent_CheckedChanged);
            // 
            // label5
            // 
            this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(402, 594);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(54, 13);
            this.label5.TabIndex = 30;
            this.label5.Text = "Sync pos:";
            // 
            // label6
            // 
            this.label6.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(591, 596);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(52, 13);
            this.label6.TabIndex = 31;
            this.label6.Text = "Key Color";
            // 
            // Button_Key_Color
            // 
            this.Button_Key_Color.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.Button_Key_Color.Location = new System.Drawing.Point(551, 592);
            this.Button_Key_Color.Name = "Button_Key_Color";
            this.Button_Key_Color.Size = new System.Drawing.Size(34, 20);
            this.Button_Key_Color.TabIndex = 32;
            this.Button_Key_Color.UseVisualStyleBackColor = true;
            this.Button_Key_Color.Click += new System.EventHandler(this.Button_Key_Color_Click_1);
            // 
            // gBVsync
            // 
            this.gBVsync.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.gBVsync.Controls.Add(this.rB_Vsync_user);
            this.gBVsync.Controls.Add(this.rB_Vsync_auto);
            this.gBVsync.Controls.Add(this.rB_Vsync_off);
            this.gBVsync.Location = new System.Drawing.Point(405, 617);
            this.gBVsync.Name = "gBVsync";
            this.gBVsync.Size = new System.Drawing.Size(272, 40);
            this.gBVsync.TabIndex = 33;
            this.gBVsync.TabStop = false;
            this.gBVsync.Text = "Vsync";
            // 
            // nuSyncUpDown
            // 
            this.nuSyncUpDown.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.nuSyncUpDown.Enabled = false;
            this.nuSyncUpDown.Location = new System.Drawing.Point(463, 592);
            this.nuSyncUpDown.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.nuSyncUpDown.Name = "nuSyncUpDown";
            this.nuSyncUpDown.Size = new System.Drawing.Size(53, 20);
            this.nuSyncUpDown.TabIndex = 34;
            this.nuSyncUpDown.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.nuSyncUpDown.Value = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.nuSyncUpDown.ValueChanged += new System.EventHandler(this.nuSyncUpDown_ValueChanged);
            // 
            // Button_Load_Param
            // 
            this.Button_Load_Param.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.Button_Load_Param.Location = new System.Drawing.Point(299, 667);
            this.Button_Load_Param.Name = "Button_Load_Param";
            this.Button_Load_Param.Size = new System.Drawing.Size(124, 28);
            this.Button_Load_Param.TabIndex = 35;
            this.Button_Load_Param.Text = "Load Parameter";
            this.Button_Load_Param.UseVisualStyleBackColor = true;
            this.Button_Load_Param.Click += new System.EventHandler(this.Button_Load_Param_Click);
            // 
            // uEye_DotNet_Direct_Renderer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(758, 706);
            this.Controls.Add(this.Button_Load_Param);
            this.Controls.Add(this.nuSyncUpDown);
            this.Controls.Add(this.gBVsync);
            this.Controls.Add(this.Button_Key_Color);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.cB_Semi_transparent);
            this.Controls.Add(this.cB_Overlay_from_file);
            this.Controls.Add(this.cB_Scale_image);
            this.Controls.Add(this.cB_Show_moving_overlay);
            this.Controls.Add(this.cB_Scale_image_and_overlay);
            this.Controls.Add(this.cB_Show_overlay);
            this.Controls.Add(this.cB_Mono);
            this.Controls.Add(this.tB_Overlay_Height);
            this.Controls.Add(this.tB_Overlay_Width);
            this.Controls.Add(this.tB_Image_Height);
            this.Controls.Add(this.tB_Image_Width);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.rB_OpenGL);
            this.Controls.Add(this.rB_Direct3D);
            this.Controls.Add(this.p1IDS_uEye);
            this.Controls.Add(this.Button_Exit);
            this.Controls.Add(this.Button_Camera_Open_Stop);
            this.Controls.Add(this.Button_Camera_Open_Freerun);
            this.Controls.Add(this.DisplayWindow);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(774, 744);
            this.Name = "uEye_DotNet_Direct_Renderer";
            this.Text = "uEye C# DirectRenderer";
            this.Shown += new System.EventHandler(this.uEye_DotNet_Direct_Renderer_Shown);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.uEye_DotNet_Simple_Live_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.DisplayWindow)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.p1IDS_uEye)).EndInit();
            this.gBVsync.ResumeLayout(false);
            this.gBVsync.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nuSyncUpDown)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox DisplayWindow;
        private System.Windows.Forms.Button Button_Camera_Open_Freerun;
        private System.Windows.Forms.Button Button_Camera_Open_Stop;
        private System.Windows.Forms.Button Button_Exit;
        private System.Windows.Forms.PictureBox p1IDS_uEye;
        private System.Windows.Forms.RadioButton rB_Direct3D;
        private System.Windows.Forms.RadioButton rB_OpenGL;
        private System.Windows.Forms.RadioButton rB_Vsync_off;
        private System.Windows.Forms.RadioButton rB_Vsync_auto;
        private System.Windows.Forms.RadioButton rB_Vsync_user;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox tB_Image_Width;
        private System.Windows.Forms.TextBox tB_Image_Height;
        private System.Windows.Forms.TextBox tB_Overlay_Width;
        private System.Windows.Forms.TextBox tB_Overlay_Height;
        private System.Windows.Forms.CheckBox cB_Mono;
        private System.Windows.Forms.CheckBox cB_Show_overlay;
        private System.Windows.Forms.CheckBox cB_Scale_image_and_overlay;
        private System.Windows.Forms.CheckBox cB_Show_moving_overlay;
        private System.Windows.Forms.CheckBox cB_Scale_image;
        private System.Windows.Forms.CheckBox cB_Overlay_from_file;
        private System.Windows.Forms.CheckBox cB_Semi_transparent;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button Button_Key_Color;
        private System.Windows.Forms.GroupBox gBVsync;
        private System.Windows.Forms.NumericUpDown nuSyncUpDown;
        private System.Windows.Forms.Button Button_Load_Param;
    }
}

