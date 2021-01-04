#pragma once

/*
	A Slice is an encapsulated C style array.
*/
template<class ElementType>
struct Slice
{
	ElementType* Begin;
	size_t Size;
};

template<class ElementType>
inline Slice<ElementType> slice_build(ElementType* p_memory, const size_t p_begin, const size_t p_end)
{
	return Slice<ElementType>{p_memory + p_begin, p_end - p_begin};
};

template<class ElementType>
inline Slice<ElementType> slice_build_memory_elementnb(ElementType* p_memory, const size_t p_element_nb)
{
	return Slice<ElementType>{p_memory, p_element_nb};
};

template<class ElementType>
inline Slice<ElementType> slice_build_memory_offset_elementnb(ElementType* p_memory, const size_t p_offset, const size_t p_element_nb)
{
	return Slice<ElementType>{p_memory + p_offset, p_element_nb};
};

template<class ElementType>
inline char* slice_memmove(Slice<ElementType>* p_target, const Slice<ElementType>* p_source)
{
#if STANDARD_ALLOCATION_BOUND_TEST
	return heap_memmove_safe(cast(char*, p_target->Begin), p_target->Size * sizeof(ElementType), cast(char*, p_source->Begin), p_source->Size * sizeof(ElementType));
#else
	return heap_memmove((char*)p_target->Begin, (char*)p_source->Begin, p_source->Size * sizeof(ElementType));
#endif
};

template<class ElementType>
inline char* slice_memcpy(Slice<ElementType>* p_target, const Slice<ElementType>* p_source)
{
#if STANDARD_ALLOCATION_BOUND_TEST
	return heap_memcpy_safe(cast(char*, p_target->Begin), p_target->Size *sizeof(ElementType), cast(char*, p_source->Begin), p_source->Size * sizeof(ElementType));
#else
	return heap_memcpy((char*)p_target->Begin, (char*)p_source->Begin, p_source->Size * sizeof(ElementType));
#endif
};
