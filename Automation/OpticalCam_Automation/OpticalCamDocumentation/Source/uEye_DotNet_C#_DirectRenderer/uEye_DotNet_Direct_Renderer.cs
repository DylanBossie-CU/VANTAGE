using System;
using System.Drawing;
using System.Windows.Forms;

namespace uEye_DotNet_Direct_Renderer
{
    public partial class uEye_DotNet_Direct_Renderer : Form
    {
        uEye.Camera m_Camera;
        private uint m_u32ImageWidth = 0;
        private uint m_u32ImageHeight = 0;

        // overlay moving
        private Int32 m_s32MovePosX = 1;
        private Int32 m_s32MovePosY = 1;

        // overlay position
        private UInt32 m_u32OverlayPositionX = 0;
        private UInt32 m_u32OverlayPositionY = 0;

        private System.Drawing.Color m_OverlayColor = System.Drawing.Color.Black;

        // overlay moving update timer
        Timer m_OverlayMoveTimer = new Timer();

        public uEye_DotNet_Direct_Renderer()
        {
            InitializeComponent();
        }

        private void uEye_DotNet_Direct_Renderer_Shown(object sender, EventArgs e)
        {
            bool bDirect3D = false;
            bool bOpenGL = false;

            uEye.Defines.Status statusRet;
            m_Camera = new uEye.Camera();

            cB_Semi_transparent.Enabled = false;

            // open first available camera
            statusRet = m_Camera.Init(0, DisplayWindow.Handle.ToInt32());
            if (statusRet == uEye.Defines.Status.SUCCESS)
            {
                m_OverlayMoveTimer.Interval = 10;
                m_OverlayMoveTimer.Tick += new EventHandler(OnOverlayMove);

                uEye.Defines.DisplayMode supportedMode;
                statusRet = m_Camera.DirectRenderer.GetSupported(out supportedMode);

                if ((supportedMode & uEye.Defines.DisplayMode.Direct3D) == uEye.Defines.DisplayMode.Direct3D)
                {
                    rB_Direct3D.Enabled = true;
                    bDirect3D = true;
                }
                else
                {
                    rB_Direct3D.Enabled = false;
                    bDirect3D = false;
                }

                if ((supportedMode & uEye.Defines.DisplayMode.OpenGL) == uEye.Defines.DisplayMode.OpenGL)
                {
                    rB_OpenGL.Enabled = true;
                    bOpenGL = true;

                    if (rB_Direct3D.Enabled != true)
                    {
                        rB_OpenGL.Checked = true;
                    }
                }
                else
                {
                    rB_OpenGL.Enabled = false;
                    bOpenGL = false;
                }

                if (((supportedMode & uEye.Defines.DisplayMode.Direct3D) == uEye.Defines.DisplayMode.Direct3D) ||
                    ((supportedMode & uEye.Defines.DisplayMode.OpenGL) == uEye.Defines.DisplayMode.OpenGL))
                {

                    if (bOpenGL == true)
                    {
                        // set display mode
                        statusRet = m_Camera.Display.Mode.Set(uEye.Defines.DisplayMode.OpenGL);
                    }

                    if (bDirect3D == true)
                    {
                        // set display mode
                        statusRet = m_Camera.Display.Mode.Set(uEye.Defines.DisplayMode.Direct3D);
                    }

                    // start live
                    Button_Camera_Open_Freerun_Click(null, EventArgs.Empty);

                    // update information
                    UpdateOverlayInformation();
                    UpdateImageInformation();

                    // set default key color
                    m_OverlayColor = System.Drawing.Color.Black;
                    Button_Key_Color.BackColor = m_OverlayColor;
                }
                else
                {
                    MessageBox.Show("Direct3D and OpenGL are not supported");
                    Close();
                }
            }
            else
            {
                MessageBox.Show("Could not open camera...");
                Close();
            }
        }

