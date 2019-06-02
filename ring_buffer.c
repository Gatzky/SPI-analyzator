/* Includes ------------------------------------------------------------------*/
#include <assert.h>
#include "ring_buffer.h"


bool RingBuffer_Init(RingBuffer *ringBuffer, char *dataBuffer, size_t dataBufferSize) 
{
	assert(ringBuffer);
	assert(dataBuffer);
	assert(dataBufferSize > 0);
	
	if ((ringBuffer) && (dataBuffer) && (dataBufferSize > 0)) {
	  	(*ringBuffer).head=0;
	    (*ringBuffer).tail=0;
	    (*ringBuffer).fullFlag=false;
	    (*ringBuffer).numOfElem=dataBufferSize;
	    (*ringBuffer).buff=dataBuffer;
	    return true;
	}
	
	return false;
}

bool RingBuffer_Clear(RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
	    (*ringBuffer).head=0;
	    (*ringBuffer).tail=0;
	    (*ringBuffer).fullFlag=false;
	    return true;
	}
	return false;
}

bool RingBuffer_IsEmpty(const RingBuffer *ringBuffer)
{
  assert(ringBuffer);	
	if(RingBuffer_GetLen(ringBuffer)!=0){
	    return false;
	}
	
	return true;
}

size_t RingBuffer_GetLen(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
	    if((*ringBuffer).fullFlag==true)
	        return (*ringBuffer).numOfElem;
		else if((*ringBuffer).tail<=(*ringBuffer).head){
		    return (*ringBuffer).head-(*ringBuffer).tail;
		} else{
		    return (*ringBuffer).numOfElem-(*ringBuffer).tail+(*ringBuffer).head;
		}
	}
	return 0;
	
}

size_t RingBuffer_GetCapacity(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
		return (*ringBuffer).numOfElem;
	}
	return 0;	
}


bool RingBuffer_PutChar(RingBuffer *ringBuffer, char c)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
		if(RingBuffer_GetLen(ringBuffer)<(*ringBuffer).numOfElem){
		    *((*ringBuffer).buff+(*ringBuffer).head)=c;
    		if((*ringBuffer).head==(*ringBuffer).numOfElem-1)
    		    (*ringBuffer).head=0;
    		else
    		    (*ringBuffer).head++;
    		if((*ringBuffer).head==(*ringBuffer).tail)
    		    (*ringBuffer).fullFlag=true;
    		return true;
		} else {
		    return false;
		}
	}
	return false;
}

bool RingBuffer_GetChar(RingBuffer *ringBuffer, char *c)
{
	assert(ringBuffer);
	assert(c);
	
  if ((ringBuffer) && (c)) {
    if(RingBuffer_IsEmpty(ringBuffer)!=true){
        *c=*((*ringBuffer).buff+(*ringBuffer).tail);
		if((*ringBuffer).tail==(*ringBuffer).numOfElem-1)
		    (*ringBuffer).tail=0;
		else
		    (*ringBuffer).tail++;
		if((*ringBuffer).fullFlag==true)
		    (*ringBuffer).fullFlag=false;		
		return true;
    } else
        return false;
	}
	return false;
}
