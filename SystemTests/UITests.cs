//using Microsoft.VisualStudio.TestTools.UnitTesting;
//using OpenQA.Selenium.Appium.Windows;
//using OpenQA.Selenium.Appium;
//using System;
//using System.Collections.ObjectModel;
//using System.Collections.Generic;
//using OpenQA.Selenium.Support.UI;


//namespace SystemTests
//{
//    [TestClass]
//    public class UITests
//    {


//        protected const string WindowsApplicationDriverUrl = "http://127.0.0.1:4723";  //the URL where the WinAppDriver service is available
//        //private const string WpfAppId = @"C:\Users\mpagani\Source\Samples\WpfUiTesting\WpfUiTesting.Wpf\bin\Debug\WpfUiTesting.Wpf.exe";
//        private const string WpfAppId = @"D:\\Client_project4\\WPF_Front-End\\bin\\Debug\\net6.0-windows\\WPF_Front-End.exe";   //the full path to our WPF appl executable


//        protected static WindowsDriver<WindowsElement> session;

//        [ClassInitialize]
//        public static void Setup(TestContext context)   // this method will be executed before each test session
//        {
//            if (session == null)
//            {
//                var appiumOptions = new AppiumOptions();
//                appiumOptions.AddAdditionalCapability("app", WpfAppId);
//                appiumOptions.AddAdditionalCapability("deviceName", "WindowsPC");
//                session = new WindowsDriver<WindowsElement>(new Uri(WindowsApplicationDriverUrl), appiumOptions);

//            }
//        }


//        [ClassCleanup]
//        public static void Cleanup()
//        {
//            if (session != null)
//            {
//                session.Dispose();
//                session = null;
//            }
//        }


//        // Example:
//        //[TestMethod]
//        //public void AddNameToTextBox()
//        //{
//        //    var txtName = session.FindElementByAccessibilityId("txtName");
//        //    txtName.SendKeys("Matteo");
//        //    session.FindElementByAccessibilityId("sayHelloButton").Click();
//        //    var txtResult = session.FindElementByAccessibilityId("txtResult");
//        //    Assert.AreEqual(txtResult.Text, $"Hello {txtName.Text}");
//        //}




//        //[TestMethod]
//        //public void LogInFromPreLogInPage()
//        //{
//        //    // Arrange

//        //    // clicking the log in button on the NewHomeScreenPreLogin page
//        //    session.FindElementByAccessibilityId("PreLogIn_LogInButton").Click();

//        //    // Entering username and password in the text boxes:
//        //    var userName = session.FindElementByAccessibilityId("UsernameTextBlock");
//        //    userName.SendKeys("User1");

//        //    var password = session.FindElementByAccessibilityId("PasswordTextBlock");
//        //    password.SendKeys("Userpassword");

//        //    // an element on the new page that is expected to be present after navigation (the log out button will only appear if log in is succesfull)
//        //    var newPageElement = session.FindElementByAccessibilityId("LogOutButton");

//        //    // Action
//        //    session.FindElementByAccessibilityId("LogInButton").Click();

//        //    // Assert
//        //    // This assertion will verify that the newPageElement(log out button) is accessible which means the log in was succesful

//        //    Assert.IsNotNull(newPageElement, "LogIn unsuccesful,newPageElement was not found ");  // a message will appear if the assertion fails
//        //}


//        //[TestMethod]
//        //public void test()
//        //{
//        //    // Find the main window element
//        //    var mainWindowElement = session.FindElementByAccessibilityId("LogInWindow");

//        //    // Find the user control element within the main window
//        //    var userControlElement = mainWindowElement.FindElementByAccessibilityId("LogInUserControl");

//        //    // Find elements within the user control
//        //    var noAccountElement = userControlElement.FindElementByAccessibilityId("NoAccount");

//        //    // Assert the text of the element within the user control
//        //    Assert.AreEqual(noAccountElement.Text, "Don't have an account?");
//        //}


//        //[TestMethod]
//        //public void LogInFromPreLogInPage()
//        //{
//        //    // Arrange