        private void UpdateOverlayInformation()
        {
            uEye.Types.Size<UInt32> overlaySize;
            uEye.Defines.Status statusRet;

            statusRet = m_Camera.DirectRenderer.Overlay.GetSize(out overlaySize);

            tB_Overlay_Width.Text = overlaySize.Width.ToString();
            tB_Overlay_Height.Text = overlaySize.Height.ToString();
        }

        private void UpdateImageInformation()
        {
            /* open the camera */
            System.Drawing.Rectangle imageRect;
            uEye.Defines.Status statusRet;

            statusRet = m_Camera.Size.AOI.Get(out imageRect);

            m_u32ImageWidth = (uint)imageRect.Width;
            m_u32ImageHeight = (uint)imageRect.Height;

            /* Image Info*/
            tB_Image_Width.Text = imageRect.Width.ToString();
            tB_Image_Height.Text = imageRect.Height.ToString();
        }
        
        private void Button_Camera_Open_Freerun_Click(object sender, EventArgs e)
        {             
            /* Start the camera acquisition */
            uEye.Defines.Status statusRet;
            statusRet = m_Camera.Acquisition.Capture();

            /* active keys Strart Live and Stop Live */
            Button_Camera_Open_Freerun.Enabled = false;
            Button_Camera_Open_Stop.Enabled = true;
        }

        private void Button_Camera_Open_Stop_Click(object sender, EventArgs e)
        {
            /* Stop the camera acquisition */
            uEye.Defines.Status statusRet;
            statusRet = m_Camera.Acquisition.Stop();
 
            /* active keys Strart Live and Stop Live */
            Button_Camera_Open_Freerun.Enabled = true;
            Button_Camera_Open_Stop.Enabled = false;
        }

        private void Button_Exit_Click(object sender, EventArgs e)
        {
            /* Close the camera */
            m_Camera.Exit();
            Close();
        }

        private void uEye_DotNet_Simple_Live_FormClosing(object sender, FormClosingEventArgs e)
        {
            /* Close the camera */
            m_Camera.Exit();
        }
              
