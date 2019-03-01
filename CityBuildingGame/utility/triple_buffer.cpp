#include "stdafx.h"
#include "triple_buffer.h"

using namespace std;

TripleBuffer::TripleBuffer() {
	for (int i = 0; i < 3; ++i) {
		buffers.push_back(new BufferElement);
	}
}
TripleBuffer::~TripleBuffer() {
	delete buffers[0];
	delete buffers[1];
	delete buffers[2];
}
template <class T>
void TripleBuffer::AddElement(T element)
{
	buffers[consumerBufferID]->AddElement(element);
}
void TripleBuffer::ExchangeProducerBuffer()
{
	bufferMutex.lock();
	std::swap(consumerBufferID, idleBuffer);
	bufferMutex.unlock();
}
void TripleBuffer::ExchangeConsumerBuffer()
{
	bufferMutex.lock();
	std::swap(idleBuffer, producerBufferID);
	bufferMutex.unlock();
}
