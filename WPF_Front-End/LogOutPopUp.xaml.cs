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
        private HomeScreenPostLogIn PostLogIn;

        private Chat CH;

        private NewPost NP;

        private MyPostsScreen MPS;

        private string str;
        public LogOutPopUp(HomeScreenPostLogIn postLogIn)
        {
            InitializeComponent();
            PostLogIn = postLogIn;

            str = "HomeScreenPostLogIn";
        }
        public LogOutPopUp(Chat ch)
        {
            InitializeComponent();
            this.CH = ch;

            str = "Chat";
        }

        public LogOutPopUp(NewPost np)
        {
            InitializeComponent();

            this.NP = np;

            str = "NewPost";
        }

        public LogOutPopUp(MyPostsScreen mps)
        {
            InitializeComponent();

            this.MPS = mps;

            str = "MyPostsScreen";
        }




        private void YesButton_Click(object sender, RoutedEventArgs e)
        {
            var newForm = new HomeScreenPreLogIn(); //create your new form.

            newForm.Show(); //show the new form.
            this.Close(); //only if you want to close the current form.

            if (this.str == "HomeScreenPostLogIn")
            {
                this.PostLogIn.Close();
            }

            else if (this.str == "Chat")
            {
                this.CH.Close();
            }

            else if (this.str == "NewPost")
            {
                this.NP.Close();
            }

            else if (this.str == "MyPostsScreen")
            {
                this.MPS.Close();
            }

        }



        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {

            var newForm = new HomeScreenPostLogIn(); //create your new form.

            this.Close(); //only if you want to close the current form.
        }







    }
}
