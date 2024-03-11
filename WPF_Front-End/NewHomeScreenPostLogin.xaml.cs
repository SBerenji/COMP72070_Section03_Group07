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
    /// Interaction logic for NewHomeScreenPostLogin.xaml
    /// </summary>
    public partial class NewHomeScreenPostLogin : Window
    {
        public ObservableCollection<Post> Posts { get; set; }

        public NewHomeScreenPostLogin()
        {
            InitializeComponent();

            Closing += CloseClient.Client_Closing;

            Hamburger.Listings.Background = Brushes.LightGray;

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
                        LookingFor = "Item X",

                    },
                new Post
                    {
                        Image = "/Images/tv.jpg",
                        Title = "Item 2",
                        EstimatedWorth = "$50",
                        Location = "City B",
                        Condition = "Excellent",
                        Delivery = "Nationwide",
                        LookingFor = "Item Y",
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
    }
}
