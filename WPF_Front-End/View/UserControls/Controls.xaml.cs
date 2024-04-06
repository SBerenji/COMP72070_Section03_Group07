using System;
using System.Collections.Generic;
using System.DirectoryServices;
using System.Linq;
using System.Net.Security;
using System.Security;
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
using System.Windows.Navigation;
using System.Windows.Shapes;
using WPF_Front_End;
using System.Runtime.CompilerServices;

namespace WPF_Front_End.View.UserControls
{
    /// <summary>
    /// Interaction logic for Controls.xaml
    /// </summary>
    public partial class Controls : UserControl
    {
        private void passwordBox_PasswordChanged(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrEmpty(passwordBox.Password))
            {
                tbPlaceholder2.Visibility = Visibility.Visible;
            }

            else
            {
                tbPlaceholder2.Visibility = Visibility.Hidden;
            }
        }


        string userinputUsernameText;

        public Controls()
        {
            InitializeComponent();
        }

        private void txtInput_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (string.IsNullOrEmpty(username.Text))
            {
                tbPlaceholder.Visibility = Visibility.Visible;
            }

            else
            {
                tbPlaceholder.Visibility = Visibility.Hidden;
            }
        }

        private void btnClear_Click(object sender, RoutedEventArgs e)
        {
            username.Clear();
            username.Focus();

            passwordBox.Clear();
            passwordBox.Focus();
        }

        private void login_Click(object sender, RoutedEventArgs e)
        {
            if (username.Text == "" || passwordBox.Password == "")
            {
                MessageBox.Show("Please enter Username AND Password!!");
            }

            else
            {
                LogIn login = new LogIn();

                login.username = new byte[ConstantVariables.username_ByteArraySize];
                login.password = new byte[ConstantVariables.password_ByteArraySize];

                globalVariables.username = username.Text;

                globalVariables.password = passwordBox.Password;

                login.username = Encoding.ASCII.GetBytes(globalVariables.username);
                login.password = Encoding.ASCII.GetBytes(globalVariables.password);



                //IntPtr BodyBuffer = Packet.serializeLoginData(login);

                IntPtr BodyBuffer = Packet.AllocateLoginPtr();

                Packet.SetLoginBodyInformation(ref BodyBuffer, login);


                IntPtr PktPtr = Packet.CreatePacket();

                IntPtr Head = Packet.AllocateHeaderPtr();

                //IntPtr Head = Marshal.AllocHGlobal(Marshal.SizeOf<Header>());

                Packet.SetHeaderInformation(ref Head, "127.0.0.1", "127.0.0.1", Route.LOGIN, true);

                Packet.SetHeader(PktPtr, Head);


                int size = ((int)ConstantVariables.username_ByteArraySize + (int)ConstantVariables.password_ByteArraySize) * sizeof(byte);


                Packet.SetBody(PktPtr, globalVariables.ClientID, BodyBuffer, size);


                IntPtr serializedRecv = Packet.SerializeData(PktPtr, out Packet.totalPktSize);

                //byte[] TxBuffer = new byte[Packet.totalPktSize];

                //Packet.DeallocateMemoryGivenToIntPtr(Head);

                Packet.TxBuffer = new byte[Packet.totalPktSize];


                Marshal.Copy(serializedRecv, Packet.TxBuffer, 0, Packet.totalPktSize);

                Packet.sendData(MySocket.ClientSocket, Packet.TxBuffer, Packet.totalPktSize);


                Packet.TxBuffer = null;

                Packet.FreeBuffer(ref Head);
                Head = IntPtr.Zero;

                Packet.FreeBuffer(ref BodyBuffer);
                BodyBuffer = IntPtr.Zero;

                Packet.FreeBuffer(ref serializedRecv);
                serializedRecv = IntPtr.Zero;

                Packet.DestroyPacket(ref PktPtr);
                PktPtr = IntPtr.Zero;

                //Marshal.FreeHGlobal(Head);
                //Head = IntPtr.Zero;

                //Packet.DeallocateMemoryGivenToIntPtr(Head);


                IntPtr recvBuffer = IntPtr.Zero;
                // byte[] RxBuffer = new byte[200000];

                int bufferSize = 2500000;

                Packet.recvData(MySocket.ClientSocket, ref recvBuffer, bufferSize);

                byte[] RxBuffer = new byte[bufferSize];

                Marshal.Copy(recvBuffer, RxBuffer, 0, bufferSize);

                UserCredentials cred = new UserCredentials();

                cred.username = new byte[ConstantVariables.username_ByteArraySize];
                cred.password = new byte[ConstantVariables.password_ByteArraySize];
                cred.email = new byte[ConstantVariables.email_ByteArraySize];

                PktPtr = Packet.CreatePacket();

                Packet.DeserializationWithoutTail(PktPtr, RxBuffer);


                Header head = new Header();

                Packet.DeserializeHeader(RxBuffer, ref head);

                head.Length = Packet.DeserializeHeaderLengthMember(RxBuffer);
                

                int sizeofHeader;

                sizeofHeader = (int)((2 * ConstantVariables.Source_Destination_ByteArraySize) + (ConstantVariables.Route_ByteArraySize) + (Marshal.SizeOf(typeof(bool))) + (Marshal.SizeOf(typeof(UInt32))));

                

                if (Encoding.ASCII.GetString(head.Route).TrimEnd('\0') == Route.LOGIN_USERFOUNDWITHIMAGE.ToString())
                {
                    globalVariables.ClientID = Packet.DeserializeClientID(RxBuffer);

                    Array.Copy(RxBuffer, sizeofHeader + Marshal.SizeOf(typeof(UInt32)), cred.username, 0, ConstantVariables.username_ByteArraySize);
                    Array.Copy(RxBuffer, sizeofHeader + Marshal.SizeOf(typeof(UInt32)) + ConstantVariables.username_ByteArraySize, cred.password, 0, ConstantVariables.password_ByteArraySize);
                    Array.Copy(RxBuffer, sizeofHeader + Marshal.SizeOf(typeof(UInt32)) + ConstantVariables.username_ByteArraySize + ConstantVariables.password_ByteArraySize, cred.email, 0, ConstantVariables.email_ByteArraySize);

                    globalVariables.receivedPostLoginImage = new byte[head.Length - (int)(ConstantVariables.username_ByteArraySize + ConstantVariables.password_ByteArraySize + ConstantVariables.email_ByteArraySize)];

                    int length = globalVariables.receivedPostLoginImage.Length;

                    Packet.CopyImageFromRawBufferToByteArray(RxBuffer, globalVariables.receivedPostLoginImage, ((int)head.Length - (int)(ConstantVariables.username_ByteArraySize + ConstantVariables.password_ByteArraySize + ConstantVariables.email_ByteArraySize)));

                    globalVariables.imageUploaded = true;
                }

                else if (Encoding.ASCII.GetString(head.Route).TrimEnd('\0') == Route.LOGIN_USERFOUNDWITHOUTIMAGE.ToString())
                {
                    globalVariables.ClientID = Packet.DeserializeClientID(RxBuffer);

                    Array.Copy(RxBuffer, sizeofHeader + Marshal.SizeOf(typeof(UInt32)), cred.username, 0, ConstantVariables.username_ByteArraySize);
                    Array.Copy(RxBuffer, sizeofHeader + Marshal.SizeOf(typeof(UInt32)) + ConstantVariables.username_ByteArraySize, cred.password, 0, ConstantVariables.password_ByteArraySize);
                    Array.Copy(RxBuffer, sizeofHeader + Marshal.SizeOf(typeof(UInt32)) + ConstantVariables.username_ByteArraySize + ConstantVariables.password_ByteArraySize, cred.email, 0, ConstantVariables.email_ByteArraySize);

                    globalVariables.imageUploaded = false;
                    globalVariables.receivedPostLoginImage = Array.Empty<byte>();
                }


                RxBuffer = null;

                Packet.DestroyPacket(ref PktPtr);
                PktPtr = IntPtr.Zero;

                Packet.FreeBuffer(ref recvBuffer);


                if (head.Authorization)
                {
                    globalVariables.OneClientFirstSignUp = false;

                    

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
                else
                {
                    MessageBox.Show("User doesn't Exist!! Please sign up instead.");
                }
            }
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
            this.userinputUsernameText = username.Text;
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