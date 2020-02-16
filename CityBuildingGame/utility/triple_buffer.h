/*TODO: To speed things up we could have separate triple buffers for objects that rarely
move, only scrolling would invalidate them (or creating/destructing)
TODO2: Don't resize internal datastructures all the time (vectors aren't resized on clear necessarily)
Note: For more than one consumer => Add additional lock in GetConsumerBuffer()*/

#pragma once
#include <vector>
#include <mutex>
#include <render_data.h>

/*

Allows efficient (asynchronous) swapping of data between a producer & consumer who work at different rates

In case of rendering:

One buffer is accessed by the renderer (consumer) to read from and one buffer by the gameloop/producer to write to (and possibly read from).
Once the gameloop is done with one game cycle he exchanges his buffer with the third and unused buffer. His old buffer is marked for rendering.
The render thread, on every render loop, checks which buffer is marked for rendering and reads from it.
The render thread can run faster than the gameloop and thus read for multiple cycles from the same buffer.
If the gameloop runs faster, the render thread will simply skip the rendering of some game cycles.
There must be an atomic operation to exchange the pointers to the buffers. The two threads never have to wait for each other.

*/
template <class T>
class TripleBuffer
{
protected:
	void TripleBuffer<T>::exchangeConsumerBuffer()  // call this before consumption cycle
	{
		bufferMutex.lock();
		if (newDataReady)
		{
			std::swap(idleBuffer, consumerBufferID);
			newDataReady = false;
		}
		bufferMutex.unlock();
	}

public:
	TripleBuffer<T>::TripleBuffer()
	{
		for (int i = 0; i < 3; ++i)
		{
			buffers.push_back(new T);
		}
	}

	TripleBuffer<T>::~TripleBuffer()
	{
		delete buffers[0];
		delete buffers[1];
		delete buffers[2];
	}

	void TripleBuffer<T>::ExchangeProducerBuffer()  // call this after production cycle
	{
		bufferMutex.lock();
		std::swap(producerBufferID, idleBuffer);
		newDataReady = true;
		bufferMutex.unlock();
	}

	T* TripleBuffer<T>::GetConsumerBuffer()
	{
		exchangeConsumerBuffer();
		return buffers[consumerBufferID];
	}

	T* TripleBuffer<T>::GetProducerBuffer()
	{
		buffers[producerBufferID]->ClearData();
		return buffers[producerBufferID];
	}

	std::mutex bufferMutex;
	std::vector<T*> buffers;

	bool newDataReady = false;

	int consumerBufferID = 0;
	int producerBufferID = 1;
	int idleBuffer = 2;
};
