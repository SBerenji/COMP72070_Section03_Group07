using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
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
    /// Interaction logic for NewPostSudhan.xaml
    /// </summary>
    public partial class CreatePost : Window
    {
        public CreatePost()
        {
            InitializeComponent();

            if (globalVariables.imageUploaded)
            {
                Hamburger.profile_icon.Visibility = Visibility.Collapsed;

                Hamburger.ProfileImage.Visibility = Visibility.Visible;

                Hamburger.ProfileImage.Source = ImageConversion.ToImage(globalVariables.receivedPostLoginImage);
            }

            Closing += CloseClient.Client_Closing;

            //Hamburger.Profile.Text = globalVariables.username;

            Hamburger.CreatePost.Background = Brushes.LightGray;

            Cursor = Cursors.Arrow;

            AddImage.MouseEnter += AddImage_MouseEnter;
            AddImage.MouseLeave += AddImage_MouseLeave;
        }



        //public string Placeholder
        //{
        //    get { return Placeholder; }
        //    set
        //    {
        //        Placeholder = value;

        //        tb_title_Placeholder.Text = Placeholder;
        //    }
        //}

        private void txtInput_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (string.IsNullOrEmpty(title.Text))
            {
                tb_title_Placeholder.Visibility = Visibility.Visible;
            }

            else
            {
                tb_title_Placeholder.Visibility = Visibility.Hidden;
            }
        }



        private void location_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (string.IsNullOrEmpty(location.Text))
            {
                tb_location_Placeholder.Visibility = Visibility.Visible;
            }

            else
            {
                tb_location_Placeholder.Visibility = Visibility.Hidden;
            }
        }



        private void condition_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (string.IsNullOrEmpty(condition.Text))
            {
                tb_condition_Placeholder.Visibility = Visibility.Visible;
            }

            else
            {
                tb_condition_Placeholder.Visibility = Visibility.Hidden;
            }
        }




        private void worth_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (string.IsNullOrEmpty(worth.Text))
            {
                tb_worth_Placeholder.Visibility = Visibility.Visible;
            }

            else
            {
                tb_worth_Placeholder.Visibility = Visibility.Hidden;
            }
        }



        private void delivery_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (string.IsNullOrEmpty(delivery.Text))
            {
                tb_delivery_Placeholder.Visibility = Visibility.Visible;
            }

            else
            {
                tb_delivery_Placeholder.Visibility = Visibility.Hidden;
            }
        }




        private void LookingFor_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (string.IsNullOrEmpty(LookingFor.Text))
            {
                tb_LookingFor_Placeholder.Visibility = Visibility.Visible;
            }

            else
            {
                tb_LookingFor_Placeholder.Visibility = Visibility.Hidden;
            }
        }




        private void btnClear_Click(object sender, RoutedEventArgs e)
        {
            title.Clear();
            title.Focus();

            location.Clear();
            location.Focus();

            condition.Clear();
            condition.Focus();

            worth.Clear();
            worth.Focus();

            delivery.Clear();
            delivery.Focus();

            LookingFor.Clear();
            LookingFor.Focus();
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

                string extension = System.IO.Path.GetExtension(imagePath);
                if (extension != ".jpeg")
                {
                    MessageBox.Show("Only JPEG files are allowed.", "Invalid File", MessageBoxButton.OK, MessageBoxImage.Error);
                    return; // Stop processing further
                }
                BitmapImage bitmap = new BitmapImage(new Uri(imagePath));
                DropAreaImage2.Source = bitmap;
                DropAreaImage2.Visibility = Visibility.Visible;

                // once the image is dropped, show the remove button and replace it with the text
                RemoveButton.Visibility = Visibility.Visible;
                RemoveText1.Visibility = Visibility.Collapsed;
                AddImage.Visibility = Visibility.Collapsed;


                // converting the image to byte array
                globalVariables.createPostImage = getJPEGFromImageControl(bitmap).ToArray();
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

            globalVariables.createPostImage = Array.Empty<byte>();
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
                globalVariables.createPostImage = getJPEGFromImageControl(bitmap);
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
        // https://stackoverflow.com/questions/553611/wpf-image-to-byte
        private byte[] getJPEGFromImageControl(BitmapImage imageC)
        {
            MemoryStream memStream = new MemoryStream();
            JpegBitmapEncoder encoder = new JpegBitmapEncoder();
            encoder.Frames.Add(BitmapFrame.Create(imageC));
            encoder.Save(memStream);
            return memStream.ToArray();
        }


        private void NewPost_Click(object sender, RoutedEventArgs e)
        {
            if (title.Text == "" || location.Text == "" || condition.Text == "" || worth.Text == "" || delivery.Text == "" || LookingFor.Text == "")
            {
                MessageBox.Show("Please Fill all the details!!");
            }

            else
            {
                Listing list = new Listing();

                list.Title = new byte[ConstantVariables.listing_ByteArraySize];
                list.Title = Encoding.ASCII.GetBytes(title.Text);

                list.Location = new byte[ConstantVariables.listing_ByteArraySize];
                list.Location = Encoding.ASCII.GetBytes(location.Text);

                list.Condition = new byte[ConstantVariables.listing_ByteArraySize];
                list.Condition = Encoding.ASCII.GetBytes(condition.Text);

                list.EstimatedWorth = new byte[ConstantVariables.listing_ByteArraySize];
                list.EstimatedWorth = Encoding.ASCII.GetBytes(worth.Text);

                list.Delivery = new byte[ConstantVariables.listing_ByteArraySize];
                list.Delivery = Encoding.ASCII.GetBytes(delivery.Text);

                list.LookingFor = new byte[ConstantVariables.listing_ByteArraySize];
                list.LookingFor = Encoding.ASCII.GetBytes(LookingFor.Text);



                int imageSize = globalVariables.createPostImage.Length;

                list.ImageStructArray = Packet.AllocateHeapMemory(imageSize);

                Packet.CopyBufferToHeap(list.ImageStructArray, globalVariables.createPostImage, imageSize);

                IntPtr BodyBuffer = Packet.AllocateListingPtr(imageSize);

                Packet.SerializeListingInformation(ref BodyBuffer, list, imageSize);


                IntPtr PktPtr = Packet.CreatePacket();

                IntPtr Head = Packet.AllocateHeaderPtr();


                Packet.SetHeaderInformation(ref Head, "127.0.0.1", "127.0.0.1", Route.POST, true);

                Packet.SetHeader(PktPtr, Head);

                int size = (int)((6 * ConstantVariables.listing_ByteArraySize) * sizeof(byte)) + imageSize;


                Packet.SetBody(PktPtr, globalVariables.ClientID, BodyBuffer, size);


                IntPtr serializedRecv = Packet.SerializeData(PktPtr, out Packet.totalPktSize);


                Packet.TxBuffer = new byte[Packet.totalPktSize];


                Marshal.Copy(serializedRecv, Packet.TxBuffer, 0, Packet.totalPktSize);


                Packet.sendData(MySocket.ClientSocket, Packet.TxBuffer, Packet.totalPktSize);



                Packet.TxBuffer = null;

                Packet.FreeBuffer(ref serializedRecv);
                serializedRecv = IntPtr.Zero;

                Packet.FreeBuffer(ref Head);
                Head = IntPtr.Zero;

                Packet.DestroyPacket(ref PktPtr);
                PktPtr = IntPtr.Zero;


                Packet.FreeBuffer(ref BodyBuffer);
                BodyBuffer = IntPtr.Zero;

                Packet.FreeBuffer(ref list.ImageStructArray);
                list.ImageStructArray = IntPtr.Zero;


                MessageBox.Show("Item successfully Posted!! You can now view your post from 'My Posts' tab.");
                
            }
        }
    }
}
