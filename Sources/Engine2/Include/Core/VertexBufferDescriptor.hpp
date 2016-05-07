#ifndef CROISSANT_ENGINE_CORE_VERTEXBUFFERDESCRIPTOR_HPP_INC
#  define CROISSANT_ENGINE_CORE_VERTEXBUFFERDESCRIPTOR_HPP_INC
#  pragma once

#  include "Core/Enums.hpp"
#  include <array>

#  define CAST(p) static_cast<size_t>(p)

namespace Croissant
{
	namespace Core
	{
		class ENGINE_API VertexBufferDescriptor final
		{
		public:
			class DescriptorElement final
			{
			public:
				DescriptorElement();
				DescriptorElement(DescriptorElement const&) = default;
				DescriptorElement(DescriptorElement&&) = default;
				bool		IsActive() const;
				uint32_t	GetOffset() const;
				uint32_t	GetSize() const;
				void		Deactivate();
				void		Activate(uint32_t offset, uint32_t size);

			private:
				bool		m_isActive;
				uint32_t	m_offset;
				uint32_t	m_size;
			};

			VertexBufferDescriptor();
			VertexBufferDescriptor(VertexBufferDescriptor const&) = default;
			VertexBufferDescriptor(VertexBufferDescriptor&&) = default;
			DescriptorElement const&	GetDescriptor(VertexComponentEnum component);

		private:
			std::array<DescriptorElement, CAST(VertexComponentEnum::MAX_ELEMENT) + 1>	m_elements;

		};
	}
}

#  include "Core/VertexBufferDescriptor.inl"

#  undef CAST
#endif // !CROISSANT_ENGINE_CORE_VERTEXBUFFERDESCRIPTOR_HPP_INC