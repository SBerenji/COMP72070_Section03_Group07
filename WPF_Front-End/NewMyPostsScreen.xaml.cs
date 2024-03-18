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
    /// Interaction logic for NewMyPostsScreen.xaml
    /// </summary>
    public partial class NewMyPostsScreen : Window
    {
        public ObservableCollection<MyPostsItem> Posts { get; set; }

        //private void SendRequestToGetCountOfPosts()
        //{

        //}


        public NewMyPostsScreen()
        {
            InitializeComponent();

            if (globalVariables.imageUploaded)
            {
                Hamburger.profile_icon.Visibility = Visibility.Collapsed;

                Hamburger.ProfileImage.Visibility = Visibility.Visible;

                Hamburger.ProfileImage.Source = ImageConversion.ToImage(globalVariables.receivedPostLoginImage);
            }





            Closing += CloseClient.Client_Closing;

            Hamburger.MyPosts.Background = Brushes.LightGray;

            Posts = new ObservableCollection<MyPostsItem>
            {

            new MyPostsItem
                    {
                        Image = "/Images/Image1.jpg",
                        Title = "Item 1",
                        EstimatedWorth = "$100",
                        Location = "City A",
                        Condition = "Good",
                        Delivery = "Local",
                        LookingFor = "Item X"


                    },
                new MyPostsItem
                    {
                        Image = "/Images/tv.jpg",
                        Title = "Item 2",
                        EstimatedWorth = "$50",
                        Location = "City B",
                        Condition = "Excellent",
                        Delivery = "Nationwide",
                        LookingFor = "Item Y"

                    },

                new MyPostsItem
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
