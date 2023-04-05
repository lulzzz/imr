// Authored by: MohammadAmin Mohammadi (https://github.com/mamohammadi)

using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using IMR.Log;
using IMR.MediaInterfaces;
using IMR.Types;

namespace IMR.Protocols
{
    public class DlmsMeterHandler
    {
        private DlmsMeterSettings settings;
        private Media media;
        private HdlcCommunicationParameters comParams;

        public DlmsMeterHandler(DlmsMeterSettings settings, Media media)
        {
            this.settings = settings;
            this.media = media;
            comParams.ClientID = settings.ClientID;
            comParams.ServerID = IMRCore.Dlms.GetServerID(settings.PhysicalAddress, settings.LogicalAddress);
            IMRCore.Init();
        }

        public unsafe MediaError Login(out DlmsErrorCode errorCode)
        {
            fixed (HdlcCommunicationParameters* comParamsPtr = &comParams)
                return IMRCore.Dlms.Login(media.ReadFromMedia, media.WriteAllToMedia, comParamsPtr, settings.Password, out errorCode);
        }

        public unsafe MediaError Logout(out DlmsErrorCode errorCode)
        {
            fixed (HdlcCommunicationParameters* comParamsPtr = &comParams)
                return IMRCore.Dlms.Logout(media.ReadFromMedia, media.WriteAllToMedia, comParamsPtr, settings.Password, out errorCode);
        }

        public unsafe MediaError ReadRegister(DlmsRegister register, out DlmsErrorCode errorCode)
        {
            fixed (DlmsRegisterInfo* regInfoPtr = &register.RegisterInfo)
            {
                fixed (HdlcCommunicationParameters* comParamsPtr = &comParams)
                    return IMRCore.Dlms.ReadRegister(media.ReadFromMedia, media.WriteAllToMedia, comParamsPtr, regInfoPtr, out errorCode);
            }
        }

        public MediaError ReadRegisters(DlmsRegisterCollection registers)
        {
            MediaError ret = MediaError.None;
            DlmsErrorCode errorCode = DlmsErrorCode.None;
            for (int i = 0; i < registers.Count; i++)
            {
                ret = ReadRegister(registers[i], out errorCode);
                registers[i].RegisterInfo.ErrorCode = errorCode;
                if (ret != MediaError.None)
                    return ret;
            }
            return ret;
        }

        public unsafe MediaError ReadProfileChannels(DlmsSurvey survey, out DlmsProfileChannelInfo[] channels, out DlmsErrorCode errorCode)
        {
            DlmsProfileInfo fileInfo = new DlmsProfileInfo(survey.LogicalAddress);
            DlmsProfileChannelInfo* channelsPtr = null;
            byte channelsCount;
            MediaError ret;
            try
            {
                fixed (HdlcCommunicationParameters* comParamsPtr = &comParams)
                    ret = IMRCore.Dlms.ReadProfileChannels(media.ReadFromMedia, media.WriteAllToMedia, comParamsPtr, fileInfo, out channelsPtr, out channelsCount, out errorCode);
                if (ret != MediaError.None || errorCode != DlmsErrorCode.None)
                {
                    // Cleanup
                    IMRCore.Release(channelsPtr);

                    channels = null;
                    return ret;
                }
                //channels = new DlmsProfileChannelInfo[channelsCount];
                //for (int c = 0; c < channelsCount; c++)
                //    channels[c] = new DlmsProfileChannelInfo(channelsPtr[c].ClassID, channelsPtr[c].Address, channelsPtr[c].AttributeIndex, channelsPtr[c].DataIndex, channels[c].Scaler, channels[c].UnitCode);
                List<DlmsProfileChannelInfo> list = new List<DlmsProfileChannelInfo>();
                for (int c = 0; c < channelsCount; c++)
                {
                    if (channelsPtr[c].IsRegister)
                        list.Add(new DlmsProfileChannelInfo(channelsPtr[c].ClassID, channelsPtr[c].Address, channelsPtr[c].AttributeIndex, channelsPtr[c].DataIndex, channelsPtr[c].Scaler, channelsPtr[c].UnitCode));
                }
                channels = list.ToArray();

                // Cleanup
                IMRCore.Release(channelsPtr);

                return ret;
            }
            catch (Exception ex)
            {
                // Cleanup
                if (channelsPtr != null)
                    IMRCore.Release(channelsPtr);
                throw ex;
            }
        }

