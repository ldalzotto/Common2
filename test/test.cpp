
#include "Common2/common2.hpp"

#include <stdio.h>

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

inline void pool_test()
{
	Pool<size_t> l_pool_sizet = pool_allocate<size_t>(10);

	{
		assert_true(pool_get_memory(&l_pool_sizet) != NULL);
		assert_true(pool_get_capacity(&l_pool_sizet) == 10);
		assert_true(pool_get_size(&l_pool_sizet) == 0);
	}

	// pool_alloc_element - allocate new element
	{
		assert_true(pool_get_free_size(&l_pool_sizet) == 0);

		size_t l_element = 3;
		Token(size_t) l_token = pool_alloc_element(&l_pool_sizet, &l_element);

		assert_true(l_token.tok == 0);
		assert_true(*pool_get(&l_pool_sizet, &l_token) == l_element);
	}

	// pool_release_element - release elements
	{
		Token(size_t) l_token = Token(size_t) { 0 };
		pool_release_element(&l_pool_sizet, &l_token);

		// memory is not deallocated
		assert_true(pool_get_size(&l_pool_sizet) == 1);
	}

	// pool_alloc_element - allocating an element while there is free slots
	{
		size_t l_element = 4;
		Token(size_t) l_token = pool_alloc_element(&l_pool_sizet, &l_element);

		pool_alloc_element_1v(&l_pool_sizet, cast(size_t, 10));
		pool_release_element_1v(
			&l_pool_sizet,
			pool_alloc_element_1v(&l_pool_sizet, cast(size_t, 10))
		);
		pool_alloc_element_1v(&l_pool_sizet, cast(size_t, 10));

		assert_true(l_token.tok == 0);
		assert_true(*pool_get(&l_pool_sizet, &l_token) == l_element);
	}

	for (pool_loop(&l_pool_sizet, i))
	{
		size_t l_element = pool_get_rv1v(&l_pool_sizet, Token(size_t) { i });
	}

	{
		pool_free(&l_pool_sizet);
		assert_true(pool_get_size(&l_pool_sizet) == 0);
		assert_true(pool_get_capacity(&l_pool_sizet) == 0);
		assert_true(pool_get_memory(&l_pool_sizet) == 0);
	}
};

inline void varyingvector_test()
{
	VaryingVector l_varyingvector = varyingvector_allocate_default();

	// varyingvector_push_back
	{
		assert_true(l_varyingvector.memory.Size == 0);

		Slice<char> l_slice = slice_build_memory_elementnb(cast(char*, &l_varyingvector), 10);
		varyingvector_push_back(&l_varyingvector, &l_slice);

		assert_true(varyingvector_get_size(&l_varyingvector) == 1);
		Slice<char> l_element_0 = varyingvector_get(&l_varyingvector, 0);
		assert_true(l_element_0.Size == 10);
		assert_true(slice_memcompare_element(&l_slice, &l_element_0));
	}

	// varyingvector_push_back_element
	{
		size_t l_element = 20;
		varyingvector_push_back_element(&l_varyingvector, &l_element);

		size_t l_inserted_index = varyingvector_get_size(&l_varyingvector) - 1;
		Slice<char> l_element_inserted = varyingvector_get(&l_varyingvector, l_inserted_index);

		assert_true(l_element_inserted.Size == sizeof(size_t));
		assert_true(memory_compare(cast(const char*, &l_element), l_element_inserted.Begin, l_element_inserted.Size));

		Slice<size_t> l_casted_slice = slice_cast<size_t>(&l_element_inserted);
		assert_true(l_casted_slice.Size == 1);

	}

	// varyingvector_pop_back
	{

		size_t l_old_size = varyingvector_get_size(&l_varyingvector);
		varyingvector_pop_back(&l_varyingvector);
		assert_true(varyingvector_get_size(&l_varyingvector) == (l_old_size - 1));
	}

	varyingvector_free(&l_varyingvector);
	l_varyingvector = varyingvector_allocate_default();

	// varyingvector_erase_element_at
	{
		for (loop(i, 0, 5))
		{
			varyingvector_push_back_element_1v(&l_varyingvector, cast(size_t, i));
		}

		assert_true(varyingvector_get_size(&l_varyingvector) == 5);
		varyingvector_erase_element_at(&l_varyingvector, 2);
		assert_true(varyingvector_get_size(&l_varyingvector) == 4);

		assert_true(*varyingvector_get_element<size_t>(&l_varyingvector, 2).Begin == 3);
		assert_true(*varyingvector_get_element<size_t>(&l_varyingvector, 3).Begin == 4);
	}

	varyingvector_free(&l_varyingvector);
	l_varyingvector = varyingvector_allocate_default();

	// varyingvector_erase_array_at
	{
		for (loop(i, 0, 5))
		{
			varyingvector_push_back_element_1v(&l_varyingvector, cast(size_t, i));
		}

		assert_true(varyingvector_get_size(&l_varyingvector) == 5);
		varyingvector_erase_array_at(&l_varyingvector, 2, 2);
		assert_true(varyingvector_get_size(&l_varyingvector) == 3);

		assert_true(*varyingvector_get_element<size_t>(&l_varyingvector, 2).Begin == 4);
	}

	varyingvector_free(&l_varyingvector);
	l_varyingvector = varyingvector_allocate_default();

	// varyingvector_expand_element_with_value varyingvector_shrink_element
	{
		for (loop(i, 0, 5))
		{
			varyingvector_push_back_element_1v(&l_varyingvector, cast(size_t, i));
		}

		size_t l_inserset_number = 30;
		Slice<char> l_expansion_slice = slice_build_aschar_memory_elementnb(&l_inserset_number, 1);
		varyingvector_expand_element_with_value(&l_varyingvector, 2, &l_expansion_slice);

		Slice<size_t> l_sizet_element_2 = slice_cast_0v<size_t>(varyingvector_get(&l_varyingvector, 2));
		assert_true(l_sizet_element_2.Size == 2);
		assert_true(slice_get_rv(&l_sizet_element_2, 1) == l_inserset_number);

		{
			size_t* l_sizet_element_3 = slice_cast_singleelement_0v<size_t>(varyingvector_get(&l_varyingvector, 3));
			assert_true(*l_sizet_element_3 == 3);
		}

		varyingvector_shrink_element(&l_varyingvector, 2, sizeof(size_t));
		l_sizet_element_2 = slice_cast_0v<size_t>(varyingvector_get(&l_varyingvector, 2));
		assert_true(l_sizet_element_2.Size == 1);
		assert_true(slice_get_rv(&l_sizet_element_2, 0) == 2);

		{
			size_t* l_sizet_element_3 = slice_cast_singleelement_0v<size_t>(varyingvector_get(&l_varyingvector, 3));
			assert_true(*l_sizet_element_3 == 3);
		}
	}

	// varyingvector_writeto_element
	{
		size_t l_element_0 = 10;
		size_t l_element_1 = 20;
		size_t l_element_2 = 30;

		varyingvector_expand_element(&l_varyingvector, 2, sizeof(size_t) * 3);
		varyingvector_writeto_element_3v(&l_varyingvector, 2, 0, slice_build_aschar_memory_singleelement(&l_element_0));
		varyingvector_writeto_element_3v(&l_varyingvector, 2, 2 * sizeof(size_t), slice_build_aschar_memory_singleelement(&l_element_2));
		varyingvector_writeto_element_3v(&l_varyingvector, 2, 1 * sizeof(size_t), slice_build_aschar_memory_singleelement(&l_element_1));


		Slice<char> l_varyingvector_element_2 = varyingvector_get(&l_varyingvector, 2);
		assert_true(*cast(size_t*, l_varyingvector_element_2.Begin) == l_element_0);
		assert_true(*slice_slide_rv(&l_varyingvector_element_2, sizeof(size_t)).Begin == l_element_1);
		assert_true(*slice_slide_rv(&l_varyingvector_element_2, 2 * sizeof(size_t)).Begin == l_element_2);
	}

	{
		for (varyingvector_loop(&l_varyingvector, i))
		{
			varyingvector_get(&l_varyingvector, i);
		}
	}

	{
		varyingvector_free(&l_varyingvector);
		assert_true(varyingvector_get_size(&l_varyingvector) == 0);
	}
};

