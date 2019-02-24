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
    public partial class CameraControl : IControl
    {
        private Timer m_UpdateTimer = new Timer();

        public CameraControl(uEye.Camera camera)
            : base(camera)
        {
            InitializeComponent();

            m_UpdateTimer.Interval = 100;
            m_UpdateTimer.Tick += OnUpdateControls;
        }

        public override void OnControlFocusActive()
        {
            UpdatePixelclock();
            UpdateFramerate();
            UpdateExposure();
            UpdateBandwidth();

            // check supported
            checkBoxFineIncrement.Enabled = m_Camera.Timing.Exposure.Fine.Supported;

            // check enabled
            Boolean isEnabled;
            m_Camera.AutoFeatures.Software.Shutter.GetEnable(out isEnabled);
            checkBoxExposureAuto.Checked = isEnabled;

            checkBoxFramerateAuto.Enabled = checkBoxExposureAuto.Checked;
            if (checkBoxExposureAuto.Checked)
            {
                m_Camera.AutoFeatures.Software.Framerate.GetEnable(out isEnabled);
                checkBoxFramerateAuto.Checked = isEnabled;
            }

            m_UpdateTimer.Start();
        }

        public override void OnControlFocusLost()
        {
            m_UpdateTimer.Stop();
        }

        private void OnUpdateControls(object sender, EventArgs e)
        {
            if (checkBoxFramerateAuto.Checked)
            {
                uEye.Defines.Status statusRet;
                uEye.Types.Range<Double> range;

                statusRet = m_Camera.Timing.Framerate.GetFrameRateRange(out range);

                Double dValue;
                statusRet = m_Camera.Timing.Framerate.Get(out dValue);

                numericUpDownFramerate.Value = Convert.ToDecimal(dValue);
                Int32 s32Value = Convert.ToInt32((dValue - range.Minimum) / range.Increment);

                trackBarFramerate.Value = s32Value;

                UpdateExposure();
            }

            if (checkBoxExposureAuto.Checked)
            {
                uEye.Defines.Status statusRet;

                uEye.Types.Range<Double> range;
                m_Camera.Timing.Exposure.GetRange(out range);

                Double dValue;
                statusRet = m_Camera.Timing.Exposure.Get(out dValue);
                numericUpDownExposure.Value = Convert.ToDecimal(dValue);

                Int32 s32Value = Convert.ToInt32((dValue - range.Minimum) / range.Increment + 0.0005);
                trackBarExposure.Value = s32Value;
            }

            if (tabControlTiming.SelectedIndex == 1)
            {
                UpdateBandwidth();
            }
        }

        private void tabControlTiming_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (tabControlTiming.SelectedIndex)
            {
                case 0:
                    UpdatePixelclock();
                    UpdateFramerate();
                    UpdateExposure();
                    break;
                case 1:
                    UpdateBandwidth();
                    break;
            }
        }

        private void UpdateBandwidth()
        {
            Double f64PeakBandwidth;
            m_Camera.Information.GetPeakBandwidth(out f64PeakBandwidth);
            labelCameraPeak.Text = Math.Round(f64PeakBandwidth, 2) + " MB/s";

            Double f64AvgBandwidth;
            m_Camera.Information.GetAvgBandwidth(out f64AvgBandwidth);
            labelCameraAvg.Text = Math.Round(f64AvgBandwidth, 2) + " MB/s";

            Int32 s32SensorBandwidth;
            m_Camera.Timing.PixelClock.Get(out s32SensorBandwidth);
            labelSensorBandwidth.Text = s32SensorBandwidth + " Mp/s";
        }

        private void UpdatePixelclock()
        {
            uEye.Defines.Status statusRet;

            uEye.Types.Range<Int32> range;
            statusRet = m_Camera.Timing.PixelClock.GetRange(out range);

            labelPixelclockMin.Text = range.Minimum.ToString() + " MHz";
            labelPixelclockMax.Text = range.Maximum.ToString() + " MHz";

            trackBarPixelclock.SetRange(range.Minimum, range.Maximum);

            numericUpDownPixelclock.Minimum = range.Minimum;
            numericUpDownPixelclock.Maximum = range.Maximum;

            Int32 s32Value;
            statusRet = m_Camera.Timing.PixelClock.Get(out s32Value);

            trackBarPixelclock.Value = s32Value;
            numericUpDownPixelclock.Value = s32Value;
        }

        private void UpdateFramerate()
        {
            uEye.Defines.Status statusRet;
            uEye.Types.Range<Double> range;

            statusRet = m_Camera.Timing.Framerate.GetFrameRateRange(out range);

            labelFramerateMin.Text = Math.Round(range.Minimum, 2) + " fps";
            labelFramerateMax.Text = Math.Round(range.Maximum, 2) + " fps";

            Int32 s32Steps = Convert.ToInt32((range.Maximum - range.Minimum) / range.Increment);
            trackBarFramerate.SetRange(0, s32Steps);

            numericUpDownFramerate.Minimum = Convert.ToDecimal(range.Minimum);
            numericUpDownFramerate.Maximum = Convert.ToDecimal(range.Maximum);

            Double dValue;
            statusRet = m_Camera.Timing.Framerate.Get(out dValue);

            numericUpDownFramerate.Value = Convert.ToDecimal(dValue);
            Int32 s32Value = Convert.ToInt32((dValue - range.Minimum) / range.Increment);

            trackBarFramerate.Value = s32Value;

            if (checkBoxFramerateMax.Checked)
            {
                m_Camera.Timing.Framerate.Set(Convert.ToDouble(numericUpDownFramerate.Maximum));

                numericUpDownFramerate.Value = numericUpDownFramerate.Maximum;
                trackBarFramerate.Value = trackBarFramerate.Maximum;
            }
        }

        private void UpdateExposure()
        {
            uEye.Defines.Status statusRet;
            uEye.Types.Range<Double> range;

            if (!checkBoxFineIncrement.Checked)
            {
                statusRet = m_Camera.Timing.Exposure.GetRange(out range);
            }
            else
            {
                statusRet = m_Camera.Timing.Exposure.Fine.GetRange(out range);
            }

            labelExposureMin.Text = Math.Round(range.Minimum, 2) + " ms";
            labelExposureMax.Text = Math.Round(range.Maximum, 2) + " ms";

            numericUpDownExposure.Minimum = Convert.ToDecimal(range.Minimum);
            numericUpDownExposure.Maximum = Convert.ToDecimal(range.Maximum);

            // trackbar range
            Int32 s32Step = Convert.ToInt32((range.Maximum - range.Minimum) / range.Increment + 0.0005);
            trackBarExposure.SetRange(0, s32Step);

            Double dValue;
            statusRet = m_Camera.Timing.Exposure.Get(out dValue);
            numericUpDownExposure.Value = Convert.ToDecimal(dValue) > numericUpDownExposure.Maximum ? numericUpDownExposure.Maximum : Convert.ToDecimal(dValue);

            Int32 s32Value = Convert.ToInt32((dValue - range.Minimum) / range.Increment + 0.0005);
            trackBarExposure.Value = s32Value > trackBarExposure.Maximum ? trackBarExposure.Maximum : s32Value;

            if (checkBoxExposureMax.Checked)
            {
                m_Camera.Timing.Exposure.Set(Convert.ToDouble(numericUpDownExposure.Maximum));

                numericUpDownExposure.Value = numericUpDownExposure.Maximum;
                trackBarExposure.Value = trackBarExposure.Maximum;
            }
        }

        private void trackBarPixelclock_Scroll(object sender, EventArgs e)
        {
            if (trackBarPixelclock.Focused)
            {
                m_Camera.Timing.PixelClock.Set(trackBarPixelclock.Value);
                numericUpDownPixelclock.Value = trackBarPixelclock.Value;

                UpdateFramerate();
                UpdateExposure();
            }
        }

        private void trackBarFramerate_Scroll(object sender, EventArgs e)
        {
            if (trackBarFramerate.Focused)
            {
                uEye.Defines.Status statusRet;
                uEye.Types.Range<Double> range;

                statusRet = m_Camera.Timing.Framerate.GetFrameRateRange(out range);
                Double dValue = range.Minimum + trackBarFramerate.Value * range.Increment;

                if(dValue > range.Maximum)
                {
                    dValue = range.Maximum;
                }

                // update numeric
                numericUpDownFramerate.Value = Convert.ToDecimal(dValue);

                // set framerate
                statusRet = m_Camera.Timing.Framerate.Set(dValue);

                // update exposure
                UpdateExposure();
            }
        }

        private void trackBarExposure_Scroll(object sender, EventArgs e)
        {
            if (trackBarExposure.Focused)
            {
                uEye.Defines.Status statusRet;
                uEye.Types.Range<Double> range;

                statusRet = m_Camera.Timing.Exposure.GetRange(out range);

                // calculate exposure
                Double dValue = range.Minimum + trackBarExposure.Value * range.Increment;

                // update numeric
                numericUpDownExposure.Value = Convert.ToDecimal(dValue > range.Maximum ? 
                    Convert.ToDecimal(range.Maximum) : Convert.ToDecimal(dValue));

                // set exposure
                statusRet = m_Camera.Timing.Exposure.Set(dValue);
            }
        }

        private void checkBoxFramerateAuto_CheckedChanged(object sender, EventArgs e)
        {
            checkBoxFramerateMax.Enabled = !checkBoxFramerateAuto.Checked;
            trackBarFramerate.Enabled = !checkBoxFramerateAuto.Checked;
            numericUpDownFramerate.Enabled = !checkBoxFramerateAuto.Checked;

            m_Camera.AutoFeatures.Software.Framerate.SetEnable(checkBoxFramerateAuto.Checked);
        }

        private void checkBoxFramerateMax_CheckedChanged(object sender, EventArgs e)
        {
            trackBarFramerate.Enabled = !checkBoxFramerateMax.Checked;
            numericUpDownFramerate.Enabled = !checkBoxFramerateMax.Checked;

            UpdateFramerate();
        }

        private void checkBoxExposureAuto_CheckedChanged(object sender, EventArgs e)
        {
            checkBoxFramerateAuto.Enabled = checkBoxExposureAuto.Checked;
            checkBoxFramerateAuto.Checked = false;

            trackBarExposure.Enabled = !checkBoxExposureAuto.Checked;
            numericUpDownExposure.Enabled = !checkBoxExposureAuto.Checked;
            checkBoxExposureMax.Enabled = !checkBoxExposureAuto.Checked;
            trackBarPixelclock.Enabled = !checkBoxExposureAuto.Checked;
            numericUpDownPixelclock.Enabled = !checkBoxExposureAuto.Checked;

            m_Camera.AutoFeatures.Software.Shutter.SetEnable(checkBoxExposureAuto.Checked);
        }

        private void checkBoxExposureMax_CheckedChanged(object sender, EventArgs e)
        {
            trackBarExposure.Enabled = !checkBoxExposureMax.Checked;
            numericUpDownExposure.Enabled = !checkBoxExposureMax.Checked;

            checkBoxExposureAuto.Enabled = !checkBoxExposureMax.Checked;

            UpdateExposure();
        }

        private void checkBoxFineIncrement_CheckedChanged(object sender, EventArgs e)
        {
            UpdateExposure();
        }

        private void numericUpDownPixelclock_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownPixelclock.Focused)
            {
                trackBarPixelclock.Value = Convert.ToInt32(numericUpDownPixelclock.Value);
                m_Camera.Timing.PixelClock.Set(trackBarPixelclock.Value);

                UpdateFramerate();
                UpdateExposure();
            }
        }

        private void numericUpDownFramerate_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownFramerate.Focused)
            {
                uEye.Defines.Status statusRet;
                uEye.Types.Range<Double> range;

                statusRet = m_Camera.Timing.Framerate.GetFrameRateRange(out range);
                Double dValue = Convert.ToDouble(numericUpDownFramerate.Value);

                // update numeric
                trackBarFramerate.Value = Convert.ToInt32((dValue - range.Minimum) / range.Increment);

                // set framerate
                statusRet = m_Camera.Timing.Framerate.Set(dValue);

                // update exposure
                UpdateExposure();
            }
        }

        private void numericUpDownExposure_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownExposure.Focused)
            {
                uEye.Defines.Status statusRet;
                uEye.Types.Range<Double> range;

                statusRet = m_Camera.Timing.Exposure.GetRange(out range);

                // calculate exposure
                Double dValue = Convert.ToDouble(numericUpDownExposure.Value);
                dValue = (dValue - range.Minimum) / range.Increment;
                // update numeric
                trackBarExposure.Value = Convert.ToInt32(dValue > trackBarExposure.Maximum ? trackBarExposure.Maximum : dValue);

                // set exposure
                statusRet = m_Camera.Timing.Exposure.Set(dValue);
            }
        }
    }
}
