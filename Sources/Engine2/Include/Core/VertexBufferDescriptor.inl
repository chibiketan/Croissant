#ifndef CROISSANT_ENGINE_CORE_VERTEXBUFFERDESCRIPTOR_INL_INC
#  define CROISSANT_ENGINE_CORE_VERTEXBUFFERDESCRIPTOR_INL_INC
#  pragma once

namespace Croissant
{
	namespace Core
	{
		// ------------------------------------------------- implémentation VertexBufferDescriptor
		inline VertexBufferDescriptor::VertexBufferDescriptor()
			: m_elements {}
		{
		}

		inline VertexBufferDescriptor::DescriptorElement const& VertexBufferDescriptor::GetDescriptor(VertexComponentEnum component)
		{
			return m_elements[CAST(component)];
		}

		// ------------------------------------------------- implémentation DescriptorElement
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

		inline void VertexBufferDescriptor::DescriptorElement::Deactivate()
		{
			m_isActive = false;
			m_offset = 0;
			m_size = 0;
		}

		inline void VertexBufferDescriptor::DescriptorElement::Activate(uint32_t offset, uint32_t size)
		{
			m_isActive = true;
			m_offset = offset;
			m_size = size;
		}
	}
}

#endif // !CROISSANT_ENGINE_CORE_VERTEXBUFFERDESCRIPTOR_INL_INC