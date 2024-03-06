using System;
using System.Collections.Generic;
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

            byte[] listingData = new byte[20];

            listingData[0] = (byte)'H';
            listingData[1] = (byte)'e';
            listingData[2] = (byte)'l';
            listingData[3] = (byte)'l';
            listingData[4] = (byte)'o';

            Packet.SetData(PktPtr, listingData, Encoding.ASCII.GetBytes(globalVariables.username), listingData.Length, globalVariables.username.Length);

            IntPtr serializedRecv = Packet.SerializeData(PktPtr, out Packet.totalPktSize);

            //byte[] TxBuffer = new byte[Packet.totalPktSize];

            Marshal.Copy(serializedRecv, Packet.TxBuffer, 0, Packet.totalPktSize);

            Packet.sendData(MySocket.ClientSocket, Packet.TxBuffer, Packet.totalPktSize);

            Window parentWindow = Window.GetWindow(this);

            double windowWidth = parentWindow.ActualWidth;
            double windowHeight = parentWindow.ActualHeight;

            NewHomeScreenPostLogin hspl = new NewHomeScreenPostLogin();

            hspl.Width = windowWidth;
            hspl.Height = windowHeight;

            hspl.Show();

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

            parentWindow.Close();
        }
    }
}