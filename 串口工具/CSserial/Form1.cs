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
    public partial class Form1 : Form
    {
        private SerialPort com = new SerialPort();
        private int baudRate = 9600;
        private Parity parity=Parity.None;
        private int dataBits=8;
        private StopBits stopBits = StopBits.One;
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
            //向com.DataReceived（是一个事件）注册一个方法Com_DataReceived，当端口类接收到信息时时会自动调用Com_DataReceived方法
              com.DataReceived += new SerialDataReceivedEventHandler(Com_DataReceived);
        }
        public Form1()
        {
            InitializeComponent();
            init();
        }
        private void Com_DataReceived(object sender, SerialDataReceivedEventArgs e)
          {
              //开辟接收缓冲区
              byte[] ReDatas = new byte[com.BytesToRead];
              //从串口读取数据
              com.Read(ReDatas, 0, ReDatas.Length);
              //实现数据的解码与显示,utf-8编码
              richTextBox_receive.AppendText(new UTF8Encoding().GetString(ReDatas));
          }

        private void button1_Click(object sender, EventArgs e)
        {
            if (comboBox_port.Items.Count <= 0)
             {
                 MessageBox.Show("未发现可用串口，请检查硬件设备");
                 return;
             }
 
             if (com.IsOpen == false)
            {
                 //设置串口相关属性
                 com.PortName = comboBox_port.SelectedItem.ToString();
                 com.BaudRate = baudRate;
                 com.Parity = parity;
                 com.DataBits = dataBits;
                 com.StopBits = stopBits;
                 try
                 {
                     //开启串口
                     com.Open();
                     button_Send.Enabled = true;
                 }
                 catch (Exception ex)
                 {
                     MessageBox.Show(ex.Message, "未能成功开启串口", MessageBoxButtons.OK, MessageBoxIcon.Error);
                     return;
                 }
                 button_Switch.Text = "关闭串口";
             }
             else
             {
                 try
                {
                     //关闭串口
                     com.Close();
                     button_Send.Enabled = false;
                 }
                 catch (Exception ex)
                 {
                     MessageBox.Show(ex.Message, "串口关闭错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                 }
                 button_Switch.Text = "开启串口";
             }
 
             comboBox_port.Enabled = !com.IsOpen;
        }

        private void button_Send_Click(object sender, EventArgs e)
        {
            byte[] sendData = null;
            sendData = Encoding.UTF8.GetBytes(richTextBox_send.Text+"\0");

            if (com.IsOpen)
             {
                 try
                {
                     //将消息传递给串口
                     com.Write(sendData, 0, sendData.Length);
                     return;
                 }
                 catch (Exception ex)
                 {
                     MessageBox.Show(ex.Message, "发送失败", MessageBoxButtons.OK, MessageBoxIcon.Error);
                 }
             }
             else
             {
                 MessageBox.Show("串口未开启", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
             }
        }

        private void button_clear_Click(object sender, EventArgs e)
        {
            richTextBox_receive.Text = "";
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            init();
        }

    }
}
