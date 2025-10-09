#include "engine/shader/framebuffer.h"

FrameBuffer FrameBuffers[0x40] = { 0, };
u64 UnusedBuffers[0x40] = { 1, };
u64 UnusedBufferCount = 0;

FrameBuffer* FrameBuffer_create (const FrameBufferDescriptor desc) {

	// If you specify a width or height, both must be specified.
	if (desc.width && !desc.height || desc.height && !desc.width) {
		return NULL;
	}

	if (!desc.format) {
		return NULL;
	}

	FrameBuffer* framebuffer = &FrameBuffers[UnusedBufferCount];

	framebuffer->clearBit = desc.clearBit;

	return framebuffer;
}

void FrameBuffer_destroy (FrameBuffer* frameBuffer) {

}
