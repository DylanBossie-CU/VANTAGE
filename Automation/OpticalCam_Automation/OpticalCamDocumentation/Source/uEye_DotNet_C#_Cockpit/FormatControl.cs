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
    public partial class FormatControl : IControl
    {
        public event EventHandler<EventArgs> DisplayChanged;

        public FormatControl(uEye.Camera camera) 
            : base(camera)
        {
            InitializeComponent();
        }

        public FormatControl()
        {
            InitializeComponent();
        }

        public override void OnControlFocusActive()
        {
            InitPixelformat();
            UpdateDisplay();
        }

        public override void OnControlFocusLost()
        {
        }

        private void tabControlFormat_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (tabControlFormat.SelectedIndex)
            {
                case 0:
                    InitPixelformat();
                    break;
                case 1:
                    UpdateDisplay();
                    break;
            }
        }

        private void InitPixelformat()
        {
            uEye.Defines.Status statusRet;
            uEye.Types.SensorInfo sensorInfo;

            uEye.Defines.DisplayMode displayMode;
            m_Camera.Display.Mode.Get(out displayMode);

            foreach (Control control in this.tabPagePixelformat.Controls)
            {
                control.Enabled = displayMode == uEye.Defines.DisplayMode.DiB;
            }

            if (displayMode != uEye.Defines.DisplayMode.DiB)
            {
                MessageBox.Show("Please change the display mode to DiB to change the pixel format");
                return;
            }

            statusRet = m_Camera.Information.GetSensorInfo(out sensorInfo);

            if (sensorInfo.SensorColorMode == uEye.Defines.SensorColorMode.Monochrome)
            {
                radioButtonGray8Normal.Enabled = true;
                radioButtonGray8High.Enabled = false;

                radioButtonGray12Normal.Enabled = true;
                radioButtonGray12High.Enabled = false;

                radioButtonGray16Normal.Enabled = true;
                radioButtonGray16High.Enabled = false;
            }
            else if (sensorInfo.SensorColorMode == uEye.Defines.SensorColorMode.Bayer)
            {
                radioButtonGray8Normal.Enabled = isColorModeSupported(uEye.Defines.ColorMode.Mono8, uEye.Defines.ColorConvertMode.Software3X3);
                radioButtonGray8High.Enabled = isColorModeSupported(uEye.Defines.ColorMode.Mono8, uEye.Defines.ColorConvertMode.Software5X5);
                radioButtonGray8Hardware.Enabled = isColorModeSupported(uEye.Defines.ColorMode.Mono8, uEye.Defines.ColorConvertMode.Hardware3X3);

                radioButtonGray12Normal.Enabled = isColorModeSupported(uEye.Defines.ColorMode.Mono12, uEye.Defines.ColorConvertMode.Software3X3);
                radioButtonGray12High.Enabled = isColorModeSupported(uEye.Defines.ColorMode.Mono12, uEye.Defines.ColorConvertMode.Software5X5);
                radioButtonGray12Hardware.Enabled = isColorModeSupported(uEye.Defines.ColorMode.Mono12, uEye.Defines.ColorConvertMode.Hardware3X3);

                radioButtonGray16Normal.Enabled = isColorModeSupported(uEye.Defines.ColorMode.Mono16, uEye.Defines.ColorConvertMode.Software3X3);
                radioButtonGray16High.Enabled = isColorModeSupported(uEye.Defines.ColorMode.Mono16, uEye.Defines.ColorConvertMode.Software5X5);
                radioButtonGray16Hardware.Enabled = isColorModeSupported(uEye.Defines.ColorMode.Mono16, uEye.Defines.ColorConvertMode.Hardware3X3);
            }

            radioButtonRaw8Normal.Enabled = isColorModeSupported(uEye.Defines.ColorMode.SensorRaw8, uEye.Defines.ColorConvertMode.Software3X3);
            radioButtonRaw8High.Enabled = isColorModeSupported(uEye.Defines.ColorMode.SensorRaw8, uEye.Defines.ColorConvertMode.Software5X5);
            radioButtonGray8Hardware.Enabled = isColorModeSupported(uEye.Defines.ColorMode.SensorRaw8, uEye.Defines.ColorConvertMode.Hardware3X3);

            radioButtonRaw12Normal.Enabled = isColorModeSupported(uEye.Defines.ColorMode.SensorRaw12, uEye.Defines.ColorConvertMode.Software3X3);
            radioButtonRaw12High.Enabled = isColorModeSupported(uEye.Defines.ColorMode.SensorRaw12, uEye.Defines.ColorConvertMode.Software5X5);
            radioButtonRaw12Hardware.Enabled = isColorModeSupported(uEye.Defines.ColorMode.SensorRaw12, uEye.Defines.ColorConvertMode.Hardware3X3);

            radioButtonRaw16Normal.Enabled = isColorModeSupported(uEye.Defines.ColorMode.SensorRaw16, uEye.Defines.ColorConvertMode.Software3X3);
            radioButtonRaw16High.Enabled = isColorModeSupported(uEye.Defines.ColorMode.SensorRaw16, uEye.Defines.ColorConvertMode.Software5X5);
            radioButtonRaw16Hardware.Enabled = isColorModeSupported(uEye.Defines.ColorMode.SensorRaw16, uEye.Defines.ColorConvertMode.Hardware3X3);

            radioButtonRGB24Normal.Enabled = isColorModeSupported(uEye.Defines.ColorMode.RGB8Packed, uEye.Defines.ColorConvertMode.Software3X3);
            radioButtonRGB24High.Enabled = isColorModeSupported(uEye.Defines.ColorMode.RGB8Packed, uEye.Defines.ColorConvertMode.Software5X5);
            radioButtonRGB24Hardware.Enabled = isColorModeSupported(uEye.Defines.ColorMode.RGB8Packed, uEye.Defines.ColorConvertMode.Hardware3X3);

            radioButtonRGB32Normal.Enabled = isColorModeSupported(uEye.Defines.ColorMode.RGBA8Packed, uEye.Defines.ColorConvertMode.Software3X3);
            radioButtonRGB32High.Enabled = isColorModeSupported(uEye.Defines.ColorMode.RGBA8Packed, uEye.Defines.ColorConvertMode.Software5X5);
            radioButtonRGB32Hardware.Enabled = isColorModeSupported(uEye.Defines.ColorMode.RGBA8Packed, uEye.Defines.ColorConvertMode.Hardware3X3);

            // select active
            uEye.Defines.ColorMode colorMode;
            uEye.Defines.ColorConvertMode convertMode;

            statusRet = m_Camera.PixelFormat.Get(out colorMode);
            statusRet = m_Camera.Color.Converter.Get(colorMode, out convertMode);


            radioButtonGray8Normal.Checked = colorMode == uEye.Defines.ColorMode.Mono8 && convertMode == uEye.Defines.ColorConvertMode.Software3X3;
            radioButtonGray8High.Checked = colorMode == uEye.Defines.ColorMode.Mono8 && convertMode == uEye.Defines.ColorConvertMode.Software5X5;
            radioButtonGray8Hardware.Checked = colorMode == uEye.Defines.ColorMode.Mono8 && convertMode == uEye.Defines.ColorConvertMode.Hardware3X3;

            radioButtonGray12Normal.Checked = colorMode == uEye.Defines.ColorMode.Mono12 && convertMode == uEye.Defines.ColorConvertMode.Software3X3;
            radioButtonGray12High.Checked = colorMode == uEye.Defines.ColorMode.Mono12 && convertMode == uEye.Defines.ColorConvertMode.Software5X5;
            radioButtonGray12Hardware.Checked = colorMode == uEye.Defines.ColorMode.Mono12 && convertMode == uEye.Defines.ColorConvertMode.Hardware3X3;

            radioButtonGray16Normal.Checked = colorMode == uEye.Defines.ColorMode.Mono16 && convertMode == uEye.Defines.ColorConvertMode.Software3X3;
            radioButtonGray16High.Checked = colorMode == uEye.Defines.ColorMode.Mono16 && convertMode == uEye.Defines.ColorConvertMode.Software5X5;
            radioButtonGray16Hardware.Checked = colorMode == uEye.Defines.ColorMode.Mono16 && convertMode == uEye.Defines.ColorConvertMode.Hardware3X3;


            radioButtonRaw8Normal.Checked = colorMode == uEye.Defines.ColorMode.SensorRaw8 && convertMode == uEye.Defines.ColorConvertMode.Software3X3;
            radioButtonRaw8High.Checked = colorMode == uEye.Defines.ColorMode.SensorRaw8 && convertMode == uEye.Defines.ColorConvertMode.Software5X5;
            radioButtonRaw8Hardware.Checked = colorMode == uEye.Defines.ColorMode.SensorRaw8 && convertMode == uEye.Defines.ColorConvertMode.Hardware3X3;

            radioButtonRaw12Normal.Checked = colorMode == uEye.Defines.ColorMode.SensorRaw12 && convertMode == uEye.Defines.ColorConvertMode.Software3X3;
            radioButtonRaw12High.Checked = colorMode == uEye.Defines.ColorMode.SensorRaw12 && convertMode == uEye.Defines.ColorConvertMode.Software5X5;
            radioButtonRaw12Hardware.Checked = colorMode == uEye.Defines.ColorMode.SensorRaw12 && convertMode == uEye.Defines.ColorConvertMode.Hardware3X3;

            radioButtonRaw16Normal.Checked = colorMode == uEye.Defines.ColorMode.SensorRaw16 && convertMode == uEye.Defines.ColorConvertMode.Software3X3;
            radioButtonRaw16High.Checked = colorMode == uEye.Defines.ColorMode.SensorRaw16 && convertMode == uEye.Defines.ColorConvertMode.Software5X5;
            radioButtonRaw16Hardware.Checked = colorMode == uEye.Defines.ColorMode.SensorRaw16 && convertMode == uEye.Defines.ColorConvertMode.Hardware3X3;


            radioButtonRGB24Normal.Checked = colorMode == uEye.Defines.ColorMode.BGR8Packed && convertMode == uEye.Defines.ColorConvertMode.Software3X3;
            radioButtonRGB24High.Checked = colorMode == uEye.Defines.ColorMode.BGR8Packed && convertMode == uEye.Defines.ColorConvertMode.Software5X5;
            radioButtonRGB24Hardware.Checked = colorMode == uEye.Defines.ColorMode.BGR8Packed && convertMode == uEye.Defines.ColorConvertMode.Hardware3X3;

            radioButtonRGB32Normal.Checked = colorMode == uEye.Defines.ColorMode.BGRA8Packed && convertMode == uEye.Defines.ColorConvertMode.Software3X3;
            radioButtonRGB32High.Checked = colorMode == uEye.Defines.ColorMode.BGRA8Packed && convertMode == uEye.Defines.ColorConvertMode.Software5X5;
            radioButtonRGB32Hardware.Checked = colorMode == uEye.Defines.ColorMode.BGRA8Packed && convertMode == uEye.Defines.ColorConvertMode.Hardware3X3;

        }

        private Boolean isColorModeSupported(uEye.Defines.ColorMode colorMode, uEye.Defines.ColorConvertMode colorConvertMode)
        {
            uEye.Defines.ColorConvertMode converterMode = 0;
            uEye.Defines.Status statusRet;

            statusRet = m_Camera.Color.Converter.GetSupported(colorMode, out converterMode);
            if (statusRet == uEye.Defines.Status.SUCCESS && converterMode != uEye.Defines.ColorConvertMode.None)
            {
                if (uEye.Defines.ColorConvertMode.Software3X3 == colorConvertMode)
                {
                    return (converterMode & uEye.Defines.ColorConvertMode.Software3X3) != uEye.Defines.ColorConvertMode.None ? true : false;
                }
                if (uEye.Defines.ColorConvertMode.Software5X5 == colorConvertMode)
                {
                    return (converterMode & uEye.Defines.ColorConvertMode.Software5X5) != uEye.Defines.ColorConvertMode.None ? true : false;
                }
                if (uEye.Defines.ColorConvertMode.Hardware3X3 == colorConvertMode)
                {
                    return (converterMode & uEye.Defines.ColorConvertMode.Hardware3X3) != uEye.Defines.ColorConvertMode.None ? true : false;
                }
            }

            return false;
        }

        private void onPixelFormat_CheckedChanged(object sender, EventArgs e)
        {
            uEye.Defines.ColorMode colorMode = 0;
            uEye.Defines.ColorConvertMode convertMode = 0;

            if (radioButtonRaw8Normal.Checked)
            {
                colorMode = uEye.Defines.ColorMode.SensorRaw8;
                convertMode = uEye.Defines.ColorConvertMode.Software3X3;
            }
            else if (radioButtonRaw8High.Checked)
            {
                colorMode = uEye.Defines.ColorMode.SensorRaw8;
                convertMode = uEye.Defines.ColorConvertMode.Software5X5;
            }
            else if (radioButtonRaw8Hardware.Checked)
            {
                colorMode = uEye.Defines.ColorMode.SensorRaw8;
                convertMode = uEye.Defines.ColorConvertMode.Hardware3X3;
            }
            else if (radioButtonRaw12Normal.Checked)
            {
                colorMode = uEye.Defines.ColorMode.SensorRaw12;
                convertMode = uEye.Defines.ColorConvertMode.Software3X3;
            }
            else if (radioButtonRaw12High.Checked)
            {
                colorMode = uEye.Defines.ColorMode.SensorRaw12;
                convertMode = uEye.Defines.ColorConvertMode.Software5X5;
            }
            else if (radioButtonRaw12Hardware.Checked)
            {
                colorMode = uEye.Defines.ColorMode.SensorRaw12;
                convertMode = uEye.Defines.ColorConvertMode.Hardware3X3;
            }
            else if (radioButtonRaw16Normal.Checked)
            {
                colorMode = uEye.Defines.ColorMode.SensorRaw16;
                convertMode = uEye.Defines.ColorConvertMode.Software3X3;
            }
            else if (radioButtonRaw16High.Checked)
            {
                colorMode = uEye.Defines.ColorMode.SensorRaw16;
                convertMode = uEye.Defines.ColorConvertMode.Software5X5;
            }
            else if (radioButtonRaw16Hardware.Checked)
            {
                colorMode = uEye.Defines.ColorMode.SensorRaw16;
                convertMode = uEye.Defines.ColorConvertMode.Hardware3X3;
            }
            else if (radioButtonGray8Normal.Checked)
            {
                colorMode = uEye.Defines.ColorMode.Mono8;
                convertMode = uEye.Defines.ColorConvertMode.Software3X3;
            }
            else if (radioButtonGray8High.Checked)
            {
                colorMode = uEye.Defines.ColorMode.Mono8;
                convertMode = uEye.Defines.ColorConvertMode.Software5X5;
            }
            else if (radioButtonGray8Hardware.Checked)
            {
                colorMode = uEye.Defines.ColorMode.Mono8;
                convertMode = uEye.Defines.ColorConvertMode.Hardware3X3;
            }
            else if (radioButtonGray12Normal.Checked)
            {
                colorMode = uEye.Defines.ColorMode.Mono12;
                convertMode = uEye.Defines.ColorConvertMode.Software3X3;
            }
            else if (radioButtonGray12High.Checked)
            {
                colorMode = uEye.Defines.ColorMode.Mono12;
                convertMode = uEye.Defines.ColorConvertMode.Software5X5;
            }
            else if (radioButtonGray12Hardware.Checked)
            {
                colorMode = uEye.Defines.ColorMode.Mono12;
                convertMode = uEye.Defines.ColorConvertMode.Hardware3X3;
            }
            else if (radioButtonGray16Normal.Checked)
            {
                colorMode = uEye.Defines.ColorMode.Mono16;
                convertMode = uEye.Defines.ColorConvertMode.Software3X3;
            }
            else if (radioButtonGray16High.Checked)
            {
                colorMode = uEye.Defines.ColorMode.Mono16;
                convertMode = uEye.Defines.ColorConvertMode.Software5X5;
            }
            else if (radioButtonGray16Hardware.Checked)
            {
                colorMode = uEye.Defines.ColorMode.Mono16;
                convertMode = uEye.Defines.ColorConvertMode.Hardware3X3;
            }
            else if (radioButtonRGB24Normal.Checked)
            {
                colorMode = uEye.Defines.ColorMode.BGR8Packed;
                convertMode = uEye.Defines.ColorConvertMode.Software3X3;
            }
            else if (radioButtonRGB24High.Checked)
            {
                colorMode = uEye.Defines.ColorMode.BGR8Packed;
                convertMode = uEye.Defines.ColorConvertMode.Software5X5;
            }
            else if (radioButtonRGB24Hardware.Checked)
            {
                colorMode = uEye.Defines.ColorMode.BGR8Packed;
                convertMode = uEye.Defines.ColorConvertMode.Hardware3X3;
            }
            else if (radioButtonRGB32Normal.Checked)
            {
                colorMode = uEye.Defines.ColorMode.BGRA8Packed;
                convertMode = uEye.Defines.ColorConvertMode.Software3X3;
            }
            else if (radioButtonRGB32High.Checked)
            {
                colorMode = uEye.Defines.ColorMode.BGRA8Packed;
                convertMode = uEye.Defines.ColorConvertMode.Software5X5;
            }
            else if (radioButtonRGB32Hardware.Checked)
            {
                colorMode = uEye.Defines.ColorMode.BGRA8Packed;
                convertMode = uEye.Defines.ColorConvertMode.Hardware3X3;
            }

            uEye.Defines.Status statusRet;

            Boolean isLive;
            statusRet = m_Camera.Acquisition.HasStarted(out isLive);

            if (isLive)
            {
                statusRet = m_Camera.Acquisition.Stop(uEye.Defines.DeviceParameter.Wait);
            }

            statusRet = m_Camera.PixelFormat.Set(colorMode);
            statusRet = m_Camera.Color.Converter.Set(colorMode, convertMode);

            // memory reallocation
            int[] idList;
            m_Camera.Memory.GetList(out idList);

            statusRet = MemoryHelper.ClearSequence(m_Camera);
            statusRet = MemoryHelper.FreeImageMems(m_Camera);

            statusRet = MemoryHelper.AllocImageMems(m_Camera, idList.Count());
            statusRet = MemoryHelper.InitSequence(m_Camera);

            if (isLive)
            {
                statusRet = m_Camera.Acquisition.Capture();
            }
        }

        private void UpdateDisplay()
        {
            uEye.Defines.DisplayMode displayMode;

            // check supported
            m_Camera.DirectRenderer.GetSupported(out displayMode);

            radioButtonDisplayOpenGL.Enabled = (displayMode & uEye.Defines.DisplayMode.OpenGL) == uEye.Defines.DisplayMode.OpenGL;
            radioButtonDisplayDirect3D.Enabled = (displayMode & uEye.Defines.DisplayMode.Direct3D) == uEye.Defines.DisplayMode.Direct3D;

            m_Camera.Display.Mode.Get(out displayMode);

            switch (displayMode)
            {
                case uEye.Defines.DisplayMode.DiB:
                    radioButtonDisplayDIB.Checked = true;
                    break;
                case uEye.Defines.DisplayMode.Direct3D:
                    radioButtonDisplayDirect3D.Checked = true;
                    break;
                case uEye.Defines.DisplayMode.OpenGL:
                    radioButtonDisplayOpenGL.Checked = true;
                    break;
            }
        }

        private void radioButtonDisplayDIB_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonDisplayDIB.Checked && radioButtonDisplayDIB.Focused)
            {
                uEye.Defines.Status statusRet;
                uEye.Defines.DisplayMode displayMode = uEye.Defines.DisplayMode.DiB;

                statusRet = m_Camera.Display.Mode.Set(displayMode);

                // inform our main form
                if (DisplayChanged != null)
                {
                    DisplayChanged(this, EventArgs.Empty);
                }
            }
        }

        private void radioButtonDisplayDirect3D_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonDisplayDirect3D.Checked && radioButtonDisplayDirect3D.Focused)
            {
                uEye.Defines.Status statusRet;

                statusRet = m_Camera.Display.Mode.Set(uEye.Defines.DisplayMode.Direct3D);

                if (statusRet != uEye.Defines.Status.SUCCESS)
                {
                    MessageBox.Show("Setting display mode failed");
                    radioButtonDisplayDIB.Checked = true;
                }
                else
                {
                    // inform our main form
                    if (DisplayChanged != null)
                    {
                        DisplayChanged(this, EventArgs.Empty);
                    }
                }
            }
        }

        private void radioButtonDisplayOpenGL_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonDisplayOpenGL.Checked && radioButtonDisplayOpenGL.Focused)
            {
                uEye.Defines.Status statusRet;

                statusRet = m_Camera.Display.Mode.Set(uEye.Defines.DisplayMode.OpenGL);

                if (statusRet != uEye.Defines.Status.SUCCESS)
                {
                    MessageBox.Show("Setting display mode failed");
                    radioButtonDisplayDIB.Checked = true;
                }
                else
                {
                    // inform our main form
                    if (DisplayChanged != null)
                    {
                        DisplayChanged(this, EventArgs.Empty);
                    }
                }
            }
        }
    }
}
