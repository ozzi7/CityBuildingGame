#include "stdafx.h"
#include "triple_buffer.h"

using namespace std;

TripleBuffer::TripleBuffer()
{
	TripleBuffer::TripleBuffer(INT32_MAX);
}

TripleBuffer::TripleBuffer(int aMaxElements)
{
	maxElements = aMaxElements;

	buffers = std::vector<std::vector<class T*>>();

	for (auto i = 0; i < 3; ++i) {
		buffers.push_back(std::vector<class T*>());
	}
}
TripleBuffer::~TripleBuffer() {};
void TripleBuffer::AddElement(class T* element)
{
	if (buffers[consumerBufferID].size() < maxElements) {
		buffers[consumerBufferID].push_back(element);
	}
}
void TripleBuffer::AddSubElement(class T* element)
{
	if (element.type == Fir) {
		buffers[producerBufferID][0]push_back(element);
	}
	if[consumerBufferID].size() < maxElements) {
		buffers[consumerBufferID].push_back(element);
	}
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
