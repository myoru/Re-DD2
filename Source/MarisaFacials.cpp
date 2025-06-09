#include "MarisaFacials.h"
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <string>
#include <set>

namespace fs = std::filesystem;

MarisaFacials::MarisaFacials()
{
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
		std::wstring path = L".\\Data\\Sprite\\Marisa\\" + _numberStr + L".png";
		AddFacial(std::to_string(i), std::make_shared<Sprite>(path.c_str()));
	}
}
