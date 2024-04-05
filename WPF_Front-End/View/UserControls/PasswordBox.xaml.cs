using Microsoft.Windows.Themes;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Security;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

//namespace WPF_Front_End.View.UserControls
//{
    /// <summary>
    /// Interaction logic for PasswordBox.xaml
    /// </summary>
    //public partial class PasswordBox : UserControl
    //{
    //    private bool _isPasswordChanging;

    //    // Using a DependencyProperty as the backing store for MyProperty.  This enables animation, styling, binding, etc...
    //    public static readonly DependencyProperty PasswordProperty =
    //        DependencyProperty.Register("Password", typeof(string), typeof(PasswordBox),
    //            new FrameworkPropertyMetadata(string.Empty, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault, 
    //                PasswordPropertyChanged, null, false, UpdateSourceTrigger.PropertyChanged));
                
    //    private static void PasswordPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
    //    {
    //        if (d is PasswordBox passwordBox)
    //        {
    //            passwordBox.UpdatePassword();
    //        }
    //    }


        

    //    public string Password
    //    {
    //        get { return (string)GetValue(PasswordProperty); }
    //        set { SetValue(PasswordProperty, value); }
    //    }

    //    public PasswordBox()
    //    {
    //        InitializeComponent();
    //    }

    //    private string placeholder;

    //    public string Placeholder
    //    {
    //        get { return placeholder; }
    //        set
    //        {
    //            placeholder = value;

    //            tbPlaceholder.Text = placeholder;
    //        }
    //    }

    //    private void txtInput_TextChanged(object sender, TextChangedEventArgs e)
    //    {

    //    }

    //    private void passwordBox_PasswordChanged(object sender, RoutedEventArgs e)
    //    {
    //        if (string.IsNullOrEmpty(passwordBox.Password))
    //        {
    //            tbPlaceholder.Visibility = Visibility.Visible;
    //        }

    //        else
    //        {
    //            tbPlaceholder.Visibility = Visibility.Hidden;
    //        }

    //        _isPasswordChanging = true;
    //        Password = passwordBox.Password;
    //        _isPasswordChanging = false;
    //    }

    //    private void UpdatePassword()
    //    {
    //        if (!_isPasswordChanging)
    //        {
    //            passwordBox.Password = Password;
    //        }
    //    }


    //    //private void PasswordBox_PasswordChanged(object sender, RoutedEventArgs e)
    //    //{
    //    //    //if (sender is PasswordBox passwordBox)
    //    //    //{
    //    //    //    SetPassword(passwordBox.Password);
    //    //    //}
    //    //}


    //    //    public SecureString Password
    //    //    {
    //    //        get { return (SecureString)GetValue(PasswordProperty); }
    //    //        set { SetValue(PasswordProperty, value); }
    //    //    }


    //    //    public static readonly DependencyProperty PasswordProperty =
    //    //        DependencyProperty.Register("Password", typeof(SecureString), typeof(PasswordBox), new PropertyMetadata(new SecureString()));

    //    //    private void SetPassword(SecureString securePassword) {
    //    //        Password = securePassword.Copy();
    //    //    }
    //    //}



    //    //public class CornerRadiusSetter
    //    //{
    //    //    public static CornerRadius GetCornerRadius(DependencyObject obj) => (CornerRadius)obj.GetValue(CornerRadiusProperty);

    //    //    public static void SetCornerRadius(DependencyObject obj, CornerRadius value) => obj.SetValue(CornerRadiusProperty, value);

    //    //    public static readonly DependencyProperty CornerRadiusProperty =
    //    //        DependencyProperty.RegisterAttached(nameof(Border.CornerRadius), typeof(CornerRadius),
    //    //            typeof(CornerRadiusSetter), new UIPropertyMetadata(new CornerRadius(), CornerRadiusChangedCallback));

    //    //    public static void CornerRadiusChangedCallback(object sender, DependencyPropertyChangedEventArgs e)
    //    //    {
    //    //        Control control = sender as Control;

    //    //        if (control == null) return;

    //    //        control.Loaded += Control_Loaded;
    //    //    }

    //    //    private static void Control_Loaded(object sender, EventArgs e)
    //    //    {
    //    //        Control control = sender as Control;

    //    //        if (control == null || control.Template == null) return;

    //    //        control.ApplyTemplate();

    //    //        CornerRadius cornerRadius = GetCornerRadius(control);

    //    //        Control toggleButton = control.Template.FindName("toggleButton", control) as Control;

    //    //        if (control is ComboBox && toggleButton != null)
    //    //        {
    //    //            toggleButton.ApplyTemplate();

    //    //            // Set border radius for border radius border
    //    //            Border toggleButtonBorder = toggleButton.Template.FindName("templateRoot", toggleButton) as Border;
    //    //            toggleButtonBorder.CornerRadius = cornerRadius;

    //    //            // Expand padding for combobox to avoid text clipping by border radius
    //    //            control.Padding = new Thickness(
    //    //                    control.Padding.Left + cornerRadius.BottomLeft,
    //    //                    control.Padding.Top,
    //    //                    control.Padding.Right + cornerRadius.BottomRight,
    //    //                    control.Padding.Bottom);

    //    //            // Decrease width of dropdown and center it to avoid showing "sticking" dropdown corners
    //    //            Popup popup = control.Template.FindName("PART_Popup", control) as Popup;

    //    //            if (popup != null)
    //    //            {
    //    //                double offset = cornerRadius.BottomLeft - 1;
    //    //                if (offset > 0)
    //    //                    popup.HorizontalOffset = offset;
    //    //            }

    //    //            SystemDropShadowChrome shadowChrome = control.Template.FindName("shadow", control) as SystemDropShadowChrome;

    //    //            if (shadowChrome != null)
    //    //            {
    //    //                double minWidth = control.ActualWidth - cornerRadius.BottomLeft - cornerRadius.BottomRight;
    //    //                if (minWidth > 0)
    //    //                    shadowChrome.MinWidth = minWidth;
    //    //            }
    //    //        }

    //    //        // setting borders for non-combobox controls
    //    //        Border border = control.Template.FindName("border", control) as Border;

    //    //        if (border == null) return;

    //    //        border.CornerRadius = cornerRadius;
    //    //    }
    //}
//}

