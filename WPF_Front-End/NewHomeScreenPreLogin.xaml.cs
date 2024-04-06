using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace WPF_Front_End
{
    /// <summary>
    /// Interaction logic for NewHomeScreenPreLogin.xaml
    /// </summary>
    public partial class NewHomeScreenPreLogin : Window
    {

        [DllImport("TCP_Client.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr setupConnection2();


        //[DllImport("TCP_Client.dll")]
        //public static extern int sendData(MySocket ClientSocket);


        


        private void ClientConnection()
        {
            if (globalVariables.initialLogin)
            {
                // Call the setupConnection function from the C++ DLL
                IntPtr socketHandle = setupConnection2();

                // Convert the IntPtr to MySocket
                MySocket mySocket = new MySocket();
                mySocket.SetHandle(socketHandle);

                MySocket.ClientSocket = mySocket;

                // Use mySocket as needed
                //sendData(MySocket.ClientSocket);


                // Remember to release the resources when done
                //mySocket.Dispose();


                //setupConnection();

                IntPtr PktPtr = Packet.CreatePacket();

                IntPtr Head = Packet.AllocateHeaderPtr();

                Packet.SetHeaderInformation(ref Head, "127.0.0.1", "127.0.0.1", Route.STARTUP_GETID, false);

                uint bodyLength = 0;

                Marshal.StructureToPtr(bodyLength, Head + ((int)ConstantVariables.Source_Destination_ByteArraySize + (int)ConstantVariables.Source_Destination_ByteArraySize + (int)ConstantVariables.Route_ByteArraySize + sizeof(bool)) * sizeof(byte), false);

                Packet.SetHeader(PktPtr, Head);


                IntPtr serializedRecv = Packet.SerializeHeaderOnlyPkt(PktPtr, out Packet.totalPktSize);


                Packet.TxBuffer = new byte[Packet.totalPktSize];


                Marshal.Copy(serializedRecv, Packet.TxBuffer, 0, Packet.totalPktSize);


                Packet.sendData(MySocket.ClientSocket, Packet.TxBuffer, Packet.totalPktSize);


                Packet.TxBuffer = null;

                Packet.FreeBuffer(ref serializedRecv);
                serializedRecv = IntPtr.Zero;

                Packet.FreeBuffer(ref Head);
                Head = IntPtr.Zero;

                Packet.DestroyPacket(ref PktPtr);
                PktPtr = IntPtr.Zero;



                IntPtr recvBuffer = IntPtr.Zero;

                int bufferSize = 500;

                Packet.RxBuffer = new byte[bufferSize];

                int recvSize = Packet.recvData(MySocket.ClientSocket, ref recvBuffer, bufferSize);

                Marshal.Copy(recvBuffer, Packet.RxBuffer, 0, bufferSize);


                Header head = new Header();

                head.Source = new byte[ConstantVariables.Source_Destination_ByteArraySize];
                head.Destination = new byte[ConstantVariables.Source_Destination_ByteArraySize];
                head.Route = new byte[ConstantVariables.Route_ByteArraySize];

                Packet.DeserializeHeader(Packet.RxBuffer, ref head);


                globalVariables.ClientID = Packet.DeserializeClientID(Packet.RxBuffer);


                //MessageBox.Show(globalVariables.ClientID.ToString());

                globalVariables.initialLogin = false;

                Packet.FreeBuffer(ref recvBuffer);
            }
        }



        private void SendRequestToGetCountOfPosts()
        {
            IntPtr PktPtr = Packet.CreatePacket();


            IntPtr Head = Packet.AllocateHeaderPtr();

            Packet.SetHeaderInformation(ref Head, "127.0.0.1", "127.0.0.1", Route.PRELOGIN_LISTINGS_COUNT, true);

            uint length = 0;

            Marshal.StructureToPtr(length, Head + (int)(ConstantVariables.Source_Destination_ByteArraySize + ConstantVariables.Source_Destination_ByteArraySize + ConstantVariables.Route_ByteArraySize) * sizeof(byte) + sizeof(bool), false);


            Packet.SetHeader(PktPtr, Head);

            IntPtr BodyBuffer = IntPtr.Zero;
            int bodyBufferSize = 0;


            Packet.SetBody(PktPtr, globalVariables.ClientID, BodyBuffer, bodyBufferSize);

            IntPtr serializedData = Packet.SerializeMyPostCountData(PktPtr, out Packet.totalPktSize);


            Packet.TxBuffer = new byte[Packet.totalPktSize];


            Marshal.Copy(serializedData, Packet.TxBuffer, 0, Packet.totalPktSize);


            Packet.sendData(MySocket.ClientSocket, Packet.TxBuffer, Packet.totalPktSize);


            Packet.FreeBuffer(ref Head);
            Head = IntPtr.Zero;

            Packet.DestroyPacket(ref PktPtr);
            PktPtr = IntPtr.Zero;
        }


        private int RecvAckOfNumberOfPosts()
        {
            IntPtr recvBuffer = IntPtr.Zero;

            int bufferSize = 500;

            byte[] RxBuffer = new byte[bufferSize];

            Packet.recvData(MySocket.ClientSocket, ref recvBuffer, bufferSize);

            Marshal.Copy(recvBuffer, RxBuffer, 0, bufferSize);

            IntPtr PktPtr = Packet.CreatePacket();

            int numberOfPosts;

            Packet.DeserializePostCountBuffer(PktPtr, RxBuffer, out numberOfPosts);

            Packet.DestroyPacket(ref PktPtr);
            PktPtr = IntPtr.Zero;

            RxBuffer = null;

            Packet.FreeBuffer(ref recvBuffer);


            return numberOfPosts;
        }



        private void recvPosts(int numberOfPosts)
        {
            while (numberOfPosts > 0)
            {
                IntPtr recvBuffer = IntPtr.Zero;

                int bufferSize = 2500000;

                byte[] RxBuffer = new byte[bufferSize];

                Packet.recvData(MySocket.ClientSocket, ref recvBuffer, bufferSize);

                Marshal.Copy(recvBuffer, RxBuffer, 0, bufferSize);

                Listing list = new Listing();

                list.Title = new byte[ConstantVariables.listing_ByteArraySize];

                list.Location = new byte[ConstantVariables.listing_ByteArraySize];

                list.Condition = new byte[ConstantVariables.listing_ByteArraySize];

                list.EstimatedWorth = new byte[ConstantVariables.listing_ByteArraySize];

                list.Delivery = new byte[ConstantVariables.listing_ByteArraySize];

                list.LookingFor = new byte[ConstantVariables.listing_ByteArraySize];



                list.Title = new byte[200];
                Array.Copy(RxBuffer, 0, list.Title, 0, 200);

                list.Location = new byte[200];
                Array.Copy(RxBuffer, 200, list.Location, 0, 200);

                list.Condition = new byte[200];
                Array.Copy(RxBuffer, 400, list.Condition, 0, 200);

                list.EstimatedWorth = new byte[200];
                Array.Copy(RxBuffer, 600, list.EstimatedWorth, 0, 200);

                list.Delivery = new byte[200];
                Array.Copy(RxBuffer, 800, list.Delivery, 0, 200);

                list.LookingFor = new byte[200];
                Array.Copy(RxBuffer, 1000, list.LookingFor, 0, 200);


                //uint imageSize = BitConverter.ToUInt32(RxBuffer, 1200);

                uint imageSize = Packet.DeserializeImageLength(RxBuffer, 1200);

                globalVariables.MyPostImage1 = new byte[imageSize];

                list.ImageStructArray = Packet.AllocateHeapMemory((int)imageSize);

                Marshal.Copy(RxBuffer, 1200 + sizeof(uint), list.ImageStructArray, (int)imageSize);

                //Marshal.Copy(list.ImageStructArray, globalVariables.MyPostImage1, 0, (int)imageSize);

                //Packet.FreeBuffer(ref list.ImageStructArray);
                //list.ImageStructArray = IntPtr.Zero;


                AddListing(ref list);

                AddImageSize(ref imageSize);

                numberOfPosts -= 1;



                String msg_str = "I am ready to receive next post :)";

                byte[] message = Encoding.ASCII.GetBytes(msg_str);


                int sendSize = Packet.sendData(MySocket.ClientSocket, message, message.Length);



                Packet.FreeBuffer(ref recvBuffer);
            }
        }


        void AddListing(ref Listing newListing)
        {
            globalVariables.listings.Add(newListing);
        }

        void AddImageSize(ref uint imageSize)
        {
            globalVariables.MyPostImageSize.Add(imageSize);
        }



        void ShowListing()
        {
            for (int i = 0; i < globalVariables.listings.Count; i++)
            {
                byte[] imageArray = new byte[(int)globalVariables.MyPostImageSize[i]];

                Marshal.Copy(globalVariables.listings[i].ImageStructArray, imageArray, 0, (int)globalVariables.MyPostImageSize[i]);

                //IntPtr imagePtr = globalVariables.listings[i].ImageStructArray;

                //Packet.FreeBuffer(ref imagePtr);
                //imagePtr = IntPtr.Zero;

                Post p = new Post
                {
                    Image = ImageConversion.ToImage(imageArray),
                    Title = System.Text.Encoding.Default.GetString(globalVariables.listings[i].Title),
                    EstimatedWorth = System.Text.Encoding.Default.GetString(globalVariables.listings[i].EstimatedWorth),
                    Location = System.Text.Encoding.Default.GetString(globalVariables.listings[i].Location),
                    Condition = System.Text.Encoding.Default.GetString(globalVariables.listings[i].Condition),
                    Delivery = System.Text.Encoding.Default.GetString(globalVariables.listings[i].Delivery),
                    LookingFor = System.Text.Encoding.Default.GetString(globalVariables.listings[i].LookingFor)
                };


                globalVariables.exchangePosts.Add(p);
            }
        }


        public NewHomeScreenPreLogin()
        {
            InitializeComponent();

            globalVariables.ClearDynamicArrays();

            ClientConnection();

            SendRequestToGetCountOfPosts();

            int numberOfMyPosts = RecvAckOfNumberOfPosts();

            recvPosts(numberOfMyPosts);

            ShowListing();

            Closing += CloseClient.Client_Closing;

            CollapseHamburgerProfile();

            Hamburger.Listings.Background = Brushes.LightGray;

            //Posts = new ObservableCollection<Post>
            //{
            //    new Post
            //        {
            //            Image = "/Images/Image1.jpg",
            //            Title = "Item 1",
            //            EstimatedWorth = "$100",
            //            Location = "City A",
            //            Condition = "Good",
            //            Delivery = "Local",
            //            LookingFor = "Item X",

            //        },
            //    new Post
            //        {
            //            Image = "/Images/tv.jpg",
            //            Title = "Item 2",
            //            EstimatedWorth = "$50",
            //            Location = "City B",
            //            Condition = "Excellent",
            //            Delivery = "Nationwide",
            //            LookingFor = "Item Y",
            //        },
            //    new Post
            //        {
            //            Image = "/Images/Image1.jpg",
            //            Title = "Item 3",
            //            EstimatedWorth = "$150",
            //            Location = "City C",
            //            Condition = "Excellent",
            //            Delivery = "Nationwide",
            //            LookingFor = "Item Z"

            //        }
            //};

            // Set the Items as the DataContext for the ListBox
            listBox.DataContext = globalVariables.exchangePosts;
        }


        private void CollapseHamburgerProfile()
        {
            Hamburger.Profile.Visibility = Visibility.Collapsed;

            Hamburger.profile_icon.Visibility = Visibility.Collapsed;

            Hamburger.LogOut.Visibility = Visibility.Collapsed;
        }

        private void login_Click(object sender, RoutedEventArgs e)
        {
            var newForm = new MainWindow(); //create your new form.

            double windowWidth = this.ActualWidth;
            double windowHeight = this.ActualHeight;

            newForm.Width = windowWidth;
            newForm.Height = windowHeight;

            newForm.Show(); //show the new form.

            Closing -= CloseClient.Client_Closing;

            this.Close(); //only if you want to close the current form.
        }

        private void CreateAccount_Click(object sender, RoutedEventArgs e)
        {
            Window parentWindow = Window.GetWindow(this);

            double windowWidth = parentWindow.ActualWidth;
            double windowHeight = parentWindow.ActualHeight;

            SignUpForm signup = new SignUpForm();

            signup.Width = windowWidth;
            signup.Height = windowHeight;


            signup.Show();

            Closing -= CloseClient.Client_Closing;

            parentWindow.Close();
        }
    }
}
