#pragma once

/*
	A Vector is a Span with an imaginary boundary (Size).
	Any memory access outside of this imaginary boundary will be in error.
	The Vector expose some safe way to insert/erase data (array or single element).
*/
template<class ElementType>
struct Vector
{
	size_t Size;
	Span<ElementType> Span;
};


template<class ElementType>
inline void _vector_bound_check(Vector<ElementType>* p_vector, const size_t p_index)
{
#if CONTAINER_BOUND_TEST
	if (p_index > p_vector->Size)
	{
		abort();
	}
#endif
};


template<class ElementType>
inline void _vector_bound_head_check(Vector<ElementType>* p_vector, const size_t p_index)
{
#if CONTAINER_BOUND_TEST
	if (p_index == p_vector->Size)
	{
		abort();
	}
#endif
};


template<class ElementType>
inline Vector<ElementType> vector_build(ElementType* p_memory, size_t p_initial_capacity)
{
	return Vector<ElementType>{0, span_build(p_memory, p_initial_capacity)};
};

template<class ElementType>
inline Vector<ElementType> vector_allocate(const size_t p_initial_capacity)
{
	return Vector<ElementType>{0, span_allocate<ElementType>(p_initial_capacity)};
};

template<class ElementType>
inline void vector_free(Vector<ElementType>* p_vector)
{
	span_free(&p_vector->Span);
	*p_vector = vector_build<ElementType>(NULL, 0);
};

template<class ElementType>
inline char vector_empty(Vector<ElementType>* p_vector)
{
	return p_vector->Size == 0;
};


template<class ElementType>
inline ElementType* vector_get(Vector<ElementType>* p_vector, const size_t p_index)
{
#if CONTAINER_BOUND_TEST
	_vector_bound_check(p_vector, p_index);
	_vector_bound_head_check(p_vector, p_index);
#endif
	return &p_vector->Span.Memory[p_index];
};

template<class ElementType>
inline ElementType vector_get_vr(Vector<ElementType>* p_vector, const size_t p_index)
{
#if CONTAINER_BOUND_TEST
	_vector_bound_check(p_vector, p_index);
	_vector_bound_head_check(p_vector, p_index);
#endif
	return p_vector->Span.Memory[p_index];
};


template<class ElementType>
inline void _vector_move_memory_down(Vector<ElementType>* p_vector, const size_t p_break_index, const size_t p_move_delta)
{
	Slice<ElementType> l_target = slice_build_memory_offset_elementnb(p_vector->Span.Memory, p_break_index + p_move_delta, p_vector->Size - p_break_index);
#if CONTAINER_BOUND_TEST
	span_bound_inside_check(&p_vector->Span, &l_target);
#endif		
	Slice<ElementType> l_source = slice_build(p_vector->Span.Memory, p_break_index, p_vector->Size);
	slice_memmove(&l_target, &l_source);
};

template<class ElementType>
inline void _vector_move_memory_up(Vector<ElementType>* p_vector, const size_t p_break_index, const size_t p_move_delta)
{
	Slice<ElementType> l_target = slice_build_memory_offset_elementnb(p_vector->Span.Memory, p_break_index, p_vector->Size - p_break_index);
#if CONTAINER_BOUND_TEST
	span_bound_inside_check(&p_vector->Span, &l_target);
#endif		
	Slice<ElementType> l_source = slice_build(p_vector->Span.Memory, p_break_index + p_move_delta, p_vector->Size);
	slice_memmove(&l_target, &l_source);
};


template<class ElementType>
inline void _vector_copy_memory(Vector<ElementType>* p_vector, const size_t p_copy_index, const Slice<ElementType>* p_elements)
{
	Slice<ElementType> l_target = slice_build_memory_elementnb(p_vector->Span.Memory + p_copy_index, p_elements->Size);

#if CONTAINER_BOUND_TEST
	span_bound_inside_check(&p_vector->Span, &l_target);
#endif

	slice_memcpy(
		&l_target,
		p_elements
	);
};

template<class ElementType>
inline char vector_insert_array_at(Vector<ElementType>* p_vector, const Slice<ElementType>* p_elements, const size_t p_index)
{
#if CONTAINER_BOUND_TEST
	_vector_bound_check(p_vector, p_index);
	_vector_bound_head_check(p_vector, p_index); // Cannot insert at the head of the vector. use vector_push_back_array.
#endif

	if (p_vector->Size + p_elements->Size > p_vector->Span.Capacity)
	{
		span_resize(&p_vector->Span, p_vector->Span.Capacity == 0 ? 1 : (p_vector->Span.Capacity * 2));
		vector_insert_array_at(p_vector, p_elements, p_index);
	}
	else
	{
		_vector_move_memory_down(p_vector, p_index, p_elements->Size);
		_vector_copy_memory(p_vector, p_index, p_elements);

		p_vector->Size += p_elements->Size;
	}

	return 1;
};

