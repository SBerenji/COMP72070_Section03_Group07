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


        [DllImport("TCP_Client.dll")]
        public static extern int sendData(MySocket ClientSocket);


        


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

                globalVariables.initialLogin = false;
            }
        }

        public ObservableCollection<Post> Posts { get; set; }

        public NewHomeScreenPreLogin()
        {
            InitializeComponent();

            ClientConnection();

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

            parentWindow.Close();
        }
    }
}
