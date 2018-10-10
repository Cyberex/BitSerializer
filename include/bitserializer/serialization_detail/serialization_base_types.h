/*******************************************************************************
* Copyright (C) 2018 by Pavel Kisliak                                          *
* This file is part of BitSerializer library, licensed under the MIT license.  *
*******************************************************************************/
#pragma once
#include <algorithm>
#include <type_traits>
#include "object_traits.h"
#include "archive_traits.h"
#include "base_object.h"
#include "media_archive_base.h"
#include "../string_conversion.h"

namespace BitSerializer {

//-----------------------------------------------------------------------------
// Serialize fundamental types
//-----------------------------------------------------------------------------
template <typename TArchive, typename TValue, std::enable_if_t<std::is_fundamental_v<TValue>, int> = 0>
static bool Serialize(TArchive& archive, const typename TArchive::key_type& key, TValue& value)
{
	if constexpr (!can_serialize_value_with_key_v<TArchive, TValue>) {
		static_assert(false, "BitSerializer. The archive doesn't support serialize fundamental type with key on this level.");
	}
	else
	{
		return archive.SerializeValue(key, value);
	}
};

template <typename TArchive, typename TValue, std::enable_if_t<std::is_fundamental_v<TValue>, int> = 0>
static void Serialize(TArchive& archive, TValue& value)
{
	if constexpr (!can_serialize_value_v<TArchive, TValue>) {
		static_assert(false, "BitSerializer. The archive doesn't support serialize fundamental type without key on this level.");
	}
	else
	{
		archive.SerializeValue(value);
	}
};

//------------------------------------------------------------------------------
// Serialize string types
//------------------------------------------------------------------------------
template <class TArchive, typename TSym, typename TAllocator>
static bool Serialize(TArchive& archive, const typename TArchive::key_type& key, std::basic_string<TSym, std::char_traits<TSym>, TAllocator>& value)
{
	if constexpr (!can_serialize_string_with_key_v<TArchive, std::basic_string<TSym, std::char_traits<TSym>, TAllocator>>) {
		static_assert(false, "BitSerializer. The archive doesn't support serialize string type with key on this level.");
	}
	else
	{
		return archive.SerializeString(key, value);
	}
};

template <class TArchive, typename TSym, typename TAllocator>
static void Serialize(TArchive& archive, std::basic_string<TSym, std::char_traits<TSym>, TAllocator>& value)
{
	if constexpr (!can_serialize_string_v<TArchive, std::basic_string<TSym, std::char_traits<TSym>, TAllocator>>) {
		static_assert(false, "BitSerializer. The archive doesn't support serialize string type without key on this level.");
	}
	else
	{
		archive.SerializeString(value);
	}
};

//-----------------------------------------------------------------------------
// Serialize enum types
//-----------------------------------------------------------------------------
template <class TArchive, class TValue, std::enable_if_t<std::is_enum_v<TValue>, int> = 0>
static bool Serialize(TArchive& archive, const typename TArchive::key_type& key, TValue& value)
{
	if constexpr (archive.IsLoading())
	{
		std::string str;
		auto result = Serialize(archive, key, str);
		Convert::Detail::FromString(str, value);
		return result;
	}
	else
	{
		auto str = Convert::ToString(value);
		return Serialize(archive, key, str);
	}
};

template <class TArchive, class TValue, std::enable_if_t<std::is_enum_v<TValue>, int> = 0>
static void Serialize(TArchive& archive, TValue& value)
{
	if constexpr (archive.IsLoading())
	{
		std::string str;
		Serialize(archive, str);
		Convert::Detail::FromString(str, value);
	}
	else
	{
		auto str = Convert::ToString(value);
		Serialize(archive, str);
	}
};

//------------------------------------------------------------------------------
// Serialize classes
//------------------------------------------------------------------------------
template <class TArchive, typename TValue, std::enable_if_t<std::is_class_v<TValue> && is_serializable_class_v<TValue>, int> = 0>
static bool Serialize(TArchive& archive, const typename TArchive::key_type& key, TValue& value)
{
	if constexpr (!is_serializable_class_v<TValue>) {
		static_assert(false, "BitSerializer. The class must have Serialize() method internally or externally (in namespace BitSerializer).");
	}
	else if constexpr (!can_serialize_object_with_key_v<TArchive>) {
		static_assert(false, "BitSerializer. The archive doesn't support serialize class with key on this level.");
	}
	else
	{
		auto objectScope = archive.OpenObjectScope(key);
		if (objectScope)
			value.Serialize(*objectScope.get());
		return objectScope != nullptr;
	}
};

template <class TArchive, class TValue, std::enable_if_t<std::is_class_v<TValue> && is_serializable_class_v<TValue>, int> = 0>
static void Serialize(TArchive& archive, TValue& value)
{
	if constexpr (!is_serializable_class_v<TValue>) {
		static_assert(false, "BitSerializer. The class must have Serialize() method internally or externally (in namespace BitSerializer).");
	}
	else if constexpr (!can_serialize_object_v<TArchive>) {
		static_assert(false, "BitSerializer. The archive doesn't support serialize class without key on this level.");
	}
	else
	{
		auto objectScope = archive.OpenObjectScope();
		value.Serialize(*objectScope.get());
	}
};

/// <summary>
/// Serializes the base class.
/// </summary>
template <typename TArchive, class TBase>
static void Serialize(TArchive& archive, BaseObject<TBase>&& value)
{
	if constexpr (!is_serializable_class_v<TBase>) {
		static_assert(false, "BitSerializer. The class must have Serialize() method internally or externally (in namespace BitSerializer).");
	}
	else if constexpr (!is_object_scope_v<TArchive>) {
		static_assert(false, "BitSerializer. The archive doesn't support serialize base class on this level.");
	}
	else
	{
		value.Object.TBase::Serialize(archive);
	}
};

//-----------------------------------------------------------------------------
// Serialize arrays
//-----------------------------------------------------------------------------
template<typename TArchive, typename TValue, size_t ArraySize>
static bool Serialize(TArchive& archive, const typename TArchive::key_type& key, TValue(&cont)[ArraySize])
{
	if constexpr (!can_serialize_array_with_key_v<TArchive>) {
		static_assert(false, "BitSerializer. The archive doesn't support serialize array with key on this level.");
	}
	else
	{
		auto arrayScope = archive.OpenArrayScope(key, ArraySize);
		if (arrayScope)
		{
			auto& scope = *arrayScope.get();
			assert(!archive.IsLoading() || ArraySize >= scope.GetSize());
			for (size_t i = 0; i < ArraySize; i++) {
				Serialize(scope, cont[i]);
			}
		}
		return arrayScope != nullptr;
	}
}

template<typename TArchive, typename TValue, size_t ArraySize>
static void Serialize(TArchive& archive, TValue(&cont)[ArraySize])
{
	if constexpr (!can_serialize_array_v<TArchive>) {
		static_assert(false, "BitSerializer. The archive doesn't support serialize array without key on this level.");
	}
	else
	{
		auto arrayScope = archive.OpenArrayScope(ArraySize);
		if (arrayScope)
		{
			auto& scope = *arrayScope.get();
			assert(!archive.IsLoading() || ArraySize >= scope.GetSize());
			for (size_t i = 0; i < ArraySize; i++) {
				Serialize(scope, cont[i]);
			}
		}
	}
}

}	// namespace BitSerializer
