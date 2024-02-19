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
    /// Interaction logic for MyPostsItem.xaml
    /// </summary>
    public partial class MyPostsItem : UserControl
    {

        public string Image { get; set; }
        public string Title { get; set; }
        public string EstimatedWorth { get; set; }
        public string Location { get; set; }
        public string ItemCondition { get; set; }
        public string Delivery { get; set; }
        public string LookingFor { get; set; }

      
     


  

        public MyPostsItem()
        {
            InitializeComponent();

            Image = string.Empty;
            Title = string.Empty;
            EstimatedWorth = string.Empty;
            Location = string.Empty;
            ItemCondition = string.Empty;
            Delivery = string.Empty;
            LookingFor = string.Empty;

        }

        private void DeleteButton_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
