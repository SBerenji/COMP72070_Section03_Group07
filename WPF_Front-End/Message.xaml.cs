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
using System.Windows.Shapes;

namespace WPF_Front_End
{
    /// <summary>
    /// Interaction logic for Message.xaml
    /// </summary>
    public partial class Message : Window
    {
        public Message()
        {
            InitializeComponent();

            Closing += CloseClient.Client_Closing;

            Hamburger.Messages.Background = Brushes.LightGray;
        }

        private void NewPostHamburger_Loaded(object sender, RoutedEventArgs e)
        {

        }

        private void PeopleAndMessage_Loaded(object sender, RoutedEventArgs e)
        {

        }
    }
}
