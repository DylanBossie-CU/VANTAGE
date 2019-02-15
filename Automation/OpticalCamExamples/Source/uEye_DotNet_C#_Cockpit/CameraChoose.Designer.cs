namespace DotnetCockpit
{
    partial class CameraChoose
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CameraChoose));
            this.listViewCamera = new System.Windows.Forms.ListView();
            this.columnHeaderAvailable = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderCamID = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderDevID = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderModell = new System.Windows.Forms.ColumnHeader();
            this.columnHeaderSerNr = new System.Windows.Forms.ColumnHeader();
            this.imageListCamera = new System.Windows.Forms.ImageList(this.components);
            this.buttonOk = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // listViewCamera
            // 
            this.listViewCamera.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.listViewCamera.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeaderAvailable,
            this.columnHeaderCamID,
            this.columnHeaderDevID,
            this.columnHeaderModell,
            this.columnHeaderSerNr});
            this.listViewCamera.GridLines = true;
            this.listViewCamera.Location = new System.Drawing.Point(0, 0);
            this.listViewCamera.MultiSelect = false;
            this.listViewCamera.Name = "listViewCamera";
            this.listViewCamera.Size = new System.Drawing.Size(486, 201);
            this.listViewCamera.SmallImageList = this.imageListCamera;
            this.listViewCamera.TabIndex = 0;
            this.listViewCamera.UseCompatibleStateImageBehavior = false;
            this.listViewCamera.View = System.Windows.Forms.View.Details;
            this.listViewCamera.DoubleClick += new System.EventHandler(this.listViewCamera_DoubleClick);
            // 
            // columnHeaderAvailable
            // 
            this.columnHeaderAvailable.Text = "Available";
            // 
            // columnHeaderCamID
            // 
            this.columnHeaderCamID.Text = "Camera ID";
            this.columnHeaderCamID.Width = 74;
            // 
            // columnHeaderDevID
            // 
            this.columnHeaderDevID.Text = "Device ID";
            // 
            // columnHeaderModell
            // 
            this.columnHeaderModell.Text = "Model";
            // 
            // columnHeaderSerNr
            // 
            this.columnHeaderSerNr.Text = "SerNr.";
            this.columnHeaderSerNr.Width = 228;
            // 
            // imageListCamera
            // 
            this.imageListCamera.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListCamera.ImageStream")));
            this.imageListCamera.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListCamera.Images.SetKeyName(0, "Camera_Open_Freerun.png");
            this.imageListCamera.Images.SetKeyName(1, "Camera_Exit.png");
            // 
            // buttonOk
            // 
            this.buttonOk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonOk.Location = new System.Drawing.Point(329, 207);
            this.buttonOk.Name = "buttonOk";
            this.buttonOk.Size = new System.Drawing.Size(75, 23);
            this.buttonOk.TabIndex = 1;
            this.buttonOk.Text = "Ok";
            this.buttonOk.UseVisualStyleBackColor = true;
            this.buttonOk.Click += new System.EventHandler(this.listViewCamera_DoubleClick);
            // 
            // buttonCancel
            // 
            this.buttonCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonCancel.Location = new System.Drawing.Point(411, 207);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 2;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // CameraChoose
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(490, 240);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonOk);
            this.Controls.Add(this.listViewCamera);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "CameraChoose";
            this.Text = "CameraChoose";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.CameraChoose_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listViewCamera;
        private System.Windows.Forms.ColumnHeader columnHeaderAvailable;
        private System.Windows.Forms.ColumnHeader columnHeaderCamID;
        private System.Windows.Forms.ColumnHeader columnHeaderDevID;
        private System.Windows.Forms.ColumnHeader columnHeaderModell;
        private System.Windows.Forms.ColumnHeader columnHeaderSerNr;
        private System.Windows.Forms.ImageList imageListCamera;
        private System.Windows.Forms.Button buttonOk;
        private System.Windows.Forms.Button buttonCancel;
    }
}