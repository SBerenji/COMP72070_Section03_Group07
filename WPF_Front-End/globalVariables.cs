using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Metadata;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WPF_Front_End
{
    public class Packet
    {
        const String dllpath = "TCP_Client.dll";

        public static int totalPktSize;

        public static byte[] TxBuffer = new byte[500]; 

        [DllImport("TCP_Client.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CreatePacket();


        [DllImport(dllpath)]
        public static extern void DestroyPacket(IntPtr Pkt);


        [DllImport(dllpath)]
        public static extern void SetData(IntPtr Pkt, byte[] ListingsData, byte[] MessageData, int ListingSize, int MessageSize);


        [DllImport(dllpath)]
        public static extern IntPtr SerializeData(IntPtr Pkt, out int TotalSize);


        [DllImport(dllpath)]
        public static extern int sendData(MySocket ClientSocket, byte[] TxBuffer, int totalPktSize);
    }

    public static class globalVariables
    {
        public static string username { get; set; }

        public static bool initialLogin = true;
    }



    public class MySocket : SafeHandle
    {
        public static MySocket ClientSocket { get; set; }

        public MySocket() : base(IntPtr.Zero, true) { }

        public override bool IsInvalid => handle == IntPtr.Zero;

        protected override bool ReleaseHandle()
        {
            // Add cleanup logic if needed
            // For example, if using Winsock, you might need to call closesocket(handle)


            return true;
        }

        public void SetHandle(IntPtr handle)
        {
            this.handle = handle;
        }
    }
}