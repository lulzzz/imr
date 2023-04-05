// Authored by: MohammadAmin Mohammadi (https://github.com/mamohammadi)

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace IMR.Types
{
    [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Ansi, Size = SIZE)]
    internal struct HdlcCommunicationParameters
    {
        public const int SIZE = 14;

        [FieldOffset(0)]
        internal uint ServerID;

        [FieldOffset(4)]
        internal uint ClientID;

        [FieldOffset(8)]
        internal byte MaxInfoLenTransmit;

        [FieldOffset(9)]
        internal byte MaxInfoLenReceive;

        [FieldOffset(10)]
        internal byte WinSizeTransmit;

        [FieldOffset(11)]
        internal byte WinSizeReceive;

        [FieldOffset(12)]
        internal byte S;

        [FieldOffset(13)]
        internal byte R;
    }
}
