using MongoDB.Bson.Serialization.Attributes;
using MongoDB.Bson;
using System.Text.RegularExpressions;

namespace Swapify_Server.Models
{

    public enum Category
    {
        ClothingAndAccessories,
        ElectronicsAndGadgets,
        HomeAndDecor,
        BooksAndMedia,
        SportsAndOutdoorEquipment,
        ToysAndGames,
        Furniture,
        ArtAndCrafts,
        KitchenwareAndAppliances,
        ToolsAndDIYSupplies,
        MusicalInstruments,
        BeautyAndPersonalCare,
        PetSupplies,
        PlantsAndGardening,
        BoardGamesAndPuzzles,
        CollectiblesAndMemorabilia,
        FitnessEquipment,
        BabyAndKidsItems
    }

    public enum ItemCondition
    {
        New,
        LikeNew,
        Good,
        Fair
    }

    public enum ListingStatus
    {
        Active,
        Inactive,
        Sold
    }

    public class ListingLocation
    {
        public string Country { get; set; } = "Canada";
        public string Province { get; set; } = string.Empty;
        public string City { get; set; } = string.Empty;
        public string PostalCode { get; set; } = string.Empty;

        public static string VerifyAndConvertToUpperCase(string postalCode)
        {
            string pattern = @"^[A-Za-z]\d[A-Za-z] \d[A-Za-z]\d$";

            Regex regex = new Regex(pattern);
            if(regex.IsMatch(postalCode))
                return postalCode.ToUpper();
            else
                return null;          
        }
    }

    public class Listing
    {
        [BsonId]
        [BsonRepresentation(BsonType.ObjectId)]
        public string Id { get; set; } = string.Empty;

        [BsonElement("Seller")]
        public string Seller { get; set; } = string.Empty;

        [BsonElement("Title")]
        public string Title { get; set; } = string.Empty;

        [BsonElement("Description")]
        public string Description { get; set; } = string.Empty;

        [BsonElement("Category")]
        public Category Category { get; set; }

        [BsonElement("Condition")]
        public ItemCondition Condition { get; set; }

        [BsonElement("Images")]
        public List<String> Images { get; set; } = new List<String>();

        [BsonElement("Value")]
        public double Value { get; set; } = 0.00;

        [BsonElement("ListingDate")]
        public DateTime ListingDate { get; set; } = DateTime.Now;

        [BsonElement("Location")]
        public ListingLocation Location { get; set; }

        [BsonElement("Status")]
        public ListingStatus Status { get; set; }

    }
}
