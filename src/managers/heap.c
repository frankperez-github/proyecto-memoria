#include "heap.h"

int reserve(size_t size, addr_t stack_limit, Heap_t *heap)
{
    // Searching first free space of size {size}
    for(int i = heap->to_addr; i > (int)stack_limit; i--)
    {
        // Empty slot founded
        if(heap->used_slots[i] != 1 && heap->used_slots[i] != 2)
        {
            for(int j = i; j < (int)i+(int)size; j++)
            {
                // Go back and keep searching
                if(j > (int)heap->from_addr || (int)heap->used_slots[j] == 1 || (int)heap->used_slots[j] == 2) break;
                
                
                if(j-i == (int)size-1)
                {
                    // Reserving {size} bytes
                    for(size_t k = 0; k < size; k++)
                    {
                        // 2 in used_slots array means reserved
                        heap->used_slots[k+i] = 2;
                        
                    }
                    //Updating heap limit if needed
                    if((int)heap->from_addr > i)
                    {
                        heap->from_addr = i;
                    }
                    return i;
                }
            }
        }
    }
    return -1;
}

int store(addr_t addr, byte val, Heap_t *heap)
{
    //Can't add out of heap
    if(addr > heap->to_addr || addr < heap->from_addr) return 1;
    
    // Busy or not allocated address
    if(heap->used_slots[addr] == 1 || heap->used_slots[addr] == 0) return 1;

    // Write val in memory
    m_write(addr, val);
    heap->used_slots[addr] = 1;
    return 0;
}

int load(addr_t addr, byte *out, Heap_t *heap)
{
    // Checking addr is valid
    if(addr > heap->to_addr || addr < heap->from_addr) return 1;

    // Checking addr is not empty
    if(heap->used_slots[(int)addr] != 1) return 1;


    // Copying value in addr to out
    *out = m_read(addr);
    heap->used_slots[addr] = 0;
    return 0;
}

Heap_t Heap_init(addr_t from, int max_len){
    Heap_t heap;
    int *used_slots = (int*)malloc(max_len*sizeof(int));
    heap.used_slots = used_slots;
    heap.from_addr = from;
    heap.to_addr = from;
    heap.reserve = reserve;
    heap.store = store;
    heap.load = load;

    return heap;
}
   