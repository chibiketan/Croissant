#ifndef CROISSANT_ENGINE_CORE_VERTEXBUFFERDESCRIPTOR_INL_INC
#  define CROISSANT_ENGINE_CORE_VERTEXBUFFERDESCRIPTOR_INL_INC
#  pragma once

namespace Croissant
{
	namespace Core
	{
		// ------------------------------------------------- impl�mentation VertexBufferDescriptor
		inline VertexBufferDescriptor::VertexBufferDescriptor()
			: m_elements {}
		{
		}

		inline VertexBufferDescriptor::DescriptorElement const& VertexBufferDescriptor::GetDescriptor(VertexComponentEnum component)
		{
			return m_elements[CAST(component)];
		}

		inline void VertexBufferDescriptor::Deactivate(VertexComponentEnum component)
		{
			auto& descriptor = m_elements[CAST(component)];

			descriptor.m_isActive = false;
			descriptor.m_offset = 0;
			descriptor.m_size = 0;
		}

		inline void VertexBufferDescriptor::Activate(VertexComponentEnum component, uint32_t offset, uint32_t size)
		{
			auto& descriptor = m_elements[CAST(component)];

			descriptor.m_isActive = true;
			descriptor.m_offset = offset;
			descriptor.m_size = size;
		}

		// ------------------------------------------------- impl�mentation DescriptorElement
		inline VertexBufferDescriptor::DescriptorElement::DescriptorElement()
			: m_isActive { false }, m_offset { 0 }, m_size { 0 }
		{
		}

		inline bool VertexBufferDescriptor::DescriptorElement::IsActive() const
		{
			return m_isActive;
		}

		inline uint32_t VertexBufferDescriptor::DescriptorElement::GetOffset() const
		{
			return m_offset;
		}

		inline uint32_t VertexBufferDescriptor::DescriptorElement::GetSize() const
		{
			return m_size;
		}
	}
}

#endif // !CROISSANT_ENGINE_CORE_VERTEXBUFFERDESCRIPTOR_INL_INC