#pragma once



typedef struct AsyncDeserializeHandler {
    // Deserializes the raw binary of a file in chunks. 
    // This should distribute the prossesing over multiple frames to avoid any 
    // 

    void (*On_Begin)(void*);
    void (*ProssesChunk(void*)); 
    void (*On_End)(void*);
    char* buffer;
} AsyncDeserializeHandler;



