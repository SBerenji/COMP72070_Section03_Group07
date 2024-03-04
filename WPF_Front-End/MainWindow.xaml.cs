﻿using System;
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
using System.Runtime.InteropServices;

namespace WPF_Front_End
{

    public partial class MainWindow : Window
    {
        

        string username;

        public MainWindow()
        {
            InitializeComponent();
        }
        
        private void Controls_Loaded(object sender, RoutedEventArgs e)
        {

            //this.username = LogInControl.getUserInputtedUsername();

            //globalVariables.username = this.username;
        }

        private void Triangle_Loaded(object sender, RoutedEventArgs e)
        {

        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            var newHome = new HomeScreenPostLogIn(); //create your new form.
            newHome.Show(); //show the new form.
            this.Close(); //only if you want to close the current form.
        }


        public string getUsername()
        {
            return this.username;
        }
    }
}
