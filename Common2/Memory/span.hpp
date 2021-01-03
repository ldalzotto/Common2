#pragma once

template<class ElementType>
struct Span 
{
	size_t Capacity;
	ElementType* Memory;
};

template<class ElementType>
inline Span<ElementType> span_build(ElementType* p_memory, const size_t p_capacity)
{
	return Span<ElementType>{p_capacity, p_memory};
};

template<class ElementType>
inline char span_resize(Span<ElementType>* p_span, const size_t p_new_capacity)
{
    if (p_new_capacity > p_span->Capacity)
    {
        ElementType* l_newMemory = (ElementType*)heap_realloc((char*)p_span->Memory, p_new_capacity * sizeof(ElementType));
        if (l_newMemory != NULL)
        {
            *p_span = span_build(l_newMemory, p_new_capacity);
            return 1;
        }
        return 0;
    }
    return 1;
};

template<class ElementType>
inline void span_free(Span<ElementType>* p_span)
{
    heap_free((char*)p_span->Memory);
    *p_span = span_build<ElementType>(NULL, 0);
};

template<class ElementType>
inline void span_bound_inside_check(const Span<ElementType>* p_span, const Slice<ElementType>* p_tested_slice)
{
#if CONTAINER_BOUND_TEST
    if ((p_tested_slice->Begin + p_tested_slice->Size) >= (p_span->Memory + p_span->Capacity))
    {
        abort();
    }
#endif
};