# EmbeddedRingBuffer
This is a header only ring buffer that is designed to work on embedded devices.

There are two objects, RingBuffer and Block. The block represents a set of items in the ringbuffer. You can write single items to the ring buffer and read many at one time. It was designed to handle ISR routines, so bytes can be writting in an interrupt handler and read in large chunks to output to slower medium like SD cards. To use this in an ISR setting, the Skip parameter of read must be set to false, then you must manually skip it the number of reads the block contains once you are finished with the block.

It does not use malloc/free since the size is defined as a template parameter and is written in plain C++98 with no special features used.

## How to use it.

With autoskip enabled.
```
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
```

Without autoskip.
```
RingBuffer<100, int> buffer;
Block<int> block;

void SD_WriteCompleted()
{
    buffer.Skip(block.Length()); /* Manually skip the buffer to prevent corruption */
}

void ISR()
{
    /* Perform an action like sample temperature sensor. */

    buffer.Append(0);
}

int main()
{
    while (true)
    {
        /* Read a block */
        block = buffer.Read(100);

        SD.write(block.Start(), block.Length()); /* non blocking mode */
    }


    return 0;
}
```


Please reference the doxygen comments for more details.


## References
  - http://www.mathcs.emory.edu/~cheung/Courses/171/Syllabus/8-List/array-queue2.html

## Todo
  - Test the buffer thoroughly to debug.
  - Benchmark it.
