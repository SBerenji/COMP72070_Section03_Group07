using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net.Http;
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


            globalVariables.imageUploaded = false;

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

                string extension = System.IO.Path.GetExtension(imagePath);
                if (extension != ".jpeg")
                {
                    MessageBox.Show("Only JPEG files are allowed.", "Invalid File", MessageBoxButton.OK, MessageBoxImage.Error);
                    return; // Stop processing further
                }

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
                globalVariables.ImageArray = getJPEGFromImageControl(bitmap).ToArray();

                globalVariables.imageUploaded = true;
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

            globalVariables.imageUploaded = false;

            globalVariables.ImageArray = Array.Empty<byte>();
        }


        // A function that enables file browsing for adding a profile image
        private void AddUserImage_MouseDown(object sender, MouseButtonEventArgs m)
        {
            OpenFileDialog dialog = new OpenFileDialog();

            // Set the filter for default file extension 
            dialog.Filter = "Image files (*jpeg) |*.jpeg"; 

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
                globalVariables.ImageArray = getJPEGFromImageControl(bitmap).ToArray();

                globalVariables.imageUploaded = true;
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



        private int userExistsCheck()
        {
            SignUpCheck check = new SignUpCheck();

            globalVariables.username = username.txtInput.Text;
            globalVariables.email = email.txtInput.Text;

            check.username = new byte[ConstantVariables.username_ByteArraySize];
            check.email = new byte[ConstantVariables.email_ByteArraySize];

            check.username = Encoding.ASCII.GetBytes(globalVariables.username);
            check.email = Encoding.ASCII.GetBytes(globalVariables.email);


            int DataSize = (int)(ConstantVariables.username_ByteArraySize + ConstantVariables.email_ByteArraySize);


            IntPtr PktPtr = Packet.CreatePacket();

            IntPtr Head = Packet.AllocateHeaderPtr();

            Packet.SetHeaderInformation(ref Head, "127.0.0.1", "127.0.0.1", Route.SIGNUP_USERCHECK, false);

            Packet.SetHeader(PktPtr, Head);


            IntPtr BodyBuffer = Packet.AllocateHeapMemory(DataSize);

            Packet.SerializeSignUpCheckInfo(ref BodyBuffer, check);


            Packet.SetBody(PktPtr, globalVariables.ClientID, BodyBuffer, DataSize);


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

            return 1;
        }


        private void SignUp_Click(object sender, RoutedEventArgs e)
        {
            if (username.txtInput.Text == "" || /*password.Password == "" ||*/ email.txtInput.Text == "")
            {
                MessageBox.Show("Please enter Username, Password AND Email Address!!");
            }

            else
            {
                int checkFuncReturn = userExistsCheck();

                Packet.RxBuffer = new byte[500];

                int recvSize = Packet.recvData(MySocket.ClientSocket, Packet.RxBuffer, 500);

                
                Header head = new Header();

                head.Source = new byte[ConstantVariables.Source_Destination_ByteArraySize];
                head.Destination = new byte[ConstantVariables.Source_Destination_ByteArraySize];
                head.Route = new byte[ConstantVariables.Route_ByteArraySize];


                Packet.DeserializeHeader(Packet.RxBuffer, ref head);

                if (head.Authorization)
                {
                    MessageBox.Show("User Already exits!! Please LogIn instead!!");
                }

                else
                {
                    SignUp signup = new SignUp();

                    globalVariables.username = username.txtInput.Text;
                    //globalVariables.password = password.Password;
                    globalVariables.email = email.txtInput.Text;

                    signup.username = new byte[ConstantVariables.username_ByteArraySize];
                    signup.password = new byte[ConstantVariables.password_ByteArraySize];
                    signup.email = new byte[ConstantVariables.email_ByteArraySize];


                    signup.username = Encoding.ASCII.GetBytes(globalVariables.username);
                    signup.password = Encoding.ASCII.GetBytes(globalVariables.password);
                    signup.email = Encoding.ASCII.GetBytes(globalVariables.email);


                    if (globalVariables.imageUploaded)
                    {
                        int imageSize = globalVariables.ImageArray.Length;

                        signup.ImageStructArray = Packet.AllocateHeapMemory(imageSize);

                        Packet.CopyBufferToHeap(signup.ImageStructArray, globalVariables.ImageArray, imageSize);


                        IntPtr BodyBuffer = Packet.AllocateSignupPtr(imageSize);

                        Packet.SerializeSignUpInformation(ref BodyBuffer, signup, imageSize);


                        IntPtr PktPtr = Packet.CreatePacket();

                        IntPtr Head = Packet.AllocateHeaderPtr();

                        //IntPtr Head = Marshal.AllocHGlobal(Marshal.SizeOf<Header>());

                        Packet.SetHeaderInformation(ref Head, "127.0.0.1", "127.0.0.1", Route.SIGNUP_IMAGEUPLOADED, true);

                        Packet.SetHeader(PktPtr, Head);


                        int size = (int)ConstantVariables.username_ByteArraySize + (int)ConstantVariables.password_ByteArraySize + (int)ConstantVariables.email_ByteArraySize + imageSize;


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

                        Packet.FreeBuffer(ref signup.ImageStructArray);
                        signup.ImageStructArray = IntPtr.Zero;
                    }

                    else
                    {
                        byte[] data = Encoding.ASCII.GetBytes("Image Not Uploaded");

                        int imageSize = data.Length;

                        signup.ImageStructArray = Packet.AllocateHeapMemory(imageSize);

                        Packet.CopyBufferToHeap(signup.ImageStructArray, data, imageSize);


                        IntPtr BodyBuffer = Packet.AllocateSignupPtr(imageSize);

                        Packet.SerializeSignUpInformation(ref BodyBuffer, signup, imageSize);


                        IntPtr PktPtr = Packet.CreatePacket();

                        IntPtr Head = Packet.AllocateHeaderPtr();

                        //IntPtr Head = Marshal.AllocHGlobal(Marshal.SizeOf<Header>());

                        Packet.SetHeaderInformation(ref Head, "127.0.0.1", "127.0.0.1", Route.SIGNUP_IMAGENOTUPLOADED, true);

                        Packet.SetHeader(PktPtr, Head);

                        int size = (int)ConstantVariables.username_ByteArraySize + (int)ConstantVariables.password_ByteArraySize + (int)ConstantVariables.email_ByteArraySize + imageSize;

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

                        Packet.FreeBuffer(ref signup.ImageStructArray);
                        signup.ImageStructArray = IntPtr.Zero;
                    }

                    globalVariables.OneClientFirstSignUp = true;


                    Window parentWindow = Window.GetWindow(this);

                    double windowWidth = parentWindow.ActualWidth;
                    double windowHeight = parentWindow.ActualHeight;

                    NewHomeScreenPostLogin hspl = new NewHomeScreenPostLogin();

                    hspl.Width = windowWidth;
                    hspl.Height = windowHeight;



                    hspl.Show();



                    parentWindow.Close();
                }
            }
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