        /// <summary>
        /// Read all profile channels.
        /// </summary>
        /// <param name="fromDate">From date.</param>
        /// <param name="toDate">To Date, toDate can be null to read to the end of profile.</param>
        /// <param name="profile">Profile table</param>
        /// <returns>Returns a DlmsReturnCode</returns>
        private unsafe MediaError ReadProfile(DlmsSurvey survey, DateTime fromDate, DateTime? toDate, out DlmsProfile profile, out DlmsErrorCode errorCode)
        {
            // Read All Channels
            MediaError ret = ReadProfileChannels(survey, out DlmsProfileChannelInfo[] channels, out errorCode);
            if (ret != MediaError.None || errorCode != DlmsErrorCode.None)
            {
                profile = null;
                return ret;
            }

            // Read Profile
            return ReadProfile(survey, fromDate, toDate, channels, out profile, out errorCode);
        }

        /// <summary>
        /// Read selected profile channels
        /// </summary>
        /// <param name="from">From Date</param>
        /// <param name="to">To date, can be null to read to current date</param>
        /// <param name="profile">Profile table</param>
        /// <returns>Returns a DlmsReturnCode</returns>
        public unsafe MediaError ReadProfile(DlmsSurvey survey, DateTime fromDate, DateTime? toDate, DlmsProfileChannelInfo[] channels, out DlmsProfile profile, out DlmsErrorCode errorCode)
        {
            DlmsProfileInfo fileInfo = new DlmsProfileInfo(survey.LogicalAddress);
            DlmsProfileField* fields = null;
            int rowsCount;
            DlmsDateTime from = DlmsDateTime.FromDateTime(fromDate);
            DlmsDateTime to = DlmsDateTime.FromDateTime(toDate);
            MediaError ret;

            if (channels == null)
                throw new Exception("channels can not be null.");
            else if (channels.Length == 0 || channels.Length > 255)
                throw new Exception("channels length should be greater than 0 and less than 4.");

            try
            {
                fixed (HdlcCommunicationParameters* comParamsPtr = &comParams)
                {
                    fixed (DlmsProfileChannelInfo* channelsPtr = channels)
                        ret = IMRCore.Dlms.ReadProfile(media.ReadFromMedia, media.WriteAllToMedia, comParamsPtr, &from, &to, fileInfo, channelsPtr, (byte)channels.Length, out fields, out rowsCount, out errorCode);
                }
                if (ret != MediaError.None || errorCode != DlmsErrorCode.None)
                {
                    // Cleanup
                    IMRCore.Release(fields);

                    profile = null;
                    return ret;
                }

                DateTime correctedFromDate = new DateTime(fromDate.Year, fromDate.Month, fromDate.Day, fromDate.Hour, fromDate.Minute, 0);
                profile = new DlmsProfile(survey, correctedFromDate, channels);
                int columnsCount = channels.Length;
                int index = 0;
                for (int row = 0; row < rowsCount; row++)
                {
                    DlmsProfileRow pRow = new DlmsProfileRow(columnsCount);
                    for (byte column = 0; column < columnsCount; column++)
                    {
                        pRow.Fields[column] = fields[index];
                        index++;
                    }
                    profile.Add(pRow);
                }

                // Cleanup
                IMRCore.Release(fields);

                return ret;
            }
            catch (Exception ex)
            {
                // Cleanup
                if (fields != null)
                    IMRCore.Release(fields);
                throw ex;
            }
        }
    }
}