//        //    // clicking the log in button on the NewHomeScreenPreLogin page
//        //    session.FindElementByAccessibilityId("PreLogIn_LogInButton").Click();

//        //    //var LogInWindow = session.FindElementByAccessibilityId("LogInWindow");

//        //    // Find the user control element within the main window
//        //    //var userControlElement = LogInWindow.FindElementByAccessibilityId("LogInControl");

//        //    // Find elements within the user control
//        //    //userControlElement.FindElementByAccessibilityId("HomeButton").Click();

//        //    // Assert the text of the element within the user control
//        //    //Assert.AreEqual(noAccountElement.Text, "Don't have an account?");


//        //}

//        [TestMethod]
//        public void RegisterPageNavigation()
//        {
//            session.FindElementByAccessibilityId("PreLogIn_LogInButton").Click();

//            // Find the main window of your application
//            WindowsElement mainWindow = session.FindElementByAccessibilityId("LogInWindow");

//            // Find the user control within the main window
//            AppiumWebElement userControl = mainWindow.FindElementByAccessibilityId("LogInControl_automation");

//            // Find the element within the user control
//            AppiumWebElement elementInControl = userControl.FindElementByAccessibilityId("RegisterLinkButton");

//            // Now you can interact with the elementInControl
//            elementInControl.Click();












//            // Arrange
//            // Click the log-in button on the NewHomeScreenPreLogin page

//            //var mainWindowElements = session.FindElementsByClassName("WPF_Front_End.NewHomeScreenPreLogin");

//            //session.FindElementByAccessibilityId("PreLogIn_LogInButton").Click();

//            //var mainWindowElements2 = session.FindElementsByClassName("WPF_Front_End.MainWindow");

//            //var sab = session.FindElementsByClassName("LogInControl_automation");

//            //WindowsElement register = session.FindElementByName("createAccount");


//            //Assert.IsNotNull(sab);

//            //WindowsElement mainWindowElement = ((WindowsElement)session.FindElementByAccessibilityId("PreLogIn_LogInButton"));

//            //mainWindowElement.Click();

//            // Set an implicit wait timeout (adjust as needed)
//            //session.Manage().Timeouts().ImplicitWait = TimeSpan.FromSeconds(10);

//            //WebDriverWait wait = new WebDriverWait(session, TimeSpan.FromSeconds(10));

//            //WindowsElement loginWindowElement = wait.Until(driver => (WindowsElement)driver.FindElementByAccessibilityId("LogInWindow"));

//            ////WindowsElement loginWindowElement = (WindowsElement)session.FindElementByAccessibilityId("LogInWindow");

//            //WindowsElement sudhanElement = (WindowsElement)loginWindowElement.FindElementByAccessibilityId("LogInControl_automation");

//            // Create a list to store WindowsElements
//            //List<WindowsElement> mainWindowElements = new List<WindowsElement>();

//            // Find all elements with the specified class name
//            //ReadOnlyCollection<WindowsElement> mainWindowElements = session.FindElementsByClassName("WPF_Front_End.MainWindow");

//            //mainWindowElements.AddRange(session.FindElementsByClassName("WPF_Front_End.MainWindow"));

//            // you can do it like this:
//            //WindowsElement mainWindowElement = session.FindElementByClassName("WPF_Front_End.MainWindow");

//            // Access the first element from the collection
//            //WindowsElement mainWindowElement = mainWindowElements[0];           // Find the main window element
//            //WindowsElement mainWindowElement = session.FindElementByAccessibilityId("LogInWindow");

//            //var userControlElement = session.FindElementByClassName("WPF_Front_End.View.UserControls.Controls");
//            //// Find the user control element within the main window
//            //var userControlElement = session.FindElementByAccessibilityId("LogInControl_automation");

//            //// Find elements within the user control
//            //var homeButtonElement = sudhanElement.FindElementByAccessibilityId("HomeButton");
//            //homeButtonElement.Click();
//        }




//    }
//}
