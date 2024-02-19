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
    /// Interaction logic for MyPostsScreen.xaml
    /// </summary>
    public partial class MyPostsScreen : Window
    {


        public ObservableCollection<MyPostsItem> Posts { get; set; }


        public MyPostsScreen()
        {
            InitializeComponent();

            Posts = new ObservableCollection<MyPostsItem>
            {

            new MyPostsItem
                    {
                        Image = "/Images/Image1.jpg",
                        Title = "Item 1",
                        EstimatedWorth = "$100",
                        Location = "City A",
                        ItemCondition = "Good",
                        Delivery = "Local",
                        LookingFor = "Item X"


                    },
                new MyPostsItem
                    {
                        Image = "/Images/Image1.jpg",
                        Title = "Item 3",
                        EstimatedWorth = "$150",
                        Location = "City C",
                        ItemCondition = "Excellent",
                        Delivery = "Nationwide",
                        LookingFor = "Item Z"

                    }
            };

            // Set the Items as the DataContext for the ListBox
            listBox.DataContext = Posts;
        }
        private void Button_Click_3(object sender, RoutedEventArgs e)
        {

        }

        private void Button_Click_7(object sender, RoutedEventArgs e)
        {
            double windowWidth = this.ActualWidth;
            double windowHeight = this.ActualHeight;

            var newForm = new HomeScreenPostLogIn(); //create your new form.

            newForm.Width = windowWidth;
            newForm.Height = windowHeight;

            newForm.Show(); //show the new form.
            this.Close(); //only if you want to close the current form.
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

        private void Button_Click_4(object sender, RoutedEventArgs e)
        {

        }

        private void Button_Click_5(object sender, RoutedEventArgs e)
        {

        }

        private void login_Click(object sender, RoutedEventArgs e)
        {
            double windowWidth = this.ActualWidth;
            double windowHeight = this.ActualHeight;

            var newForm = new HomeScreenPreLogIn(); //create your new form.

            newForm.Width = windowWidth;
            newForm.Height = windowHeight;

            newForm.Show(); //show the new form.
            this.Close(); //only if you want to close the current form.
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {

        }

        private void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        
    }
}
