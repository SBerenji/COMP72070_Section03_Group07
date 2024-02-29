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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WPF_Front_End.View.UserControls
{
    /// <summary>
    /// Interaction logic for NewPostHamburger.xaml
    /// </summary>
    public partial class NewPostHamburger : UserControl
    {
        public NewPostHamburger()
        {
            InitializeComponent();

            if (globalVariables.username != "")
            {
                Profile.Text = globalVariables.username;
            }
            
        }

        private void LogOutClick(object sender, RoutedEventArgs e)
        {
            double desiredWidth = 400;

            double desiredHeight = 200;

            Window parentWindow = Window.GetWindow(this);

            double windowWidth = parentWindow.ActualWidth;
            double windowHeight = parentWindow.ActualHeight;


            double left = parentWindow.Left + (windowWidth - desiredWidth) / 2;
            double top = parentWindow.Top + (windowHeight - desiredHeight) / 2;


            // creating an instance of the new pop-up window

            Window newForm = null;

            if ((parentWindow.GetType()).FullName == "WPF_Front_End.NewHomeScreenPostLogin")
            {
                newForm = new LogOutPopUp((NewHomeScreenPostLogin)parentWindow);
            }

            else if ((parentWindow.GetType()).FullName == "WPF_Front_End.CreatePost")
            {
                newForm = new LogOutPopUp((CreatePost)parentWindow);
            }

            else if ((parentWindow.GetType()).FullName == "WPF_Front_End.Message")
            {
                newForm = new LogOutPopUp((Message)parentWindow);
            }

            else if ((parentWindow.GetType()).FullName == "WPF_Front_End.NewMyPostsScreen")
            {
                newForm = new LogOutPopUp((NewMyPostsScreen)parentWindow);
            }

            // setting the margin of the new window
            newForm.Left = left;
            newForm.Top = top;
            newForm.Width = desiredWidth;
            newForm.Height = desiredHeight;

            // show the new window (pop-up)
            newForm.Show();
        }

        private void CreatePostClick(object sender, RoutedEventArgs e)
        {
            Window parentWindow = Window.GetWindow(this);

            if ((parentWindow.GetType()).FullName != "WPF_Front_End.NewHomeScreenPreLogin")
            {
                double windowWidth = parentWindow.ActualWidth;
                double windowHeight = parentWindow.ActualHeight;

                CreatePost cp = new CreatePost();

                cp.Width = windowWidth;
                cp.Height = windowHeight;

                cp.Show();

                parentWindow.Close();
            }
        }

        private void MessageClick(object sender, RoutedEventArgs e)
        {
            Window parentWindow = Window.GetWindow(this);

            if ((parentWindow.GetType()).FullName != "WPF_Front_End.NewHomeScreenPreLogin")
            {
                double windowWidth = parentWindow.ActualWidth;
                double windowHeight = parentWindow.ActualHeight;

                Message m = new Message();

                m.Width = windowWidth;
                m.Height = windowHeight;

                m.Show();

                parentWindow.Close();
            }
        }

        private void HelpClick(object sender, RoutedEventArgs e)
        {
            // Setting the width and height for the new window
            double desiredWidth = 800;

            double desiredHeight = 550;

            Window parentWindow = Window.GetWindow(this);

            double windowWidth = parentWindow.ActualWidth;
            double windowHeight = parentWindow.ActualHeight;

            // calculations to determine the margin
            //double windowWidth = this.ActualWidth;
            //double windowHeight = this.ActualHeight;
            double left = parentWindow.Left + (windowWidth - desiredWidth) / 2;
            double top = parentWindow.Top + (windowHeight - desiredHeight) / 2;


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

        private void ListingsClick(object sender, RoutedEventArgs e)
        {
            Window parentWindow = Window.GetWindow(this);

            if (((parentWindow.GetType()).FullName != "WPF_Front_End.NewHomeScreenPreLogin") && ((parentWindow.GetType()).FullName != "WPF_Front_End.NewHomeScreenPostLogin"))
            {
                double windowWidth = parentWindow.ActualWidth;
                double windowHeight = parentWindow.ActualHeight;

                NewHomeScreenPostLogin nhspl = new NewHomeScreenPostLogin();

                nhspl.Width = windowWidth;
                nhspl.Height = windowHeight;

                nhspl.Show();

                parentWindow.Close();
            }
        }

        private void SavedPostsClick(object sender, RoutedEventArgs e)
        {
            Window parentWindow = Window.GetWindow(this);

            if ((parentWindow.GetType()).FullName != "WPF_Front_End.NewMyPostsScreen" && ((parentWindow.GetType()).FullName != "WPF_Front_End.NewHomeScreenPreLogin"))
            {
                double windowWidth = parentWindow.ActualWidth;
                double windowHeight = parentWindow.ActualHeight;

                NewMyPostsScreen nmps = new NewMyPostsScreen();

                nmps.Width = windowWidth;
                nmps.Height = windowHeight;

                nmps.Show();

                parentWindow.Close();
            }
        }
    }
}
