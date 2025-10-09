#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "engine_core/engine_error.h"
#include "engine_core/hash_table.h"
#include "engine_core/string.h"
#include "engine/math.h"

#include "engine/shader/shader_uniform.h"
#include "engine/shader.h"

#define MAX_ALIAS_SIZE 512

HashTable* UniformBufferTable = NULL;
HashTable* ShaderProgramTable = NULL;
//HashTable* TextureTable = NULL;
HashTable* ShaderCompilationTable = NULL;

void InitShaders () {
    UniformBufferTable = HashTable_create(UniformBuffer, 4);
    ShaderProgramTable = HashTable_create(Shader, 4);
    // TextureTable = HashTable_create(Texture, 512);
}

ecode DereferenceShaders () {
    // Function to de-reference all shaders and shader objects. 
    // After this is called, all functions will fail until InitShaders() is called again.
    //

    // TODO: For some reason, if you try destroying the uniform buffers first, everything falls apart. Calling it this way works?? 
    // This is probably a massive security vulnerability.
    
    // Destroy the shader programs.
    for (HashTable_array_iterator(ShaderProgramTable)) {
        Shader* shader = HashTable_array_at(Shader, ShaderProgramTable, i);
        Shader_deinitialize(shader);
    }

    // Destroy the uniform buffers.
    for (HashTable_array_iterator(UniformBufferTable)) {
        UniformBuffer* buffer = HashTable_array_at(UniformBuffer, UniformBufferTable, i);
        UniformBuffer_deinitialize(buffer);
    }


    // TODO: Check if this is still necessary. Textures *should* manage themselves.
    // Destroy the textures.
    //for (u64 i = 0; i < TextureTable->Size; i++) {
    //    if (TextureTable->Array[i].Value) {
    //        Texture_destroy((Texture**)&(TextureTable->Array[i].Value));
    //    }
    //}

    // Now destroy the tables which store them. 
    HashTable_destroy(&UniformBufferTable);
    HashTable_destroy(&ShaderProgramTable);
    return 0;
}

void UniformBuffer_deinitialize (UniformBuffer* buffer) {

    if (!buffer) {
        return;
    }

    if (buffer->References > 1) {
        buffer->References--;
        return;
    }

    TryDestroyUniforms:
    if (!buffer->Uniforms) {
        goto TryDestroyUniformStructs;
    }

    glDeleteBuffers(1, &buffer->BufferObject);

    for (HashTable_array_iterator(buffer->Uniforms)) {
        Uniform* uniform = HashTable_array_at(Uniform, buffer->Uniforms, i);
        if (uniform && !String_invalid(uniform->Alias)) {
            String_free_dirty(&uniform->Alias);
        }
        if (uniform && uniform->UniformType == UNIFORM_TYPE_SINGLE) {
            free(uniform->Data);
        }
    }

    HashTable_destroy(&buffer->Uniforms);

    TryDestroyUniformStructs:
    if (!buffer->UniformStructs) {
        goto RemoveFromHashTable;
    }

    for (HashTable_array_iterator(buffer->UniformStructs)) {
        UniformStruct* uniformStruct = HashTable_array_at(UniformStruct, buffer->UniformStructs, i);
        if (uniformStruct && !String_invalid(uniformStruct->Alias)) {
            String_free_dirty(&uniformStruct->Alias);
        }
    }

    HashTable_destroy(&buffer->UniformStructs);

    //Destroy the shared data block.
    free(buffer->buffer);

    RemoveFromHashTable:
    String alias;
    String_create_dirty(&buffer->Alias, &alias);
    String_free_dirty(&buffer->Alias);
    HashTable_remove(UniformBufferTable, alias);
    String_free_dirty(&alias);

}

void internal_UniformBuffer_set_region (const UniformBuffer* buffer, const u64 byteIndex, const u64 regionSizeInBytes, const void* data) {
    glBindBuffer(GL_UNIFORM_BUFFER, buffer->BufferObject);
    glBufferSubData(GL_UNIFORM_BUFFER, byteIndex, regionSizeInBytes, data);
    glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
}

void internal_UniformBuffer_set_all (const UniformBuffer* buffer, const void* data) {
    glBindBuffer(GL_UNIFORM_BUFFER, buffer->BufferObject);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, buffer->Size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
}

