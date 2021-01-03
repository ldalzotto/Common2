
#include "Common2/common2.hpp"

#include <stdio.h>

int main()
{
	/*
	Span<int> l_sp = span_build<int>(NULL, 0);
	span_resize(&l_sp, 10);
	

	Slice<int> l_slice = slice_build(l_sp.Memory, 5, l_sp.Capacity);

	Vector<int> l_vector = vector_build<int>(NULL, 0);

	int l_ints[10] = { 0,1,2,3,4,5,6,7,8,9 };

	Slice<int> l_ss = slice_build(l_ints, 0, 10);
	vector_insert_at(&l_vector, &l_ss, 0);
	vector_insert_at(&l_vector, &l_ss, 5);

	span_free(&l_sp);
	*/

	Vector<size_t> l_st_v = vector_build<size_t>(NULL, 0);

	for (size_t i = 0; i < 1000000; i++)
	{
		Slice<size_t> tmp_insertion = slice_build_memory_elementnb(&i, 1);
		vector_insert_at(&l_st_v, &tmp_insertion, 0);
	}

	printf("%ld", l_st_v.Size);
}