inline void vectorofvector_test()
{
	//TODO -> VectorOfVector<size_t> isn't type strict when calling "vectorofvector_XXXX" methods
	VectorOfVector<size_t> l_vectorofvector_size_t = varyingvector_allocate_default();

	// vectorofvector_push_back vectorofvector_push_back_element
	{
		Span<size_t> l_sizets = span_allocate<size_t>(10);
		for (loop(i, 0, l_sizets.Capacity))
		{
			*slice_get(&l_sizets.slice, i) = i;
		}

		vectorofvector_push_back<size_t>(&l_vectorofvector_size_t);

		vectorofvector_push_back_element(&l_vectorofvector_size_t, &l_sizets.slice);
		VectorOfVector_Element<size_t> l_element =
			vectorofvector_get<size_t>(
				&l_vectorofvector_size_t,
				varyingvector_get_size(&l_vectorofvector_size_t) - 1
				);

		vectorofvector_push_back<size_t>(&l_vectorofvector_size_t);

		assert_true(l_element.Header.Size == l_sizets.Capacity);
		for (loop(i, 0, l_sizets.Capacity))
		{
			assert_true(slice_get_rv(&l_element.Memory, i) == i);
		}

		span_free(&l_sizets);
	}

	// vectorofvector_insert_at 
	{
	}

	// vectorofvector_element_push_back_element
	{
		for (loop(i, 0, 2))
		{
			vectorofvector_push_back<size_t>(&l_vectorofvector_size_t);

			size_t l_element = 30;
			size_t l_index = varyingvector_get_size(&l_vectorofvector_size_t) - 2;
			vectorofvector_element_push_back_element(&l_vectorofvector_size_t, l_index, &l_element);
			VectorOfVector_Element<size_t> l_element_nested = vectorofvector_get<size_t>(&l_vectorofvector_size_t, l_index);
			assert_true(l_element_nested.Header.Size == 1);
			assert_true(slice_get_rv(&l_element_nested.Memory, 0) == l_element);

			l_element = 35;
			vectorofvector_element_clear<size_t>(&l_vectorofvector_size_t, l_index);
			vectorofvector_element_push_back_element(&l_vectorofvector_size_t, l_index, &l_element);
			assert_true(l_element_nested.Header.Size == 1);
			assert_true(slice_get_rv(&l_element_nested.Memory, 0) == l_element);
		}
	}

	// vectorofvector_element_push_back_array
	{
	}


	varyingvector_free(&l_vectorofvector_size_t);
};

inline void sandbox_test()
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

		vector_insert_array_at_always(&l_vector_sizet, &l_elements_slice, l_vector_sizet.Size);
	}


}

int main()
{
	span_test();
	vector_test();
	pool_test();
	varyingvector_test();
	vectorofvector_test();
	sandbox_test();
}