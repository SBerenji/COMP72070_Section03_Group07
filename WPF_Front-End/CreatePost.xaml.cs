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
    /// Interaction logic for NewPostSudhan.xaml
    /// </summary>
    public partial class CreatePost : Window
    {
        public CreatePost()
        {
            InitializeComponent();

            //Hamburger.Profile.Text = globalVariables.username;

            Hamburger.CreatePost.Background = Brushes.LightGray;
        }

        private void hamburger_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Hamburger Worked!!");
        }

        private void dropdown_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("dropdown Worked!!");
        }


        // functions for the drag and drop feature of the sign up form:


        // an event handler for the DragEnter event
        // checks to see if what the user is dropping is in correct format or not as soon as it enters the drop area (is it a file)
        // checks when something enters the drop area
        private void DropArea_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                e.Effects = DragDropEffects.Copy;
            else
            {
                e.Effects = DragDropEffects.None;
                MessageBox.Show("The file is acceptable");
            }

        }

        // an event handler for the DragOver event 
        // checks to see if what the user has dropped is in correct format or not (is it a file)
        // ongoing check when something (the cursor) is in the drop area

        private void DropArea_DragOver(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                e.Effects = DragDropEffects.Copy;
            else
                e.Effects = DragDropEffects.None;

        }


        private void DropArea_Drop(object sender, DragEventArgs e)
        {
            string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);
            if (files != null && files.Length > 0)
            {
                string imagePath = files[0];


                BitmapImage bitmap = new BitmapImage(new Uri(imagePath));
                DropAreaImage.Source = bitmap;

                // once the image is dropped, show the remove button and replace it with the text
                RemoveButton.Visibility = Visibility.Visible;
                RemoveText1.Visibility = Visibility.Collapsed;


            }
        }


        // Event handler for the Click event of the RemoveButton
        private void RemoveButton_Click(object sender, RoutedEventArgs e)
        {
            DropAreaImage.Source = null;
            RemoveButton.Visibility = Visibility.Collapsed;
            RemoveText1.Visibility = Visibility.Visible;
        }
    }
}
