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
    public partial class CameraChoose : Form
    {
        public delegate void AddCameraListItem();
        public AddCameraListItem cameraDelegate;

        private Int32 m_CameraIdToOpen;
        private Int32 m_DeviceIdToOpen;

        public Int32 CameraID
        {
            get
            {
                return m_CameraIdToOpen;
            }
        }

        public Int32 DeviceID
        {
            get
            {
                return m_DeviceIdToOpen;
            }
        }

        public CameraChoose()
        {
            InitializeComponent();

            cameraDelegate = new AddCameraListItem(UpdateCameraList);
            listViewCamera.FullRowSelect = true;
            
            uEye.Info.Camera.EventNewDevice += onCameraEvent;
            uEye.Info.Camera.EventDeviceRemoved += onCameraEvent;
            
            InitCameraList();
            UpdateCameraList();
        }

        private void onCameraEvent(object sender, EventArgs e)
        {
            //InitCameraList();
            this.Invoke(cameraDelegate);
        }

        private void UpdateCameraList()
        {
            listViewCamera.Items.Clear();

            uEye.Types.CameraInformation[] cameraList;
            uEye.Info.Camera.GetCameraList(out cameraList);

            foreach (uEye.Types.CameraInformation info in cameraList)
            {
                ListViewItem item = new ListViewItem();
                item.Text = info.InUse ? "No" : "Yes";
                item.ImageIndex = info.InUse ? 1 : 0;

                item.SubItems.Add(info.CameraID.ToString());
                item.SubItems.Add(info.DeviceID.ToString());
                item.SubItems.Add(info.Model);
                item.SubItems.Add(info.SerialNumber);

                listViewCamera.Items.Add(item);
            }

            if (cameraList.Count() != 0)
            {
                listViewCamera.Items[0].Selected = true;
                buttonOk.Enabled = true;
            }
            else
            {
                // no camera disable button
                buttonOk.Enabled = false;
            }

            // auto resize
            listViewCamera.AutoResizeColumn(3, ColumnHeaderAutoResizeStyle.ColumnContent);
            listViewCamera.AutoResizeColumn(4, ColumnHeaderAutoResizeStyle.ColumnContent);
            listViewCamera.Columns[listViewCamera.Columns.Count - 1].Width = -2;
        }

        private void InitCameraList()
        {
            listViewCamera.HideSelection = false;
        }

        private void listViewCamera_DoubleClick(object sender, EventArgs e)
        {
            if (listViewCamera.SelectedItems.Count != 0)
            {
                m_CameraIdToOpen = Convert.ToInt32(listViewCamera.SelectedItems[0].SubItems[1].Text);
                m_DeviceIdToOpen = Convert.ToInt32(listViewCamera.SelectedItems[0].SubItems[2].Text);
                this.DialogResult = DialogResult.OK;
                Close();
            }
            else
            {
                MessageBox.Show("No camera selected...");
            }
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            Close();
        }

        private void CameraChoose_FormClosing(object sender, FormClosingEventArgs e)
        {
            uEye.Info.Camera.EventNewDevice -= onCameraEvent;
            uEye.Info.Camera.EventDeviceRemoved -= onCameraEvent;
        }
    }
}
