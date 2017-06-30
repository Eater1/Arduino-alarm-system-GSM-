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

namespace ArduinoControlPanel
{
    public partial class Form2 : Form
    {
        private string in_data;
        public Form2()
        {
            InitializeComponent();
            Off_btn.Enabled = false;
            On_btn.Enabled = false;

            //baud rates Доступный
            int[] baudrates = {
                                  4800,
                                  9600,
                                  14400,
                                  19200,
                                  28800,
                                  38400,
                                  57600,
                                  115200
                              };
            foreach (int i in baudrates)
            {
                baudrate_combobox.Items.Add(Convert.ToString(i));
            }

            //доступный COM ports
            SerialPort tmp;
            foreach (string str in SerialPort.GetPortNames())
            {
                tmp = new SerialPort(str);
                if (tmp.IsOpen == false)
                    port_combobox.Items.Add(str);
            }
        }

        private void On_btn_Click(object sender, EventArgs e)
        {
                try
                {
                    //отправка сообщения Arduino
                    // serialPort.Write("1");
                    int n = msg_listbox.Items.Add("On");
                    msg_listbox.SelectedIndex = n;
                    msg_listbox.ClearSelected();
                    //message_textbox.Text = "";
                    serialPort.DataReceived += serialPort_DataReceived;

                    if (serialPort.PortName != null)
                    {
                        lblConnectionStatus.Text = "Connected at " + serialPort.PortName;
                    }

                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

        }

        private void Off_btn_Click(object sender, EventArgs e)
        {
            try
            {
                //отправка сообщения Arduino
                serialPort.Write("2");
                int n = msg_listbox.Items.Add("Off");
                msg_listbox.SelectedIndex = n;
                msg_listbox.ClearSelected();
                //message_textbox.Text = "";
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                if (textBox1.Text == "")
                {
                    MessageBox.Show("Введите номер телефона");
                }
                else
                {
                    serialPort.Write("7");
                    //byte[] data = CommandMessage(textBox1.Text);            
                    ////byte[] data = CommandMessage("TurnOnLights");
                    //serialPort.Write(data, 0, data.Length);  //the serial port that i assume you have set up
                    ////This will send "~TurnOnLights~" to the arduino
                    byte[] data = CommandMessage("AT + CMGS = \"" + textBox1.Text + "\"");
                    serialPort.Write(data, 0, data.Length);  //the serial port that i assume you have set up
                    //This will send "~TurnOnLights~" to the arduino
                }
            }
            else
            {
                MessageBox.Show("Нужно открыть порт");
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                if (txtMessage.Text == "")
                {
                    MessageBox.Show("Введите текст оповещения");
                }
                else
                {
                    serialPort.Write("8");
                    // byte[] data = CommandMessage(textBox1.Text);
                    byte[] data = CommandMessage(txtMessage.Text);
                    serialPort.Write(data, 0, data.Length);  //the serial port that i assume you have set up
                    //This will send "~TurnOnLights~" to the arduino
                }
            }
            else
            {
                MessageBox.Show("Нужно открыть порт");
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Write("6");
                serialPort.WriteLine(trackBar1.Value.ToString());
            }
            else
            {
                MessageBox.Show("Нужно открыть порт");
            }
        }

        private void baudrate_combobox_SelectedIndexChanged(object sender, EventArgs e)
        {
            //выбор baudrate
            serialPort.BaudRate = Convert.ToInt16(baudrate_combobox.Text);

            //Включить кнопку отправки?
            if (port_combobox.Text.CompareTo("") != 0)
                Off_btn.Enabled = true;
            On_btn.Enabled = true;

        }

        private void port_combobox_SelectedIndexChanged(object sender, EventArgs e)
        {
            serialPort.PortName = port_combobox.Text;

            try
            {
                //open serial port
                serialPort.Open();
                //port_combobox.Enabled = false;

                //Включить кнопку отправки??
                if (baudrate_combobox.Text.CompareTo("") != 0)
                    Off_btn.Enabled = true;
                On_btn.Enabled = true;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                port_combobox.Text = "";
            }
        }

        public byte[] CommandMessage(string CMD)
        {
            //Это принимает строку и преобразует ее в массив байтов, готовый к отправке через последовательный

            byte[] message = new byte[CMD.Length + 2]; //Add 2 for the terminal chars
            message[0] = BitConverter.GetBytes('~')[0]; //Add Start terminal char
            for (int i = 1; i < message.Length - 1; i++)
            {
                //loop through command to be sent (Пересылка по команде для отправки)
                message[i] = BitConverter.GetBytes(CMD[i - 1])[0];
            }
            message[message.Length - 1] = BitConverter.GetBytes('~')[0]; //Add end terminal char

            return message;
        }

        public byte[] CommandMessagex(string CMDx)
        {
            //Это принимает строку и преобразует ее в массив байтов, готовый к отправке через последовательный

            byte[] messagex = new byte[CMDx.Length + 2]; //Add 2 for the terminal chars
            messagex[0] = BitConverter.GetBytes('~')[0]; //Add Start terminal char
            for (int i = 1; i < messagex.Length - 1; i++)
            {
                //loop through command to be sent (Пересылка по команде для отправки)
                messagex[i] = BitConverter.GetBytes(CMDx[i - 1])[0];
            }
            messagex[messagex.Length - 1] = BitConverter.GetBytes('~')[0]; //Add end terminal char

            return messagex;
        }

        void serialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            in_data = serialPort.ReadLine();
            this.Invoke(new EventHandler(displaydata_event));
        }

        private void displaydata_event(object sender, EventArgs e)
        {

            //if (in_data == "Allert\r")
            //{
            //   // byte[] data = CommandMessage(textBox1.Text);
            //    byte[] data = CommandMessage("AT + CMGS = \""+textBox1.Text+"\"");
            //    serialPort.Write(data, 0, data.Length);  //the serial port that i assume you have set up
            //    //This will send "~TurnOnLights~" to the arduino
            //}

            //if (in_data == "Sending SMS...\r")
            //{
            //    // byte[] data = CommandMessage(textBox1.Text);
            //    byte[] data = CommandMessage(txtMessage.Text);
            //    serialPort.Write(data, 0, data.Length);  //the serial port that i assume you have set up
            //    //This will send "~TurnOnLights~" to the arduino
            //}

            //textBox1.AppendText(in_data + "\n");
            int n = msg_listbox.Items.Add(in_data);
            msg_listbox.SelectedIndex = n;
            msg_listbox.ClearSelected();

        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            // Правильными символами считаются цифры,
            // Остальные символы запрещены.
            // Чтобы запрещенный символ не отображался 
            // в поле редактирования,присвоим 
            // значение true свойству Handled параметра e

            if (!Char.IsDigit(e.KeyChar) && e.KeyChar != 8 && e.KeyChar != 43)
            {
                // остальные символы запрещены
                e.Handled = true;
            }
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {

                label5.Text = "Длительность взвода =" + ((trackBar1.Value) / 1000).ToString();
            }
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void lblConnectionStatus_Click(object sender, EventArgs e)
        {

        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Write("9");
            }
            else
            {
                MessageBox.Show("Нужно открыть порт");
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Write("0");
            }
            else
            {
                MessageBox.Show("Нужно открыть порт");
            }
        }
    }
}
