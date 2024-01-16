using MongoDB.Bson.Serialization.Attributes;
using MongoDB.Bson;

namespace Swapify_Server.Models
{

    public enum MessageStatus
    {
        Read,
        Unread
    }
    public class Message
    {
        [BsonId]
        [BsonRepresentation(BsonType.ObjectId)]
        public string Id { get; set; } = string.Empty;

        [BsonElement("SenderId")]
        public string SenderId { get; set; } = string.Empty;

        [BsonElement("RecipientId")]
        public string RecipientId { get; set; } = string.Empty;

        [BsonElement("ConversationId")]
        public string ConversationId { get; set; } = string.Empty;

        [BsonElement("MessageContent")]
        public string MessageContent { get; set; } = string.Empty;

        [BsonElement("TimeStamp")]
        public DateTime TimeStamp { get; set; } = DateTime.Now;

        [BsonElement("MessageStatus")]
        public MessageStatus MessageStatus { get; set; } = MessageStatus.Unread;
    }
}
