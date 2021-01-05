#pragma once

using VaryingVectorMemory_t = Vector<char>;
using VaryingVectorChunks_t = Vector<SliceIndex>;

struct VaryingVector
{
	VaryingVectorMemory_t memory;
	VaryingVectorChunks_t chunks;
};

inline VaryingVector varyingvector_build(const VaryingVectorMemory_t p_memory, const VaryingVectorChunks_t p_chunks)
{
	return VaryingVector{ p_memory, p_chunks };
};

inline VaryingVector varyingvector_allocate(const size_t p_memory_array_initial_capacity, const size_t p_chunk_array_initial_capacity)
{
	return varyingvector_build(vector_allocate<char>(p_memory_array_initial_capacity), vector_allocate<SliceIndex>(p_chunk_array_initial_capacity));
};

inline VaryingVector varyingvector_allocate_default()
{
	return varyingvector_allocate(0, 0);
};


inline void varyingvector_free(VaryingVector* p_varyfingvector)
{
	vector_free(&p_varyfingvector->memory);
	vector_free(&p_varyfingvector->chunks);
};

inline size_t varyingvector_get_size(VaryingVector* p_varyfingvector)
{
	return p_varyfingvector->chunks.Size;
};

inline void varyingvector_push_back(VaryingVector* p_varyingvector, Slice<char>* p_bytes)
{
	SliceIndex l_chunk = sliceindex_build(p_varyingvector->memory.Size, p_bytes->Size);
	vector_push_back_array(&p_varyingvector->memory, p_bytes);
	vector_push_back_element(&p_varyingvector->chunks, &l_chunk);
};

inline void varyingvector_push_back_1v(VaryingVector* p_varyingvector, Slice<char> p_bytes)
{
	varyingvector_push_back(p_varyingvector, &p_bytes);
};

template<class ElementType>
inline void varyingvector_push_back_element(VaryingVector* p_varyingvector, const ElementType* p_element)
{
	varyingvector_push_back_1v(
		p_varyingvector,
		slice_build_memory_elementnb(cast(char*, p_element), sizeof(ElementType))
	);
};

template<class ElementType>
inline void varyingvector_push_back_element_1v(VaryingVector* p_varyingvector, const ElementType p_element)
{
	varyingvector_push_back_element(p_varyingvector, &p_element);
};

inline void varyingvector_pop_back(VaryingVector* p_varyingvector)
{
	vector_pop_back_array(
		&p_varyingvector->memory,
		vector_get(&p_varyingvector->chunks, p_varyingvector->chunks.Size - 1)->Size
	);
	vector_pop_back(&p_varyingvector->chunks);
};

inline void varyingvector_erase_element_at(VaryingVector* p_varyingvector, const size_t p_index)
{
	SliceIndex* l_chunk = vector_get(&p_varyingvector->chunks, p_index);
	vector_erase_array_at(
		&p_varyingvector->memory,
		l_chunk->Begin,
		l_chunk->Size
	);

	for (loop(i, p_index, p_varyingvector->chunks.Size))
	{
		vector_get(&p_varyingvector->chunks, i)->Begin -= l_chunk->Size;
	};

	vector_erase_element_at(&p_varyingvector->chunks, p_index);
};


inline void varyingvector_erase_array_at(VaryingVector* p_varyingvector, const size_t p_index, const size_t p_element_nb)
{
#if CONTAINER_BOUND_TEST
	assert_true(p_element_nb != 0);
#endif	

	SliceIndex l_removed_chunk = sliceSizedIndex_build_default();

	for (loop(i, p_index, p_index + p_element_nb))
	{
		l_removed_chunk.Size += vector_get(&p_varyingvector->chunks, i)->Size;
	};

	SliceIndex* l_first_chunk = vector_get(&p_varyingvector->chunks, p_index);
	l_removed_chunk.Begin = l_first_chunk->Begin;

	vector_erase_array_at(
		&p_varyingvector->memory,
		l_removed_chunk.Begin,
		l_removed_chunk.Size
	);

	for (loop(i, p_index + p_element_nb, p_varyingvector->chunks.Size))
	{
		vector_get(&p_varyingvector->chunks, i)->Begin -= l_removed_chunk.Size;
	};

	vector_erase_array_at(&p_varyingvector->chunks, p_index, p_element_nb);
};

inline void varyingvector_expand_element(VaryingVector* p_varyingvector, const size_t p_index, const Slice<char>* p_pushed_element)
{
	SliceIndex* l_updated_chunk = vector_get(&p_varyingvector->chunks, p_index);

#if CONTAINER_BOUND_TEST
	assert_true(p_pushed_element->Size != 0);
#endif

	size_t l_size_delta = p_pushed_element->Size;
	size_t l_new_varyingvector_size = p_varyingvector->memory.Size + l_size_delta;

	if (l_new_varyingvector_size > vector_get_capacity(&p_varyingvector->memory))
	{
		vector_resize_expand(&p_varyingvector->memory);
		varyingvector_expand_element(p_varyingvector, p_index, p_pushed_element);
	}
	else
	{
		vector_insert_array_at(&p_varyingvector->memory, p_pushed_element, l_updated_chunk->Begin + l_updated_chunk->Size);
		l_updated_chunk->Size += l_size_delta;

		for (loop(i, p_index + 1, p_varyingvector->chunks.Size))
		{
			vector_get(&p_varyingvector->chunks, i)->Begin += l_size_delta;
		}
	}
};

inline void varyingvector_shrink_element(VaryingVector* p_varyingvector, const size_t p_index, const size_t p_size_delta)
{
	SliceIndex* l_updated_chunk = vector_get(&p_varyingvector->chunks, p_index);

#if CONTAINER_BOUND_TEST
	assert_true(p_size_delta != 0);
	assert_true(p_size_delta <= l_updated_chunk->Size);
#endif

	vector_erase_array_at(&p_varyingvector->memory, l_updated_chunk->Begin + l_updated_chunk->Size - p_size_delta, p_size_delta);
	l_updated_chunk->Size -= p_size_delta;

	for (loop(i, p_index + 1, p_varyingvector->chunks.Size))
	{
		vector_get(&p_varyingvector->chunks, i)->Begin -= p_size_delta;
	}
};

inline Slice<char> varyingvector_get(VaryingVector* p_varyingvector, const size_t p_index)
{
	SliceIndex* l_chunk = vector_get(&p_varyingvector->chunks, p_index);
	return slice_build_memory_offset_elementnb(
		vector_get_memory(&p_varyingvector->memory),
		l_chunk->Begin,
		l_chunk->Size
	);
};

template<class ElementType>
inline Slice<ElementType> varyingvector_get_element(VaryingVector* p_varyingvector, const size_t p_index)
{
	return slice_cast_0v<ElementType>(
		varyingvector_get(p_varyingvector, p_index)
		);
};

#define varyingvector_loop(VaryingVectorVariable, Iteratorname) size_t Iteratorname = 0; Iteratorname < varyingvector_get_size((VaryingVectorVariable)); Iteratorname++