void internal_UniformBuffer_buffer (const UniformBuffer* buffer) {
    glBindBuffer(GL_UNIFORM_BUFFER, buffer->BufferObject);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, buffer->Size, buffer->buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
}

void internal_UniformBuffer_set (UniformBuffer* buffer, const char* alias, void* data) {
    // set the value of an item in a buffer by its variable name.

    if (!buffer) {
        printf("Error UniformBuffer_set:\t\tBuffer value \"%s\" does not exist.\n", alias);
        return;
    }

    Uniform* uniform;
    UniformBuffer_get_Uniform(buffer, alias, &uniform);

    if (uniform) {
        Uniform_set_data(uniform, data);
        buffer->ChangesMade++;
    }
}

void UniformBuffer_get_Uniform (const UniformBuffer* buffer, const char* alias, Uniform** outVal) {
    // Get an item in a buffer by its variable name.
    String aliasString = String_from_ptr(alias);
    if(buffer) HashTable_find_reference(buffer->Uniforms, aliasString, outVal);
}

void UniformBuffer_get_Struct (const UniformBuffer* buffer, const char* alias, UniformStruct** outVal) {
    // Get the whole buffer from its name.
    String aliasString = String_from_ptr(alias);
    if(buffer) HashTable_find_reference(buffer->UniformStructs, aliasString, outVal);
}

void internal_UniformBuffer_set_Struct (UniformBuffer* buffer, const char* alias, const char* memberAlias, void* data) {
    // upload data to the uniform struct.
    UniformStruct* uniformStruct = NULL;
    Uniform* uniform = NULL;
    String aliasString = String_from_ptr(alias);
    HashTable_find_reference(buffer->UniformStructs, aliasString, &uniformStruct);
    
    if (!uniformStruct) {
        printf("Error UniformBuffer_set_Struct\t: Uniform Structure \"%s\" does not exist.\n", alias);
        return;
    
    }

    UniformStruct_set_member (uniformStruct, memberAlias, data);

    buffer->ChangesMade++;
}

void internal_UniformBuffer_set_Struct_at (UniformBuffer* buffer, const char* alias, const char* memberAlias, u64 i, void* data) {
    
    if (!buffer) {
        printf("Error UniformBuffer_set_Struct_at\t: Uniform Buffer \"%s\" does not exist.\n", alias);
        return;
    }

    UniformStruct* uniformStruct;
    Uniform* uniform;
    String aliasString = String_from_ptr(alias);
    HashTable_find_reference(buffer->UniformStructs, aliasString, &uniformStruct);
    if (!uniformStruct) {
        printf("Error UniformBuffer_set_Struct_at\t: Uniform Structure \"%s\" does not exist.\n", alias);
        return;
    }

    UniformStruct_get_member (uniformStruct, memberAlias, &uniform);
    if (!uniform) {
        printf("Error UniformBuffer_set_Struct_at\t: Structure \"%s\" does not contain a member named \"%s\"\n", alias, memberAlias);
        return;
    }

    UniformStruct_set_member_at (uniformStruct, memberAlias, i, data);
    buffer->ChangesMade++;
}

UniformBuffer* UniformBuffer_get_self (const char* alias) {
    UniformBuffer* outVal;
    String aliasString = String_from_ptr(alias);
    HashTable_find_reference(UniformBufferTable, aliasString, &outVal);
    
    if (!outVal) {
        printf("Error UniformBuffer_get_self:\t\tBuffer \"%s\" does not exist.\n", alias);
    }
    
    return outVal;
}

void UniformBuffer_update_all () {
    // Upload all uniform buffers.

    for (HashTable_array_iterator(UniformBufferTable)) {
        UniformBuffer* buffer = HashTable_array_at(UniformBuffer, UniformBufferTable, i);
        //printf("%s\\t Changes made: %d \n", buffer->Alias.start, buffer->ChangesMade);
        if (buffer && buffer->ChangesMade != 0) {
            internal_UniformBuffer_buffer(buffer);
            buffer->ChangesMade = 0;
        }
    }
}

