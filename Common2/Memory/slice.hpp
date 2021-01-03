#pragma once

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
inline char* slice_memmove(Slice<ElementType>* p_target, const Slice<ElementType>* p_source)
{
	return heap_memmove_safe((char*)p_target->Begin, p_target->Size * sizeof(ElementType), (char*)p_source->Begin, p_source->Size * sizeof(ElementType));
};

template<class ElementType>
inline char* slice_memcpy(Slice<ElementType>* p_target, const Slice<ElementType>* p_source)
{
	return heap_memcpy_safe((char*)p_target->Begin, p_target->Size *sizeof(ElementType), (char*)p_source->Begin, p_source->Size * sizeof(ElementType));
};
