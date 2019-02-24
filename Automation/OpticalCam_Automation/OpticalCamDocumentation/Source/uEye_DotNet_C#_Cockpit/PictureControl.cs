using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace DotnetCockpit
{
    public partial class PictureControl : IControl
    {
        private Timer m_UpdateTimer = new Timer();

        public PictureControl(uEye.Camera camera)
            : base(camera)
        {
            InitializeComponent();

            m_UpdateTimer.Interval = 100;
            m_UpdateTimer.Tick += OnUpdateControls;
        }

        public override void OnControlFocusActive()
        {
            UpdateGainControls();
            UpdateGammaControls();

            // check enabled controls
            uEye.Defines.ActivateMode activeMode;
            m_Camera.AutoFeatures.Software.WhiteBalance.GetEnable(out activeMode);

            if (activeMode != uEye.Defines.ActivateMode.Disable)
            {
                radioButtonWhitebalanceOnce.Checked = (activeMode & uEye.Defines.ActivateMode.Enable) 
                    == uEye.Defines.ActivateMode.Enable;
                radioButtonWhitebalanceAuto.Checked = (activeMode & uEye.Defines.ActivateMode.Once) 
                    == uEye.Defines.ActivateMode.Once;
            }
            else
            {
                radioButtonWhitebalanceDisabled.Checked = true;
            }

            m_UpdateTimer.Start();
        }

        public override void OnControlFocusLost()
        {
            m_UpdateTimer.Stop();
        }

        private void OnUpdateControls(object sender, EventArgs e)
        {
            if (checkBoxMasterGainAuto.Checked)
            {
                Int32 s32Value;
                m_Camera.Gain.Hardware.Scaled.GetMaster(out s32Value);

                trackBarGainMaster.Value = s32Value;
                numericUpDownGainMaster.Value = s32Value;
            }

            if (radioButtonWhitebalanceAuto.Checked || radioButtonWhitebalanceOnce.Checked)
            {
                Int32 s32Value;

                m_Camera.Gain.Hardware.Scaled.GetRed(out s32Value);
                trackBarGainRed.Value = s32Value;
                numericUpDownGainRed.Value = s32Value;

                m_Camera.Gain.Hardware.Scaled.GetGreen(out s32Value);
                trackBarGainGreen.Value = s32Value;
                numericUpDownGainGreen.Value = s32Value;

                m_Camera.Gain.Hardware.Scaled.GetBlue(out s32Value);
                trackBarGainBlue.Value = s32Value;
                numericUpDownGainBlue.Value = s32Value;
            }
        }

        private void UpdateGainControls()
        {
            Boolean isRedSupported, isBlueSupported, isGreenSupported, isMasterSupported;
            m_Camera.Gain.Hardware.GetSupported(out isMasterSupported, out isRedSupported,
                                                out isGreenSupported, out isBlueSupported);

            trackBarGainBlue.Enabled = numericUpDownGainBlue.Enabled = isBlueSupported;
            trackBarGainGreen.Enabled = numericUpDownGainGreen.Enabled = isGreenSupported;
            trackBarGainMaster.Enabled = numericUpDownGainMaster.Enabled = isMasterSupported;
            trackBarGainRed.Enabled = numericUpDownGainRed.Enabled = isRedSupported;

            checkBoxMasterGainBoost.Enabled = m_Camera.Gain.Hardware.Boost.Supported;

            Int32 s32Gain;
            uEye.Defines.Status statusRet;

            // master gain
            labelGainMasterMin.Text = "0";
            labelGainMasterMax.Text = "100";

            numericUpDownGainMaster.Minimum = 0;
            numericUpDownGainMaster.Maximum = 100;

            trackBarGainMaster.SetRange(0, 100);

            statusRet = m_Camera.Gain.Hardware.Scaled.GetMaster(out s32Gain);
            numericUpDownGainMaster.Value = s32Gain;
            trackBarGainMaster.Value = s32Gain;

            // red gain
            labelGainRedMin.Text = "0";
            labelGainRedMax.Text = "100";

            numericUpDownGainRed.Minimum = 0;
            numericUpDownGainRed.Maximum = 100;

            trackBarGainRed.SetRange(0, 100);

            statusRet = m_Camera.Gain.Hardware.Scaled.GetRed(out s32Gain);
            numericUpDownGainRed.Value = s32Gain;
            trackBarGainRed.Value = s32Gain;

            // green gain
            labelGainGreenMin.Text = "0";
            labelGainGreenMax.Text = "100";

            numericUpDownGainGreen.Minimum = 0;
            numericUpDownGainGreen.Maximum = 100;

            trackBarGainGreen.SetRange(0, 100);

            statusRet = m_Camera.Gain.Hardware.Scaled.GetGreen(out s32Gain);
            numericUpDownGainGreen.Value = s32Gain;
            trackBarGainGreen.Value = s32Gain;

            // blue gain
            labelGainBlueMin.Text = "0";
            labelGainBlueMax.Text = "100";

            numericUpDownGainBlue.Minimum = 0;
            numericUpDownGainBlue.Maximum = 100;

            trackBarGainBlue.SetRange(0, 100);

            statusRet = m_Camera.Gain.Hardware.Scaled.GetBlue(out s32Gain);
            numericUpDownGainBlue.Value = s32Gain;
            trackBarGainBlue.Value = s32Gain;
            
        }

        private void UpdateGammaControls()
        {
            checkBoxHardwareGamma.Enabled = m_Camera.Gamma.Hardware.Supported;
            

            trackBarGamma.SetRange(100, 220);
            numericUpDownGamma.Minimum = new Decimal(1.0);
            numericUpDownGamma.Maximum = new Decimal(2.2);
            numericUpDownGamma.Increment = new Decimal(0.1);

            labelGammaMin.Text = "1.0";
            labelGammaMax.Text = "2.2";
        }

        private void trackBarGainMaster_Scroll(object sender, EventArgs e)
        {
            if (trackBarGainMaster.Focused)
            {
                Int32 s32Value = trackBarGainMaster.Value;

                m_Camera.Gain.Hardware.Scaled.SetMaster(s32Value);
                numericUpDownGainMaster.Value = s32Value;
            }
        }

        private void trackBarGainRed_Scroll(object sender, EventArgs e)
        {
            if (trackBarGainRed.Focused)
            {
                Int32 s32Value = trackBarGainRed.Value;

                m_Camera.Gain.Hardware.Scaled.SetRed(s32Value);
                numericUpDownGainRed.Value = s32Value;
            }
        }

        private void trackBarGainGreen_Scroll(object sender, EventArgs e)
        {
            if (trackBarGainGreen.Focused)
            {
                Int32 s32Value = trackBarGainGreen.Value;

                m_Camera.Gain.Hardware.Scaled.SetGreen(s32Value);
                numericUpDownGainGreen.Value = s32Value;
            }
        }

        private void trackBarGainBlue_Scroll(object sender, EventArgs e)
        {
            if (trackBarGainBlue.Focused)
            {
                Int32 s32Value = trackBarGainBlue.Value;

                m_Camera.Gain.Hardware.Scaled.SetBlue(s32Value);
                numericUpDownGainBlue.Value = s32Value;
            }
        }

        private void numericUpDownGainRed_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownGainRed.Focused)
            {
                Int32 s32Value = Convert.ToInt32(numericUpDownGainRed.Value);
                m_Camera.Gain.Hardware.Scaled.SetRed(s32Value);
                trackBarGainRed.Value = s32Value;
            }
        }

        private void numericUpDownGainGreen_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownGainGreen.Focused)
            {
                Int32 s32Value = Convert.ToInt32(numericUpDownGainGreen.Value);
                m_Camera.Gain.Hardware.Scaled.SetGreen(s32Value);
                trackBarGainGreen.Value = s32Value;
            }
        }

        private void numericUpDownGainBlue_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownGainBlue.Focused)
            {
                Int32 s32Value = Convert.ToInt32(numericUpDownGainBlue.Value);
                m_Camera.Gain.Hardware.Scaled.SetBlue(s32Value);
                trackBarGainBlue.Value = s32Value;
            }
        }

        private void numericUpDownGainMaster_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownGainMaster.Focused)
            {
                Int32 s32Value = Convert.ToInt32(numericUpDownGainMaster.Value);
                m_Camera.Gain.Hardware.Scaled.SetMaster(s32Value);
                trackBarGainMaster.Value = s32Value;
            }
        }

        private void checkBoxGammaEnable_CheckedChanged(object sender, EventArgs e)
        {

            if (checkBoxGammaEnable.Checked)
            {
                trackBarGamma.Enabled = true;
                numericUpDownGamma.Enabled = true;
            }
            else
            {
                trackBarGamma.Enabled = false;
                numericUpDownGamma.Enabled = false;

                numericUpDownGamma.Value = new Decimal(1.0);
                trackBarGamma.Value = 100;
                m_Camera.Gamma.Software.Set(100);
            }
        }

        private void trackBarGamma_Scroll(object sender, EventArgs e)
        {
            if (trackBarGamma.Focused)
            {
                Int32 s32Value = trackBarGamma.Value;
                numericUpDownGamma.Value = Convert.ToDecimal(s32Value / 100.0);

                m_Camera.Gamma.Software.Set(s32Value);
            }
        }

        private void numericUpDownGamma_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownGamma.Focused)
            {
                Int32 s32Value = Convert.ToInt32(numericUpDownGamma.Value * 100);
                trackBarGamma.Value = s32Value;

                m_Camera.Gamma.Software.Set(s32Value);
            }
        }

        private void checkBoxHardwareGamma_CheckedChanged(object sender, EventArgs e)
        {
            m_Camera.Gamma.Hardware.SetEnable(checkBoxGammaEnable.Checked);
        }

        private void checkBoxMasterGainAuto_CheckedChanged(object sender, EventArgs e)
        {
            trackBarGainMaster.Enabled = !checkBoxMasterGainAuto.Checked;
            numericUpDownGainMaster.Enabled = !checkBoxMasterGainAuto.Checked;

            m_Camera.AutoFeatures.Software.Gain.SetEnable(checkBoxMasterGainAuto.Checked);
        }

        private void checkBoxMasterGainBoost_CheckedChanged(object sender, EventArgs e)
        {
            m_Camera.Gain.Hardware.Boost.SetEnable(checkBoxMasterGainBoost.Checked);
        }

        private void radioButtonWhitebalanceDisabled_CheckedChanged(object sender, EventArgs e)
        {
            m_Camera.AutoFeatures.Software.WhiteBalance.SetEnable(false);

            trackBarGainBlue.Enabled = true;
            numericUpDownGainBlue.Enabled = true;

            trackBarGainGreen.Enabled = true;
            numericUpDownGainGreen.Enabled = true;

            trackBarGainRed.Enabled = true;
            numericUpDownGainRed.Enabled = true;
        }

        private void radioButtonWhitebalanceAuto_CheckedChanged(object sender, EventArgs e)
        {
            m_Camera.AutoFeatures.Software.WhiteBalance.SetEnable(true);

            trackBarGainBlue.Enabled = false;
            numericUpDownGainBlue.Enabled = false;

            trackBarGainGreen.Enabled = false;
            numericUpDownGainGreen.Enabled = false;

            trackBarGainRed.Enabled = false;
            numericUpDownGainRed.Enabled = false;
        }

        private void radioButtonWhitebalanceOnce_CheckedChanged(object sender, EventArgs e)
        {
            m_Camera.AutoFeatures.Software.WhiteBalance.SetEnable(uEye.Defines.ActivateMode.Once);

            trackBarGainBlue.Enabled = false;
            numericUpDownGainBlue.Enabled = false;

            trackBarGainGreen.Enabled = false;
            numericUpDownGainGreen.Enabled = false;

            trackBarGainRed.Enabled = false;
            numericUpDownGainRed.Enabled = false;
        }
    }
}
