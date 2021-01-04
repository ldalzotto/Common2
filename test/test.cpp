
#include "Common2/common2.hpp"

#include <stdio.h>

//TODO
/*
	cast with macro
*/

inline void assert_true(bool p_condition)
{
	if (!p_condition) { abort(); }
};

template<class ElementType>
inline void assert_span_unitialized(Span<ElementType>* p_span)
{
	assert_true(p_span->Capacity == 0);
	assert_true(p_span->Memory == NULL);
};

inline void span_test()
{
	Span<size_t> l_span_sizet = span_build<size_t>(NULL, 0);

	//When resizing the span, new memory is allocated
	{
		size_t l_new_capacity = 10;
		span_resize(&l_span_sizet, 10);
		assert_true(l_span_sizet.Capacity == l_new_capacity);
		assert_true(l_span_sizet.Memory != NULL);
	}
	
	//When freeing the span, it's structure is resetted
	{
		span_free(&l_span_sizet);
		assert_span_unitialized(&l_span_sizet);
	}
};

inline void vector_test()
{
	Vector<size_t> l_vector_sizet = vector_build((size_t*)NULL, 0);

	// vector_push_back_array
	{
		size_t l_old_size = l_vector_sizet.Size;
		size_t l_elements[5] = { 0,1,2,3,4 };
		Slice<size_t> l_elements_slice = slice_build_memory_elementnb(l_elements, 5);

		vector_push_back_array(&l_vector_sizet, &l_elements_slice);
		assert_true(l_vector_sizet.Size == l_old_size + 5);
		for (loop(i, l_old_size, l_vector_sizet.Size))
		{
			assert_true(*vector_get(&l_vector_sizet, i) == l_elements[i - l_old_size]);
		}
	}

	// vector_push_back_element
	{
		size_t l_old_size = l_vector_sizet.Size;
		size_t l_element = 25;
		vector_push_back_element(&l_vector_sizet, &l_element);
		assert_true(l_vector_sizet.Size == l_old_size + 1);
		assert_true(*vector_get(&l_vector_sizet, l_vector_sizet.Size - 1) == l_element);
	}

	// vector_insert_array_at
	{
		size_t l_old_size = l_vector_sizet.Size;
		size_t l_elements[5] = { 0,1,2,3,4 };
		Slice<size_t> l_elements_slice = slice_build_memory_elementnb(l_elements, 5);
		vector_insert_array_at(&l_vector_sizet, &l_elements_slice, 0);
		assert_true(l_vector_sizet.Size == l_old_size + 5);
		for (loop_si(i, 0, 5))
		{
			assert_true((*vector_get(&l_vector_sizet, i)) == i);
		}

		vector_insert_array_at(&l_vector_sizet, &l_elements_slice, 3);
		assert_true(l_vector_sizet.Size == l_old_size + 10);
		for (loop_si(i, 0, 3))
		{
			assert_true((*vector_get(&l_vector_sizet, i)) == l_elements[i]);
		}
		//Middle insertion
		for (loop_si(i, 3, 8))
		{
			assert_true((*vector_get(&l_vector_sizet, i)) == l_elements[i - cast(size_t, 3)]);
		}
		for (loop_si(i, 8, 10))
		{
			assert_true((*vector_get(&l_vector_sizet, i)) == l_elements[i - cast(size_t, 5)]);
		}
	}

	// vector_insert_element_at
	{
		size_t l_element = 20;
		size_t l_old_size = l_vector_sizet.Size;

		vector_insert_element_at(&l_vector_sizet, &l_element, 7);
		assert_true(*vector_get(&l_vector_sizet, 7) == l_element);
		assert_true(l_vector_sizet.Size == l_old_size + 1);

		vector_insert_element_at_1v(&l_vector_sizet, cast(size_t, 20), 9);
	}

	// vector_erase_element_at
	{
		size_t l_old_size = l_vector_sizet.Size;
		size_t l_erase_index = 1;
		size_t l_element_after = *vector_get(&l_vector_sizet, l_erase_index + 1);
		vector_erase_element_at(&l_vector_sizet, 1);
		assert_true(l_vector_sizet.Size == l_old_size - 1);
		assert_true(*vector_get(&l_vector_sizet, 1) == l_element_after);
	}

	// vector_erase_array_at
	{
		size_t l_old_size = l_vector_sizet.Size;
		size_t l_erase_begin_index = 3; 
		const size_t l_erase_nb = 6;
		const size_t l_old_element_check_nb = 3;

		size_t l_old_values[l_old_element_check_nb];
		for (loop(i, l_erase_begin_index + l_erase_nb, (l_erase_begin_index + l_erase_nb) + l_old_element_check_nb))
		{
			l_old_values[i - (l_erase_begin_index + l_erase_nb)] = *vector_get(&l_vector_sizet, i);
		}

		vector_erase_array_at(&l_vector_sizet, l_erase_begin_index, l_erase_nb);

		assert_true(l_vector_sizet.Size == l_old_size - l_erase_nb);
		for (loop(i, 0, l_old_element_check_nb))
		{
			assert_true(*vector_get(&l_vector_sizet, l_erase_begin_index + i) == l_old_values[i]);
		}
	}

	// vector_pop_back
	{
		size_t l_old_size = l_vector_sizet.Size;
		vector_pop_back(&l_vector_sizet);
		assert_true(l_vector_sizet.Size == l_old_size - 1);
	}

	// vector_pop_back_array
	{
		size_t l_old_size = l_vector_sizet.Size;
		vector_pop_back_array(&l_vector_sizet, 3);
		assert_true(l_vector_sizet.Size == l_old_size - 3);
	}

	//When freeing the vcetor, it's structure is resetted
	{
		vector_free(&l_vector_sizet);
		assert_true(l_vector_sizet.Size == 0);
		assert_span_unitialized(&l_vector_sizet.Span);
	}
};

inline void sandbox_test()
{
	Vector<size_t> l_t = vector_allocate<size_t>(10);

	for (vector_loop(&l_t, i))
	{
	
	}

	vector_free(&l_t);
}

int main()
{
	span_test();
	vector_test();
	sandbox_test();
}