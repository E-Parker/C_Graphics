#include "engine_core/engine_io.h"
#include "engine_core/engine_error.h"

#include "string.h"

ecode Engine_read(const IODescriptor iodesc) {
    if (!iodesc.file || !iodesc.buffer) {
        return ERROR_BADPOINTER;
    }

    if (!iodesc.bufferSize) {
        return ERROR_BADVALUE;
    }

    u64 bufferSize = iodesc.bufferSize;

    fseek((FILE*)iodesc.file, 0, SEEK_END);
    u64 fileSize = ftell(iodesc.file);
    fseek(iodesc.file, 0, SEEK_SET);

    if (iodesc.flags & ENGINE_IO_ZERO_BUFFER) {
        memset(iodesc.buffer, '\0', iodesc.bufferSize);
    }

    if (iodesc.flags & ENGINE_IO_ADD_NULL_TERMINATOR) {
        bufferSize--;
    }

    if (fileSize > bufferSize) {
        return ERROR_IO_FILETOBIG;
    }

    char* buffer = (char*)iodesc.buffer;

    for (u64 i = 0; i < fileSize / ENGINE_BUFFER_READ_STEP; ++i, buffer += ENGINE_BUFFER_READ_STEP) {
        fread(buffer, sizeof(char), ENGINE_BUFFER_READ_STEP, iodesc.file);
        fflush(iodesc.file);
    }

    u64 remainder = fileSize % ENGINE_BUFFER_READ_STEP;

    if (remainder) {
        fread(buffer, 1, remainder, iodesc.file);
        fflush(iodesc.file);
    }

    if (iodesc.flags & ENGINE_IO_ADD_NULL_TERMINATOR) {
        char* c = ((char*)iodesc.buffer) + fileSize;
        *c = '\0';
    }

    return ferror(iodesc.file);
}

ecode Engine_write(const IODescriptor iodesc) {
    return ERROR_GENERIC;
}