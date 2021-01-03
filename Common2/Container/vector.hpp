#pragma once

template<class ElementType>
struct Vector
{
	size_t Size;
	Span<ElementType> Span;
};

template<class ElementType>
inline Vector<ElementType> vector_build(ElementType* p_memory, size_t p_initial_capacity)
{
	return Vector<ElementType>{0, span_build(p_memory, p_initial_capacity)};
};

template<class ElementType>
inline char vector_insert_at(Vector<ElementType>* p_vector, Slice<ElementType>* p_elements, const size_t p_index)
{
#if CONTAINER_BOUND_TEST
	if (p_index > p_vector->Size)
	{
		abort();
	}
#endif

	if (p_vector->Size + p_elements->Size > p_vector->Span.Capacity)
	{
		span_resize(&p_vector->Span, p_vector->Span.Capacity == 0 ? 1 : (p_vector->Span.Capacity * 2));
		vector_insert_at(p_vector, p_elements, p_index);
	}
	else
	{
		ElementType* l_initialElement = p_vector->Span.Memory + p_index;

		// If we insert between existing elements, we move down memory to give space for new elements
		if (p_vector->Size > p_index)
		{
			Slice<ElementType> l_target = slice_build(p_vector->Span.Memory, p_index + p_elements->Size, p_vector->Size + p_elements->Size);
#if CONTAINER_BOUND_TEST
			span_bound_inside_check(&p_vector->Span, &l_target);
#endif		
			Slice<ElementType> l_source = slice_build(p_vector->Span.Memory, p_index, p_vector->Size);
			slice_memmove(&l_target, &l_source);
		}

		Slice<ElementType> l_target = slice_build_memory_elementnb(p_vector->Span.Memory + p_index, p_elements->Size);

#if CONTAINER_BOUND_TEST
		span_bound_inside_check(&p_vector->Span, &l_target);
#endif	

		slice_memcpy(
			&l_target,
			p_elements
		);

		p_vector->Size += p_elements->Size;
	}

	return 1;
};