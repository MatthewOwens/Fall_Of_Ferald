#include "catch.hpp"
#include <json/json.h>
#include <string>
#include <vector>
#include <fstream>

static std::vector<std::string> requiredFields = {
	"name", "description", "icon", "power", "range"
};

static std::string jsonLoc = "data/items.json";

bool meetsSchema(Json::Value weapon)
{
	std::vector<std::string> validatedFields;

	for(auto itr = weapon.begin(); itr != weapon.end(); ++itr)
	{
		std::string key = itr.memberName();
		for(int i = 0; i < requiredFields.size(); ++i)
		{
			if(requiredFields[i] == key && 
				std::find(validatedFields.begin(), validatedFields.end(), key) == validatedFields.end())
				validatedFields.push_back(key);
		}
	}

	return(validatedFields.size() == requiredFields.size());
}

bool isNotNull(Json::Value weapon) { return weapon != Json::nullValue; }

Json::Value getWeapons()
{
	std::ifstream ifs(jsonLoc);
	Json::Reader reader;
	Json::Value root;
	reader.parse(ifs, root);
	return root["data"]["Weapons"];
}

TEST_CASE("weapons json is non-null", "[weapon schema]")
{
	REQUIRE(getWeapons() != Json::nullValue);
}

TEST_CASE("weapons are valid", "[weapon schema]")
{
	Json::Value weapons = getWeapons();

	for(auto weapon : weapons)
	{
		REQUIRE( isNotNull(weapon) );
		REQUIRE( meetsSchema(weapon) );
	}
}
