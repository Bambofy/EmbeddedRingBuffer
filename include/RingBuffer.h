/* Changelog ------------------------------------------------------------------
 *  06/06/2021     Initial version
 * ---------------------------------------------------------------------------*/

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include "Block.h"

#include <string.h>

/**
 * @brief            A ring buffer is a FIFO structure that can be used to
 *                     spool data between devices.
 *
 *                     There is a Skip() function that allows the client to
 *                     control when the read cursor is changed. This is so the
 *                     client can perform an action after Read() without the
 *                     write cursor overwriting data while the read block is used.
 *
 *                     For e.g with the sequence of events:
 *                         1.    Read(1000, false)
 *                         2.    Busy writing to sd card for 5 seconds
 *                         3.    Skip()
 *
 *                     Because the skip isn't called until the writing
 *                     has finished, another thread can .Append() without
 *                     corrupting the data being written.
 *
 *
 * @attention        The ring buffer can only contain Length-1 number of entries,
 *                   because the last index is reserved for overrun checks.
 *
 * @tparam Length    The length of the backing store array.
 * @tparam T         The type of data stored.
 */
template<unsigned int LENGTH, class T>
class RingBuffer
{
public:
    RingBuffer() : read_position(0), write_position(0)
    {
    }

    ~RingBuffer()
    {
    }

    /**
     * @brief     Appends a value the end of the
     *            buffer.
     */
    void Append(T value)
    {
        /*
         * If the next position is where the read cursor
         * is then we have a full buffer.
         */
        bool buffer_full;

        buffer_full = ((write_position + 1U) % LENGTH) == read_position;

        if (buffer_full)
        {
            /*
             * Tried to append a value while the buffer is full.
             */
            overrun_flag = true;
        }
        else
        {
            /*
             * Buffer isn't full yet, write to the curr write position
             * and increment it by 1.
             */
            overrun_flag         = false;
            data[write_position] = value;
            write_position       = (write_position + 1U) % LENGTH;
        }
    }

    /**
     * @brief                        Retrieve a continuous block of
     *                               valid buffered data.
     * @param num_reads_requested    How many reads are required.
     * @return                       A block of items containing the maximum
     *                               number the buffer can provide at this time.
     */
    Block<T> Read(unsigned int num_reads_requested)
    {
        bool bridges_zero;
        Block<T> block;

        /*
          * Make sure the number of reads does not bridge the 0 index.
          * This is because we can only provide 1 contiguous block at
          * a time.
          */
        bridges_zero = (read_position > write_position);

        if (bridges_zero)
        {
            unsigned int reads_to_end;
            bool req_surpasses_buffer_end;

            reads_to_end             = LENGTH - read_position;
            req_surpasses_buffer_end = num_reads_requested > reads_to_end;

            if (req_surpasses_buffer_end)
            {
                /*
                 * If the block requested exceeds the buffer end. Then
                 * return a block that reaches the end and no more.
                 */
                block.SetStart(&(data[read_position]));
                block.SetLength(reads_to_end);
            }
            else
            {
                /*
                 * If the block requested does not exceed 0
                 * then return a block that reaches the number of reads required.
                 */
                block.SetStart(&(data[read_position]));
                block.SetLength(num_reads_requested);
            }
        }
        else
        {
            /*
             * If the block doesn't bridge the zero then
             * return the maximum number of reads to the write
             * cursor.
             */
            unsigned int max_num_reads;
            unsigned int num_reads_to_write_position;

            num_reads_to_write_position = (write_position - read_position);

            if (num_reads_requested > num_reads_to_write_position)
            {
                /*
                 * If the block length requested exceeds the
                 * number of items available, then restrict
                 * the block length to the distance to the write position.
                 */
                max_num_reads = num_reads_to_write_position;
            }
            else
            {
                /*
                 * If the block length requested does not exceed the
                 * number of items available then the entire
                 * block is valid.
                 */
                max_num_reads = num_reads_requested;
            }

            block.SetStart(&(data[read_position]));
            block.SetLength(max_num_reads);
        }

        read_position = (read_position + num_reads_requested) % LENGTH;

        return block;
    }

    /**
     * @brief    Advances the read position.
     *
     */
    void Skip(unsigned int num_reads)
    {
    	if ((read_position + num_reads) % LENGTH <= write_position){
    		read_position = (read_position + num_reads) % LENGTH;
    	}else{
    		read_position = write_position;
    	}
    }

    bool Overrun()
    {
        return overrun_flag;
    }

    /**
     * @brief    The total size of the ring buffer including the full position.
     *
     */
    unsigned int Length()
    {
        return LENGTH;
    }

    /**
     * @brief    Returns the number of reads available.
     *
     */
    unsigned int Available()
    {
        bool bridges_zero;
        unsigned available_reads;

        bridges_zero = read_position > write_position;
        available_reads = 0;

        if (bridges_zero)
        {
            /* Add the number of reads to zero, and number of reads from 0 to the write cursor */
            unsigned int num_reads_to_zero;
            unsigned int num_reads_to_write_position;
            
            num_reads_to_zero = LENGTH - read_position;
            num_reads_to_write_position = write_position;

            available_reads = num_reads_to_zero + num_reads_to_write_position;
        }
        else
        {
            /* The number of available reads is between the write position and the read position. */
            available_reads = write_position - read_position;
        }

        return available_reads;
    }

private:
    volatile unsigned int read_position;
    volatile unsigned int write_position;

    T data[LENGTH];

    bool overrun_flag;
};

#endif
