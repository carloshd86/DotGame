#ifndef __PROPERTIES_H__
#define __PROPERTIES_H__


#include <string>
#include <map>


class Properties
{
public:

	enum P_Language
	{
		Spanish,
		English
	};

	~Properties();
	
	static Properties * Instance       (const char * file, const P_Language lang);
	static void         RemoveInstance ();

	const std::string& GetProperty(const std::string& key);


private:

	Properties(P_Language lang);

	static Properties *mInstance;

	const P_Language                    mLang;
	std::map<std::string, std::string> mTexts;

	static const char * GetLanguageSuffix(const P_Language lang);
};

#endif