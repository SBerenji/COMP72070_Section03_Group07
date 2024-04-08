using System;
using System.Collections.Specialized;
using System.Runtime.CompilerServices;
using System.Threading;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using TestStack.White;
using TestStack.White.Factory;
using TestStack.White.InputDevices;
using TestStack.White.UIItems;
using TestStack.White.UIItems.Finders;
using TestStack.White.UIItems.WindowItems;
using static System.Net.Mime.MediaTypeNames;
using System.Windows;
using System.Windows.Input;
using TestStack.White.UIItems.Scrolling;

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
        public void TEST_SYS_28_LogInPage_wrong_credentials()
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
        public void TEST_SYS_28_LogInPage_correct_credentials()
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
        public void TEST_SYS_28_Logout()
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
        public void TEST_SYS_35_PreLogInPageAccessAuthorization()
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


        /// <summary>
        /// This test verifies that the user can navigate to all of the tabs once they are logged in
        /// </summary>
        [TestMethod]
        public void TEST_SYS_35_PostLogInPageAccessAuthorization()
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

            Window postLogInPage = wpfApp.GetWindow("NewHomeScreenPostLogin", InitializeOption.NoCache);
            Assert.IsNotNull(postLogInPage, "Page not found");



            Button createPostButton = postLogInPage.Get<Button>(SearchCriteria.ByText("Create Post"));

            createPostButton.Click();
            Thread.Sleep(500);


            Window createPostWindow = wpfApp.GetWindow("CreatePost", InitializeOption.NoCache);

            Button messagesButton = createPostWindow.Get<Button>(SearchCriteria.ByText("Messages"));

            messagesButton.Click();
            Thread.Sleep(500);

            Window messagesWindow = wpfApp.GetWindow("Message", InitializeOption.NoCache);


            Button myPostButton = messagesWindow.Get<Button>(SearchCriteria.ByText("My Posts"));

            myPostButton.Click();
            Thread.Sleep(500);


            wpfApp.Kill();

        }




        /// <summary>
        /// This test verifies that the user can sign up by entering their username, email, and password
        /// </summary>
        [TestMethod]
        public void TEST_SYS_28_TestUserSignUp()
        {
            // Retrieve the window by its title
            Window preLogInWindow = wpfApp.GetWindow("NewHomeScreenPreLogin", InitializeOption.NoCache);

            var createAccountTextBlock = preLogInWindow.Get<TestStack.White.UIItems.Label>(SearchCriteria.ByText("Create An Account"));
            createAccountTextBlock.Click();


            // Switching to the sign up window
            Window signUpPage = wpfApp.GetWindow("Swapify", InitializeOption.NoCache);

            // Finding the username text box by its Automation ID within the sign up window
            var signUpTextBox= signUpPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("SignUpUsernameTextBox"));


            Assert.IsNotNull(signUpTextBox, "Element not found");

            // Performing actions on the elements found
            if (signUpTextBox != null)
            {
                signUpTextBox.Enter("Student1");
            }



            //Finding the email text box by its Automation ID within the sign up window
            var emailTextBox = signUpPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("SignUpEmailTextBox"));
            Assert.IsNotNull(emailTextBox, "Email text box not found");

            // Enter email into the text box
            if (emailTextBox != null)
            {
                emailTextBox.Enter("student1@gmail.com");
            }



            //Finding the password text box by its Automation ID within the sign up window
            var passwordTextBox = signUpPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("SignUpPasswordTextBox"));
            Assert.IsNotNull(passwordTextBox, "Password text box not found");

            // Enter password into the text box

            if (passwordTextBox != null)
            {
                passwordTextBox.Enter("student1Password");
            }


            //Finding and clicking the sign up button within the sign up window
            Button signUpButton = signUpPage.Get<Button>(SearchCriteria.ByAutomationId("SignUpButton"));
            signUpButton.Click();

            Thread.Sleep(500);


            Window postSignUpWindow = wpfApp.GetWindow("NewHomeScreenPostLogin", InitializeOption.NoCache);


            Assert.IsNotNull(postSignUpWindow);
            Thread.Sleep(500);


            wpfApp.Kill();
        }






        /// <summary>
        /// This test verifies that the user will be denied to create an account if the account already exists
        /// </summary>

        [TestMethod]
        public void TEST_SYS_28_TestUserSignUpError()
        {
            // Retrieve the window by its title
            Window preLogInWindow = wpfApp.GetWindow("NewHomeScreenPreLogin", InitializeOption.NoCache);

            var createAccountTextBlock = preLogInWindow.Get<TestStack.White.UIItems.Label>(SearchCriteria.ByText("Create An Account"));
            createAccountTextBlock.Click();


            // Switching to the sign up window
            Window signUpPage = wpfApp.GetWindow("Swapify", InitializeOption.NoCache);

            // Finding the username text box by its Automation ID within the sign up window
            var signUpTextBox = signUpPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("SignUpUsernameTextBox"));


            Assert.IsNotNull(signUpTextBox, "Element not found");

            // Performing actions on the elements found
            if (signUpTextBox != null)
            {
                signUpTextBox.Enter("Student1");
            }



            //Finding the email text box by its Automation ID within the sign up window
            var emailTextBox = signUpPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("SignUpEmailTextBox"));
            Assert.IsNotNull(emailTextBox, "Email text box not found");

            // Enter email into the text box
            if (emailTextBox != null)
            {
                emailTextBox.Enter("student1@gmail.com");
            }



            //Finding the password text box by its Automation ID within the sign up window
            var passwordTextBox = signUpPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("SignUpPasswordTextBox"));
            Assert.IsNotNull(passwordTextBox, "Password text box not found");

            // Enter password into the text box

            if (passwordTextBox != null)
            {
                passwordTextBox.Enter("student1Password");
            }


            //Finding and clicking the sign up button within the sign up window
            Button signUpButton = signUpPage.Get<Button>(SearchCriteria.ByAutomationId("SignUpButton"));
            signUpButton.Click();

            // verify the message box that appears

            Window messageBox = signUpPage.MessageBox("");

            Assert.IsNotNull(messageBox, "Message box not found");

            var label = messageBox.Get<Label>(SearchCriteria.Indexed(0));
            Assert.AreEqual("User Already exits!! Please LogIn instead.", label.Text);

            Thread.Sleep(500);

            wpfApp.Kill();
        }


        /// <summary>
        /// This test verifies that the user can sign up by entering their username, email, and password, and profile image
        /// </summary>
        // IMPORTANT NOTE: IN ORDER TO RUN THIS TEST YOU CAN COPY 'TestImage.jpeg' FROM THE 'Test_Images' directory
        // in the project directory AND PASTE IT IN 'DOWNLOAD' DIRECTORY OF YOUR LAPTOP
        [TestMethod]
        public void TEST_SYS_28_SignUpWithImage()
        {
            // Retrieve the window by its title
            Window preLogInWindow = wpfApp.GetWindow("NewHomeScreenPreLogin", InitializeOption.NoCache);

            var createAccountTextBlock = preLogInWindow.Get<TestStack.White.UIItems.Label>(SearchCriteria.ByText("Create An Account"));
            createAccountTextBlock.Click();


            // Switching to the sign up window
            Window signUpPage = wpfApp.GetWindow("Swapify", InitializeOption.NoCache);

            // Finding the username text box by its Automation ID within the sign up window
            var signUpTextBox = signUpPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("SignUpUsernameTextBox"));


            Assert.IsNotNull(signUpTextBox, "Element not found");

            // Performing actions on the elements found
            if (signUpTextBox != null)
            {
                signUpTextBox.Enter("Student2");
            }



            //Finding the email text box by its Automation ID within the sign up window
            var emailTextBox = signUpPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("SignUpEmailTextBox"));
            Assert.IsNotNull(emailTextBox, "Email text box not found");

            // Enter email into the text box
            if (emailTextBox != null)
            {
                emailTextBox.Enter("student2@gmail.com");
            }



            //Finding the password text box by its Automation ID within the sign up window
            var passwordTextBox = signUpPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("SignUpPasswordTextBox"));
            Assert.IsNotNull(passwordTextBox, "Password text box not found");

            // Enter password into the text box

            if (passwordTextBox != null)
            {
                passwordTextBox.Enter("student2Password");
            }


            // Adding the profile image by browsing the file explorer

            //var browseButton = signUpPage.Get<TestStack.White.UIItems.Image>(SearchCriteria.ByAutomationId("BrowsingButton"));
            //browseButton.Click();

            // coordinates of where to click on the sign up page to access the file explorer 
            int targetX = 1250;
            int targetY = 440;

            // Simulate a mouse click on the specified coordinates
            AttachedMouse attachedMouse = signUpPage.Mouse;

            Thread.Sleep(1000);

            attachedMouse.Click(new System.Windows.Point(targetX, targetY));


            // Waiting for the file dialog window to appear
            var fileExplorerWindow = signUpPage.ModalWindow("Open");

            Assert.IsNotNull(fileExplorerWindow);


            // Finding and selecting the desired image file by entering its file name
            var addressBarTextBox = fileExplorerWindow.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByText("File name:"));
            //addressBarTextBox.Enter("../../../../Test_Images");

            addressBarTextBox.Enter("TestImage.jpeg");   // Resolution: 506 x 398  Size: 46.9 KB

            var openButton = fileExplorerWindow.Get<Button>(SearchCriteria.ByText("Open"));
            openButton.Click();



            //Finding and clicking the sign up button within the sign up window
            Button signUpButton = signUpPage.Get<Button>(SearchCriteria.ByAutomationId("SignUpButton"));
            signUpButton.Click();

            Thread.Sleep(500);

            // Verify the post log in page
            Window postSignUpWindow = wpfApp.GetWindow("NewHomeScreenPostLogin", InitializeOption.NoCache);
            Assert.IsNotNull(postSignUpWindow);


            // Verify that the profile image is accessible in the post log in page

            var profileImage = postSignUpWindow.Get<TestStack.White.UIItems.Image>(SearchCriteria.ByAutomationId("ProfileImage"));
            Assert.IsNotNull(profileImage);

            //Thread.Sleep(500);

            wpfApp.Kill();
        }



        /// <summary>
        /// This test verifies that the user can succesfully create a post
        /// </summary>

        // IMPORTANT NOTE: IN ORDER TO RUN THIS TEST YOU CAN COPY 'TestImage4.jpeg' FROM THE 'Test_Images' directory
        // in the project directory AND PASTE IT IN 'DOWNLOAD' DIRECTORY OF YOUR LAPTOP
        [TestMethod]
        public void TEST_SYS_27_CreatingPost()
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

            Window postLogInPage = wpfApp.GetWindow("NewHomeScreenPostLogin", InitializeOption.NoCache);
            Assert.IsNotNull(postLogInPage, "Page not found");



            Button createPostButton = postLogInPage.Get<Button>(SearchCriteria.ByText("Create Post"));

            createPostButton.Click();
            Thread.Sleep(500);

            // verifying the presence of the CreatePost window
            Window createPostPage = wpfApp.GetWindow("CreatePost", InitializeOption.NoCache);
            Assert.IsNotNull(createPostPage, "Page not found");

            // Filling out all the information in the create post tab
            var titleTextBox = createPostPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("titleTextBox"));
            Assert.IsNotNull(titleTextBox, "Title text box not found");
            titleTextBox.Enter("Mixer");


            var locationTextBox = createPostPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("locationTextBox"));
            Assert.IsNotNull(locationTextBox, "Location text box not found");
            locationTextBox.Enter("Toronto");



            var conditionTextBox = createPostPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("conditionTextBox"));
            Assert.IsNotNull(conditionTextBox, "Condition text box not found");
            conditionTextBox.Enter("New");



            var worthTextBox = createPostPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("worthTextBox"));
            Assert.IsNotNull(worthTextBox, "worth text box not found");
            worthTextBox.Enter("About $100");


            var deliveryTextBox = createPostPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("deliveryTextBox"));
            Assert.IsNotNull(deliveryTextBox, "worth text box not found");
            deliveryTextBox.Enter("I can drop off if in Toronto");


            var lookingForTextBox = createPostPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("LookingForTextBox"));
            Assert.IsNotNull(lookingForTextBox, "worth text box not found");
            lookingForTextBox.Enter("Nothing, just want to give it away");


            // Adding the image of the item by browsing the file explorer

            // coordinates of where to click on the sign up page to access the file explorer 
            int targetX = 730;
            int targetY = 365;

            // Simulate a mouse click on the specified coordinates
            AttachedMouse attachedMouse = createPostPage.Mouse;

            Thread.Sleep(1000);

            attachedMouse.Click(new System.Windows.Point(targetX, targetY));


            // Waiting for the file dialog window to appear
            var fileExplorerWindow = createPostPage.ModalWindow("Open");

            Assert.IsNotNull(fileExplorerWindow);


            // Finding and selecting the desired image file by entering its file name
            var addressBarTextBox = fileExplorerWindow.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByText("File name:"));

            addressBarTextBox.Enter("TestImage4.jpeg");   // Resolution: 1500 x 1125  Size: 73.9 KB

            var openButton = fileExplorerWindow.Get<Button>(SearchCriteria.ByText("Open"));
            openButton.Click();


            //Finding and clicking the sign up button within the sign up window
            Button postingButton = createPostPage.Get<Button>(SearchCriteria.ByAutomationId("PostClick"));
            postingButton.Click();

            Thread.Sleep(500);

            Window messageBox = createPostPage.MessageBox("");

            Assert.IsNotNull(messageBox, "Message box not found");

            var label = messageBox.Get<Label>(SearchCriteria.Indexed(0));
            Assert.AreEqual("Item successfully Posted!! You can now view your post from 'My Posts' tab.", label.Text);

            Thread.Sleep(500);

            wpfApp.Kill();

        }


        /// </summary>
        /// This test will verify that the users can delete the posts they created from the My Posts tab
        /// </summary>
        // IMPORTANT NOTE: IN ORDER TO RUN THIS TEST YOU CAN COPY 'TestImage5.jpeg' FROM THE 'Test_Images' directory
        // in the project directory AND PASTE IT IN 'DOWNLOAD' DIRECTORY OF YOUR LAPTOP
        [TestMethod]
        public void TEST_SYS_33_DeletePosts()
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
                element.Enter("Saba");
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

            Window postLogInPage = wpfApp.GetWindow("NewHomeScreenPostLogin", InitializeOption.NoCache);
            Assert.IsNotNull(postLogInPage, "Page not found");

            Button createPostButton = postLogInPage.Get<Button>(SearchCriteria.ByText("Create Post"));

            createPostButton.Click();
            Thread.Sleep(500);

            // verifying the presence of the CreatePost window
            Window createPostPage = wpfApp.GetWindow("CreatePost", InitializeOption.NoCache);
            Assert.IsNotNull(createPostPage, "Page not found");



            // Filling out all the information in the create post tab
            var titleTextBox = createPostPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("titleTextBox"));
            Assert.IsNotNull(titleTextBox, "Title text box not found");
            titleTextBox.Enter("Map");


            var locationTextBox = createPostPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("locationTextBox"));
            Assert.IsNotNull(locationTextBox, "Location text box not found");
            locationTextBox.Enter("Waterloo");



            var conditionTextBox = createPostPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("conditionTextBox"));
            Assert.IsNotNull(conditionTextBox, "Condition text box not found");
            conditionTextBox.Enter("New");



            var worthTextBox = createPostPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("worthTextBox"));
            Assert.IsNotNull(worthTextBox, "worth text box not found");
            worthTextBox.Enter("About $10");


            var deliveryTextBox = createPostPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("deliveryTextBox"));
            Assert.IsNotNull(deliveryTextBox, "worth text box not found");
            deliveryTextBox.Enter("Drop off");


            var lookingForTextBox = createPostPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("LookingForTextBox"));
            Assert.IsNotNull(lookingForTextBox, "worth text box not found");
            lookingForTextBox.Enter("I want to swap with a globe");


            // Adding the image of the item by browsing the file explorer

            // coordinates of where to click on the sign up page to access the file explorer 
            int targetX = 730;
            int targetY = 365;

            // Simulate a mouse click on the specified coordinates
            AttachedMouse attachedMouse = createPostPage.Mouse;

            Thread.Sleep(1000);

            attachedMouse.Click(new System.Windows.Point(targetX, targetY));


            // Waiting for the file dialog window to appear
            var fileExplorerWindow = createPostPage.ModalWindow("Open");

            Assert.IsNotNull(fileExplorerWindow);


            // Finding and selecting the desired image file by entering its file name
            var addressBarTextBox = fileExplorerWindow.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByText("File name:"));

            addressBarTextBox.Enter("TestImage6.jpeg");   // Resolution: 3000 x 2050  Size: 1.94 MB

            var openButton = fileExplorerWindow.Get<Button>(SearchCriteria.ByText("Open"));
            openButton.Click();


            //Finding and clicking the sign up button within the sign up window
            Button postingButton = createPostPage.Get<Button>(SearchCriteria.ByAutomationId("PostClick"));
            postingButton.Click();

            Thread.Sleep(500);


            // Verifying the content of the popped up message box
            Window messageBox = createPostPage.MessageBox("");
            
            Assert.IsNotNull(messageBox, "Message box not found");

            var label = messageBox.Get<Label>(SearchCriteria.Indexed(0));
            Assert.AreEqual("Item successfully Posted!! You can now view your post from 'My Posts' tab.", label.Text);


            Thread.Sleep(500);

            int targetXX = 1120;
            int targetYY = 600;

            // Simulate a mouse click on the specified coordinates
            AttachedMouse attachedMouseOkButton = createPostPage.Mouse;

            Thread.Sleep(500);

            attachedMouseOkButton.Click(new System.Windows.Point(targetXX, targetYY));


            Button myPostsButton = createPostPage.Get<Button>(SearchCriteria.ByText("My Posts"));
            myPostsButton.Click();


            Window mypostsWindow = wpfApp.GetWindow("NewMyPostsScreen", InitializeOption.NoCache);
            Assert.IsNotNull(mypostsWindow, "Page not found");

            var mypostsItemWindow = mypostsWindow.Get<Label>(SearchCriteria.ByText("I want to swap with a globe"));
            Assert.IsNotNull(mypostsItemWindow, "Not found");

            Thread.Sleep(500);

            int targetXXX = 1120;
            int targetYYY = 550;

            // Simulate a mouse click on the specified coordinates
            AttachedMouse attachedMouseOkButton2 = createPostPage.Mouse;

            Thread.Sleep(500);

            attachedMouseOkButton2.Click(new System.Windows.Point(targetXXX, targetYYY));

            // Verifying the content of the popped up message box
            Window messageBoxDelete = mypostsWindow.MessageBox("");

            Assert.IsNotNull(messageBoxDelete, "Message box not found");

            var label2 = messageBoxDelete.Get<Label>(SearchCriteria.Indexed(0));
            Assert.AreEqual("Post Successfully deleted!! Please switch to another screen and come back to see the changes.", label2.Text);




            //var scrollBar = mypostsWindow.ScrollBars.Vertical;

            // Variable to keep track of whether we can still scroll down
            //IVScrollBar scrollBar = mypostsWindow.ScrollBars.Vertical;

            //// Variable to keep track of whether we can still scroll down
            //bool canScroll = true;

            //while (canScroll)
            //{
            //    // Scroll down using the appropriate method provided by TestStack.White
            //    scrollBar.ScrollDown();

            //    // Check if we can still scroll down
            //    canScroll = scrollBar.IsScrollable;

            //    // Sleep for a short duration to allow the UI to update
            //    Thread.Sleep(500); // Adjust sleep duration as needed
            //}






            //var scrollBar = mypostsWindow.ScrollBars.Vertical;
            //scrollBar.ScrollDownLarge();


            //Thread.Sleep(2000);

            // Scroll to the maximum position to go to the bottom
            //verticalScrollBar.ScrollDown(ScrollAmount.SmallIncrement);

            //System.Windows.Point referenceElementPosition = mypostsItemWindow.Bounds.Location;

            //// Calculate the target coordinates relative to the reference element
            ////double offsetX = 20; // Adjust this value as needed
            ////double offsetY = 10; // Adjust this value as needed
            //Thread.Sleep(2000);

            //double targetXXX = referenceElementPosition.X;
            //double targetYYY = referenceElementPosition.Y;

            //Thread.Sleep(2000);

            //// Simulate a mouse click on the specified coordinates
            //AttachedMouse deleteMouseOkButton = mypostsWindow.Mouse;

            //Thread.Sleep(500);

            //deleteMouseOkButton.Click(new System.Windows.Point(targetXXX, targetYYY));





            //Window mypostsItemWindow = wpfApp.GetWindow("MyPostsItem", InitializeOption.NoCache);
            //Assert.IsNotNull(mypostsItemWindow, "Page not found");

            //var postContentLabel = mypostsItemWindow.Get<TestStack.White.UIItems.Label>(SearchCriteria.ByAutomationId("lookingFor"));
            //Assert.IsNotNull(postContentLabel, "Post content label not found");

            //// Then you can assert the text of the post content label to match the expected post content
            //Assert.AreEqual("I'm looking for Jane Austen books", postContentLabel.Text, "Post content doesn't match expected");



            //var myItemPosted = mypostsItemWindow.Get<Text>(SearchCriteria.ByText("My Posts"));


            wpfApp.Kill();

        }





        /// </summary>
        /// This test will verify that the users will have to fill out all the information in order to be able to post 
        /// and they cannot leave any of the fields blank
        /// </summary>

        [TestMethod]
        public void TEST_SYS_32_ImageNotIncluded()
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

            Window postLogInPage = wpfApp.GetWindow("NewHomeScreenPostLogin", InitializeOption.NoCache);
            Assert.IsNotNull(postLogInPage, "Page not found");



            Button createPostButton = postLogInPage.Get<Button>(SearchCriteria.ByText("Create Post"));

            createPostButton.Click();
            Thread.Sleep(500);

            // verifying the presence of the CreatePost window
            Window createPostPage = wpfApp.GetWindow("CreatePost", InitializeOption.NoCache);
            Assert.IsNotNull(createPostPage, "Page not found");




            // Filling out all the information in the create post tab
            var titleTextBox = createPostPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("titleTextBox"));
            Assert.IsNotNull(titleTextBox, "Title text box not found");
            titleTextBox.Enter("Harry Potter books");


            var locationTextBox = createPostPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("locationTextBox"));
            Assert.IsNotNull(locationTextBox, "Location text box not found");
            locationTextBox.Enter("Kitchener");

            var conditionTextBox = createPostPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("conditionTextBox"));
            Assert.IsNotNull(conditionTextBox, "Condition text box not found");
            conditionTextBox.Enter("Had them for 10 years but they look new");


            var worthTextBox = createPostPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("worthTextBox"));
            Assert.IsNotNull(worthTextBox, "worth text box not found");
            worthTextBox.Enter("About $200");


            var deliveryTextBox = createPostPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("deliveryTextBox"));
            Assert.IsNotNull(deliveryTextBox, "worth text box not found");
            deliveryTextBox.Enter("Pick-up");


            var lookingForTextBox = createPostPage.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByAutomationId("LookingForTextBox"));
            Assert.IsNotNull(lookingForTextBox, "worth text box not found");
            lookingForTextBox.Enter("I'm looking for Jane Austen books");



            //Finding and clicking the sign up button within the sign up window
            Button postingButton = createPostPage.Get<Button>(SearchCriteria.ByAutomationId("PostClick"));
            postingButton.Click();

            Thread.Sleep(500);


            // Seeing the dialog box that says the user should fill out all the information
            Window messageBox = createPostPage.MessageBox("");

            Assert.IsNotNull(messageBox, "Message box not found");

            var label = messageBox.Get<Label>(SearchCriteria.Indexed(0));
            Assert.AreEqual("Please Fill all the details!!", label.Text);













            //// Adding the image of the item by browsing the file explorer

            //// coordinates of where to click on the sign up page to access the file explorer 
            //int targetX = 730;
            //int targetY = 365;

            //// Simulate a mouse click on the specified coordinates
            //AttachedMouse attachedMouse = createPostPage.Mouse;

            //Thread.Sleep(1000);

            //attachedMouse.Click(new System.Windows.Point(targetX, targetY));


            //// Waiting for the file dialog window to appear
            //var fileExplorerWindow = createPostPage.ModalWindow("Open");

            //Assert.IsNotNull(fileExplorerWindow);

            //// Finding and selecting the desired image file by entering its file name
            //var addressBarTextBox = fileExplorerWindow.Get<TestStack.White.UIItems.TextBox>(SearchCriteria.ByText("File name:"));

            //addressBarTextBox.Enter("TestImage6.jpeg");   // Resulotuion: 3000 x 2050  Size: 1.94 MB

            //var openButton = fileExplorerWindow.Get<Button>(SearchCriteria.ByText("Open"));
            //openButton.Click();


            ////Finding and clicking the sign up button within the sign up window
            //Button postingButton2 = createPostPage.Get<Button>(SearchCriteria.ByAutomationId("PostClick"));
            //postingButton2.Click();

            //Thread.Sleep(500);

            //Window messageBox = createPostPage.MessageBox("");

            //Assert.IsNotNull(messageBox, "Message box not found");

            //var label = messageBox.Get<Label>(SearchCriteria.Indexed(0));
            //Assert.AreEqual("Item successfully Posted!! You can now view your post from 'My Posts' tab.", label.Text);

            //Thread.Sleep(500);

            //    wpfApp.Kill();

            //}














            /// This test also ensures that the client and server can handle images that are larger than 1 MB







            /// </summary>
            /// This test will verify that the users will have to fill out all the information in order to be able to post 
            /// and they cannot leave any of the fields blank
            /// </summary>









            /// <summary>
            /// This test verifies that the users are limited to using JPEG files and if they try to drag and drop another type of file,
            /// they will be prevented from uploading it
            /// </summary>
            // IMPORTANT NOTE: IN ORDER TO RUN THIS TEST YOU CAN COPY 'testImage.jpeg' FROM THE 'Test_Images' directory
            // in the project directory AND PASTE IT IN 'DOWNLOAD' DIRECTORY OF YOUR LAPTOP

        }






        }
}


