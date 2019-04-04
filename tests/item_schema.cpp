#include "catch.hpp"
#include <json/json.h>
#include <string>
#include <vector>
#include <fstream>

static std::vector<std::string> requiredWeaponFields = {
	"name", "description", "icon", "power", "range"
};

static std::vector<std::string> requiredTrinketFields = {
	"name", "description", "icon"
};

static std::vector<std::string> requiredArmourFields = {
	"name", "description", "icon", "damageReduction"
};

static std::string jsonLoc = "data/items.json";

bool meetsSchema(Json::Value val, std::vector<std::string>& schema)
{
	std::vector<std::string> validatedFields;
	for(auto itr = val.begin(); itr != val.end(); ++itr)
	{
		std::string key = itr.memberName();
		for(int i = 0; i < schema.size(); ++i)
		{
			if(schema[i] == key && 
				std::find(validatedFields.begin(), validatedFields.end(), key) == validatedFields.end())
				validatedFields.push_back(key);
		}
	}

	return(validatedFields.size() == schema.size());
}

bool isNotNull(Json::Value val) { return val != Json::nullValue; }

Json::Value getAll(const char* type)
{
	std::ifstream ifs(jsonLoc);
	Json::Reader reader;
	Json::Value root;
	reader.parse(ifs, root);
	return root["data"][type];
}

TEST_CASE("weapons json is non-null", "[schema]")
{
	REQUIRE(getAll("Weapons") != Json::nullValue);
}

TEST_CASE("trinkets json is non-null", "[schema]")
{
	REQUIRE(getAll("Trinkets") != Json::nullValue);
}

TEST_CASE("armours json is non-null", "[schema]")
{
	REQUIRE(getAll("Armours") != Json::nullValue);
}

TEST_CASE("weapons are valid", "[schema]")
{
	Json::Value weapons = getAll("Weapons");

	for(auto weapon : weapons)
	{
		INFO(weapon["name"].asString());
		REQUIRE( isNotNull(weapon) );
		REQUIRE( meetsSchema(weapon, requiredWeaponFields) );
	}
}

TEST_CASE("trinkets are valid", "[schema]")
{
	Json::Value trinkets = getAll("Trinkets");

	for(auto trinket : trinkets)
	{
		INFO(trinket["name"].asString());
		REQUIRE( isNotNull(trinket) );
		REQUIRE( meetsSchema(trinket, requiredTrinketFields) );
	}
}

TEST_CASE("armours are valid", "[schema]")
{
	Json::Value armours = getAll("Armours");

	for(auto armour : armours)
	{
		INFO(armour["name"].asString());
		REQUIRE( isNotNull(armour) );
		REQUIRE( meetsSchema(armour, requiredTrinketFields) );
	}
}
