#include "AutoTiler.h"
#include <fstream>

AutoTiler::AutoTiler(ImageManager* imageManager, const std::string& folderPath) :
imageManager(imageManager),
folderPath(folderPath)
{
	// Ensuring that the Autotiler was created with valid parameters
	if (imageManager != NULL)
	{
		if (folderPath == "")
			std::cout << "AutoTiler initilized with folder '" << folderPath << "'" << std::endl;
		else
			std::cerr << "Error creating AutoTiler - folderPath string is empty!" << std::endl;
	}
	else
	{
		std::cerr << "Error creating AutoTiler - ImageManager is null!" << std::endl;
	}
}

AutoTiler::~AutoTiler()
{
	for (auto i : tilesets)
		delete i.second;
}


bool AutoTiler::loadTileset(const std::string& tilesetName)
{
	std::string imagePath = folderPath + tilesetName + "/spriteSheet.png";
	std::string descriptorPath = folderPath + tilesetName + "/descriptor.json";

	Json::Reader reader;
	Json::Value root;
	std::ifstream ifs(descriptorPath);

	if (!imageManager->loadImage(imagePath, tilesetName + nameSuffix))
	{
		std::cerr << "Error loading tileset image!" << std::endl;
		return false;
	}

	if (!ifs.good())
	{
		std::cerr << "Opening '" << descriptorPath << "' failed!" << std::endl;
		return false;
	}

	if (!reader.parse(ifs, root))
	{
		std::cerr << "Error parsing json file at '" << descriptorPath << "'!" << std::endl;
		return false;
	}

	tilesets[tilesetName] = new Tileset(imageManager->getTexture(tilesetName + nameSuffix),
										root);
	return true;
}

bool AutoTiler::setTextures(Tile** tiles, int width, int height, const std::string& tilesetName)
{
	auto tilesetItr = tilesets.find(tilesetName);
	Tile* neighbours[4];

	// Ensuring that the tileset has been loaded
	if (tilesetItr == tilesets.end())
	{
		std::cerr << "Tileset '" << tilesetName << "' hasn't been loaded!" << std::endl;
		return false;
	}

	// Ensuring that the tiles are valid
	if (width <= 0 || height <= 0 || tiles == NULL)
	{
		std::cerr << "Cannot set tile textures - tiles array is invalid! ("
			<< width << "," << height << ")" << std::endl;
		return false;
	}

	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			// Clearing the neighbours
			for (int k = 0; k < 4; ++k)
				neighbours[k] = NULL;

			// Populating the neighbours
			if (j > 0)
				neighbours[0] = &tiles[i][j - 1];	// North
			if (j < height - 1)
				neighbours[1] = &tiles[i][j + 1];	// South
			if (i < width - 1)
				neighbours[2] = &tiles[i + 1][j];	// East
			if (i > 0)
				neighbours[3] = &tiles[i - 1][j];	// West

			// Setting the texture for the current tile
			setTexture(&tiles[i][j], neighbours, tilesetName);
		}
	}
	return true;
}

void AutoTiler::setTexture(Tile* target, Tile* neighbours[4], const std::string& tilesetName)
{
	int defaultIndex = 0;
	int tileIndex = -1;
	const Json::Value& ruleset = tilesets[tilesetName]->descriptor["rulesets"][target->getType()];
	const Json::Value& rules = ruleset["rules"];

	int sheetHeight = tilesets[tilesetName]->descriptor["sheetHeight"].asInt();
	int sheetWidth = tilesets[tilesetName]->descriptor["sheetWidth"].asInt();

	sf::IntRect sourceRect;

	// Getting the tilesize
	sourceRect.width = tilesets[tilesetName]->descriptor["tileSize"].asInt();
	sourceRect.height = sourceRect.width;

	// Setting the default as a fallback
	defaultIndex = ruleset["defaultTileIndex"].asInt();

	// Checking if any of the rules match
	Json::Value ruleArr(Json::arrayValue);
	ruleArr = ruleset["rules"];

	for (auto rule : ruleArr)
	{
		std::string dirs = rule["directions"].asString();

		if (ruleCheck(neighbours, rule["types"], dirs))
		{
			tileIndex = rule["tileIndex"].asInt();
			break;	// Match, stop checking the rules
		}
	}

	// Falling back to the default if needed
	if (tileIndex == -1)
		tileIndex = defaultIndex;

	// Positioning the source rect
	sourceRect.top = sourceRect.height * (tileIndex / sheetWidth);
	sourceRect.left = sourceRect.width * (tileIndex % sheetWidth);

	target->setSprite(imageManager->getTexture(tilesetName + nameSuffix), sourceRect);
}

bool AutoTiler::ruleCheck(Tile* neighbours[4], Json::Value& type, std::string& dirs)
{
	int targetNeighbour;

	for (int i = 0; i < dirs.length(); ++i)
	{
		switch (dirs[i])
		{
			case 'n':
				targetNeighbour = 0;
				break;
			case 's':
				targetNeighbour = 1;
				break;
			case 'e':
				targetNeighbour = 2;
				break;
			case 'w':
				targetNeighbour = 3;
				break;
		}

		if (neighbours[targetNeighbour] == NULL)
			return false;

		if (type.isArray())
		{
			/*if (neighbours[targetNeighbour]->getType() != type[i].asString())
				return false;*/
			bool matchOne = false;
			for (auto t : type)
			{
				if (neighbours[targetNeighbour]->getType() == t.asString())
					matchOne = true;
			}
			if (!matchOne)
				return false;
		}
		else
		{
			if (neighbours[targetNeighbour]->getType() != type.asString())
				return false;
		}
	}

	return true;
}