Uniform* internal_Uniform_create_shared (const UniformInformation info, void* sharedBuffer) {
    // Initialize a Uniform that is a part of a buffer.

    if (info.BlockOffset == -1) {
        return NULL;
    }

    // Get the byte size of the type.
    u64 size = size_from_gl_type(info.Type);

    // allocate enough space for the Uniform header + the space required to store it's data. 
    Uniform* newUniform = (Uniform*)malloc(sizeof(Uniform));
    
    if (!newUniform) {
        return NULL;
    }

    String_create_dirty((String*)&info.Alias, &newUniform->Alias);

    newUniform->Data = (void*)(((u8*)sharedBuffer) + info.BlockOffset);
    newUniform->UniformType = UNIFORM_TYPE_SHARED;
    newUniform->Location = info.Location;
    newUniform->Elements = info.Elements;
    newUniform->Offset = info.BlockOffset;
    newUniform->Size = size;
    newUniform->Stride = size + (16 - (size % 16));
    newUniform->Type = info.Type;

    return newUniform;
}

Uniform* internal_Uniform_create (const UniformInformation info) {
    // Internal function to initialize a Uniform* 
    // 
    //
    
    // Get the byte size of the type.
    u64 size = size_from_gl_type(info.Type);

    // allocate enough space for the Uniform header + the space required to store it's data. 
    Uniform* newUniform = (Uniform*)malloc(sizeof(Uniform));
    
    if (!newUniform) {
        return NULL;
    }

    String_create_dirty ((String*)&info.Alias, &newUniform->Alias);

    newUniform->Data = calloc(size,info.Elements);
    newUniform->UniformType = UNIFORM_TYPE_SINGLE;
    newUniform->Location = info.Location;    
    newUniform->Size = size;
    newUniform->Stride = size + (16 - (size % 16));
    newUniform->Elements = info.Elements;
    newUniform->Offset = info.BlockOffset;
    newUniform->Type = info.Type;

    return newUniform;
}

void internal_Uniform_set_at (Uniform* uniform, u32 i, void* data) {
    // set the value of a particular index in a uniform.

    if (i < 0 || i >= uniform->Elements) {
        printf("Error Uniform_set_at:\tIndex: %d is out of range. The uniform only has %d element(s)\n", i, uniform->Elements);
        return;
    }

    u8* elementAddress = (u8*)uniform->Data;
    u8* dataAddress = (u8*)data + (i * uniform->Size);
    memcpy(elementAddress, dataAddress, uniform->Size);
    
    //printf("Name:\t%s\tStride:\t%04X\tOffset:\t%04X\n",uniform->Alias, (int)(i * uniform->Stride), (int)uniform->Offset);
}

void internal_Uniform_set_data(Uniform* uniform, void* data) {
    // set all of the data. If the uniform is an array it expects elements * size of data. 

    if (uniform->Size == uniform->Stride) {
        memcpy(uniform->Data, data, uniform->Size * uniform->Elements);
        return;
    }

    u8* elementAddress = NULL;
    u8* dataAddress = NULL;

    for (u32 i = 0; i < uniform->Elements; i++) {
        elementAddress = (u8*)uniform->Data;
        dataAddress = (u8*)data + (i * uniform->Size);
        memcpy(elementAddress, dataAddress, uniform->Size);
    }
}


UniformStruct* internal_UniformStruct_create (String alias, const UniformInformation* info, const u16 memberCount, const u64 elements, void* shared) {

    u64 totalSize = 0;
    u64 stride = 0;
    u32* offsets = (u32*)malloc(sizeof(u32) * memberCount);
    Engine_validate(offsets, ENOMEM);

    // for each member, get it's local offset.
    for (u16 i = 0; i < memberCount; i++) {
        offsets[i] = stride;
        stride += (size_from_gl_type(info[i].Type) * info[i].Elements);
        stride += 16 - (stride % 16);  // OpenGL structs are aligned to 16.
    }

    // TODO: rework this. Something is wrong with the stride.
    stride -= 16;

    totalSize = stride * elements;
    UniformStruct* newStruct = (UniformStruct*)malloc(sizeof(UniformStruct));
    Engine_validate(newStruct, ENOMEM);

    String_create_dirty(&alias, &(newStruct->Alias));

    newStruct->Data = shared;
    newStruct->Members = HashTable_create(Uniform, memberCount);
    newStruct->Size = totalSize;
    newStruct->Elements = elements;
    newStruct->Offset = info[0].BlockOffset;

    for (u16 i = 0; i < memberCount; i++) {
        
        // Find the smallest offset in the struct, which is the same as the offset of the struct. 
        if (info[i].BlockOffset < newStruct->Offset) newStruct->Offset = info[0].BlockOffset;

        Uniform* newMember = internal_Uniform_create_shared(info[i], shared);
        newMember->Stride = stride;
        HashTable_insert(newStruct->Members, info[i].Alias, newMember);
    }

    return newStruct;
}

