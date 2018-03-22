using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Configuration;
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
        const byte side_RED = 8;
        const byte side_BLUE = 10;
        const byte side_WHITE = 12;
        const byte BACK_RED = 13;
        const byte BACK_BLUE = 14;
        const byte BACK_WHITE = 15;

        SerialPort port = new SerialPort();
        String indata;
        Button[] prox_state = new Button[6];
        Button[] side_state = new Button[7];
        Button[] back_state = new Button[7];
        Label[] lbl_current_side = new Label[7];
        Label[] lbl_current_back = new Label[7];
        Label[] lbl_upper_side = new Label[7];
        Label[] lbl_lower_side = new Label[7];
        Label[] lbl_upper_back = new Label[7];
        Label[] lbl_lower_back = new Label[7];
        TrackBar[] tb_side = new TrackBar[7];
        TrackBar[] tb_back = new TrackBar[7];

        //used to move the visible line
        private float minLineSideY, maxLineSideY, minLineBackX, maxLineBackX;

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
            public UInt16 background;
            public UInt16 line;
        }
        struct ColorThreshold
        {
            public Threshold red;
            public Threshold blue;
            public Threshold white;
        }
        ColorValLimit[] side_limit = new ColorValLimit[7];
        ColorValLimit[] back_limit = new ColorValLimit[7];
        ColorThreshold[] side_thres = new ColorThreshold[7];
        ColorThreshold[] back_thres = new ColorThreshold[7];
        
      


        public Form1()
        {
            InitializeComponent();

            minLineSideY = btnF0.Location.Y;
            maxLineSideY = btnF6.Location.Y;
            minLineBackX = btnB6.Location.X;
            maxLineBackX = btnB0.Location.X;

            port.BaudRate = 115200;
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

            side_state[0] = btnF0;
            side_state[1] = btnF1;
            side_state[2] = btnF2;
            side_state[3] = btnF3;
            side_state[4] = btnF4;
            side_state[5] = btnF5;
            side_state[6] = btnF6;

            back_state[0] = btnB0;
            back_state[1] = btnB1;
            back_state[2] = btnB2;
            back_state[3] = btnB3;
            back_state[4] = btnB4;
            back_state[5] = btnB5;
            back_state[6] = btnB6;

            lbl_current_side[0] = lblCurrentF0;
            lbl_current_side[1] = lblCurrentF1;
            lbl_current_side[2] = lblCurrentF2;
            lbl_current_side[3] = lblCurrentF3;
            lbl_current_side[4] = lblCurrentF4;
            lbl_current_side[5] = lblCurrentF5;
            lbl_current_side[6] = lblCurrentF6;

            lbl_current_back[0] = lblCurrentB0;
            lbl_current_back[1] = lblCurrentB1;
            lbl_current_back[2] = lblCurrentB2;
            lbl_current_back[3] = lblCurrentB3;
            lbl_current_back[4] = lblCurrentB4;
            lbl_current_back[5] = lblCurrentB5;
            lbl_current_back[6] = lblCurrentB6;

            lbl_upper_side[0] = lblUpperF0;
            lbl_upper_side[1] = lblUpperF1;
            lbl_upper_side[2] = lblUpperF2;
            lbl_upper_side[3] = lblUpperF3;
            lbl_upper_side[4] = lblUpperF4;
            lbl_upper_side[5] = lblUpperF5;
            lbl_upper_side[6] = lblUpperF6;

            lbl_upper_back[0] = lblUpperB0;
            lbl_upper_back[1] = lblUpperB1;
            lbl_upper_back[2] = lblUpperB2;
            lbl_upper_back[3] = lblUpperB3;
            lbl_upper_back[4] = lblUpperB4;
            lbl_upper_back[5] = lblUpperB5;
            lbl_upper_back[6] = lblUpperB6;

            lbl_lower_side[0] = lblLowerF0;
            lbl_lower_side[1] = lblLowerF1;
            lbl_lower_side[2] = lblLowerF2;
            lbl_lower_side[3] = lblLowerF3;
            lbl_lower_side[4] = lblLowerF4;
            lbl_lower_side[5] = lblLowerF5;
            lbl_lower_side[6] = lblLowerF6;

            lbl_lower_back[0] = lblLowerB0;
            lbl_lower_back[1] = lblLowerB1;
            lbl_lower_back[2] = lblLowerB2;
            lbl_lower_back[3] = lblLowerB3;
            lbl_lower_back[4] = lblLowerB4;
            lbl_lower_back[5] = lblLowerB5;
            lbl_lower_back[6] = lblLowerB6;

            tb_side[0] = tbF0;
            tb_side[1] = tbF1;
            tb_side[2] = tbF2;
            tb_side[3] = tbF3;
            tb_side[4] = tbF4;
            tb_side[5] = tbF5;
            tb_side[6] = tbF6;

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
                {
                    lblInfo.Text = _indata;
                    btnSignal.BackColor = Color.Red;
                    timer2.Enabled = true;
                }

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
                        side_state[i].BackColor = Color.Gray;
                    else
                        side_state[i].BackColor = Color.Yellow;

                }

                #region MoveTheVisibleLine
                ///Move the visible line
                float offset_LTR = 0;
                float offset_RTL = 0;
                Boolean isFullLine = true;
                Boolean isNotFound = true;
                for (int index = 0; index < 7; index++)
                {
                    if (val[index] == 1)
                    {
                        offset_LTR = (index - 3);
                        isNotFound = false;
                        break;
                    }
                    else
                        isFullLine = false;
                }
                for (int index = 6; index >= 0; index--)
                {
                    if (val[index] == 1)
                    {
                        offset_RTL = (index - 3);
                        isNotFound = false;
                        break;
                    }
                    else
                        isFullLine = false;
                }
                float loc = (offset_LTR + offset_RTL) / -2.0f;
                if (isFullLine)
                {
                    lineFullSide.Visible = true;
                    lineSide.Visible = false;
                }
                else if (isNotFound)
                {
                    lineFullSide.Visible = false;
                    lineSide.Visible = false;
                    loc = 255;
                }
                else
                {
                    lineFullSide.Visible = false;
                    lineSide.Visible = true;

                    float averageY = (minLineSideY + maxLineSideY ) /2;
                    float step = (minLineSideY + maxLineSideY) / 12;
                    lineSide.Location = new Point(lineSide.Location.X, (int)(averageY - step * loc));
                    lblSideOffset.Location = new Point(lblSideOffset.Location.X, (int)(averageY - step * loc));
                }
                lblSideOffset.Text = loc.ToString();
                #endregion


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
                #region MoveTheVisibleLine
                ///Move the visible line
                float offset_LTR = 0;
                float offset_RTL = 0;
                Boolean isFullLine = true;
                Boolean isNotFound = true;
                for (int index = 0; index < 7; index++)
                {
                    if (val[index] == 1)
                    {
                        offset_LTR = (index - 3);
                        isNotFound = false;
                        break;
                    }
                    else
                        isFullLine = false;
                }
                for (int index = 6; index >= 0; index--)
                {
                    if (val[index] == 1)
                    {
                        offset_RTL = (index - 3);
                        isNotFound = false;
                        break;
                    }
                    else
                        isFullLine = false;
                }
                float loc = (offset_LTR + offset_RTL) / -2.0f;
                if (isFullLine)
                {
                    lineFullBack.Visible = true;
                    lineBack.Visible = false;
                }
                else if (isNotFound)
                {
                    lineFullBack.Visible = false;
                    lineBack.Visible = false;
                    loc = 255;
                }
                else
                {
                    lineFullBack.Visible = false;
                    lineBack.Visible = true;

                    float averageX = (maxLineBackX + minLineBackX) / 2;
                    float step = (maxLineBackX + minLineBackX) / 12;
                    lineBack.Location = new Point((int)(averageX + step * loc), lineBack.Location.Y);
                    lblBackOffset.Location = new Point((int)(averageX + step * loc), lblBackOffset.Location.Y);
                }
                lblBackOffset.Text = loc.ToString();
                #endregion
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
                    lbl_current_side[i].Text = val[i].ToString();
                    tb_side[i].Value = val[i];
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
                side_limit[i].red.bottom = UInt16.Parse(lbl_current_side[i].Text);

                int error = side_limit[i].red.top - side_limit[i].red.bottom;
                int average = (side_limit[i].red.top + side_limit[i].red.bottom) / 2;
                int tolerance = error / 10;

                side_thres[i].red.background = (UInt16)(average + tolerance);
                side_thres[i].red.line = (UInt16)(average - tolerance);

                lbl_upper_side[i].Text = side_thres[i].red.background.ToString();
                lbl_lower_side[i].Text = side_thres[i].red.line.ToString();
            }

            data[1] = side_RED;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(side_thres[i].red.background)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(side_thres[i].red.line)), 0, data, 16 + 2 * i, 2);
            }
            writeBytes(data);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            //red background
            for (int i = 0; i < 7; i++)
            {
                side_limit[i].red.top = UInt16.Parse(lbl_current_side[i].Text);

                int error = side_limit[i].red.top - side_limit[i].red.bottom;
                int average = (side_limit[i].red.top + side_limit[i].red.bottom) / 2;
                int tolerance = error / 10;

                side_thres[i].red.background = (UInt16)(average + tolerance);
                side_thres[i].red.line = (UInt16)(average - tolerance);

                lbl_upper_side[i].Text = side_thres[i].red.background.ToString();
                lbl_lower_side[i].Text = side_thres[i].red.line.ToString();
            }

            data[1] = side_RED;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(side_thres[i].red.background)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(side_thres[i].red.line)), 0, data, 16 + 2 * i, 2);
            }
            writeBytes(data);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            //blue line
            for (int i = 0; i < 7; i++)
            {
                side_limit[i].blue.bottom = UInt16.Parse(lbl_current_side[i].Text);

                int error = side_limit[i].blue.top - side_limit[i].blue.bottom;
                int average = (side_limit[i].blue.top + side_limit[i].blue.bottom) / 2;
                int tolerance = error / 10;

                side_thres[i].blue.background = (UInt16)(average + tolerance);
                side_thres[i].blue.line = (UInt16)(average - tolerance);

                lbl_upper_side[i].Text = side_thres[i].blue.background.ToString();
                lbl_lower_side[i].Text = side_thres[i].blue.line.ToString();
            }
            data[1] = side_BLUE;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(side_thres[i].blue.background)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(side_thres[i].blue.line)), 0, data, 16 + 2 * i, 2);
            }
            writeBytes(data);

        }

        private void button6_Click(object sender, EventArgs e)
        {
            //blue background
            for (int i = 0; i < 7; i++)
            {
                side_limit[i].blue.top = UInt16.Parse(lbl_current_side[i].Text);

                int error = side_limit[i].blue.top - side_limit[i].blue.bottom;
                int average = (side_limit[i].blue.top + side_limit[i].blue.bottom) / 2;
                int tolerance = error / 10;

                side_thres[i].blue.background = (UInt16)(average + tolerance);
                side_thres[i].blue.line = (UInt16)(average - tolerance);

                lbl_upper_side[i].Text = side_thres[i].blue.background.ToString();
                lbl_lower_side[i].Text = side_thres[i].blue.line.ToString();
            }
            data[1] = side_BLUE;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(side_thres[i].blue.background)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(side_thres[i].blue.line)), 0, data, 16 + 2 * i, 2);
            }
            writeBytes(data);
        }

        private void button7_Click(object sender, EventArgs e)
        {
            //white line
            for (int i = 0; i < 7; i++)
            {
                side_limit[i].white.bottom = UInt16.Parse(lbl_current_side[i].Text);

                int error = side_limit[i].white.top - side_limit[i].white.bottom;
                int average = (side_limit[i].white.top + side_limit[i].white.bottom) / 2;
                int tolerance = error / 10;

                side_thres[i].white.background = (UInt16)(average + tolerance);
                side_thres[i].white.line = (UInt16)(average - tolerance);

                lbl_upper_side[i].Text = side_thres[i].white.background.ToString();
                lbl_lower_side[i].Text = side_thres[i].white.line.ToString();
            }
            data[1] = side_WHITE;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(side_thres[i].white.background)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(side_thres[i].white.line)), 0, data, 16 + 2 * i, 2);
            }
            writeBytes(data);
        }

        private void button8_Click(object sender, EventArgs e)
        {
            //white background
            for (int i = 0; i < 7; i++)
            {
                side_limit[i].white.top = UInt16.Parse(lbl_current_side[i].Text);

                int error = side_limit[i].white.top - side_limit[i].white.bottom;
                int average = (side_limit[i].white.top + side_limit[i].white.bottom) / 2;
                int tolerance = error / 10;

                side_thres[i].white.background = (UInt16)(average + tolerance);
                side_thres[i].white.line = (UInt16)(average - tolerance);

                lbl_upper_side[i].Text = side_thres[i].white.background.ToString();
                lbl_lower_side[i].Text = side_thres[i].white.line.ToString();
            }
            data[1] = side_WHITE;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(side_thres[i].white.background)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(side_thres[i].white.line)), 0, data, 16 + 2 * i, 2);
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

                back_thres[i].red.background = (UInt16)(average + tolerance);
                back_thres[i].red.line = (UInt16)(average - tolerance);

                lbl_upper_back[i].Text = back_thres[i].red.background.ToString();
                lbl_lower_back[i].Text = back_thres[i].red.line.ToString();
            }
            data[1] = BACK_RED;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].red.background)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].red.line)), 0, data, 16 + 2 * i, 2);
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

                back_thres[i].red.background = (UInt16)(average + tolerance);
                back_thres[i].red.line = (UInt16)(average - tolerance);

                lbl_upper_back[i].Text = back_thres[i].red.background.ToString();
                lbl_lower_back[i].Text = back_thres[i].red.line.ToString();
            }
            data[1] = BACK_RED;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].red.background)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].red.line)), 0, data, 16 + 2 * i, 2);
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

                back_thres[i].blue.background = (UInt16)(average + tolerance);
                back_thres[i].blue.line = (UInt16)(average - tolerance);

                lbl_upper_back[i].Text = back_thres[i].blue.background.ToString();
                lbl_lower_back[i].Text = back_thres[i].blue.line.ToString();
            }
            data[1] = BACK_BLUE;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].blue.background)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].blue.line)), 0, data, 16 + 2 * i, 2);
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

                back_thres[i].blue.background = (UInt16)(average + tolerance);
                back_thres[i].blue.line = (UInt16)(average - tolerance);

                lbl_upper_back[i].Text = back_thres[i].blue.background.ToString();
                lbl_lower_back[i].Text = back_thres[i].blue.line.ToString();
            }
            data[1] = BACK_BLUE;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].blue.background)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].blue.line)), 0, data, 16 + 2 * i, 2);
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

                back_thres[i].white.background = (UInt16)(average + tolerance);
                back_thres[i].white.line = (UInt16)(average - tolerance);

                lbl_upper_back[i].Text = back_thres[i].white.background.ToString();
                lbl_lower_back[i].Text = back_thres[i].white.line.ToString();
            }
            data[1] = BACK_WHITE;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].white.background)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].white.line)), 0, data, 16 + 2 * i, 2);
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

                back_thres[i].white.background = (UInt16)(average + tolerance);
                back_thres[i].white.line = (UInt16)(average - tolerance);

                lbl_upper_back[i].Text = back_thres[i].white.background.ToString();
                lbl_lower_back[i].Text = back_thres[i].white.line.ToString();
            }
            data[1] = BACK_WHITE;
            for (int i = 0; i < 7; i++)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].white.background)), 0, data, 2 + 2 * i, 2);
                Buffer.BlockCopy(BitConverter.GetBytes((UInt16)(back_thres[i].white.line)), 0, data, 16 + 2 * i, 2);
            }
            writeBytes(data);
        }

        private void button17_Click(object sender, EventArgs e)
        {
            data[1] = SET_RED;
            writeBytes(data);
            panelCurrentColor.BackColor = Color.Red;
            lineSide.BackColor = Color.Red;
            lineBack.BackColor = Color.Red;
            lineFullBack.BackColor = Color.Red;
            lineFullSide.BackColor = Color.Red;
            panelRealColor.BackColor = Color.Green;
        }

        private void button18_Click(object sender, EventArgs e)
        {
            data[1] = SET_BLUE;
            writeBytes(data);
            panelCurrentColor.BackColor = Color.Blue;
            lineSide.BackColor = Color.Blue;
            lineBack.BackColor = Color.Blue;
            lineFullBack.BackColor = Color.Blue;
            lineFullSide.BackColor = Color.Blue;
            panelRealColor.BackColor = Color.Green;
        }

        private void button19_Click(object sender, EventArgs e)
        {
            data[1] = SET_WHITE;
            writeBytes(data);
            panelCurrentColor.BackColor = Color.White;
            lineSide.BackColor = Color.White;
            lineBack.BackColor = Color.White;
            lineFullBack.BackColor = Color.White;
            lineFullSide.BackColor = Color.White;
            panelRealColor.BackColor = Color.Red;
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
            panelCurrentColor.BackColor = Color.Black;
            panelRealColor.BackColor = Color.Black;
        }

        private void button16_Click(object sender, EventArgs e)
        {
            data[1] = READ_BACKUP;
            writeBytes(data);
            panelCurrentColor.BackColor = Color.Black;
            panelRealColor.BackColor = Color.Black;
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            timer2.Enabled = false;
            btnSignal.BackColor = Color.White;
        }
    }
}
