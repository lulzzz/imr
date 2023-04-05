// Authored by: MohammadAmin Mohammadi (https://github.com/mamohammadi)

using IMR.Log;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace IMR.Types
{
    internal static class EdmiConstants
    {
        public const byte STX = 2;
        public const byte ETX = 3;
    }

    public class EdmiMeterSettings
    {
        public uint SerialNumber { get; set; }
        public string Username { get; set; }
        public string Password { get; set; }
        public float CT { get; set; }
        public float PT { get; set; }
    }

    public enum EdmiErrorCode : byte
    {
        None = 0x00,
        CanNotWrite = 0x01,
        UnimplementedOperation = 0x02,
        RegisterNotFound = 0x03,
        AccessDenied = 0x04,
        RequestWrongLength = 0x05,
        BadTypeCodeInternalError = 0x06,
        DataNotReadyYet = 0x07,
        OutOfRange = 0x08,
        NotLoggedin = 0x09,
        RequestCRCError = 0x0A,
        ResponseCRCError = 0x0B,
        RequestResponseCommandMismatch = 0x0C,
        RequestResponseRegisterMismatch = 0x0D,
        LoginFailed = 0x0E,
        LogoutFailed = 0x0F,
        GetMeterAttentionFailed = 0x10,
        ResponseWrongLength = 0x11,
        UnImplementedDataType = 0x12
    }

    public enum EdmiType : byte
    {
        String = 0x41,                         // Null terminated ASCII string (EXT: EDMI_TYPE_STRING)
        Boolean = 0x42,                        // This is a byte that can be 0 meaning false, or none zero meaning true (EXT: EDMI_TYPE_BOOLEAN)
        Byte = 0x43,                           // 8-bit unsigned integer (EXT: EDMI_TYPE_BYTE)
        Double = 0x44,                         // Double precision IEEE 64-bit floating point number (EXT: EDMI_TYPE_DOUBLE)
        EFAString = 0x45,                      // External 16 character + null EFA string. Internal 16 - bit unsigned integer (EXT: EDMI_TYPE_STRING, INT: EDMI_TYPE_HEX_SHORT)
        Float = 0x46,                          // Single precision IEEE 32-bit floating point number (EXT: EDMI_TYPE_FLOAT)
        StringLong = 0x47,                     // External string representation of an integer. Internal 32 - bit signed long integer (EXT: EDMI_TYPE_STRING, INT: EDMI_TYPE_LONG)
        HexShort = 0x48,                       // 16-bit Hex unsigned short (EXT: EDMI_TYPE_HEX_SHORT)
        Short = 0x49,                          // 16-bit signed short integer (EXT: EDMI_TYPE_SHORT)
        VariableSpace = 0x4A,                  // If the written size is less than the maximum size it is padded with zeros. When read always returns the maximum size (EXT: EDMI_TYPE_VARIABLE_SPACE)
        Long = 0x4C,                           // 32-bit signed long integer (EXT: EDMI_TYPE_LONG)
        None = 0x4E,                           // Used by the meter to indicate an invalid type (EXT: EDMI_TYPE_NONE)
        FloatEnergy = 0x4F,                    // Internal 32-bit unsigned integer micropulses. External float(with transformer ratio applied) (EXT: EDMI_TYPE_FLOAT, INT: EDMI_TYPE_LONG)
        PowerFactor = 0x50,                    // Internal signed short from –32767 to 32767. External float from –1.0 to + 1.0 (EXT: EDMI_TYPE_FLOAT, INT: EDMI_TYPE_SHORT)
        Time = 0x51,                           // Internal unsigned 32-bit seconds since midnight. External time as 3 bytes: {Hour} {Minute} {Second} Displays on an LCD formatted as HH:MM:SS (EXT: EDMI_TYPE_TIME, INT: EDMI_TYPE_HEX_LONG)
        Date = 0x52,                           // Internal unsigned 32-bit seconds since 1/1/96. External date as 3 bytes: {Date} {Month} {Year} Displays on an LCD formatted as DD/MM/YY (EXT: EDMI_TYPE_DATE, INT:EDMI_TYPE_HEX_LONG)
        Special = 0x53,                        // Special (not a regular type) (EXT: EDMI_TYPE_SPECIAL)
        TimeDate = 0x54,                       // Internal unsigned 32-bit seconds since 1/1/96. External time / date as 6 bytes: {Date} {Month} {Year} {Hour} {Minute} {Second} Displays on an LCD formatted as HH MM:SS DD/MM/YY (EXT: EDMI_TYPE_TIME_DATE, INT:EDMI_TYPE_HEX_LONG)
        DoubleEnergy = 0x55,                   // Internal 64-bit integer micropulses. External double(with transformer ratio applied) (EXT: EDMI_TYPE_DOUBLE, INT: EDMI_TYPE_LONG_LONG)
        LongLong = 0x56,                       // 64-bit signed integer generally expressed in hexadecimal for ease of display(16 digits) (EXT: EDMI_TYPE_LONG_LONG)
        Waveform = 0x57,                       // Captured waveform, with status data. Format of the F402 register (EXT: EDMI_TYPE_WAVEFORM)
        HexLong = 0x58,                        // Hex unsigned long (EXT: EDMI_TYPE_HEX_LONG)
        RegisterNumberHexLong = 0x5A           // Register number, type is generally equivalent to HexLong (EXT: EDMI_TYPE_REGISTER_NUMBER_HEX_LONG)
    }
    internal static class EdmiTypeHelper
    {
        internal static DataType ToDataType(this EdmiType type)
        {
            switch (type)
            {
                case EdmiType.String:
                    return DataType.String;
                case EdmiType.Boolean:
                    return DataType.Boolean;
                case EdmiType.Byte:
                    return DataType.Byte;
                case EdmiType.Double:
                    return DataType.Double;
                case EdmiType.EFAString:
                    return DataType.String;
                case EdmiType.Float:
                    return DataType.Float;
                case EdmiType.StringLong:
                    return DataType.String;
                case EdmiType.HexShort:
                    return DataType.UInt16;
                case EdmiType.Short:
                    return DataType.Int16;
                case EdmiType.VariableSpace:
                    return DataType.Unknown; // DataType.VariableSpace;
                case EdmiType.Long:
                    return DataType.Int32;
                case EdmiType.None:
                    return DataType.Unknown; // DataType.Invalid;
                case EdmiType.FloatEnergy:
                    return DataType.Float;
                case EdmiType.PowerFactor:
                    return DataType.Float;
                case EdmiType.Time:
                    return DataType.Time;
                case EdmiType.Date:
                    return DataType.Date;
                case EdmiType.Special:
                    return DataType.Unknown; // DataType.Special;
                case EdmiType.TimeDate:
                    return DataType.DateTime;
                case EdmiType.DoubleEnergy:
                    return DataType.Double;
                case EdmiType.LongLong:
                    return DataType.Int64;
                case EdmiType.Waveform:
                    return DataType.Unknown; // DataType.Waveform;
                case EdmiType.HexLong:
                    return DataType.UInt32;
                case EdmiType.RegisterNumberHexLong:
                    return DataType.UInt32;
                default:
                    return DataType.Unknown;
            }
        }
    }

    public enum EdmiUnit : byte
    {
        Amps = 0x41,
        LitersPerHour = 0x42,
        AngleInDegrees = 0x44,
        CubicMetersPerHour = 0x47,
        Hertz = 0x48,
        JoulesPerHour = 0x49,
        Joules = 0x4A,
        Liters = 0x4C,
        Minutes = 0x4D,
        NoUnit = 0x4E,
        CubicMeters = 0x4F,
        Percent = 0x50,
        PowerFactor = 0x51,
        Vars = 0x52,
        VA = 0x53,
        Seconds = 0x54,
        Unknown = 0x55,
        Voltes = 0x56,
        Watts = 0x57,
        WattHours = 0x58,
        Varh = 0x59,
        Vah = 0x5A
    }

    internal enum EdmiSurveyAddress : short
    {
        LS1 = 0x0305,
        LS2 = 0x0325,
        LS3 = 0x0345,
        LS4 = 0x0365,
        LS5 = 0x0385,
        LS6 = 0x0395,
        LS7 = 0x03A5,
        LS8 = 0x03B5,
        LS9 = 0x03C5,
        LS10 = 0x03D5
    }

    public unsafe struct EdmiSurvey
    {
        public short Address { get; }

        public EdmiSurvey(short address)
        {
            Address = address;
        }

        public static EdmiSurvey LS1 = new EdmiSurvey((short)EdmiSurveyAddress.LS1);
        public static EdmiSurvey LS2 = new EdmiSurvey((short)EdmiSurveyAddress.LS2);
        public static EdmiSurvey LS3 = new EdmiSurvey((short)EdmiSurveyAddress.LS3);
        public static EdmiSurvey LS4 = new EdmiSurvey((short)EdmiSurveyAddress.LS4);
        public static EdmiSurvey LS5 = new EdmiSurvey((short)EdmiSurveyAddress.LS5);
        public static EdmiSurvey LS6 = new EdmiSurvey((short)EdmiSurveyAddress.LS6);
        public static EdmiSurvey LS7 = new EdmiSurvey((short)EdmiSurveyAddress.LS7);
        public static EdmiSurvey LS8 = new EdmiSurvey((short)EdmiSurveyAddress.LS8);
        public static EdmiSurvey LS9 = new EdmiSurvey((short)EdmiSurveyAddress.LS9);
        public static EdmiSurvey LS10 = new EdmiSurvey((short)EdmiSurveyAddress.LS10);
    }

    [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Ansi, Size = SIZE)]
    internal struct EdmiRegisterInfo
    {
        public const int SIZE = DATA_OFFSET + 25;
        public const int DATA_OFFSET = 7;

        [FieldOffset(0)]
        internal int Address;

        [FieldOffset(4)]
        internal EdmiType Type;

        [FieldOffset(5)]
        internal EdmiUnit unitCode;

        [FieldOffset(6)]
        internal EdmiErrorCode errorCode;
    }

    public abstract class EdmiRegister
    {
        internal EdmiRegisterInfo RegisterInfo;

        public DataType DataType { get; }

        public EdmiUnit Unit
        {
            get { return RegisterInfo.unitCode; }
        }

        public EdmiErrorCode ErrorCode
        {
            get { return RegisterInfo.errorCode; }
        }

        internal EdmiRegister(int address, EdmiType registerType, EdmiUnit unit)
        {
            RegisterInfo.Address = address;
            RegisterInfo.Type = registerType;
            RegisterInfo.unitCode = unit;
            RegisterInfo.errorCode = EdmiErrorCode.None;
            this.DataType = registerType.ToDataType();
        }

        public static EdmiFloatRegister CreateCurrentMultiplierRegister()
        {
            return new EdmiFloatRegister(0xF700, EdmiType.Float, EdmiUnit.NoUnit);
        }

        public static EdmiFloatRegister CreateVoltageMultiplierRegister()
        {
            return new EdmiFloatRegister(0xF701, EdmiType.Float, EdmiUnit.NoUnit);
        }

        public static EdmiFloatRegister CreateCurrentDivisorRegister()
        {
            return new EdmiFloatRegister(0xF702, EdmiType.Float, EdmiUnit.NoUnit);
        }

        public static EdmiFloatRegister CreateVoltageDivisorRegister()
        {
            return new EdmiFloatRegister(0xF703, EdmiType.Float, EdmiUnit.NoUnit);
        }

        public static EdmiFloatRegister CreatePhaseAVoltageRegister()
        {
            return new EdmiFloatRegister(0xE000, EdmiType.FloatEnergy, EdmiUnit.Voltes);
        }

        public static EdmiFloatRegister CreatePhaseBVoltageRegister()
        {
            return new EdmiFloatRegister(0xE001, EdmiType.FloatEnergy, EdmiUnit.Voltes);
        }

        public static EdmiFloatRegister CreatePhaseCVoltageRegister()
        {
            return new EdmiFloatRegister(0xE002, EdmiType.FloatEnergy, EdmiUnit.Voltes);
        }

        public static EdmiFloatRegister CreatePhaseACurrentRegister()
        {
            return new EdmiFloatRegister(0xE010, EdmiType.FloatEnergy, EdmiUnit.Amps);
        }

        public static EdmiFloatRegister CreatePhaseBCurrentRegister()
        {
            return new EdmiFloatRegister(0xE011, EdmiType.FloatEnergy, EdmiUnit.Amps);
        }

        public static EdmiFloatRegister CreatePhaseCCurrentRegister()
        {
            return new EdmiFloatRegister(0xE012, EdmiType.FloatEnergy, EdmiUnit.Amps);
        }

        public static EdmiFloatRegister CreatePhaseAAngleRegister()
        {
            return new EdmiFloatRegister(0xE020, EdmiType.FloatEnergy, EdmiUnit.AngleInDegrees);
        }

        public static EdmiFloatRegister CreatePhaseBAngleRegister()
        {
            return new EdmiFloatRegister(0xE021, EdmiType.FloatEnergy, EdmiUnit.AngleInDegrees);
        }

        public static EdmiFloatRegister CreatePhaseCAngleRegister()
        {
            return new EdmiFloatRegister(0xE022, EdmiType.FloatEnergy, EdmiUnit.AngleInDegrees);
        }

        public static EdmiFloatRegister CreatePowerFactorRegister()
        {
            return new EdmiFloatRegister(0xE026, EdmiType.PowerFactor, EdmiUnit.PowerFactor);
        }

        public static EdmiFloatRegister CreateFrequencyRegister()
        {
            return new EdmiFloatRegister(0xE060, EdmiType.FloatEnergy, EdmiUnit.Hertz);
        }

    }

    public abstract class EdmiRegister<T> : EdmiRegister
    {
        private T value;

        public unsafe T Value
        {
            get
            {
                fixed (byte* ptr = new byte[EdmiRegisterInfo.SIZE])
                {
                    *(EdmiRegisterInfo*)ptr = RegisterInfo;
                    value = GetValue(ptr + EdmiRegisterInfo.DATA_OFFSET);
                    return value;
                }
            }
            internal set
            {
                fixed (byte* ptr = new byte[EdmiRegisterInfo.SIZE])
                {
                    this.value = value;
                    *(EdmiRegisterInfo*)ptr = RegisterInfo;
                    SetValue(ptr + EdmiRegisterInfo.DATA_OFFSET, value);
                    RegisterInfo = *(EdmiRegisterInfo*)ptr;
                }
            }
        }

        protected abstract unsafe T GetValue(byte* ptr);

        protected abstract unsafe void SetValue(byte* ptr, T value);

        internal EdmiRegister(int address, EdmiType registerType, EdmiUnit unit)
            : base(address, registerType, unit) { }
    }

    public sealed class EdmiFloatRegister : EdmiRegister<float>
    {
        protected override unsafe float GetValue(byte* ptr)
        {
            return *(float*)ptr;
        }

        protected override unsafe void SetValue(byte* ptr, float value)
        {
            *(float*)ptr = value;
        }

        public EdmiFloatRegister(int address, EdmiType registerType, EdmiUnit unit)
            : base(address, registerType, unit) { }

        public void ApplyTransformRatio(float coeff)
        {
            this.Value *= coeff;
        }

        public void ApplyTransformRatio(EdmiFloatRegister multiplier, EdmiFloatRegister divisor)
        {
            this.Value *= divisor.Value / multiplier.Value;
        }

        public void ApplyTransformRatio(float multiplier, float divisor)
        {
            this.Value *= divisor / multiplier;
        }
    }

    public class EdmiRegisterCollection : List<EdmiRegister> { }

    [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Ansi, Size = SIZE)]
    internal struct EdmiFileChannelInfo
    {
        public const int SIZE = NAME_OFFSET + 25;
        public const int NAME_OFFSET = 7;

        // Registers
        [FieldOffset(0)]
        internal EdmiType Type;

        [FieldOffset(1)]
        internal EdmiUnit UnitCode;

        // Access
        [FieldOffset(2)]
        internal byte ScalingCode;

        [FieldOffset(3)]
        internal float ScalingFactor;

        public unsafe string Name
        {
            get
            {
                fixed (byte* ptr = new byte[SIZE])
                {
                    fixed (char* ch = new char[25])
                    {
                        *(EdmiFileChannelInfo*)ptr = this;
                        for (int i = 0; i < 25; i++)
                            ch[i] = (char)ptr[NAME_OFFSET + i];
                        return new string(ch);
                    }
                }
            }
        }
    }

    [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Ansi, Size = SIZE)]
    internal struct EdmiDateTime
    {
        public const int SIZE = 7;

        [FieldOffset(0)]
        internal byte Year;

        [FieldOffset(1)]
        internal byte Month;

        [FieldOffset(2)]
        internal byte Day;

        [FieldOffset(3)]
        internal byte Hour;

        [FieldOffset(4)]
        internal byte Minute;

        [FieldOffset(5)]
        internal byte Second;

        [FieldOffset(6)]
        internal byte IsNull;

        public EdmiDateTime(DateTime? dateTime)
        {
            if (dateTime == null)
            {
                Year = 0xFF;
                Month = 0xFF;
                Day = 0xFF;
                Hour = 0xFF;
                Minute = 0xFF;
                Second = 0xFF;
                IsNull = 0x01;
                return;
            }

            DateTime dt = dateTime.Value;
            Year = (byte)(dt.Year - 2000);
            Month = (byte)dt.Month;
            Day = (byte)dt.Day;
            Hour = (byte)dt.Hour;
            Minute = (byte)dt.Minute;
            Second = (byte)dt.Second;
            IsNull = 0x00;
        }

        public static EdmiDateTime FromDate(DateTime? dateTime)
        {
            if (dateTime != null)
            {
                DateTime? date = new DateTime(dateTime.Value.Year, dateTime.Value.Month, dateTime.Value.Day);
                return new EdmiDateTime(date);
            }
            return new EdmiDateTime(null);

        }

        public static DateTime? ToDate(EdmiDateTime edmiDateTime)
        {
            if (edmiDateTime.IsNull == 0x01)
                return null;
            return new DateTime(edmiDateTime.Year + 2000, edmiDateTime.Month, edmiDateTime.Day);
        }

        public static EdmiDateTime FromTime(DateTime? dateTime)
        {
            if (dateTime != null)
            {
                DateTime date = new DateTime(2000, 1, 1, dateTime.Value.Hour, dateTime.Value.Minute, dateTime.Value.Second);
                return new EdmiDateTime(date);
            }
            return new EdmiDateTime(null);
        }

        public static DateTime? ToTime(EdmiDateTime edmiDateTime)
        {
            if (edmiDateTime.IsNull == 0x01)
                return null;
            return new DateTime(2000, 1, 1, edmiDateTime.Hour, edmiDateTime.Minute, edmiDateTime.Second);
        }

        public static EdmiDateTime FromDateTime(DateTime? dateTime)
        {
            return new EdmiDateTime(dateTime);
        }

        public static DateTime? ToDateTime(EdmiDateTime edmiDateTime)
        {
            if (edmiDateTime.IsNull == 0x01)
                return null;
            return new DateTime(edmiDateTime.Year + 2000, edmiDateTime.Month, edmiDateTime.Day, edmiDateTime.Hour, edmiDateTime.Minute, edmiDateTime.Second);
        }
    }

    public struct EdmiProfileColumn
    {
        public string Name { get; internal set; }
        public DataType DataType { get; internal set; }
        public EdmiUnit Unit { get; internal set; }
    }

    public sealed class EdmiProfileColumnCollection
    {
        private EdmiProfileColumn[] columns;

        public EdmiProfileColumn this[int index] => columns[index + 1];

        internal EdmiProfileColumn StatusColumn => columns[0];

        public int Count => columns.Length - 1;

        public EdmiProfileColumnCollection(EdmiProfileColumn[] columns)
        {
            this.columns = columns;
        }
    }

    [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Ansi, Size = SIZE)]
    public unsafe struct EdmiProfileField
    {
        public const int SIZE = 25;

        public byte ToByte()
        {
            fixed (EdmiProfileField* ptr = &this)
                return *(byte*)ptr;
        }

        public bool ToBoolean()
        {
            fixed (EdmiProfileField* ptr = &this)
                return *(bool*)ptr;
        }

        public override string ToString()
        {
            fixed (byte* ptr = new byte[SIZE])
            {
                fixed (char* ch = new char[SIZE])
                {
                    *(EdmiProfileField*)ptr = this;
                    for (int i = 0; i < SIZE && ptr[i] > 0; i++)
                        ch[i] = (char)ptr[i];
                    return new string(ch);
                }
            }
        }

        public short ToInt16()
        {
            fixed (EdmiProfileField* ptr = &this)
                return *(short*)ptr;
        }

        public ushort ToUInt16()
        {
            fixed (EdmiProfileField* ptr = &this)
                return *(ushort*)ptr;
        }

        public int ToInt32()
        {
            fixed (EdmiProfileField* ptr = &this)
                return *(int*)ptr;
        }

        public uint ToUInt32()
        {
            fixed (EdmiProfileField* ptr = &this)
                return *(uint*)ptr;
        }

        public long ToInt64()
        {
            fixed (EdmiProfileField* ptr = &this)
                return *(long*)ptr;
        }

        public ulong ToUInt64()
        {
            fixed (EdmiProfileField* ptr = &this)
                return *(ulong*)ptr;
        }

        public float ToFloat()
        {
            fixed (EdmiProfileField* ptr = &this)
                return *(float*)ptr;
        }

        public double ToDouble()
        {
            fixed (EdmiProfileField* ptr = &this)
                return *(double*)ptr;
        }

        public DateTime? ToDate()
        {
            fixed (EdmiProfileField* ptr = &this)
                return EdmiDateTime.ToDate(*(EdmiDateTime*)ptr);
        }

        public DateTime? ToTime()
        {
            fixed (EdmiProfileField* ptr = &this)
                return EdmiDateTime.ToTime(*(EdmiDateTime*)ptr);
        }

        public DateTime? ToDateTime()
        {
            fixed (EdmiProfileField* ptr = &this)
                return EdmiDateTime.ToDateTime(*(EdmiDateTime*)ptr);
        }
    }

    [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Ansi, Size = SIZE)]
    internal unsafe struct EdmiProfileSpecification
    {
        public const int SIZE = NAME_OFFSET + 25;
        public const int CHANNELS_INFO_OFFSET = 25; // Max 16 Channels (32 byte each channel)
        public const int NAME_OFFSET = CHANNELS_INFO_OFFSET + (16 * 32);

        [FieldOffset(0)]
        internal EdmiSurvey Survey;

        [FieldOffset(2)]
        internal int Interval;

        [FieldOffset(6)]
        internal EdmiDateTime FromDateTime;

        [FieldOffset(13)]
        internal EdmiDateTime ToDateTime;

        [FieldOffset(20)]
        internal int RecordsCount;

        [FieldOffset(24)]
        internal byte ChannelsCount;
        
        // 16 Channels of size 32

        internal unsafe EdmiFileChannelInfo[] ChannelsInfo
        {
            get
            {
                EdmiFileChannelInfo[] infoes = new EdmiFileChannelInfo[ChannelsCount];
                fixed (byte* clone = new byte[SIZE])
                {
                    *(EdmiProfileSpecification*)clone = this;
                    byte* ptr = clone + CHANNELS_INFO_OFFSET;
                    EdmiFileChannelInfo* infoPtr = (EdmiFileChannelInfo*)ptr;
                    for (byte ch = 0; ch < ChannelsCount; ch++)
                        infoes[ch] = *infoPtr++;
                }
                return infoes;
            }
        }

        // Name of size 25

        internal unsafe string Name
        {
            get
            {
                fixed (byte* ptr = new byte[SIZE])
                {
                    fixed (char* ch = new char[25])
                    {
                        *(EdmiProfileSpecification*)ptr = this;
                        for (int i = 0; i < 25; i++)
                            ch[i] = (char)ptr[NAME_OFFSET + i];
                        return new string(ch);
                    }
                }
            }
        }

    }

    public struct EdmiProfileRowStatus
    {
        internal bool errorReadingRegisters;
        internal bool noData;
        internal bool powerFailed;
        internal bool incomplete;
        internal bool effectedByDaylightSaving;
        internal bool calibrationLost;
        internal bool svfrmeFailure;
        internal bool efaFailure;
        internal bool dataCheckSumError;

        public bool ErrorReadingRegisters => errorReadingRegisters;
        public bool NoData => noData;
        public bool PowerFailed => powerFailed;
        public bool Incomplete => incomplete;
        public bool EffectedByDaylightSaving => effectedByDaylightSaving;
        public bool CalibrationLost => calibrationLost;
        public bool SVFRMEFailure => svfrmeFailure;
        public bool EFAFailure => efaFailure;
        public bool DataCheckSumError => dataCheckSumError;

        public bool LostDueToRewind => noData && incomplete;
    }

    public sealed class EdmiProfileRow
    {
        internal readonly EdmiProfileField[] Fields;

        public EdmiProfileField this[int index] => Fields[index + 1];

        public short StatusCode => Fields[0].ToInt16();

        public EdmiProfileRowStatus Status
        {
            get
            {
                short statusCode = Fields[0].ToInt16();
                EdmiProfileRowStatus status;
                status.errorReadingRegisters = (statusCode & 0x01) == 0x01 ? true : false;
                status.noData = ((statusCode >> 1) & 0x01) == 0x01 ? true : false;
                status.powerFailed = ((statusCode >> 2) & 0x01) == 0x01 ? true : false;
                status.incomplete = ((statusCode >> 3) & 0x01) == 0x01 ? true : false;
                status.effectedByDaylightSaving = ((statusCode >> 4) & 0x01) == 0x01 ? true : false;
                status.calibrationLost = ((statusCode >> 5) & 0x01) == 0x01 ? true : false;
                status.svfrmeFailure = ((statusCode >> 6) & 0x01) == 0x01 ? true : false;
                status.efaFailure = ((statusCode >> 7) & 0x01) == 0x01 ? true : false;
                status.dataCheckSumError = ((statusCode >> 8) & 0x01) == 0x01 ? true : false;
                return status;
            }
        }

        internal EdmiProfileRow(int channelsCount)
        {
            Fields = new EdmiProfileField[channelsCount];
        }
    }

    public sealed class EdmiProfile
    {
        private readonly EdmiProfileRow[] Rows;

        internal EdmiSurvey Survey { get; }

        public string Name { get; }

        public int Interval { get; }

        public DateTime FromDate { get; }

        public DateTime? ToDate { get; }

        public EdmiProfileColumnCollection Columns { get; }

        internal EdmiProfile(EdmiProfileSpecification profileSpecification)
        {
            Name = profileSpecification.Name;
            Survey = profileSpecification.Survey;
            Interval = profileSpecification.Interval;
            FromDate = (DateTime)EdmiDateTime.ToDateTime(profileSpecification.FromDateTime);
            ToDate = EdmiDateTime.ToDateTime(profileSpecification.ToDateTime);
            Rows = new EdmiProfileRow[profileSpecification.RecordsCount];
            for (int r = 0; r < profileSpecification.RecordsCount; r++)
                Rows[r] = new EdmiProfileRow(profileSpecification.ChannelsCount);
            int channelsCount = profileSpecification.ChannelsCount;
            EdmiProfileColumn[] columns = new EdmiProfileColumn[channelsCount];
            for (byte c = 0; c < channelsCount; c++)
            {
                EdmiFileChannelInfo cInfo = profileSpecification.ChannelsInfo[c];
                columns[c].Name = cInfo.Name;
                columns[c].DataType = cInfo.Type.ToDataType();
                columns[c].Unit = cInfo.UnitCode;
            }
            Columns = new EdmiProfileColumnCollection(columns);
        }

        public int RowsCount => Rows.Length;

        public int ColumnsCount => Columns.Count;

        public EdmiProfileRow this[int index] => Rows[index];
    }
}
