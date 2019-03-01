#include "stdafx.h"
#include "triple_buffer.h"

using namespace std;

template <class T>
TripleBuffer<T>::TripleBuffer() {
	for (int i = 0; i < 3; ++i) {
		buffers.push_back(new T);
	}
}
template <class T>
TripleBuffer<T>::~TripleBuffer() {
	delete buffers[0];
	delete buffers[1];
	delete buffers[2];
}
template <class T>
void TripleBuffer<T>::ExchangeProducerBuffer()
{
	bufferMutex.lock();
	std::swap(consumerBufferID, idleBuffer);
	newDataReady = true;
	bufferMutex.unlock();
}
template <class T>
void TripleBuffer<T>::ExchangeConsumerBuffer()
{
	bufferMutex.lock();
	if (newDataReady) {
		std::swap(idleBuffer, producerBufferID);
		newDataReady = false;
	}
	bufferMutex.unlock();
}
template <class T>
T TripleBuffer<T>::GetConsumerBuffer()
{
	return buffers[consumerBufferID];
}