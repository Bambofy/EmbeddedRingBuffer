#include <iostream>

#include "RingBuffer.h"

int main()
{
	RingBuffer<100, int> buffer;
	Block<int> block;

	/* Write 100 ints */
	for (int i = 0; i < buffer.Length(); i++)
	{
		buffer.Append(i);
	}

	/* Read a block */
	block = buffer.Read(100);

	/* Print out the block */
	for (int i = 0; i < block.Length(); i++)
	{
		std::cout << block.At(i) << std::endl;
	}

	/* Read another block */
	block = buffer.Read(1000);

	/* Print out the block */
	for (int i = 0; i < block.Length(); i++)
	{
		std::cout << block.At(i) << std::endl;
	}


	return 0;
}
