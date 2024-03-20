using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Automation;
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
    /// 






    public partial class NewMyPostsScreen : Window
    {
        //private void SendRequestToGetCountOfPosts()
        //{
        //    IntPtr PktPtr = Packet.CreatePacket();


        //    IntPtr Head = Packet.AllocateHeaderPtr();

        //    Packet.SetHeaderInformation(ref Head, "127.0.0.1", "127.0.0.1", Route.MYPOSTS_COUNT, true);

        //    uint length = 0;

        //    Marshal.StructureToPtr(length, Head + (int)(ConstantVariables.Source_Destination_ByteArraySize + ConstantVariables.Source_Destination_ByteArraySize + ConstantVariables.Route_ByteArraySize) * sizeof(byte) + sizeof(bool), false);


        //    Packet.SetHeader(PktPtr, Head);

        //    IntPtr serializedData = Packet.SerializeMyPostCountData(PktPtr, out Packet.totalPktSize);


        //    Packet.TxBuffer = new byte[Packet.totalPktSize];


        //    Marshal.Copy(serializedData, Packet.TxBuffer, 0, Packet.totalPktSize);


        //    Packet.sendData(MySocket.ClientSocket, Packet.TxBuffer, Packet.totalPktSize);
        //}


        //private int RecvAckOfNumberOfPosts()
        //{
        //    byte[] RxBuffer = new byte[500];

        //    Packet.recvData(MySocket.ClientSocket, RxBuffer, 500);

        //    IntPtr PktPtr = Packet.CreatePacket();

        //    int numberOfPosts;

        //    Packet.DeserializePostCountBuffer(PktPtr, RxBuffer, out numberOfPosts);

        //    Packet.DestroyPacket(ref PktPtr);
        //    PktPtr = IntPtr.Zero;

        //    RxBuffer = null;


        //    return numberOfPosts;
        //}


        //private void recvPosts(int numberOfPosts)
        //{
        //    while(numberOfPosts > 0)
        //    {
        //        byte[] RxBuffer = new byte[200000];

        //        Packet.recvData(MySocket.ClientSocket, RxBuffer, 200000);

        //        Listing list = new Listing();

        //        list.Title = new byte[ConstantVariables.listing_ByteArraySize];

        //        list.Location = new byte[ConstantVariables.listing_ByteArraySize];

        //        list.Condition = new byte[ConstantVariables.listing_ByteArraySize];

        //        list.EstimatedWorth = new byte[ConstantVariables.listing_ByteArraySize];

        //        list.Delivery = new byte[ConstantVariables.listing_ByteArraySize];

        //        list.LookingFor = new byte[ConstantVariables.listing_ByteArraySize];



        //        list.Title = new byte[200];
        //        Array.Copy(RxBuffer, 0, list.Title, 0, 200);

        //        list.Location = new byte[200];
        //        Array.Copy(RxBuffer, 200, list.Location, 0, 200);

        //        list.Condition = new byte[200];
        //        Array.Copy(RxBuffer, 400, list.Condition, 0, 200);

        //        list.EstimatedWorth = new byte[200];
        //        Array.Copy(RxBuffer, 600, list.EstimatedWorth, 0, 200);

        //        list.Delivery = new byte[200];
        //        Array.Copy(RxBuffer, 800, list.Delivery, 0, 200);

        //        list.LookingFor = new byte[200];
        //        Array.Copy(RxBuffer, 1000, list.LookingFor, 0, 200);


        //        uint imageSize = BitConverter.ToUInt32(RxBuffer, 1200);

        //        globalVariables.MyPostImage1 = new byte[imageSize];

        //        list.ImageStructArray = Packet.AllocateHeapMemory((int)imageSize);

        //        Marshal.Copy(RxBuffer, 1200 + sizeof(uint), list.ImageStructArray, (int)imageSize);

        //        //Marshal.Copy(list.ImageStructArray, globalVariables.MyPostImage1, 0, (int)imageSize);

        //        //Packet.FreeBuffer(ref list.ImageStructArray);
        //        //list.ImageStructArray = IntPtr.Zero;


        //        AddListing(ref list);

        //        AddImageSize(ref imageSize);

        //        numberOfPosts -= 1;
        //    }
        //}


        //void AddListing(ref Listing newListing)
        //{
        //    globalVariables.listings.Add(newListing);
        //}

        //void AddImageSize(ref uint imageSize)
        //{
        //    globalVariables.MyPostImageSize.Add(imageSize);
        //}


        //void ShowListing()
        //{
        //    for (int i = 0; i < globalVariables.listings.Count; i++)
        //    {
        //        byte[] imageArray = new byte[(int)globalVariables.MyPostImageSize[i]];

        //        Marshal.Copy(globalVariables.listings[i].ImageStructArray, imageArray, 0, (int)globalVariables.MyPostImageSize[i]);

        //        IntPtr imagePtr = globalVariables.listings[i].ImageStructArray;

        //        Packet.FreeBuffer(ref imagePtr);
        //        imagePtr = IntPtr.Zero;

        //        MyPostsItem mpi = new MyPostsItem
        //        {
        //            Image = ImageConversion.ToImage(imageArray),
        //            Title = System.Text.Encoding.Default.GetString(globalVariables.listings[i].Title),
        //            EstimatedWorth = System.Text.Encoding.Default.GetString(globalVariables.listings[i].EstimatedWorth),
        //            Location = System.Text.Encoding.Default.GetString(globalVariables.listings[i].Location),
        //            Condition = System.Text.Encoding.Default.GetString(globalVariables.listings[i].Condition),
        //            Delivery = System.Text.Encoding.Default.GetString(globalVariables.listings[i].Delivery),
        //            LookingFor = System.Text.Encoding.Default.GetString(globalVariables.listings[i].LookingFor)
        //        };


        //        globalVariables.Posts.Add( mpi );
        //    }
        //}



        private void SendRequestToGetCountOfPosts()
        {
            IntPtr PktPtr = Packet.CreatePacket();


            IntPtr Head = Packet.AllocateHeaderPtr();

            Packet.SetHeaderInformation(ref Head, "127.0.0.1", "127.0.0.1", Route.MYPOSTS_COUNT, true);

            uint length = 0;

            Marshal.StructureToPtr(length, Head + (int)(ConstantVariables.Source_Destination_ByteArraySize + ConstantVariables.Source_Destination_ByteArraySize + ConstantVariables.Route_ByteArraySize) * sizeof(byte) + sizeof(bool), false);


            Packet.SetHeader(PktPtr, Head);

            IntPtr serializedData = Packet.SerializeMyPostCountData(PktPtr, out Packet.totalPktSize);


            Packet.TxBuffer = new byte[Packet.totalPktSize];


            Marshal.Copy(serializedData, Packet.TxBuffer, 0, Packet.totalPktSize);


            Packet.sendData(MySocket.ClientSocket, Packet.TxBuffer, Packet.totalPktSize);
        }


        private int RecvAckOfNumberOfPosts()
        {
            byte[] RxBuffer = new byte[500];

            Packet.recvData(MySocket.ClientSocket, RxBuffer, 500);

            IntPtr PktPtr = Packet.CreatePacket();

            int numberOfPosts;

            Packet.DeserializePostCountBuffer(PktPtr, RxBuffer, out numberOfPosts);

            Packet.DestroyPacket(ref PktPtr);
            PktPtr = IntPtr.Zero;

            RxBuffer = null;


            return numberOfPosts;
        }


        private void recvPosts(int numberOfPosts)
        {
            while (numberOfPosts > 0)
            {
                byte[] RxBuffer = new byte[200000];

                Packet.recvData(MySocket.ClientSocket, RxBuffer, 200000);

                Listing list = new Listing();

                list.Title = new byte[ConstantVariables.listing_ByteArraySize];

                list.Location = new byte[ConstantVariables.listing_ByteArraySize];

                list.Condition = new byte[ConstantVariables.listing_ByteArraySize];

                list.EstimatedWorth = new byte[ConstantVariables.listing_ByteArraySize];

                list.Delivery = new byte[ConstantVariables.listing_ByteArraySize];

                list.LookingFor = new byte[ConstantVariables.listing_ByteArraySize];



                list.Title = new byte[200];
                Array.Copy(RxBuffer, 0, list.Title, 0, 200);

                list.Location = new byte[200];
                Array.Copy(RxBuffer, 200, list.Location, 0, 200);

                list.Condition = new byte[200];
                Array.Copy(RxBuffer, 400, list.Condition, 0, 200);

                list.EstimatedWorth = new byte[200];
                Array.Copy(RxBuffer, 600, list.EstimatedWorth, 0, 200);

                list.Delivery = new byte[200];
                Array.Copy(RxBuffer, 800, list.Delivery, 0, 200);

                list.LookingFor = new byte[200];
                Array.Copy(RxBuffer, 1000, list.LookingFor, 0, 200);


                uint imageSize = BitConverter.ToUInt32(RxBuffer, 1200);

                globalVariables.MyPostImage1 = new byte[imageSize];

                list.ImageStructArray = Packet.AllocateHeapMemory((int)imageSize);

                Marshal.Copy(RxBuffer, 1200 + sizeof(uint), list.ImageStructArray, (int)imageSize);

                //Marshal.Copy(list.ImageStructArray, globalVariables.MyPostImage1, 0, (int)imageSize);

                //Packet.FreeBuffer(ref list.ImageStructArray);
                //list.ImageStructArray = IntPtr.Zero;


                AddListing(ref list);

                AddImageSize(ref imageSize);

                numberOfPosts -= 1;
            }
        }


        void AddListing(ref Listing newListing)
        {
            globalVariables.listings.Add(newListing);
        }

        void AddImageSize(ref uint imageSize)
        {
            globalVariables.MyPostImageSize.Add(imageSize);
        }


        void ShowListing()
        {
            for (int i = 0; i < globalVariables.listings.Count; i++)
            {
                byte[] imageArray = new byte[(int)globalVariables.MyPostImageSize[i]];

                Marshal.Copy(globalVariables.listings[i].ImageStructArray, imageArray, 0, (int)globalVariables.MyPostImageSize[i]);

                //IntPtr imagePtr = globalVariables.listings[i].ImageStructArray;

                //Packet.FreeBuffer(ref imagePtr);
                //imagePtr = IntPtr.Zero;

                MyPostsItem mpi = new MyPostsItem
                {
                    Image = ImageConversion.ToImage(imageArray),
                    Title = System.Text.Encoding.Default.GetString(globalVariables.listings[i].Title),
                    EstimatedWorth = System.Text.Encoding.Default.GetString(globalVariables.listings[i].EstimatedWorth),
                    Location = System.Text.Encoding.Default.GetString(globalVariables.listings[i].Location),
                    Condition = System.Text.Encoding.Default.GetString(globalVariables.listings[i].Condition),
                    Delivery = System.Text.Encoding.Default.GetString(globalVariables.listings[i].Delivery),
                    LookingFor = System.Text.Encoding.Default.GetString(globalVariables.listings[i].LookingFor)
                };


                globalVariables.Posts.Add(mpi);
            }
        }


        public NewMyPostsScreen()
        {
            InitializeComponent();

            globalVariables.ClearDynamicArrays();

            if (globalVariables.imageUploaded)
            {
                Hamburger.profile_icon.Visibility = Visibility.Collapsed;

                Hamburger.ProfileImage.Visibility = Visibility.Visible;

                Hamburger.ProfileImage.Source = ImageConversion.ToImage(globalVariables.receivedPostLoginImage);
            }


            SendRequestToGetCountOfPosts();

            int numberOfMyPosts = RecvAckOfNumberOfPosts();

            recvPosts(numberOfMyPosts);

            ShowListing();


            Closing += CloseClient.Client_Closing;

            Hamburger.MyPosts.Background = Brushes.LightGray;

            //Posts = new ObservableCollection<MyPostsItem>
            //{

            //new MyPostsItem
            //        {
            //            Image = ImageConversion.ToImage(globalVariables.MyPostImage1),
            //            Title = "Item 1",
            //            EstimatedWorth = "$100",
            //            Location = "City A",
            //            Condition = "Good",
            //            Delivery = "Local",
            //            LookingFor = "Item X"


            //        }
            //    new MyPostsItem
            //        {
            //            Image = "/Images/tv.jpg",
            //            Title = "Item 2",
            //            EstimatedWorth = "$50",
            //            Location = "City B",
            //            Condition = "Excellent",
            //            Delivery = "Nationwide",
            //            LookingFor = "Item Y"

            //        },

            //    new MyPostsItem
            //        {
            //            Image = "/Images/Image1.jpg",
            //            Title = "Item 3",
            //            EstimatedWorth = "$150",
            //            Location = "City C",
            //            Condition = "Excellent",
            //            Delivery = "Nationwide",
            //            LookingFor = "Item Z"

            //        }
            //};


            // Set the Items as the DataContext for the ListBox
            listBox.DataContext = globalVariables.Posts;

            
        }
    }
}
