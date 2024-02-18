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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WPF_Front_End.View.UserControls
{
    /// <summary>
    /// Interaction logic for ClearableTextBox.xaml
    /// </summary>
    public partial class ClearableTextBox : UserControl
    {
        public ClearableTextBox()
        {
            InitializeComponent();
        }

        private string placeholder;

        public string Placeholder
        {
            get { return placeholder; }
            set
            {
                placeholder = value;

                tbPlaceholder.Text = placeholder;
            }
        }

        //public static readonly DependencyProperty IsPasswordBoxProperty =
        //    DependencyProperty.Register("IsPasswordBox", typeof(bool), typeof(ClearableTextBox), new PropertyMetadata(false, OnIsPasswordBoxChanged));

        //public bool IsPasswordBox
        //{
        //    get { return (bool)GetValue(IsPasswordBoxProperty); }
        //    set { SetValue(IsPasswordBoxProperty, value); }
        //}


        //private static void OnIsPasswordBoxChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        //{
        //    var txtInput = (d as ClearableTextBox)?.txtInput;

        //    if (txtInput != null)
        //    {
        //        if ((bool)e.NewValue)
        //        {
        //            txtInput.PasswordChar = '*';
        //        }

        //        else
        //        {
        //            txtInput.PasswordChar = '\0';
        //        }
        //    }
        //}

        private void txtInput_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (string.IsNullOrEmpty(txtInput.Text))
            {
                tbPlaceholder.Visibility = Visibility.Visible;
            }

            else
            {
                tbPlaceholder.Visibility = Visibility.Hidden;
            }
        }

        private void btnClear_Click(object sender, RoutedEventArgs e)
        {
            txtInput.Clear();
            txtInput.Focus();
        }
    }
}
