#pragma once

#include "engine_core/engine_types.h"

typedef struct FrameBuffer {
	u32 frameBufferObject;
	u32 textureObject;
	u32 clearBit;
	float clearColor[4];
	bool followWindowSize;
} FrameBuffer;


typedef struct FrameBufferDescriptor {
	u32 width;
	u32 height;
	u32 format;
	float clearColor[4];
	u32 clearBit;
	bool useRenderbuffer;	
} FrameBufferDescriptor;

FrameBuffer* FrameBuffer_create(const FrameBufferDescriptor desc);
void FrameBuffer_destroy(FrameBuffer* frameBuffer);