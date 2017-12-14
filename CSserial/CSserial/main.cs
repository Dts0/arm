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
using System.Threading;

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
            serialPort1.Encoding = new UTF8Encoding();
            serialPort1.DataReceived += new SerialDataReceivedEventHandler(DataReceived);
            comboBox1.SelectedIndex = 0;
            comboBox2.SelectedIndex = 0;
            comboBox3.SelectedIndex = 0;
            comboBox4.SelectedIndex = 0;
            comboBox5.SelectedIndex = 0;
            comboBox6.SelectedIndex = 0;
            comboBox7.SelectedIndex = 0;
            comboBox8.SelectedIndex = 0;
            comboBox9.SelectedIndex = 0;
            comboBox10.SelectedIndex = 0;
            comboBox11.SelectedIndex = 0;
            comboBox12.SelectedIndex = 0;
        }
        void DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                string received;
                if (serialPort1.BytesToRead < 30)
                {
                    received = serialPort1.ReadLine();
                    received += "\n";
                }
                else
                {
                    received = serialPort1.ReadTo("}");
                    received += "}";
                }
                richTextBox1.Text += received;
                if (received.StartsWith("{"))
                {
                    received = received.TrimStart(new char[] { '{' }).TrimEnd(new char[] { '}' });
                    string[] receivedChars = received.Split(new char[] { ' ', '}', '{' });
                    //MessageBox.Show(received+"\n"+receivedChars.Length+"");


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

                    label20.Text = "已完成装载的物料数:  " + Convert.ToInt32(receivedChars[25]);
                    label21.Text = "当前在装载的层数:    " + Convert.ToInt32(receivedChars[26]);
                    label22.Text = "当前在装载的列数:    " + Convert.ToInt32(receivedChars[27]);
                    label23.Text = "当前在装载的物料编号:" + Convert.ToInt32(receivedChars[28]);

                    toolStripStatusLabel2.Text = "信息最后更新时间:" + DateTime.Now.ToString();

                }
                else if (received.Equals("OK,stop\n"))
                {
                    richTextBox1.AppendText("[" + DateTime.Now.ToString() + "]已停止\n");
                }
                else if (received.Equals("OK,reset\n"))
                {
                    richTextBox1.AppendText("[" + DateTime.Now.ToString() + "]已重启\n");
                }
                else if (received.Equals("OK,continue\n"))
                {
                    richTextBox1.AppendText("[" + DateTime.Now.ToString() + "]已继续运行\n");
                }
                else if (received.Equals("OK,but error\n"))
                {
                    richTextBox1.AppendText("ARM端已收到消息,但执行时发生错误\n");
                }
                else if (received.Equals("ERR,unknown cmd\n"))
                {
                    richTextBox1.AppendText("[" + DateTime.Now.ToString() + "]ARM端无法理解的指令\n");
                }
            }
            catch (Exception err)
            {
                MessageBox.Show("命令解析出错\n"+err.Data.ToString());
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
            button3.Enabled = true;
            button1.Enabled = false;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            send("CONT\0");
            button1.Enabled = true;
            button3.Enabled = false;
        }

        private void 保存SToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            string s="SET 01 ";
            if (checkBox1.Checked == true)
                s += "1\0";
            else s += "0\0";
            send(s);
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            string s = "SET 02 ";
            if (checkBox1.Checked == true)
                s += "1\0";
            else s += "0\0";
            send(s);
        }

        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {
            string s = "SET 03 ";
            if (checkBox1.Checked == true)
                s += "1\0";
            else s += "0\0";
            send(s);
        }

        private void checkBox4_CheckedChanged(object sender, EventArgs e)
        {
            string s = "SET 04 ";
            if (checkBox1.Checked == true)
                s += "1\0";
            else s += "0\0";
            send(s);
        }

        private void checkBox5_CheckedChanged(object sender, EventArgs e)
        {
            string s = "SET 05 ";
            if (checkBox1.Checked == true)
                s += "1\0";
            else s += "0\0";
            send(s);
        }

        private void checkBox6_CheckedChanged(object sender, EventArgs e)
        {
            string s = "SET 06 ";
            if (checkBox1.Checked == true)
                s += "1\0";
            else s += "0\0";
            send(s);
        }

        private void checkBox7_CheckedChanged(object sender, EventArgs e)
        {
            string s = "SET 07 ";
            if (checkBox1.Checked == true)
                s += "1\0";
            else s += "0\0";
            send(s);
        }

        private void checkBox13_CheckedChanged(object sender, EventArgs e)
        {
            string s = "SET 13 ";
            if (checkBox1.Checked == true)
                s += "1\0";
            else s += "0\0";
            send(s);
        }

        private void checkBox14_CheckedChanged(object sender, EventArgs e)
        {
            string s = "SET 14 ";
            if (checkBox1.Checked == true)
                s += "1\0";
            else s += "0\0";
            send(s);
        }

        private void checkBox15_CheckedChanged(object sender, EventArgs e)
        {
            string s = "SET 15 ";
            if (checkBox1.Checked == true)
                s += "1\0";
            else s += "0\0";
            send(s);
        }

        private void checkBox16_CheckedChanged(object sender, EventArgs e)
        {
            string s = "SET 16 ";
            if (checkBox1.Checked == true)
                s += "1\0";
            else s += "0\0";
            send(s);
        }

        private void checkBox17_CheckedChanged(object sender, EventArgs e)
        {
            string s = "SET 17 ";
            if (checkBox1.Checked == true)
                s += "1\0";
            else s += "0\0";
            send(s);
        }

        private void checkBox18_CheckedChanged(object sender, EventArgs e)
        {
            string s = "SET 18 ";
            if (checkBox1.Checked == true)
                s += "1\0";
            else s += "0\0";
            send(s);
        }

        private void 串口工具ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (Program.FormMain.serialPort1.IsOpen == true)
                (new Form1()).Show();
            else MessageBox.Show("未开启串口,无法使用串口工具");
        }
        
    }
}
