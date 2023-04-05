// Authored by: MohammadAmin Mohammadi (https://github.com/mamohammadi)

using System;
using System.Text;

namespace IMR
{
    internal static class Extensions
    {
        public static byte[] Mid(this byte[] buffer, int index, int count = -1)
        {
            count = (count == -1) ? buffer.Length - index : count;

            byte[] result = new byte[count];
            Buffer.BlockCopy(buffer, index, result, 0, count);
            return result;
        }

        public unsafe static string ToANSIString(this byte[] buffer)
        {
            int count = buffer.Length -1;
            fixed(byte* pt = buffer)
            {
                fixed(char* ch = new char[count])
                {
                    for (int i = 0; i < count; i++)
                        ch[i] = (char)pt[i];
                    return new string(ch, 0, count);
                }
            }
        }

        public static string ToHexString(this byte[] buffer)
        {
            StringBuilder hex = new StringBuilder(buffer.Length * 2);
            foreach (byte b in buffer)
                hex.AppendFormat("{0:x2}", b);
            return hex.ToString();
        }

        private static unsafe void Reverse(byte* pt, int byteLen)
        {
            int count = byteLen >> 1;
            for (int i = 0, j = byteLen - 1; i < count; i++, j--)
            {
                byte a = pt[i];
                pt[i] = pt[j];
                pt[j] = a;
            }
        }

        public static unsafe short ToInt16(this byte[] buffer)
        {
            if (buffer.Length < 2)
                return buffer[0];

            short value;
            fixed (byte* pt = buffer)
            {
                Reverse(pt, 2);
                value = *(short*)pt;
            }
            return value;
        }

        public static unsafe ushort ToUInt16(this byte[] buffer)
        {
            if (buffer.Length < 2)
                return buffer[0];

            ushort value;
            fixed (byte* pt = buffer)
            {
                Reverse(pt, 2);
                value = *(ushort*)pt;
            }
            return value;
        }

        public static unsafe int ToInt32(this byte[] buffer)
        {
            if (buffer.Length < 4)
                return ToInt16(buffer);

            int value;
            fixed (byte* pt = buffer)
            {
                Reverse(pt, 4);
                value = *(int*)pt;
            }
            return value;
        }

        public static unsafe uint ToUInt32(this byte[] buffer)
        {
            if (buffer.Length < 4)
                return ToUInt16(buffer);

            uint value;
            fixed (byte* pt = buffer)
            {
                Reverse(pt, 4);
                value = *(uint*)pt;
            }
            return value;
        }

        public static unsafe long ToInt64(this byte[] buffer)
        {
            if (buffer.Length < 8)
                return ToInt32(buffer);

            long value;
            fixed (byte* pt = buffer)
            {
                Reverse(pt, 8);
                value = *(long*)pt;
            }
            return value;
        }

        public static unsafe ulong ToUInt64(this byte[] buffer)
        {
            if (buffer.Length < 8)
                return ToUInt32(buffer);

            ulong value;
            fixed (byte* pt = buffer)
            {
                Reverse(pt, 8);
                value = *(ulong*)pt;
            }
            return value;
        }

        public static unsafe float ToSingle(this byte[] buffer)
        {
            if (buffer.Length < 4)
                return ToInt16(buffer);

            float value;
            fixed (byte* pt = buffer)
            {
                Reverse(pt, 4);
                value = *(float*)pt;
            }
            return value;
        }

        public static unsafe double ToDouble(this byte[] buffer)
        {
            if (buffer.Length < 8)
                return ToInt32(buffer);

            double value;
            fixed (byte* pt = buffer)
            {
                Reverse(pt, 8);
                value = *(double*)pt;
            }
            return value;
        }
    }
}
