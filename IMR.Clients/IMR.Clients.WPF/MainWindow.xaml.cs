// Authored by: MohammadAmin Mohammadi (https://github.com/mamohammadi)

using System.Windows;
using IMR.Protocols;
using IMR.MediaInterfaces;
using IMR.Types;
using System.Threading.Tasks;
using System;
using System.Globalization;
using System.Threading;

namespace IMR.Clients.WPF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private int tableWidth = 200;

        private string PrintLine()
        {
            return new string('-', tableWidth) + "\r\n";
        }

        private string PrintRow(params string[] columns)
        {
            int width = (tableWidth - columns.Length) / columns.Length;
            string row = "|";

            bool overlaped = false;
            for (int i = 0; i < columns.Length; i++)
            {
                string text = columns[i].Substring(0, Math.Min(columns[i].Length, width));
                row += AlignCentre(text, width) + "|";
                if (columns[i].Length > width)
                {
                    columns[i] = columns[i].Substring(width);
                    overlaped = true;
                }
                else
                    columns[i] = string.Empty;
            }
            row += "\r\n";

            if (overlaped)
                row += PrintRow(columns);

            return row;
        }

        private string AlignCentre(string text, int width)
        {
            if (string.IsNullOrEmpty(text))
                return new string(' ', width);
            return text.PadRight(width - (width - text.Length) / 2).PadLeft(width);
        }

        private string ReadActarisRegisters(DlmsMeterSettings settings, string ip, int port)
        {
            string output = string.Empty;
            NetMedia netMedia = new NetMedia(ip, port);
            if (netMedia.Connect())
            {
                DlmsMeterHandler dlms = new DlmsMeterHandler(settings, netMedia);

                // Login
                DlmsErrorCode errorCode;
                MediaError ret = dlms.Login(out errorCode);
                if (ret == MediaError.None && errorCode == DlmsErrorCode.None)
                {
                    // Voltage
                    var phaseAVoltage = DlmsRegister.CreatePhaseAVoltageRegister();
                    var phaseBVoltage = DlmsRegister.CreatePhaseBVoltageRegister();
                    var phaseCVoltage = DlmsRegister.CreatePhaseCVoltageRegister();

                    // Current
                    var phaseACurrent = DlmsRegister.CreatePhaseACurrentRegister();
                    var phaseBCurrent = DlmsRegister.CreatePhaseBCurrentRegister();
                    var phaseCCurrent = DlmsRegister.CreatePhaseCCurrentRegister();

                    // Angle
                    var phaseAAngle = DlmsRegister.CreatePhaseAAngleRegister();
                    var phaseBAngle = DlmsRegister.CreatePhaseBAngleRegister();
                    var phaseCAngle = DlmsRegister.CreatePhaseCAngleRegister();

                    // Misc
                    var frequency = DlmsRegister.CreateFrequencyRegister();
                    var powerFactor = DlmsRegister.CreatePowerFactorRegister();
                    var temperature = DlmsRegister.CreateTemperatureRegister();

                    DlmsRegisterCollection regs = new DlmsRegisterCollection()
                    {
                        phaseAVoltage,
                        phaseBVoltage,
                        phaseCVoltage,
                        phaseACurrent,
                        phaseBCurrent,
                        phaseCCurrent,
                        phaseAAngle,
                        phaseBAngle,
                        phaseCAngle,
                        frequency,
                        powerFactor,
                        temperature
                    };

                    ret = dlms.ReadRegisters(regs);
                    if (ret == MediaError.None && errorCode == DlmsErrorCode.None)
                    {
                        // Read Success
                        phaseAVoltage.ApplyTransformRatio(settings.PT);
                        phaseBVoltage.ApplyTransformRatio(settings.PT);
                        phaseCVoltage.ApplyTransformRatio(settings.PT);

                        phaseACurrent.ApplyTransformRatio(settings.CT);
                        phaseBCurrent.ApplyTransformRatio(settings.CT);
                        phaseCCurrent.ApplyTransformRatio(settings.CT);

                        // Voltages
                        output += "Voltage:\r\n";

                        output += "\tPhase A:\t";
                        output += phaseAVoltage.ErrorCode == DlmsErrorCode.None ? phaseAVoltage.Value.Value.ToString("0.00") + "\t" + phaseAVoltage.Unit.Name : phaseAVoltage.ErrorCode.ToString();
                        output += "\r\n";

                        output += "\tPhase B:\t";
                        output += phaseBVoltage.ErrorCode == DlmsErrorCode.None ? phaseBVoltage.Value.Value.ToString("0.00") + "\t" + phaseBVoltage.Unit.Name : phaseBVoltage.ErrorCode.ToString();
                        output += "\r\n";

                        output += "\tPhase C:\t";
                        output += phaseCVoltage.ErrorCode == DlmsErrorCode.None ? phaseCVoltage.Value.Value.ToString("0.00") + "\t" + phaseCVoltage.Unit.Name : phaseCVoltage.ErrorCode.ToString();
                        output += "\r\n";

                        // Currents
                        output += "Current:\r\n";

                        output += "\tPhase A:\t";
                        output += phaseACurrent.ErrorCode == DlmsErrorCode.None ? phaseACurrent.Value.Value.ToString("0.00") + "\t" + phaseACurrent.Unit.Name : phaseACurrent.ErrorCode.ToString();
                        output += "\r\n";

                        output += "\tPhase B:\t";
                        output += phaseBCurrent.ErrorCode == DlmsErrorCode.None ? phaseBCurrent.Value.Value.ToString("0.00") + "\t" + phaseBCurrent.Unit.Name : phaseBCurrent.ErrorCode.ToString();
                        output += "\r\n";

                        output += "\tPhase C:\t";
                        output += phaseCCurrent.ErrorCode == DlmsErrorCode.None ? phaseCCurrent.Value.Value.ToString("0.00") + "\t" + phaseCCurrent.Unit.Name : phaseCCurrent.ErrorCode.ToString();
                        output += "\r\n";

                        // Angles
                        output += "Angle:\r\n";

                        output += "\tPhase A:\t";
                        output += phaseAAngle.ErrorCode == DlmsErrorCode.None ? phaseAAngle.Value.Value.ToString("0.00") + "\t" + phaseAAngle.Unit.Name : phaseAAngle.ErrorCode.ToString();
                        output += "\r\n";

                        output += "\tPhase B:\t";
                        output += phaseBAngle.ErrorCode == DlmsErrorCode.None ? phaseBAngle.Value.Value.ToString("0.00") + "\t" + phaseBAngle.Unit.Name : phaseBAngle.ErrorCode.ToString();
                        output += "\r\n";

                        output += "\tPhase C:\t";
                        output += phaseCAngle.ErrorCode == DlmsErrorCode.None ? phaseCAngle.Value.Value.ToString("0.00") + "\t" + phaseCAngle.Unit.Name : phaseCAngle.ErrorCode.ToString();
                        output += "\r\n";

                        // Misc
                        output += "Misc:\r\n";

                        output += "\tFreq:\t";
                        output += frequency.ErrorCode == DlmsErrorCode.None ? frequency.Value.Value.ToString("0.00") + "\t" + frequency.Unit.Name : frequency.ErrorCode.ToString();
                        output += "\r\n";

                        output += "\tPF:\t";
                        output += powerFactor.ErrorCode == DlmsErrorCode.None ? powerFactor.Value.Value.ToString("0.00") + "\t" + powerFactor.Unit.Name : powerFactor.ErrorCode.ToString();
                        output += "\r\n";

                        output += "\tTemp:\t";
                        output += temperature.ErrorCode == DlmsErrorCode.None ? temperature.Value.Value.ToString("0.00") + "\t" + temperature.Unit.Name : temperature.ErrorCode.ToString();
                    }
                    else
                        output += "Read Registers Failed. Ret:" + ret.ToString() + " ErrorCode:" + errorCode.ToString();

                    // Logout
                    ret = dlms.Logout(out errorCode);
                }
                else
                    output += "Login Failed. Ret:" + ret.ToString() + " ErrorCode:" + errorCode.ToString();

                netMedia.Disconnect();
            }
            else
                output += "Failed to connect.";

            return output;
        }

        private string ReadActarisProfile(DlmsMeterSettings settings, string ip, int port)
        {
            string output = string.Empty;
            NetMedia netMedia = new NetMedia(ip, port);
            if (netMedia.Connect())
            {
                DlmsMeterHandler dlms = new DlmsMeterHandler(settings, netMedia);

                // Login
                DlmsErrorCode errorCode;
                MediaError ret = dlms.Login(out errorCode);
                if (ret == MediaError.None && errorCode == DlmsErrorCode.None)
                {
                    // Read Profile Channels
                    dlms.ReadProfileChannels(DlmsSurvey.LS2, out DlmsProfileChannelInfo[] channels, out errorCode);

                    if (ret == MediaError.None && errorCode == DlmsErrorCode.None)
                    {
                        // Choose Selected Channels
                        DlmsProfileChannelInfo[] sChannels = new DlmsProfileChannelInfo[4];
                        sChannels[0] = channels[0];
                        sChannels[1] = channels[1];
                        sChannels[2] = channels[2];
                        sChannels[3] = channels[3];

                        DlmsProfile profile;
                        var now = DateTime.Now;
                        DateTime fromDate = new DateTime(now.Year, now.Month, now.Day, 0, 15, 0);
                        ret = dlms.ReadProfile(DlmsSurvey.LS2, fromDate, null, sChannels, out profile, out errorCode);

                        if (ret == MediaError.None && errorCode == DlmsErrorCode.None)
                        {
                            // Actaris does not implemented profile record interval attribute so we use default 900 second for profile 1
                            // If the interval readed from meter does not match the actual value, Just igonre it and set it manually.
                            int interval = profile.Interval;

                            // Use profile dounded date
                            DateTime pDate = profile.FromDate;
                            output += "\r\n";
                            output += PrintLine();
                            output += " Name: " + profile.Name + "\tInterval (seconds): " + interval.ToString() + "\r\n";
                            output += PrintLine();
                            string[] columns = new string[3 + profile.ColumnsCount];
                            columns[0] = "Date Time";
                            columns[1] = "Status Code";
                            columns[2] = "Status";
                            for (int i = 0; i < profile.ColumnsCount; i++)
                            {
                                var column = profile.Columns[i];
                                columns[3 + i] = column.Name; // + column.Unit.Name; // + '(' + column.Unit + ')' + '\t';
                            }
                            output += PrintRow(columns);
                            output += PrintLine();
                            for (int row = 0; row < profile.RowsCount; row++)
                            {
                                columns[0] = pDate.ToString("yyyy/MM/dd HH:mm:ss");
                                columns[1] = "0000";
                                columns[2] = "Valid";
                                for (int column = 0; column < profile.ColumnsCount; column++)
                                {
                                    int? v = profile[row][column].ToInt32();
                                    columns[3 + column] = (v == null) ? "Null" : v.ToString();
                                }
                                output += PrintRow(columns);
                                output += PrintLine();
                                pDate = pDate.AddSeconds(profile.Interval);
                            }
                            output += "\r\n";
                        }
                        else
                            output += "Read Profile Failed. Ret:" + ret.ToString() + " ErrorCode:" + errorCode.ToString();
                    }
                    else
                        output += "Read Profile Channels Failed. Ret:" + ret.ToString() + " ErrorCode:" + errorCode.ToString();

                    // Logout
                    ret = dlms.Logout(out errorCode);
                }
                else
                    output += "Login failed. Ret:" + ret.ToString() + " ErrorCode:" + errorCode.ToString();

                netMedia.Disconnect();
            }
            else
                output += "Failed to connect.";

            return output;
        }

        private string ReadEdmiRegisters(EdmiMeterSettings settings, string ip, int port)
        {
            string output = string.Empty;
            NetMedia netMedia = new NetMedia(ip,port);
            if (netMedia.Connect())
            {
                EdmiMeterHandler edmi = new EdmiMeterHandler(settings, netMedia);
                EdmiErrorCode errorCode;

                // Login
                MediaError ret = edmi.Login(out errorCode);
                if (ret == MediaError.None && errorCode == EdmiErrorCode.None)
                {
                    // Voltage Registers
                    var phaseAVoltage = EdmiRegister.CreatePhaseAVoltageRegister();
                    var phaseBVoltage = EdmiRegister.CreatePhaseBVoltageRegister();
                    var phaseCVoltage = EdmiRegister.CreatePhaseCVoltageRegister();
                    // Edmi Voltage Multiplier and Divisor Registers
                    var voltageMultiplier = EdmiRegister.CreateVoltageMultiplierRegister();
                    var voltageDivisor = EdmiRegister.CreateVoltageDivisorRegister();
                    // Current Registers
                    var phaseACurrent = EdmiRegister.CreatePhaseACurrentRegister();
                    var phaseBCurrent = EdmiRegister.CreatePhaseBCurrentRegister();
                    var phaseCCurrent = EdmiRegister.CreatePhaseCCurrentRegister();
                    // Edmi Current Multiplier and Divisor Registers
                    var currentMultiplier = EdmiRegister.CreateCurrentMultiplierRegister();
                    var currentDivisor = EdmiRegister.CreateCurrentDivisorRegister();
                    // Angle Registers
                    var phaseAAngle = EdmiRegister.CreatePhaseAAngleRegister();
                    var phaseBAngle = EdmiRegister.CreatePhaseBAngleRegister();
                    var phaseCAngle = EdmiRegister.CreatePhaseCAngleRegister();
                    // Misc
                    var powerFactor = EdmiRegister.CreatePowerFactorRegister();
                    var frequency = EdmiRegister.CreateFrequencyRegister();

                    EdmiRegisterCollection regs = new EdmiRegisterCollection() {
                        phaseAVoltage,
                        phaseBVoltage,
                        phaseCVoltage,
                        voltageMultiplier,
                        voltageDivisor,
                        phaseACurrent,
                        phaseBCurrent,
                        phaseCCurrent,
                        currentMultiplier,
                        currentDivisor,
                        powerFactor,
                        frequency
                    };
                    ret = edmi.ReadRegisters(regs, out errorCode);
                    if (ret == MediaError.None && errorCode == EdmiErrorCode.None)
                    {
                        // Read Success
                        phaseAVoltage.ApplyTransformRatio(voltageMultiplier, voltageDivisor);
                        phaseAVoltage.ApplyTransformRatio(settings.PT);
                        phaseBVoltage.ApplyTransformRatio(voltageMultiplier, voltageDivisor);
                        phaseBVoltage.ApplyTransformRatio(settings.PT);
                        phaseCVoltage.ApplyTransformRatio(voltageMultiplier, voltageDivisor);
                        phaseCVoltage.ApplyTransformRatio(settings.PT);

                        phaseACurrent.ApplyTransformRatio(currentMultiplier, currentDivisor);
                        phaseACurrent.ApplyTransformRatio(settings.CT);
                        phaseBCurrent.ApplyTransformRatio(currentMultiplier, currentDivisor);
                        phaseBCurrent.ApplyTransformRatio(settings.CT);
                        phaseCCurrent.ApplyTransformRatio(currentMultiplier, currentDivisor);
                        phaseCCurrent.ApplyTransformRatio(settings.CT);

                        // Voltages
                        output += "Voltage:\r\n";

                        output += "\tPhase A:\t";
                        output += phaseAVoltage.ErrorCode == EdmiErrorCode.None ? phaseAVoltage.Value.ToString("0.00") + "\t" + phaseAVoltage.Unit.ToString() : phaseAVoltage.ErrorCode.ToString();
                        output += "\r\n";

                        output += "\tPhase B:\t";
                        output += phaseBVoltage.ErrorCode == EdmiErrorCode.None ? phaseBVoltage.Value.ToString("0.00") + "\t" + phaseBVoltage.Unit.ToString() : phaseBVoltage.ErrorCode.ToString();
                        output += "\r\n";

                        output += "\tPhase C:\t";
                        output += phaseCVoltage.ErrorCode == EdmiErrorCode.None ? phaseCVoltage.Value.ToString("0.00") + "\t" + phaseCVoltage.Unit.ToString() : phaseCVoltage.ErrorCode.ToString();
                        output += "\r\n";

                        // Currents
                        output += "Current:\r\n";

                        output += "\tPhase A:\t";
                        output += phaseACurrent.ErrorCode == EdmiErrorCode.None ? phaseACurrent.Value.ToString("0.00") + "\t" + phaseACurrent.Unit.ToString() : phaseACurrent.ErrorCode.ToString();
                        output += "\r\n";

                        output += "\tPhase B:\t";
                        output += phaseBCurrent.ErrorCode == EdmiErrorCode.None ? phaseBCurrent.Value.ToString("0.00") + "\t" + phaseBCurrent.Unit.ToString() : phaseBCurrent.ErrorCode.ToString();
                        output += "\r\n";

                        output += "\tPhase C:\t";
                        output += phaseCCurrent.ErrorCode == EdmiErrorCode.None ? phaseCCurrent.Value.ToString("0.00") + "\t" + phaseCCurrent.Unit.ToString() : phaseCCurrent.ErrorCode.ToString();
                        output += "\r\n";

                        // Angles
                        output += "Angle:\r\n";

                        output += "\tPhase A:\t";
                        output += phaseAAngle.ErrorCode == EdmiErrorCode.None ? phaseAAngle.Value.ToString("0.00") + "\t" + phaseAAngle.Unit.ToString() : phaseAAngle.ErrorCode.ToString();
                        output += "\r\n";

                        output += "\tPhase B:\t";
                        output += phaseBAngle.ErrorCode == EdmiErrorCode.None ? phaseBAngle.Value.ToString("0.00") + "\t" + phaseBAngle.Unit.ToString() : phaseBAngle.ErrorCode.ToString();
                        output += "\r\n";

                        output += "\tPhase C:\t";
                        output += phaseCAngle.ErrorCode == EdmiErrorCode.None ? phaseCAngle.Value.ToString("0.00") + "\t" + phaseCAngle.Unit.ToString() : phaseCAngle.ErrorCode.ToString();
                        output += "\r\n";

                        // Misc
                        output += "Misc:\r\n";

                        output += "\tFreq:\t";
                        output += frequency.ErrorCode == EdmiErrorCode.None ? frequency.Value.ToString("0.00") + "\t" + frequency.Unit.ToString() : frequency.ErrorCode.ToString();
                        output += "\r\n";

                        output += "\tPF:\t";
                        output += powerFactor.ErrorCode == EdmiErrorCode.None ? powerFactor.Value.ToString("0.00") + "\t" + powerFactor.Unit.ToString() : powerFactor.ErrorCode.ToString();
                        output += "\r\n";
                    }
                    else
                        output += "Read Registers Failed. Ret:" + ret.ToString() + " ErrorCode:" + errorCode.ToString();

                    // Logout
                    ret = edmi.Logout(out errorCode);
                }
                else
                    output += "Login Failed. Ret:" + ret.ToString() + " ErrorCode:" + errorCode.ToString();

                netMedia.Disconnect();
            }
            else
                output += "Failed to connect.";

            return output;
        }

        private string ReadEdmiProfile(EdmiMeterSettings settings, string ip, int port)
        {
            string output = string.Empty;
            NetMedia netMedia = new NetMedia(ip, port);
            if (netMedia.Connect())
            {
                EdmiMeterHandler edmi = new EdmiMeterHandler(settings, netMedia);
                EdmiErrorCode errorCode;

                // Login
                MediaError ret = edmi.Login(out errorCode);
                if (ret == MediaError.None && errorCode == EdmiErrorCode.None)
                {
                    var now = DateTime.Now;
                    DateTime fromDate = new DateTime(now.Year, now.Month, now.Day, 1, 0, 0);
                    ret = edmi.ReadProfile(EdmiSurvey.LS2, fromDate, null, out EdmiProfile profile, out errorCode);

                    if (ret == MediaError.None && errorCode == EdmiErrorCode.None)
                    {

                        output += "\r\n";
                        output += PrintLine();
                        output += " Name: " + profile.Name + "\tInterval (seconds): " + profile.Interval.ToString() + "\r\n";
                        output += PrintLine();
                        string[] columns = new string[3 + profile.ColumnsCount];
                        columns[0] = "Date";
                        columns[1] = "Status Code";
                        columns[2] = "Status";
                        for (int i = 0; i < profile.ColumnsCount; i++)
                        {
                            var column = profile.Columns[i];
                            columns[3 + i] = column.Name + '(' + column.Unit + ')';
                        }
                        output += PrintRow(columns);
                        output += PrintLine();

                        // Use Profile founded Date
                        DateTime pDate = profile.FromDate;
                        for (int row = 0; row < profile.RowsCount; row++)
                        {
                            columns[0] = pDate.ToString("yyyy/MM/dd HH:mm:ss");
                            columns[1] = profile[row].StatusCode.ToString("X4");
                            columns[2] = profile[row].Status.ErrorReadingRegisters ? "Error" : "Valid";
                            for (int column = 0; column < profile.ColumnsCount; column++)
                                columns[3 + column] = profile[row][column].ToFloat().ToString();
                            output += PrintRow(columns);
                            output += PrintLine();
                            pDate = pDate.AddSeconds(profile.Interval);
                        }
                        output += "\r\n";
                    }
                    else
                        output += "Read Profile Failed. Ret:" + ret.ToString() + " ErrorCode:" + errorCode.ToString();

                    // Logout
                    ret = edmi.Logout(out errorCode);
                }
                else
                    output += "Login Failed. Ret:" + ret.ToString() + " ErrorCode:" + errorCode.ToString();

                netMedia.Disconnect();
            }
            else
                output += "Failed to connect";

            return output;
        }

        public MainWindow()
        {
            InitializeComponent();
        }

        private DlmsMeterSettings ActarisReadSettings(out string ip, out int port)
        {
            ip = actarisIPTextBox.Text;
            port = int.Parse(actarisPortTextBox.Text);
            return new DlmsMeterSettings()
            {
                SerialNumber = uint.Parse(actarisSerialTextBox.Text),
                ClientID = (byte)int.Parse(actarisClientIDTextBox.Text),
                Password = actarisPasswordTextBox.Text,
                PhysicalAddress = uint.Parse(actarisPhysicalAddrTextBox.Text),
                LogicalAddress = uint.Parse(actarisLogicalAddrTextBox.Text),
                CT = float.Parse(actarisCTTextBox.Text),
                PT = float.Parse(actarisPTTextBox.Text)
            };
        }

        private void ActarisReadRegistersButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                var settings = ActarisReadSettings(out string ip, out int port);
                actarisOutputTextBlock.Text = "Reading Registers ...\r\n\r\n";
                Task.Run(() =>
                {
                    string output = ReadActarisRegisters(settings, ip, port);
                    Dispatcher.Invoke(() =>
                    {
                        actarisOutputTextBlock.Text += output;
                    });
                });
            }
            catch(Exception ex)
            {
                //MessageBox.Show("Invalid actaris settings.");
                actarisOutputTextBlock.Text = ex.ToString();
            }
        }

        private void ActarisReadProfileButton_Click(object sender, RoutedEventArgs e)
        {   
            try
            {
                var settings = ActarisReadSettings(out string ip, out int port);
                actarisOutputTextBlock.Text = "Reading Profile ...\r\n\r\n";
                Task.Run(() =>
                {
                    string output = ReadActarisProfile(settings, ip, port);
                    Dispatcher.Invoke(() =>
                    {
                        actarisOutputTextBlock.Text += output;
                    });
                });
            }
            catch(Exception ex)
            {
                actarisOutputTextBlock.Text = ex.ToString();
                //MessageBox.Show("Invalid actaris settings.");
            }
        }

        private EdmiMeterSettings EdmiReadSettings(out string ip, out int port)
        {
            ip = edmiIPTextBox.Text;
            port = int.Parse(edmiPortTextBox.Text, CultureInfo.InvariantCulture);
            return new EdmiMeterSettings()
            {
                SerialNumber = uint.Parse(edmiSerialTextBox.Text, CultureInfo.InvariantCulture),
                Username = edmiUsernameTextBox.Text,
                Password = edmiPasswordTextBox.Text,
                CT = float.Parse(edmiCTTextBox.Text,CultureInfo.InvariantCulture),
                PT = float.Parse(edmiPTTextBox.Text, CultureInfo.InvariantCulture)
            };
        }

        private void EdmiReadRegistersButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                var settings = EdmiReadSettings(out string ip, out int port);
                edmiOutputTextBlock.Text = "Reading Registers ...\r\n\r\n";
                Task.Run(() =>
                {
                    string output = ReadEdmiRegisters(settings, ip, port);
                    Dispatcher.Invoke(() =>
                    {
                        edmiOutputTextBlock.Text += output;
                    });
                });
            }
            catch(Exception ex)
            {
                //MessageBox.Show("Invalid edmi settings.");
                edmiOutputTextBlock.Text = ex.ToString();
            }
        }

        private void EdmiReadProfileButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                var settings = EdmiReadSettings(out string ip, out int port);
                edmiOutputTextBlock.Text = "Reading Profile ...\r\n\r\n";
                Task.Run(() =>
                {
                    string output = ReadEdmiProfile(settings, ip, port);
                    Dispatcher.Invoke(() =>
                    {
                        edmiOutputTextBlock.Text += output;
                    });
                });
            }
            catch(Exception ex)
            {
                //MessageBox.Show("Invalid edmi settings.");
                edmiOutputTextBlock.Text = ex.ToString();
            }
        }
    }
}
