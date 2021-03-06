#include "include/RingBuffer.h"

#include <iostream>

int main(int argc, char** argv)
{
    RingBuffer<32, int> buffer;
    Block<int> block;
    
    /* Write 10 ints */
    for (int i = 0; i < buffer.Length(); i++)
    {
        buffer.Append(i);
    }
    
    /* Read a block */
    block = buffer.Read(10);
    
    /* Print out the block */
    for (int i = 0; i < block.Length(); i++)
    {
        std::cout << block.At(i) << std::endl;
    }
    
    return 0;
}