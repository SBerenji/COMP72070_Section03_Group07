using Swapify_Server.Models;
using Swapify_Server.Services;
using Microsoft.AspNetCore.Mvc;
using BCrypt.Net;


namespace Swapify_Server.Controllers
{
    public class UserController : ControllerBase
    {
        private readonly UserService _userService;
        private readonly EmailSenderService _emailSenderService;
        
        public UserController(UserService userService)
        {
            _userService = userService;
            _emailSenderService = new EmailSenderService();
        }

        [HttpPost]
        [Route("user/register")]
        public async Task<IActionResult> RegisterUser([FromBody] User user)
        {
            try
            {
                var existingUser = await _userService.GetUserByEmailAsync(user.Email);

                if (existingUser != null)
                {
                    return StatusCode(409, new { Message = "User already exists" });
                }
                else
                {
                    user.DateCreated = DateTime.Now;
                    user.AccountState = AccountState.Registered;

                    user.Password = BCrypt.Net.BCrypt.HashPassword(user.Password);

                    await _emailSenderService.SendEmailAsync(user.Email, "Swapify Registration", $"Thank you for registering with Swapify! Your verification code is: {user.VerificationCode}");


                    await _userService.SaveUserAsync(user);
                    return StatusCode(201, new { Message = "User created successfully" });
                }
            }
            catch (Exception ex)
            {
                return StatusCode(500, new { Message = $"Error: {ex.Message}" });
            }
        }

        [HttpPost]
        [Route("user/verify")]
        public async Task<IActionResult> VerifyUser([FromBody] ApiRequestPacket apiRequest)
        {
            try
            {
                var user = apiRequest.User;
                var existingUser = await _userService.GetUserByEmailAsync(user.Email);

                if (existingUser == null)
                {
                    return StatusCode(404, new { Message = "User not found" });
                }
                else
                {
                    if (existingUser.VerificationCode == user.VerificationCode)
                    {
                        existingUser.AccountState = AccountState.Verified;
                        await _userService.UpdateUserAsync(existingUser.Id, existingUser);
                        return StatusCode(200, new { Message = "User verified successfully" });
                    }
                    else
                    {
                        return StatusCode(401, new { Message = "Invalid verification code" });
                    }
                }
            }
            catch (Exception ex)
            {
                return StatusCode(500, new { Message = $"Error: {ex.Message}" });
            }
        }

        [HttpPost]
        [Route("user/login")]
        public async Task<IActionResult> LoginUser([FromBody] ApiRequestPacket apiRequest)
        {
            try
            {
                var user = apiRequest.User;
                var existingUser = await _userService.GetUserByEmailAsync(user.Email);

                if (existingUser == null)
                {
                    return StatusCode(404, new { Message = "User not found" });
                }
                else
                {
                    if (BCrypt.Net.BCrypt.Verify(user.Password, existingUser.Password))
                    {
                        if(existingUser.AccountState != AccountState.Verified)
                            existingUser.AccountState = AccountState.Verified;
                           
                        return StatusCode(200, new { Message = "User logged in successfully" });
                    }
                    else
                    {
                        return StatusCode(401, new { Message = "Invalid password" });
                    }
                }
            }
            catch (Exception ex)
            {
                return StatusCode(500, new { Message = $"Error: {ex.Message}" });
            }
        }

        //User Must provide password to deactivate account
        [HttpPost]
        [Route("user/Deactivate")] 
        public async Task<IActionResult> DeactivateUser([FromBody] ApiRequestPacket apiRequest)
        {
            try
            {
                var user = apiRequest.User;
                var existingUser = await _userService.GetUserByEmailAsync(user.Email);

                if (existingUser == null)
                {
                    return StatusCode(404, new { Message = "User not found" });
                }
                else
                {
                    if (BCrypt.Net.BCrypt.Verify(user.Password, existingUser.Password))
                    {
                        existingUser.AccountState = AccountState.Deactivated;
                        await _userService.UpdateUserAsync(existingUser.Id, existingUser);
                        return StatusCode(200, new { Message = "User deactivated successfully" });
                    }
                    else
                    {
                        return StatusCode(401, new { Message = "Invalid password" });
                    }
                }
            }
            catch (Exception ex)
            {
                return StatusCode(500, new { Message = $"Error: {ex.Message}" });
            }
        }

        //User must provide password to close account
        //User may re-open account by logging in
        //A cron job will be setup to delete closed accounts after 7 days
        [HttpPost]
        [Route("user/Close")]
        public async Task<IActionResult> CloseUser([FromBody] ApiRequestPacket apiRequest)
        {
            try
            {
                var user = apiRequest.User;
                var existingUser = await _userService.GetUserByEmailAsync(user.Email);

                if (existingUser == null)
                {
                    return StatusCode(404, new { Message = "User not found" });
                }
                else
                {
                    if (BCrypt.Net.BCrypt.Verify(user.Password, existingUser.Password))
                    {
                        existingUser.AccountState = AccountState.Closed;
                        await _userService.UpdateUserAsync(existingUser.Id, existingUser);
                        return StatusCode(200, new { Message = "User closed successfully" });
                    }
                    else
                    {
                        return StatusCode(401, new { Message = "Invalid password" });
                    }
                }
            }
            catch (Exception ex)
            {
                return StatusCode(500, new { Message = $"Error: {ex.Message}" });
            }
        }

        [HttpGet]
        [Route("user/GetUserById")]
        public async Task<IActionResult> GetUser([FromBody] ApiRequestPacket apiRequest)
        {
            try
            {
                var userToGet = apiRequest.RequestedUserId;
                Console.WriteLine("here");
                var user = await _userService.GetUserByIdAsync(userToGet);

                if (user == null)
                {
                    return StatusCode(404, new { Message = "User not found" });
                }
                else
                {
                    return StatusCode(200, new { Message = "User found", User = user });
                }
            }
            catch (Exception ex)
            {
                return StatusCode(500, new { Message = $"Error: {ex.Message}" });
            }
        }

        [HttpGet]
        [Route("user/TestRoute")]
        public async Task<IActionResult> TestRoute()
        {
            try
            {
                return StatusCode(200, new { Message = "Test route successful" });
            }
            catch (Exception ex)
            {
                return StatusCode(500, new { Message = $"Error: {ex.Message}" });
            }
        }

    }
}
