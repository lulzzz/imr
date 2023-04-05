// Authored by: MohammadAmin Mohammadi (https://github.com/mamohammadi)

using System;
using IMR.Protocols;
using IMR.MediaInterfaces;
using IMR.Types;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Linq;

namespace IMR.Clients.Console
{
    class Program
    {
        static int tableWidth = 77;
        static void PrintLine()
        {
            System.Console.WriteLine(new string('-', tableWidth));
        }
        static void PrintRow(params string[] columns)
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
            System.Console.WriteLine(row);

            if (overlaped)
                PrintRow(columns);
        }

        static string AlignCentre(string text, int width)
        {
            if (string.IsNullOrEmpty(text))
                return new string(' ', width);
            return text.PadRight(width - (width - text.Length) / 2).PadLeft(width);
        }

        private static void ReadEdmiRegisters()
        {
            NetMedia netMedia = new NetMedia("ip", /* Port */ 0);
            System.Console.WriteLine("Connecting ...");
            if (netMedia.Connect())
            {
                System.Console.WriteLine("Connected");
                EdmiMeterSettings settings = new EdmiMeterSettings()
                {
                    SerialNumber = 0,
                    CT = 1.0f,
                    PT = 1.0f,
                    Username = "Username",
                    Password = "Password"
                };

                EdmiMeterHandler edmi = new EdmiMeterHandler(settings, netMedia);
                EdmiErrorCode errorCode;

                // Login
                System.Console.WriteLine("Login ...");
                MediaError ret = edmi.Login(out errorCode);
                if (ret == MediaError.None && errorCode == EdmiErrorCode.None)
                {
                    System.Console.WriteLine("Login success");

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

                    System.Console.WriteLine("Reading registers ...");
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

                        System.Console.WriteLine(
                            "\nVoltage:\nA: {0}\tB: {1}\tC: {2}\n" +
                            "Current:\nA: {3}\tB: {4}\tC: {5}\n" +
                            "Angle:\nA: {6}\tB: {7}\tC: {8}\n" +
                            "Power Factor: {9}\nFrequency: {10}\n\n",
                            phaseAVoltage.ErrorCode == EdmiErrorCode.None ? phaseAVoltage.Value.ToString() : phaseAVoltage.ErrorCode.ToString(),
                            phaseBVoltage.ErrorCode == EdmiErrorCode.None ? phaseBVoltage.Value.ToString() : phaseBVoltage.ErrorCode.ToString(),
                            phaseCVoltage.ErrorCode == EdmiErrorCode.None ? phaseCVoltage.Value.ToString() : phaseCVoltage.ErrorCode.ToString(),
                            phaseACurrent.ErrorCode == EdmiErrorCode.None ? phaseACurrent.Value.ToString() : phaseACurrent.ErrorCode.ToString(),
                            phaseBCurrent.ErrorCode == EdmiErrorCode.None ? phaseBCurrent.Value.ToString() : phaseBCurrent.ErrorCode.ToString(),
                            phaseCCurrent.ErrorCode == EdmiErrorCode.None ? phaseCCurrent.Value.ToString() : phaseCCurrent.ErrorCode.ToString(),
                            phaseAAngle.ErrorCode == EdmiErrorCode.None ? phaseAAngle.Value.ToString() : phaseAAngle.ErrorCode.ToString(),
                            phaseBAngle.ErrorCode == EdmiErrorCode.None ? phaseBAngle.Value.ToString() : phaseBAngle.ErrorCode.ToString(),
                            phaseCAngle.ErrorCode == EdmiErrorCode.None ? phaseCAngle.Value.ToString() : phaseCAngle.ErrorCode.ToString(),
                            powerFactor.ErrorCode == EdmiErrorCode.None ? powerFactor.Value.ToString() : powerFactor.ErrorCode.ToString(),
                            frequency.ErrorCode == EdmiErrorCode.None ? frequency.Value.ToString() : frequency.ErrorCode.ToString());
                    }
                    else
                        System.Console.WriteLine("Ret:" + ret.ToString() + " ErrorCode:" + errorCode.ToString());

                    // Logout
                    ret = edmi.Logout(out errorCode);
                    System.Console.WriteLine("Logout");
                }
                else
                {
                    System.Console.WriteLine("Ret:" + ret.ToString() + " ErrorCode:" + errorCode.ToString());
                    System.Console.WriteLine("Login failed");
                }

                netMedia.Disconnect();
            }
            else
                System.Console.WriteLine("Failed to connect");
        }

        private static void ReadEdmiProfile()
        {
            NetMedia netMedia = new NetMedia("ip", /* Port */ 0);
            System.Console.WriteLine("Connecting ...");
            if (netMedia.Connect())
            {
                System.Console.WriteLine("Connected");
                EdmiMeterSettings settings = new EdmiMeterSettings()
                {
                    SerialNumber = 0,
                    CT = 1.0f,
                    PT = 1.0f,
                    Username = "Username",
                    Password = "Password"
                };

                EdmiMeterHandler edmi = new EdmiMeterHandler(settings, netMedia);
                EdmiErrorCode errorCode;

                // Login
                System.Console.WriteLine("Login ...");
                MediaError ret = edmi.Login(out errorCode);
                if (ret == MediaError.None && errorCode == EdmiErrorCode.None)
                {
                    System.Console.WriteLine("Login success");
                    var now = DateTime.Now;
                    DateTime fromDate = new DateTime(now.Year, now.Month, now.Day, 1, 0, 0);
                    ret = edmi.ReadProfile(EdmiSurvey.LS2, fromDate, null, out EdmiProfile profile, out errorCode);

                    if (ret == MediaError.None && errorCode == EdmiErrorCode.None)
                    {

                        System.Console.WriteLine();
                        PrintLine();
                        System.Console.WriteLine("Profile Name: " + profile.Name + "\tInterval (seconds): " + profile.Interval);
                        PrintLine();
                        string[] columns = new string[3 + profile.ColumnsCount];
                        columns[0] = "Date";
                        columns[1] = "Status Code";
                        columns[2] = "Status";
                        for (int i = 0; i < profile.ColumnsCount; i++)
                        {
                            var column = profile.Columns[i];
                            columns[3 + i] = column.Name + '(' + column.Unit + ')';
                        }
                        PrintRow(columns);
                        PrintLine();

                        // Use Profile founded Date
                        DateTime pDate = profile.FromDate;
                        for (int row = 0; row < profile.RowsCount; row++)
                        {
                            columns[0] = pDate.ToShortDateString() + ' ' + pDate.ToShortTimeString();
                            columns[1] = profile[row].StatusCode.ToString("X4");
                            columns[2] = profile[row].Status.ErrorReadingRegisters ? "Error" : "Valid";
                            for (int column = 0; column < profile.ColumnsCount; column++)
                                columns[3 + column] = profile[row][column].ToFloat().ToString();
                            PrintRow(columns);
                            PrintLine();
                            pDate = pDate.AddSeconds(profile.Interval);
                        }
                        System.Console.WriteLine();
                    }
                    else
                        System.Console.WriteLine("Ret:" + ret.ToString() + " ErrorCode:" + errorCode.ToString());

                    // Logout
                    ret = edmi.Logout(out errorCode);
                    System.Console.WriteLine("Logout");
                }
                else
                {
                    System.Console.WriteLine("Ret:" + ret.ToString() + " ErrorCode:" + errorCode.ToString());
                    System.Console.WriteLine("Login failed");
                }

                netMedia.Disconnect();
            }
            else
                System.Console.WriteLine("Failed to connect");
        }

        private static void ReadActarisRegisters()
        {
            NetMedia netMedia = new NetMedia("IP", /* Port */ 0);
            System.Console.WriteLine("Connecting ...");
            if (netMedia.Connect())
            {
                System.Console.WriteLine("Connected");

                DlmsMeterSettings settings = new DlmsMeterSettings()
                {
                    SerialNumber = 0,
                    CT = 1.0f,
                    PT = 1.0f,
                    Password = "Password",
                    ClientID = 0,
                    PhysicalAddress = 0,
                    LogicalAddress = 0
                };

                DlmsMeterHandler dlms = new DlmsMeterHandler(settings, netMedia);

                // Login
                System.Console.WriteLine("Login ...");
                DlmsErrorCode errorCode;
                MediaError ret = dlms.Login(out errorCode);
                if (ret == MediaError.None && errorCode == DlmsErrorCode.None)
                {
                    System.Console.WriteLine("Login success");

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
                    System.Console.WriteLine("Reading registers ...");
                    if (ret == MediaError.None && errorCode == DlmsErrorCode.None)
                    {
                        // Read Success
                        phaseAVoltage.ApplyTransformRatio(settings.PT);
                        phaseBVoltage.ApplyTransformRatio(settings.PT);
                        phaseCVoltage.ApplyTransformRatio(settings.PT);

                        phaseACurrent.ApplyTransformRatio(settings.CT);
                        phaseBCurrent.ApplyTransformRatio(settings.CT);
                        phaseCCurrent.ApplyTransformRatio(settings.CT);

                        System.Console.WriteLine("Phase A Voltage: " + ((phaseAVoltage.ErrorCode == DlmsErrorCode.None) ? phaseAVoltage.Value.ToString() + " " + phaseAVoltage.Unit.Name : phaseAVoltage.ErrorCode.ToString()));
                        System.Console.WriteLine("Phase B Voltage: " + ((phaseBVoltage.ErrorCode == DlmsErrorCode.None) ? phaseBVoltage.Value.ToString() + " " + phaseBVoltage.Unit.Name : phaseBVoltage.ErrorCode.ToString()));
                        System.Console.WriteLine("Phase C Voltage: " + ((phaseCVoltage.ErrorCode == DlmsErrorCode.None) ? phaseCVoltage.Value.ToString() + " " + phaseCVoltage.Unit.Name : phaseCVoltage.ErrorCode.ToString()));

                        System.Console.WriteLine("Phase A Current: " + ((phaseACurrent.ErrorCode == DlmsErrorCode.None) ? phaseACurrent.Value.ToString() + " " + phaseACurrent.Unit.Name : phaseACurrent.ErrorCode.ToString()));
                        System.Console.WriteLine("Phase B Current: " + ((phaseBCurrent.ErrorCode == DlmsErrorCode.None) ? phaseBCurrent.Value.ToString() + " " + phaseBCurrent.Unit.Name : phaseBCurrent.ErrorCode.ToString()));
                        System.Console.WriteLine("Phase C Current: " + ((phaseCCurrent.ErrorCode == DlmsErrorCode.None) ? phaseCCurrent.Value.ToString() + " " + phaseCCurrent.Unit.Name : phaseCCurrent.ErrorCode.ToString()));

                        System.Console.WriteLine("Phase A Angle: " + ((phaseAAngle.ErrorCode == DlmsErrorCode.None) ? phaseAAngle.Value.ToString() + " " + phaseAAngle.Unit.Name : phaseAAngle.ErrorCode.ToString()));
                        System.Console.WriteLine("Phase B Angle: " + ((phaseBAngle.ErrorCode == DlmsErrorCode.None) ? phaseBAngle.Value.ToString() + " " + phaseBAngle.Unit.Name : phaseBAngle.ErrorCode.ToString()));
                        System.Console.WriteLine("Phase C Angle: " + ((phaseCAngle.ErrorCode == DlmsErrorCode.None) ? phaseCAngle.Value.ToString() + " " + phaseCAngle.Unit.Name : phaseCAngle.ErrorCode.ToString()));

                        System.Console.WriteLine("Frequency: " + ((frequency.ErrorCode == DlmsErrorCode.None) ? frequency.Value.ToString() + " " + frequency.Unit.Name : frequency.ErrorCode.ToString()));
                        System.Console.WriteLine("Power Factor: " + ((powerFactor.ErrorCode == DlmsErrorCode.None) ? powerFactor.Value.ToString() + " " + powerFactor.Unit.Name : powerFactor.ErrorCode.ToString()));
                        System.Console.WriteLine("Temperature: " + ((temperature.ErrorCode == DlmsErrorCode.None) ? temperature.Value.ToString() + " " + temperature.Unit.Name : temperature.ErrorCode.ToString()));
                    }
                    else
                        System.Console.WriteLine("Ret:" + ret.ToString() + " ErrorCode:" + errorCode.ToString());

                    // Logout
                    ret = dlms.Logout(out errorCode);
                    System.Console.WriteLine("Logout");
                }
                else
                {
                    System.Console.WriteLine("Ret:" + ret.ToString() + " ErrorCode:" + errorCode.ToString());
                    System.Console.WriteLine("Login failed");
                }

                netMedia.Disconnect();
            }
            else
                System.Console.WriteLine("Failed to connect");
        }

        private static void ReadActarisProfile()
        {
            NetMedia netMedia = new NetMedia("IP", /* Port */ 0);
            System.Console.WriteLine("Connecting ...");
            if (netMedia.Connect())
            {
                System.Console.WriteLine("Connected");

                DlmsMeterSettings settings = new DlmsMeterSettings()
                {
                    SerialNumber = 0,
                    CT = 1.0f,
                    PT = 1.0f,
                    Password = "Password",
                    ClientID = 0,
                    PhysicalAddress = 0,
                    LogicalAddress = 0
                };

                DlmsMeterHandler dlms = new DlmsMeterHandler(settings, netMedia);

                // Login
                System.Console.WriteLine("Login ...");
                DlmsErrorCode errorCode;
                MediaError ret = dlms.Login(out errorCode);
                if (ret == MediaError.None && errorCode == DlmsErrorCode.None)
                {
                    // Read Profile Channels
                    System.Console.WriteLine("Reading channels ...");
                    dlms.ReadProfileChannels(DlmsSurvey.LS2, out DlmsProfileChannelInfo[] channels, out errorCode);

                    if (ret == MediaError.None && errorCode == DlmsErrorCode.None)
                    {
                        // Choose Selected Channels
                        DlmsProfileChannelInfo[] sChannels = new DlmsProfileChannelInfo[5];
                        sChannels[0] = channels[0];
                        sChannels[1] = channels[1];
                        sChannels[2] = channels[2];
                        sChannels[3] = channels[3];
                        sChannels[4] = channels[4];

                        DlmsProfile profile;
                        var now = DateTime.Now;
                        DateTime fromDate = new DateTime(now.Year, now.Month, now.Day - 1, 0, 15, 0);
                        System.Console.WriteLine("Reading Profile ...");
                        ret = dlms.ReadProfile(DlmsSurvey.LS2, fromDate, null, sChannels, out profile, out errorCode);

                        if (ret == MediaError.None && errorCode == DlmsErrorCode.None)
                        {
                            // Actaris does not implemented profile record interval attribute so we use default 900 second for profile 1
                            // If the interval readed from meter does not match the actual value, Just igonre it and set it manually.
                            int interval = profile.Interval;

                            // Use profile dounded date
                            DateTime pDate = profile.FromDate;
                            System.Console.WriteLine();
                            PrintLine();
                            System.Console.WriteLine("Profile Name: " + profile.Name + "\tInterval (seconds): " + interval);
                            PrintLine();
                            string[] columns = new string[3 + profile.ColumnsCount];
                            columns[0] = "Date";
                            columns[1] = "Status Code";
                            columns[2] = "Status";
                            for (int i = 0; i < profile.ColumnsCount; i++)
                            {
                                var column = profile.Columns[i];
                                columns[3 + i] = column.Name; // + column.Unit.Name; // + '(' + column.Unit + ')' + '\t';
                            }
                            PrintRow(columns);
                            PrintLine();
                            for (int row = 0; row < profile.RowsCount; row++)
                            {
                                columns[0] = pDate.ToShortDateString() + ' ' + pDate.ToShortTimeString();
                                columns[1] = "0000";
                                columns[2] = "Valid";
                                for (int column = 0; column < profile.ColumnsCount; column++)
                                {
                                    int? v = profile[row][column].ToInt32();
                                    columns[3 + column] = (v == null) ? "Null" : v.ToString();
                                }
                                PrintRow(columns);
                                PrintLine();
                                pDate = pDate.AddSeconds(profile.Interval);
                            }
                            System.Console.WriteLine();
                        }
                        else
                            System.Console.WriteLine("Ret:" + ret.ToString() + " ErrorCode:" + errorCode.ToString());
                    }
                    else
                        System.Console.WriteLine("Ret:" + ret.ToString() + " ErrorCode:" + errorCode.ToString());

                    // Logout
                    ret = dlms.Logout(out errorCode);
                    System.Console.WriteLine("Logout");
                }
                else
                {
                    System.Console.WriteLine("Ret:" + ret.ToString() + " ErrorCode:" + errorCode.ToString());
                    System.Console.WriteLine("Login failed");
                }

                netMedia.Disconnect();
            }
            else
                System.Console.WriteLine("Failed to connect");
        }

        [DllImport("kernel32.dll", ExactSpelling = true)]
        private static extern IntPtr GetConsoleWindow();
        private static IntPtr ThisConsole = GetConsoleWindow();
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);
        private const int HIDE = 0;
        private const int MAXIMIZE = 3;
        private const int MINIMIZE = 6;
        private const int RESTORE = 9;
        static void Main(string[] args)
        {
            System.Console.SetWindowSize(System.Console.LargestWindowWidth, System.Console.LargestWindowHeight);
            ShowWindow(ThisConsole, MAXIMIZE);

            ReadEdmiRegisters();
            //ReadEdmiProfile();

            //ReadActarisRegisters();
            //ReadActarisProfile();

            System.Console.ReadKey();
        }
    }
}
