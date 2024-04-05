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

namespace WPF_Front_End
{
    /// <summary>
    /// Interaction logic for Post.xaml
    /// </summary>
    public partial class Post : UserControl
    {

        public BitmapImage Image { get; set; }
        public string Title { get; set; }
        public string EstimatedWorth { get; set; }
        public string Location { get; set; }
        public string Condition { get; set; }
        public string Delivery { get; set; }
        public string LookingFor { get; set; }


        public Post()
        {
            InitializeComponent();

            // Image = string.Empty;
            Title = string.Empty;
            EstimatedWorth = string.Empty;
            Location = string.Empty;
            Condition = string.Empty;
            Delivery = string.Empty;
            LookingFor = string.Empty;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {

        }

        private void RequestSwap(object sender, RoutedEventArgs e)
        {
            Window parentWindow = Window.GetWindow(this);

            if (parentWindow.GetType().FullName != "WPF_Front_End.NewHomeScreenPreLogin")
            {
                double windowWidth = parentWindow.ActualWidth;
                double windowHeight = parentWindow.ActualHeight;

                Message m = new Message();

                m.Width = windowWidth;
                m.Height = windowHeight;

                m.Show();

                parentWindow.Close();
            }

            else
            {
                BlockUnLoggedUser unauthorized = new BlockUnLoggedUser();

                unauthorized.ShowDialog();
            }
        }
    }
}
