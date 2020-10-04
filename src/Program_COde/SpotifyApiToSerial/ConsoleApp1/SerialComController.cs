using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO.Ports;
using System.Threading;

namespace SerialCommunication
{
    public class SerialComController : IDisposable, INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        //public event EventHandler<SerialDataEventArgs> NewSerialDataRecieved;

        #region Fields

        private static SerialPort _serialPort;

        //private SerialSettings _currentSerialSettings = new SerialSettings();
        private string _latestRecieved = string.Empty;

        private static bool _continue;
        private string _portName;
        private List<string> _portNameCollection;
        private int _baudRate; // = 115200;
        private Parity _parity; // = Parity.None;
        private int _dataBits;// = 8;
        private int[] _dataBitsCollection = new int[] { 5, 6, 7, 8 };
        private StopBits _stopBits;// = StopBits.One;

        #endregion Fields

        public SerialComController()
        {
            _baudRate = 115200;
            _parity = Parity.None;
            _dataBits = 8;
            _stopBits = StopBits.One;
        }

        public SerialComController(SerialPort serialPort, string portName, int baudRate, Parity parity, int dataBits, StopBits stopBits)
        {
            _serialPort = serialPort;
            _portName = portName;
            _baudRate = baudRate;
            _parity = parity;
            _dataBits = dataBits;
            _stopBits = stopBits;
        }

        //public SerialComController()
        //{
        //    string name;
        //    string message;
        //    StringComparer stringComparer = StringComparer.OrdinalIgnoreCase;

        //    // Create a new SerialPort object with default settings.
        //    _serialPort = new SerialPort();
        //}

        public static void Start()
        {
            Thread readThread = new Thread(Read);
            // Set the read/write timeouts
            _serialPort.ReadTimeout = 500;
            _serialPort.WriteTimeout = 500;

            _serialPort.Open();
            _continue = true;
            readThread.Start();

            readThread.Join();
            _serialPort.Close();
        }

        public static void Read()
        {
            while (_continue)
            {
                try
                {
                    string message = _serialPort.ReadLine();
                    Console.WriteLine(message);
                }
                catch (TimeoutException) { }
            }
        }

        public string PortName
        {
            get { return _portName; }
            set
            {
                if (!_portName.Equals(value))
                {
                    _portName = value;
                    SendPropertyChangedEvent("PortName");
                }
            }
        }

        public List<String> GetPortNames()
        {
            List<String> temp = new List<string>();
            Console.WriteLine("Available Ports:");
            foreach (string s in SerialPort.GetPortNames())
            {
                //Console.WriteLine("   {0}", s);
                temp.Add(s);
            }
            return temp;
        }

        //public static string SetPortName(string defaultPortName)
        //{
        //    string portName;

        //    Console.WriteLine("Available Ports:");
        //    foreach (string s in SerialPort.GetPortNames())
        //    {
        //        Console.WriteLine("   {0}", s);
        //    }

        //    Console.Write("Enter COM port value (Default: {0}): ", defaultPortName);
        //    portName = Console.ReadLine();

        //    if (portName == "" || !(portName.ToLower()).StartsWith("com"))
        //    {
        //        portName = defaultPortName;
        //    }
        //    return portName;
        //}

        // Display BaudRate values and prompt user to enter a value.

        public int BaudRate
        {
            get { return _baudRate; }
            set
            {
                if (_baudRate != value)
                {
                    _baudRate = value;
                    SendPropertyChangedEvent("BaudRate");
                }
            }
        }

        public Parity Parity
        {
            get { return _parity; }
            set
            {
                if (_parity != value)
                {
                    _parity = value;
                    SendPropertyChangedEvent("Parity");
                }
            }
        }

        //// Display PortParity values and prompt user to enter a value.
        //public static Parity SetPortParity(Parity defaultPortParity)
        //{
        //    string parity;

        //    Console.WriteLine("Available Parity options:");
        //    foreach (string s in Enum.GetNames(typeof(Parity)))
        //    {
        //        Console.WriteLine("   {0}", s);
        //    }

        //    Console.Write("Enter Parity value (Default: {0}):", defaultPortParity.ToString(), true);
        //    parity = Console.ReadLine();

        //    if (parity == "")
        //    {
        //        parity = defaultPortParity.ToString();
        //    }

        //    return (Parity)Enum.Parse(typeof(Parity), parity, true);
        //}

        // Display DataBits values and prompt user to enter a value.

        public int DataBits
        {
            get { return _dataBits; }
            set
            {
                if (_dataBits != value)
                {
                    _dataBits = value;
                    SendPropertyChangedEvent("DataBits");
                }
            }
        }

        public static int SetPortDataBits(int defaultPortDataBits)
        {
            string dataBits;

            Console.Write("Enter DataBits value (Default: {0}): ", defaultPortDataBits);
            dataBits = Console.ReadLine();

            if (dataBits == "")
            {
                dataBits = defaultPortDataBits.ToString();
            }

            return int.Parse(dataBits.ToUpperInvariant());
        }

        public StopBits StopBits
        {
            get { return _stopBits; }
            set
            {
                if (_stopBits != value)
                {
                    _stopBits = value;
                    SendPropertyChangedEvent("StopBits");
                }
            }
        }

        //// Display StopBits values and prompt user to enter a value.
        //public static StopBits SetPortStopBits(StopBits defaultPortStopBits)
        //{
        //    string stopBits;

        //    Console.WriteLine("Available StopBits options:");
        //    foreach (string s in Enum.GetNames(typeof(StopBits)))
        //    {
        //        Console.WriteLine("   {0}", s);
        //    }

        //    Console.Write("Enter StopBits value (None is not supported and \n" +
        //     "raises an ArgumentOutOfRangeException. \n (Default: {0}):", defaultPortStopBits.ToString());
        //    stopBits = Console.ReadLine();

        //    if (stopBits == "")
        //    {
        //        stopBits = defaultPortStopBits.ToString();
        //    }

        //    return (StopBits)Enum.Parse(typeof(StopBits), stopBits, true);
        //}

        public static Handshake SetPortHandshake(Handshake defaultPortHandshake)
        {
            string handshake;

            Console.WriteLine("Available Handshake options:");
            foreach (string s in Enum.GetNames(typeof(Handshake)))
            {
                Console.WriteLine("   {0}", s);
            }

            Console.Write("Enter Handshake value (Default: {0}):", defaultPortHandshake.ToString());
            handshake = Console.ReadLine();

            if (handshake == "")
            {
                handshake = defaultPortHandshake.ToString();
            }

            return (Handshake)Enum.Parse(typeof(Handshake), handshake, true);
        }

        /// <summary>
        /// Send a PropertyChanged event
        /// </summary>
        /// <param name="propertyName">Name of changed property</param>
        private void SendPropertyChangedEvent(String propertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }

        public void Dispose()
        {
            throw new NotImplementedException();
        }
    }
}