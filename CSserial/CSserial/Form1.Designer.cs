namespace CSserial
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.richTextBox_receive = new System.Windows.Forms.RichTextBox();
            this.richTextBox_send = new System.Windows.Forms.RichTextBox();
            this.button_Send = new System.Windows.Forms.Button();
            this.button_clear = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // richTextBox_receive
            // 
            this.richTextBox_receive.Location = new System.Drawing.Point(12, 51);
            this.richTextBox_receive.Name = "richTextBox_receive";
            this.richTextBox_receive.Size = new System.Drawing.Size(295, 176);
            this.richTextBox_receive.TabIndex = 2;
            this.richTextBox_receive.Text = "";
            // 
            // richTextBox_send
            // 
            this.richTextBox_send.Location = new System.Drawing.Point(350, 51);
            this.richTextBox_send.Name = "richTextBox_send";
            this.richTextBox_send.Size = new System.Drawing.Size(295, 176);
            this.richTextBox_send.TabIndex = 3;
            this.richTextBox_send.Text = "";
            // 
            // button_Send
            // 
            this.button_Send.Location = new System.Drawing.Point(453, 233);
            this.button_Send.Name = "button_Send";
            this.button_Send.Size = new System.Drawing.Size(75, 23);
            this.button_Send.TabIndex = 4;
            this.button_Send.Text = "发送";
            this.button_Send.UseVisualStyleBackColor = true;
            this.button_Send.Click += new System.EventHandler(this.button_Send_Click);
            // 
            // button_clear
            // 
            this.button_clear.Location = new System.Drawing.Point(106, 233);
            this.button_clear.Name = "button_clear";
            this.button_clear.Size = new System.Drawing.Size(75, 23);
            this.button_clear.TabIndex = 5;
            this.button_clear.Text = "清空接收区";
            this.button_clear.UseVisualStyleBackColor = true;
            this.button_clear.Click += new System.EventHandler(this.button_clear_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(689, 317);
            this.Controls.Add(this.button_clear);
            this.Controls.Add(this.button_Send);
            this.Controls.Add(this.richTextBox_send);
            this.Controls.Add(this.richTextBox_receive);
            this.Name = "Form1";
            this.Text = "串口工具";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.RichTextBox richTextBox_receive;
        private System.Windows.Forms.RichTextBox richTextBox_send;
        private System.Windows.Forms.Button button_Send;
        private System.Windows.Forms.Button button_clear;
    }
}

