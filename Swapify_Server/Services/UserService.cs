using Microsoft.Extensions.Options;
using MongoDB.Driver;
using Swapify_Server.Models;
using System;
namespace Swapify_Server.Services
{
    public class UserService : IUserService
    {
        private readonly IMongoCollection<User> _userCollection;

        public UserService(MongoDbSettings mongoDbSettings)
        {
            var mongoClient = new MongoClient(mongoDbSettings.ConnectionString);
            var database = mongoClient.GetDatabase(mongoDbSettings.DatabaseName);
            _userCollection = database.GetCollection<User>(mongoDbSettings.UserCollectionName);
        }

        async public Task<List<User>> GetUsersAsync()
        {
            return await _userCollection.Find(user => true).ToListAsync();
        }

        async public Task<User> SaveUserAsync(User user)
        {
            return await _userCollection.InsertOneAsync(user).ContinueWith(task => user);
        }

        async public Task<User> GetUserByIdAsync(string id)
        {
            return await _userCollection.Find(user => user.Id == id).FirstOrDefaultAsync();
        }

        async public Task<User> GetUserByEmailAsync(string email)
        {
            return await _userCollection.Find(user => user.Email == email).FirstOrDefaultAsync();
        }

        async public Task<User> UpdateUserAsync(string id, User user)
        {
            return await _userCollection.ReplaceOneAsync(user => user.Id == id, user).ContinueWith(task => user);
        }
    }
}
