#include "stdafx.hpp"
#include "Core/Type.hpp"

namespace Croissant
{
	namespace Core
	{

		template<>
		void Type::Make<Type::Empty>(std::vector<const Type*>&)
		{
		}

		template<>
		class Pimpl<Type>::Implementation
		{
		public:
			Implementation(Type* ref, const String& name, std::vector<const Type*> parents);

            void SetRef(Type* ref)
            {
                m_ref = ref;
            }

			const String m_name;
			const std::vector<const Type*> m_parents;
		private:
			Type* m_ref;
		};

		typedef Croissant::Core::Pimpl<Type>::Implementation TypeImpl;

		// ------------------------------- implémentation Type::Pimpl

		Pimpl<Type>::Implementation::Implementation(Type* ref, const String& name, std::vector<const Type*> parents)
			: m_name(name), m_parents(std::move(parents)), m_ref(ref)
		{
		}

		// ------------------------------- implémentation Type

		Type::Type(const String& name, std::vector<const Type*> parents)
			//: m_pimpl(CROISSANT_NEW Pimpl(name, std::move(parents)))
			: Croissant::Core::Pimpl<Type>::Detail(name, parents)
		{
		}

		Type::Type(Type&& source)
			: Croissant::Core::Pimpl<Type>::Detail(static_cast<Croissant::Core::Pimpl<Type>::Detail&&>(source))
		{
		}

		Type::~Type()
		{
		}

		String Type::Name() const
		{
			const Pimpl<Type>::Implementation& imp = **this;

			return imp.m_name;
		}

		const std::vector<const Type*>& Type::Parents() const
		{
			const Pimpl<Type>::Implementation& imp = **this;

			return imp.m_parents;
		}

	}
}