void UniformStruct_get_member (UniformStruct* uniformStruct, const char* alias, Uniform** outVal) {
    String shaderAlias = String_from_ptr(alias);
    HashTable_find_reference(uniformStruct->Members, shaderAlias, outVal);
}

void UniformStruct_set_member_at (UniformStruct* uniformStruct, const char* alias, u64 i, void* data) {
    String shaderAlias = String_from_ptr(alias);
    Uniform* uniform;
    HashTable_find_reference(uniformStruct->Members, shaderAlias, &uniform);
    if (!uniform) {
        printf("Error UniformStruct_set_member\t: could not find Uniform in Structure \"%s\" named \"%s\"\n", uniformStruct->Alias.start, alias);
        return;
    }
    u8* elementAddress = NULL;
    u8* dataAddress = NULL;

    elementAddress = (u8*)uniformStruct->Data + (i * uniform->Stride) + uniform->Offset;
    memcpy(elementAddress, data, uniform->Size);
}

void UniformStruct_set_member (UniformStruct* uniformStruct, const char* alias, void* data) {
    String shaderAlias = String_from_ptr(alias);
    Uniform* uniform;
    HashTable_find_reference(uniformStruct->Members, shaderAlias, &uniform);
    if (!uniform) {
        printf("Error UniformStruct_set_member\t: could not find Uniform in Structure \"%s\" named \"%s\"\n", uniformStruct->Alias.start, alias);
        return;
    }
    u8* elementAddress;
    u8* dataAddress;

    for (u64 i = 0; i < uniformStruct->Elements; i++) {
        elementAddress = (u8*)uniformStruct->Data + (i * uniform->Stride) + uniform->Offset;
        dataAddress = (u8*)data + (i * uniform->Size);
        memcpy(elementAddress, dataAddress, uniform->Size);
    }
}


void internal_Shader_create (const GLuint program, const char* alias) {
    
    if (program == GL_NONE) {
        return;
    }
    
    String shaderAlias = String_from_ptr(alias);
    Shader shader = {0,};
    Shader* shaderReference;

    HashTable_find_reference(ShaderProgramTable, shaderAlias, &shaderReference);

    if (shaderReference) {
        shaderReference->references++;
        return;
    }
    else {
        shaderReference = &shader;
    }

    GLint uniformCount = internal_Program_uniform_count(program);
    GLint bufferCount = internal_Program_buffer_count(program);

    shaderReference->uniforms = HashTable_create(Uniform, uniformCount);
    shaderReference->uniformBuffers = HashTable_create(UniformBuffer, bufferCount);
    shaderReference->program = program;
    shaderReference->references = 0;
   
    internal_Program_uniform_parse(program, shaderReference->uniforms);
    internal_Program_buffer_parse(program, shaderReference->uniformBuffers);
    HashTable_insert(ShaderProgramTable, shaderAlias, shaderReference);
}


void Shader_delete(const char* alias) {
    Shader_delete_String((String)String_from_ptr(alias));

}

void Shader_delete_String(String alias) {
    Shader* shader = NULL;
    HashTable_find_reference(ShaderProgramTable, alias, &shader);

    if (!shader) {
        return;
    }

    if (shader->references > 1) {
        shader->references--;
        return;
    }

    Shader_deinitialize(shader);
    HashTable_remove(ShaderProgramTable, alias);
}


