/**
 * This test writes 10 numbers in order 0 to 9.
 * Then writes another 10 numbers in order 0 to 9
 * Then reads 20 numbers from the buffer.
 * Then writes another 10 numbers in order 0 to 9
 * Then reads 10 numbers from the buffer and prints them.
 */

#include <RingBuffer.h>

#include <iostream>

int main(int argc, char** argv)
{
    RingBuffer<32, int> buffer;
    Block<int> block;
    
    /* Write 10 ints */
    for (int i = 0; i < 10; i++)
    {
        buffer.Append(i);
    }
    
    /* Write 10 ints */
    for (int i = 0; i < 10; i++)
    {
        buffer.Append(i);
    }
    
    /* Read 20 ints block */
    block = buffer.Read(20);
    buffer.Skip(block.Length());
    
    /* Write 10 ints */
    for (int i = 0; i < 10; i++)
    {
        buffer.Append(i);
    }
    
    /* Read 10 ints block */
    block = buffer.Read(10);
    buffer.Skip(block.Length());
    
    /* Print out the block */
    for (int i = 0; i < block.Length(); i++)
    {
        if (block.At(i) != (i%10))
        {
            return 1; /* Error: buffer contents don't match */
        }
        
        std::cout << block.At(i) << std::endl;
    }
    
    return 0;
}