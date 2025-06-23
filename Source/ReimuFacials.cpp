#include "ReimuFacials.h"
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <string>
#include <set>

namespace fs = std::filesystem;

ReimuFacials::ReimuFacials()
{
#if 1
	AddFacial(reinterpret_cast<const char*>(u8"’Êí"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Reimu2\\Normal.png"));
	AddFacial(reinterpret_cast<const char*>(u8"ÎŠç"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Reimu2\\Smile.png"));
	AddFacial(reinterpret_cast<const char*>(u8"”L–Ú"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Reimu2\\CatEye.png"));
	AddFacial(reinterpret_cast<const char*>(u8"”ß‚µ‚¢"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Reimu2\\Sad.png"));
	AddFacial(reinterpret_cast<const char*>(u8"“{‚è"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Reimu2\\Angly.png"));
	AddFacial(reinterpret_cast<const char*>(u8"•@’ñ“”"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Reimu2\\Sleep.png"));

#else
	fs::path _folderPath = "./Data/Sprite/Reimu";
	int _imageCount = 0;
	// ‘ÎÛ‚Æ‚·‚éŠg’£qi‚·‚×‚Ä¬•¶šj
	std::set<std::string> imageExtensions = { ".png", ".jpg", ".jpeg" };

	if (fs::exists(_folderPath) && fs::is_directory(_folderPath))
	{
		for (const auto& entry : fs::directory_iterator(_folderPath))
		{
			if (entry.is_regular_file())
			{
				std::string ext = entry.path().extension().string();
				std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);  // ¬•¶š‚É•ÏŠ·

				if (imageExtensions.count(ext) > 0)
				{
					++_imageCount;
				}
			}
		}
	}

	for (int i = 1; i <= _imageCount; i++)
	{
		std::wstring _numberStr = std::to_wstring(i);
		if (i < 10)
		{
			_numberStr = L"0" + std::to_wstring(i);
		}
		std::wstring path = L".\\Data\\Sprite\\Reimu\\" + _numberStr + L".png";
		AddFacial(std::to_string(i), std::make_shared<Sprite>(path.c_str()));
	}
#endif
}