void Shader_deinitialize (Shader* shader) {

    glDeleteProgram(shader->program);
    shader->program = GL_NONE;
    for (HashTable_array_iterator(shader->uniforms)) {
        Uniform* uniform = HashTable_array_at(Uniform, shader->uniforms, i);
        if (uniform->UniformType == UNIFORM_TYPE_SINGLE) {
            free(uniform->Data);
            free(uniform->Alias.start);
        }
        uniform = NULL;
    }

    for (HashTable_array_iterator(shader->uniformBuffers)) {
        UniformBuffer* buffer = HashTable_array_at(UniformBuffer, shader->uniformBuffers, i);
        UniformBuffer_deinitialize(buffer);
    }
}


Shader* Shader_get(const char* alias) {
    Shader* shader = NULL;
    HashTable_find_reference(ShaderProgramTable, (String) String_from_ptr(alias), &shader);
    return shader;
}

Shader* Shader_get_String(String alias) {
    Shader* shader = NULL;
    HashTable_find_reference(ShaderProgramTable, alias, &shader);
    return shader;
}


void Shader_get_uniform(const Shader* shader, const char* alias, Uniform** outVal) {
    HashTable_find_reference(shader->uniforms, (String) String_from_ptr(alias), outVal);
}

void Shader_get_uniformBuffer(const Shader* shader, const char* alias, UniformBuffer** outVal) {
    HashTable_find_reference(shader->uniformBuffers, (String)String_from_ptr(alias), outVal);
}

void Shader_debug(const GLuint program) {
    char buffer[512] = {'\0',};
    int bufferLength = 0;
    glGetShaderInfoLog(program, 512, &bufferLength, buffer);
    printf(buffer);
}

void Shader_set_uniform(const Shader* shader, const char* alias, void* data) {
    /* Set the value of a Shader's Uniform by the variable name. */

    Uniform* uniform;
    Shader_get_uniform(shader, alias, &uniform);

    if (uniform != NULL) {
        Uniform_set_data(uniform, data);
    }
}

void Shader_set_uniformBuffer(const Shader* shader, const char* alias, void* data) {
    /* Set the value of a Shader's Uniform by the variable name. */
    UniformBuffer* buffer;
    Shader_get_uniformBuffer(shader, alias, &buffer);
    UniformBuffer_set(buffer, alias, data);
}

void Shader_use(const Shader* shader) {

    glUseProgram(shader->program);
       
    // for each non-buffer uniform, upload it to the GPU.
    for (HashTable_array_iterator(shader->uniforms)) {
        Uniform* uniform = HashTable_array_at(Uniform, shader->uniforms, i);
        if (uniform != NULL) {
            //printf("Uniform Location: %d", uniform->Location);
            //printf("\t array location: %d %c", shader->Uniforms->ActiveIndicies[i], '\n');
            upload_from_gl_type(uniform->Location, uniform->Type, uniform->Elements, internal_Uniform_get_data(void, uniform));
        }
    }
    

    // Set the active texture for each texture in the material.
    //for (u16 i = 0; i < material->TexturesUsed; i++) {
    //    if (material->Textures[i] != nullptr) {
    //        glActiveTexture(GL_TEXTURE0 + i);
    //        glBindTexture(GL_TEXTURE_2D, material->Textures[i]->ID);
    //    }
    //}
}

GLint internal_Program_buffer_count(const GLuint program) {
    GLint* params = (GLint*)malloc(sizeof(GLint));
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, params);

    GLint uniformCount = 0;
    if (params) {
        uniformCount = *params;    
        free(params);
    }

    return uniformCount;
}

GLint internal_Program_uniform_count(const GLuint program) {
    GLint* params = (GLint*)malloc(sizeof(GLint));
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, params);
    
    GLint uniformCount = 0;
    if (params) {
        uniformCount = *params;
        free(params);
    }

    return uniformCount;
}




