#include "properties.h"
#include "asserts.h"
#include "memorycontrol.h"
#include "globals.h"
#include <fstream>
#include <algorithm>


Properties *  Properties::mInstance = nullptr;

// *************************************************
//
// *************************************************

Properties::Properties(const P_Language lang) :
	mLang (lang) {}

// *************************************************
//
// *************************************************

Properties::~Properties()
{
}

// *************************************************
//
// *************************************************

Properties * Properties::Instance(const char * file, const P_Language lang)
{
	if (mInstance && lang == mInstance->mLang) return mInstance;

	GAME_DELETE(mInstance);
	mInstance = nullptr;

	GAME_ASSERT(file);
	const char * languageSuffix = GetLanguageSuffix(lang);
	GAME_ASSERT(languageSuffix);

	mInstance = GAME_NEW(Properties, (lang));

	std::string propertiesSrc = DATA_FOLDER;
	propertiesSrc.append(file);
	propertiesSrc.append("_");
	propertiesSrc.append(languageSuffix);
	propertiesSrc.append(".properties");

	std::ifstream propertiesFile(propertiesSrc, std::ios::binary);
	GAME_ASSERT(propertiesFile.is_open());

	std::string line;
	std::string key;
	std::string value;
	while (!propertiesFile.eof())
	{
		std::getline(propertiesFile, line);
		size_t index = line.find('=', 0);
		key = line.substr(0, index - 1);
		value = line.substr(index + 1);
		value.erase(std::remove(value.begin(), value.end(), '\r'), value.end());
		value.erase(std::remove(value.begin(), value.end(), '\n'), value.end());
		mInstance->mTexts.insert(make_pair(key, value));
	}

	return mInstance;
}

// *************************************************
//
// *************************************************

void Properties::RemoveInstance()
{
 GAME_DELETE(	mInstance);
 mInstance = nullptr;
}

// *************************************************
//
// *************************************************

const std::string& Properties::GetProperty(const std::string& key) 
{
	if (mTexts.end() != mTexts.find(key)) return mTexts[key];
	else                                  return key;
}

// *************************************************
//
// *************************************************

const char * Properties::GetLanguageSuffix(const P_Language lang)
{
	switch (lang)
	{
		case Spanish: return "es";
		case English: return "en";
	}

	return nullptr;
}