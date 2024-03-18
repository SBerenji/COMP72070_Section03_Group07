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
using System.Windows.Media;
using System.Windows;
using System.Xaml;
using System.Printing.IndexedProperties;
using System.Windows.Media.Imaging;
using System.Reflection.PortableExecutable;

namespace WPF_Front_End
{
    public class ImageConversion
    {
        public static BitmapImage ToImage(byte[] array)
        {
            using (var ms = new System.IO.MemoryStream(array))
            {
                var image = new BitmapImage();
                image.BeginInit();
                image.CacheOption = BitmapCacheOption.OnLoad; // here
                image.StreamSource = ms;
                image.EndInit();
                return image;
            }
        }
    }


    public enum Route {LOGIN, SIGNUP_IMAGEUPLOADED, SIGNUP_IMAGENOTUPLOADED,  SIGNUP_USERCHECK, POST}



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

        public const uint Route_ByteArraySize = 40;

        public const uint username_ByteArraySize = 10;

        public const uint password_ByteArraySize = 20;

        public const uint email_ByteArraySize = 40;
    }


    public struct Header
    {
        public byte[] Source;
        public byte[] Destination;
        public byte[] Route;
        public bool Authorization;
        public uint Length;
    }


    public struct LogIn
    {
        public byte[] username;
        public byte[] password;
    }


    public struct SignUp
    {
        public byte[] username;
        public byte[] password;
        public byte[] email;
        public IntPtr ImageStructArray;
    }


    public struct SignUpCheck {
	    public byte[] username;
        public byte[] email;
    }


public class Packet
    {
        const String dllpath = "TCP_Client.dll";

        public static int totalPktSize;

        public static byte[] TxBuffer;

        public static byte[] RxBuffer;


        public static void SetLoginBodyInformation(ref IntPtr BodyBuffer, LogIn login)
        {
            Marshal.Copy(login.username, 0, BodyBuffer, login.username.Length);

            Marshal.Copy(login.password, 0, BodyBuffer + (int)ConstantVariables.username_ByteArraySize * sizeof(byte), login.password.Length);
        }


        public static void DeserializeHeader(byte[] RxBuffer, ref Header head)
        {
            head.Source = new byte[ConstantVariables.Source_Destination_ByteArraySize];

            Array.Copy(RxBuffer, 0, head.Source, 0, head.Source.Length);


            head.Destination = new byte[ConstantVariables.Source_Destination_ByteArraySize];

            Array.Copy(RxBuffer, ConstantVariables.Source_Destination_ByteArraySize, head.Destination, 0, head.Destination.Length);


            head.Route = new byte[ConstantVariables.Route_ByteArraySize];

            Array.Copy(RxBuffer, ConstantVariables.Source_Destination_ByteArraySize + ConstantVariables.Source_Destination_ByteArraySize, head.Route, 0, head.Route.Length);


            head.Authorization = BitConverter.ToBoolean(RxBuffer, (int)(ConstantVariables.Source_Destination_ByteArraySize + ConstantVariables.Source_Destination_ByteArraySize + ConstantVariables.Route_ByteArraySize));

            unsafe
            {
                int size_auth = sizeof(bool);


                head.Length = BitConverter.ToUInt32(RxBuffer, (int)(ConstantVariables.Source_Destination_ByteArraySize + ConstantVariables.Source_Destination_ByteArraySize + ConstantVariables.Route_ByteArraySize) + size_auth);
            }

            
        }

        public static void SerializeSignUpInformation(ref IntPtr BodyBuffer, SignUp signup, int imageSize)
        {
            //Packet.SerializeStaticDataToBuffer(BodyBuffer, signup.username, signup.password, signup.email);

            Marshal.Copy(signup.username, 0, BodyBuffer, signup.username.Length);

            Marshal.Copy(signup.password, 0, BodyBuffer + (int)ConstantVariables.username_ByteArraySize * sizeof(byte), signup.password.Length);

            Marshal.Copy(signup.email, 0, BodyBuffer + ((int)ConstantVariables.username_ByteArraySize + (int)ConstantVariables.password_ByteArraySize) * sizeof(byte), signup.email.Length);

            Packet.CopyBufferToHeap(BodyBuffer + ((int)ConstantVariables.username_ByteArraySize + (int)ConstantVariables.password_ByteArraySize + (int)ConstantVariables.email_ByteArraySize) * sizeof(byte), signup.ImageStructArray, imageSize);
        }


        public static void SerializeSignUpCheckInfo(ref IntPtr BodyBuffer, SignUpCheck check)
        {
            Marshal.Copy(check.username, 0, BodyBuffer, check.username.Length);

            Marshal.Copy(check.email, 0, BodyBuffer + (int)ConstantVariables.username_ByteArraySize * sizeof(byte), check.email.Length);
        }


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
        public static extern IntPtr AllocateHeapMemory(int size);


        [DllImport(dllpath)]
        public static extern IntPtr AllocateHeaderPtr();


        [DllImport(dllpath)]
        public static extern IntPtr AllocateLoginPtr();


        [DllImport(dllpath)]
        public static extern IntPtr AllocateSignupPtr(int imageSize);


        [DllImport(dllpath)]
        public static extern void SerializeStaticDataToBuffer(IntPtr heapBuffer, byte[] username, byte[] password, byte[] email);


        [DllImport(dllpath)]
        public static extern void CopyBufferToHeap(IntPtr heapBuffer, byte[] srcBuffer, int size);


        [DllImport(dllpath)]
        public static extern void CopyBufferToHeap(IntPtr heapBuffer, IntPtr srcBuffer, int size);


        [DllImport(dllpath)]
        public static extern void DestroyPacket(ref IntPtr Pkt);


        [DllImport(dllpath)]
        public static extern void FreeBuffer(ref IntPtr Buffer);


        [DllImport(dllpath)]
        public static extern void SetHeader(IntPtr Packet, IntPtr Head);


        [DllImport(dllpath)]
        public static extern IntPtr serializeLoginData(LogIn login);


        [DllImport(dllpath)]
        public static extern void SetBody(IntPtr Pkt, char User, IntPtr Data, int DataSize);


        [DllImport(dllpath)]
        public static extern IntPtr SerializeData(IntPtr Pkt, out int TotalSize);


        [DllImport(dllpath)]
        public static extern int sendData(MySocket ClientSocket, byte[] TxBuffer, int totalPktSize);


        [DllImport(dllpath)]
        public static extern int recvData(MySocket ClientSocket, byte[] RxBuffer, int RxBufferSize);


        [DllImport(dllpath)]
        public static extern void Deserialization(IntPtr Pkt, byte[] src);
    }

    public static class globalVariables
    {
        public static string bodyBuffer {  get; set; }

        public static string username { get; set; }

        public static ImageSource profileImageDropped { get; set; }

        public static ImageSource profileImageSelected { get; set; }

        public static bool initialLogin = true;
        public static string password { get; set; }


        public static byte[] ImageArray {  get; set; }

        public static string email { get; set; }


        public static bool imageUploaded { get; set; }


        public static byte[] receivedPostLoginImage { get; set; }


        public static bool FirstPostLogin = true;
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