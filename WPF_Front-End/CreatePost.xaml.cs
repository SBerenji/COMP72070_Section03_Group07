using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
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

            Closing += CloseClient.Client_Closing;

            //Hamburger.Profile.Text = globalVariables.username;

            Hamburger.CreatePost.Background = Brushes.LightGray;

            Cursor = Cursors.Arrow;

            AddImage.MouseEnter += AddImage_MouseEnter;
            AddImage.MouseLeave += AddImage_MouseLeave;
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
                DropAreaImage2.Source = bitmap;

                // once the image is dropped, show the remove button and replace it with the text
                RemoveButton.Visibility = Visibility.Visible;
                RemoveText1.Visibility = Visibility.Collapsed;
                AddImage.Visibility = Visibility.Collapsed;


                // converting the image to byte array
                byte[] byteArray = getJPEGFromImageControl(bitmap);

            }
        }


        // Event handler for the Click event of the RemoveButton
        private void RemoveButton_Click(object sender, RoutedEventArgs e)
        {
            DropAreaImage2.Source = null;
            SelectedImage2.Source = null;

            RemoveButton.Visibility = Visibility.Collapsed;
            RemoveText1.Visibility = Visibility.Visible;
            AddImage.Visibility = Visibility.Visible;
        }


        // A function that enables file browsing for adding a profile image
        private void AddImage_MouseDown(object sender, MouseButtonEventArgs m)
        {
            OpenFileDialog dialog = new OpenFileDialog();

            // Set the filter for default file extension 
            dialog.Filter = "Image files (*jpeg) |*.jpeg";

            if (dialog.ShowDialog() == true)
            {
                string selectedFileName = dialog.FileName;
                BitmapImage bitmap = new BitmapImage(new Uri(selectedFileName));
                // Show the selected image
                SelectedImage2.Source = bitmap;
                SelectedImage2.Visibility = Visibility.Visible;

                RemoveButton.Visibility = Visibility.Visible;
                RemoveText1.Visibility = Visibility.Collapsed;
                AddImage.Visibility = Visibility.Collapsed;

                // converting the image to byte array
                byte[] byteArray = getJPEGFromImageControl(bitmap);
            }

        }


        // The following two functions will change the shape of the cursor when the cursor enter the area that clicking on it will enable folder browsing
        private void AddImage_MouseEnter(object sender, MouseEventArgs e)
        {
            // Change the shape of the cursor to indicate browsing is enabled
            Cursor = Cursors.Hand;
        }

        private void AddImage_MouseLeave(object sender, MouseEventArgs e)
        {
            // Revert the shape of the cursor to default when mouse leaves the rectangle
            Cursor = Cursors.Arrow;
        }


        // source code from: Jonathan Escobedo 
        // changed JpegBitmapEncoder to PngBitmapEncoder so it will be compatible with all image file types
        // https://stackoverflow.com/questions/553611/wpf-image-to-byte
        private byte[] getJPEGFromImageControl(BitmapImage imageC)
        {
            MemoryStream memStream = new MemoryStream();
            JpegBitmapEncoder encoder = new JpegBitmapEncoder();
            encoder.Frames.Add(BitmapFrame.Create(imageC));
            encoder.Save(memStream);
            return memStream.ToArray();
        }


    }
}
