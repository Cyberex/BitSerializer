/*******************************************************************************
* Copyright (C) 2018 by Pavel Kisliak                                          *
* This file is part of BitSerializer library, licensed under the MIT license.  *
*******************************************************************************/
#pragma once
#include "media_archive_base.h"

namespace BitSerializer {

/// <summary>
/// Checks that the type is inherited from ArchiveScope.
/// </summary>
template <typename T>
struct is_archive_scope
{
	constexpr static bool value = std::is_base_of<ArchiveScope<SerializeMode::Load>, T>::value ||
		std::is_base_of<ArchiveScope<SerializeMode::Save>, T>::value;
};

template <typename T>
constexpr bool is_archive_scope_v = is_archive_scope<T>::value;

//------------------------------------------------------------------------------

/// <summary>
/// Checks that the FUNDAMENTAL VALUE can be serialized in target archive scope.
/// </summary>
template <typename TArchive, typename TValue>
struct can_serialize_value
{
private:
	template <typename TObj, typename TVal>
	static std::enable_if_t<std::is_void_v<decltype(std::declval<TObj>().SerializeValue(std::declval<TVal&>()))>, std::true_type> test(int);

	template <typename, typename>
	static std::false_type test(...);

public:
	typedef decltype(test<TArchive, TValue>(0)) type;
	enum { value = type::value };
};

template <typename TArchive, typename TValue>
constexpr bool can_serialize_value_v = can_serialize_value<TArchive, TValue>::value;

/// <summary>
/// Checks that the FUNDAMENTAL VALUE can be serialized WITH KEY in target archive scope.
/// </summary>
template <typename TArchive, typename TValue>
struct can_serialize_value_with_key
{
private:
	template <typename TObj, typename TVal>
	static std::enable_if_t<std::is_void_v<decltype(std::declval<TObj>().SerializeValue(std::declval<const typename TArchive::key_type&>(), std::declval<TVal&>()))>, std::true_type> test(int);

	template <typename, typename>
	static std::false_type test(...);

public:
	typedef decltype(test<TArchive, TValue>(0)) type;
	enum { value = type::value };
};

template <typename TArchive, typename TValue>
constexpr bool can_serialize_value_with_key_v = can_serialize_value_with_key<TArchive, TValue>::value;

//------------------------------------------------------------------------------

/// <summary>
/// Checks that the STRING can be serialized in target archive scope.
/// </summary>
template <typename TArchive, typename TValue>
struct can_serialize_string
{
private:
	template <typename TObj, typename TVal>
	static std::enable_if_t<std::is_void_v<decltype(std::declval<TObj>().SerializeString(std::declval<TVal&>()))>, std::true_type> test(int);

	template <typename, typename>
	static std::false_type test(...);

public:
	typedef decltype(test<TArchive, TValue>(0)) type;
	enum { value = type::value };
};

template <typename TArchive, typename TValue>
constexpr bool can_serialize_string_v = can_serialize_string<TArchive, TValue>::value;

/// <summary>
/// Checks that the STRING can be serialized WITH KEY in target archive scope.
/// </summary>
template <typename TArchive, typename TValue>
struct can_serialize_string_with_key
{
private:
	template <typename TObj, typename TVal>
	static std::enable_if_t<std::is_void_v<decltype(std::declval<TObj>().SerializeString(std::declval<const typename TArchive::key_type&>(), std::declval<TVal&>()))>, std::true_type> test(int);

	template <typename, typename>
	static std::false_type test(...);

public:
	typedef decltype(test<TArchive, TValue>(0)) type;
	enum { value = type::value };
};

template <typename TArchive, typename TValue>
constexpr bool can_serialize_string_with_key_v = can_serialize_string_with_key<TArchive, TValue>::value;

//------------------------------------------------------------------------------

/// <summary>
/// Checks that the CLASS OBJECT can be serialized in target archive scope.
/// </summary>
template <typename TArchive>
struct can_serialize_object
{
private:
	template <typename TObj>
	static std::enable_if_t<!std::is_void_v<decltype(std::declval<TObj>().OpenScopeForSerializeObject())>, std::true_type> test(int);

	template <typename>
	static std::false_type test(...);

public:
	typedef decltype(test<TArchive>(0)) type;
	enum { value = type::value };
};

template <typename TArchive>
constexpr bool can_serialize_object_v = can_serialize_object<TArchive>::value;

/// <summary>
/// Checks that the CLASS OBJECT can be serialized WITH KEY in target archive scope.
/// </summary>
template <typename TArchive>
struct can_serialize_object_with_key
{
private:
	template <typename TObj>
	static std::enable_if_t<!std::is_void_v<decltype(std::declval<TObj>().OpenScopeForSerializeObject(std::declval<const typename TArchive::key_type&>()))>, std::true_type> test(int);

	template <typename>
	static std::false_type test(...);

public:
	typedef decltype(test<TArchive>(0)) type;
	enum { value = type::value };
};

template <typename TArchive>
constexpr bool can_serialize_object_with_key_v = can_serialize_object_with_key<TArchive>::value;

/// <summary>
/// Checks that the archive scope has support serialize values with keys (by checking existence of GetKeyByIndex() method).
/// </summary>
template <typename TArchive>
struct is_object_scope
{
private:
	template <typename TObj>
	static std::enable_if_t<std::is_same_v<decltype(std::declval<TObj>().GetKeyByIndex(std::declval<int>())), typename TArchive::key_type>, std::true_type> test(int);

	template <typename>
	static std::false_type test(...);

public:
	typedef decltype(test<TArchive>(0)) type;
	enum { value = type::value };
};

template <typename TArchive>
constexpr bool is_object_scope_v = is_object_scope<TArchive>::value;

//------------------------------------------------------------------------------

/// <summary>
/// Checks that the ARRAY can be serialized in target archive scope.
/// </summary>
template <typename TArchive>
struct can_serialize_array
{
private:
	template <typename TObj>
	static std::enable_if_t<!std::is_void_v<decltype(std::declval<TObj>().OpenScopeForSerializeArray(std::declval<size_t>()))>, std::true_type> test(int);

	template <typename>
	static std::false_type test(...);

public:
	typedef decltype(test<TArchive>(0)) type;
	enum { value = type::value };
};

template <typename TArchive>
constexpr bool can_serialize_array_v = can_serialize_array<TArchive>::value;

/// <summary>
/// Checks that the ARRAY can be serialized WITH KEY in target archive scope.
/// </summary>
template <typename TArchive>
struct can_serialize_array_with_key
{
private:
	template <typename TObj>
	static std::enable_if_t<!std::is_void_v<decltype(std::declval<TObj>().OpenScopeForSerializeArray(std::declval<const typename TArchive::key_type&>(), std::declval<size_t>()))>, std::true_type> test(int);

	template <typename>
	static std::false_type test(...);

public:
	typedef decltype(test<TArchive>(0)) type;
	enum { value = type::value };
};

template <typename TArchive>
constexpr bool can_serialize_array_with_key_v = can_serialize_array_with_key<TArchive>::value;

}	// namespace BitSerializer
