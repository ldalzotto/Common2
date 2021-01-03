#pragma once

inline char* heap_realloc(char* p_memory, const size_t p_new_size)
{
	return (char*)::realloc(p_memory, p_new_size);
};

inline void heap_free(char* p_memory)
{
	::free(p_memory);
};


inline char* heap_memcpy(char* p_dst, const char* p_src, const size_t p_size)
{
	return (char*)::memcpy(p_dst, p_src, p_size);
};

inline static char* heap_memcpy_safe(char* p_dst, const size_t p_dst_size, const char* p_src, const  size_t p_size)
{
#if STANDARD_ALLOCATION_BOUND_TEST
	if (p_size > p_dst_size)
	{
		abort();
	}
#endif

	return heap_memcpy(p_dst, p_src, p_size);
};


inline char* heap_memmove(char* p_dst, const char* p_src,const size_t p_size)
{
	return (char*)::memmove(p_dst, p_src, p_size);
};


inline char* heap_memmove_safe(char* p_dst, const size_t p_dst_size, const char* p_src, const size_t p_size)
{
#if STANDARD_ALLOCATION_BOUND_TEST
	if (p_size > p_dst_size)
	{
		abort();
	}
#endif

	return heap_memmove(p_dst, p_src, p_size);
};


template<class ElementType>
inline size_t memory_offset_bytes(const size_t p_size)
{
	return sizeof(ElementType) * p_size;
};

