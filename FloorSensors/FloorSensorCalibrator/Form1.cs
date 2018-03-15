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

namespace FloorSensorCalibrator
{
    //data: 100 + COMMAND + 28 Bytes data
    public partial class Form1 : Form
    {
        const byte START_BYTE = 100;
        const byte SAVE = 1;
        const byte SAVE_BACKUP = 2;
        const byte READ = 3;
        const byte READ_BACKUP = 4;
        const byte SET_RED = 5;
        const byte SET_BLUE = 6;
        const byte SET_WHITE = 7;
        const byte FRONT_RED = 8;
        const byte FRONT_BLUE = 10;
        const byte FRONT_WHITE = 12;
        const byte BACK_RED = 13;
        const byte BACK_BLUE = 14;
        const byte BACK_WHITE = 15;

        SerialPort port = new SerialPort();
        String indata;
        Button[] prox_state = new Button[6];
        Button[] front_state = new Button[7];
        Button[] back_state = new Button[7];
        Label[] lbl_current_front = new Label[7];
        Label[] lbl_current_back = new Label[7];
        Label[] lbl_upper_front = new Label[7];
        Label[] lbl_lower_front = new Label[7];
        Label[] lbl_upper_back = new Label[7];
        Label[] lbl_lower_back = new Label[7];
        TrackBar[] tb_front = new TrackBar[7];
        TrackBar[] tb_back = new TrackBar[7];

        byte[] data = new byte[30];
        struct Limit
        {
            public UInt16 top;
            public UInt16 bottom;
        }
        struct ColorValLimit
        {
            public Limit red;
            public Limit blue;
            public Limit white;
        }
        struct Threshold
        {
            public UInt16 upper;
            public UInt16 lower;
        }
        struct ColorThreshold
        {
            public Threshold red;
            public Threshold blue;
            public Threshold white;
        }
        ColorValLimit[] front_limit = new ColorValLimit[7];
        ColorValLimit[] back_limit = new ColorValLimit[7];
        ColorThreshold[] front_thres = new ColorThreshold[7];
        ColorThreshold[] back_thres = new ColorThreshold[7];


