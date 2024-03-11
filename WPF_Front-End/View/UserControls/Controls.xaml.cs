using System;
using System.Collections.Generic;
using System.DirectoryServices;
using System.Linq;
using System.Net.Security;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using WPF_Front_End;

namespace WPF_Front_End.View.UserControls
{
    /// <summary>
    /// Interaction logic for Controls.xaml
    /// </summary>
    public partial class Controls : UserControl
    {

        string userinputUsernameText;

        public Controls()
        {
            InitializeComponent();
        }




        private void login_Click(object sender, RoutedEventArgs e)
        {
            globalVariables.username = username.txtInput.Text;

            IntPtr PktPtr = Packet.CreatePacket();

            IntPtr Head = Packet.AllocateHeaderPtr();

            //IntPtr Head = Marshal.AllocHGlobal(Marshal.SizeOf<Header>());

            Packet.SetHeaderInformation(ref Head , "127.0.0.1", "127.0.0.1", Route.LOGIN, true);

            Packet.SetHeader(PktPtr, Head);

            Packet.SetBody(PktPtr, '1', Encoding.ASCII.GetBytes(globalVariables.username), globalVariables.username.Length);

            IntPtr serializedRecv = Packet.SerializeData(PktPtr, out Packet.totalPktSize);

            //byte[] TxBuffer = new byte[Packet.totalPktSize];

            //Packet.DeallocateMemoryGivenToIntPtr(Head);

            Packet.TxBuffer = new byte[Packet.totalPktSize];


            Marshal.Copy(serializedRecv, Packet.TxBuffer, 0, Packet.totalPktSize);

            Packet.sendData(MySocket.ClientSocket, Packet.TxBuffer, Packet.totalPktSize);


            Packet.TxBuffer = null;

            Packet.FreeBuffer(Head);
            Head = IntPtr.Zero;

            Packet.FreeBuffer(serializedRecv);
            serializedRecv = IntPtr.Zero;
            
            Packet.DestroyPacket(PktPtr);
            PktPtr = IntPtr.Zero;

            Marshal.FreeHGlobal(Head);
            Head = IntPtr.Zero;

            //Packet.DeallocateMemoryGivenToIntPtr(Head);

             Window parentWindow = Window.GetWindow(this);

            double windowWidth = parentWindow.ActualWidth;
            double windowHeight = parentWindow.ActualHeight;

            NewHomeScreenPostLogin hspl = new NewHomeScreenPostLogin();

            hspl.Width = windowWidth;
            hspl.Height = windowHeight;

            hspl.Show();

            parentWindow.Closing -= CloseClient.Client_Closing;

            parentWindow.Close();
        }

        private void createAccount_Click_1(object sender, RoutedEventArgs e)
        {
            Window parentWindow = Window.GetWindow(this);

            double windowWidth = parentWindow.ActualWidth;
            double windowHeight = parentWindow.ActualHeight;

            SignUpForm signup = new SignUpForm();

            signup.Width = windowWidth;
            signup.Height = windowHeight;

            signup.Show();

            parentWindow.Closing -= CloseClient.Client_Closing;

            parentWindow.Close();
        }

        private void username_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            this.userinputUsernameText = username.txtInput.Text;
        }


        public string getUserInputtedUsername()
        {
            return this.userinputUsernameText;
        }

        private void BackButton_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Window parentWindow = Window.GetWindow(this);

            double windowWidth = parentWindow.ActualWidth;
            double windowHeight = parentWindow.ActualHeight;

            NewHomeScreenPreLogin nhspl = new NewHomeScreenPreLogin();

            nhspl.Width = windowWidth;
            nhspl.Height = windowHeight;

            nhspl.Show();

            parentWindow.Closing -= CloseClient.Client_Closing;

            parentWindow.Close();
        }
    }
}