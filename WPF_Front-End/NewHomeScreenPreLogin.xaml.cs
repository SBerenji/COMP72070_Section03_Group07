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



                Packet.RxBuffer = new byte[500];

                int recvSize = Packet.recvData(MySocket.ClientSocket, Packet.RxBuffer, 500);


                Header head = new Header();

                head.Source = new byte[ConstantVariables.Source_Destination_ByteArraySize];
                head.Destination = new byte[ConstantVariables.Source_Destination_ByteArraySize];
                head.Route = new byte[ConstantVariables.Route_ByteArraySize];

                Packet.DeserializeHeader(Packet.RxBuffer, ref head);


                globalVariables.ClientID = Packet.DeserializeClientID(Packet.RxBuffer);


                //MessageBox.Show(globalVariables.ClientID.ToString());

                globalVariables.initialLogin = false;
            }
        }

        public ObservableCollection<Post> Posts { get; set; }

        public NewHomeScreenPreLogin()
        {
            InitializeComponent();

            globalVariables.FirstPostLogin = true;

            ClientConnection();

            Closing += CloseClient.Client_Closing;

            CollapseHamburgerProfile();

            Hamburger.Listings.Background = Brushes.LightGray;

            Posts = new ObservableCollection<Post>
            {
                new Post
                    {
                        Image = "/Images/Image1.jpg",
                        Title = "Item 1",
                        EstimatedWorth = "$100",
                        Location = "City A",
                        Condition = "Good",
                        Delivery = "Local",
                        LookingFor = "Item X",

                    },
                new Post
                    {
                        Image = "/Images/tv.jpg",
                        Title = "Item 2",
                        EstimatedWorth = "$50",
                        Location = "City B",
                        Condition = "Excellent",
                        Delivery = "Nationwide",
                        LookingFor = "Item Y",
                    },
                new Post
                    {
                        Image = "/Images/Image1.jpg",
                        Title = "Item 3",
                        EstimatedWorth = "$150",
                        Location = "City C",
                        Condition = "Excellent",
                        Delivery = "Nationwide",
                        LookingFor = "Item Z"

                    }
            };

            // Set the Items as the DataContext for the ListBox
            listBox.DataContext = Posts;
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
