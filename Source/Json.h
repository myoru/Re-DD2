#pragma once
#include <DirectXMath.h>
#include <nlohmann/json.hpp>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/polymorphic.hpp>

using Json = nlohmann::json;

namespace DirectX
{
	template<class T>
	void serialize(T& archive, DirectX::XMFLOAT2& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y)
		);
	}

	template<class T>
	void serialize(T& archive, DirectX::XMFLOAT3& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z)
		);

	}

	template < class T>
	void serialize(T& archive, DirectX::XMFLOAT4& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z),
			cereal::make_nvp("w", v.w)
		);
	}

	template < class T>
	void serialize(T& archive, DirectX::XMFLOAT4X4& m)
	{
		archive(
			cereal::make_nvp("_11", m._11), cereal::make_nvp("_12", m._12),
			cereal::make_nvp("_13", m._13), cereal::make_nvp("_14", m._14),
			cereal::make_nvp("_21", m._21), cereal::make_nvp("_22", m._22),
			cereal::make_nvp("_23", m._23), cereal::make_nvp("_24", m._24),
			cereal::make_nvp("_31", m._31), cereal::make_nvp("_32", m._32),
			cereal::make_nvp("_33", m._33), cereal::make_nvp("_34", m._34),
			cereal::make_nvp("_41", m._41), cereal::make_nvp("_42", m._42),
			cereal::make_nvp("_43", m._43), cereal::make_nvp("_44", m._44)
		);
	}

	template < class T>
	void serialize(T& archive, DirectX::XMINT2& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y)
		);
	}

	/*template < class T>
	void serialize(T& archive, int& value)
	{
		archive(
			cereal::make_nvp("value", value)
		);
	}

	template < class T>
	void serialize(T& archive, bool& flag)
	{
		archive(
			cereal::make_nvp("flag", flag)
		);
	}*/
}

template < class T>
static void LoadJson(const char* a_filename, T& a_elements)
{
	std::ifstream ifs(a_filename);
	cereal::JSONInputArchive archive(ifs);
	archive(CEREAL_NVP(a_elements));
}

template < class T>
static void SaveJson(const char* a_filename, T& a_elements)
{
	std::ofstream ofs(a_filename);
	cereal::JSONOutputArchive archive(ofs);
	archive(CEREAL_NVP(a_elements));
}