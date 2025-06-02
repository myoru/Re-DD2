#pragma once
#include "Slide.h"
#include <map>

class Chapter
{
public:
	Chapter();
	~Chapter() {}

	////std::map<std::string, std::shared_ptr<Character>> GetUsedCharacters() { return m_usedCharacters; }
	//std::vector<std::shared_ptr<Character>>& GetUsedCharacters() { return m_usedCharacters; }
	std::vector<Slide>& GetSlides() { return m_slides; }
protected:
	//std::map<std::string, std::shared_ptr<Character>> m_usedCharacters;
	/*std::vector<std::shared_ptr<Character>> m_usedCharacters;*/
	std::vector<Slide> m_slides;
};