void internal_Program_buffer_parse(const GLuint program, HashTable* table) {
    //  Function to parse out uniform blocks from a shader. inserts them into the provided table, as well as the global table.
    //
    //

    char aliasBuffer[MAX_ALIAS_SIZE];

    GLint bufferCount = internal_Program_buffer_count(program);
    GLint binding;
    GLint indicies;
    GLint* uniformIndicies;
    GLint aliasLength;
    GLint size;

    for (GLint i = 0; i < bufferCount; i++) {

        // get the name of the uniform buffer:
        //glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_NAME_LENGTH, &aliasLength);
        
        glGetActiveUniformBlockName(program, i, MAX_ALIAS_SIZE, &aliasLength, aliasBuffer);
        
        if (aliasLength <= 0) {
            continue;
        }

        String alias = String_from_ptr_size(aliasBuffer, aliasLength);
        UniformBuffer* uniformBuffer;
        HashTable_find_reference(UniformBufferTable, alias, &uniformBuffer);
        glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_BINDING, &binding);

        // if it already exists, insert that one instead.
        if (uniformBuffer) {
            uniformBuffer->References++;
            HashTable_insert(table, alias, uniformBuffer);
            continue;
        }
        
        // Otherwise, we need to fill out the other values and create a new buffer.
        glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
        glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &indicies);

        // TODO: Fix this hack to get rid of the calloc that was here. 
        UniformBuffer newUniformBuffer = { 0, };
        uniformBuffer = &newUniformBuffer;

        uniformBuffer->buffer = (u8*)calloc(size, 1);

        // Hash tables are intentionally oversized. 
        // This is because we don't know how many indicies are actually parts of structures. 
        // The above function returns ALL indicies, including those not a part of the buffer.
        uniformBuffer->Uniforms = HashTable_create(Uniform, indicies);
        uniformBuffer->UniformStructs = HashTable_create(UniformStruct, indicies);

        uniformIndicies = (GLint*)calloc(indicies, sizeof(GLint));
        Engine_validate(uniformIndicies, ENOMEM);

        glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, uniformIndicies);
        
        internal_Program_buffer_uniform_parse(program, indicies, uniformIndicies, uniformBuffer);
        internal_program_uniformStruct_parse(program, indicies, uniformIndicies, uniformBuffer);

        free(uniformIndicies);

        String_create_dirty(&alias, &(uniformBuffer->Alias));
        uniformBuffer->UniformType = UNIFORM_TYPE_BUFFER;
        uniformBuffer->Size = size;
        uniformBuffer->BindingIndex = binding;
        uniformBuffer->References = 1;
        uniformBuffer->ChangesMade = 0;

        glGenBuffers(1, &(uniformBuffer->BufferObject));
        glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer->BufferObject);
        glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, uniformBuffer->BindingIndex, uniformBuffer->BufferObject);
        glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);

        HashTable_insert(UniformBufferTable, alias, uniformBuffer);
        HashTable_insert(table, alias, uniformBuffer);
    }
}

