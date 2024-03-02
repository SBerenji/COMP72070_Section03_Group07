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
    /// Interaction logic for PeopleAndMessage.xaml
    /// </summary>
    public partial class PeopleAndMessage : UserControl
    {
        public PeopleAndMessage()
        {
            InitializeComponent();

            CustomiseClearableTextBox();
        }


        private void CustomiseClearableTextBox()
        {
            TypeMessage.tbPlaceholder.Foreground = Brushes.Black;

            CornerRadius cr = new CornerRadius(0.1);

            CornerRadiusSetter.SetCornerRadius(TypeMessage.txtInput, cr);
        }
    }
}
