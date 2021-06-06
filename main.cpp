#include <iostream>

/**
 * @brief	A block represents a continuous section
 * 			of the ring buffer.
 * @tparam T	The type of data stored in the ring buffer.
 */
template<class T>
class Block
{
public:
	Block();
	
	~Block();

	/*
	 * @brief	Sets the block's starting
	 * 			position to a point in memory.
	 */
	void SetStart(T* start);

	/*
	 * @brief	Sets the number of items in the
	 * 			block.
	 */
	void SetLength(unsigned int length);

	/*
	 * @retu	Returns the block's starting
	 * 			point in memory.
	 * @returns
	 */
	T* Start();

	/*
	 * @brief
	 */
	unsigned int Length();

	T At(unsigned int offset);

private:
	T* start;

	unsigned int length;
};

template<class T>
Block<T>::Block() : start(nullptr), length(0)
{
}

template<class T>
Block<T>::~Block()
{
}

template<class T>
void Block<T>::SetStart(T* start)
{
	this->start = start;
}

template<class T>
void Block<T>::SetLength(unsigned int length)
{
	this->length = length;
}

template<class T>
T* Block<T>::Start()
{
	return this->start;
}

template<class T>
unsigned int Block<T>::Length()
{
	return this->length;
}

template<class T>
T Block<T>::At(unsigned int offset)
{
	return this->start[offset];
}


/**
 * @attention	The ring buffer can only contain Length-1 number of entries,
 * 				because the last index is reserved for overrun checks.
 *
 * @tparam Length
 * @tparam T
 */
template<unsigned int LENGTH, class T>
class RingBuffer
{
public:
	RingBuffer();

	~RingBuffer();

	/*
	 * @brief 	Append a value the end of the
	 * 			buffer.
	 */
	void Append(T value);

	/*
	 * @brief	Retrieve a continuous block of
	 * 			valid buffered data.
	 */
	Block<T> Read(unsigned int num_reads_requested, bool skip = true);

	/*
	 * @brief	Advances the read position.
	 * 			This is so the client can
	 * 			perform an action after Read().
	 * 			If the Read() function changed
	 * 			the read position, then if
	 * 			a concurrent process Appends(),
	 * 			the appending may write into
	 * 			data being read by the client.
	 */
	void Skip(unsigned int num_reads);
	
	bool Overrun();
	
	unsigned int Length();

private:
	unsigned int read_position;
	unsigned int write_position;

	T data[LENGTH];

	bool overrun_flag;
};

template<unsigned int LENGTH, class T>
RingBuffer<LENGTH, T>::RingBuffer() : read_position(0), write_position(0)
{
}

template<unsigned int LENGTH, class T>
RingBuffer<LENGTH, T>::~RingBuffer()
{
	memset(data, 0, LENGTH);
}

template<unsigned int LENGTH, class T>
void RingBuffer<LENGTH, T>::Append(T value)
{
	/*
	 * If the next position is where the read cursor
	 * is then we have a full buffer.
	 */
	if (((write_position + 1U) % LENGTH) == read_position)
	{
		overrun_flag = true;
	}
	else
	{
		overrun_flag         = false;
		data[write_position] = value;
		write_position       = (write_position + 1U) % LENGTH;
	}
}

template<unsigned int LENGTH, class T>
Block<T> RingBuffer<LENGTH, T>::Read(unsigned int num_reads_requested,
									 bool skip)
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
		/*
		 * If the block bridges zero then return the maximum
		 * number of reads til the end of the buffer.
		 */
		unsigned int num_reads_to_end;
		bool block_length_surpasses_end;

		num_reads_to_end           = LENGTH - read_position;
		block_length_surpasses_end = num_reads_requested > num_reads_to_end;

		if (block_length_surpasses_end)
		{
			block.SetStart(&(data[read_position]));
			block.SetLength(num_reads_to_end);
			
			if (skip)
			{
				read_position = (read_position + num_reads_to_end) % LENGTH;
			}
		}
		else
		{
			block.SetStart(&(data[read_position]));
			block.SetLength(num_reads_requested);
			
			if (skip)
			{
				read_position = (read_position + num_reads_requested) % LENGTH;
			}
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

		/*
		 * If the number of reads requested exceeds the
		 * number of reads available, then it is underran.
		 */
		if (num_reads_requested > num_reads_to_write_position)
		{
			max_num_reads = num_reads_to_write_position;
		}
		else
		{
			max_num_reads = num_reads_requested;
		}

		block.SetStart(&(data[read_position]));
		block.SetLength(max_num_reads);
		
		if (skip)
		{
			read_position = (read_position + max_num_reads) % LENGTH;
		}
	}

	return block;
}

template<unsigned int LENGTH, class T>
void RingBuffer<LENGTH, T>::Skip(unsigned int num_reads)
{
	read_position = (read_position + num_reads) % LENGTH;
}

template<unsigned int LENGTH, class T>
bool RingBuffer<LENGTH, T>::Overrun()
{
	return overrun_flag;
}

template<unsigned int LENGTH, class T>
unsigned int RingBuffer<LENGTH, T>::Length()
{
	return LENGTH;
}


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
