using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace DotnetCockpit
{
    public partial class MainForm : Form
    {

        #region Variables

        // our camera class
        uEye.Camera m_Camera;

        uEye.Defines.DisplayRenderMode m_RenderMode; // render mode
        Boolean m_IsLive; // saves the capture state
        Int32 m_s32FrameCount; 
        private Timer m_UpdateTimer = new Timer();
        private const int m_cnNumberOfSeqBuffers = 3;

        #endregion

        public MainForm()
        {
            InitializeComponent();

            // Check Runtime Version
            Version verMin = new Version(3, 5);
            Boolean bOk = false;
            foreach (Version ver in InstalledDotNetVersions())
            {
                if (ver >= verMin)
                {
                    bOk = true;
                    break;
                }
            }

            if (!bOk)
            {
                this.Load += CloseOnStart;
            }

            pictureBoxDisplay.SizeMode = PictureBoxSizeMode.CenterImage;
            this.pictureBoxDisplay.Width = panelDisplay.Width;
            this.pictureBoxDisplay.Height = panelDisplay.Height;

            // initialize camera object
            // camera is not opened here
            m_Camera = new uEye.Camera();

            m_IsLive = false;
            m_RenderMode = uEye.Defines.DisplayRenderMode.FitToWindow;

            m_UpdateTimer.Interval = 100;
            m_UpdateTimer.Tick += OnUpdateControls;
        }

        private void CloseOnStart(object sender, EventArgs e)
        {
            MessageBox.Show(".NET Runtime Version 3.5.0 is required", "Runtime Error");
            this.Close();
        }

        public static System.Collections.ObjectModel.Collection<Version> InstalledDotNetVersions()
        {
            System.Collections.ObjectModel.Collection<Version> versions = new System.Collections.ObjectModel.Collection<Version>();
            Microsoft.Win32.RegistryKey NDPKey = Microsoft.Win32.Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Microsoft\NET Framework Setup\NDP");
            if (NDPKey != null)
            {
                string[] subkeys = NDPKey.GetSubKeyNames();
                foreach (string subkey in subkeys)
                {
                    GetDotNetVersion(NDPKey.OpenSubKey(subkey), subkey, versions);
                    GetDotNetVersion(NDPKey.OpenSubKey(subkey).OpenSubKey("Client"), subkey, versions);
                    GetDotNetVersion(NDPKey.OpenSubKey(subkey).OpenSubKey("Full"), subkey, versions);
                }
            }
            return versions;
        }

        private static void GetDotNetVersion(Microsoft.Win32.RegistryKey parentKey, string subVersionName, System.Collections.ObjectModel.Collection<Version> versions)
        {
            if (parentKey != null)
            {
                string installed = Convert.ToString(parentKey.GetValue("Install"));
                if (installed == "1")
                {
                    string version = Convert.ToString(parentKey.GetValue("Version"));
                    if (string.IsNullOrEmpty(version))
                    {
                        if (subVersionName.StartsWith("v"))
                            version = subVersionName.Substring(1);
                        else
                            version = subVersionName;
                    }

                    Version ver = new Version(version);

                    if (!versions.Contains(ver))
                        versions.Add(ver);
                }
            }
        }

        private void OnUpdateControls(object sender, EventArgs e)
        {
            // we update here our statusbar 
            Double dFramerate;
            m_Camera.Timing.Framerate.GetCurrentFps(out dFramerate);
            toolStripStatusLabelFPS.Text = "Fps: " + dFramerate.ToString("00.00");

            uEye.Types.CaptureStatus captureStatus;
            m_Camera.Information.GetCaptureStatus(out captureStatus);

            if (null != captureStatus)
            {
                toolStripStatusLabelFailed.Text = "Failed: " + captureStatus.Total;
            }

            toolStripStatusLabelFrameCount.Text = "Frames: " + m_s32FrameCount;
        }

        private uEye.Defines.Status initCamera()
        {
            CameraChoose chooseForm = new CameraChoose();
            uEye.Defines.Status statusRet = uEye.Defines.Status.NO_SUCCESS;
            
            if (chooseForm.ShowDialog() == DialogResult.OK)
            {
                statusRet = m_Camera.Init(chooseForm.DeviceID | (Int32)uEye.Defines.DeviceEnumeration.UseDeviceID, pictureBoxDisplay.Handle);
                if (statusRet != uEye.Defines.Status.SUCCESS)
                {
                    MessageBox.Show("Initializing the camera failed");
                    return statusRet;
                }

                statusRet = MemoryHelper.AllocImageMems(m_Camera, m_cnNumberOfSeqBuffers);
                if (statusRet != uEye.Defines.Status.SUCCESS)
                {
                    MessageBox.Show("Allocating memory failed");
                    return statusRet;
                }

                statusRet = MemoryHelper.InitSequence(m_Camera);
                if (statusRet != uEye.Defines.Status.SUCCESS)
                {
                    MessageBox.Show("Add to sequence failed");
                    return statusRet;
                }

                // set event
                m_Camera.EventFrame += onFrameEvent;

                // reset framecount
                m_s32FrameCount = 0;

                // start update timer for our statusbar
                m_UpdateTimer.Start();

                uEye.Types.SensorInfo sensorInfo;
                m_Camera.Information.GetSensorInfo(out sensorInfo);

                pictureBoxDisplay.SizeMode = PictureBoxSizeMode.Normal;
                toolStripStatusLabelCamera.Text = sensorInfo.SensorName;    
            }

            return statusRet;
        }

        private void UpdateToolbar()
        {
            toolStripButton1To1.Enabled = m_Camera.IsOpened;
            toolStripButton1To2.Enabled = m_Camera.IsOpened;
            toolStripButtonFitToWnd.Enabled = m_Camera.IsOpened;

            // general function to update all mainwindow controls
            if (m_Camera.IsOpened)
            {
                // check if directrender or dib mode is active
                uEye.Defines.DisplayMode displayMode;
                m_Camera.Display.Mode.Get(out displayMode);
    
                toolStripButton1To2.Enabled = displayMode == uEye.Defines.DisplayMode.DiB;
            }

            // enable/disable controls
            toolStripButtonAES.Enabled = m_Camera.IsOpened ? m_Camera.AutoFeatures.Software.Shutter.Supported : false;
            toolStripButtonAWB.Enabled = m_Camera.IsOpened ? m_Camera.AutoFeatures.Software.WhiteBalance.Supported : false;
            toolStripButtonExit.Enabled = m_Camera.IsOpened;
            toolStripButtonFreerun.Enabled = m_Camera.IsOpened;
            toolStripButtonSettings.Enabled = m_Camera.IsOpened;
            toolStripButtonSnapshot.Enabled = m_Camera.IsOpened;
            toolStripButtonSaveImage.Enabled = m_Camera.IsOpened;
            toolStripButtonLoadImage.Enabled = m_Camera.IsOpened;
            toolStripButtonVideoRec.Enabled = m_Camera.IsOpened;

            toolStripButtonOpenFreerun.Enabled = !m_Camera.IsOpened;
            toolStripButtonOpenStop.Enabled = !m_Camera.IsOpened;
            
            toolStripButtonFreerun.Checked = m_Camera.IsOpened ? m_IsLive : false;
            toolStripButtonVideoRec.Checked = m_Camera.IsOpened ? m_Camera.Video.Running : false;

            toolStripMenuItemCloseCamera.Enabled = m_Camera.IsOpened;
            toolStripMenuItemLoadImage.Enabled = m_Camera.IsOpened;
            toolStripMenuItemLoadParameterFromEEPROM.Enabled = m_Camera.IsOpened;
            toolStripMenuItemLoadParameterFromFile.Enabled = m_Camera.IsOpened;
            toolStripMenuItemOpenCamera.Enabled = !m_Camera.IsOpened;
            toolStripMenuItemOpenCameraLive.Enabled = !m_Camera.IsOpened;
            toolStripMenuItemSaveImage.Enabled = m_Camera.IsOpened;
            toolStripMenuItemSaveParameterToEEPROM.Enabled = m_Camera.IsOpened;
            toolStripMenuItemSaveParameterToFile.Enabled = m_Camera.IsOpened;

            toolStripMenuItemSnapshot.Enabled = m_Camera.IsOpened;
            toolStripMenuItemFreerun.Enabled = m_Camera.IsOpened;
            toolStripMenuItemFreerun.Checked = m_Camera.IsOpened ? m_IsLive : false;

            toolStripMenuItem1To1.Enabled = m_Camera.IsOpened;
            toolStripMenuItem1To2.Enabled = m_Camera.IsOpened;
            toolStripMenuItemFitToWindow.Enabled = m_Camera.IsOpened;

            toolStripMenuItemMirrorRightLeft.Enabled = m_Camera.IsOpened;
            toolStripMenuItemMirrorUpDown.Enabled = m_Camera.IsOpened;
            toolStripMenuItemCrosshair.Enabled = m_Camera.IsOpened;
            toolStripMenuItemTimestamp.Enabled = m_Camera.IsOpened;
            toolStripMenuItemApiErrorReport.Enabled = m_Camera.IsOpened;

            toolStripButtonFitToWnd.Checked = false;
            toolStripButton1To1.Checked = false;
            toolStripButton1To2.Checked = false;

            toolStripMenuItemFitToWindow.Checked = false;
            toolStripMenuItem1To1.Checked = false;
            toolStripMenuItem1To2.Checked = false;

            toolStripStatusLabelFPS.Visible = m_Camera.IsOpened;
            toolStripStatusLabelFailed.Visible = m_Camera.IsOpened;
            toolStripStatusLabelFrameCount.Visible = m_Camera.IsOpened;
            toolStripStatusLabelCamera.Visible = m_Camera.IsOpened;

            if (m_Camera.IsOpened)
            {
                // update render mode
                switch (m_RenderMode)
                {
                    case uEye.Defines.DisplayRenderMode.FitToWindow:
                        toolStripButtonFitToWnd.Checked = true;
                        toolStripMenuItemFitToWindow.Checked = true;
                        break;

                    case uEye.Defines.DisplayRenderMode.Normal:
                        toolStripButton1To1.Checked = true;
                        toolStripMenuItem1To1.Checked = true;
                        break;

                    case uEye.Defines.DisplayRenderMode.DownScale_1_2:
                        toolStripButton1To2.Checked = true;
                        toolStripMenuItem1To2.Checked = true;
                        break;
                }

                // update 
                Boolean isEnabled;
                m_Camera.AutoFeatures.Software.WhiteBalance.GetEnable(out isEnabled);
                toolStripButtonAWB.Checked = isEnabled;

                m_Camera.AutoFeatures.Software.Shutter.GetEnable(out isEnabled);
                toolStripButtonAES.Checked = isEnabled;
            }
            else
            {
                toolStripMenuItemCrosshair.Checked = false;
                toolStripMenuItemTimestamp.Checked = false;

                toolStripButtonAES.Checked = false;
                toolStripButtonAWB.Checked = false;
            }
        }

        private void onFrameEvent(object sender, EventArgs e)
        {
            // convert sender object to our camera object
            uEye.Camera camera = sender as uEye.Camera;

            if (camera.IsOpened)
            {
                uEye.Defines.DisplayMode mode;
                camera.Display.Mode.Get(out mode);

                // only display in dib mode
                if (mode == uEye.Defines.DisplayMode.DiB)
                {
                    Int32 s32MemID;
                    uEye.Defines.Status statusRet = camera.Memory.GetLast(out s32MemID);

                    if ((uEye.Defines.Status.SUCCESS == statusRet) && (0 < s32MemID))
                    {
                        if (uEye.Defines.Status.SUCCESS == camera.Memory.Lock(s32MemID))
                        {
                            // do any drawings?
                            if (toolStripMenuItemTimestamp.Checked || toolStripMenuItemCrosshair.Checked)
                            {
                                Bitmap bitmap;
                                m_Camera.Memory.ToBitmap(s32MemID, out bitmap);

                                if (bitmap != null && bitmap.PixelFormat != System.Drawing.Imaging.PixelFormat.Format8bppIndexed)
                                {
                                    Graphics graphics = Graphics.FromImage(bitmap);
                                    DoDrawing(ref graphics, s32MemID);

                                    graphics.Dispose();
                                    bitmap.Dispose();
                                }
                            }

                            camera.Display.Render(s32MemID, m_RenderMode);
                            camera.Memory.Unlock(s32MemID);
                        }
                    }
                }

                ++m_s32FrameCount;
            }
        }

        #region Toolbar Events

        private void DoDrawing(ref Graphics graphics, Int32 s32MemID)
        {
            if (toolStripMenuItemCrosshair.Checked)
            {
                // get image size
                System.Drawing.Rectangle rect;
                m_Camera.Size.AOI.Get(out rect);

                graphics.DrawLine(new Pen(Color.Red, 3), rect.Width / 2, 0, rect.Width / 2, rect.Height);
                graphics.DrawLine(new Pen(Color.Red, 3), 0, rect.Height / 2, rect.Width, rect.Height / 2);
            }

            if (toolStripMenuItemTimestamp.Checked && s32MemID != 0)
            {
                uEye.Types.ImageInfo imageInfo;
                m_Camera.Information.GetImageInfo(s32MemID, out imageInfo);

                graphics.DrawString(imageInfo.TimestampSystem.ToString(), new Font("Courier", 16), Brushes.Red, 0, 0);
            }
        }

        private void toolStripButtonOpenFreerun_Click(object sender, EventArgs e)
        {
            uEye.Defines.Status statusRet;
            statusRet = initCamera();

            if (statusRet == uEye.Defines.Status.SUCCESS)
            {
                // start capture
                statusRet = m_Camera.Acquisition.Capture();
                if (statusRet != uEye.Defines.Status.SUCCESS)
                {
                    MessageBox.Show("Starting live video failed");
                }
                else
                {
                    // everything is ok
                    m_IsLive = true;
                    UpdateToolbar();
                }
            }

            // cleanup on any camera error
            if (statusRet != uEye.Defines.Status.SUCCESS && m_Camera.IsOpened)
            {
                m_Camera.Exit();
            }
        }

        private void toolStripButtonOpenStop_Click(object sender, EventArgs e)
        {
            uEye.Defines.Status statusRet;
            statusRet = initCamera();

            if (statusRet == uEye.Defines.Status.SUCCESS)
            {
                // start capture
                statusRet = m_Camera.Acquisition.Freeze();
                if (statusRet != uEye.Defines.Status.SUCCESS)
                {
                    MessageBox.Show("Starting live video failed");
                }
                else
                {
                    // everything is ok
                    m_IsLive = false;
                    UpdateToolbar();
                }
            }

            // cleanup on any camera error
            if (statusRet != uEye.Defines.Status.SUCCESS && m_Camera.IsOpened)
            {
                m_Camera.Exit();
            }
        }

        private void toolStripButtonExit_Click(object sender, EventArgs e)
        {
            m_UpdateTimer.Stop();

            m_IsLive = false;
            m_Camera.EventFrame -= onFrameEvent;
            m_Camera.Exit();

            UpdateToolbar();

            // set correct display size
            this.pictureBoxDisplay.Width = panelDisplay.Width;
            this.pictureBoxDisplay.Height = panelDisplay.Height;
            this.pictureBoxDisplay.Anchor = AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Top;

            pictureBoxDisplay.Invalidate();
            pictureBoxDisplay.SizeMode = PictureBoxSizeMode.CenterImage;

            m_RenderMode = uEye.Defines.DisplayRenderMode.FitToWindow;
        }

        private void toolStripButtonFitToWnd_Click(object sender, EventArgs e)
        {
            // render mode == Fit to window
            m_RenderMode = uEye.Defines.DisplayRenderMode.FitToWindow;

            this.pictureBoxDisplay.Width = panelDisplay.Width;
            this.pictureBoxDisplay.Height = panelDisplay.Height;

            this.pictureBoxDisplay.Anchor = AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Top;

            // direct render
            uEye.Defines.DisplayMode mode;
            m_Camera.Display.Mode.Get(out mode);
            if (mode != uEye.Defines.DisplayMode.DiB)
            {
                m_Camera.DirectRenderer.SetScaling(true);
            }

            UpdateToolbar();
        }

        private void toolStripButton1To1_Click(object sender, EventArgs e)
        {
            // render mode == 1 to 1
            m_RenderMode = uEye.Defines.DisplayRenderMode.Normal;

            this.pictureBoxDisplay.Anchor = AnchorStyles.Top | AnchorStyles.Left;

            // get image size
            System.Drawing.Rectangle rect;
            m_Camera.Size.AOI.Get(out rect);

            this.pictureBoxDisplay.Width = rect.Width;
            this.pictureBoxDisplay.Height = rect.Height;

            // direct render
            uEye.Defines.DisplayMode mode;
            m_Camera.Display.Mode.Get(out mode);
            if (mode != uEye.Defines.DisplayMode.DiB)
            {
                m_Camera.DirectRenderer.SetScaling(false);
            }

            UpdateToolbar();
        }

        private void toolStripButton1To2_Click(object sender, EventArgs e)
        {
            // render mode == 1 to 2
            m_RenderMode = uEye.Defines.DisplayRenderMode.DownScale_1_2;

            this.pictureBoxDisplay.Anchor = AnchorStyles.Top | AnchorStyles.Left;

            // get image size
            System.Drawing.Rectangle rect;
            m_Camera.Size.AOI.Get(out rect);

            this.pictureBoxDisplay.Width = rect.Width / 2;
            this.pictureBoxDisplay.Height = rect.Height / 2;

            UpdateToolbar();
        }

        #endregion

        private void toolStripButtonFreerun_Click(object sender, EventArgs e)
        {
            uEye.Defines.Status statusRet;
            if (toolStripButtonFreerun.Checked)
            {
                // start capture
                statusRet = m_Camera.Acquisition.Capture();
                m_IsLive = true;
            }
            else
            {
                statusRet = m_Camera.Acquisition.Stop();
                m_IsLive = false;
            }

            UpdateToolbar();
        }

        private void toolStripButtonSnapshot_Click(object sender, EventArgs e)
        {
            uEye.Defines.Status statusRet;

            // start capture
            statusRet = m_Camera.Acquisition.Freeze();
            m_IsLive = false;

            UpdateToolbar();
        }

        private void OnDisplayChanged(object sender, EventArgs e)
        {
            uEye.Defines.DisplayMode displayMode;
            m_Camera.Display.Mode.Get(out displayMode);

            // set scaling options
            if (displayMode != uEye.Defines.DisplayMode.DiB)
            {
                if (m_RenderMode == uEye.Defines.DisplayRenderMode.DownScale_1_2)
                {
                    m_RenderMode = uEye.Defines.DisplayRenderMode.Normal;

                    this.pictureBoxDisplay.Anchor = AnchorStyles.Top | AnchorStyles.Left;

                    // get image size
                    System.Drawing.Rectangle rect;
                    m_Camera.Size.AOI.Get(out rect);

                    this.pictureBoxDisplay.Width = rect.Width;
                    this.pictureBoxDisplay.Height = rect.Height;
                }
                else
                {
                    m_Camera.DirectRenderer.SetScaling(m_RenderMode == uEye.Defines.DisplayRenderMode.FitToWindow);
                }

                // update drawings
                toolStripMenuItemCrosshair_Click(null, EventArgs.Empty);
            }
            else
            {
                if (m_RenderMode != uEye.Defines.DisplayRenderMode.FitToWindow)
                {
                    this.pictureBoxDisplay.Anchor = AnchorStyles.Top | AnchorStyles.Left;

                    // get image size
                    System.Drawing.Rectangle rect;
                    m_Camera.Size.AOI.Get(out rect);

                    if (m_RenderMode != uEye.Defines.DisplayRenderMode.Normal)
                    {

                        this.pictureBoxDisplay.Width = rect.Width / 2;
                        this.pictureBoxDisplay.Height = rect.Height / 2;
                    }
                    else
                    {
                        this.pictureBoxDisplay.Width = rect.Width;
                        this.pictureBoxDisplay.Height = rect.Height;
                    }
                }
            }

            UpdateToolbar();
        }

        private void toolStripButtonSettings_Click(object sender, EventArgs e)
        {
            uEye.Types.SensorInfo sensorInfo;
            m_Camera.Information.GetSensorInfo(out sensorInfo);

            if (sensorInfo.SensorID != uEye.Defines.Sensor.XS &&
                sensorInfo.SensorID != uEye.Defines.Sensor.UI1008_C &&
                sensorInfo.SensorID != uEye.Defines.Sensor.UI1013XC)
            {
                // avoid multiple instances
                SettingsForm settingsForm = new SettingsForm(m_Camera);
                settingsForm.SizeControl.AOIChanged += OnDisplayChanged;
                settingsForm.FormatControl.DisplayChanged += OnDisplayChanged;
                settingsForm.ShowDialog();

                Boolean isEnabled;

                // check if any autofeature is enabled
                m_Camera.AutoFeatures.Software.WhiteBalance.GetEnable(out isEnabled);
                toolStripButtonAWB.Checked = isEnabled;

                m_Camera.AutoFeatures.Software.Shutter.GetEnable(out isEnabled);
                toolStripButtonAES.Checked = isEnabled;

                UpdateToolbar();
            }
            else
            {
                MessageBox.Show("Settings are not supported with this type of camera!");
            }
        }

        private void toolStripButtonAES_Click(object sender, EventArgs e)
        {
            Boolean isEnabled;
            m_Camera.AutoFeatures.Software.Shutter.GetEnable(out isEnabled);
            
            m_Camera.AutoFeatures.Software.Shutter.SetEnable(!isEnabled);
            toolStripButtonAES.Checked = !isEnabled;
        }

        private void toolStripButtonAWB_Click(object sender, EventArgs e)
        {
            Boolean isEnabled;
            m_Camera.AutoFeatures.Software.WhiteBalance.GetEnable(out isEnabled);

            m_Camera.AutoFeatures.Software.WhiteBalance.SetEnable(!isEnabled);
            toolStripButtonAWB.Checked = !isEnabled;
        }

        private void toolStripButtonSaveImage_Click(object sender, EventArgs e)
        {
            m_Camera.Image.Save("");
        }

        private void toolStripButtonLoadImage_Click(object sender, EventArgs e)
        {
            uEye.Defines.Status statusRet;
            statusRet = m_Camera.Image.Load("");

            if (statusRet == uEye.Defines.Status.SUCCESS)
            {
                // update drawing
                onFrameEvent(m_Camera, EventArgs.Empty);
            }
        }

        private void toolStripButtonAbout_Click(object sender, EventArgs e)
        {
            AboutBox aboutBox = new AboutBox();
            aboutBox.ShowDialog();
        }

        private void toolStripMenuItemLoadParameterFromFile_Click(object sender, EventArgs e)
        {
            if (m_IsLive)
            {
                m_Camera.Acquisition.Stop();
            }

            MemoryHelper.ClearSequence(m_Camera);
            MemoryHelper.FreeImageMems(m_Camera);

            m_Camera.Parameter.Load("");

            uEye.Defines.ColorMode colorMode;
            m_Camera.PixelFormat.Get(out colorMode);

            // allocate new standard memory
            MemoryHelper.AllocImageMems(m_Camera, m_cnNumberOfSeqBuffers);
            MemoryHelper.InitSequence(m_Camera);

            if (m_IsLive)
            {
                m_Camera.Acquisition.Capture();
            }

            UpdateToolbar();
        }

        private void toolStripMenuItemLoadParameterFromEEPROM_Click(object sender, EventArgs e)
        {
            if (m_IsLive)
            {
                m_Camera.Acquisition.Stop();
            }

            MemoryHelper.ClearSequence(m_Camera);
            MemoryHelper.FreeImageMems(m_Camera);

            m_Camera.Parameter.Load();

            uEye.Defines.ColorMode colorMode;
            m_Camera.PixelFormat.Get(out colorMode);

            // allocate new standard memory
            MemoryHelper.AllocImageMems(m_Camera, m_cnNumberOfSeqBuffers);
            MemoryHelper.InitSequence(m_Camera);

            if (m_IsLive)
            {
                m_Camera.Acquisition.Capture();
            }

            UpdateToolbar();
        }

        private void toolStripMenuItemSaveParameterToFile_Click(object sender, EventArgs e)
        {
            m_Camera.Parameter.Save("");
        }

        private void toolStripMenuItemSaveParameterToEEPROM_Click(object sender, EventArgs e)
        {
            m_Camera.Parameter.Save();
        }

        private void toolStripMenuItemExit_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void toolStripMenuItemMirrorRightLeft_Click(object sender, EventArgs e)
        {
            m_Camera.RopEffect.Set(uEye.Defines.RopEffectMode.LeftRight, toolStripMenuItemMirrorRightLeft.Checked);
        }

        private void toolStripMenuItemMirrorUpDown_Click(object sender, EventArgs e)
        {
            m_Camera.RopEffect.Set(uEye.Defines.RopEffectMode.UpDown, toolStripMenuItemMirrorUpDown.Checked);
        }

        private void toolStripMenuItemApiErrorReport_Click(object sender, EventArgs e)
        {
            m_Camera.Information.SetEnableErrorReport(toolStripMenuItemApiErrorReport.Checked);
        }

        private void viewToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            if (m_Camera.IsOpened)
            {
                uEye.Defines.Status statusRet;

                // update selected ropeffect
                uEye.Defines.RopEffectMode ropMode;
                statusRet = m_Camera.RopEffect.Get(out ropMode);

                toolStripMenuItemMirrorRightLeft.Checked = (ropMode & uEye.Defines.RopEffectMode.LeftRight) == uEye.Defines.RopEffectMode.LeftRight;
                toolStripMenuItemMirrorUpDown.Checked = (ropMode & uEye.Defines.RopEffectMode.UpDown) == uEye.Defines.RopEffectMode.UpDown;

                uEye.Defines.DisplayMode displayMode;
                statusRet = m_Camera.Display.Mode.Get(out displayMode);

                // directrenderer
                toolStripMenuItemTimestamp.Enabled = displayMode == uEye.Defines.DisplayMode.DiB;
            }
        }

        private void toolStripMenuItemCrosshair_Click(object sender, EventArgs e)
        {
            uEye.Defines.Status statusRet;

            uEye.Defines.DisplayMode mode;
            statusRet = m_Camera.Display.Mode.Get(out mode);

            // directrenderer
            if (mode != uEye.Defines.DisplayMode.DiB)
            {
                statusRet = m_Camera.DirectRenderer.SetWindowHandle(pictureBoxDisplay.Handle.ToInt32());
                statusRet = m_Camera.DirectRenderer.Overlay.SetVisible(toolStripMenuItemCrosshair.Checked);

                Graphics graphics;
                statusRet = m_Camera.DirectRenderer.Overlay.GetGraphics(out graphics);

                if (statusRet == uEye.Defines.Status.SUCCESS && graphics != null)
                {
                    DoDrawing(ref graphics, 0);
                    statusRet = m_Camera.DirectRenderer.Overlay.SetGraphics(ref graphics);
                }
            }
        }

        private void toolStripMenuItemFreerun_Click(object sender, EventArgs e)
        {
            uEye.Defines.Status statusRet;
            if (toolStripMenuItemFreerun.Checked)
            {
                // start capture
                statusRet = m_Camera.Acquisition.Capture();
                m_IsLive = true;
            }
            else
            {
                statusRet = m_Camera.Acquisition.Stop();
                m_IsLive = false;
            }

            UpdateToolbar();
        }

        private void toolStripButtonVideoRec_Click(object sender, EventArgs e)
        {
            uEye.Defines.Status statusRet = uEye.Defines.Status.SUCCESS;

            if (toolStripButtonVideoRec.Checked)
            {
                SaveFileDialog fileDialog = new SaveFileDialog();
                fileDialog.Filter = "Video file (*.avi)|*.avi";
                fileDialog.DefaultExt = "avi";
                if (fileDialog.ShowDialog() == DialogResult.OK)
                {
                    statusRet = m_Camera.Video.Start(fileDialog.FileName);
                    if (statusRet != uEye.Defines.Status.SUCCESS)
                    {
                        MessageBox.Show("Could not start video recording");
                        toolStripButtonVideoRec.Checked = false;
                    }
                }
                else
                {
                    toolStripButtonVideoRec.Checked = false;
                }
            }
            else
            {
                statusRet = m_Camera.Video.Stop();
            }
        }
    }
}
