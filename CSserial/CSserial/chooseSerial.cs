using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace CSserial
{
    public partial class chooseSerial : Form
    {
        private void init()
        {
            comboBox_port.Items.Clear();
            comboBox_port.Items.AddRange(SerialPort.GetPortNames());
            if (comboBox_port.Items.Count > 0)
            {
                comboBox_port.SelectedIndex = 0;
            }
            else
            {
                comboBox_port.Text = "未检测到串口";
            }
        }
        public chooseSerial()
        {
            InitializeComponent();
            init();
        }

        private void button_Switch_Click(object sender, EventArgs e)
        {
            if (comboBox_port.Items.Count <= 0)
            {
                MessageBox.Show("未发现可用串口，请检查硬件设备");
                return;
            }
            if (Program.FormMain.serialPort1.IsOpen == false)
            {
                try
                {
                    Program.FormMain.serialPort1.PortName = comboBox_port.SelectedItem.ToString();
                    //开启串口
                    Program.FormMain.serialPort1.Open();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "未能成功开启串口", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
            }
            Program.FormMain.Enabled = true;
            Program.FormMain.disableBtn2();
            Program.FormMain.enableItems2();
            Program.FormMain.Focus();
            this.Hide();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            init();
        }

        private void chooseSerial_Load(object sender, EventArgs e)
        {
            Program.FormMain.Enabled = false;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Program.FormMain.Enabled = true;
            Program.FormMain.Focus();
            this.Hide();
        }
    }
}
