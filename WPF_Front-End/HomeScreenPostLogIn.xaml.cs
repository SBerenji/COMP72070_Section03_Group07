using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
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
    /// Interaction logic for HomeScreenPostLogIn.xaml
    /// </summary>
    public partial class HomeScreenPostLogIn : Window
    {
        public ObservableCollection<Post> Posts { get; set; }


        public HomeScreenPostLogIn()
        {
            InitializeComponent();

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
                        LookingFor = "Item X"


                    },
                new Post
                    {
                        Image = "/Images/tv.jpg",
                        Title = "Item 2",
                        EstimatedWorth = "$50",
                        Location = "City B",
                        Condition = "Excellent",
                        Delivery = "Nationwide",
                        LookingFor = "Item Y"

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

        private void Button_Click_7(object sender, RoutedEventArgs e)
        {

        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {

        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {

        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {

        }

        private void Button_Click_6(object sender, RoutedEventArgs e)
        {
            double windowWidth = this.ActualWidth;
            double windowHeight = this.ActualHeight;

            var newForm = new NewPost(); //create your new form.

            newForm.Width = windowWidth;
            newForm.Height = windowHeight;

            newForm.Show(); //show the new form.
            this.Close(); //only if you want to close the current form.
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            double windowWidth=this.ActualWidth;
            double windowHeight=this.ActualHeight;

            var newForm = new Message();

            newForm.Width = windowWidth;
            newForm.Height = windowHeight;

            newForm.Show();
            this.Close();
        }

        private void Button_Click_4(object sender, RoutedEventArgs e)
        {
            double windowWidth = this.ActualWidth;
            double windowHeight = this.ActualHeight;

            var newForm = new MyPostsScreen(); //create your new form.

            newForm.Width = windowWidth;
            newForm.Height = windowHeight;

            newForm.Show(); //show the new form.
            this.Close(); //only if you want to close the current form.
        }

        private void Button_Click_5(object sender, RoutedEventArgs e)
        {
            // Setting the width and height for the new window
            double desiredWidth = 800;

            double desiredHeight = 550;

            // calculations to determine the margin
            double windowWidth = this.ActualWidth;
            double windowHeight = this.ActualHeight;
            double left = this.Left + (windowWidth - desiredWidth) / 2;
            double top = this.Top + (windowHeight - desiredHeight) / 2;


            // creating an instance of the new pop-up window
            var newForm = new HelpPopUp();

            // setting the margin of the new window
            newForm.Left = left;
            newForm.Top = top;
            newForm.Width = desiredWidth;
            newForm.Height = desiredHeight;

            // show the new window (pop-up)
            newForm.Show();
        }

        private void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void login_Click(object sender, RoutedEventArgs e)
        {

            // Setting the width and height for the new window
            double desiredWidth = 400; 

            double desiredHeight = 200; 

            // calculations to determine the margin
            double windowWidth = this.ActualWidth;
            double windowHeight = this.ActualHeight;
            double left = this.Left + (windowWidth - desiredWidth) / 2; 
            double top = this.Top + (windowHeight - desiredHeight) / 2;

         
            // creating an instance of the new pop-up window
            var newForm = new LogOutPopUp(this);

            // setting the margin of the new window
            newForm.Left = left;
            newForm.Top = top;
            newForm.Width = desiredWidth;
            newForm.Height = desiredHeight;

            // show the new window (pop-up)
            newForm.Show();
        }

        private void Run_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Window parentWindow = Window.GetWindow(this);

            double windowWidth = parentWindow.ActualWidth;
            double windowHeight = parentWindow.ActualHeight;

            Chat c = new Chat();

            c.Width = windowWidth;
            c.Height = windowHeight;

            c.Show();

            parentWindow.Close();
        }
    }
}
