#pragma once


template<class ElementType>
using VectorOfVector = VaryingVector;

using VectorOfVector_ElementHeader = size_t;

inline VectorOfVector_ElementHeader vectorofvector_elementheader_build(const size_t p_capacity)
{
	return p_capacity;
};

inline VectorOfVector_ElementHeader vectorofvector_elementheader_build_default()
{
	return 0;
};

inline Slice<char> vectorofvector_elementheader_get_vector(VectorOfVector_ElementHeader* p_header)
{
	return slice_build_memory_elementnb(cast(char*, p_header) + sizeof(VectorOfVector_ElementHeader), *p_header);
};

