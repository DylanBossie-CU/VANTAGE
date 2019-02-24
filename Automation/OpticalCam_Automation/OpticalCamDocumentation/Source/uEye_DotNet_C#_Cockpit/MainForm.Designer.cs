namespace DotnetCockpit
{
    partial class MainForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.panelDisplay = new System.Windows.Forms.Panel();
            this.toolStripMain = new System.Windows.Forms.ToolStrip();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
            this.loadParameterToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemLoadParameterFromFile = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemLoadParameterFromEEPROM = new System.Windows.Forms.ToolStripMenuItem();
            this.saveParameterToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemSaveParameterToFile = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemSaveParameterToEEPROM = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.cameraToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator8 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator10 = new System.Windows.Forms.ToolStripSeparator();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator9 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripMenuItemMirrorRightLeft = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemMirrorUpDown = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator11 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripMenuItemCrosshair = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemTimestamp = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabelFPS = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabelFailed = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabelFrameCount = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabelCamera = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripSeparator12 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonOpenFreerun = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonOpenStop = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonExit = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonFreerun = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonSnapshot = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonFitToWnd = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton1To1 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButton1To2 = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonSettings = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonAES = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonAWB = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonSaveImage = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonLoadImage = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonVideoRec = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonAbout = new System.Windows.Forms.ToolStripButton();
            this.toolStripMenuItemLoadImage = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemSaveImage = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemExit = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemOpenCameraLive = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemOpenCamera = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemCloseCamera = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemFreerun = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemSnapshot = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemApiErrorReport = new System.Windows.Forms.ToolStripMenuItem();
            this.displayToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemFitToWindow = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1To1 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1To2 = new System.Windows.Forms.ToolStripMenuItem();
            this.pictureBoxDisplay = new System.Windows.Forms.PictureBox();
            this.panelDisplay.SuspendLayout();
            this.toolStripMain.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.statusStrip.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxDisplay)).BeginInit();
            this.SuspendLayout();
            // 
            // panelDisplay
            // 
            this.panelDisplay.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.panelDisplay.AutoScroll = true;
            this.panelDisplay.Controls.Add(this.pictureBoxDisplay);
            this.panelDisplay.Location = new System.Drawing.Point(0, 52);
            this.panelDisplay.Name = "panelDisplay";
            this.panelDisplay.Size = new System.Drawing.Size(784, 485);
            this.panelDisplay.TabIndex = 0;
            // 
            // toolStripMain
            // 
            this.toolStripMain.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.toolStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButtonOpenFreerun,
            this.toolStripButtonOpenStop,
            this.toolStripButtonExit,
            this.toolStripSeparator1,
            this.toolStripButtonFreerun,
            this.toolStripButtonSnapshot,
            this.toolStripSeparator2,
            this.toolStripButtonFitToWnd,
            this.toolStripButton1To1,
            this.toolStripButton1To2,
            this.toolStripSeparator3,
            this.toolStripButtonSettings,
            this.toolStripButtonAES,
            this.toolStripButtonAWB,
            this.toolStripSeparator4,
            this.toolStripButtonSaveImage,
            this.toolStripButtonLoadImage,
            this.toolStripSeparator5,
            this.toolStripButtonVideoRec,
            this.toolStripSeparator12,
            this.toolStripButtonAbout});
            this.toolStripMain.Location = new System.Drawing.Point(0, 24);
            this.toolStripMain.Name = "toolStripMain";
            this.toolStripMain.RenderMode = System.Windows.Forms.ToolStripRenderMode.Professional;
            this.toolStripMain.Size = new System.Drawing.Size(784, 39);
            this.toolStripMain.TabIndex = 1;
            this.toolStripMain.Text = "toolStripMain";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 39);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 39);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 39);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 39);
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(6, 39);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.cameraToolStripMenuItem,
            this.viewToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.Professional;
            this.menuStrip1.Size = new System.Drawing.Size(784, 24);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItemLoadImage,
            this.toolStripMenuItemSaveImage,
            this.toolStripSeparator6,
            this.loadParameterToolStripMenuItem,
            this.saveParameterToolStripMenuItem,
            this.toolStripSeparator7,
            this.toolStripMenuItemExit});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // toolStripSeparator6
            // 
            this.toolStripSeparator6.Name = "toolStripSeparator6";
            this.toolStripSeparator6.Size = new System.Drawing.Size(154, 6);
            // 
            // loadParameterToolStripMenuItem
            // 
            this.loadParameterToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItemLoadParameterFromFile,
            this.toolStripMenuItemLoadParameterFromEEPROM});
            this.loadParameterToolStripMenuItem.Name = "loadParameterToolStripMenuItem";
            this.loadParameterToolStripMenuItem.Size = new System.Drawing.Size(157, 22);
            this.loadParameterToolStripMenuItem.Text = "Load parameter";
            // 
            // toolStripMenuItemLoadParameterFromFile
            // 
            this.toolStripMenuItemLoadParameterFromFile.Enabled = false;
            this.toolStripMenuItemLoadParameterFromFile.Name = "toolStripMenuItemLoadParameterFromFile";
            this.toolStripMenuItemLoadParameterFromFile.Size = new System.Drawing.Size(149, 22);
            this.toolStripMenuItemLoadParameterFromFile.Text = "from file...";
            this.toolStripMenuItemLoadParameterFromFile.Click += new System.EventHandler(this.toolStripMenuItemLoadParameterFromFile_Click);
            // 
            // toolStripMenuItemLoadParameterFromEEPROM
            // 
            this.toolStripMenuItemLoadParameterFromEEPROM.Enabled = false;
            this.toolStripMenuItemLoadParameterFromEEPROM.Name = "toolStripMenuItemLoadParameterFromEEPROM";
            this.toolStripMenuItemLoadParameterFromEEPROM.Size = new System.Drawing.Size(149, 22);
            this.toolStripMenuItemLoadParameterFromEEPROM.Text = "from EEPROM";
            this.toolStripMenuItemLoadParameterFromEEPROM.Click += new System.EventHandler(this.toolStripMenuItemLoadParameterFromEEPROM_Click);
            // 
            // saveParameterToolStripMenuItem
            // 
            this.saveParameterToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItemSaveParameterToFile,
            this.toolStripMenuItemSaveParameterToEEPROM});
            this.saveParameterToolStripMenuItem.Name = "saveParameterToolStripMenuItem";
            this.saveParameterToolStripMenuItem.Size = new System.Drawing.Size(157, 22);
            this.saveParameterToolStripMenuItem.Text = "Save parameter";
            // 
            // toolStripMenuItemSaveParameterToFile
            // 
            this.toolStripMenuItemSaveParameterToFile.Enabled = false;
            this.toolStripMenuItemSaveParameterToFile.Name = "toolStripMenuItemSaveParameterToFile";
            this.toolStripMenuItemSaveParameterToFile.Size = new System.Drawing.Size(134, 22);
            this.toolStripMenuItemSaveParameterToFile.Text = "to file...";
            this.toolStripMenuItemSaveParameterToFile.Click += new System.EventHandler(this.toolStripMenuItemSaveParameterToFile_Click);
            // 
            // toolStripMenuItemSaveParameterToEEPROM
            // 
            this.toolStripMenuItemSaveParameterToEEPROM.Enabled = false;
            this.toolStripMenuItemSaveParameterToEEPROM.Name = "toolStripMenuItemSaveParameterToEEPROM";
            this.toolStripMenuItemSaveParameterToEEPROM.Size = new System.Drawing.Size(134, 22);
            this.toolStripMenuItemSaveParameterToEEPROM.Text = "to EEPROM";
            this.toolStripMenuItemSaveParameterToEEPROM.Click += new System.EventHandler(this.toolStripMenuItemSaveParameterToEEPROM_Click);
            // 
            // toolStripSeparator7
            // 
            this.toolStripSeparator7.Name = "toolStripSeparator7";
            this.toolStripSeparator7.Size = new System.Drawing.Size(154, 6);
            // 
            // cameraToolStripMenuItem
            // 
            this.cameraToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItemOpenCameraLive,
            this.toolStripMenuItemOpenCamera,
            this.toolStripMenuItemCloseCamera,
            this.toolStripSeparator8,
            this.toolStripMenuItemFreerun,
            this.toolStripMenuItemSnapshot,
            this.toolStripSeparator10,
            this.toolStripMenuItemApiErrorReport});
            this.cameraToolStripMenuItem.Name = "cameraToolStripMenuItem";
            this.cameraToolStripMenuItem.Size = new System.Drawing.Size(60, 20);
            this.cameraToolStripMenuItem.Text = "Camera";
            // 
            // toolStripSeparator8
            // 
            this.toolStripSeparator8.Name = "toolStripSeparator8";
            this.toolStripSeparator8.Size = new System.Drawing.Size(191, 6);
            // 
            // toolStripSeparator10
            // 
            this.toolStripSeparator10.Name = "toolStripSeparator10";
            this.toolStripSeparator10.Size = new System.Drawing.Size(191, 6);
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.displayToolStripMenuItem,
            this.toolStripSeparator9,
            this.toolStripMenuItemMirrorRightLeft,
            this.toolStripMenuItemMirrorUpDown,
            this.toolStripSeparator11,
            this.toolStripMenuItemCrosshair,
            this.toolStripMenuItemTimestamp});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.viewToolStripMenuItem.Text = "View";
            this.viewToolStripMenuItem.DropDownOpening += new System.EventHandler(this.viewToolStripMenuItem_DropDownOpening);
            // 
            // toolStripSeparator9
            // 
            this.toolStripSeparator9.Name = "toolStripSeparator9";
            this.toolStripSeparator9.Size = new System.Drawing.Size(160, 6);
            // 
            // toolStripMenuItemMirrorRightLeft
            // 
            this.toolStripMenuItemMirrorRightLeft.CheckOnClick = true;
            this.toolStripMenuItemMirrorRightLeft.Enabled = false;
            this.toolStripMenuItemMirrorRightLeft.Name = "toolStripMenuItemMirrorRightLeft";
            this.toolStripMenuItemMirrorRightLeft.Size = new System.Drawing.Size(163, 22);
            this.toolStripMenuItemMirrorRightLeft.Text = "Mirror Left/Right";
            this.toolStripMenuItemMirrorRightLeft.Click += new System.EventHandler(this.toolStripMenuItemMirrorRightLeft_Click);
            // 
            // toolStripMenuItemMirrorUpDown
            // 
            this.toolStripMenuItemMirrorUpDown.CheckOnClick = true;
            this.toolStripMenuItemMirrorUpDown.Enabled = false;
            this.toolStripMenuItemMirrorUpDown.Name = "toolStripMenuItemMirrorUpDown";
            this.toolStripMenuItemMirrorUpDown.Size = new System.Drawing.Size(163, 22);
            this.toolStripMenuItemMirrorUpDown.Text = "Mirror Up/Down";
            this.toolStripMenuItemMirrorUpDown.Click += new System.EventHandler(this.toolStripMenuItemMirrorUpDown_Click);
            // 
            // toolStripSeparator11
            // 
            this.toolStripSeparator11.Name = "toolStripSeparator11";
            this.toolStripSeparator11.Size = new System.Drawing.Size(160, 6);
            // 
            // toolStripMenuItemCrosshair
            // 
            this.toolStripMenuItemCrosshair.CheckOnClick = true;
            this.toolStripMenuItemCrosshair.Enabled = false;
            this.toolStripMenuItemCrosshair.Name = "toolStripMenuItemCrosshair";
            this.toolStripMenuItemCrosshair.Size = new System.Drawing.Size(163, 22);
            this.toolStripMenuItemCrosshair.Text = "Crosshair";
            this.toolStripMenuItemCrosshair.Click += new System.EventHandler(this.toolStripMenuItemCrosshair_Click);
            // 
            // toolStripMenuItemTimestamp
            // 
            this.toolStripMenuItemTimestamp.CheckOnClick = true;
            this.toolStripMenuItemTimestamp.Enabled = false;
            this.toolStripMenuItemTimestamp.Name = "toolStripMenuItemTimestamp";
            this.toolStripMenuItemTimestamp.Size = new System.Drawing.Size(163, 22);
            this.toolStripMenuItemTimestamp.Text = "Timestamp";
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabelFPS,
            this.toolStripStatusLabelFailed,
            this.toolStripStatusLabelFrameCount,
            this.toolStripStatusLabelCamera});
            this.statusStrip.Location = new System.Drawing.Point(0, 540);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.statusStrip.Size = new System.Drawing.Size(784, 22);
            this.statusStrip.TabIndex = 2;
            // 
            // toolStripStatusLabelFPS
            // 
            this.toolStripStatusLabelFPS.Name = "toolStripStatusLabelFPS";
            this.toolStripStatusLabelFPS.Size = new System.Drawing.Size(29, 17);
            this.toolStripStatusLabelFPS.Text = ":FPS";
            this.toolStripStatusLabelFPS.Visible = false;
            // 
            // toolStripStatusLabelFailed
            // 
            this.toolStripStatusLabelFailed.Name = "toolStripStatusLabelFailed";
            this.toolStripStatusLabelFailed.Size = new System.Drawing.Size(41, 17);
            this.toolStripStatusLabelFailed.Text = ":Failed";
            this.toolStripStatusLabelFailed.Visible = false;
            // 
            // toolStripStatusLabelFrameCount
            // 
            this.toolStripStatusLabelFrameCount.Name = "toolStripStatusLabelFrameCount";
            this.toolStripStatusLabelFrameCount.Size = new System.Drawing.Size(48, 17);
            this.toolStripStatusLabelFrameCount.Text = ":Frames";
            this.toolStripStatusLabelFrameCount.Visible = false;
            // 
            // toolStripStatusLabelCamera
            // 
            this.toolStripStatusLabelCamera.Name = "toolStripStatusLabelCamera";
            this.toolStripStatusLabelCamera.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.toolStripStatusLabelCamera.Size = new System.Drawing.Size(51, 17);
            this.toolStripStatusLabelCamera.Text = "Camera:";
            this.toolStripStatusLabelCamera.Visible = false;
            // 
            // toolStripSeparator12
            // 
            this.toolStripSeparator12.Name = "toolStripSeparator12";
            this.toolStripSeparator12.Size = new System.Drawing.Size(6, 39);
            // 
            // toolStripButtonOpenFreerun
            // 
            this.toolStripButtonOpenFreerun.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonOpenFreerun.Image = global::uEye_DotNet_Cockpit.Properties.Resources.Camera_Open_Freerun;
            this.toolStripButtonOpenFreerun.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonOpenFreerun.Name = "toolStripButtonOpenFreerun";
            this.toolStripButtonOpenFreerun.Size = new System.Drawing.Size(36, 36);
            this.toolStripButtonOpenFreerun.Text = "Open Freerun";
            this.toolStripButtonOpenFreerun.Click += new System.EventHandler(this.toolStripButtonOpenFreerun_Click);
            // 
            // toolStripButtonOpenStop
            // 
            this.toolStripButtonOpenStop.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonOpenStop.Image = global::uEye_DotNet_Cockpit.Properties.Resources.Camera_Open_Stop;
            this.toolStripButtonOpenStop.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonOpenStop.Name = "toolStripButtonOpenStop";
            this.toolStripButtonOpenStop.Size = new System.Drawing.Size(36, 36);
            this.toolStripButtonOpenStop.Text = "Open and stop";
            this.toolStripButtonOpenStop.Click += new System.EventHandler(this.toolStripButtonOpenStop_Click);
            // 
            // toolStripButtonExit
            // 
            this.toolStripButtonExit.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonExit.Enabled = false;
            this.toolStripButtonExit.Image = global::uEye_DotNet_Cockpit.Properties.Resources.Camera_Exit;
            this.toolStripButtonExit.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonExit.Name = "toolStripButtonExit";
            this.toolStripButtonExit.Size = new System.Drawing.Size(36, 36);
            this.toolStripButtonExit.Text = "Exit";
            this.toolStripButtonExit.Click += new System.EventHandler(this.toolStripButtonExit_Click);
            // 
            // toolStripButtonFreerun
            // 
            this.toolStripButtonFreerun.CheckOnClick = true;
            this.toolStripButtonFreerun.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonFreerun.Enabled = false;
            this.toolStripButtonFreerun.Image = global::uEye_DotNet_Cockpit.Properties.Resources.Capture_Freerun;
            this.toolStripButtonFreerun.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonFreerun.Name = "toolStripButtonFreerun";
            this.toolStripButtonFreerun.Size = new System.Drawing.Size(36, 36);
            this.toolStripButtonFreerun.Text = "Freerun";
            this.toolStripButtonFreerun.Click += new System.EventHandler(this.toolStripButtonFreerun_Click);
            // 
            // toolStripButtonSnapshot
            // 
            this.toolStripButtonSnapshot.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonSnapshot.Enabled = false;
            this.toolStripButtonSnapshot.Image = global::uEye_DotNet_Cockpit.Properties.Resources.Capture_Snap;
            this.toolStripButtonSnapshot.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonSnapshot.Name = "toolStripButtonSnapshot";
            this.toolStripButtonSnapshot.Size = new System.Drawing.Size(36, 36);
            this.toolStripButtonSnapshot.Text = "Snapshot";
            this.toolStripButtonSnapshot.Click += new System.EventHandler(this.toolStripButtonSnapshot_Click);
            // 
            // toolStripButtonFitToWnd
            // 
            this.toolStripButtonFitToWnd.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonFitToWnd.Enabled = false;
            this.toolStripButtonFitToWnd.Image = global::uEye_DotNet_Cockpit.Properties.Resources.Display_FitWindow_1;
            this.toolStripButtonFitToWnd.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonFitToWnd.Name = "toolStripButtonFitToWnd";
            this.toolStripButtonFitToWnd.Size = new System.Drawing.Size(36, 36);
            this.toolStripButtonFitToWnd.Text = "FitToWnd";
            this.toolStripButtonFitToWnd.ToolTipText = "Fit to Window";
            this.toolStripButtonFitToWnd.Click += new System.EventHandler(this.toolStripButtonFitToWnd_Click);
            // 
            // toolStripButton1To1
            // 
            this.toolStripButton1To1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton1To1.Enabled = false;
            this.toolStripButton1To1.Image = global::uEye_DotNet_Cockpit.Properties.Resources.Display_1_1;
            this.toolStripButton1To1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1To1.Name = "toolStripButton1To1";
            this.toolStripButton1To1.Size = new System.Drawing.Size(36, 36);
            this.toolStripButton1To1.Text = "toolStripButton1To1";
            this.toolStripButton1To1.ToolTipText = "Display 1 to 1";
            this.toolStripButton1To1.Click += new System.EventHandler(this.toolStripButton1To1_Click);
            // 
            // toolStripButton1To2
            // 
            this.toolStripButton1To2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton1To2.Enabled = false;
            this.toolStripButton1To2.Image = global::uEye_DotNet_Cockpit.Properties.Resources.Display_1_2;
            this.toolStripButton1To2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton1To2.Name = "toolStripButton1To2";
            this.toolStripButton1To2.Size = new System.Drawing.Size(36, 36);
            this.toolStripButton1To2.Text = "toolStripButton1To2";
            this.toolStripButton1To2.ToolTipText = "Display 1 to 2";
            this.toolStripButton1To2.Click += new System.EventHandler(this.toolStripButton1To2_Click);
            // 
            // toolStripButtonSettings
            // 
            this.toolStripButtonSettings.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonSettings.Enabled = false;
            this.toolStripButtonSettings.Image = global::uEye_DotNet_Cockpit.Properties.Resources.Camera_Settings;
            this.toolStripButtonSettings.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonSettings.Name = "toolStripButtonSettings";
            this.toolStripButtonSettings.Size = new System.Drawing.Size(36, 36);
            this.toolStripButtonSettings.Text = "toolStripButtonSettings";
            this.toolStripButtonSettings.ToolTipText = "Settings";
            this.toolStripButtonSettings.Click += new System.EventHandler(this.toolStripButtonSettings_Click);
            // 
            // toolStripButtonAES
            // 
            this.toolStripButtonAES.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonAES.Enabled = false;
            this.toolStripButtonAES.Image = global::uEye_DotNet_Cockpit.Properties.Resources.AES_Set;
            this.toolStripButtonAES.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonAES.Name = "toolStripButtonAES";
            this.toolStripButtonAES.Size = new System.Drawing.Size(36, 36);
            this.toolStripButtonAES.Text = "toolStripButtonAES";
            this.toolStripButtonAES.ToolTipText = "Auto shutter";
            this.toolStripButtonAES.Click += new System.EventHandler(this.toolStripButtonAES_Click);
            // 
            // toolStripButtonAWB
            // 
            this.toolStripButtonAWB.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonAWB.Enabled = false;
            this.toolStripButtonAWB.Image = global::uEye_DotNet_Cockpit.Properties.Resources.AWB_Set;
            this.toolStripButtonAWB.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonAWB.Name = "toolStripButtonAWB";
            this.toolStripButtonAWB.Size = new System.Drawing.Size(36, 36);
            this.toolStripButtonAWB.Text = "toolStripButtonAWB";
            this.toolStripButtonAWB.ToolTipText = "Auto whitebalance";
            this.toolStripButtonAWB.Click += new System.EventHandler(this.toolStripButtonAWB_Click);
            // 
            // toolStripButtonSaveImage
            // 
            this.toolStripButtonSaveImage.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonSaveImage.Enabled = false;
            this.toolStripButtonSaveImage.Image = global::uEye_DotNet_Cockpit.Properties.Resources.saveImage;
            this.toolStripButtonSaveImage.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonSaveImage.Name = "toolStripButtonSaveImage";
            this.toolStripButtonSaveImage.Size = new System.Drawing.Size(36, 36);
            this.toolStripButtonSaveImage.Text = "Save image";
            this.toolStripButtonSaveImage.Click += new System.EventHandler(this.toolStripButtonSaveImage_Click);
            // 
            // toolStripButtonLoadImage
            // 
            this.toolStripButtonLoadImage.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonLoadImage.Enabled = false;
            this.toolStripButtonLoadImage.Image = global::uEye_DotNet_Cockpit.Properties.Resources.loadImage;
            this.toolStripButtonLoadImage.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonLoadImage.Name = "toolStripButtonLoadImage";
            this.toolStripButtonLoadImage.Size = new System.Drawing.Size(36, 36);
            this.toolStripButtonLoadImage.Text = "Load image";
            this.toolStripButtonLoadImage.Click += new System.EventHandler(this.toolStripButtonLoadImage_Click);
            // 
            // toolStripButtonVideoRec
            // 
            this.toolStripButtonVideoRec.CheckOnClick = true;
            this.toolStripButtonVideoRec.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonVideoRec.Enabled = false;
            this.toolStripButtonVideoRec.Image = global::uEye_DotNet_Cockpit.Properties.Resources.video;
            this.toolStripButtonVideoRec.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonVideoRec.Name = "toolStripButtonVideoRec";
            this.toolStripButtonVideoRec.Size = new System.Drawing.Size(36, 36);
            this.toolStripButtonVideoRec.Text = "Record video";
            this.toolStripButtonVideoRec.Click += new System.EventHandler(this.toolStripButtonVideoRec_Click);
            // 
            // toolStripButtonAbout
            // 
            this.toolStripButtonAbout.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButtonAbout.Image = global::uEye_DotNet_Cockpit.Properties.Resources.information;
            this.toolStripButtonAbout.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonAbout.Name = "toolStripButtonAbout";
            this.toolStripButtonAbout.Size = new System.Drawing.Size(36, 36);
            this.toolStripButtonAbout.Text = "About";
            this.toolStripButtonAbout.Click += new System.EventHandler(this.toolStripButtonAbout_Click);
            // 
            // toolStripMenuItemLoadImage
            // 
            this.toolStripMenuItemLoadImage.Enabled = false;
            this.toolStripMenuItemLoadImage.Image = global::uEye_DotNet_Cockpit.Properties.Resources.loadImage;
            this.toolStripMenuItemLoadImage.Name = "toolStripMenuItemLoadImage";
            this.toolStripMenuItemLoadImage.Size = new System.Drawing.Size(157, 22);
            this.toolStripMenuItemLoadImage.Text = "Load image";
            this.toolStripMenuItemLoadImage.Click += new System.EventHandler(this.toolStripButtonLoadImage_Click);
            // 
            // toolStripMenuItemSaveImage
            // 
            this.toolStripMenuItemSaveImage.Enabled = false;
            this.toolStripMenuItemSaveImage.Image = global::uEye_DotNet_Cockpit.Properties.Resources.saveImage;
            this.toolStripMenuItemSaveImage.Name = "toolStripMenuItemSaveImage";
            this.toolStripMenuItemSaveImage.Size = new System.Drawing.Size(157, 22);
            this.toolStripMenuItemSaveImage.Text = "Save image";
            this.toolStripMenuItemSaveImage.Click += new System.EventHandler(this.toolStripButtonSaveImage_Click);
            // 
            // toolStripMenuItemExit
            // 
            this.toolStripMenuItemExit.Image = global::uEye_DotNet_Cockpit.Properties.Resources.exit;
            this.toolStripMenuItemExit.Name = "toolStripMenuItemExit";
            this.toolStripMenuItemExit.Size = new System.Drawing.Size(157, 22);
            this.toolStripMenuItemExit.Text = "Exit";
            this.toolStripMenuItemExit.Click += new System.EventHandler(this.toolStripMenuItemExit_Click);
            // 
            // toolStripMenuItemOpenCameraLive
            // 
            this.toolStripMenuItemOpenCameraLive.Image = global::uEye_DotNet_Cockpit.Properties.Resources.Camera_Open_Freerun;
            this.toolStripMenuItemOpenCameraLive.Name = "toolStripMenuItemOpenCameraLive";
            this.toolStripMenuItemOpenCameraLive.Size = new System.Drawing.Size(194, 22);
            this.toolStripMenuItemOpenCameraLive.Text = "Open camera live";
            this.toolStripMenuItemOpenCameraLive.Click += new System.EventHandler(this.toolStripButtonOpenFreerun_Click);
            // 
            // toolStripMenuItemOpenCamera
            // 
            this.toolStripMenuItemOpenCamera.Image = global::uEye_DotNet_Cockpit.Properties.Resources.Camera_Open_Stop;
            this.toolStripMenuItemOpenCamera.Name = "toolStripMenuItemOpenCamera";
            this.toolStripMenuItemOpenCamera.Size = new System.Drawing.Size(194, 22);
            this.toolStripMenuItemOpenCamera.Text = "Open camera and stop";
            this.toolStripMenuItemOpenCamera.Click += new System.EventHandler(this.toolStripButtonOpenStop_Click);
            // 
            // toolStripMenuItemCloseCamera
            // 
            this.toolStripMenuItemCloseCamera.Enabled = false;
            this.toolStripMenuItemCloseCamera.Image = global::uEye_DotNet_Cockpit.Properties.Resources.Camera_Exit;
            this.toolStripMenuItemCloseCamera.Name = "toolStripMenuItemCloseCamera";
            this.toolStripMenuItemCloseCamera.Size = new System.Drawing.Size(194, 22);
            this.toolStripMenuItemCloseCamera.Text = "Close camera";
            this.toolStripMenuItemCloseCamera.Click += new System.EventHandler(this.toolStripButtonExit_Click);
            // 
            // toolStripMenuItemFreerun
            // 
            this.toolStripMenuItemFreerun.CheckOnClick = true;
            this.toolStripMenuItemFreerun.Enabled = false;
            this.toolStripMenuItemFreerun.Image = global::uEye_DotNet_Cockpit.Properties.Resources.Capture_Freerun;
            this.toolStripMenuItemFreerun.Name = "toolStripMenuItemFreerun";
            this.toolStripMenuItemFreerun.Size = new System.Drawing.Size(194, 22);
            this.toolStripMenuItemFreerun.Text = "Freerun";
            this.toolStripMenuItemFreerun.Click += new System.EventHandler(this.toolStripMenuItemFreerun_Click);
            // 
            // toolStripMenuItemSnapshot
            // 
            this.toolStripMenuItemSnapshot.Enabled = false;
            this.toolStripMenuItemSnapshot.Image = global::uEye_DotNet_Cockpit.Properties.Resources.Capture_Snap;
            this.toolStripMenuItemSnapshot.Name = "toolStripMenuItemSnapshot";
            this.toolStripMenuItemSnapshot.Size = new System.Drawing.Size(194, 22);
            this.toolStripMenuItemSnapshot.Text = "Snapshot";
            this.toolStripMenuItemSnapshot.Click += new System.EventHandler(this.toolStripButtonSnapshot_Click);
            // 
            // toolStripMenuItemApiErrorReport
            // 
            this.toolStripMenuItemApiErrorReport.CheckOnClick = true;
            this.toolStripMenuItemApiErrorReport.Image = global::uEye_DotNet_Cockpit.Properties.Resources.apiErrorReport;
            this.toolStripMenuItemApiErrorReport.Name = "toolStripMenuItemApiErrorReport";
            this.toolStripMenuItemApiErrorReport.Size = new System.Drawing.Size(194, 22);
            this.toolStripMenuItemApiErrorReport.Text = "Api Error report";
            this.toolStripMenuItemApiErrorReport.Click += new System.EventHandler(this.toolStripMenuItemApiErrorReport_Click);
            // 
            // displayToolStripMenuItem
            // 
            this.displayToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItemFitToWindow,
            this.toolStripMenuItem1To1,
            this.toolStripMenuItem1To2});
            this.displayToolStripMenuItem.Image = global::uEye_DotNet_Cockpit.Properties.Resources.monitor;
            this.displayToolStripMenuItem.Name = "displayToolStripMenuItem";
            this.displayToolStripMenuItem.Size = new System.Drawing.Size(163, 22);
            this.displayToolStripMenuItem.Text = "Display";
            // 
            // toolStripMenuItemFitToWindow
            // 
            this.toolStripMenuItemFitToWindow.CheckOnClick = true;
            this.toolStripMenuItemFitToWindow.Enabled = false;
            this.toolStripMenuItemFitToWindow.Image = global::uEye_DotNet_Cockpit.Properties.Resources.Display_FitWindow_1;
            this.toolStripMenuItemFitToWindow.Name = "toolStripMenuItemFitToWindow";
            this.toolStripMenuItemFitToWindow.Size = new System.Drawing.Size(146, 22);
            this.toolStripMenuItemFitToWindow.Text = "Fit to window";
            this.toolStripMenuItemFitToWindow.Click += new System.EventHandler(this.toolStripButtonFitToWnd_Click);
            // 
            // toolStripMenuItem1To1
            // 
            this.toolStripMenuItem1To1.CheckOnClick = true;
            this.toolStripMenuItem1To1.Enabled = false;
            this.toolStripMenuItem1To1.Image = global::uEye_DotNet_Cockpit.Properties.Resources.Display_1_1;
            this.toolStripMenuItem1To1.Name = "toolStripMenuItem1To1";
            this.toolStripMenuItem1To1.Size = new System.Drawing.Size(146, 22);
            this.toolStripMenuItem1To1.Text = "1 to 1";
            this.toolStripMenuItem1To1.Click += new System.EventHandler(this.toolStripButton1To1_Click);
            // 
            // toolStripMenuItem1To2
            // 
            this.toolStripMenuItem1To2.CheckOnClick = true;
            this.toolStripMenuItem1To2.Enabled = false;
            this.toolStripMenuItem1To2.Image = global::uEye_DotNet_Cockpit.Properties.Resources.Display_1_2;
            this.toolStripMenuItem1To2.Name = "toolStripMenuItem1To2";
            this.toolStripMenuItem1To2.Size = new System.Drawing.Size(146, 22);
            this.toolStripMenuItem1To2.Text = "1 to 2";
            this.toolStripMenuItem1To2.Click += new System.EventHandler(this.toolStripButton1To2_Click);
            // 
            // pictureBoxDisplay
            // 
            this.pictureBoxDisplay.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBoxDisplay.BackColor = System.Drawing.Color.Gainsboro;
            this.pictureBoxDisplay.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.pictureBoxDisplay.Image = global::uEye_DotNet_Cockpit.Properties.Resources.uEyeLogo;
            this.pictureBoxDisplay.Location = new System.Drawing.Point(0, 14);
            this.pictureBoxDisplay.Name = "pictureBoxDisplay";
            this.pictureBoxDisplay.Size = new System.Drawing.Size(753, 275);
            this.pictureBoxDisplay.TabIndex = 1;
            this.pictureBoxDisplay.TabStop = false;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(784, 562);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.toolStripMain);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.panelDisplay);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.Text = "Dotnet Cockpit";
            this.panelDisplay.ResumeLayout(false);
            this.toolStripMain.ResumeLayout(false);
            this.toolStripMain.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxDisplay)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panelDisplay;
        private System.Windows.Forms.PictureBox pictureBoxDisplay;
        private System.Windows.Forms.ToolStrip toolStripMain;
        private System.Windows.Forms.ToolStripButton toolStripButtonOpenFreerun;
        private System.Windows.Forms.ToolStripButton toolStripButtonOpenStop;
        private System.Windows.Forms.ToolStripButton toolStripButtonExit;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton toolStripButtonFreerun;
        private System.Windows.Forms.ToolStripButton toolStripButtonSnapshot;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton toolStripButtonFitToWnd;
        private System.Windows.Forms.ToolStripButton toolStripButton1To1;
        private System.Windows.Forms.ToolStripButton toolStripButton1To2;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton toolStripButtonSettings;
        private System.Windows.Forms.ToolStripButton toolStripButtonAES;
        private System.Windows.Forms.ToolStripButton toolStripButtonAWB;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripButton toolStripButtonSaveImage;
        private System.Windows.Forms.ToolStripButton toolStripButtonLoadImage;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripButton toolStripButtonAbout;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemLoadImage;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemSaveImage;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
        private System.Windows.Forms.ToolStripMenuItem loadParameterToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveParameterToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemExit;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemLoadParameterFromFile;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemLoadParameterFromEEPROM;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemSaveParameterToFile;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemSaveParameterToEEPROM;
        private System.Windows.Forms.ToolStripMenuItem cameraToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemOpenCameraLive;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemOpenCamera;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemCloseCamera;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator8;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemFreerun;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemSnapshot;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelFPS;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelFailed;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelFrameCount;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelCamera;
        private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem displayToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemFitToWindow;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1To1;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1To2;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator10;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemApiErrorReport;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator9;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemMirrorRightLeft;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemMirrorUpDown;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemCrosshair;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator11;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemTimestamp;
        private System.Windows.Forms.ToolStripButton toolStripButtonVideoRec;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator12;
    }
}

