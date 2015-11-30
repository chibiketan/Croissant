#pragma once
#ifndef TYPE_H_
#define TYPE_H_

#include "Engine.hpp"
#include "Core/NonCopyable.hpp"
#include "Define.hpp"
#include "Core/Pimpl.hpp"
#include <vector>
#include "Core/String.hpp"

using String = Croissant::Core::String;

namespace Croissant
{
	namespace Core
	{
		class ENGINE_API Type : public Croissant::Core::NonCopyable, public Croissant::Core::Pimpl<Type>::Detail
		{
		public:
			Type(const String& name, std::vector<const Type*> parents);
			Type(Type&& source);
			Type(const Type&) = delete;
			~Type();
			String Name() const;
			const std::vector<const Type*>& Parents() const;
			Type& operator=(const Type&) = delete;
			Type& operator=(Type&&) = delete;

			template<typename... Args>
			static Type Make(const String& name)
			{
				std::vector<const Type*> parent;

				Make<Args..., Empty>(parent);
				return Type(name, parent);
			}

		private:
			class Empty
			{
			};

			template<class T, typename... Args>
			static void Make(std::vector<const Type*>& parent)
			{
				parent.push_back(&T::s_type);
				Make<Args...>(parent);
			}

		};

		template<>
		void Type::Make<Type::Empty>(std::vector<const Type*>&);

	}
}

#define _CROISSANT_GET_TYPE(type) &type::s_type,

#define CROISSANT_TYPE_DECLARATION public:\
	static const Croissant::Core::Type s_type;

#endif
