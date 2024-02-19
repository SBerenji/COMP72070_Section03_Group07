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
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Window parentWindow = Window.GetWindow(this);

            double windowWidth = parentWindow.ActualWidth;
            double windowHeight = parentWindow.ActualHeight;

           MainWindow main = new MainWindow();

            main.Width = windowWidth;
            main.Height = windowHeight;

            main.Show();

            parentWindow.Close(); 
        }

        private void TextBlock_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Window parentWindow = Window.GetWindow(this);

            double windowWidth = parentWindow.ActualWidth;
            double windowHeight = parentWindow.ActualHeight;

            NewPost np = new NewPost();

            np.Width = windowWidth;
            np.Height = windowHeight;

            np.Show();

            parentWindow.Close();
        }
    }
}
