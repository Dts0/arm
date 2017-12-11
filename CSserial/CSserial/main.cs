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

    public partial class main : Form
    {
        private byte[] sendData = null;
        private chooseSerial FormChooseSerial;
        private void send(string msg)
        {
            sendData = Encoding.UTF8.GetBytes(msg);

            if (serialPort1.IsOpen)
            {
                try
                {
                    //将消息传递给串口
                    serialPort1.Write(sendData, 0, sendData.Length);
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
        public main()
        {
            InitializeComponent();
            disableItems();
            disableItems2();
            FormChooseSerial = new chooseSerial();
            FormChooseSerial.Hide();
            serialPort1.NewLine="\n";
            serialPort1.DataReceived += new SerialDataReceivedEventHandler(DataReceived);
            richTextBox1.Enabled = false;
        }
        void DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            //开辟接收缓冲区
            byte[] ReDatas = new byte[serialPort1.BytesToRead];
            //从串口读取数据
            serialPort1.Read(ReDatas, 0, ReDatas.Length);
            //实现数据的解码与显示,utf-8编码
            string received = new UTF8Encoding().GetString(ReDatas);
            if (received.StartsWith("{"))
            {
                string[] receivedChars = received.Split(new char[] { ' ', '#', '}' });
                comboBox1.SelectedIndex = Convert.ToInt32(receivedChars[0]);
                comboBox2.SelectedIndex = Convert.ToInt32(receivedChars[1]);
                comboBox6.SelectedIndex = Convert.ToInt32(receivedChars[2]);
                comboBox3.SelectedIndex = Convert.ToInt32(receivedChars[3]);
                comboBox7.SelectedIndex = Convert.ToInt32(receivedChars[4]);
                comboBox8.SelectedIndex = Convert.ToInt32(receivedChars[5]);
                comboBox9.SelectedIndex = Convert.ToInt32(receivedChars[6]);
                comboBox10.SelectedIndex = Convert.ToInt32(receivedChars[7]);
                comboBox11.SelectedIndex = Convert.ToInt32(receivedChars[8]);
                comboBox4.SelectedIndex = Convert.ToInt32(receivedChars[9]);
                comboBox5.SelectedIndex = Convert.ToInt32(receivedChars[10]);
                comboBox12.SelectedIndex = Convert.ToInt32(receivedChars[11]);

                if (Convert.ToInt32(receivedChars[12]) == 1)
                    checkBox1.Checked = true;
                else checkBox1.Checked = false;
                if (Convert.ToInt32(receivedChars[13]) == 1)
                    checkBox2.Checked = true;
                else checkBox2.Checked = false;
                if (Convert.ToInt32(receivedChars[14]) == 1)
                    checkBox3.Checked = true;
                else checkBox3.Checked = false;
                if (Convert.ToInt32(receivedChars[15]) == 1)
                    checkBox4.Checked = true;
                else checkBox4.Checked = false;
                if (Convert.ToInt32(receivedChars[16]) == 1)
                    checkBox5.Checked = true;
                else checkBox5.Checked = false;
                if (Convert.ToInt32(receivedChars[17]) == 1)
                    checkBox6.Checked = true;
                else checkBox6.Checked = false;
                if (Convert.ToInt32(receivedChars[18]) == 1)
                    checkBox7.Checked = true;
                else checkBox7.Checked = false;

                if (Convert.ToInt32(receivedChars[19]) == 1)
                    checkBox13.Checked = true;
                else checkBox13.Checked = false;
                if (Convert.ToInt32(receivedChars[20]) == 1)
                    checkBox16.Checked = true;
                else checkBox16.Checked = false;
                if (Convert.ToInt32(receivedChars[21]) == 1)
                    checkBox15.Checked = true;
                else checkBox15.Checked = false;
                if (Convert.ToInt32(receivedChars[22]) == 1)
                    checkBox18.Checked = true;
                else checkBox18.Checked = false;
                if (Convert.ToInt32(receivedChars[23]) == 1)
                    checkBox14.Checked = true;
                else checkBox14.Checked = false;
                if (Convert.ToInt32(receivedChars[24]) == 1)
                    checkBox17.Checked = true;
                else checkBox17.Checked = false;
            }
            else if (received.Equals("OK,stop\n"))
            {
                richTextBox1.AppendText("已停止\n");
            }
            else if (received.Equals("OK,reset\n"))
            {
                richTextBox1.AppendText("已重启\n");
            }
            else if (received.Equals("OK,continue\n"))
            {
                richTextBox1.AppendText("已继续运行\n");
            }
            else if (received.Equals("OK,but error\n"))
            {
                richTextBox1.AppendText("ARM端已收到消息,但执行时发生错误\n");
            }
            else if (received.Equals("ERR,unknown cmd\n"))
            {
                richTextBox1.AppendText("ARM端无法理解的指令\n");
            }
        }
        private void button2_Click(object sender, EventArgs e)
        {
            FormChooseSerial.Show();
        }

        private void main_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort1.IsOpen == true)
                serialPort1.Close();
        }
        public void enableItems()
        {         
            this.comboBox1.Enabled = true;
            this.comboBox2.Enabled = true;
            this.comboBox3.Enabled = true;
            this.comboBox4.Enabled = true;
            this.comboBox5.Enabled = true;
            this.comboBox6.Enabled = true;
            this.comboBox7.Enabled = true;
            this.comboBox8.Enabled = true;
            this.comboBox9.Enabled = true;
            this.comboBox10.Enabled = true;
            this.comboBox11.Enabled = true;
            this.comboBox12.Enabled = true;
        }
        public void enableItems2()
        {
            this.button1.Enabled = true;
            this.checkBox13.Enabled = true;
            this.checkBox14.Enabled = true;
            this.checkBox15.Enabled = true;
            this.checkBox16.Enabled = true;
            this.checkBox17.Enabled = true;
            this.checkBox18.Enabled = true;
            //this.checkBox19.Enabled = true;
            this.checkBox1.Enabled = true;
            this.checkBox2.Enabled = true;
            this.checkBox3.Enabled = true;
            this.checkBox4.Enabled = true;
            this.checkBox5.Enabled = true;
            this.checkBox6.Enabled = true;
            this.checkBox7.Enabled = true;
        }
        public void disableBtn2()
        {
            this.button2.Enabled = false;
            this.button3.Enabled = false;
        }
        public void disableItems()
        {           
            this.comboBox1.Enabled = false;
            this.comboBox2.Enabled = false;
            this.comboBox3.Enabled = false;
            this.comboBox4.Enabled = false;
            this.comboBox5.Enabled = false;
            this.comboBox6.Enabled = false;
            this.comboBox7.Enabled = false;
            this.comboBox8.Enabled = false;
            this.comboBox9.Enabled = false;
            this.comboBox10.Enabled = false;
            this.comboBox11.Enabled = false;
            this.comboBox12.Enabled = false;
        }
        public void disableItems2()
        {
            this.button1.Enabled = false;
            this.button3.Enabled = false;
            this.checkBox13.Enabled = false;
            this.checkBox14.Enabled = false;
            this.checkBox15.Enabled = false;
            this.checkBox16.Enabled = false;
            this.checkBox17.Enabled = false;
            this.checkBox18.Enabled = false;
           // this.checkBox19.Enabled = false;
            this.checkBox1.Enabled = false;
            this.checkBox2.Enabled = false;
            this.checkBox3.Enabled = false;
            this.checkBox4.Enabled = false;
            this.checkBox5.Enabled = false;
            this.checkBox6.Enabled = false;
            this.checkBox7.Enabled = false;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            send("STOP\0");       
        }

        private void button3_Click(object sender, EventArgs e)
        {
            send("CONT\0");
        }
        
    }
}
