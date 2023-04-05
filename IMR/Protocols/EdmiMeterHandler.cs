// Authored by: MohammadAmin Mohammadi (https://github.com/mamohammadi)

using IMR.Log;
using IMR.MediaInterfaces;
using IMR.Types;
using System;
using System.Collections.Generic;

namespace IMR.Protocols
{
    public class EdmiMeterHandler
    {
        private Media media;
        private readonly EdmiMeterSettings MeterSettings;

        public EdmiMeterHandler(EdmiMeterSettings settings, Media media)
        {
            this.media = media;
            this.MeterSettings = settings;
            IMRCore.Init();
        }

        public unsafe MediaError Login(out EdmiErrorCode errorCode)
        {
            return IMRCore.Edmi.Login(media.ReadFromMedia, media.WriteAllToMedia, MeterSettings.SerialNumber, MeterSettings.Username, MeterSettings.Password, out errorCode);
        }

        public unsafe MediaError Logout(out EdmiErrorCode errorCode)
        {
            return IMRCore.Edmi.Logout(media.ReadFromMedia, media.WriteAllToMedia, MeterSettings.SerialNumber, out errorCode);
        }

        public unsafe MediaError ReadRegister(EdmiRegister register, out EdmiErrorCode errorCode)
        {
            fixed (EdmiRegisterInfo* registerPtr = &register.RegisterInfo)
                return IMRCore.Edmi.ReadRegister(media.ReadFromMedia, media.WriteAllToMedia, MeterSettings.SerialNumber, registerPtr, out errorCode);
        }

        public unsafe MediaError ReadRegisters(EdmiRegisterCollection registers, out EdmiErrorCode errorCode)
        {
            int count = registers.Count;
            EdmiRegisterInfo[] registersInfo = new EdmiRegisterInfo[count];

            fixed (EdmiRegisterInfo* registersInfoPtr = registersInfo)
            {
                for (int i = 0; i < count; i++)
                    registersInfoPtr[i] = registers[i].RegisterInfo;

                MediaError ret = IMRCore.Edmi.ReadRegisters(media.ReadFromMedia, media.WriteAllToMedia, MeterSettings.SerialNumber, registersInfoPtr, count, out errorCode);
                if (ret != MediaError.None || errorCode != EdmiErrorCode.None)
                    return ret;
            }

            for (int i = 0; i < count; i++)
                registers[i].RegisterInfo = registersInfo[i];

            return MediaError.None;
        }

        public unsafe MediaError ReadProfile(EdmiSurvey survey, DateTime fromDate, DateTime? toDate, out EdmiProfile profile, out EdmiErrorCode errorCode)
        {
            MediaError ret = IMRCore.Edmi.ReadProfile(media.ReadFromMedia, media.WriteAllToMedia, MeterSettings.SerialNumber, survey.Address, fromDate, toDate, out EdmiProfileSpecification profileSpec, out EdmiProfileField* fields, out errorCode);
            if(ret != MediaError.None || errorCode != EdmiErrorCode.None)
            {
                // Cleanup
                IMRCore.Release(fields);

                profile = null;
                return ret;
            }

            profile = new EdmiProfile(profileSpec);
            int index = 0;
            for (int record = 0; record < profileSpec.RecordsCount; record++)
            {
                for (byte channel = 0; channel < profileSpec.ChannelsCount; channel++)
                {
                    profile[record].Fields[channel] = fields[index];
                    index++;
                }
            }

            // Cleanup
            IMRCore.Release(fields);

            return ret;
        }
    }
}
