using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace SimpleLive
{
    public partial class uEye_DotNet_Simple_Live : Form
    {
        private uEye.Camera m_Camera;
        IntPtr m_displayHandle = IntPtr.Zero;
        private bool m_bLive = false;
        private const int m_cnNumberOfSeqBuffers = 3;

        public uEye_DotNet_Simple_Live()
        {
            InitializeComponent();

            m_displayHandle = DisplayWindow.Handle;
            InitCamera();
        }

        private uEye.Defines.Status AllocImageMems()
        {
            uEye.Defines.Status statusRet = uEye.Defines.Status.SUCCESS;

            for (int i = 0; i < m_cnNumberOfSeqBuffers; i++)
            {
                statusRet = m_Camera.Memory.Allocate();

                if (statusRet != uEye.Defines.Status.SUCCESS)
                {
                    FreeImageMems();
                }
            }

            return statusRet;
        }

        private uEye.Defines.Status FreeImageMems()
        {
            int[] idList;
            uEye.Defines.Status statusRet = m_Camera.Memory.GetList(out idList);

            if (uEye.Defines.Status.SUCCESS == statusRet)
            {
                foreach (int nMemID in idList)
                {
                    do
                    {
                        statusRet = m_Camera.Memory.Free(nMemID);

                        if (uEye.Defines.Status.SEQ_BUFFER_IS_LOCKED == statusRet)
                        {
                            Thread.Sleep(1);
                            continue;
                        }

                        break;
                    }
                    while (true);
                }
            }

            return statusRet;
        }

        private uEye.Defines.Status InitSequence()
        {
            int[] idList;
            uEye.Defines.Status statusRet = m_Camera.Memory.GetList(out idList);

            if (uEye.Defines.Status.SUCCESS == statusRet)
            {
                statusRet = m_Camera.Memory.Sequence.Add(idList);

                if (uEye.Defines.Status.SUCCESS != statusRet)
                {
                    ClearSequence();
                }
            }

            return statusRet;
        }

        private uEye.Defines.Status ClearSequence()
        {
            return m_Camera.Memory.Sequence.Clear();
        }

        private void InitCamera()
        {
            m_Camera = new uEye.Camera();

            uEye.Defines.Status statusRet = 0;

            // Open Camera
            statusRet = m_Camera.Init(0);
            if (statusRet != uEye.Defines.Status.Success)
            {
                MessageBox.Show("Camera initializing failed");
                Environment.Exit(-1);
            }

            // Allocate Memory
            statusRet = AllocImageMems();
            if (statusRet != uEye.Defines.Status.Success)
            {
                MessageBox.Show("Allocate Memory failed");
                Environment.Exit(-1);
            }

            statusRet = InitSequence();
            if (statusRet != uEye.Defines.Status.Success)
            {
                MessageBox.Show("Add to sequence failed");
                Environment.Exit(-1);
            }

            // Start Live Video
            statusRet = m_Camera.Acquisition.Capture();
            if (statusRet != uEye.Defines.Status.Success)
            {
                MessageBox.Show("Start Live Video failed");
            }
            else
            {
                m_bLive = true;
            }

            // Connect Event
            m_Camera.EventFrame += onFrameEvent;
            m_Camera.EventAutoBrightnessFinished += onAutoShutterFinished;

            CB_Auto_Gain_Balance.Enabled = m_Camera.AutoFeatures.Software.Gain.Supported;
            CB_Auto_White_Balance.Enabled = m_Camera.AutoFeatures.Software.WhiteBalance.Supported;
        }

        private void onFrameEvent(object sender, EventArgs e)
        {
            uEye.Camera Camera = sender as uEye.Camera;

            Int32 s32MemID;
            uEye.Defines.Status statusRet = Camera.Memory.GetLast(out s32MemID);

            if ((uEye.Defines.Status.SUCCESS == statusRet) && (0 < s32MemID))
            {
                if (uEye.Defines.Status.SUCCESS == Camera.Memory.Lock(s32MemID))
                {
                    Camera.Display.Render(s32MemID, m_displayHandle, uEye.Defines.DisplayRenderMode.FitToWindow);
                    Camera.Memory.Unlock(s32MemID);
                }
            }
        }

        private void onAutoShutterFinished(object sender, EventArgs e)
        {
            MessageBox.Show("AutoShutter finished...");
        }

        private void Button_Live_Video_Click(object sender, EventArgs e)
        {
            // Open Camera and Start Live Video
            if (m_Camera.Acquisition.Capture() == uEye.Defines.Status.Success)
            {
                m_bLive = true;
            }
        }

        private void Button_Stop_Video_Click(object sender, EventArgs e)
        {
            // Stop Live Video
            if (m_Camera.Acquisition.Stop() == uEye.Defines.Status.Success)
            {
                m_bLive = false;
            }
        }
        
        private void Button_Freeze_Video_Click(object sender, EventArgs e)
        {
            if (m_Camera.Acquisition.Freeze() == uEye.Defines.Status.Success)
            {
                m_bLive = false;
            }
        }

        private void CB_Auto_White_Balance_CheckedChanged(object sender, EventArgs e)
        {
            m_Camera.AutoFeatures.Software.WhiteBalance.SetEnable(CB_Auto_White_Balance.Checked);
        }

        private void CB_Auto_Gain_Balance_CheckedChanged(object sender, EventArgs e)
        {
            uEye.Defines.Status status;
            if (CB_Auto_Gain_Balance.Checked)
            {
                status = m_Camera.AutoFeatures.Software.Gain.SetEnable(true);
            }
            else
            {
                status = m_Camera.AutoFeatures.Software.Gain.SetEnable(false);
            }
        }

        private void Button_Load_Parameter_Click(object sender, EventArgs e)
        {
            uEye.Defines.Status statusRet = 0;

            m_Camera.Acquisition.Stop();

            ClearSequence();
            FreeImageMems();

            statusRet = m_Camera.Parameter.Load("");
            if (statusRet != uEye.Defines.Status.Success)
            {
                MessageBox.Show("Loading parameter failed: " + statusRet);
            }

            // Allocate Memory
            statusRet = AllocImageMems();
            if (statusRet != uEye.Defines.Status.Success)
            {
                MessageBox.Show("Allocate Memory failed");
                Environment.Exit(-1);
            }

            statusRet = InitSequence();
            if (statusRet != uEye.Defines.Status.Success)
            {
                MessageBox.Show("Add to sequence failed");
                Environment.Exit(-1);
            }

            if (m_bLive == true)
            {
                m_Camera.Acquisition.Capture();
            }
        }

        private void Button_Exit_Prog_Click(object sender, EventArgs e)
        {
            m_Camera.Acquisition.Stop();

            ClearSequence();
            FreeImageMems();

            // Close the Camera
            m_Camera.Exit();
            Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            uEye.Defines.Status status;
            status = m_Camera.AutoFeatures.Software.Shutter.SetEnable(uEye.Defines.ActivateMode.Once);
            status = m_Camera.AutoFeatures.Software.Shutter.SetEnable(uEye.Defines.ActivateMode.Enable);
        }
    }
}
