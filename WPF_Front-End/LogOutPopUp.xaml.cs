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
        }
        public LogOutPopUp(Message m)
        {
            InitializeComponent();
            this.m = m;

            str = "Message";
        }

        public LogOutPopUp(CreatePost cp)
        {
            InitializeComponent();

            this.cp = cp;

            str = "CreatePost";
        }

        public LogOutPopUp(NewMyPostsScreen mps)
        {
            InitializeComponent();

            this.MPS = mps;

            str = "MyPostsScreen";
        }




        private void YesButton_Click(object sender, RoutedEventArgs e)
        {
            var newForm = new NewHomeScreenPreLogin(); //create your new form.

            newForm.Show(); //show the new form.
            this.Close(); //only if you want to close the current form.

            if (this.str == "NewHomeScreenPostLogIn")
            {
                this.PostLogIn.Close();
            }

            else if (this.str == "Message")
            {
                this.m.Close();
            }

            else if (this.str == "CreatePost")
            {
                this.cp.Close();
            }

            else if (this.str == "MyPostsScreen")
            {
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
