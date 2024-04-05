using System;
using System.Threading;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using TestStack.White;
using TestStack.White.Factory;
using TestStack.White.UIItems;
using TestStack.White.UIItems.Finders;
using TestStack.White.UIItems.WindowItems;
using static System.Net.Mime.MediaTypeNames;

namespace SystemTests_TestStack
{
    [TestClass]
    public class UITests
    {
        private TestStack.White.Application wpfApp;
        private Window myWindow;

        [TestInitialize]
        public void SetUp()
        {
            wpfApp = TestStack.White.Application.Launch("D:\\Client_project4\\WPF_Front-End\\bin\\Debug\\net6.0-windows\\WPF_Front-End.exe");
        }

        [TestMethod]
        public void TestLogInPage()
        {
            // Retrieve the window by its title
            myWindow = wpfApp.GetWindow("NewHomeScreenPreLogin", InitializeOption.NoCache);

            // Find the "Log In" button 
            Button loginButton = myWindow.Get<Button>(SearchCriteria.ByText("Log In"));

            // Click the "Log In" button
            loginButton.Click();
            //Thread.Sleep(1000); // You may replace this with a more robust waiting mechanism

            // Now, switch to the next window
            Window nextPageWindow = wpfApp.GetWindow("LogIn", InitializeOption.NoCache);

            // Find the username text box by its Automation ID within the next window
            var element = nextPageWindow.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("UsernameTextBox"));
            Assert.IsNotNull(element, "Element not found");

            // Perform actions on the element
            // For example, if it's a TextBox, you can enter text into it
            if (element != null)
            {
                element.Enter("Username1");
            }



            //Find the password text box by its Automation ID within the next window
           var passwordTextBox = nextPageWindow.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("PasswordTextBlock"));
            Assert.IsNotNull(passwordTextBox, "Password text box not found");

            // Enter password into the text box
            passwordTextBox.Enter("UserPassword");

            //Find and click the login button within the next window
            Button logInButton = nextPageWindow.Get<Button>(SearchCriteria.ByAutomationId("LogInButton"));
            logInButton.Click();






            // Find other controls (password TextBox and login Button) within the log in window and assert their presence
            //var passwordTextBox = myWindow.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByText("Password"));
            //Assert.IsNotNull(passwordTextBox);

            //var loginButton = myWindow.Get<TestStack.White.UIItems.Button>(SearchCriteria.ByText("Login"));
            //Assert.IsNotNull(loginButton);
        }

    }
}


