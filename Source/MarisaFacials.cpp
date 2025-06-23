#include "MarisaFacials.h"
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <string>
#include <set>

namespace fs = std::filesystem;

MarisaFacials::MarisaFacials()
{
#if 1
	AddFacial(reinterpret_cast<const char*>(u8"�ʏ�"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Marisa2\\Normal.png"));
	AddFacial(reinterpret_cast<const char*>(u8"�Ί�"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Marisa2\\Smile.png"));
	AddFacial(reinterpret_cast<const char*>(u8"�߂���"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Marisa2\\Sad.png"));
	AddFacial(reinterpret_cast<const char*>(u8"��"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Marisa2\\Dark.png"));
	AddFacial(reinterpret_cast<const char*>(u8"�{��"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Marisa2\\Angly.png"));
	AddFacial(reinterpret_cast<const char*>(u8"�Ƃ�"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Marisa2\\Shy.png"));
	AddFacial(reinterpret_cast<const char*>(u8"�E�z"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Marisa2\\Uho.png"));
#else
	fs::path _folderPath = "./Data/Sprite/Marisa";
	int _imageCount = 0;
	// �ΏۂƂ���g���q�i���ׂď������j
	std::set<std::string> imageExtensions = { ".png", ".jpg", ".jpeg" };

	if (fs::exists(_folderPath) && fs::is_directory(_folderPath))
	{
		for (const auto& entry : fs::directory_iterator(_folderPath))
		{
			if (entry.is_regular_file())
			{
				std::string ext = entry.path().extension().string();
				std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);  // �������ɕϊ�

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
#endif
}
