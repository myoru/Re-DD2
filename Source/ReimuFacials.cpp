#include "ReimuFacials.h"
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <string>
#include <set>

namespace fs = std::filesystem;

ReimuFacials::ReimuFacials()
{
	AddFacial(reinterpret_cast<const char*>(u8"通常"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Reimu\\Normal.png"));
	AddFacial(reinterpret_cast<const char*>(u8"笑顔"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Reimu\\Smile.png"));
	AddFacial(reinterpret_cast<const char*>(u8"猫目"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Reimu\\CatEye.png"));
	AddFacial(reinterpret_cast<const char*>(u8"悲しい"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Reimu\\Sad.png"));
	AddFacial(reinterpret_cast<const char*>(u8"怒り"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Reimu\\Angly.png"));
	AddFacial(reinterpret_cast<const char*>(u8"鼻提灯"), std::make_shared<Sprite>(L".\\Data\\Sprite\\Reimu\\Sleep.png"));

	//fs::path _folderPath = "./Data/Sprite/Reimu";
	//int _imageCount = 0;
	//// 対象とする拡張子（すべて小文字）
	//std::set<std::string> imageExtensions = { ".png", ".jpg", ".jpeg" };

	//if (fs::exists(_folderPath) && fs::is_directory(_folderPath))
	//{
	//	for (const auto& entry : fs::directory_iterator(_folderPath))
	//	{
	//		if (entry.is_regular_file())
	//		{
	//			std::string ext = entry.path().extension().string();
	//			std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);  // 小文字に変換

	//			if (imageExtensions.count(ext) > 0)
	//			{
	//				++_imageCount;
	//			}
	//		}
	//	}
	//}

	//for (int i = 1; i <= _imageCount; i++)
	//{
	//	std::wstring _numberStr = std::to_wstring(i);
	//	if (i < 10)
	//	{
	//		_numberStr = L"0" + std::to_wstring(i);
	//	}
	//	std::wstring path = L".\\Data\\Sprite\\Reimu\\" + _numberStr + L".png";
	//	AddFacial(std::to_string(i), std::make_shared<Sprite>(path.c_str()));
	//}
}
