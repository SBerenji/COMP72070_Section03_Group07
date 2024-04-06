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
            // Retrieving the window by its title
            myWindow = wpfApp.GetWindow("NewHomeScreenPreLogin", InitializeOption.NoCache);

            // Finding the "Log In" button 
            Button loginButton = myWindow.Get<Button>(SearchCriteria.ByText("Log In"));

            // Clicking the "Log In" button
            loginButton.Click();

            // Switch to the next window
            Window nextPageWindow = wpfApp.GetWindow("LogIn", InitializeOption.NoCache);

            // Finding the username text box by its Automation ID within the log in window
            var element = nextPageWindow.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("UsernameTextBox"));
            Assert.IsNotNull(element, "Element not found");

            if (element != null)
            {
                // Entering the username into the text box
                element.Enter("Sudhan");
            }


            //Finding the password text box by its Automation ID within the log in window
            var passwordTextBox = nextPageWindow.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("PasswordTextBlock"));
            Assert.IsNotNull(passwordTextBox, "Password text box not found");

            // Enterung password into the text box
            passwordTextBox.Enter("Legend27");

            //Finding and clicking the login button within the log in window
            Button logInButton = nextPageWindow.Get<Button>(SearchCriteria.ByAutomationId("LogInButton"));
            logInButton.Click();


            Thread.Sleep(500);  // waiting for a whilte

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

            // Finding the "Log In" button 
            Button loginButton = myWindow.Get<Button>(SearchCriteria.ByText("Log In"));

            // Clicking the "Log In" button
            loginButton.Click();

            // Switching to the log in window
            Window nextPageWindow = wpfApp.GetWindow("LogIn", InitializeOption.NoCache);

            // Finding the username text box by its Automation ID within the log in window
            var element = nextPageWindow.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("UsernameTextBox"));
            Assert.IsNotNull(element, "Element not found");

            if (element != null)
            {
                // Entering username into the text box
                element.Enter("Sudhan");
            }


            //Finding the password text box by its Automation ID within the log in window
            var passwordTextBox = nextPageWindow.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("PasswordTextBlock"));
            Assert.IsNotNull(passwordTextBox, "Password text box not found");

            // Entering password into the text box
            passwordTextBox.Enter("Legend27");

            //Finding and clicking the login button within the log in window
            Button logInButton = nextPageWindow.Get<Button>(SearchCriteria.ByAutomationId("LogInButton"));
            logInButton.Click();


            Thread.Sleep(500);

            // Verifying log in by accessing the log out button from the post log in page meaning that the user logged in succesfully
            Window postLogInPage = wpfApp.GetWindow("NewHomeScreenPostLogin", InitializeOption.NoCache);

            Button LogOutButton = postLogInPage.Get<Button>(SearchCriteria.ByAutomationId("LogOutButton"));


            LogOutButton.Click();

            // Verify the content of the log out confirmation screen
            Window logOutWindow = wpfApp.GetWindow("LogOut", InitializeOption.NoCache);

         
            Assert.IsNotNull(logOutWindow, "Message box not found");

            var logOutText = logOutWindow.Get<TestStack.White.UIItems.Label>(SearchCriteria.ByText("Are you sure you want to log out?"));

            Assert.IsNotNull(logOutText);

            // Clicking on the 'Yes' button to log out of the account
            Button logOutButton = logOutWindow.Get<Button>(SearchCriteria.ByAutomationId("LogOutYesButton"));

            logOutButton.Click();

            // Verify that the user has logged out by not having access to the my posts tab
            // by finding and clicking on the 'Create Post' button in the pre-log in page.

            Window preLogInWindow = wpfApp.GetWindow("NewHomeScreenPreLogin", InitializeOption.NoCache);

            Button myPostsButton = preLogInWindow.Get<Button>(SearchCriteria.ByText("Create Post"));
           
            myPostsButton.Click();

            // Verifying the pop up that would block the users from accessing the tab
            Window notAutorizedToPostWindow = wpfApp.GetWindow("BlockUnLoggedUser", InitializeOption.NoCache);
            Thread.Sleep(500);


            Assert.IsNotNull(notAutorizedToPostWindow, "Message box not found");

            wpfApp.Kill();
        }







        /// <summary>
        /// This test verifies that if the user is not authorized they will not have access to the 'Create Post', 'Messages', and 'My Posts' pages
        /// </summary>
        [TestMethod]
        public void TestPreLogInPageAccessAuthorization()
        {
            // Retrieve the window by its title
            Window preLogInWindow = wpfApp.GetWindow("NewHomeScreenPreLogin", InitializeOption.NoCache);


            // Verifying the pop up that would block the users from accessing the 'Create Post' tab
            Button createPostButton = preLogInWindow.Get<Button>(SearchCriteria.ByText("Create Post"));

            createPostButton.Click();
            Window notAutorizedToPostWindow = wpfApp.GetWindow("BlockUnLoggedUser", InitializeOption.NoCache);

            Thread.Sleep(500);

            Assert.IsNotNull(notAutorizedToPostWindow);

            // close the current notAutorizedToPostWindow
            notAutorizedToPostWindow.Close();


            // Verifying the pop up that would block the users from accessing the 'Messages' tab
            Button messagesButton = preLogInWindow.Get<Button>(SearchCriteria.ByText("Messages"));

            messagesButton.Click();
            Thread.Sleep(500);

            Window notAutorizedMessagesWindow = wpfApp.GetWindow("BlockUnLoggedUser", InitializeOption.NoCache);

            Assert.IsNotNull(notAutorizedMessagesWindow);
            notAutorizedMessagesWindow.Close();


            // Verifying the pop up that would block the users from accessing the 'Messages' tab
            Button myPostsButton = preLogInWindow.Get<Button>(SearchCriteria.ByText("My Posts"));


            myPostsButton.Click();

            Window notAutorizedMyPostsWindow = wpfApp.GetWindow("BlockUnLoggedUser", InitializeOption.NoCache);


            Assert.IsNotNull(notAutorizedMyPostsWindow);
            Thread.Sleep(1000);


            wpfApp.Kill();
        }












        }
}


