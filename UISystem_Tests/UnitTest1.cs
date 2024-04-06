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
            string basePath = AppDomain.CurrentDomain.BaseDirectory;
            string path = basePath + "../../../../WPF_Front-End/bin/Debug/net6.0-windows/WPF_Front-End.exe";

            wpfApp = TestStack.White.Application.Launch(path);
        }


        /// <summary>
        /// This test verifies that if the user enters incorrect credentials, 
        /// they will receive a message that the user does not exist and they should sign up instead.
        /// </summary>
        [TestMethod]
        public void TestLogInPage_wrong_credentials()
        {
            // Retrieve the window by its title
            myWindow = wpfApp.GetWindow("NewHomeScreenPreLogin", InitializeOption.NoCache);

            // Finding the "Log In" button 
            Button loginButton = myWindow.Get<Button>(SearchCriteria.ByText("Log In"));

            // Clicking the "Log In" button
            loginButton.Click();

            // Switching to the next window
            Window nextPageWindow = wpfApp.GetWindow("LogIn", InitializeOption.NoCache);

            // Finding the username text box by its Automation ID within the next window
            var element = nextPageWindow.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("UsernameTextBox"));
            Assert.IsNotNull(element, "Element not found");

            // Performing actions on the elements found
            if (element != null)
            {
                element.Enter("Username1");
            }


            //Finding the password text box by its Automation ID within the log in window
            var passwordTextBox = nextPageWindow.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("PasswordTextBlock"));
            Assert.IsNotNull(passwordTextBox, "Password text box not found");

            // Enter password into the text box
            passwordTextBox.Enter("UserPassword");

            //Finding and clicking the login button within the log in window
            Button logInButton = nextPageWindow.Get<Button>(SearchCriteria.ByAutomationId("LogInButton"));
            logInButton.Click();


            Thread.Sleep(500); 

            // Finding the popped up message box and verifying its content
            Window messageBox = nextPageWindow.MessageBox("");

            Assert.IsNotNull(messageBox, "Message box not found");

            var label = messageBox.Get<Label>(SearchCriteria.Indexed(0));
            Assert.AreEqual("User doesn't Exist!! Please sign up instead.", label.Text);

            wpfApp.Kill();
        }


        /// <summary>
        /// This test verifies that if the user enters correct credentials, 
        /// they will be directed to the post log in page
        /// </summary>
        [TestMethod]
        public void TestLogInPage_correct_credentials()
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
                element.Enter("Sudhan");
            }


            //Find the password text box by its Automation ID within the next window
            var passwordTextBox = nextPageWindow.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("PasswordTextBlock"));
            Assert.IsNotNull(passwordTextBox, "Password text box not found");

            // Enter password into the text box
            passwordTextBox.Enter("Legend27");

            //Find and click the login button within the next window
            Button logInButton = nextPageWindow.Get<Button>(SearchCriteria.ByAutomationId("LogInButton"));
            logInButton.Click();


            Thread.Sleep(500); // Adjust the waiting time as necessary

            // Verifying log in by accessing the log out button from the post log in page meaning that the user logged in succesfully
            Window postLogInPage = wpfApp.GetWindow("NewHomeScreenPostLogin", InitializeOption.NoCache);

            Button LogOutButton = postLogInPage.Get<Button>(SearchCriteria.ByAutomationId("LogOutButton"));

            Assert.IsNotNull(LogOutButton);

            wpfApp.Kill();
        }




        /// <summary>
        /// This test verifies that the user can succesfully log out of their account 
        /// </summary>
        [TestMethod]
        public void TestLogout()
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
                element.Enter("Sudhan");
            }


            //Find the password text box by its Automation ID within the next window
            var passwordTextBox = nextPageWindow.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("PasswordTextBlock"));
            Assert.IsNotNull(passwordTextBox, "Password text box not found");

            // Enter password into the text box
            passwordTextBox.Enter("Legend27");

            //Find and click the login button within the next window
            Button logInButton = nextPageWindow.Get<Button>(SearchCriteria.ByAutomationId("LogInButton"));
            logInButton.Click();


            Thread.Sleep(500); // Adjust the waiting time as necessary

            // Verifying log in by accessing the log out button from the post log in page meaning that the user logged in succesfully
            Window postLogInPage = wpfApp.GetWindow("NewHomeScreenPostLogin", InitializeOption.NoCache);

            Button LogOutButton = postLogInPage.Get<Button>(SearchCriteria.ByAutomationId("LogOutButton"));


            LogOutButton.Click();

            // Verify the content of the log out confirmation screen
            Window logOutWindow = wpfApp.GetWindow("LogOut", InitializeOption.NoCache);

         
            Assert.IsNotNull(logOutWindow, "Message box not found");

            var logOutText = logOutWindow.Get<TestStack.White.UIItems.Label>(SearchCriteria.ByText("Are you sure you want to log out?"));

            Assert.IsNotNull(logOutText);

            // Click on the 'Yes' button to log out of the account
            Button logOutButton = logOutWindow.Get<Button>(SearchCriteria.ByAutomationId("LogOutYesButton"));

            logOutButton.Click();

            // Verify that the user has logged out by not having access to the my posts tab




            wpfApp.Kill();
        }


























        //verify that the user cannot access the pages on the pre log in screen









    }
}


