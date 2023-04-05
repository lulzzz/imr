// Authored by: MohammadAmin Mohammadi (https://github.com/mamohammadi)

using IMR.Types;
using IMR.Log;
using System;
using System.Net.Sockets;

namespace IMR.MediaInterfaces
{
    public class NetMedia : Media
    {
        private bool init = false;
        private string hostname;
        private int port;
        private TcpClient tcpClient = new TcpClient();

        private bool CheckInit()
        {
            if (!init)
            {
                Loger.Log("Net media is not initiated.");
                return false;
            }
            return true;
        }

        public NetMedia(string ip, int port, int timeout = 5000)
            : base(timeout)
        {
            if (string.IsNullOrWhiteSpace(ip))
                throw new Exception("Net media host name is null or whitespace.");

            this.hostname = ip;
            this.port = port;
            tcpClient.SendTimeout = timeout;
            tcpClient.ReceiveTimeout = timeout;
            init = true;
        }

        public override bool Connect()
        {
            if (!CheckInit())
                return false;

            try
            {
                tcpClient.Connect(this.hostname, this.port);
            }
            catch (Exception ex)
            {
                Loger.Log(ex.ToString());
                return false;
            }
            return true;
        }

        public override bool Disconnect()
        {
            if (!CheckInit())
                return false;

            try
            {
                tcpClient.GetStream().Close();
                tcpClient.Close();
            }
            catch (Exception ex)
            {
                Loger.Log(ex.ToString());
                return false;
            }
            return true;
        }

        public override MediaError Read(out byte b)
        {
            b = 0;

            if (!CheckInit())
                return MediaError.NotInitiated;

            if (!tcpClient.Connected)
            {
                Loger.Log("Can not read from Net media. Net media is not connected.");
                return MediaError.NotConnected;
            }

            try
            {
                NetworkStream stream = tcpClient.GetStream();
                int available = tcpClient.Available;
                if (available > 0)
                {
                    int res = stream.ReadByte();
                    if (res == -1)
                        return MediaError.ReadError;
                    b = (byte)res;
                    return MediaError.None;
                }
                else
                    return MediaError.ReadError;
            }
            catch (Exception ex)
            {
                Loger.Log(ex.ToString());
                return MediaError.ReadError;
            }
        }

        public override byte[] ReadAll()
        {
            if (!CheckInit())
                //return MediaError.DeviceIsNotInitiated;
                return new byte[0];

            if (!tcpClient.Connected)
            {
                Loger.Log("Can not read from Net media. Net media is not connected.");
                //return MediaError.DeviceIsNotOpen;
                return new byte[0];
            }

            try
            {
                NetworkStream stream = tcpClient.GetStream();
                int available = tcpClient.Available;
                if (available > 0)
                {
                    byte[] buffer = new byte[available];
                    stream.Read(buffer, 0, available);

                    Loger.Log("Media << " + buffer.ToHexString());

                    return buffer;
                }
                else
                    return new byte[0];
            }
            catch (Exception ex)
            {
                Loger.Log(ex.ToString());
                return new byte[0];
            }
        }

        public override MediaError WriteAll(byte[] buffer)
        {
            if (!CheckInit())
                return MediaError.NotInitiated;

            if (!tcpClient.Connected)
            {
                Loger.Log("Can not write into Net media. Net media is not connected.");
                return MediaError.NotConnected;
            }

            try
            {
                var stream = tcpClient.GetStream();
                stream.Write(buffer, 0, buffer.Length);
                stream.Flush();
            }
            catch (Exception ex)
            {
                Loger.Log(ex.ToString());
                return MediaError.Exception;
            }

            return MediaError.None;
        }

        public override bool WaitUntilReadyRead()
        {
            int counter = 0;
            while (counter <= Timeout)
            {
                if (tcpClient.Available > 0)
                    return true;
                if (counter >= Timeout)
                    return false;
                System.Threading.Thread.Sleep(10);
                counter += 10;
            }
            return false;
        }
    }
}