        public Form1()
        {
            InitializeComponent();
            port.BaudRate = 250000;
            port.Parity = Parity.None;
            port.DataBits = 8;
            port.StopBits = StopBits.One;
            port.DtrEnable = true;
            port.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
            prox_state[0] = btnFL;
            prox_state[1] = btnFC;
            prox_state[2] = btnFR;
            prox_state[3] = btnSF;
            prox_state[4] = btnSM;
            prox_state[5] = btnSB;

            front_state[0] = btnF0;
            front_state[1] = btnF1;
            front_state[2] = btnF2;
            front_state[3] = btnF3;
            front_state[4] = btnF4;
            front_state[5] = btnF5;
            front_state[6] = btnF6;

            back_state[0] = btnB0;
            back_state[1] = btnB1;
            back_state[2] = btnB2;
            back_state[3] = btnB3;
            back_state[4] = btnB4;
            back_state[5] = btnB5;
            back_state[6] = btnB6;

            lbl_current_front[0] = lblCurrentF0;
            lbl_current_front[1] = lblCurrentF1;
            lbl_current_front[2] = lblCurrentF2;
            lbl_current_front[3] = lblCurrentF3;
            lbl_current_front[4] = lblCurrentF4;
            lbl_current_front[5] = lblCurrentF5;
            lbl_current_front[6] = lblCurrentF6;

            lbl_current_back[0] = lblCurrentB0;
            lbl_current_back[1] = lblCurrentB1;
            lbl_current_back[2] = lblCurrentB2;
            lbl_current_back[3] = lblCurrentB3;
            lbl_current_back[4] = lblCurrentB4;
            lbl_current_back[5] = lblCurrentB5;
            lbl_current_back[6] = lblCurrentB6;

            lbl_upper_front[0] = lblUpperF0;
            lbl_upper_front[1] = lblUpperF1;
            lbl_upper_front[2] = lblUpperF2;
            lbl_upper_front[3] = lblUpperF3;
            lbl_upper_front[4] = lblUpperF4;
            lbl_upper_front[5] = lblUpperF5;
            lbl_upper_front[6] = lblUpperF6;

            lbl_upper_back[0] = lblUpperF0;
            lbl_upper_back[1] = lblUpperB1;
            lbl_upper_back[2] = lblUpperB2;
            lbl_upper_back[3] = lblUpperB3;
            lbl_upper_back[4] = lblUpperB4;
            lbl_upper_back[5] = lblUpperB5;
            lbl_upper_back[6] = lblUpperB6;

            lbl_lower_front[0] = lblLowerF0;
            lbl_lower_front[1] = lblLowerF1;
            lbl_lower_front[2] = lblLowerF2;
            lbl_lower_front[3] = lblLowerF3;
            lbl_lower_front[4] = lblLowerF4;
            lbl_lower_front[5] = lblLowerF5;
            lbl_lower_front[6] = lblLowerF6;

            lbl_lower_back[0] = lblLowerB0;
            lbl_lower_back[1] = lblLowerB1;
            lbl_lower_back[2] = lblLowerB2;
            lbl_lower_back[3] = lblLowerB3;
            lbl_lower_back[4] = lblLowerB4;
            lbl_lower_back[5] = lblLowerB5;
            lbl_lower_back[6] = lblLowerB6;

            tb_front[0] = tbF0;
            tb_front[1] = tbF1;
            tb_front[2] = tbF2;
            tb_front[3] = tbF3;
            tb_front[4] = tbF4;
            tb_front[5] = tbF5;
            tb_front[6] = tbF6;

            tb_back[0] = tbB0;
            tb_back[1] = tbB1;
            tb_back[2] = tbB2;
            tb_back[3] = tbB3;
            tb_back[4] = tbB4;
            tb_back[5] = tbB5;
            tb_back[6] = tbB6;

            data[0] = START_BYTE;
        }
        private void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                Console.Write("\n\n[" + DateTime.Now.ToString("hh:mm:ss.fff") + "] Incomming data: ");
                indata = port.ReadLine();
                Console.Write(indata);
            }
            catch (Exception error)
            {
                Console.WriteLine("\nError reading bytes! " + error.ToString());
                Console.WriteLine("\n");
            }
        }
        private void btnClose_Click(object sender, EventArgs e)
        {
            port.Close();
            cboComName.Text = "CLOSED";
        }
        private void timer1_Tick(object sender, EventArgs e)
        {
            if (!port.IsOpen)
                return;
            if (indata == null || indata == "")
                return;
            //copy version
            String _indata = indata;
            indata = "";
            //Console.WriteLine(_indata);
            if (_indata.IndexOf('=') < 0)
            {
                if (_indata.Length > 1)
                    lblInfo.Text = _indata;
                return;
            }
            String command = substring(_indata, 0, _indata.IndexOf('='));
            _indata = substring(_indata, _indata.IndexOf('{') + 1, _indata.IndexOf('}'));
            if (command == "prox")
            {
                int[] val = new int[6];
                for (int i = 0; i < 6; i++)
                {
                    val[i] = int.Parse(substring(_indata, 0, _indata.IndexOf(',')));
                    _indata = substring(_indata, _indata.IndexOf(',') + 1, _indata.Length);
                    //Console.WriteLine(val[i]);
                    if (val[i] == 0)
                        prox_state[i].BackColor = Color.Gray;
                    else
                        prox_state[i].BackColor = Color.Yellow;
                }
            }
            //update states
            if (command == "fs")
            {
                int[] val = new int[7];
                for (int i = 0; i < 7; i++)
                {
                    val[i] = int.Parse(substring(_indata, 0, _indata.IndexOf(',')));
                    _indata = substring(_indata, _indata.IndexOf(',') + 1, _indata.Length);
                    //Console.WriteLine(val[i]);
                    if (val[i] == 0)
                        front_state[i].BackColor = Color.Gray;
                    else
                        front_state[i].BackColor = Color.Yellow;
                }
            }
            if (command == "bs")
            {
                int[] val = new int[7];
                for (int i = 0; i < 7; i++)
                {
                    val[i] = int.Parse(substring(_indata, 0, _indata.IndexOf(',')));
                    _indata = substring(_indata, _indata.IndexOf(',') + 1, _indata.Length);
                    //Console.WriteLine(val[i]);
                    if (val[i] == 0)
                        back_state[i].BackColor = Color.Gray;
                    else
                        back_state[i].BackColor = Color.Yellow;
                }
            }
            //update values
            if (command == "fv")
            {
                int[] val = new int[7];
                for (int i = 0; i < 7; i++)
                {
                    val[i] = int.Parse(substring(_indata, 0, _indata.IndexOf(',')));
                    _indata = substring(_indata, _indata.IndexOf(',') + 1, _indata.Length);
                    //Console.WriteLine(val[i]);
                    lbl_current_front[i].Text = val[i].ToString();
                    tb_front[i].Value = val[i];
                }
            }
            if (command == "bv")
            {
                int[] val = new int[7];
                for (int i = 0; i < 7; i++)
                {
                    val[i] = int.Parse(substring(_indata, 0, _indata.IndexOf(',')));
                    _indata = substring(_indata, _indata.IndexOf(',') + 1, _indata.Length);
                    //Console.WriteLine(val[i]);
                    lbl_current_back[i].Text = val[i].ToString();
                    tb_back[i].Value = val[i];
                }
            }

        }
        private void cboComName_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (port.IsOpen)
            {
                port.Close();
            }
            try
            {
                port.PortName = cboComName.Text;
                port.Open();
                port.Write("00000000");
            }
            catch (Exception exp)
            {
                Console.WriteLine(exp.ToString());
                MessageBox.Show("Không mở được " + port.PortName + "! Kiểm tra kết nối?", "LỖI", MessageBoxButtons.OK, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button1);
                cboComName.Text = "Chưa kết nối";
            }
        }
        string substring(string data, int start, int end)
        {
            int len = end - start;
            if ((len <= 0) || (start + len > data.Length) || (start < 0) || (end < 0))
                return "";
            data = data.Substring(start, len);
            return data;
        }
        private void cboComName_DropDown(object sender, EventArgs e)
        {
            cboComName.Items.Clear();
            string[] Ports = SerialPort.GetPortNames();
            foreach (string portInActive in Ports)
            {
                cboComName.Items.Add(portInActive);
            }
            cboComName.Sorted = true;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            //red line
            for (int i = 0; i < 7; i++)
            {
                front_limit[i].red.bottom = UInt16.Parse(lbl_current_front[i].Text);

                int error = front_limit[i].red.top - front_limit[i].red.bottom;
                int average = (front_limit[i].red.top + front_limit[i].red.bottom) / 2;
                int tolerance = error / 10;

                front_thres[i].red.upper = (UInt16)(average + tolerance);
                front_thres[i].red.lower = (UInt16)(average - tolerance);

                lbl_upper_front[i].Text = front_thres[i].red.upper.ToString();
                lbl_lower_front[i].Text = front_thres[i].red.lower.ToString();
            }

            data[1] = FRONT_RED;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(front_thres[i].red.upper)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(front_thres[i].red.lower)), 0, data, 16 + 2 * i, 2);
            }
            writeBytes(data);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            //red background
            for (int i = 0; i < 7; i++)
            {
                front_limit[i].red.top = UInt16.Parse(lbl_current_front[i].Text);

                int error = front_limit[i].red.top - front_limit[i].red.bottom;
                int average = (front_limit[i].red.top + front_limit[i].red.bottom) / 2;
                int tolerance = error / 10;

                front_thres[i].red.upper = (UInt16)(average + tolerance);
                front_thres[i].red.lower = (UInt16)(average - tolerance);

                lbl_upper_front[i].Text = front_thres[i].red.upper.ToString();
                lbl_lower_front[i].Text = front_thres[i].red.lower.ToString();
            }

            data[1] = FRONT_RED;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(front_thres[i].red.upper)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(front_thres[i].red.lower)), 0, data, 16 + 2 * i, 2);
            }
            writeBytes(data);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            //blue line
            for (int i = 0; i < 7; i++)
            {
                front_limit[i].blue.bottom = UInt16.Parse(lbl_current_front[i].Text);

                int error = front_limit[i].blue.top - front_limit[i].blue.bottom;
                int average = (front_limit[i].blue.top + front_limit[i].blue.bottom) / 2;
                int tolerance = error / 10;

                front_thres[i].blue.upper = (UInt16)(average + tolerance);
                front_thres[i].blue.lower = (UInt16)(average - tolerance);

                lbl_upper_front[i].Text = front_thres[i].blue.upper.ToString();
                lbl_lower_front[i].Text = front_thres[i].blue.lower.ToString();
            }
            data[1] = FRONT_BLUE;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(front_thres[i].blue.upper)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(front_thres[i].blue.lower)), 0, data, 16 + 2 * i, 2);
            }
            writeBytes(data);

        }

        private void button6_Click(object sender, EventArgs e)
        {
            //blue background
            for (int i = 0; i < 7; i++)
            {
                front_limit[i].blue.top = UInt16.Parse(lbl_current_front[i].Text);

                int error = front_limit[i].blue.top - front_limit[i].blue.bottom;
                int average = (front_limit[i].blue.top + front_limit[i].blue.bottom) / 2;
                int tolerance = error / 10;

                front_thres[i].blue.upper = (UInt16)(average + tolerance);
                front_thres[i].blue.lower = (UInt16)(average - tolerance);

                lbl_upper_front[i].Text = front_thres[i].blue.upper.ToString();
                lbl_lower_front[i].Text = front_thres[i].blue.lower.ToString();
            }
            data[1] = FRONT_BLUE;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(front_thres[i].blue.upper)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(front_thres[i].blue.lower)), 0, data, 16 + 2 * i, 2);
            }
            writeBytes(data);
        }

        private void button7_Click(object sender, EventArgs e)
        {
            //white line
            for (int i = 0; i < 7; i++)
            {
                front_limit[i].white.bottom = UInt16.Parse(lbl_current_front[i].Text);

                int error = front_limit[i].white.top - front_limit[i].white.bottom;
                int average = (front_limit[i].white.top + front_limit[i].white.bottom) / 2;
                int tolerance = error / 10;

                front_thres[i].white.upper = (UInt16)(average + tolerance);
                front_thres[i].white.lower = (UInt16)(average - tolerance);

                lbl_upper_front[i].Text = front_thres[i].white.upper.ToString();
                lbl_lower_front[i].Text = front_thres[i].white.lower.ToString();
            }
            data[1] = FRONT_WHITE;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(front_thres[i].white.upper)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(front_thres[i].white.lower)), 0, data, 16 + 2 * i, 2);
            }
            writeBytes(data);
        }

        private void button8_Click(object sender, EventArgs e)
        {
            //white background
            for (int i = 0; i < 7; i++)
            {
                front_limit[i].white.top = UInt16.Parse(lbl_current_front[i].Text);

                int error = front_limit[i].white.top - front_limit[i].white.bottom;
                int average = (front_limit[i].white.top + front_limit[i].white.bottom) / 2;
                int tolerance = error / 10;

                front_thres[i].white.upper = (UInt16)(average + tolerance);
                front_thres[i].white.lower = (UInt16)(average - tolerance);

                lbl_upper_front[i].Text = front_thres[i].white.upper.ToString();
                lbl_lower_front[i].Text = front_thres[i].white.lower.ToString();
            }
            data[1] = FRONT_WHITE;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(front_thres[i].white.upper)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(front_thres[i].white.lower)), 0, data, 16 + 2 * i, 2);
            }
            writeBytes(data);
        }

        private void button12_Click(object sender, EventArgs e)
        {
            //red line
            for (int i = 0; i < 7; i++)
            {
                back_limit[i].red.bottom = UInt16.Parse(lbl_current_back[i].Text);

                int error = back_limit[i].red.top - back_limit[i].red.bottom;
                int average = (back_limit[i].red.top + back_limit[i].red.bottom) / 2;
                int tolerance = error / 10;

                back_thres[i].red.upper = (UInt16)(average + tolerance);
                back_thres[i].red.lower = (UInt16)(average - tolerance);

                lbl_upper_back[i].Text = back_thres[i].red.upper.ToString();
                lbl_lower_back[i].Text = back_thres[i].red.lower.ToString();
            }
            data[1] = BACK_RED;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].red.upper)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].red.lower)), 0, data, 16 + 2 * i, 2);
            }
            writeBytes(data);
        }

        private void button9_Click(object sender, EventArgs e)
        {
            //red background
            for (int i = 0; i < 7; i++)
            {
                back_limit[i].red.top = UInt16.Parse(lbl_current_back[i].Text);

                int error = back_limit[i].red.top - back_limit[i].red.bottom;
                int average = (back_limit[i].red.top + back_limit[i].red.bottom) / 2;
                int tolerance = error / 10;

                back_thres[i].red.upper = (UInt16)(average + tolerance);
                back_thres[i].red.lower = (UInt16)(average - tolerance);

                lbl_upper_back[i].Text = back_thres[i].red.upper.ToString();
                lbl_lower_back[i].Text = back_thres[i].red.lower.ToString();
            }
            data[1] = BACK_RED;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].red.upper)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].red.lower)), 0, data, 16 + 2 * i, 2);
            }
            writeBytes(data);
        }

        private void button11_Click(object sender, EventArgs e)
        {
            //blue line
            for (int i = 0; i < 7; i++)
            {
                back_limit[i].blue.bottom = UInt16.Parse(lbl_current_back[i].Text);

                int error = back_limit[i].blue.top - back_limit[i].blue.bottom;
                int average = (back_limit[i].blue.top + back_limit[i].blue.bottom) / 2;
                int tolerance = error / 10;

                back_thres[i].blue.upper = (UInt16)(average + tolerance);
                back_thres[i].blue.lower = (UInt16)(average - tolerance);

                lbl_upper_back[i].Text = back_thres[i].blue.upper.ToString();
                lbl_lower_back[i].Text = back_thres[i].blue.lower.ToString();
            }
            data[1] = BACK_BLUE;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].blue.upper)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].blue.lower)), 0, data, 16 + 2 * i, 2);
            }
            writeBytes(data);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            //blue background
            for (int i = 0; i < 7; i++)
            {
                back_limit[i].blue.top = UInt16.Parse(lbl_current_back[i].Text);

                int error = back_limit[i].blue.top - back_limit[i].blue.bottom;
                int average = (back_limit[i].blue.top + back_limit[i].blue.bottom) / 2;
                int tolerance = error / 10;

                back_thres[i].blue.upper = (UInt16)(average + tolerance);
                back_thres[i].blue.lower = (UInt16)(average - tolerance);

                lbl_upper_back[i].Text = back_thres[i].blue.upper.ToString();
                lbl_lower_back[i].Text = back_thres[i].blue.lower.ToString();
            }
            data[1] = BACK_BLUE;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].blue.upper)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].blue.lower)), 0, data, 16 + 2 * i, 2);
            }
            writeBytes(data);
        }

        private void button10_Click(object sender, EventArgs e)
        {
            //white line
            for (int i = 0; i < 7; i++)
            {
                back_limit[i].white.bottom = UInt16.Parse(lbl_current_back[i].Text);

                int error = back_limit[i].white.top - back_limit[i].white.bottom;
                int average = (back_limit[i].white.top + back_limit[i].white.bottom) / 2;
                int tolerance = error / 10;

                back_thres[i].white.upper = (UInt16)(average + tolerance);
                back_thres[i].white.lower = (UInt16)(average - tolerance);

                lbl_upper_back[i].Text = back_thres[i].white.upper.ToString();
                lbl_lower_back[i].Text = back_thres[i].white.lower.ToString();
            }
            data[1] = BACK_WHITE;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].white.upper)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].white.lower)), 0, data, 16 + 2 * i, 2);
            }
            writeBytes(data);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //white background
            for (int i = 0; i < 7; i++)
            {
                back_limit[i].white.top = UInt16.Parse(lbl_current_back[i].Text);

                int error = back_limit[i].white.top - back_limit[i].white.bottom;
                int average = (back_limit[i].white.top + back_limit[i].white.bottom) / 2;
                int tolerance = error / 10;

                back_thres[i].white.upper = (UInt16)(average + tolerance);
                back_thres[i].white.lower = (UInt16)(average - tolerance);

                lbl_upper_back[i].Text = back_thres[i].white.upper.ToString();
                lbl_lower_back[i].Text = back_thres[i].white.lower.ToString();
            }
            data[1] = BACK_WHITE;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].white.upper)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].white.lower)), 0, data, 16 + 2 * i, 2);
            }
            writeBytes(data);
        }

        private void button17_Click(object sender, EventArgs e)
        {
            data[1] = SET_RED;
            writeBytes(data);
        }

        private void button18_Click(object sender, EventArgs e)
        {
            data[1] = SET_BLUE;
            writeBytes(data);
        }

        private void button19_Click(object sender, EventArgs e)
        {
            data[1] = SET_WHITE;
            writeBytes(data);
        }
        private void write(String x)
        {
            if (!port.IsOpen)
                return;
            port.WriteLine(x);
        }
        private void writeBytes(byte[] buffer)
        {
            try
            {
                if (port.IsOpen)
                {
                    Console.WriteLine("\n ----------------------------------------------------------------------------------------");
                    port.Write(buffer, 0, buffer.Length);
                    foreach (byte b in buffer)
                    {
                        Console.Write(b);
                        Console.Write(' ');
                    }
                    Console.WriteLine("\n ------------------------{0} bytes have been written-------------------------------------", buffer.Length);
                }
            }
            catch
            {
                //MessageBox.Show("Mất kết nối!", "LỖI", MessageBoxButtons.OK, MessageBoxIcon.Stop);
            }
        }

        private void button13_Click(object sender, EventArgs e)
        {
            data[1] = SAVE;
            writeBytes(data);
        }

        private void button14_Click(object sender, EventArgs e)
        {
            data[1] = SAVE_BACKUP;
            writeBytes(data);
        }

        private void button15_Click(object sender, EventArgs e)
        {
            data[1] = READ;
            writeBytes(data);
        }

        private void button16_Click(object sender, EventArgs e)
        {
            data[1] = READ_BACKUP;
            writeBytes(data);
        }
    }
}
