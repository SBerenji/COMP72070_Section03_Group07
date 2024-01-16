using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;
using static System.Net.WebRequestMethods;

namespace Swapify_Server.Models
{

    public enum AccountState
    {
        Registered,
        Verified,
        Deactivated,
        Closed
    }

   

    public class User
    {

        

        [BsonId]
        [BsonRepresentation(BsonType.ObjectId)]
        public string Id { get; set; } = string.Empty;

        [BsonElement("FirstName")]
        public string FirstName { get; set; } = string.Empty;

        [BsonElement("LastName")]
        public string LastName { get; set; } = string.Empty;

        [BsonElement("Email")]
        public string Email { get; set; } = string.Empty;

        [BsonElement("Password")]
        public string Password { get; set; } = string.Empty;

        [BsonElement("VerificationCode")]
        public string VerificationCode { get; set; } = GenerateVerificationCode();

        [BsonElement("ProfilePictureId")]
        public string ProfilePictureId { get; set; } = string.Empty;

        [BsonElement("DateCreated")]
        public DateTime DateCreated { get; set; }

        [BsonElement("AccountState")]
        public AccountState AccountState { get; set; }
        private static string GenerateVerificationCode()
        {
            Random random = new Random();
            int verificationNumber = random.Next(100000, 999999);
            return verificationNumber.ToString();
        }
    }
}
