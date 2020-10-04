using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WpfApp1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// https://stackoverflow.com/questions/3481667/serialport-buffer-on-new-thread
    /// </summary>
    public partial class MainWindow : Window
    {
        SerialPort mySerialPort = new SerialPort("COM9");
        public MainWindow()
        {
            InitializeComponent();

        }

        private void btnConnect_Click(object sender, RoutedEventArgs e)
        {
           


            mySerialPort.Dispose();
            mySerialPort.BaudRate = 115200;
            mySerialPort.Parity = Parity.None;
            mySerialPort.StopBits = StopBits.One;
            mySerialPort.DataBits = 8;
            mySerialPort.Handshake = Handshake.None;
            mySerialPort.RtsEnable = true;
            //mySerialPort.ReadTimeout = 500000;
            //mySerialPort.WriteTimeout = 500;

            mySerialPort.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);

            mySerialPort.Open();




        }

        private void DataReceivedHandler(
                            object sender,
                            SerialDataReceivedEventArgs e)
        {
            //SerialPort sp = (SerialPort)sender;
            //string indata = sp.ReadExisting();

            SerialPort sp = mySerialPort;
            string indata = sp.ReadExisting();

            //Console.WriteLine("Data Received:");
            //Console.Write(indata);

            Debug.WriteLine(indata.Length);
            this.Dispatcher.Invoke(() =>
            {
                Debug.WriteLine(indata);
                txtBox.Text += indata;
            });


        }

        private void txtBox_Loaded(object sender, RoutedEventArgs e)
        {

        }
    }
}
