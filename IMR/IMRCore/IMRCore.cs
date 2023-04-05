// Authored by: MohammadAmin Mohammadi (https://github.com/mamohammadi)

using IMR.Types;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using IMR;
using System.Threading.Tasks;
using System.Threading;

namespace IMR
{
    internal unsafe delegate MediaError WriteAllToMediaDelegate(byte* buffer, int length);
    internal unsafe delegate MediaError ReadFromMediaDelegate(byte* b);

    internal static unsafe class IMRCore
    {
        private static class IMRCore32
        {
            #region Shared
            // Shared
            [DllImport("IMR.Core.x86.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern void Init();

            [DllImport("IMR.Core.x86.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern void Release(void* buffer);
            #endregion

            #region Media
            // Media
            [DllImport("IMR.Core.x86.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError WriteAllToMedia(WriteAllToMediaDelegate callback, byte* buffer, int length);

            [DllImport("IMR.Core.x86.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError ReadFromMedia(ReadFromMediaDelegate callback, byte* b);
            #endregion

            #region Edmi
            // Login
            [DllImport("IMR.Core.x86.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError EdmiLogin(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, uint serial, [MarshalAs(UnmanagedType.LPStr)] string username, int usernameLength, [MarshalAs(UnmanagedType.LPStr)] string passwor, int passwordLength, out EdmiErrorCode errorCode);

            // Logout
            [DllImport("IMR.Core.x86.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError EdmiLogout(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, uint serial, out EdmiErrorCode errorCode);

            // Read Register Extended
            [DllImport("IMR.Core.x86.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError EdmiReadRegister(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, uint serial, EdmiRegisterInfo* register, out EdmiErrorCode errorCode);

            // Read Registers Extended
            [DllImport("IMR.Core.x86.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError EdmiReadRegisters(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, uint serial, EdmiRegisterInfo* registers, int registersCount, out EdmiErrorCode errorCode);

            // Read Profile
            [DllImport("IMR.Core.x86.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError EdmiReadProfile(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, uint serial, short survey, EdmiDateTime fromDateTime, EdmiDateTime toDateTime, out EdmiProfileSpecification profileSpec, out EdmiProfileField* fields, out EdmiErrorCode errorCode);
            #endregion Edmi

            #region Dlms
            // Login
            [DllImport("IMR.Core.x86.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError DlmsLogin(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, HdlcCommunicationParameters* comParams, [MarshalAs(UnmanagedType.LPStr)] string password, int passwordLength, out DlmsErrorCode errorCode);

            // Logout
            [DllImport("IMR.Core.x86.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError DlmsLogout(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, HdlcCommunicationParameters* comParams, [MarshalAs(UnmanagedType.LPStr)] string password, int passwordLength, out DlmsErrorCode errorCode);

            // Read Register
            [DllImport("IMR.Core.x86.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError DlmsReadRegister(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, HdlcCommunicationParameters* comParams, DlmsRegisterInfo* reg, out DlmsErrorCode errorCode);

            // Read Profile Channels
            [DllImport("IMR.Core.x86.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError DlmsReadProfileChannels(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, HdlcCommunicationParameters* comParams, DlmsProfileInfo fileInfo, out DlmsProfileChannelInfo* channels, out byte channelsCount, out DlmsErrorCode errorCode);

            // Read Profile
            [DllImport("IMR.Core.x86.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError DlmsReadProfile(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, HdlcCommunicationParameters* comParams, DlmsDateTime* from, DlmsDateTime* to, DlmsProfileInfo fileInfo, DlmsProfileChannelInfo* channels, byte channelsCount, out DlmsProfileField* fields, out int rowsCount, out DlmsErrorCode errorCode);

            // Misc
            [DllImport("IMR.Core.x86.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern uint DlmsGetServerID(uint physicalAddress, uint logicalAddress);
            #endregion
        }

        private static class IMRCore64
        {
            #region Shared
            // Shared
            [DllImport("IMR.Core.x64.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern void Init();

            [DllImport("IMR.Core.x64.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern void Release(void* buffer);
            #endregion

            #region Media
            // Media
            [DllImport("IMR.Core.x64.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError WriteAllToMedia(WriteAllToMediaDelegate callback, byte* buffer, int length);

            [DllImport("IMR.Core.x64.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError ReadFromMedia(ReadFromMediaDelegate callback, byte* b);
            #endregion

            #region Edmi
            // Login
            [DllImport("IMR.Core.x64.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError EdmiLogin(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, uint serial, [MarshalAs(UnmanagedType.LPStr)] string username, int usernameLength, [MarshalAs(UnmanagedType.LPStr)] string passwor, int passwordLength, out EdmiErrorCode errorCode);

            // Logout
            [DllImport("IMR.Core.x64.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError EdmiLogout(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, uint serial, out EdmiErrorCode errorCode);

            // Read Register Extended
            [DllImport("IMR.Core.x64.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError EdmiReadRegister(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, uint serial, EdmiRegisterInfo* register, out EdmiErrorCode errorCode);

            // Read Registers Extended
            [DllImport("IMR.Core.x64.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError EdmiReadRegisters(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, uint serial, EdmiRegisterInfo* registers, int registersCount, out EdmiErrorCode errorCode);

            // Read Profile
            [DllImport("IMR.Core.x64.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError EdmiReadProfile(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, uint serial, short surveyAddress, EdmiDateTime fromDateTime, EdmiDateTime toDateTime, out EdmiProfileSpecification profileSpec, out EdmiProfileField* fields, out EdmiErrorCode errorCode);
            #endregion Edmi

            #region Dlms
            // Login
            [DllImport("IMR.Core.x64.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError DlmsLogin(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, HdlcCommunicationParameters* comParams, [MarshalAs(UnmanagedType.LPStr)] string password, int passwordLength, out DlmsErrorCode errorCode);

            // Logout
            [DllImport("IMR.Core.x64.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError DlmsLogout(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, HdlcCommunicationParameters* comParams, [MarshalAs(UnmanagedType.LPStr)] string password, int passwordLength, out DlmsErrorCode errorCode);

            // Read Register
            [DllImport("IMR.Core.x64.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError DlmsReadRegister(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, HdlcCommunicationParameters* comParams, DlmsRegisterInfo* reg, out DlmsErrorCode errorCode);

            // Read Profile Channels
            [DllImport("IMR.Core.x64.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError DlmsReadProfileChannels(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, HdlcCommunicationParameters* comParams, DlmsProfileInfo fileInfo, out DlmsProfileChannelInfo* channels, out byte channelsCount, out DlmsErrorCode errorCode);

            // Read Profile
            [DllImport("IMR.Core.x64.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern MediaError DlmsReadProfile(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, HdlcCommunicationParameters* comParams, DlmsDateTime* from, DlmsDateTime* to, DlmsProfileInfo fileInfo, DlmsProfileChannelInfo* channels, byte channelsCount, out DlmsProfileField* fields, out int rowsCount, out DlmsErrorCode errorCode);

            // Misc
            [DllImport("IMR.Core.x64.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode, SetLastError = true)]
            public static extern uint DlmsGetServerID(uint physicalAddress, uint logicalAddress);

            #endregion
        }

        static bool init = false;
        static Semaphore initSemaphore = new Semaphore(1, 1);
        public static void Init()
        {
            initSemaphore.WaitOne();

            if (init == false)
            {
                init = true;
                Task.Run(() =>
                {
                    if (Environment.Is64BitProcess)
                        IMRCore64.Init();
                    else
                        IMRCore32.Init();
                });
            }

            initSemaphore.Release();

        }

        public static void Release(void* buffer)
        {
            if (Environment.Is64BitProcess)
                IMRCore64.Release(buffer);
            else
                IMRCore32.Release(buffer);
        }

        public static class Edmi
        {
            // Login
            public static MediaError Login(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, uint serial, string username, string password, out EdmiErrorCode errorCode)
            {
                if (Environment.Is64BitProcess)
                    return IMRCore64.EdmiLogin(readCallback, writeCallback, serial, username, username.Length, password, password.Length, out errorCode);
                return IMRCore32.EdmiLogin(readCallback, writeCallback, serial, username, username.Length, password, password.Length, out errorCode);
            }

            // Logout
            public static MediaError Logout(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, uint serial, out EdmiErrorCode errorCode)
            {
                if (Environment.Is64BitProcess)
                    return IMRCore64.EdmiLogout(readCallback, writeCallback, serial, out errorCode);
                return IMRCore32.EdmiLogout(readCallback, writeCallback, serial, out errorCode);
            }

            // Read Register Extended
            public static MediaError ReadRegister(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, uint serial, EdmiRegisterInfo* registerInfo, out EdmiErrorCode errorCode)
            {
                if (Environment.Is64BitProcess)
                    return IMRCore64.EdmiReadRegister(readCallback, writeCallback, serial, registerInfo, out errorCode);
                return IMRCore32.EdmiReadRegister(readCallback, writeCallback, serial, registerInfo, out errorCode);
            }

            // Read Registers Extended
            public static MediaError ReadRegisters(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, uint serial, EdmiRegisterInfo* registersInfo, int registersCount, out EdmiErrorCode errorCode)
            {
                if (Environment.Is64BitProcess)
                    return IMRCore64.EdmiReadRegisters(readCallback, writeCallback, serial, registersInfo, registersCount, out errorCode);
                return IMRCore32.EdmiReadRegisters(readCallback, writeCallback, serial, registersInfo, registersCount, out errorCode);
            }

            // Read Profile
            public static MediaError ReadProfile(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, uint serial, short surveyAddress, DateTime fromDateTime, DateTime? toDateTime, out EdmiProfileSpecification profileSpec, out EdmiProfileField* fields, out EdmiErrorCode errorCode)
            {
                if (Environment.Is64BitProcess)
                    return IMRCore64.EdmiReadProfile(readCallback, writeCallback, serial, surveyAddress, EdmiDateTime.FromDateTime(fromDateTime), EdmiDateTime.FromDateTime(toDateTime), out profileSpec, out fields, out errorCode);
                return IMRCore32.EdmiReadProfile(readCallback, writeCallback, serial, surveyAddress, EdmiDateTime.FromDateTime(fromDateTime), EdmiDateTime.FromDateTime(toDateTime), out profileSpec, out fields, out errorCode);
            }
        }

        public static class Dlms
        {
            // Login
            public static MediaError Login(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, HdlcCommunicationParameters* comParams, string password, out DlmsErrorCode errorCode)
            {
                if (Environment.Is64BitProcess)
                    return IMRCore64.DlmsLogin(readCallback, writeCallback, comParams, password, password.Length, out errorCode);
                return IMRCore32.DlmsLogin(readCallback, writeCallback, comParams, password, password.Length, out errorCode);
            }

            // Logout
            public static MediaError Logout(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, HdlcCommunicationParameters* comParams, string password, out DlmsErrorCode errorCode)
            {
                if (Environment.Is64BitProcess)
                    return IMRCore64.DlmsLogout(readCallback, writeCallback, comParams, password, password.Length, out errorCode);
                return IMRCore32.DlmsLogout(readCallback, writeCallback, comParams, password, password.Length, out errorCode);
            }

            // Read Register
            public static MediaError ReadRegister(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, HdlcCommunicationParameters* comParams, DlmsRegisterInfo* reg, out DlmsErrorCode errorCode)
            {
                if (Environment.Is64BitProcess)
                    return IMRCore64.DlmsReadRegister(readCallback, writeCallback, comParams, reg, out errorCode);
                return IMRCore32.DlmsReadRegister(readCallback, writeCallback, comParams, reg, out errorCode);
            }

            // Read Profile
            public static MediaError ReadProfileChannels(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, HdlcCommunicationParameters* comParams, DlmsProfileInfo fileInfo, out DlmsProfileChannelInfo* channels, out byte channelsCount, out DlmsErrorCode errorCode)
            {
                if (Environment.Is64BitProcess)
                    return IMRCore64.DlmsReadProfileChannels(readCallback, writeCallback, comParams, fileInfo, out channels, out channelsCount, out errorCode);
                return IMRCore32.DlmsReadProfileChannels(readCallback, writeCallback, comParams, fileInfo, out channels, out channelsCount, out errorCode);
            }

            // Read Profile
            public static MediaError ReadProfile(ReadFromMediaDelegate readCallback, WriteAllToMediaDelegate writeCallback, HdlcCommunicationParameters* comParams, DlmsDateTime* from, DlmsDateTime* to, DlmsProfileInfo fileInfo, DlmsProfileChannelInfo* channels, byte channelsCount, out DlmsProfileField* fields, out int rowsCount, out DlmsErrorCode errorCode)
            {
                if (Environment.Is64BitProcess)
                    return IMRCore64.DlmsReadProfile(readCallback, writeCallback, comParams, from, to, fileInfo, channels, channelsCount, out fields, out rowsCount, out errorCode);
                return IMRCore32.DlmsReadProfile(readCallback, writeCallback, comParams, from, to, fileInfo, channels, channelsCount, out fields, out rowsCount, out errorCode);
            }

            // Misc
            public static uint GetServerID(uint physicalAddress, uint logicalAddress)
            {
                if (Environment.Is64BitProcess)
                    return IMRCore64.DlmsGetServerID(physicalAddress, logicalAddress);
                return IMRCore32.DlmsGetServerID(physicalAddress, logicalAddress);
            }
        }

    }


}
