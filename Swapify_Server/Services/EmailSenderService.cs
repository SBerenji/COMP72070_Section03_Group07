using System.Net;
using System.Net.Mail;

namespace Swapify_Server.Services
{
    //todo make email account
    public class EmailSenderService : IEmailSenderService
    {
        public Task SendEmailAsync(string email, string subject, string message)
        {
            //E-Mail Sender
            var mail = "";

            var password = "";
            var client = new SmtpClient("smtp-mail.outlook.com")
            {
                Port = 587,
                DeliveryMethod = SmtpDeliveryMethod.Network,
                UseDefaultCredentials = false,
                Credentials = new NetworkCredential(mail, password),
                EnableSsl = true,
            };

            return client.SendMailAsync(
                new MailMessage(mail, email, subject, message)
                {
                    IsBodyHtml = true
                }
            );    
        }
    }
}
