using System;
using System.Collections.Generic;
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
using System.Windows.Navigation;
using System.Windows.Shapes;
using WPF_Front_End;

namespace WPF_Front_End
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class NewPost : Window
    {
        public NewPost()
        {
            InitializeComponent();

        }

        private void Triangle_Loaded(object sender, RoutedEventArgs e)
        {

        }

        private void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            var newHome = new HomeScreenPostLogIn(); //create your new form.
            newHome.Show(); //show the new form.
            this.Close(); //only if you want to close the current form.
        }

        private void TextBox_TextChanged_1(object sender, TextChangedEventArgs e)
        {

        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {

        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {

        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {

        }

        private void Button_Click_4(object sender, RoutedEventArgs e)
        {

        }

        private void Button_Click_5(object sender, RoutedEventArgs e)
        {

        }

        private void Button_Click_6(object sender, RoutedEventArgs e)
        {

        }

        private void Button_Click_7(object sender, RoutedEventArgs e)
        {
            var newHome = new HomeScreenPostLogIn(); //create your new form.
            newHome.Show(); //show the new form.
            this.Close(); //only if you want to close the current form.
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

        private void Button_Click_8(object sender, RoutedEventArgs e)
        {
            double windowWidth = this.ActualWidth;
            double windowHeight = this.ActualHeight;

            var newForm = new MyPostsScreen(); //create your new form.

            newForm.Width = windowWidth;
            newForm.Height = windowHeight;

            newForm.Show(); //show the new form.
            this.Close(); //only if you want to close the current form.
        }


        private void Help_button(object sender, RoutedEventArgs e)
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





    }
}
