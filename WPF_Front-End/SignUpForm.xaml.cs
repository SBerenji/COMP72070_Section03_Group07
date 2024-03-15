using System;
using System.Collections.Generic;
using System.Linq;
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
using System.Windows.Shapes;

namespace WPF_Front_End
{
    /// <summary>
    /// Interaction logic for SignUpForm.xaml
    /// </summary>
    public partial class SignUpForm : Window
    {
        //[DllImport("TCP_Client.dll")]
        //public static extern int sendData(MySocket ClientSocket);

        public SignUpForm()
        {
            InitializeComponent();

            //sendData(MySocket.ClientSocket);
        }

        private void SignUpControl_Loaded(object sender, RoutedEventArgs e)
        {

        }
    }
}
