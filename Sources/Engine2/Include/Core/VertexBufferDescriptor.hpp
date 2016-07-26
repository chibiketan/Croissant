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
				friend VertexBufferDescriptor;

				DescriptorElement();
				DescriptorElement(DescriptorElement const&) = default;
				DescriptorElement(DescriptorElement&&) noexcept = delete;
				DescriptorElement&	operator=(DescriptorElement const&) = default;
				bool					IsActive() const;
				uint32_t				GetOffset() const;
				uint32_t				GetSize() const;
				VertexComponentTypeEnum	GetType() const;
				bool		operator==(DescriptorElement const& right) const;

			private:
				bool					m_isActive;
				uint32_t				m_offset;
				uint32_t				m_size;
				VertexComponentTypeEnum	m_type;
			};

			VertexBufferDescriptor();
			VertexBufferDescriptor(VertexBufferDescriptor const&) = default;
			VertexBufferDescriptor(VertexBufferDescriptor&&) = default;
			VertexBufferDescriptor&	operator=(VertexBufferDescriptor const&) = default;
			VertexBufferDescriptor&	operator=(VertexBufferDescriptor&&) noexcept = default;
			bool					operator==(VertexBufferDescriptor const& right) const;
			DescriptorElement const&	GetDescriptor(VertexComponentEnum component);
			void						Deactivate(VertexComponentEnum component);
			void						Activate(VertexComponentEnum component, VertexComponentTypeEnum type, uint32_t offset, uint32_t size);
			uint32_t					GetStride() const;
			void						SetStride(uint32_t stride);

		private:
			std::array<DescriptorElement, CAST(VertexComponentEnum::MAX_ELEMENT) + 1>	m_elements;
			uint32_t																	m_stride;


		};
	}
}

#  include "Core/VertexBufferDescriptor.inl"

#  undef CAST
#endif // !CROISSANT_ENGINE_CORE_VERTEXBUFFERDESCRIPTOR_HPP_INC