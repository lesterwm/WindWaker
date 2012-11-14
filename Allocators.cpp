#include "Allocators.h"
#include "Mem.h"
#include <common.h>

// Constructs a stack allocator with the given size
StackAllocator::StackAllocator(const char* name, void* base, u32 stackSize_bytes)
{
	m_Name = name;
	m_pBase = (u8*)base;
	m_pTop = m_pBase;
	m_pEnd = m_pBase + stackSize_bytes;
}

// Constructs a stack allocator and uses allocator to allocate stackSize_bytes for its use
StackAllocator::StackAllocator(const char* name, u32 stackSize_bytes, StackAllocator* allocator)
{
	m_Name = name;
	m_ParentAllocator = allocator;
	m_pBase = (u8*) m_ParentAllocator->Alloc(stackSize_bytes);
	m_pTop = m_pBase;
	m_PrevMarker = m_pBase;
	m_pEnd = m_pBase + stackSize_bytes;

	m_ParentMarker = allocator->GetMarker();
}

// Allocates a new block of memory from the stack top
void* StackAllocator::Alloc(u32 size_bytes)
{
	++m_Allocations;

	if (m_pTop == m_pEnd)
		return NULL;	// Out of memory

	void* ptr = (void*)m_pTop;
	m_pTop += size_bytes;

	return ptr;
}

// Return a marker that can be used as an argument to Free()
void* StackAllocator::GetMarker()
{
	void** marker = (void**) m_pTop;

	// Write the previous marker to the top of the stack
	*marker = m_PrevMarker;
	
	// Save the new marker for comparison on the next free
	m_PrevMarker = marker;
	
	m_pTop += sizeof(void*);

	return marker;
}

// Rolls the stack back to a previous marker
void StackAllocator::Free(void* marker)
{
	// Only the most recent marker can be passed to free, not arbitrary pointers
	ASSERT(marker == m_PrevMarker);

	// Ensure we're not trying to free something from an empty stack
	ASSERT(m_pTop != m_pBase);

	--m_Allocations;
	m_pTop = (u8*)marker;
	m_PrevMarker = *(void**)m_pTop;
}

// Clears the entire stack (rolls back to zero)
void StackAllocator::Clear()
{
	m_pTop = m_pBase;
}

// Return the allocated size of the given memory pointer
u32 StackAllocator::Size(void* marker)
{
	return 0; // Unimplemented
}

StackAllocator::~StackAllocator()
{
	// If we have a parent allocator, then we own our memory pool
	// Otherwise someone else owns it, and we don't need to free it
	if (m_ParentAllocator)
		m_ParentAllocator->Free(m_ParentMarker);

	// Set the total allocated size before we error-check in the parent destructor
	m_Size = m_pTop - m_pBase;
}

PoolAllocator::PoolAllocator(const char* name, void* base, u32 sizeOfElement, u32 numElements)
{
	ASSERT(sizeOfElement >= sizeof(Ptr));
	
	m_ElementSize = sizeOfElement;
	m_Name = name;
	m_pBase = base;
	m_pLLHead = m_pBase;
	
	// Construct a linked-list that points to each new block
	void** ptr = (void**)m_pBase;
	for (u32 i = 0; i < numElements; i++)
	{
		*ptr = ptr + sizeOfElement;
		ptr += sizeOfElement;
	}
}

// Allocates a new block from the pool
void* PoolAllocator::Alloc(u32 unnused)
{
	++m_Allocations;
	m_Size += m_ElementSize;

	Ptr firstFree = m_pLLHead;
	m_pLLHead = *(Ptr*)m_pLLHead;

	return firstFree;
}

// Rolls the stack back to a previous marker
void PoolAllocator::Free(void* ptr)
{
	--m_Allocations;
	m_Size -= m_ElementSize;

	*(Ptr*)ptr =  m_pLLHead;
	m_pLLHead = ptr;
}

u32 PoolAllocator::Size(void* ptr)
{
	return m_ElementSize;
}