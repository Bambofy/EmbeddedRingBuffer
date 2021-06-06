/* Changelog ------------------------------------------------------------------
 *  06/06/2021     Initial version
 * ---------------------------------------------------------------------------*/

#ifndef RINGBUFFER_BLOCK_H
#define RINGBUFFER_BLOCK_H

#include <stddef.h>

/**
 * @brief        A block represents a continuous section
 *               of the ring buffer.
 * @tparam T     The type of data stored in the ring buffer.
 */
template<class T>
class Block
{
public:
    Block() : start(NULL), length(0)
    {
    }

    ~Block()
    {
    }

    /**
     * @brief    Sets the block's starting
     *           position to a point in memory.
     */
    void SetStart(T* start)
    {
        this->start = start;
    }

    /**
     * @brief    Sets the number of items in the
     *           block.
     */
    void SetLength(unsigned int length)
    {
        this->length = length;
    }

    /**
     * @return    The block's starting
     *            point in memory.
     */
    T* Start()
    {
        return this->start;
    }

    /**
     * @return    The number of items in the block.
     */
    unsigned int Length()
    {
        return this->length;
    }

    /**
     * @param index        The index of the item in the block.
     * @return             The item in the block at the index.
     */
    T At(unsigned int index)
    {
        return this->start[index];
    }


private:
    T* start;

    unsigned int length;
};


#endif