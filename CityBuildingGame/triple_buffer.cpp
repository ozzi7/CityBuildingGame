#include "stdafx.h"
#include "triple_buffer.h"

using namespace std;

TripleBuffer::TripleBuffer(int maxCapacity)
{
	maxElements = maxCapacity;

	buffer0 = new vector<class T*>();
	buffer1 = new vector<class T*>();
	buffer2 = new vector<class T*>();
}
TripleBuffer::~TripleBuffer() {};
void TripleBuffer::AddData(class T*)
{
	if(
}
void TripleBuffer::ExchangeBuffers()
{
	bufferMutex.lock();
	swap(buffer0, buffer1);
}




















































/986*/////////////////////////////////6/98