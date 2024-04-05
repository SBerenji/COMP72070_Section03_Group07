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
        //public bool _isPasswordChanging;

        //// Using a DependencyProperty as the backing store for MyProperty.  This enables animation, styling, binding, etc...
        //public static readonly DependencyProperty PasswordProperty =
        //    DependencyProperty.Register("Password", typeof(string), typeof(PasswordBox),
        //        new FrameworkPropertyMetadata(string.Empty, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault,
        //            PasswordPropertyChanged, null, false, UpdateSourceTrigger.PropertyChanged));


        //public static void PasswordPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        //{
        //    if (d is PasswordBox passwordBox)
        //    {
        //        UpdatePassword();
        //    }
        //}




        //public string Password
        //{
        //    get { return (string)GetValue(PasswordProperty); }
        //    set { SetValue(PasswordProperty, value); }
        //}

        ////public PasswordBox()
        ////{
        ////    InitializeComponent();
        ////}

        //public string placeholder2;

        //public string Placeholder2
        //{
        //    get { return placeholder2; }
        //    set
        //    {
        //        placeholder2 = value;

        //        tbPlaceholder.Text = placeholder2;
        //    }
        //}

        //public void txtInput_TextChanged(object sender, TextChangedEventArgs e)
        //{

        //}

        //public void passwordBox_PasswordChanged(object sender, RoutedEventArgs e)
        //{
        //    if (string.IsNullOrEmpty(passwordBox.Password))
        //    {
        //        tbPlaceholder.Visibility = Visibility.Visible;
        //    }

        //    else
        //    {
        //        tbPlaceholder.Visibility = Visibility.Hidden;
        //    }

        //    _isPasswordChanging = true;
        //    Password = passwordBox.Password;
        //    _isPasswordChanging = false;
        //}

        //public void UpdatePassword()
        //{
        //    if (!_isPasswordChanging)
        //    {
        //        passwordBox.Password = Password;
        //    }
        //}







    // Define events to notify when password changes and when the control gains/loses focus
    public event EventHandler PasswordChanged;
        public event EventHandler GotFocus;
        public event EventHandler LostFocus;


        //private bool _isPasswordChanging;

        //// Using a DependencyProperty as the backing store for MyProperty.  This enables animation, styling, binding, etc...
        //public static readonly DependencyProperty PasswordProperty =
        //    DependencyProperty.Register("Password", typeof(string), typeof(PasswordBox),
        //        new FrameworkPropertyMetadata(string.Empty, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault,
        //            PasswordPropertyChanged, null, false, UpdateSourceTrigger.PropertyChanged));

        //public void UpdatePassword()
        //{
        //    if (!_isPasswordChanging)
        //    {
        //        passwordBox.Password = Password;
        //    }
        //}

        //private static void PasswordPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        //{
        //    if (d is PasswordBox passwordBox)
        //    {
        //        passwordBox.UpdatePassword();
        //    }
        //}

        string userinputUsernameText;

        public Controls()
        {
            InitializeComponent();


            passwordBox.PasswordChanged += (sender, e) => PasswordChanged?.Invoke(this, EventArgs.Empty);
            passwordBox.GotFocus += (sender, e) => GotFocus?.Invoke(this, EventArgs.Empty);
            passwordBox.LostFocus += (sender, e) => LostFocus?.Invoke(this, EventArgs.Empty);

            //PasswordBox pBox = new PasswordBox();

            //passwordBox.Template = pBox;

            //passwordBox = (System.Windows.Controls.PasswordBox)pBox;

            //passwordBox.PasswordChanged += pBox.passwordBox_PasswordChanged;


            //pBox.Placeholder2 = "Password";

            //tbPlaceholder2.Text = pBox.Placeholder2;


            Placeholder = "Username";
        }

        private string placeholder;

        public string Placeholder
        {
            get { return placeholder; }
            set
            {
                placeholder = value;

                tbPlaceholder.Text = placeholder;
            }
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
        }


        //public string Password
        //{
        //    get { return (string)GetValue(PasswordProperty); }
        //    set { SetValue(PasswordProperty, value); }
        //}






        //public string password_placeholder
        //{
        //    get { return placeholder; }
        //    set
        //    {
        //        placeholder = value;

        //        tbPlaceholder.Text = placeholder;
        //    }
        //}

        //private void password_txtInput_TextChanged(object sender, TextChangedEventArgs e)
        //{

        //}

        //private void passwordBox_PasswordChanged(object sender, RoutedEventArgs e)
        //{
        //    if (string.IsNullOrEmpty(passwordBox.Password))
        //    {
        //        tbPlaceholder.Visibility = Visibility.Visible;
        //    }

        //    else
        //    {
        //        tbPlaceholder.Visibility = Visibility.Hidden;
        //    }

        //    _isPasswordChanging = true;
        //    Password = passwordBox.Password;
        //    _isPasswordChanging = false;
        //}























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


                Packet.SetBody(PktPtr, '1', BodyBuffer, size);


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








    public partial class PasswordBox
    {
        public bool _isPasswordChanging;

        // Using a DependencyProperty as the backing store for MyProperty.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty PasswordProperty =
            DependencyProperty.Register("Password", typeof(string), typeof(PasswordBox),
                new FrameworkPropertyMetadata(string.Empty, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault,
                    PasswordPropertyChanged, null, false, UpdateSourceTrigger.PropertyChanged));

        public static void PasswordPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (d is PasswordBox passwordBox)
            {
                passwordBox.UpdatePassword();
            }
        }




        public string Password
        {
            get { return (string)GetValue(PasswordProperty); }
            set { SetValue(PasswordProperty, value); }
        }

        public PasswordBox()
        {
            InitializeComponent();
        }

        public string placeholder2;

        public string Placeholder2
        {
            get { return placeholder2; }
            set
            {
                placeholder2 = value;

                tbPlaceholder.Text = placeholder2;
            }
        }

        public void txtInput_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        public void passwordBox_PasswordChanged(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrEmpty(passwordBox.Password))
            {
                tbPlaceholder.Visibility = Visibility.Visible;
            }

            else
            {
                tbPlaceholder.Visibility = Visibility.Hidden;
            }

            _isPasswordChanging = true;
            Password = passwordBox.Password;
            _isPasswordChanging = false;
        }

        public void UpdatePassword()
        {
            if (!_isPasswordChanging)
            {
                passwordBox.Password = Password;
            }
        }
    }

}