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


        public string Title { get; set; }
        public string EstimatedWorth { get; set; }
        public string Location { get; set; }
        public string Condition { get; set; }
        public string Delivery { get; set; }
        public string LookingFor { get; set; }


        public Post()
        {
            InitializeComponent();

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
    }
}