void internal_program_uniformStruct_parse(const GLuint program, const u16 uniformCount, GLint* indicies, UniformBuffer* uniformBuffer) {
    char aliasBuffer[MAX_ALIAS_SIZE];
    u16 ValidStructCount = 0;
    GLint* blockOffsetParams = (GLint*)malloc(uniformCount * sizeof(GLint));
    Engine_validate(blockOffsetParams, ENOMEM);
    
    // Struct parsing variables:
    char* structName = NULL;
    char* nextStructName = NULL;
    u16 structMembers = 0;
    u64 structElements = 1;
    char* ArrayIdent = NULL;
    char* structIdent = NULL;
    bool ArrayIsFist = false;

    bool indiciesProvided = indicies? true : false ;

    if (!indiciesProvided) {
        indicies = (GLint*)malloc(uniformCount * sizeof(GLint));
        Engine_validate(indicies, ENOMEM);

        for (u16 i = 0; i < uniformCount; i++) {
            indicies[i] = i;
        }
    }
    
    // Create InfoArray at the max size possible to be safe.
    UniformInformation* infoArray = (UniformInformation*)calloc(1, uniformCount * sizeof(UniformInformation));
    Engine_validate(infoArray, ENOMEM);

    glGetActiveUniformsiv(program, uniformCount, indicies, GL_UNIFORM_OFFSET, blockOffsetParams);

    for (u16 i = 0; i < uniformCount; i++) {

        GLsizei length;
        GLint elements;
        GLenum type;
        u16 CurrentOffset = 0;

        glGetActiveUniform(program, indicies[i], MAX_ALIAS_SIZE, &length, &elements, &type, aliasBuffer);

        ArrayIdent = strchr(aliasBuffer, '[');
        ArrayIsFist = strchr(aliasBuffer, '0') == (ArrayIdent + 1) && strchr(aliasBuffer, ']') == (ArrayIdent + 2);  // the first item is always name[0] so we check that "[" is followed by a "0" then a "]"
        structIdent = strchr(aliasBuffer, '.');

        char* nameEnd = ArrayIdent ? ArrayIdent : structIdent;
        u64 structNameLength = ArrayIdent ? ArrayIdent - aliasBuffer + 1 : structIdent - aliasBuffer + 1;

        if (!structIdent) {
            continue;
        }

        if (nextStructName) free(nextStructName);
        nextStructName = (char*)calloc(1, structNameLength);
        Engine_validate(nextStructName, ENOMEM);
        memcpy(nextStructName, aliasBuffer, nameEnd - aliasBuffer);

        if (!structName) {
            structName = (char*)calloc(1, structNameLength);
            Engine_validate(structName, ENOMEM);
            memcpy(structName, aliasBuffer, nameEnd - aliasBuffer);
        }

        // found a different struct or end of uniforms.
        if (strcmp(structName, nextStructName) && structMembers != 0) {
            // do insert and reset counters.
            // TODO: element count is probably wrong here too. Fix it.
            //printf("\nstructElements:%d\n", structElements);
            String structNameString = String_from_ptr(structName);
            UniformStruct* newStruct = internal_UniformStruct_create(structNameString, infoArray, structMembers, structElements / structMembers, uniformBuffer->buffer);
            HashTable_insert(uniformBuffer->UniformStructs, structNameString, newStruct);
            free(newStruct);
            ValidStructCount++;
            structMembers = 0;
            structElements = 1;
            free(structName);
            structName = NULL;
        }
            
        // if it's in an array, and not the first index of the array, discard this element and increment the count of elements.
        // this will result in structElements being elements * members so we will have to divide by that later. 
        if(ArrayIdent && !ArrayIsFist){
            structElements++;
            continue;
        }

        // The current member is unique. Create info for it 
        String memberNameString;
        u64 memberNameLength = length - (structIdent - aliasBuffer);
        String memberNameBuffer = String_from_ptr_size(structIdent + 1, memberNameLength - 1);
        String_create_dirty(&memberNameBuffer, &memberNameString);

        UniformInformation info = { 
            .Alias = {memberNameString.start, memberNameString.end},
            .Location = indicies[i],
            .UniformType = UNIFORM_TYPE_SHARED,
            .Type = type, 
            .Elements = elements, 
            blockOffsetParams[i]
        };

        infoArray[structMembers] = info;

        structMembers++;
    }

    // final check to ensure the last index gets uploaded.
    if (structMembers != 0) {
        //TODO: element count is wrong here. fix it.
        //printf("\nstructElements:%d\n", structElements);
        String structNameString = String_from_ptr(structName);
        UniformStruct* newStruct = internal_UniformStruct_create(structNameString, infoArray, structMembers, structElements / structMembers, uniformBuffer->buffer);
        HashTable_insert(uniformBuffer->UniformStructs, structNameString, newStruct);
        free(newStruct);
        ValidStructCount++;
        structMembers = 0;
        structElements = 0;
        free(structName);
        structName = NULL;
    }

    // Resize the table to fit the actual number found.
    HashTable_resize(uniformBuffer->UniformStructs, ValidStructCount);

    for (u64 i = 0; i < structMembers; ++i) {
        String_free_dirty(&infoArray[i].Alias);
    }

    if (nextStructName) free(nextStructName);
    if (!indiciesProvided) free(indicies);
    free(blockOffsetParams);
    free(infoArray);
}


