using System;
using System.Collections.Generic;
using System.ComponentModel;
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
    /// 

    //public partial class MyPostsItem : INotifyPropertyChanged
    //{
    //    public event PropertyChangedEventHandler PropertyChanged;

    //    private byte[] imageData;
    //    public byte[] ImageData
    //    {
    //        get { return imageData; }
    //        set
    //        {
    //            imageData = value;
    //            OnPropertyChanged(nameof(ImageData));
    //        }
    //    }

    //    // Other properties
    //    // ...

    //    protected virtual void OnPropertyChanged(string propertyName)
    //    {
    //        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    //    }
    //}
    public partial class MyPostsItem : UserControl
    {

        public BitmapImage Image { get; set; }
        public string Title { get; set; }
        public string EstimatedWorth { get; set; }
        public string Location { get; set; }
        public string Condition { get; set; }
        public string Delivery { get; set; }
        public string LookingFor { get; set; }


        public MyPostsItem()
        {
            InitializeComponent();

            //Image = string.Empty;
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
