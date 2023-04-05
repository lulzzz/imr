// Authored by: MohammadAmin Mohammadi (https://github.com/mamohammadi)

using IMR.Log;
using IMR.Types;
using System.Collections.Generic;

namespace IMR.MediaInterfaces
{
    public abstract class Media
    {
        protected int Timeout { get; }

        public abstract bool Connect();
		public abstract bool Disconnect();
		public abstract MediaError Read(out byte b);
		public abstract byte[] ReadAll();
        public abstract MediaError WriteAll(byte[] buffer);

        public abstract bool WaitUntilReadyRead();

        public Media(int timeout)
        {
            Timeout = timeout;
        }

        internal unsafe MediaError WriteAllToMedia(byte* buffer, int length)
        {
            byte[] bytes = new byte[length];
            fixed (byte* ptr = bytes)
            {
                for (int i = 0; i < length; i++)
                    ptr[i] = *buffer++;
            }
            Loger.Log(">> " + bytes.ToHexString());
            return WriteAll(bytes);
        }

#if DEBUG
        private static List<byte> readBuffer = new List<byte>();
#endif
        internal unsafe MediaError ReadFromMedia(byte* b)
        {
            byte readedByte;
            if (!WaitUntilReadyRead())
                return MediaError.ResponseTimeOut;
            MediaError ret = Read(out readedByte);
            *b = readedByte;
#if DEBUG
            readBuffer.Add(readedByte);
            if (readBuffer.Count > 1 && readBuffer[readBuffer.Count - 1] == 0x7E)
            {
                Loger.Log("<< " + readBuffer.ToArray().ToHexString());
                readBuffer.Clear();
            }
#endif
            return ret;
        }
    }
}
