#pragma once

struct VectorOfVector_VectorHeader
{
	size_t Size;
	size_t Capacity;
};

template<class ElementType>
struct VectorOfVector_Element
{
	VectorOfVector_VectorHeader Header;
	Slice<ElementType> Memory;
};

template<class ElementType>
using VectorOfVector = VaryingVector;




inline VectorOfVector_VectorHeader _vectorofvector_vectorheader_build(const size_t p_size, const size_t p_capacity)
{
	return VectorOfVector_VectorHeader{ p_size, p_capacity };
};

inline VectorOfVector_VectorHeader _vectorofvector_vectorheader_build_default()
{
	return _vectorofvector_vectorheader_build(0, 0);
};

inline Span<char> _vectorofvector_vectorheader_allocate(const Slice<char>* p_vector_slice, const size_t p_vector_size)
{
	Span<char> l_allocated_element = span_allocate<char>(sizeof(VectorOfVector_VectorHeader) + p_vector_slice->Size);
	VectorOfVector_VectorHeader l_vector_header = _vectorofvector_vectorheader_build(p_vector_size, p_vector_size);
	span_copy_memory_2v(&l_allocated_element, 0, slice_build_aschar_memory_elementnb(&l_vector_header, 1));
	span_copy_memory(&l_allocated_element, sizeof(l_vector_header), p_vector_slice);
	return l_allocated_element;
};

inline Span<char> _vectorofvector_vectorheader_allocate_0v(const Slice<char> p_vector_slice, const size_t p_vector_size)
{
	return _vectorofvector_vectorheader_allocate(&p_vector_slice, p_vector_size);
};

template<class ElementType>
inline Span<char> _vectorofvector_vectorheader_allocate_vectorelements(const Slice<ElementType>* p_vector_elements)
{
	return _vectorofvector_vectorheader_allocate_0v(slice_build_aschar_slice(p_vector_elements), p_vector_elements->Size);
};

inline size_t _vectorofvector_vectorheader_get_vector_offset()
{
	return sizeof(VectorOfVector_VectorHeader);
};

inline size_t _vectorofvector_vectorheader_get_vector_element_offset(const size_t p_element_index, const size_t p_element_size)
{
	return _vectorofvector_vectorheader_get_vector_offset() + (p_element_index * p_element_size);
};

inline Slice<char> _vectorofvector_vectorheader_get_vector(VectorOfVector_VectorHeader* p_header)
{
	return slice_build_memory_elementnb(cast(char*, p_header) + sizeof(VectorOfVector_VectorHeader), p_header->Size);
};

inline Slice<char> _vectorofvector_vectorheader_get_vectormemory(VectorOfVector_VectorHeader* p_header)
{
	return slice_build_memory_elementnb(cast(char*, p_header) + sizeof(VectorOfVector_VectorHeader), p_header->Capacity);
};




template<class ElementType>
inline void vectorofvector_push_back(VectorOfVector<ElementType>* p_vector_of_vector)
{
	VectorOfVector_VectorHeader l_header = _vectorofvector_vectorheader_build_default();
	Slice<char> l_header_slice = slice_build_aschar_memory_elementnb(&l_header, 1);
	varyingvector_push_back(p_vector_of_vector, &l_header_slice);
};

template<class ElementType>
inline void vectorofvector_push_back_element(VectorOfVector<ElementType>* p_vector_of_vector, Slice<ElementType>* p_vector_elements)
{
	Span<char> l_pushed_memory = _vectorofvector_vectorheader_allocate_vectorelements(p_vector_elements);
	varyingvector_push_back(p_vector_of_vector, &l_pushed_memory.slice);
	span_free(&l_pushed_memory);
};


template<class ElementType>
inline void vectorofvector_insert_at(VectorOfVector<ElementType>* p_vector_of_vector, const size_t p_index)
{
	VectorOfVector_VectorHeader l_header = _vectorofvector_vectorheader_build_default();
	Slice<char> l_header_slice = slice_build_aschar_memory_elementnb(&l_header, 1);
	varyingvector_insert_at(p_vector_of_vector, &l_header_slice, p_index);
};


template<class ElementType>
inline VectorOfVector_Element<ElementType> vectorofvector_get(VectorOfVector<ElementType>* p_vector_of_vector, const size_t p_index)
{
	Slice<char> l_element = varyingvector_get(p_vector_of_vector, p_index);
	return VectorOfVector_Element<ElementType>{
		*(cast(VectorOfVector_VectorHeader*, l_element.Begin)),
		slice_cast_0v<ElementType>(slice_slide_rv(&l_element, _vectorofvector_vectorheader_get_vector_offset()))
	};
};

template<class ElementType>
inline VectorOfVector_VectorHeader* vectorofvector_get_vectorheader(VectorOfVector<ElementType>* p_vector_of_vector, const size_t p_index)
{
	return cast(VectorOfVector_VectorHeader*, varyingvector_get(p_vector_of_vector, p_index).Begin);
};

template<class ElementType>
inline void vectorofvector_element_push_back_element(VectorOfVector<ElementType>* p_vector_of_vector, const size_t p_nested_vector_index, const ElementType* p_element)
{
	VectorOfVector_VectorHeader* l_vector_header = vectorofvector_get_vectorheader<ElementType>(p_vector_of_vector, p_nested_vector_index);
	
	if (l_vector_header->Size + 1 > l_vector_header->Capacity)
	{
		varyingvector_expand_element_with_value_2v(
			p_vector_of_vector,
			p_nested_vector_index,
			slice_build_aschar_memory_elementnb(p_element, 1)
		);

		// /!\ because we potentially reallocate the p_vector_of_vector, we nee to requery for the VectorOfVector_VectorHeader
		l_vector_header = vectorofvector_get_vectorheader<ElementType>(p_vector_of_vector, p_nested_vector_index);
		l_vector_header->Capacity += 1;
	}
	else
	{
		varyingvector_writeto_element_3v(
			p_vector_of_vector,
			p_nested_vector_index,
			_vectorofvector_vectorheader_get_vector_element_offset(l_vector_header->Size, sizeof(ElementType)),
			slice_build_aschar_memory_singleelement(p_element)
		);
	}

	l_vector_header->Size += 1;
};


template<class ElementType>
inline void vectorofvector_element_push_back_array(VectorOfVector<ElementType>* p_vector_of_vector, const size_t p_nested_vector_index, const Slice<ElementType>* p_elements)
{
	VectorOfVector_VectorHeader* l_vector_header = vectorofvector_get_vectorheader<ElementType>(p_vector_of_vector, p_nested_vector_index);

	if (l_vector_header->Size + p_elements->Size > l_vector_header->Capacity)
	{
		//TODO write between l_vector_header->Size and l_vector_header->Capacity
		//TODO expand between l_vector_header->Capacity and l_vector_header->Capacity - (l_vector_header->Size + p_elements->Size)

		// l_vector_header->Capacity += p_elements->Size;
	}
	else
	{
		varyingvector_writeto_element_3v(
			p_vector_of_vector,
			p_nested_vector_index,
			_vectorofvector_vectorheader_get_vector_element_offset(l_vector_header->Size, sizeof(ElementType));
		);

		l_vector_header->Size += p_elements->Size;
	}
};

//TODO -> erase_at

template<class ElementType>
inline void vectorofvector_element_clear(VectorOfVector<ElementType>* p_vector_of_vector, const size_t p_nested_vector_index)
{
	vectorofvector_get_vectorheader<ElementType>(p_vector_of_vector, p_nested_vector_index)->Size = 0;
}