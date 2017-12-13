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
    
        public Form1()
        {
            InitializeComponent();
            Program.FormMain.serialPort1.DataReceived += new SerialDataReceivedEventHandler(Com_DataReceived);
        }
        private void Com_DataReceived(object sender, SerialDataReceivedEventArgs e)
          {
              //开辟接收缓冲区
              byte[] ReDatas = new byte[Program.FormMain.serialPort1.BytesToRead];
              //从串口读取数据
              Program.FormMain.serialPort1.Read(ReDatas, 0, ReDatas.Length);
              //实现数据的解码与显示,utf-8编码
              richTextBox_receive.AppendText(new UTF8Encoding().GetString(ReDatas));
          }

        

        private void button_Send_Click(object sender, EventArgs e)
        {
            byte[] sendData = null;
            sendData = Encoding.UTF8.GetBytes(richTextBox_send.Text+"\0");

            if (Program.FormMain.serialPort1.IsOpen)
             {
                 try
                {
                     //将消息传递给串口
                    Program.FormMain.serialPort1.Write(sendData, 0, sendData.Length);
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


    }
}