        //////////////////////////////////////////////////////////////////////////////////////////////
        //
        //  cB_Mono_CheckedChanged
        //
        //////////////////////////////////////////////////////////////////////////////////////////////
        private void cB_Mono_CheckedChanged(object sender, EventArgs e)
        {
            uEye.Defines.DisplayMode displayMode;
            m_Camera.Display.Mode.Get(out displayMode);

            if (cB_Mono.Checked == true)
            {
                displayMode |= uEye.Defines.DisplayMode.Mono;
            }
            else
            {
                displayMode &= ~uEye.Defines.DisplayMode.Mono;
            }

            m_Camera.Display.Mode.Set(displayMode);

            cB_Overlay_from_file.Checked = false;
            cB_Show_overlay.Checked = false;
            cB_Show_moving_overlay.Checked = false;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////
        //
        //  cB_Show_overlay_CheckedChanged
        //
        //////////////////////////////////////////////////////////////////////////////////////////////
        private void cB_Show_overlay_CheckedChanged(object sender, EventArgs e)
        {
            uEye.Defines.Status statusRet;
            statusRet = m_Camera.DirectRenderer.Overlay.Clear();

            if (cB_Show_overlay.Checked)
            {
                cB_Show_moving_overlay.Checked = false;
                cB_Overlay_from_file.Checked = false;

                // set overlay position
                m_Camera.DirectRenderer.Overlay.SetPosition(15, 60);

                // get graphics
                Graphics graph;
                m_Camera.DirectRenderer.Overlay.GetGraphics(out graph);

                Pen RedPen = new Pen(Color.Red);
                SolidBrush BlueBrush = new SolidBrush(Color.Blue);
                Pen YellowPen = new Pen(Color.Yellow);
                SolidBrush GreenBrush = new SolidBrush(Color.Green);

                // draw into graphics
                graph.FillEllipse(GreenBrush, 30, 30, 300, 300);
                graph.DrawEllipse(RedPen, new Rectangle(30, 31, 300, 300));
                graph.FillEllipse(BlueBrush, 400, 10, 200, 300);
                graph.DrawEllipse(YellowPen, new Rectangle(401, 11, 200, 300));

                // set graphics
                m_Camera.DirectRenderer.Overlay.SetGraphics(ref graph);

                // show overlay
                m_Camera.DirectRenderer.Overlay.Show();
            }
            else
            {
                m_Camera.DirectRenderer.Overlay.Clear();
                m_Camera.DirectRenderer.Overlay.Hide();
            }

            // force display update
            if (Button_Camera_Open_Freerun.Enabled)
            {
                m_Camera.DirectRenderer.Update();
            }

            UpdateOverlayInformation();
        }

        //////////////////////////////////////////////////////////////////////////////////////////////
        //
        //  cB_Scale_image_and_overlay_CheckedChanged
        //
        //////////////////////////////////////////////////////////////////////////////////////////////
        private void cB_Scale_image_and_overlay_CheckedChanged(object sender, EventArgs e)
        {
            if (cB_Scale_image_and_overlay.Checked == true)
            {
                cB_Scale_image.Checked = false;
                m_Camera.DirectRenderer.EnableScaling();
            }
            else
            {
                m_Camera.DirectRenderer.DisableScaling();
            }

            // force display update
            if (Button_Camera_Open_Freerun.Enabled)
            {
                m_Camera.DirectRenderer.Update();
            }
        }

        //////////////////////////////////////////////////////////////////////////////////////////////
        //
        //  cB_Show_moving_overlay_CheckedChanged
        //
        //////////////////////////////////////////////////////////////////////////////////////////////
        private void cB_Show_moving_overlay_CheckedChanged(object sender, EventArgs e)
        {
            m_Camera.DirectRenderer.Overlay.Clear();
            uEye.Defines.Status statusRet = uEye.Defines.Status.SUCCESS;

            if (cB_Show_moving_overlay.Checked == true)
            {
                cB_Show_overlay.Checked = false;
                cB_Overlay_from_file.Checked = false;

                int nhDC = 0;
                statusRet = m_Camera.DirectRenderer.Overlay.GetDC(out nhDC);

                if (statusRet == uEye.Defines.Status.SUCCESS && nhDC != 0)
                {
                    Graphics graph = Graphics.FromHdc((IntPtr)nhDC);
                    graph.DrawString("uEye", new Font("Courier", 48), System.Drawing.Brushes.Azure, 0, 0);
                    m_Camera.DirectRenderer.Overlay.ReleaseDC();

                    // show overlay
                    m_Camera.DirectRenderer.Overlay.Show();

                    // start overlay timer
                    m_OverlayMoveTimer.Start();
                }
                else
                {
                    cB_Show_moving_overlay.Checked = false;
                    m_Camera.DirectRenderer.Overlay.Clear();
                }
            }
            else
            {
                m_Camera.DirectRenderer.Overlay.Clear();
                m_OverlayMoveTimer.Stop();
            }

            UpdateOverlayInformation();
        }

        private void OnOverlayMove(object source, EventArgs e)
        {
            m_Camera.DirectRenderer.Overlay.SetPosition(m_u32OverlayPositionX, m_u32OverlayPositionY);
            m_Camera.DirectRenderer.Overlay.Show();

            // force display update
            if (Button_Camera_Open_Freerun.Enabled)
            {
                m_Camera.DirectRenderer.Update();
            }

            if (m_u32OverlayPositionX > this.m_u32ImageWidth)
            {
                m_s32MovePosX = -1;
            }

            if (m_u32OverlayPositionY > this.m_u32ImageHeight)
            {
                m_s32MovePosY = -1;
            }

            if (m_u32OverlayPositionX == 0)
            {
                m_s32MovePosX = 1;
            }

            if (m_u32OverlayPositionY == 0)
            {
                m_s32MovePosY = 1;
            }

            m_u32OverlayPositionX = (uint)((int)m_u32OverlayPositionX + m_s32MovePosX);
            m_u32OverlayPositionY = (uint)((int)m_u32OverlayPositionY + m_s32MovePosY);
        }

        //////////////////////////////////////////////////////////////////////////////////////////////
        //
        //  cB_Scale_image_CheckedChanged
        //
        //////////////////////////////////////////////////////////////////////////////////////////////
        private void cB_Scale_image_CheckedChanged(object sender, EventArgs e)
        {
            if (cB_Scale_image.Checked == true)
            {
                cB_Scale_image_and_overlay.Checked = false;

                m_Camera.DirectRenderer.EnableImageScaling();
            }
            else
            {
                m_Camera.DirectRenderer.DisableScaling();
            }

           cB_Show_moving_overlay.Checked = false;
           cB_Show_overlay.Checked = false;

           // force display update
           if (Button_Camera_Open_Freerun.Enabled)
           {
               m_Camera.DirectRenderer.Update();
           }
        }

        //////////////////////////////////////////////////////////////////////////////////////////////
        //
        //  cB_Overlay_from_file_CheckedChanged
        //
        //////////////////////////////////////////////////////////////////////////////////////////////
        private void cB_Overlay_from_file_CheckedChanged(object sender, EventArgs e)
        {
            string dataName = string.Empty;

            if (cB_Overlay_from_file.Checked == true)
            {
                cB_Show_moving_overlay.Checked = false;
                cB_Show_overlay.Checked = false;

                OpenFileDialog openFileDialog1 = new OpenFileDialog();
                openFileDialog1.Filter = "Bitmap file (*.bmp)|*.bmp|All files (*.*)|*.*";

                if (openFileDialog1.ShowDialog() == DialogResult.OK)
                {
                    dataName = openFileDialog1.FileName;

                    m_Camera.DirectRenderer.Overlay.SetKeyColor(m_OverlayColor);
                    m_Camera.DirectRenderer.Overlay.LoadImage(dataName);
                    m_Camera.DirectRenderer.Overlay.Show();
                }
                else
                {
                    cB_Overlay_from_file.Checked = false;
                }
            }
            else
            {
                m_Camera.DirectRenderer.Overlay.Hide();
            }

            // force display update
            if (Button_Camera_Open_Freerun.Enabled)
            {
                m_Camera.DirectRenderer.Update();
            }

            UpdateOverlayInformation();
        }

        //////////////////////////////////////////////////////////////////////////////////////////////
        //
        //  cB_Semi_transparent_CheckedChanged
        //
        //////////////////////////////////////////////////////////////////////////////////////////////
        private void cB_Semi_transparent_CheckedChanged(object sender, EventArgs e)
        {
            m_Camera.DirectRenderer.Overlay.SetSemiTransparent(cB_Semi_transparent.Checked);

            // force display update
            if (Button_Camera_Open_Freerun.Enabled)
            {
                m_Camera.DirectRenderer.Update();
            }
        }

        //////////////////////////////////////////////////////////////////////////////////////////////
        //
        //  rB_Vsync 
        //
        //////////////////////////////////////////////////////////////////////////////////////////////
        private void rB_Vsync_off_CheckedChanged(object sender, EventArgs e)
        {
            nuSyncUpDown.Enabled = false;
            cB_Show_overlay.Checked = false;
            cB_Show_moving_overlay.Checked = false;
            cB_Overlay_from_file.Checked = false;

            m_Camera.DirectRenderer.VsyncOff();
        }

        private void rB_Vsync_auto_CheckedChanged(object sender, EventArgs e)
        {
            nuSyncUpDown.Enabled = false;
            cB_Show_overlay.Checked = false;
            cB_Show_moving_overlay.Checked = false;
            cB_Overlay_from_file.Checked = false;

            m_Camera.DirectRenderer.VsyncAuto();
        }

        private void rB_Vsync_user_CheckedChanged(object sender, EventArgs e)
        {
            uint uWertUPDown = 0;

            nuSyncUpDown.Enabled = true;
            cB_Show_overlay.Checked = false;
            cB_Show_moving_overlay.Checked = false;
            cB_Overlay_from_file.Checked = false;

            uWertUPDown = (uint)nuSyncUpDown.Value;
            m_Camera.DirectRenderer.SetUserSync(uWertUPDown);
        }

        private void nuSyncUpDown_ValueChanged(object sender, EventArgs e)
        {
            m_Camera.DirectRenderer.SetUserSync((uint)nuSyncUpDown.Value);
        }

        //////////////////////////////////////////////////////////////////////////////////////////////
        //
        //  rB_OpenGL_CheckedChanged
        //
        //////////////////////////////////////////////////////////////////////////////////////////////
        private void rB_OpenGL_CheckedChanged(object sender, EventArgs e)
        {
            if (rB_OpenGL.Checked)
            {
                // not supported with opengl
                rB_Vsync_user.Enabled = false;
                cB_Overlay_from_file.Enabled = false;
                cB_Scale_image.Enabled = false;
                nuSyncUpDown.Enabled = false;

                uEye.Defines.DisplayMode displayMode = uEye.Defines.DisplayMode.OpenGL;
                cB_Semi_transparent.Enabled = true;
                
                if (cB_Mono.Checked)
                {
                    displayMode |= uEye.Defines.DisplayMode.Mono;
                }

                m_Camera.Display.Mode.Set(displayMode);

                // set key color
                m_Camera.DirectRenderer.Overlay.SetKeyColor(m_OverlayColor);

                // update overlay information
                UpdateOverlayInformation();
            }

            cB_Scale_image_and_overlay.Checked = false;
            cB_Scale_image.Checked = false;
            cB_Show_overlay.Checked = false;
            cB_Show_moving_overlay.Checked = false;
            cB_Overlay_from_file.Checked = false;
            cB_Semi_transparent.Checked = false;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////
        //
        //  rB_Direct3D_CheckedChanged
        //
        //////////////////////////////////////////////////////////////////////////////////////////////
        private void rB_Direct3D_CheckedChanged(object sender, EventArgs e)
        {
            if (rB_Direct3D.Checked)
            {
                rB_Vsync_user.Enabled = true;
                cB_Overlay_from_file.Enabled = true;
                cB_Scale_image.Enabled = true;
                cB_Semi_transparent.Enabled = false;
                nuSyncUpDown.Enabled = rB_Vsync_user.Checked;

                uEye.Defines.DisplayMode displayMode = uEye.Defines.DisplayMode.Direct3D;

                if (cB_Mono.Checked)
                {
                    displayMode |= uEye.Defines.DisplayMode.Mono;
                }

                m_Camera.Display.Mode.Set(displayMode);

                // set key color
                m_Camera.DirectRenderer.Overlay.SetKeyColor(m_OverlayColor);

                // update overlay information
                UpdateOverlayInformation();
            }

            cB_Scale_image_and_overlay.Checked = false;
            cB_Scale_image.Checked = false;
            cB_Show_overlay.Checked = false;
            cB_Show_moving_overlay.Checked = false;
            cB_Overlay_from_file.Checked = false;
            cB_Semi_transparent.Checked = false;
        }

        private void Button_Load_Param_Click(object sender, EventArgs e)
        {
            m_Camera.Parameter.Load("");

            tB_Image_Width.Text = m_u32ImageWidth.ToString();
            tB_Image_Height.Text = m_u32ImageHeight.ToString();

            UpdateOverlayInformation();
        }

        private void Button_Key_Color_Click_1(object sender, EventArgs e)
        {
            ColorDialog dialog = new ColorDialog();

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                m_OverlayColor = dialog.Color;
                Button_Key_Color.BackColor = m_OverlayColor;

                // set overlay key color
                m_Camera.DirectRenderer.Overlay.SetKeyColor(m_OverlayColor);

                // force display update
                if (Button_Camera_Open_Freerun.Enabled)
                {
                    m_Camera.DirectRenderer.Update();
                }
            }
        }
    }
}

