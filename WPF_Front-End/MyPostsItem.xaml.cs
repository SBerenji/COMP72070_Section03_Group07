using System;
using System.Collections.Generic;
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

        private void Button_Delete(object sender, RoutedEventArgs e)
        {
            Listing list = new Listing();

            list.Title = new byte[ConstantVariables.listing_ByteArraySize];
            list.Title = Encoding.ASCII.GetBytes(title.Text);

            list.Location = new byte[ConstantVariables.listing_ByteArraySize];
            list.Location = Encoding.ASCII.GetBytes(location.Text);

            list.Condition = new byte[ConstantVariables.listing_ByteArraySize];
            list.Condition = Encoding.ASCII.GetBytes(condition.Text);

            list.EstimatedWorth = new byte[ConstantVariables.listing_ByteArraySize];
            list.EstimatedWorth = Encoding.ASCII.GetBytes(Estimated_Worth.Text);

            list.Delivery = new byte[ConstantVariables.listing_ByteArraySize];
            list.Delivery = Encoding.ASCII.GetBytes(delivery.Text);

            list.LookingFor = new byte[ConstantVariables.listing_ByteArraySize];
            list.LookingFor = Encoding.ASCII.GetBytes(lookingFor.Text);


            byte[] PostImageArray = ImageConversion.ToArrayFromImage((BitmapImage)temp.Source);


            int imageSize = PostImageArray.Length;

            list.ImageStructArray = Packet.AllocateHeapMemory(imageSize);

            Packet.CopyBufferToHeap(list.ImageStructArray, PostImageArray, imageSize);

            IntPtr BodyBuffer = Packet.AllocateListingPtr(imageSize);

            Packet.SerializeListingInformation(ref BodyBuffer, list, imageSize);






            IntPtr PktPtr = Packet.CreatePacket();


            IntPtr Head = Packet.AllocateHeaderPtr();

            Packet.SetHeaderInformation(ref Head, "127.0.0.1", "127.0.0.1", Route.DELETE_POST, true);


            Packet.SetHeader(PktPtr, Head);




            int size = (int)((6 * ConstantVariables.listing_ByteArraySize) * sizeof(byte)) + imageSize;


            Packet.SetBody(PktPtr, globalVariables.ClientID, BodyBuffer, size);


            IntPtr serializedRecv = Packet.SerializeData(PktPtr, out Packet.totalPktSize);


            Packet.TxBuffer = new byte[Packet.totalPktSize];


            Marshal.Copy(serializedRecv, Packet.TxBuffer, 0, Packet.totalPktSize);


            Packet.mockLogFile(ref serializedRecv);


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


            MessageBox.Show("Post Successfully deleted!! Please switch to another screen and come back to see the changes.");
        }
    }
}
