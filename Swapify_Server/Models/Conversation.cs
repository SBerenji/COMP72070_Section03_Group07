using MongoDB.Bson.Serialization.Attributes;
using MongoDB.Bson;

namespace Swapify_Server.Models
{
    public class Conversation
    {

        [BsonId]
        [BsonRepresentation(BsonType.ObjectId)]
        public string Id { get; set; } = string.Empty;

        [BsonElement("Users")]
        public String[] Users { get; set; } = new string[2];

        [BsonElement("Messages")]
        public List<String> Messages { get; set; } = new List<String>();
    }
}
