using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Windows;
using System.Windows.Controls;

namespace SpotifyApiToSerial
{
    public partial class MainWindow : Window, IDisposable
    {
        private bool _continue;

        //private string _portName = "";
        //private int _dataBits = 8;
        //private int _baudRate = 115200;
        //private Parity _parity = Parity.None;
        //private StopBits _stopBits = StopBits.One;
        //private Handshake _handshake = Handshake.None;
        //private SerialPort _serialPort = new SerialPort("COM9");

        private string _portName;
        private int _dataBits;
        private int _baudRate;
        private Parity _parity;
        private StopBits _stopBits;
        private Handshake _handshake;
        private SerialPort _serialPort = new SerialPort();

        public MainWindow()
        {
            InitializeComponent();
            //updatePorts();
            getPorts();
        }

        public void AppendTextBox(string value)
        {
            if (CheckAccess())
            {
                Dispatcher.Invoke(new Action<string>(AppendTextBox), new object[] { value });
                return;
            }
            TextBox1.Text += value;
        }

        private void btnSend_Click(object sender, RoutedEventArgs e)
        {
            sendData();
        }

        private void btnClear_Click(object sender, RoutedEventArgs e)
        {
            //Clear the screen½
            txtSendData.Clear();
        }

        private void btnConnect_Click(object sender, RoutedEventArgs e)
        {
            if (_serialPort.IsOpen)
            {
                disconnect();
            }
            else
            {
                connect();
            }
        }

        private void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = _serialPort;
            string indata = sp.ReadExisting(); // readLine
            Debug.WriteLine(indata.Length);
            this.Dispatcher.Invoke(() =>
            {
                Debug.WriteLine(indata);
                TextBox1.Text += indata;
                TextBox1.ScrollToEnd();
            });
        }

        private void connect()
        {

            try
            {
                //if (_serialPort !=null )
                //{
                //}
                _serialPort.Dispose();

                //_serialPort.BaudRate = 115200;
                //_serialPort.Parity = Parity.None;
                //_serialPort.StopBits = StopBits.One;
                //_serialPort.DataBits = 8;
                //_serialPort.Handshake = Handshake.None;
                //_serialPort.RtsEnable = true;
                // Allow the user to set the appropriate properties.
                GetDataFromComboBoxes();

                _serialPort.PortName = _portName;
                _serialPort.BaudRate = _baudRate;
                _serialPort.Parity = _parity;
                _serialPort.DataBits = _dataBits;
                _serialPort.StopBits = _stopBits;
                _serialPort.Handshake = _handshake;
                _serialPort.RtsEnable = true;

                _serialPort.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);

                _serialPort.Open();
                MessageBox.Show("Connect succes" + Environment.NewLine + _portName, "Succes", MessageBoxButton.OK);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error accessing port." + Environment.NewLine + ex.Message, "Port Error!!!", MessageBoxButton.OK);
                _serialPort.Dispose();
            }

            _continue = true;
            if (_serialPort.IsOpen)
            {
                Debug.WriteLine("treahstart");
                //readThread.Start();//
            }
        }

        private void disconnect()
        {
            MessageBox.Show("Disconnected" + Environment.NewLine + _portName, "Disconnected", MessageBoxButton.OK);
            // _serialPort.Dispose();
            _serialPort.Close();

            _continue = false;
            // btnConnect.Text = "Connect";
            btnSend.IsEnabled = false;
            // groupBox1.Enabled = true;
        }

        private void rtxtDataArea_TextChanged(object sender, TextChangedEventArgs e)
        {
            //if(_serialPort.IsOpen)
            //{
            //    TextBox1.Text = _serialPort.ReadLine();
            //}
        }

        private void getPorts()
        {
            string[] ports = SerialPort.GetPortNames();
            foreach (string port in ports)
            {
                cmbComPort.Items.Add(port);
            }
        }

        private void updatePorts()
        {
            cmbComPort.Items.Clear();
            // Retrieve the list of all COM ports on your Computer
            string[] ports = SerialPort.GetPortNames();
            List<string> f = new List<string>();
            foreach (var item in cmbComPort.Items)
            {
                f.Add(item.ToString());
            }
            

            foreach (var port in ports)
            {
                if (!f.Any(two => two == port))
                {
                    cmbComPort.Items.Add(port);
                }
                
            }
        }

        public void Read()
        {
            while (_continue)
            {
                try
                {
                    if (!_serialPort.IsOpen)
                    {
                        _serialPort.Open();
                    }
                    string message = _serialPort.ReadLine();
                    AppendTextBox(message);
                    Debug.WriteLine(message);

                    //Console.WriteLine(message);
                    //Thread.Sleep(100);
                }
                catch (TimeoutException ex)
                {
                    MessageBox.Show("Error port." + Environment.NewLine + ex.Message, "Port Error!!!", MessageBoxButton.OK);
                }
            }
        }

        private void GetDataFromComboBoxes()
        {
            if (cmbComPort.SelectedItem != null)
            {
                _portName = cmbComPort.SelectedItem.ToString();
            }

            if (cmbBaudRate.SelectedItem != null)
            {
                _baudRate = (int)(cmbBaudRate.SelectedItem);
            }

            if (cmbParity.SelectedItem != null)
            {
                _parity = (Parity)cmbParity.SelectedItem;
            }

            if (cmbDataBits.SelectedItem != null)
            {
                _dataBits = (int)cmbDataBits.SelectedItem;
            }

            if (cmbStopBits.SelectedItem != null)
            {
                _stopBits = (StopBits)cmbStopBits.SelectedItem;
            }
            if (cmbPortHandshake.SelectedItem != null)
            {
                _handshake = (Handshake)cmbPortHandshake.SelectedItem;
            }

            if (cmbComPort.SelectedItem == null
                || cmbBaudRate.SelectedItem == null
                || cmbDataBits.SelectedItem == null)
            {
                MessageBox.Show("Serial port not selected.");
            }

            //
        }

        private void Serial_Port_Interface_Loaded(object sender, RoutedEventArgs e)
        {
            //cmbComPort.Text = "";
            //cmbComPort.Items.Remove(0);

            //updatePorts();
        }

        private void Serial_Port_Interface_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (_serialPort.IsOpen) _serialPort.Close();
        }

        private void sendData()
        {
            throw new NotImplementedException();
        }

        public void Dispose()
        {
            ((IDisposable)_serialPort).Dispose();
        }

        private delegate void SetTextCallback(string text);

        private void btnReconnect_Click(object sender, RoutedEventArgs e)
        {
            updatePorts();
        }

        //private void SetText(string text)
        //{
        //    TextRange textRange = new TextRange(rtxtDataArea.Document.ContentStart, rtxtDataArea.Document.ContentEnd);
        //    textRange.Text = "Another world, another text!";

        //    rtxtDataArea.
        //    //invokeRequired required compares the thread ID of the calling thread to the thread of the creating thread.
        //    // if these threads are different, it returns true
        //    if (this.rtxtDataArea.InvokeRequired)
        //    {
        //        rtxtDataArea.ColoForeColor = Color.Green;    //write text data in Green colour

        //        SetTextCallback d = new SetTextCallback(SetText);
        //        rtxtDataArea(d, new object[] { text });
        //    }
        //    else
        //    {
        //        this.rtxtDataArea.AppendText(text);
        //    }
        //}
    }
}