template<class ElementType>
inline char vector_insert_array_at_1v(Vector<ElementType>* p_vector, const Slice<ElementType> p_elements, const size_t p_index)
{
	return vector_insert_array_at(p_vector, &p_elements, p_index);
}

template<class ElementType>
inline char vector_insert_element_at(Vector<ElementType>* p_vector, const ElementType* p_element, const size_t p_index)
{
#if CONTAINER_BOUND_TEST
	_vector_bound_check(p_vector, p_index);
	_vector_bound_head_check(p_vector, p_index); // Cannot insert at the head of the vector. use vector_push_back_element.
#endif

	if (p_vector->Size + 1 > p_vector->Span.Capacity)
	{
		span_resize(&p_vector->Span, p_vector->Span.Capacity == 0 ? 1 : (p_vector->Span.Capacity * 2));
		return vector_insert_element_at(p_vector, p_element, p_index);
	}
	else
	{
		_vector_move_memory_down(p_vector, p_index, 1);
		p_vector->Span.Memory[p_index] = *p_element;
		p_vector->Size += 1;
	}

	return 1;
};

template<class ElementType>
inline char vector_insert_element_at_1v(Vector<ElementType>* p_vector, const ElementType p_element, const size_t p_index)
{
	return vector_insert_element_at(p_vector, &p_element, p_index);
};


template<class ElementType>
inline char vector_push_back_array(Vector<ElementType>* p_vector, const Slice<ElementType>* p_elements)
{
	if (p_vector->Size + p_elements->Size >= p_vector->Span.Capacity)
	{
		span_resize(&p_vector->Span, p_vector->Span.Capacity == 0 ? 1 : (p_vector->Span.Capacity * 2));
		return vector_push_back_array(p_vector, p_elements);
	}
	else
	{
		_vector_copy_memory(p_vector, p_vector->Size, p_elements);
		p_vector->Size += p_elements->Size;
	}

	return 1;
};

template<class ElementType>
inline char vector_push_back_array_1v(Vector<ElementType>* p_vector, const Slice<ElementType> p_elements)
{
	return vector_push_back_array(p_vector, &p_elements);
};

template<class ElementType>
inline char vector_push_back_element(Vector<ElementType>* p_vector, const ElementType* p_element)
{
	if (p_vector->Size >= p_vector->Span.Capacity)
	{
		span_resize(&p_vector->Span, p_vector->Span.Capacity == 0 ? 1 : (p_vector->Span.Capacity * 2));
		return vector_push_back_element(p_vector, p_element);
	}
	else
	{
		p_vector->Span.Memory[p_vector->Size] = *p_element;
		p_vector->Size += 1;
	}

	return 1;
};

template<class ElementType>
inline char vector_push_back_element_1v(Vector<ElementType>* p_vector, const ElementType p_element)
{
	return vector_push_back_element(p_vector, &p_element);
};


template<class ElementType>
inline char vector_erase_array_at(Vector<ElementType>* p_vector, const size_t p_index, const size_t p_element_nb)
{

#if CONTAINER_BOUND_TEST
	_vector_bound_check(p_vector, p_index);
	_vector_bound_check(p_vector, p_index + p_element_nb);
	_vector_bound_head_check(p_vector, p_index); // use vector_pop_back_array
#endif

	_vector_move_memory_up(p_vector, p_index, p_element_nb);
	p_vector->Size -= p_element_nb;

	return 1;
};

template<class ElementType>
inline char vector_erase_element_at(Vector<ElementType>* p_vector, const size_t p_index)
{
#if CONTAINER_BOUND_TEST
	_vector_bound_check(p_vector, p_index);
	_vector_bound_head_check(p_vector, p_index); // use vector_pop_back
#endif

	_vector_move_memory_up(p_vector, p_index, 1);
	p_vector->Size -= 1;

	return 1;
};


template<class ElementType>
inline char vector_pop_back_array(Vector<ElementType>* p_vector, const size_t p_element_nb)
{
	p_vector->Size -= p_element_nb;
	return 1;
};

template<class ElementType>
inline char vector_pop_back(Vector<ElementType>* p_vector)
{
	p_vector->Size -= 1;
	return 1;
};


#define vector_loop(VectorVariable, Iteratorname) size_t Iteratorname = 0; Iteratorname < (VectorVariable)->Size; Iteratorname++