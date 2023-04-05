// Authored by: MohammadAmin Mohammadi (https://github.com/mamohammadi)

using IMR.Log;
using IMR.Protocols;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace IMR.Types
{
    public enum DlmsErrorCode : byte
    {
        None = 0,
        HardwareFault = 1,
        TemporaryFailure = 2,
        ReadWriteDenied = 3,
        ObjectUndefined = 4,
        ObjectClassInconsistent = 9,
        ObjectUnavailable = 11,
        TypeUnmatched = 12,
        ScopeOfAccessViolated = 13,
        DataBlockUnavailable = 14,
        LongGetAborted = 15,
        NoLongGetInProgress = 16,
        LongSetAborted = 17,
        NoLongSetInProgress = 18,
        DataBlockNumberInvalid = 19,
        OtherReason = 250,

        ParseError = 100,
        InvalidFrame = 101,
        HCSError = 102,
        FCSError = 103,
        AuthenticationFailed = 104,
        NotImplemented = 105,
        TypeError = 106,
        SRError = 107

        //InvalidParameter,
        //NotInitialized,
        //OutOfMemory,
        //NotReply,
        //FrameRejected,
        //InvalidLogicalName,
        //InvalidClientAddress,
        //InvalidServerAddress,
        //InvalidDataFormat,
        //InvalidVersionNumber,
        //ClientAddressNotMatch,
        //ServerAddressNot_match,
        //WrongCrc,
        //InvalidResponse,
        //InvalidTag,
        //EncodingFailed,
        //RejectedPermament,
        //RejectedTransient,
        //NoReasonGiven,
        //ApplicationContextNameNotSupported,
        //AuthenticationMechanismNameNotRecognised,
        //AuthenticationMechanismNameRequired,
        //AuthenticationFailure,
        //AuthenticationRequired,


        //Unknown,
        //SendFailed,
        //ReceiveFailed,
        //NotImplemented,
        //MediaError
    }

    public class DlmsMeterSettings
    {
        public uint SerialNumber { get; set; }
        public string Password { get; set; }
        public byte ClientID { get; set; }
        public uint PhysicalAddress { get; set; }
        public uint LogicalAddress { get; set; }
        public float CT { get; set; }
        public float PT { get; set; }
    }

    [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Ansi, Size = SIZE)]
    internal struct DlmsDateTime
    {
        public const int SIZE = 33;

        [FieldOffset(0)]
        internal byte IsNull;

        [FieldOffset(1)]
        internal short Year;

        [FieldOffset(3)]
        internal byte Month;

        [FieldOffset(4)]
        internal byte Day;

        [FieldOffset(5)]
        internal byte Hour;

        [FieldOffset(6)]
        internal byte Minute;

        [FieldOffset(7)]
        internal byte Second;

        internal DlmsDateTime(byte isNull, short year, byte month, byte day, byte hour, byte minute, byte second)
        {
            IsNull = isNull;
            Year = year;
            Month = month;
            Day = day;
            Hour = hour;
            Minute = minute;
            Second = second;
        }

        internal static DlmsDateTime FromDateTime(DateTime? dateTime)
        {
            if (dateTime == null)
                return new DlmsDateTime(1, unchecked((short)0xFFFF), 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

            DateTime value = dateTime.Value;
            return new DlmsDateTime(0, (short)value.Year, (byte)value.Month, (byte)value.Day, (byte)value.Hour, (byte)value.Minute, (byte)value.Second);
        }

        internal static DateTime? ToDateTime(DlmsDateTime dateTime)
        {
            if (dateTime.IsNull == 1)
                return null;

            return new DateTime(dateTime.Year, dateTime.Month, dateTime.Day, dateTime.Hour, dateTime.Minute, dateTime.Second);
        }
    }

    public enum DlmsType : byte
    {
        Null = 0x00,                // Null data
        Boolean = 0x03,             // Boolean
        BitString = 0x04,           // An ordered sequence of boolean values
        DoubleLong = 0x05,          // Integer32
        DoubleLongUnsigned = 0x06,  // Unsigned32
        OctetString = 0x09,         // An ordered sequence of octets (8 bit bytes)
        VisibleString = 0x0a,       // An ordered sequence of asci characters
        Utf8String = 0x0c,          // An ordered sequence of characters encoded utf-8
        Bcd = 0x0d,                 // Binary coded decimal
        Integer = 0x0f,             // Integer8
        Long = 0x10,                // Integer16
        Unsigned = 0x11,            // Unsinged8
        LongUnsigend = 0x12,        // Unsigned16
        Long64 = 0x14,              // Integer64
        Long64Unsigned = 0x15,      // Unsigned64
        Enum = 0x16,                // Enum
        Float32 = 0x17,             // Octet string size = 4
        Float64 = 0x18,             // Octet string size = 8
        DateTime = 0x19,            // Octet string size = 12
        Date = 0x1a,                // Octet string size = 5
        Time = 0x1b,                // Octet string size = 4
        Array = 0x01,               // Array
        Structure = 0x02,           // Structure
        CompactArray = 0x13,        // Compact array
    }

    public class DlmsUnit
    {
        public string Symbol { get; }
        public string Quantity { get; }
        public string Name { get; }

        public DlmsUnit(string symbol, string quantity, string name)
        {
            Symbol = symbol;
            Quantity = quantity;
            Name = name;
        }

        internal static DlmsUnit FromDlmsUnitCode(byte unitCode)
        {
            if (unitCode >= 0 && unitCode <= 70)
                return unites[unitCode];
            if (unitCode == 255)
                return new DlmsUnit("count", "Not Unit, Unit Less, Count", "");
            return unites[0];
        }

        private static DlmsUnit[] unites = new DlmsUnit[]
        {
           /*00*/ new DlmsUnit("Unknown","Unknown","Unknown"), 
           /*01*/ new DlmsUnit("a", "Time", "Year"),
           /*02*/ new DlmsUnit("mo", "Time", "Month"),
           /*03*/ new DlmsUnit("wk", "Time", "Week"),
           /*04*/ new DlmsUnit("d","Time", "Day"),
           /*05*/ new DlmsUnit("h", "Time", "Hour"),
           /*06*/ new DlmsUnit("min", "Time", "Minute"),
           /*07*/ new DlmsUnit("s", "Time (t)", "Second"),
           /*08*/ new DlmsUnit("°","Phase Angle", "Degree"),
           /*09*/ new DlmsUnit("°C", "Temperature (T)", "Degree Celsius"),
           /*10*/ new DlmsUnit("currency", "Local Currency", ""),
           /*11*/ new DlmsUnit("m", "Length (l)", "Meter"),
           /*12*/ new DlmsUnit("m/s", "Speed (v)", "Meter Per Second"),
           /*13*/ new DlmsUnit("m^3","Volume (V)", "Cubic Meter"),
           /*14*/ new DlmsUnit("m^3", "Corrected Volume", "Cubic Meter"),
           /*15*/ new DlmsUnit("m^3/h", "Volume Flux", "Cubic Meter Per Hour"),
           /*16*/ new DlmsUnit("m^3/h", "Corrected Volume Flux","Cubic Meter Per Hour"),
           /*17*/ new DlmsUnit("m^3/d", "Volume Flux",""),
           /*18*/ new DlmsUnit("m^3/d","Corrected Volume Flux",""),
           /*19*/ new DlmsUnit("l", "Volume", "Liter"),
           /*20*/ new DlmsUnit("kg", "Mass (m)","Kilogram"),
           /*21*/ new DlmsUnit("N", "Force (F)", "Newton"),
           /*22*/ new DlmsUnit("Nm", "Energy", "Newton Meter"),
           /*23*/ new DlmsUnit("Pa", "Pressure (p)", "Pascal"),
           /*24*/ new DlmsUnit("bar", "Pressure (p)", "Bar"),
           /*25*/ new DlmsUnit("J", "Energy", "Joule"),
           /*26*/ new DlmsUnit("J/h", "Thermal Power", "Joule Per Hour"),
           /*27*/ new DlmsUnit("W", "Active Power (P)", "Watt"),
           /*28*/ new DlmsUnit("VA", "Apparent Power (S)", "Volt Ampere"),
           /*29*/ new DlmsUnit("var", "Reactive Power (Q)", "Var"),
           /*30*/ new DlmsUnit("Wh", "Active Energy", "Watt Hour"),
           /*31*/ new DlmsUnit("VAh", "Apparent Energy", "Volt Ampere Hour"),
           /*32*/ new DlmsUnit("varh", "Reactive Energy", "Var Hour"),
           /*33*/ new DlmsUnit("A", "Current (I)", "Ampere"),
           /*34*/ new DlmsUnit("C", "Electrical Charge (Q)", "Coulomb"),
           /*35*/ new DlmsUnit("V", "Voltage (U)", "Volt"),
           /*36*/ new DlmsUnit("V/m", "Electric Field Strength (E)", "Volt Per Meter"),
           /*37*/ new DlmsUnit("F", "Capacitance (C)", "Farad"),
           /*38*/ new DlmsUnit("Ω", "Resistance (R)", "Ohm"),
           /*39*/ new DlmsUnit("Ωm^2/m", "Resistivity (ρ)", ""),
           /*40*/ new DlmsUnit("Wb", "Magnetic Flux (Φ)", "Weber"),
           /*41*/ new DlmsUnit("T", "Magnetic Flux Density (B)", "Tesla"),
           /*42*/ new DlmsUnit("A/m", "Magnetic Field Strength (H)", "Ampere Per Meter"),
           /*43*/ new DlmsUnit("H", "Inductance (L)" ,"Henry"),
           /*44*/ new DlmsUnit("Hz", "Frequency (f, ω)", "Hertz"),
           /*45*/ new DlmsUnit("1/(Wh)", "R_W, Active Energy Meter Constant Or Pulse Value", ""),
           /*46*/ new DlmsUnit("1/(varh)", "R_B, Reactive Energy Meter Constant Or Pulse Value",""),
           /*47*/ new DlmsUnit("1/(VAh)", "R_S, Apparent Energy Meter Constant Or Pulse Value",""),
           /*48*/ new DlmsUnit("V^2h","Volt Squared Hour", "Volt Squared Hours" ),
           /*49*/ new DlmsUnit("A^2h", "Ampere Squared Hour", "Ampere Squared Hours"),
           /*50*/ new DlmsUnit("kg/s", "Mass Flux","Kilogram Per Second"),
           /*51*/ new DlmsUnit("S, mho", "Conductance", "Siemens"),
           /*52*/ new DlmsUnit("K", "Temperature (T)", "Kelvin"),
           /*53*/ new DlmsUnit("1/(V^2h)", "R_U2h, Volt Squarded Hour Meter Constant Or Pulse Value",""),
           /*54*/ new DlmsUnit("1/(A^2h)", "R_I2h, Ampere Squarded Hour Meter Constant Or Pulse Value",""),
           /*55*/ new DlmsUnit("1/m^3", "R_v, Meter Constant Or Pulse Value (Volume)", ""),
           /*56*/ new DlmsUnit("%", "Percentage", "Percentage"),
           /*57*/ new DlmsUnit("Ah", "Ampere Hours", "Ampere Hour"),
           /*58*/ new DlmsUnit("","",""),
           /*59*/ new DlmsUnit("","",""),
           /*60*/ new DlmsUnit("Wh/m^3","Energy Per Volume","3.6*10^3 J/m^3"),
           /*61*/ new DlmsUnit("J/m^3", "Calorific Value, Wobbe",""),
           /*62*/ new DlmsUnit("Mol %", "Molar Fraction Of Gas Composition", "Mole Percent"),
           /*63*/ new DlmsUnit("g/m^3", "Mass Density, Quantity Of Material", ""),
           /*64*/ new DlmsUnit("Pa s", "Dynamic Viscosity", "Pascal Second"),
           /*65*/ new DlmsUnit("J/kg", "Specific Energy, The Amount Of Energy Per Unit Of Mass Of A Substance", "Joule Per Kilogram"),
           /*66*/ new DlmsUnit("","",""),
           /*67*/ new DlmsUnit("","",""),
           /*68*/ new DlmsUnit("","",""),
           /*69*/ new DlmsUnit("","",""),
           /*70*/ new DlmsUnit("dBm","Signal Strength","")
        };
    }

    internal static class DlmsTypeHelper
    {
        internal static DataType GetDataType(this DlmsType type)
        {
            switch (type)
            {
                case DlmsType.Boolean:
                    return DataType.Boolean;
                case DlmsType.DoubleLong:
                    return DataType.Int32;
                case DlmsType.DoubleLongUnsigned:
                    return DataType.UInt32;
                case DlmsType.Integer:
                    return DataType.Byte;
                case DlmsType.Long:
                    return DataType.Int16;
                case DlmsType.Unsigned:
                    return DataType.Byte;
                case DlmsType.LongUnsigend:
                    return DataType.UInt16;
                case DlmsType.Long64:
                    return DataType.Int64;
                case DlmsType.Long64Unsigned:
                    return DataType.UInt64;
                case DlmsType.Float32:
                    return DataType.Float;
                case DlmsType.Float64:
                    return DataType.Double;
                case DlmsType.DateTime:
                    return DataType.DateTime;
                case DlmsType.Date:
                    return DataType.Date;
                case DlmsType.Time:
                    return DataType.Time;
                default:
                    return DataType.Unknown;
            }
        }
    }

    [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Ansi, Size = SIZE)]
    internal struct DlmsRegisterInfo
    {
        public const int SIZE = VALUE_OFFSET + 25;
        public const int ADDRESS_OFFSET = 0;
        public const int VALUE_OFFSET = 11;

        //[FieldOffset(0)]
        //internal fixed byte Address[6];

        [FieldOffset(6)]
        internal DlmsType Type;

        [FieldOffset(7)]
        internal DlmsErrorCode ErrorCode;

        [FieldOffset(8)]
        internal sbyte Scaler;

        [FieldOffset(9)]
        internal byte UnitCode;

        [FieldOffset(10)]
        internal byte IsNull;

        //[FieldOffset(11)]
        //internal fixed byte Value[25];
    }

    public abstract class DlmsRegister
    {
        internal unsafe static void ParseLogicalAddress(string logicalAddress, byte* address)
        {
            string[] list = logicalAddress.Split('.');
            if (list.Length != 6)
                throw new Exception("Invalid OBIS code");

            for (int i = 0; i < list.Length; i++)
            {
                if (!int.TryParse(list[i], out int value) || value < 0 || value > 255)
                    throw new Exception("Invalid OBIS code");
                address[i] = (byte)value;
            }
        }

        internal DlmsRegisterInfo RegisterInfo;

        internal DataType DataType
        {
            get { return RegisterInfo.Type.GetDataType(); }
        }

        public DlmsUnit Unit
        {
            get { return DlmsUnit.FromDlmsUnitCode(RegisterInfo.UnitCode); }
        }

        public DlmsErrorCode ErrorCode
        {
            get { return RegisterInfo.ErrorCode; }
        }

        internal unsafe DlmsRegister(string logicalAddress)
        {
            fixed (DlmsRegisterInfo* ptr = &RegisterInfo)
                ParseLogicalAddress(logicalAddress, ((byte*)ptr) + DlmsRegisterInfo.ADDRESS_OFFSET);
            RegisterInfo.ErrorCode = DlmsErrorCode.None;
        }

        protected unsafe object GetInternalValue()
        {
            fixed (DlmsRegisterInfo* ptr = &RegisterInfo)
            {
                byte* valuePtr = ((byte*)ptr) + DlmsRegisterInfo.VALUE_OFFSET;
                DataType type = RegisterInfo.Type.GetDataType();
                switch (type)
                {
                    case DataType.Boolean:
                    case DataType.Byte:
                        return *valuePtr;
                    case DataType.Int16:
                    case DataType.UInt16:
                        return *(short*)valuePtr;
                    case DataType.Int32:
                    case DataType.UInt32:
                        return *(int*)valuePtr;
                    case DataType.Int64:
                    case DataType.UInt64:
                        return *(long*)valuePtr;
                    case DataType.Float:
                        return *(float*)valuePtr;
                    case DataType.Double:
                        return *(float*)valuePtr;
                    default:
                        throw new Exception("Not sopported type exception.");
                }
            }
        }

        public static DlmsFloatRegister CreatePhaseAVoltageRegister()
        {
            return new DlmsFloatRegister("1.1.32.7.0.255");
        }

        public static DlmsFloatRegister CreatePhaseBVoltageRegister()
        {
            return new DlmsFloatRegister("1.1.52.7.0.255");
        }

        public static DlmsFloatRegister CreatePhaseCVoltageRegister()
        {
            return new DlmsFloatRegister("1.1.72.7.0.255");
        }

        public static DlmsFloatRegister CreatePhaseACurrentRegister()
        {
            return new DlmsFloatRegister("1.1.31.7.0.255");
        }

        public static DlmsFloatRegister CreatePhaseBCurrentRegister()
        {
            return new DlmsFloatRegister("1.1.51.7.0.255");
        }

        public static DlmsFloatRegister CreatePhaseCCurrentRegister()
        {
            return new DlmsFloatRegister("1.1.71.7.0.255");
        }

        public static DlmsFloatRegister CreatePhaseAAngleRegister()
        {
            return new DlmsFloatRegister("1.1.81.7.10.255");
        }

        public static DlmsFloatRegister CreatePhaseBAngleRegister()
        {
            return new DlmsFloatRegister("1.1.81.7.2.255");
        }

        public static DlmsFloatRegister CreatePhaseCAngleRegister()
        {
            return new DlmsFloatRegister("1.1.81.7.21.255");
        }

        public static DlmsFloatRegister CreatePowerFactorRegister()
        {
            return new DlmsFloatRegister("1.1.13.7.0.255");
        }

        public static DlmsFloatRegister CreateFrequencyRegister()
        {
            return new DlmsFloatRegister("1.1.14.7.0.255");
        }

        public static DlmsFloatRegister CreateTemperatureRegister()
        {
            return new DlmsFloatRegister("0.0.96.9.0.255");
        }
    }

    public sealed class DlmsFloatRegister : DlmsRegister
    {
        private float coeff = 1.0f;


        public float? Value
        {
            get
            {
                if (RegisterInfo.IsNull == 1)
                    return null;
                return Convert.ToSingle(GetInternalValue()) * (float)Math.Pow(10, RegisterInfo.Scaler) * coeff;
            }
        }

        public DlmsFloatRegister(string logicalAddress)
            : base(logicalAddress) { }

        public void ApplyTransformRatio(float coeff)
        {
            this.coeff *= coeff;
        }
    }

    public class DlmsRegisterCollection : List<DlmsRegister> { }

    [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Ansi, Size = SIZE)]
    internal unsafe struct DlmsProfileInfo
    {
        public const int SIZE = 6;

        [FieldOffset(0)]
        internal fixed byte Address[6];

        internal DlmsProfileInfo(string logicalAddress)
        {
            fixed (byte* ptr = Address)
                DlmsRegister.ParseLogicalAddress(logicalAddress, ptr);
        }
    }

    [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Ansi, Size = SIZE)]
    public unsafe struct DlmsProfileChannelInfo
    {
        internal const int SIZE = 13;

        [FieldOffset(0)]
        internal ushort ClassID;

        [FieldOffset(2)]
        internal fixed byte Address[6];
        public string LogicalAddress
        {
            get
            {
                string logicalAddress = Address[0].ToString();
                for (int i = 1; i < 6; i++)
                    logicalAddress += "." + Address[i].ToString();
                return logicalAddress;
            }
        }

        public bool IsData
        {
            get
            {
                if (ClassID == 0x01)
                    return true;
                return false;
            }
        }

        public bool IsRegister
        {
            get
            {
                if (ClassID == 0x03)
                    return true;
                return false;
            }
        }

        [FieldOffset(8)]
        internal byte AttributeIndex;

        [FieldOffset(9)]
        internal ushort DataIndex;

        [FieldOffset(11)]
        internal sbyte Scaler;

        [FieldOffset(12)]
        internal byte UnitCode;

        internal DlmsProfileChannelInfo(ushort classId, byte* address, byte attributeIndex, ushort dataIndex, sbyte scaler, byte unitCode)
        {
            ClassID = classId;
            for (int i = 0; i < 6; i++)
                Address[i] = *address++;
            AttributeIndex = attributeIndex;
            DataIndex = dataIndex;
            Scaler = scaler;
            UnitCode = unitCode;
        }
    }

    public unsafe struct DlmsProfileColumn
    {
        public string Name { get; }

        //public DlmsUnit Unit { get; }

        internal DlmsProfileColumn(string name/*, DlmsUnit unit*/)
        {
            Name = name;
            //Unit = unit;
        }
    }

    public sealed class DlmsProfileColumnCollection
    {
        private DlmsProfileColumn[] columns;

        public DlmsProfileColumn this[int index] => columns[index];

        public int Count => columns.Length;

        internal DlmsProfileColumnCollection(DlmsProfileChannelInfo[] channels)
        {
            columns = new DlmsProfileColumn[channels.Length];
            for (int i = 0; i < channels.Length; i++)
                columns[i] = new DlmsProfileColumn("Column " + i.ToString()/*, DlmsUnit.FromDlmsUnitCode(channels[i].UnitCode)*/);
        }
    }

    [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Ansi, Size = SIZE)]
    public unsafe struct DlmsProfileField
    {
        public const int SIZE = DATA_OFFSET + 25;
        public const int DATA_OFFSET = 2;


        [FieldOffset(0)]
        private byte isNull;

        [FieldOffset(1)]
        private DlmsType type;

        public byte? ToByte()
        {
            if (isNull == 1)
                return null;

            fixed (DlmsProfileField* ptr = &this)
                return *((byte*)ptr + DATA_OFFSET);
        }

        public bool? ToBoolean()
        {
            if (isNull == 1)
                return null;

            fixed (DlmsProfileField* ptr = &this)
                return *(bool*)((byte*)ptr + DATA_OFFSET);
        }

        public override string ToString()
        {
            if (isNull == 1)
                return null;

            fixed (byte* ptr = new byte[SIZE])
            {
                fixed (char* ch = new char[SIZE])
                {
                    *(DlmsProfileField*)ptr = this;
                    for (int i = DATA_OFFSET; i < SIZE && ptr[i] > 0; i++)
                        ch[i] = (char)ptr[i];
                    return new string(ch + DATA_OFFSET);
                }
            }
        }

        public short? ToInt16()
        {
            if (isNull == 1)
                return null;

            fixed (DlmsProfileField* ptr = &this)
                return *(short*)((byte*)ptr + DATA_OFFSET);
        }

        public ushort? ToUInt16()
        {
            if (isNull == 1)
                return null;

            fixed (DlmsProfileField* ptr = &this)
                return *(ushort*)((byte*)ptr + DATA_OFFSET);
        }

        public int? ToInt32()
        {
            if (isNull == 1)
                return null;

            fixed (DlmsProfileField* ptr = &this)
                return *(int*)((byte*)ptr + DATA_OFFSET);
        }

        public uint? ToUInt32()
        {
            if (isNull == 1)
                return null;

            fixed (DlmsProfileField* ptr = &this)
                return *(uint*)((byte*)ptr + DATA_OFFSET);
        }

        public long? ToInt64()
        {
            if (isNull == 1)
                return null;

            fixed (DlmsProfileField* ptr = &this)
                return *(long*)((byte*)ptr + DATA_OFFSET);
        }

        public ulong? ToUInt64()
        {
            if (isNull == 1)
                return null;

            fixed (DlmsProfileField* ptr = &this)
                return *(ulong*)((byte*)ptr + DATA_OFFSET);
        }

        public float? ToFloat()
        {
            fixed (DlmsProfileField* ptr = &this)
                return *(float*)((byte*)ptr + DATA_OFFSET);
        }

        public double? ToDouble()
        {
            if (isNull == 1)
                return null;

            fixed (DlmsProfileField* ptr = &this)
                return *(double*)((byte*)ptr + DATA_OFFSET);
        }

        /* public DateTime ToDate()
         {
             fixed (DlmsProfileField* ptr = &this)
                 return EdmiDateTime.ToDate(*(EdmiDateTime*)ptr);
         }

         public DateTime ToTime()
         {
             fixed (DlmsProfileField* ptr = &this)
                 return EdmiDateTime.ToTime(*(EdmiDateTime*)ptr);
         }

         public DateTime ToDateTime()
         {
             fixed (DlmsProfileField* ptr = &this)
                 return EdmiDateTime.ToDateTime(*(EdmiDateTime*)ptr);
         }*/
    }

    public sealed class DlmsProfileRow
    {
        internal readonly DlmsProfileField[] Fields;

        public DlmsProfileField this[int index] => Fields[index];

        internal DlmsProfileRow(int columnssCount)
        {
            Fields = new DlmsProfileField[columnssCount];
        }
    }

    public unsafe struct DlmsSurvey
    {
        public string Name { get; }
        public string LogicalAddress { get; }
        public int Interval { get; }

        public DlmsSurvey(string name, string logicalAddress, int interval)
        {
            Name = name;
            LogicalAddress = logicalAddress;
            Interval = interval;
        }

        public static DlmsSurvey LS2 = new DlmsSurvey("Profile 2", "0.0.99.2.0.255", 900);
    }

    public sealed class DlmsProfile
    {
        private readonly List<DlmsProfileRow> Rows;
        internal DlmsSurvey Survey;

        public string Name
        {
            get { return Survey.Name; }
        }

        public string LogicalAddress
        {
            get { return Survey.LogicalAddress; }
        }

        public int Interval
        {
            get { return Survey.Interval; }
        }

        public DlmsProfileColumnCollection Columns { get; }

        public DateTime FromDate { get; }

        internal DlmsProfile(DlmsSurvey survey, DateTime fromDate, DlmsProfileChannelInfo[] channels)
        {
            Survey = survey;
            FromDate = fromDate;
            Columns = new DlmsProfileColumnCollection(channels);
            Rows = new List<DlmsProfileRow>();
        }

        public int RowsCount => Rows.Count;

        public int ColumnsCount => Columns.Count;

        public DlmsProfileRow this[int index] => Rows[index];

        internal void Add(DlmsProfileRow row)
        {
            Rows.Add(row);
        }
    }
}