void internal_Program_buffer_uniform_parse(const GLuint program, const u16 uniformCount, const GLint* indicies, UniformBuffer* uniformBuffer) {
    char aliasBuffer[MAX_ALIAS_SIZE];
    GLint* blockOffsetParams = (GLint*)calloc(uniformCount, sizeof(GLint));
    Engine_validate(blockOffsetParams, ENOMEM);
    
    glGetActiveUniformsiv(program, uniformCount, indicies, GL_UNIFORM_OFFSET, blockOffsetParams);

    for (u16 i = 0; i < uniformCount; i++) {
        
        GLsizei length;
        GLint elements;
        GLenum type;

        glGetActiveUniform(program, indicies[i], MAX_ALIAS_SIZE, &length, &elements, &type, aliasBuffer);
        
        char* ArrayIdent = strchr(aliasBuffer, '[');
        char* structIdent = strchr(aliasBuffer, '.');
        bool ArrayIsFirst = strchr(aliasBuffer, '0') == (ArrayIdent + 1) && strchr(aliasBuffer, ']') == (ArrayIdent + 2);  // the first item is always name[0] so we check that "[" is followed by a "0" then a "]"

        // if its a struct, skip it.
        if (structIdent) {
            continue;
        }

        // Dirty hack to cut off array identifier.
        if (ArrayIdent) {
            *ArrayIdent = '\0';
            length = ArrayIdent - aliasBuffer;
        }

        UniformInformation info = { 
            .Alias = String_from_ptr(aliasBuffer),
            .UniformType = UNIFORM_TYPE_SHARED, 
            .Location = indicies[i], 
            .Type = type, 
            .Elements = elements, 
            .BlockOffset = blockOffsetParams[i]
        };

        Uniform* newUniform = internal_Uniform_create_shared(info, uniformBuffer->buffer);
        HashTable_insert(uniformBuffer->Uniforms, info.Alias, newUniform);
        free(newUniform);
    }
    free(blockOffsetParams);
}

void internal_Program_uniform_parse(const GLuint program, HashTable* table) {
    // Function to parse out uniforms from a shader. Inserts the uniforms into the provided table.
    //
    //
    char aliasBuffer[MAX_ALIAS_SIZE];
    GLint uniformCount = internal_Program_uniform_count(program);
    u64 validUniformCount = uniformCount;
    GLsizei length;
    GLint location;
    GLint elements;
    GLenum type;

    GLint* blockOffsetParams = (GLint*)malloc(uniformCount * sizeof(GLint));
    GLuint* indicies = (GLuint*)malloc(uniformCount * sizeof(GLuint));
    //Uniform** uniforms = (Uniform**)calloc(uniformCount, sizeof(Uniform*));
 
    Engine_validate(blockOffsetParams, ENOMEM);
    Engine_validate(indicies, ENOMEM);
    //Engine_validate(uniforms, ENOMEM);

    for(GLuint i = 0; i < uniformCount; i++ ) {
        indicies[i] = i;
    }

    glGetActiveUniformsiv(program, uniformCount, indicies, GL_UNIFORM_OFFSET, blockOffsetParams);
    free(indicies);

    for (GLint i = 0; i < uniformCount; i++) {

        if (blockOffsetParams[i] != -1) {
            validUniformCount--;
            continue;
        }

        glGetActiveUniform(program, i, MAX_ALIAS_SIZE, &length, &elements, &type, aliasBuffer);
        location = glGetUniformLocation(program, aliasBuffer);

        // Skip any Uniform which does not exist or is prefixed with "gl_"
        if (location == -1) {
            validUniformCount--;
            continue;
        }

        char* ArrayIdent = strchr(aliasBuffer, '[');
        char* structIdent = strchr(aliasBuffer, '.');
        bool ArrayIsFirst = strchr(aliasBuffer, '0') == (ArrayIdent + 1) && strchr(aliasBuffer, ']') == (ArrayIdent + 2);  // the first item is always name[0] so we check that "[" is followed by a "0" then a "]"

        // if its a struct, skip it.
        if (structIdent) {
            validUniformCount--;
            continue;
        }

        // Dirty hack to cut off array identifier.
        if (ArrayIdent) {
            *ArrayIdent = '\0';
            length = ArrayIdent - aliasBuffer;
        }

        UniformInformation info = { 
            .Alias = String_from_ptr(aliasBuffer), 
            .UniformType = UNIFORM_TYPE_SINGLE, 
            .Location = location, 
            .Type = type, 
            .Elements = elements, 
            .BlockOffset = -1
        };

        Uniform* uniform = internal_Uniform_create(info);
        HashTable_insert(table, info.Alias, uniform);
        free(uniform);
    }
    HashTable_resize(table, validUniformCount);
    free(blockOffsetParams);
}
