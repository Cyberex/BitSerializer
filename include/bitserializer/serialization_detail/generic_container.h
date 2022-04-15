/*******************************************************************************
* Copyright (C) 2018-2022 by Pavel Kisliak                                     *
* This file is part of BitSerializer library, licensed under the MIT license.  *
*******************************************************************************/
#pragma once

namespace BitSerializer::Detail
{
	/// <summary>
	/// Generic function for serialization containers.
	/// </summary>
	template<typename TArchive, typename TContainer>
	static void SerializeContainer(TArchive& arrayScope, TContainer& cont)
	{
		if constexpr (arrayScope.IsLoading())
		{
			// Resize container when is known approximate size
			if (const auto estimatedSize = arrayScope.GetEstimatedSize(); estimatedSize != 0)
			{
				cont.resize(estimatedSize);
			}

			// Load existing items
			size_t loadedItems = 0;
			for (auto it = cont.begin(); it != cont.end() && !arrayScope.IsEnd(); ++it, ++loadedItems)
			{
				Serialize(arrayScope, *it);
			}
			// Load all left items
			for (; !arrayScope.IsEnd(); ++loadedItems)
			{
				using ValueType = typename TContainer::value_type;
				ValueType value;
				Serialize(arrayScope, value);
				cont.push_back(std::move(value));
			}
			// Resize container for case when loaded less items than estimated
			cont.resize(loadedItems);
		}
		else
		{
			for (auto& value : cont)
			{
				Serialize(arrayScope, value);
			}
		}
	}

	/// <summary>
	/// Returns reference to internal container which is used as base for a container adapter such as std::queue.
	/// </summary>
	template <class TContainer>
	typename TContainer::container_type& GetBaseContainer(TContainer& container)
	{
		struct Accessor : TContainer
		{
			static auto& Get(TContainer& cont) {
				return cont.*(&Accessor::c);
			}
		};
		return Accessor::Get(container);
	}

	/// <summary>
	/// Returns constant reference to internal container which is used as base for a container adapter such as std::queue.
	/// </summary>
	template <class TContainer>
	const typename TContainer::container_type& GetBaseContainer(const TContainer& container)
	{
		struct Accessor : TContainer
		{
			static const auto& Get(const TContainer& cont) {
				return cont.*(&Accessor::c);
			}
		};
		return Accessor::Get(container);
	}
}
