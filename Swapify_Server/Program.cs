using Swapify_Server.Models;
using Microsoft.Extensions.Options;
using MongoDB.Driver;
using Swapify_Server.Services;

var builder = WebApplication.CreateBuilder(args);


// Add CORS policy
var origin = "_origin";
builder.Services.AddCors(options =>
{
    options.AddDefaultPolicy(builder =>
    {
        builder
            .WithOrigins("http://localhost:5000")
            .AllowAnyMethod()
            .AllowAnyHeader()
            .AllowCredentials();
    });
});

var mongoDbSettings = new MongoDbSettings();
builder.Configuration.Bind("MongoDbSettings", mongoDbSettings);
builder.Services.AddSingleton(mongoDbSettings); builder.Services.AddScoped<UserService>();
builder.Services.AddControllers();
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddHttpContextAccessor();
builder.Services.AddDistributedMemoryCache();

var app = builder.Build();

app.UseStaticFiles();

app.UseRouting();
app.UseCors();

app.MapControllers();

app.Run("http://localhost:8000");


