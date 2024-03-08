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
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Win32;   //This namesapce is added for file browsing purposes in windows (WPF is primarily designed for windows)

namespace WPF_Front_End.View.UserControls
{
    /// <summary>hnology primarily designed for building Windows desktop applications. It's tightly integrated with the Windows operat
    /// Interaction logic for SignUpControl.xaml
    /// </summary>
    public partial class SignUpControl : UserControl
    {
        private NewPostHamburger hamburgerMenu;


        public SignUpControl()
        {
            InitializeComponent();
            // Subscribe to the Click event of the RemoveButton
            RemoveButton.Click += RemoveButton_Click;
            // Initially hide the RemoveButton
            RemoveButton.Visibility = Visibility.Collapsed;
            RemoveText1.Visibility = Visibility.Visible; 
            RemoveText2.Visibility = Visibility.Visible;
            
            Cursor = Cursors.Arrow;

            AddUserImage.MouseEnter += AddUserImage_MouseEnter;
            AddUserImage.MouseLeave += AddUserImage_MouseLeave;

        }

        private void LogIn_Click(object sender, RoutedEventArgs e)
        {
            Window parentWindow = Window.GetWindow(this);

            double windowWidth = parentWindow.ActualWidth;
            double windowHeight = parentWindow.ActualHeight;

            MainWindow mainWindow = new MainWindow();

            mainWindow.Width = windowWidth;
            mainWindow.Height = windowHeight;

            mainWindow.Show();

            parentWindow.Close();
        }



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
                AddUserImage.Visibility = Visibility.Collapsed;
                RemoveText1.Visibility = Visibility.Collapsed;
                RemoveText2.Visibility = Visibility.Collapsed;


                //the following commented code was to display the uplaoded profile image in all the windows instead of the profile icon
                //globalVariables.profileImageDropped = DropAreaImage.Source;

                // converting the image to byte array
                byte[] byteArray = getJPGFromImageControl(bitmap);


            }
        }


        // Event handler for the Click event of the RemoveButton
        private void RemoveButton_Click(object sender, RoutedEventArgs e)
        {
            DropAreaImage.Source = null;
            SelectedImage.Source = null;
            RemoveButton.Visibility = Visibility.Collapsed;
            AddUserImage.Visibility = Visibility.Visible;
            RemoveText1.Visibility = Visibility.Visible;
            RemoveText2.Visibility = Visibility.Visible;
        }


        // A function that enables file browsing for adding a profile image
        private void AddUserImage_MouseDown(object sender, MouseButtonEventArgs m)
        {
            OpenFileDialog dialog = new OpenFileDialog();

            // Set the filter for default file extension 
            dialog.Filter = "Image files (*png; *jpeg; *jpg) |*.png; *.jpeg; *.jpg |All files (*.*) | *.*"; 

            if(dialog.ShowDialog() == true) 
            {
                string selectedFileName = dialog.FileName;
                BitmapImage bitmap = new BitmapImage(new Uri(selectedFileName));
                // Show the selected image
                SelectedImage.Source = bitmap;
                SelectedImage.Visibility = Visibility.Visible;
                AddUserImage.Visibility = Visibility.Collapsed;

                RemoveButton.Visibility = Visibility.Visible;
                RemoveText1.Visibility = Visibility.Collapsed;
                RemoveText2.Visibility = Visibility.Collapsed;


                //the following commented code was to display the uplaoded profile image in all the windows instead of the profile icon
                //globalVariables.profileImageSelected = SelectedImage.Source;

                // converting the image to byte array
                byte[] byteArray = getJPGFromImageControl(bitmap);
            }


        }


        // The following two functions will change the shape of the cursor when the cursor enter the area that clicking on it will enable folder browsing
        private void AddUserImage_MouseEnter(object sender, MouseEventArgs e)
        {
            // Change the shape of the cursor to indicate browsing is enabled
            Cursor = Cursors.Hand;
        }

        private void AddUserImage_MouseLeave(object sender, MouseEventArgs e)
        {
            // Revert the shape of the cursor to default when mouse leaves the rectangle
            Cursor = Cursors.Arrow;
        }






        private void SignUp_Click(object sender, RoutedEventArgs e)
        {
            globalVariables.username = username.txtInput.Text;

            Window parentWindow = Window.GetWindow(this);

            double windowWidth = parentWindow.ActualWidth;
            double windowHeight = parentWindow.ActualHeight;

            NewHomeScreenPostLogin hspl = new NewHomeScreenPostLogin();

            hspl.Width = windowWidth;
            hspl.Height = windowHeight;

          

            hspl.Show();

           

            parentWindow.Close();


        }

        // source code from: Jonathan Escobedo 
        // changed JpegBitmapEncoder to PngBitmapEncoder so it will be compatible with all image file types
        // https://stackoverflow.com/questions/553611/wpf-image-to-byte
        private byte[] getJPGFromImageControl(BitmapImage imageC)
        {
            MemoryStream memStream = new MemoryStream();
            PngBitmapEncoder encoder = new PngBitmapEncoder();
            encoder.Frames.Add(BitmapFrame.Create(imageC));
            encoder.Save(memStream);
            return memStream.ToArray();
        }



    }
}
