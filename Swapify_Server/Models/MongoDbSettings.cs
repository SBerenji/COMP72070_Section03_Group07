namespace Swapify_Server.Models
{
    public class MongoDbSettings
    {
        public string ConnectionString { get; set; } = String.Empty;
        public string DatabaseName { get; set; } = String.Empty;
        public string UserCollectionName { get; set; } = String.Empty;
    }
}
