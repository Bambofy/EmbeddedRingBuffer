# Embedded Ring Buffer
This is a header only ring buffer that is designed to work on embedded devices.

There are two objects, RingBuffer and Block. The block represents a set of items in the ringbuffer. You can write single items to the ring buffer and read many at once. 
It was designed to handle ISR routines and non-blocking devices therefore bytes can be wrote in an interrupt handler and read in large chunks to output to slower medium like SD cards. 

It does not use malloc/free since the size is defined as a template parameter and is written in plain C++98 with no special features used.

## Sponsors

 <a href="https://arnmore.co.uk/"><img src="https://arnmore.co.uk/wp-content/uploads/2019/08/arnmore_logo_v4.svg" width="300" height="100"></a><br/>
 
## How to use it.

Once a block has been used you must Skip() the buffer the number of reads you made, this is so that writing to the buffer does not corrupt the blocks data as it is being used.


Without ISR
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
    buffer.Skip(block.Length());

    /* Print out the block */
    for (int i = 0; i < block.Length(); i++)
    {
        std::cout << block.At(i) << std::endl;
    }

    /* Read another block */
    block = buffer.Read(1000);
    buffer.Skip(block.Length());

    /* Print out the block */
    for (int i = 0; i < block.Length(); i++)
    {
        std::cout << block.At(i) << std::endl;
    }


    return 0;
}
```

With ISR.
```
RingBuffer<100, int> buffer;
Block<int> block;
bool writing = false;

void SD_WriteCompleted()
{
    buffer.Skip(block.Length()); /* Manually skip the buffer to prevent corruption */
    
    writing = false;
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
        if (writing) continue;
        
        /* Read a block */
        block = buffer.Read(100, false); /* NOTE: here the second parameter disables the automatic skipping */

        writing = true;
        
        SD.write(block.Start(), block.Length()); /* non blocking mode */
    }


    return 0;
}
```


Please reference the doxygen comments for more details.

## Issues

### Blocks cannot span the 0th index of the buffer.
Since the ring buffer is just an array, to return a block that overlaps the 0th index would mean having 2 regions of the array to process, however the sd card writing (DMA) only works on single arrays at a time.
This means that if you have a ringbuffer of 100 items at your reading from position 98, the maximum number of elements a block can return is 2. 
This may sound unrealistic but this was designed for SD cards that write extremely large blocks at a time (>30KB) using buffers that are large enough to store a considerable number of elements before requiring writing to the sd card.
In practice the ring buffers write position will never exceed far from the read position since the SD card is emptying it constantly. You must calculate the timing of your input data and output data carefully for the buffer.

## References
  - http://www.mathcs.emory.edu/~cheung/Courses/171/Syllabus/8-List/array-queue2.html

## Todo
  - Test the buffer thoroughly to debug.
  - Benchmark it.
