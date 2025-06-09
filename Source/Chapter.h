#pragma once
#include "Slide.h"
#include "Json.h"
#include <map>

class Chapter
{
public:
	Chapter();
	~Chapter() {}

	////std::map<std::string, std::shared_ptr<Character>> GetUsedCharacters() { return m_usedCharacters; }
	//std::vector<std::shared_ptr<Character>>& GetUsedCharacters() { return m_usedCharacters; }

	std::vector<Slide>& GetSlides() { return m_slides; }

	template<class T>
	void serialize(T& archive)
	{
		archive(CEREAL_NVP(m_slides));
	}

	static void LoadSlideValue(const char* filename, Chapter& chapter)
	{
		std::ifstream ifs(filename);
		cereal::JSONInputArchive archive(ifs);
		archive(CEREAL_NVP(chapter));
	}

	static void SaveSlideValue(const char* filename, Chapter& chapter)
	{
		std::ofstream ofs(filename);
		cereal::JSONOutputArchive archive(ofs);
		archive(CEREAL_NVP(chapter));
	}
protected:
	std::vector<Slide> m_slides;
};