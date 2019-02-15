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
    public partial class SizeControl : IControl
    {
        public event EventHandler<EventArgs> AOIChanged;

        public SizeControl(uEye.Camera camera)
            : base(camera)
        {
            InitializeComponent();
        }

        public override void OnControlFocusActive()
        {
            UpdateAoiControls();
            UpdateBinningSubsamplingControls();
        }

        public override void OnControlFocusLost()
        {
        }

        private void tabControlSize_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (tabControlSize.SelectedIndex)
            {
                case 0:
                    UpdateAoiControls();
                    break;
                case 1:
                    UpdateBinningSubsamplingControls();
                    break;
            }
        }

        private void UpdateAoiControls()
        {
            uEye.Defines.Status statusRet;

            // get camera aoi range size
            uEye.Types.Range<Int32> rangeWidth, rangeHeight;
            statusRet = m_Camera.Size.AOI.GetSizeRange(out rangeWidth, out rangeHeight);

            // set control ranges
            trackBarAoiHeight.SetRange(rangeHeight.Minimum, rangeHeight.Maximum);

            numericUpDownAoiHeight.Minimum = rangeHeight.Minimum;
            numericUpDownAoiHeight.Maximum = rangeHeight.Maximum;
            numericUpDownAoiHeight.Increment = rangeHeight.Increment;

            labelAoiHeightMin.Text = rangeHeight.Minimum.ToString();
            labelAoiHeightMax.Text = rangeHeight.Maximum.ToString();

            trackBarAoiWidth.SetRange(rangeWidth.Minimum, rangeWidth.Maximum);
            trackBarAoiWidth.TickFrequency = rangeHeight.Increment;

            numericUpDownAoiWidth.Minimum = rangeWidth.Minimum;
            numericUpDownAoiWidth.Maximum = rangeWidth.Maximum;
            numericUpDownAoiWidth.Increment = rangeWidth.Increment;

            labelAoiWidthMin.Text = rangeWidth.Minimum.ToString();
            labelAoiWidthMax.Text = rangeWidth.Maximum.ToString();

            // get actual aoi
            System.Drawing.Rectangle rect;
            statusRet = m_Camera.Size.AOI.Get(out rect);

            // set aoi values
            trackBarAoiHeight.Value = rect.Height;
            numericUpDownAoiHeight.Value = rect.Height;

            trackBarAoiWidth.Value = rect.Width;
            numericUpDownAoiWidth.Value = rect.Width;

            uEye.Types.Range<Int32> rangePosX, rangePosY;
            statusRet = m_Camera.Size.AOI.GetPosRange(out rangePosX, out rangePosY);

            // update top aoi
            numericUpDownAoiTop.Minimum = 0;
            numericUpDownAoiTop.Maximum = trackBarAoiHeight.Maximum - trackBarAoiHeight.Value;
            numericUpDownAoiTop.Increment = rangePosY.Increment;
            trackBarAoiTop.SetRange((int)numericUpDownAoiTop.Minimum, (int)numericUpDownAoiTop.Maximum);

            labelAoiTopMin.Text = trackBarAoiTop.Minimum.ToString();
            labelAoiTopMax.Text = trackBarAoiTop.Maximum.ToString();

            // update left aoi
            numericUpDownAoiLeft.Minimum = 0;
            numericUpDownAoiLeft.Maximum = trackBarAoiWidth.Maximum - trackBarAoiWidth.Value;
            numericUpDownAoiLeft.Increment = rangePosX.Increment;
            trackBarAoiLeft.SetRange((int)numericUpDownAoiLeft.Minimum, (int)numericUpDownAoiLeft.Maximum);

            labelAoiLeftMin.Text = trackBarAoiLeft.Minimum.ToString();
            labelAoiLeftMax.Text = trackBarAoiLeft.Maximum.ToString();
        }

        private void UpdateBinningSubsamplingControls()
        {
            // subsampling && binning
            updateHorizontalBinning();
            updateVerticalBinning();

            updateHorizontalSubsampling();
            updateVerticalSubsampling();
        }

        private void updateHorizontalBinning()
        {
            // horizontal binning
            comboBoxFormatBinningHorizontal.Items.Clear();
            comboBoxFormatBinningHorizontal.Items.Add("1x");

            uEye.Defines.BinningMode mode;
            m_Camera.Size.Binning.GetSupported(out mode);
            if ((mode & uEye.Defines.BinningMode.Disable) == mode)
            {
                comboBoxFormatBinningHorizontal.Enabled = false;
            }
            else
            {
                if (m_Camera.Size.Binning.IsSupported(uEye.Defines.BinningMode.Horizontal2X))
                {
                    comboBoxFormatBinningHorizontal.Items.Add("2x");
                }
                if (m_Camera.Size.Binning.IsSupported(uEye.Defines.BinningMode.Horizontal3X))
                {
                    comboBoxFormatBinningHorizontal.Items.Add("3x");
                }
                if (m_Camera.Size.Binning.IsSupported(uEye.Defines.BinningMode.Horizontal4X))
                {
                    comboBoxFormatBinningHorizontal.Items.Add("4x");
                }
                if (m_Camera.Size.Binning.IsSupported(uEye.Defines.BinningMode.Horizontal5X))
                {
                    comboBoxFormatBinningHorizontal.Items.Add("5x");
                }
                if (m_Camera.Size.Binning.IsSupported(uEye.Defines.BinningMode.Horizontal6X))
                {
                    comboBoxFormatBinningHorizontal.Items.Add("6x");
                }
                if (m_Camera.Size.Binning.IsSupported(uEye.Defines.BinningMode.Horizontal8X))
                {
                    comboBoxFormatBinningHorizontal.Items.Add("8x");
                }
                if (m_Camera.Size.Binning.IsSupported(uEye.Defines.BinningMode.Horizontal16X))
                {
                    comboBoxFormatBinningHorizontal.Items.Add("16x");
                }
            }

            Int32 s32Factor;

            m_Camera.Size.Binning.GetFactorHorizontal(out s32Factor);
            comboBoxFormatBinningHorizontal.SelectedItem = s32Factor + "x";
        }

        private void updateVerticalBinning()
        {
            // vertical binning
            comboBoxFormatBinningVertical.Items.Clear();
            comboBoxFormatBinningVertical.Items.Add("1x");

            uEye.Defines.BinningMode mode;
            m_Camera.Size.Binning.GetSupported(out mode);
            if ((mode & uEye.Defines.BinningMode.Disable) == mode)
            {
                comboBoxFormatBinningVertical.Enabled = false;
            }
            else
            {
                if (m_Camera.Size.Binning.IsSupported(uEye.Defines.BinningMode.Vertical2X))
                {
                    comboBoxFormatBinningVertical.Items.Add("2x");
                }
                if (m_Camera.Size.Binning.IsSupported(uEye.Defines.BinningMode.Vertical3X))
                {
                    comboBoxFormatBinningVertical.Items.Add("3x");
                }
                if (m_Camera.Size.Binning.IsSupported(uEye.Defines.BinningMode.Vertical4X))
                {
                    comboBoxFormatBinningVertical.Items.Add("4x");
                }
                if (m_Camera.Size.Binning.IsSupported(uEye.Defines.BinningMode.Vertical5X))
                {
                    comboBoxFormatBinningVertical.Items.Add("5x");
                }
                if (m_Camera.Size.Binning.IsSupported(uEye.Defines.BinningMode.Vertical6X))
                {
                    comboBoxFormatBinningVertical.Items.Add("6x");
                }
                if (m_Camera.Size.Binning.IsSupported(uEye.Defines.BinningMode.Vertical8X))
                {
                    comboBoxFormatBinningVertical.Items.Add("8x");
                }
                if (m_Camera.Size.Binning.IsSupported(uEye.Defines.BinningMode.Vertical16X))
                {
                    comboBoxFormatBinningVertical.Items.Add("16x");
                }
            }

            Int32 s32Factor;

            m_Camera.Size.Binning.GetFactorVertical(out s32Factor);
            comboBoxFormatBinningVertical.SelectedItem = s32Factor + "x";
        }

        private void updateHorizontalSubsampling()
        {
            // horizontal binning
            comboBoxFormatSubsamplingHorizontal.Items.Clear();
            comboBoxFormatSubsamplingHorizontal.Items.Add("1x");

            uEye.Defines.SubsamplingMode mode;
            m_Camera.Size.Subsampling.GetSupported(out mode);
            if ((mode & uEye.Defines.SubsamplingMode.Disable) == mode)
            {
                comboBoxFormatSubsamplingHorizontal.Enabled = false;
            }
            else
            {
                if (m_Camera.Size.Subsampling.IsSupported(uEye.Defines.SubsamplingMode.Horizontal2X))
                {
                    comboBoxFormatSubsamplingHorizontal.Items.Add("2x");
                }
                if (m_Camera.Size.Subsampling.IsSupported(uEye.Defines.SubsamplingMode.Horizontal3X))
                {
                    comboBoxFormatSubsamplingHorizontal.Items.Add("3x");
                }
                if (m_Camera.Size.Subsampling.IsSupported(uEye.Defines.SubsamplingMode.Horizontal4X))
                {
                    comboBoxFormatSubsamplingHorizontal.Items.Add("4x");
                }
                if (m_Camera.Size.Subsampling.IsSupported(uEye.Defines.SubsamplingMode.Horizontal5X))
                {
                    comboBoxFormatSubsamplingHorizontal.Items.Add("5x");
                }
                if (m_Camera.Size.Subsampling.IsSupported(uEye.Defines.SubsamplingMode.Horizontal6X))
                {
                    comboBoxFormatSubsamplingHorizontal.Items.Add("6x");
                }
                if (m_Camera.Size.Subsampling.IsSupported(uEye.Defines.SubsamplingMode.Horizontal8X))
                {
                    comboBoxFormatSubsamplingHorizontal.Items.Add("8x");
                }
                if (m_Camera.Size.Subsampling.IsSupported(uEye.Defines.SubsamplingMode.Horizontal16X))
                {
                    comboBoxFormatSubsamplingHorizontal.Items.Add("16x");
                }
            }

            Int32 s32Factor;

            m_Camera.Size.Subsampling.GetFactorHorizontal(out s32Factor);
            comboBoxFormatSubsamplingHorizontal.SelectedItem = s32Factor + "x";
        }

        private void updateVerticalSubsampling()
        {
            // vertical Subsampling
            comboBoxFormatSubsamplingVertical.Items.Clear();
            comboBoxFormatSubsamplingVertical.Items.Add("1x");

            uEye.Defines.SubsamplingMode mode;
            m_Camera.Size.Subsampling.GetSupported(out mode);
            if ((mode & uEye.Defines.SubsamplingMode.Disable) == mode)
            {
                comboBoxFormatSubsamplingVertical.Enabled = false;
            }
            else
            {
                if (m_Camera.Size.Subsampling.IsSupported(uEye.Defines.SubsamplingMode.Vertical2X))
                {
                    comboBoxFormatSubsamplingVertical.Items.Add("2x");
                }
                if (m_Camera.Size.Subsampling.IsSupported(uEye.Defines.SubsamplingMode.Vertical3X))
                {
                    comboBoxFormatSubsamplingVertical.Items.Add("3x");
                }
                if (m_Camera.Size.Subsampling.IsSupported(uEye.Defines.SubsamplingMode.Vertical4X))
                {
                    comboBoxFormatSubsamplingVertical.Items.Add("4x");
                }
                if (m_Camera.Size.Subsampling.IsSupported(uEye.Defines.SubsamplingMode.Vertical5X))
                {
                    comboBoxFormatSubsamplingVertical.Items.Add("5x");
                }
                if (m_Camera.Size.Subsampling.IsSupported(uEye.Defines.SubsamplingMode.Vertical6X))
                {
                    comboBoxFormatSubsamplingVertical.Items.Add("6x");
                }
                if (m_Camera.Size.Subsampling.IsSupported(uEye.Defines.SubsamplingMode.Vertical8X))
                {
                    comboBoxFormatSubsamplingVertical.Items.Add("8x");
                }
                if (m_Camera.Size.Subsampling.IsSupported(uEye.Defines.SubsamplingMode.Vertical16X))
                {
                    comboBoxFormatSubsamplingVertical.Items.Add("16x");
                }
            }

            Int32 s32Factor;

            m_Camera.Size.Subsampling.GetFactorVertical(out s32Factor);
            comboBoxFormatSubsamplingVertical.SelectedItem = s32Factor + "x";
        }

        private uEye.Defines.BinningMode GetBinningMode()
        {
            // get current selected binning mode
            string strMode = "";
            strMode = comboBoxFormatBinningVertical.SelectedItem.ToString();

            // check which binning mode
            // and save it to nMode
            uEye.Defines.BinningMode modeVertical = uEye.Defines.BinningMode.Disable;
            if (strMode == "1x")
            {
                modeVertical = uEye.Defines.BinningMode.Disable;
            }
            if (strMode == "2x")
            {
                modeVertical = uEye.Defines.BinningMode.Vertical2X;
            }
            if (strMode == "3x")
            {
                modeVertical = uEye.Defines.BinningMode.Vertical3X;
            }
            if (strMode == "4x")
            {
                modeVertical = uEye.Defines.BinningMode.Vertical4X;
            }
            if (strMode == "5x")
            {
                modeVertical = uEye.Defines.BinningMode.Vertical5X;
            }
            if (strMode == "6x")
            {
                modeVertical = uEye.Defines.BinningMode.Vertical6X;
            }
            if (strMode == "8x")
            {
                modeVertical = uEye.Defines.BinningMode.Vertical8X;
            }
            if (strMode == "16x")
            {
                modeVertical = uEye.Defines.BinningMode.Vertical16X;
            }

            string strModeHorizontal = "";
            strModeHorizontal = comboBoxFormatBinningHorizontal.SelectedItem.ToString();

            // check which binning mode
            // and save it to nMode
            uEye.Defines.BinningMode modeHorizontal = uEye.Defines.BinningMode.Disable;
            if (strModeHorizontal == "1x")
            {
                modeHorizontal = uEye.Defines.BinningMode.Disable;
            }
            if (strModeHorizontal == "2x")
            {
                modeHorizontal = uEye.Defines.BinningMode.Horizontal2X;
            }
            if (strModeHorizontal == "3x")
            {
                modeHorizontal = uEye.Defines.BinningMode.Horizontal3X;
            }
            if (strModeHorizontal == "4x")
            {
                modeHorizontal = uEye.Defines.BinningMode.Horizontal4X;
            }
            if (strModeHorizontal == "5x")
            {
                modeHorizontal = uEye.Defines.BinningMode.Horizontal5X;
            }
            if (strModeHorizontal == "6x")
            {
                modeHorizontal = uEye.Defines.BinningMode.Horizontal6X;
            }
            if (strModeHorizontal == "8x")
            {
                modeHorizontal = uEye.Defines.BinningMode.Horizontal8X;
            }
            if (strModeHorizontal == "16x")
            {
                modeHorizontal = uEye.Defines.BinningMode.Horizontal16X;
            }

            return modeVertical | modeHorizontal;
        }

        private uEye.Defines.SubsamplingMode GetSubsamplingMode()
        {
            // get current selected subsampling mode
            string strModeVertical = "";
            strModeVertical = comboBoxFormatSubsamplingVertical.SelectedItem.ToString();

            // check which subsampling mode
            // and save it to nMode
            uEye.Defines.SubsamplingMode modeVertical = uEye.Defines.SubsamplingMode.Disable;
            if (strModeVertical == "1x")
            {
                modeVertical = uEye.Defines.SubsamplingMode.Disable;
            }
            if (strModeVertical == "2x")
            {
                modeVertical = uEye.Defines.SubsamplingMode.Vertical2X;
            }
            if (strModeVertical == "3x")
            {
                modeVertical = uEye.Defines.SubsamplingMode.Vertical3X;
            }
            if (strModeVertical == "4x")
            {
                modeVertical = uEye.Defines.SubsamplingMode.Vertical4X;
            }
            if (strModeVertical == "5x")
            {
                modeVertical = uEye.Defines.SubsamplingMode.Vertical5X;
            }
            if (strModeVertical == "6x")
            {
                modeVertical = uEye.Defines.SubsamplingMode.Vertical6X;
            }
            if (strModeVertical == "8x")
            {
                modeVertical = uEye.Defines.SubsamplingMode.Vertical8X;
            }
            if (strModeVertical == "16x")
            {
                modeVertical = uEye.Defines.SubsamplingMode.Vertical16X;
            }

            // get current selected subsampling mode
            string strModeHorizontal = "";
            strModeHorizontal = comboBoxFormatSubsamplingHorizontal.SelectedItem.ToString();

            // check which subsampling mode
            // and save it to nMode
            uEye.Defines.SubsamplingMode modeHorizontal = uEye.Defines.SubsamplingMode.Disable;
            if (strModeHorizontal == "1x")
            {
                modeHorizontal = uEye.Defines.SubsamplingMode.Disable;
            }
            if (strModeHorizontal == "2x")
            {
                modeHorizontal = uEye.Defines.SubsamplingMode.Horizontal2X;
            }
            if (strModeHorizontal == "3x")
            {
                modeHorizontal = uEye.Defines.SubsamplingMode.Horizontal3X;
            }
            if (strModeHorizontal == "4x")
            {
                modeHorizontal = uEye.Defines.SubsamplingMode.Horizontal4X;
            }
            if (strModeHorizontal == "5x")
            {
                modeHorizontal = uEye.Defines.SubsamplingMode.Horizontal5X;
            }
            if (strModeHorizontal == "6x")
            {
                modeHorizontal = uEye.Defines.SubsamplingMode.Horizontal6X;
            }
            if (strModeHorizontal == "8x")
            {
                modeHorizontal = uEye.Defines.SubsamplingMode.Horizontal8X;
            }
            if (strModeHorizontal == "16x")
            {
                modeHorizontal = uEye.Defines.SubsamplingMode.Horizontal16X;
            }

            return modeVertical | modeHorizontal;
        }

        private void checkBoxFormatMirrorLeftRight_CheckedChanged(object sender, EventArgs e)
        {
            uEye.Defines.Status statusRet;

            statusRet = m_Camera.RopEffect.Set(uEye.Defines.RopEffectMode.LeftRight, checkBoxFormatMirrorLeftRight.Checked);
        }

        private void checkBoxFormatMirrorUpDown_CheckedChanged(object sender, EventArgs e)
        {
            uEye.Defines.Status statusRet;

            statusRet = m_Camera.RopEffect.Set(uEye.Defines.RopEffectMode.UpDown, checkBoxFormatMirrorUpDown.Checked);
        }

        private void SetAoiWidth(Int32 s32Value)
        {
            uEye.Defines.Status statusRet;
            System.Drawing.Rectangle rect;

            uEye.Types.Range<Int32> rangeWidth, rangeHeight;
            m_Camera.Size.AOI.GetPosRange(out rangeWidth, out rangeHeight);

            while ((s32Value % rangeWidth.Increment) != 0)
            {
                --s32Value;
            }

            statusRet = m_Camera.Size.AOI.Get(out rect);
            rect.Width = s32Value;
            numericUpDownAoiWidth.Value = s32Value;
            trackBarAoiWidth.Value = s32Value;

            Boolean isLive;
            statusRet = m_Camera.Acquisition.HasStarted(out isLive);

            if (isLive)
            {
                statusRet = m_Camera.Acquisition.Stop(uEye.Defines.DeviceParameter.Wait);
            }

            statusRet = m_Camera.Size.AOI.Set(rect);

            // update left aoi
            numericUpDownAoiLeft.Minimum = 0;
            numericUpDownAoiLeft.Maximum = numericUpDownAoiLeft.Value + trackBarAoiWidth.Maximum - trackBarAoiWidth.Value;
            trackBarAoiLeft.SetRange((int)numericUpDownAoiLeft.Minimum, (int)numericUpDownAoiLeft.Maximum);

            labelAoiLeftMin.Text = trackBarAoiLeft.Minimum.ToString();
            labelAoiLeftMax.Text = trackBarAoiLeft.Maximum.ToString();

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

            // inform our main form
            AOIChanged.Invoke(this, EventArgs.Empty);
        }

        private void trackBarAoiWidth_Scroll(object sender, EventArgs e)
        {
            if (trackBarAoiWidth.Focused)
            {
                SetAoiWidth(trackBarAoiWidth.Value);
            }
        }

        private void SetAoiHeight(Int32 s32Value)
        {
            uEye.Defines.Status statusRet;
            System.Drawing.Rectangle rect;

            uEye.Types.Range<Int32> rangeWidth, rangeHeight;
            statusRet = m_Camera.Size.AOI.GetPosRange(out rangeWidth, out rangeHeight);

            while ((s32Value % rangeHeight.Increment) != 0)
            {
                --s32Value;
            }

            statusRet = m_Camera.Size.AOI.Get(out rect);
            rect.Height = s32Value;
            numericUpDownAoiHeight.Value = s32Value;
            trackBarAoiHeight.Value = s32Value;

            Boolean isLive;
            statusRet = m_Camera.Acquisition.HasStarted(out isLive);

            if (isLive)
            {
                statusRet = m_Camera.Acquisition.Stop(uEye.Defines.DeviceParameter.Wait);
            }

            statusRet = m_Camera.Size.AOI.Set(rect);

            // update top aoi
            numericUpDownAoiTop.Minimum = 0;
            numericUpDownAoiTop.Maximum = numericUpDownAoiTop.Value + trackBarAoiHeight.Maximum - trackBarAoiHeight.Value;
            trackBarAoiTop.SetRange((int)numericUpDownAoiTop.Minimum, (int)numericUpDownAoiTop.Maximum);

            labelAoiTopMin.Text = trackBarAoiTop.Minimum.ToString();
            labelAoiTopMax.Text = trackBarAoiTop.Maximum.ToString();

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

            // inform our main form
            AOIChanged.Invoke(this, EventArgs.Empty);
        }

        private void trackBarAoiHeight_Scroll(object sender, EventArgs e)
        {
            if (trackBarAoiHeight.Focused)
            {
                SetAoiHeight(trackBarAoiHeight.Value);
            }
        }

        private void SetAoiLeft(Int32 s32Value)
        {
            uEye.Defines.Status statusRet;
            System.Drawing.Rectangle rect;

            uEye.Types.Range<Int32> rangePosX, rangePosY;
            statusRet = m_Camera.Size.AOI.GetPosRange(out rangePosX, out rangePosY);

            while ((s32Value % rangePosX.Increment) != 0)
            {
                --s32Value;
            }

            statusRet = m_Camera.Size.AOI.Get(out rect);
            rect.X = s32Value;
            numericUpDownAoiLeft.Value = s32Value;
            trackBarAoiLeft.Value = s32Value;

            statusRet = m_Camera.Size.AOI.Set(rect);

            // update aoi width
            uEye.Types.Range<Int32> rangeWidth, rangeHeight;
            statusRet = m_Camera.Size.AOI.GetSizeRange(out rangeWidth, out rangeHeight);

            trackBarAoiWidth.Maximum = rangeWidth.Maximum;
            numericUpDownAoiWidth.Maximum = trackBarAoiWidth.Maximum;

            labelAoiWidthMax.Text = trackBarAoiWidth.Maximum.ToString();

            // inform our main form
            AOIChanged.Invoke(this, EventArgs.Empty);
        }

        private void trackBarAoiLeft_Scroll(object sender, EventArgs e)
        {
            if (trackBarAoiLeft.Focused)
            {
                SetAoiLeft(trackBarAoiLeft.Value);
            }
        }

        private void SetAoiTop(Int32 s32Value)
        {
            uEye.Defines.Status statusRet;
            System.Drawing.Rectangle rect;

            uEye.Types.Range<Int32> rangePosX, rangePosY;
            statusRet = m_Camera.Size.AOI.GetPosRange(out rangePosX, out rangePosY);

            while ((s32Value % rangePosY.Increment) != 0)
            {
                --s32Value;
            }

            statusRet = m_Camera.Size.AOI.Get(out rect);
            rect.Y = s32Value;
            numericUpDownAoiTop.Value = s32Value;
            trackBarAoiTop.Value = s32Value;

            statusRet = m_Camera.Size.AOI.Set(rect);

            // update aoi height
            uEye.Types.Range<Int32> rangeWidth, rangeHeight;
            statusRet = m_Camera.Size.AOI.GetSizeRange(out rangeWidth, out rangeHeight);

            trackBarAoiHeight.Maximum = rangeHeight.Maximum;
            numericUpDownAoiHeight.Maximum = trackBarAoiHeight.Maximum;

            labelAoiHeightMax.Text = trackBarAoiHeight.Maximum.ToString();

            // inform our main form
            AOIChanged.Invoke(this, EventArgs.Empty);
        }

        private void trackBarAoiTop_Scroll(object sender, EventArgs e)
        {
            if (trackBarAoiTop.Focused)
            {
                SetAoiTop(trackBarAoiTop.Value);
            }
        }

        private void comboBoxFormatBinningHorizontal_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBoxFormatBinningHorizontal.Focused)
            {
                uEye.Defines.Status statusRet;

                Boolean isLive;
                statusRet = m_Camera.Acquisition.HasStarted(out isLive);

                if (isLive)
                {
                    statusRet = m_Camera.Acquisition.Stop(uEye.Defines.DeviceParameter.Wait);
                }

                uEye.Defines.BinningMode mode = GetBinningMode();
                statusRet = m_Camera.Size.Binning.Set(mode);

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

                updateVerticalBinning();
                updateHorizontalSubsampling();
                updateVerticalSubsampling();

                // inform our main form
                AOIChanged.Invoke(this, EventArgs.Empty);
            }
        }

        private void comboBoxFormatBinningVertical_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBoxFormatBinningVertical.Focused)
            {
                uEye.Defines.Status statusRet;

                Boolean isLive;
                statusRet = m_Camera.Acquisition.HasStarted(out isLive);

                if (isLive)
                {
                    statusRet = m_Camera.Acquisition.Stop(uEye.Defines.DeviceParameter.Wait);
                }

                uEye.Defines.BinningMode mode = GetBinningMode();
                statusRet = m_Camera.Size.Binning.Set(mode);

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

                updateHorizontalBinning();
                updateVerticalSubsampling();
                updateHorizontalSubsampling();

                // inform our main form
                AOIChanged.Invoke(this, EventArgs.Empty);
            }
        }

        private void comboBoxFormatSubsamplingHorizontal_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBoxFormatSubsamplingHorizontal.Focused)
            {
                uEye.Defines.Status statusRet;

                Boolean isLive;
                statusRet = m_Camera.Acquisition.HasStarted(out isLive);

                if (isLive)
                {
                    statusRet = m_Camera.Acquisition.Stop(uEye.Defines.DeviceParameter.Wait);
                }

                uEye.Defines.SubsamplingMode mode = GetSubsamplingMode();
                statusRet = m_Camera.Size.Subsampling.Set(mode);

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

                updateVerticalSubsampling();
                updateHorizontalBinning();
                updateVerticalBinning();

                // inform our main form
                AOIChanged.Invoke(this, EventArgs.Empty);
            }
        }

        private void comboBoxFormatSubsamplingVertical_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBoxFormatSubsamplingVertical.Focused)
            {
                uEye.Defines.Status statusRet;

                Boolean isLive;
                statusRet = m_Camera.Acquisition.HasStarted(out isLive);

                if (isLive)
                {
                    statusRet = m_Camera.Acquisition.Stop(uEye.Defines.DeviceParameter.Wait);
                }

                uEye.Defines.SubsamplingMode mode = GetSubsamplingMode();
                statusRet = m_Camera.Size.Subsampling.Set(mode);

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

                updateHorizontalSubsampling();
                updateVerticalBinning();
                updateHorizontalBinning();

                // inform our main form
                AOIChanged.Invoke(this, EventArgs.Empty);
            }
        }

        private void numericUpDownAoiWidth_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownAoiWidth.Focused)
            {
                SetAoiWidth(Convert.ToInt32(numericUpDownAoiWidth.Value));
            }
        }

        private void numericUpDownAoiHeight_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownAoiHeight.Focused)
            {
                SetAoiHeight(Convert.ToInt32(numericUpDownAoiHeight.Value));
            }
        }

        private void numericUpDownAoiLeft_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownAoiLeft.Focused)
            {
                SetAoiLeft(Convert.ToInt32(numericUpDownAoiLeft.Value));
            }
        }

        private void numericUpDownAoiTop_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownAoiTop.Focused)
            {
                SetAoiTop(Convert.ToInt32(numericUpDownAoiTop.Value));
            }
        }
    }
}
