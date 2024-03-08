using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Reflection.Metadata;
using System.Runtime.InteropServices;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Xaml;

namespace WPF_Front_End
{

    public enum Route {LOGIN, SIGNUP, POST}

    public class CloseClient
    {
        [DllImport("TCP_Client.dll")]
        public static extern int CloseSocket(MySocket ClientSocket);

        public static void Client_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            MessageBoxResult result = MessageBox.Show("Do you really want to close the application?", "Confirmation", MessageBoxButton.YesNo);

            if (result == MessageBoxResult.Yes)
            {
                CloseSocket(MySocket.ClientSocket);
            }

            else
            {
                e.Cancel = true;
            }
        }
    }


    public class ConstantVariables
    {
        public const uint Source_Destination_ByteArraySize = 20;

        public const uint Route_ByteArraySize = 10;
    }


    public struct Header
    {
        public byte[] Source;
        public byte[] Destination;
        public byte[] Route;
        public bool Authorization;
        public uint Length;
    }

    
    public class Packet
    {
        const String dllpath = "TCP_Client.dll";

        public static int totalPktSize;

        public static byte[] TxBuffer; 

        public static void SetHeaderInformation(ref IntPtr HeadPtr, String Source, String Destination, Route r, bool Auth) {
            //var VarTypeHead = Marshal.PtrToStructure(HeadPtr, typeof(Header));

            Header Head = new Header();

            Head.Source = new byte[ConstantVariables.Source_Destination_ByteArraySize];
            Head.Source = Encoding.ASCII.GetBytes(Source);


            Head.Destination = new byte[ConstantVariables.Source_Destination_ByteArraySize];
            Head.Destination = Encoding.ASCII.GetBytes(Destination);


            Head.Route = new byte[ConstantVariables.Route_ByteArraySize];
            Head.Route = Encoding.ASCII.GetBytes(r.ToString());


            if (globalVariables.username != "")
            {
                Head.Authorization = true;
            }

            else
            {
                Head.Authorization = false;
            }


            //IntPtr HeadPtr;

            //HeadPtr = Marshal.AllocHGlobal(Marshal.SizeOf<Header>());


            Marshal.Copy(Head.Source, 0, HeadPtr, Head.Source.Length);

            Marshal.Copy(Head.Destination, 0, HeadPtr + (int)ConstantVariables.Source_Destination_ByteArraySize * sizeof(byte), Head.Destination.Length);

            Marshal.Copy(Head.Route, 0, HeadPtr + ((int)ConstantVariables.Source_Destination_ByteArraySize + (int)ConstantVariables.Source_Destination_ByteArraySize) * sizeof(byte), r.ToString().Length);


            Marshal.StructureToPtr(Head.Authorization, HeadPtr + ((int)ConstantVariables.Source_Destination_ByteArraySize + (int)ConstantVariables.Source_Destination_ByteArraySize + (int)ConstantVariables.Route_ByteArraySize) * sizeof(byte), false);

            //Marshal.StructureToPtr(Head.Length, HeadPtr + ((int)ConstantVariables.Source_Destination_ByteArraySize + (int)ConstantVariables.Source_Destination_ByteArraySize + (int)ConstantVariables.Route_ByteArraySize) * sizeof(byte) + sizeof(bool), false);

            //Marshal.FreeHGlobal(HeadPtr);

            //Marshal.StructureToPtr(Head, HeadPtr, false);

            //return ref HeadPtr;
        }


        public static void DeallocateMemoryGivenToIntPtr(IntPtr ptr)
        {
            Marshal.FreeHGlobal(ptr);
        }


        [DllImport(dllpath)]
        public static extern IntPtr CreatePacket();


        [DllImport(dllpath)]
        public static extern IntPtr AllocateHeaderPtr();


        [DllImport(dllpath)]
        public static extern void DestroyPacket(IntPtr Pkt);


        [DllImport(dllpath)]
        public static extern void FreeBuffer(IntPtr Buffer);


        [DllImport(dllpath)]
        public static extern void SetHeader(IntPtr Packet, IntPtr Head);


        [DllImport(dllpath)]
        public static extern void SetBody(IntPtr Pkt, char User, byte[] Data, int DataSize);


        [DllImport(dllpath)]
        public static extern IntPtr SerializeData(IntPtr Pkt, out int TotalSize);


        [DllImport(dllpath)]
        public static extern int sendData(MySocket ClientSocket, byte[] TxBuffer, int totalPktSize);


        [DllImport(dllpath)]
        public static extern void Deserialization(IntPtr Pkt, byte[] src);
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