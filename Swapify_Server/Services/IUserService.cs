using Swapify_Server.Models;
using System.Net;

namespace Swapify_Server.Services
{
    public interface IUserService
    {
       Task<List<User>> GetUsersAsync();
    }
}
