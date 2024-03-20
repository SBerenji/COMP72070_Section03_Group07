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
using WPF_Front_End;

namespace WPF_Front_End
{
    /// <summary>
    /// Interaction logic for LogOutPopUp.xaml
    /// </summary>
    public partial class LogOutPopUp : Window
    {
        private double windowWidth;
        private double windowHeight;

        private NewHomeScreenPostLogin PostLogIn;

        private Message m;

        private CreatePost cp;

        private NewMyPostsScreen MPS;

        private string str;
        public LogOutPopUp(NewHomeScreenPostLogin postLogIn)
        {
            InitializeComponent();

            PostLogIn = postLogIn;

            str = "NewHomeScreenPostLogIn";

            this.windowWidth = postLogIn.ActualWidth;
            this.windowHeight = postLogIn.ActualHeight;
            
            //if (PostLogIn != null && PostLogIn.ImagePlaceHolder != null)
            //{
            //    PostLogIn.ImagePlaceHolder.Visibility = Visibility.Collapsed;
            //}

        }
        public LogOutPopUp(Message m)
        {
            InitializeComponent();

            this.m = m;

            str = "Message";

            this.windowWidth = m.ActualWidth;
            this.windowHeight = m.ActualHeight;
        }

        public LogOutPopUp(CreatePost cp)
        {
            InitializeComponent();

            this.cp = cp;

            str = "CreatePost";

            this.windowWidth = cp.ActualWidth;
            this.windowHeight = cp.ActualHeight;
        }

        public LogOutPopUp(NewMyPostsScreen mps)
        {
            InitializeComponent();

            this.MPS = mps;

            str = "MyPostsScreen";

            this.windowWidth = mps.ActualWidth;
            this.windowHeight = mps.ActualHeight;
        }




        private void YesButton_Click(object sender, RoutedEventArgs e)
        {
            globalVariables.ClearDynamicArrays();

            var newForm = new NewHomeScreenPreLogin(); //create your new form.

            newForm.Width = this.windowWidth;
            newForm.Height = this.windowHeight;

            newForm.Show(); //show the new form.

            this.Close(); //only if you want to close the current form.

            if (this.str == "NewHomeScreenPostLogIn")
            {
                this.PostLogIn.Closing -= CloseClient.Client_Closing;

                this.PostLogIn.Close();
            }

            else if (this.str == "Message")
            {
                this.m.Closing -= CloseClient.Client_Closing;

                this.m.Close();
            }

            else if (this.str == "CreatePost")
            {
                this.cp.Closing -= CloseClient.Client_Closing;

                this.cp.Close();
            }

            else if (this.str == "MyPostsScreen")
            {
                this.MPS.Closing -= CloseClient.Client_Closing;

                this.MPS.Close();
            }

        }



        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {

            //var newForm = new NewHomeScreenPostLogin(); //create your new form.

            this.Close(); //only if you want to close the current form.
        }







    }
}
