/*TODO: To speed things up we could have separate triple buffers for objects that rarely
move, only scrolling would invalidate them (or creating/destructing) */

#pragma once
#include <vector>
#include <mutex>

using namespace std;

/*

Allows efficient (asynchronous) swapping of data between a producer & consumer who work at different rates 

In case of rendering:

One buffer is accessed by the renderer (consumer) to read from and one buffer by the gameloop/producer to write to (and possibly read from).
Once the gameloop is done with one game cycle he exchanges his buffer with the third and unused buffer. His old buffer is marked for rendering.
The render thread, on every render loop, checks which buffer is marked for rendering and reads from it.
The render thread can run faster than the gameloop and thus read for multiple cycles from the same buffer.
If the gameloop runs faster the render thread will simply skip the rendering of some game cycles.
There must be an atomic operation to exchange the pointers to the buffers. The two threads never have to wait for each other.

*/
class TripleBuffer
{
public:
	TripleBuffer();
	TripleBuffer(int maxCapacity);
	~TripleBuffer();

	void AddElement(class T* element);
	void ExchangeBuffers();
	
	int maxElements = INT32_MAX;

	mutex bufferMutex;
	vector<vector<class T*>> buffers;

	// this allows us to keep old data in the buffer without clearing it
	int consumerBufferSize = 0; 

	int consumerBufferID = 0;
	int nextConsumerBufferID = 0;
	int producerBufferID = 1;
};
