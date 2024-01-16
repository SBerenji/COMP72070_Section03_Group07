namespace Swapify_Server.Models
{
    public class ApiRequestPacket
    {
        public string RequestedUserId { get; set; } = string.Empty;
        public User User { get; set; } = new User();
        public string ListingId { get; set; } = string.Empty;

    }
}
