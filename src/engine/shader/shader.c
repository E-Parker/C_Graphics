#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "assert.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"

#include "engine_core/hash_table.h"
#include "engine_core/string.h"
#include "engine/math.h"

#include "engine/shader/shader_uniform.h"
#include "engine/shader.h"

#define MAX_ALIAS_SIZE 512

static HashTable* UniformBufferTable = NULL;
static HashTable* ShaderProgramTable = NULL;
static HashTable* TextureTable = NULL;
static HashTable* ShaderCompilationTable = NULL;

void InitShaders() {
    UniformBufferTable = HashTable_create(UniformBuffer, 128);
    ShaderProgramTable = HashTable_create(Shader, 512);
    // TextureTable = HashTable_create(Texture, 512);
}

void DereferenceShaders() {
    // Function to de-reference all shaders and shader objects. 
    // After this is called, all functions will fail until InitShaders() is called again.
    //

    // Destroy the uniform buffers.
    for (uint64_t i = 0; i < UniformBufferTable->Size; i++) {
        if (UniformBufferTable->Array[i].Value) {
            UniformBuffer_destroy((UniformBuffer**)&(UniformBufferTable->Array[i].Value));
            UniformBufferTable->Array[i].Value = NULL;  // TODO: Rewrite this!! you should not have to set this manually to avoid a double free!!!
        }
    }

    // Destroy the shader programs.
    for (uint64_t i = 0; i < ShaderProgramTable->Size; i++) {
        if (ShaderProgramTable->Array[i].Value) {
            Shader_destroy((Shader**)&(ShaderProgramTable->Array[i].Value));
            ShaderProgramTable->Array[i].Value = NULL;  // TODO: Rewrite this!! you should not have to set this manually to avoid a double free!!!
        }
    }

    // TODO: Check if this is still necessary. Textures *should* manage themselves.
    // Destroy the textures.
    //for (uint64_t i = 0; i < TextureTable->Size; i++) {
    //    if (TextureTable->Array[i].Value) {
    //        Texture_destroy((Texture**)&(TextureTable->Array[i].Value));
    //    }
    //}

    // Now destroy the tables which store them. 
    HashTable_destroy(&ShaderProgramTable);
    HashTable_destroy(&UniformBufferTable);

}

void UniformBuffer_destroy(UniformBuffer** buffer) {

    if (!(*buffer)) {
        return;
    }

    if (--(*buffer)->References != 0) {
        return;
    }

    if (!(*buffer)->Uniforms) {
        return;
    }

    glDeleteBuffers(1, &((*buffer)->BufferObject));

    for (HashTable_array_iterator((*buffer)->Uniforms)) {
        Uniform* uniform = HashTable_array_at(Uniform, (*buffer)->Uniforms, i);
        if (uniform) {
            free(uniform->Alias);
        }
    }

    // Call destroy first since removing from the global table will incorrectly destroy the object.
    HashTable_destroy(&(*buffer)->Uniforms);
    free((*buffer)->Alias);
    
    // DO NOT CALL FREE. Removing it from the global table will handle that. 
    //free(*buffer);
    
    // Now that it's been cleaned up, try to remove it from the global table to avoid deleting twice.
    HashTable_remove(UniformBufferTable, (*buffer)->Alias);

    *buffer = NULL;
}

void internal_UniformBuffer_set_region(const UniformBuffer* buffer, const uint64_t byteIndex, const uint64_t regionSizeInBytes, const void* data) {
    glBindBuffer(GL_UNIFORM_BUFFER, buffer->BufferObject);
    glBufferSubData(GL_UNIFORM_BUFFER, byteIndex, regionSizeInBytes, data);
    glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
}

void internal_UniformBuffer_set_all(const UniformBuffer* buffer, const void* data) {
    glBindBuffer(GL_UNIFORM_BUFFER, buffer->BufferObject);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, buffer->Size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
}

void internal_UniformBuffer_buffer(const UniformBuffer* buffer) {
    glBindBuffer(GL_UNIFORM_BUFFER, buffer->BufferObject);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, buffer->Size, UniformBuffer_get_shared(buffer));
    glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
}

void internal_UniformBuffer_set(UniformBuffer* buffer, const char* alias, void* data) {
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

void UniformBuffer_get_Uniform(const UniformBuffer* buffer, const char* alias, Uniform** outVal) {
    // Get an item in a buffer by its variable name.
    if(buffer) HashTable_find(buffer->Uniforms, alias, outVal);
}

void UniformBuffer_get_Struct(const UniformBuffer* buffer, const char* alias, UniformStruct** outVal) {
    // Get the whole buffer from its name.
    if(buffer) HashTable_find(buffer->UniformStructs, alias, outVal);
}

void internal_UniformBuffer_set_Struct(UniformBuffer* buffer, const char* alias, const char* memberAlias, void* data) {
    // upload data to the uniform struct.
    UniformStruct* uniformStruct;
    Uniform* uniform;
    HashTable_find(buffer->UniformStructs, alias, &uniformStruct);
    
    if (!uniformStruct) {
        printf("Error UniformBuffer_set_Struct\t: Uniform Structure \"%s\" does not exist.\n", alias);
        return;
    
    }

    UniformStruct_set_member(uniformStruct, memberAlias, data);

    buffer->ChangesMade++;
}

void internal_UniformBuffer_set_Struct_at(UniformBuffer* buffer, const char* alias, const char* memberAlias, int i, void* data) {
    
    if (!buffer) {
        printf("Error UniformBuffer_set_Struct_at\t: Uniform Buffer \"%s\" does not exist.\n", alias);
        return;
    }

    UniformStruct* uniformStruct;
    Uniform* uniform;
    HashTable_find(buffer->UniformStructs, alias, &uniformStruct);
    if (!uniformStruct) {
        printf("Error UniformBuffer_set_Struct_at\t: Uniform Structure \"%s\" does not exist.\n", alias);
        return;
    }

    UniformStruct_get_member(uniformStruct, memberAlias, &uniform);
    if (!uniform) {
        printf("Error UniformBuffer_set_Struct_at\t: Structure \"%s\" does not contain a member named \"%s\"", alias, memberAlias);
        return;
    }

    UniformStruct_set_member_at(uniformStruct, memberAlias, i, data);
    buffer->ChangesMade++;
}

UniformBuffer* UniformBuffer_get_self(const char* alias) {
    UniformBuffer* outVal;
    HashTable_find(UniformBufferTable, alias, &outVal);
    
    if (!outVal) {
        printf("Error UniformBuffer_get_self:\t\tBuffer \"%s\" does not exist.\n", alias);
    }
    
    return outVal;
}

void UniformBuffer_update_all() {
    // Upload all uniform buffers.

    for (HashTable_array_iterator(UniformBufferTable)) {
        UniformBuffer* buffer = HashTable_array_at(UniformBuffer, UniformBufferTable, i);
        //printf(buffer->Alias);
        //printf("\t Changes made: %d %c %c", buffer->ChangesMade, '\n', '\n');
        if (buffer && buffer->ChangesMade != 0) {
            internal_UniformBuffer_buffer(buffer);
            buffer->ChangesMade = 0;
        }
    }
}

Uniform* internal_Uniform_create_shared(const UniformInformation* info, void* sharedBuffer) {
    // Initialize a Uniform that is a part of a buffer.

    if (!info || info->BlockOffset == -1) {
        return NULL;
    }

    // Get the byte size of the type.
    uint64_t size = size_from_gl_type(info->Type);

    // allocate enough space for the Uniform header + the space required to store it's data. 
    Uniform* newUniform = (Uniform*)malloc(sizeof(Uniform));
    
    if (!newUniform) {
        return NULL;
    }

    newUniform->Data = (void*)(((uint8_t*)sharedBuffer) + info->BlockOffset);
    newUniform->UniformType = UNIFORM_TYPE_SHARED;
    newUniform->Location = info->Location;
    newUniform->Alias = info->Alias;
    newUniform->AliasLength = info->AliasLength;
    newUniform->Elements = info->Elements;
    newUniform->Offset = info->BlockOffset;
    newUniform->Size = size;
    newUniform->Stride = size + (16 - (size % 16));
    newUniform->Type = info->Type;

    return newUniform;
}

Uniform* internal_Uniform_create(const UniformInformation* info) {
    // Internal function to initialize a Uniform* 
    // 
    //
    
    // Get the byte size of the type.
    uint64_t size = size_from_gl_type(info->Type);

    // allocate enough space for the Uniform header + the space required to store it's data. 
    Uniform* newUniform = (Uniform*)malloc(sizeof(Uniform));
    
    if (!newUniform) {
        return NULL;
    }

    newUniform->Data = calloc(size,info->Elements);
    newUniform->UniformType = UNIFORM_TYPE_SINGLE;
    newUniform->Location = info->Location;
    newUniform->Alias = info->Alias;
    newUniform->AliasLength = info->AliasLength;
    newUniform->Size = size;
    newUniform->Stride = size + (16 - (size % 16));
    newUniform->Elements = info->Elements;
    newUniform->Offset = info->BlockOffset;
    newUniform->Type = info->Type;

    return newUniform;
}

void internal_Uniform_set_at(Uniform* uniform, int i, void* data) {
    // set the value of a particular index in a uniform.

    if (i < 0 || i >= uniform->Elements) {
        printf("Error Uniform_set_at:\tIndex: %d is out of range. The uniform only has %d element(s)\n", i, uniform->Elements);
        return;
    }

    uint8_t* elementAddress;
    uint8_t* dataAddress;

    elementAddress = (uint8_t*)uniform->Data;
    dataAddress = (uint8_t*)data + (i * uniform->Size);
    memcpy(elementAddress, dataAddress, uniform->Size);
    
    //printf("Name:\t%s\tStride:\t%04X\tOffset:\t%04X\n",uniform->Alias, (int)(i * uniform->Stride), (int)uniform->Offset);
}

void internal_Uniform_set_data(Uniform* uniform, void* data) {
    // set all of the data. If the uniform is an array it expects elements * size of data. 

    if (uniform->Size == uniform->Stride) {
        memcpy(uniform->Data, data, uniform->Size * uniform->Elements);
        return;
    }

    uint8_t* elementAddress;
    uint8_t* dataAddress;

    for (uint32_t i = 0; i < uniform->Elements; i++) {
        elementAddress = (uint8_t*)uniform->Data;
        dataAddress = (uint8_t*)data + (i * uniform->Size);
        memcpy(elementAddress, dataAddress, uniform->Size);
    }
}

UniformStruct* internal_UniformStruct_create(char* alias, const uint16_t aliasLength, const UniformInformation* info, const uint16_t memberCount, const uint64_t elements, void* shared) {

    uint64_t totalSize = 0;
    uint64_t stride = 0;
    uint32_t* offsets = (uint32_t*)malloc(sizeof(uint32_t) * memberCount);
    assert(offsets);

    // for each member, get it's local offset.
    for (uint16_t i = 0; i < memberCount; i++) {
        offsets[i] = stride;
        stride += size_from_gl_type(info[i].Type) * info[i].Elements;
        stride += 16 - (stride % 16);  // OpenGL structs are aligned to 16.
    }

    totalSize = stride * elements;
    UniformStruct* newStruct = (UniformStruct*)malloc(sizeof(UniformStruct));
    assert(newStruct);

    newStruct->Alias = alias;
    newStruct->AliasLength = aliasLength;
    newStruct->Data = shared;
    newStruct->Members = HashTable_create(Uniform, memberCount);
    newStruct->Size = totalSize;
    newStruct->Elements = elements;
    newStruct->Offset = info[0].BlockOffset;

    for (uint16_t i = 0; i < memberCount; i++) {
        
        // Find the smallest offset in the struct, which is the same as the offset of the struct. 
        if (info[i].BlockOffset < newStruct->Offset) newStruct->Offset = info[0].BlockOffset;

        Uniform* newMember = internal_Uniform_create_shared(&(info[i]), shared);
        newMember->Stride = stride;
        HashTable_insert(newStruct->Members, info[i].Alias, newMember);
    }

    return newStruct;
}

void UniformStruct_get_member(UniformStruct* uniformStruct, const char* alias, Uniform** outVal) {
    HashTable_find(uniformStruct->Members, alias, outVal);
}

void UniformStruct_set_member_at(UniformStruct* uniformStruct, const char* alias, uint64_t i, void* data) {

    Uniform* uniform;
    HashTable_find(uniformStruct->Members, alias, &uniform);
    if (!uniform) {
        printf("Error UniformStruct_set_member\t: could not find Uniform in Structure \"%s\" named \"%s\"\n", uniformStruct->Alias, alias);
        return;
    }
    uint8_t* elementAddress;
    uint8_t* dataAddress;

    elementAddress = (uint8_t*)uniformStruct->Data + (i * uniform->Stride) + uniform->Offset;
    dataAddress = (uint8_t*)data + (i * uniform->Size);
    memcpy(elementAddress, dataAddress, uniform->Size);
}

void UniformStruct_set_member(UniformStruct* uniformStruct, const char* alias, void* data) {

    Uniform* uniform;
    HashTable_find(uniformStruct->Members, alias, &uniform);
    if (!uniform) {
        printf("Error UniformStruct_set_member\t: could not find Uniform in Structure \"%s\" named \"%s\"\n", uniformStruct->Alias, alias);
        return;
    }
    uint8_t* elementAddress;
    uint8_t* dataAddress;

    for (uint64_t i = 0; i < uniformStruct->Elements; i++) {
        elementAddress = (uint8_t*)uniformStruct->Data + (i * uniform->Stride) + uniform->Offset;
        dataAddress = (uint8_t*)data + (i * uniform->Size);
        memcpy(elementAddress, dataAddress, uniform->Size);
    }
}

void internal_Shader_create(const GLuint program, const char* alias) {
    /* create a new shader, populate the fields and return a pointer to it. */

    Shader* shader;
    HashTable_find(ShaderProgramTable, alias, &shader);

    if (shader) {
        shader->references++;
        return shader;
    }

    GLint uniformCount = internal_Program_uniform_count(program);
    GLint bufferCount = internal_Program_buffer_count(program);
    GLint uniformCountTotal = uniformCount + bufferCount;

    // Allocate the shader.
    shader = (Shader*)calloc(1, sizeof(Shader));
    assert(shader != NULL);

    shader->uniforms = HashTable_create(Uniform, uniformCount);
    shader->uniformBuffers = HashTable_create(UniformBuffer, bufferCount);
   
    internal_Program_uniform_parse(program, shader->uniforms);
    internal_Program_buffer_parse(program, shader->uniformBuffers);

    char* aliasEnd = FindBufferEnd(alias);
    uint64_t aliasLength = aliasEnd - alias + 1;

    char* shaderName = (char*)malloc(aliasLength);
    assert(shaderName != NULL);

    memcpy(shaderName, alias, aliasLength);

    shader->alias.start = shaderName;
    shader->alias.end = shaderName + aliasLength - 1;
    shader->program = program;

    HashTable_insert(ShaderProgramTable, alias, shader);
}

void Shader_destroy(Shader** shader){
    /* Free a shader allocated with CreateShader. */
    
    if ((*shader)->references > 1) {
        (*shader)->references--;
        return;
    }

    glDeleteProgram((*shader)->program);
    (*shader)->program = GL_NONE;

    for (HashTable_array_iterator((*shader)->uniforms)) {
        Uniform* uniform = HashTable_array_at(Uniform, (*shader)->uniforms, i);
        if (uniform) {
            free(uniform->Alias);
            free(uniform->Data);
            (*shader)->uniforms->Array[i].Value = NULL;
        }
    }

    for (HashTable_array_iterator((*shader)->uniformBuffers)) {
        UniformBuffer* buffer = (UniformBuffer*)((*shader)->uniformBuffers->Array[i].Value);
        UniformBuffer_destroy(&buffer);
    }

    free((*shader)->alias.start);
    free((*shader));
    *shader = NULL;
}

Shader* Shader_get(const char* alias) {

    Shader* shader = NULL;
    HashTable_find(ShaderProgramTable, alias, &shader);
    
    if (shader) {
        shader->references++;
    }

    return shader;
}

void Shader_get_uniform(const Shader* shader, const char* alias, Uniform** outVal) {
    HashTable_find(shader->uniforms, alias, outVal);
}

void Shader_get_uniformBuffer(const Shader* shader, const char* alias, UniformBuffer** outVal) {
    HashTable_find(shader->uniformBuffers, alias, outVal);
}

void Shader_debug(const GLuint program) {
    char* buffer = (char*)calloc(1, 512);
    assert(buffer != NULL);
    int bufferLength = 0;
    glGetShaderInfoLog(program, 512, &bufferLength, buffer);
    printf(buffer);
    free(buffer);
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
    //for (uint16_t i = 0; i < material->TexturesUsed; i++) {
    //    if (material->Textures[i] != nullptr) {
    //        glActiveTexture(GL_TEXTURE0 + i);
    //        glBindTexture(GL_TEXTURE_2D, material->Textures[i]->ID);
    //    }
    //}
}

GLint internal_Program_buffer_count(const GLuint program) {
    
    GLint* params = (GLint*)malloc(sizeof(GLint));

    glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, params);

    //glGetProgramiv(program, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, params);

    if (params == NULL) {
        // something has gone very wrong and the shader has no uniforms.
        assert(false);
    }

    GLint uniformCount = *params;
    free(params);

    return uniformCount;
}

GLint internal_Program_uniform_count(const GLuint program) {
    // Get the number of uniforms.

    GLint* params = (GLint*)malloc(sizeof(GLint));
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, params);

    if (params == NULL) {
        // something has gone very wrong and the shader has no uniforms.
        assert(false);
    }

    GLint uniformCount = *params;
    free(params);
    
    return uniformCount;
}


void internal_Program_buffer_parse(const GLuint program, HashTable* table) {
    //  Function to parse out uniform blocks from a shader. inserts them into the provided table, as well as the global table.
    //
    //

    assert(UniformBufferTable != NULL);

    GLint bufferCount = internal_Program_buffer_count(program);
    GLint binding;
    GLint indicies;
    GLint* uniformIndicies;
    GLint aliasLength;
    GLint size;
    char* alias;

    char* buffer = (char*)malloc(MAX_ALIAS_SIZE);
    assert(buffer != NULL);

    for (GLint i = 0; i < bufferCount; i++) {

        // get the name of the uniform buffer:
        //glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_NAME_LENGTH, &aliasLength);
        
        glGetActiveUniformBlockName(program, i, MAX_ALIAS_SIZE, &aliasLength, buffer);
        alias = (char*)malloc(aliasLength + 1);
        assert(alias != NULL);
        memcpy(alias, buffer, aliasLength + 1);

        UniformBuffer* uniformBuffer;
        HashTable_find(UniformBufferTable, alias, &uniformBuffer);
        glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_BINDING, &binding);

        // if it already exists, insert that one instead.
        if (uniformBuffer) {
            uniformBuffer->References++;
            // TODO: For some reason, explicitly using this macro causes issues. Just not binding seems to work for some reason.
            //glUniformBlockBinding(program, uniformBuffer->BindingIndex, uniformBuffer->BufferObject);
            HashTable_insert(table, alias, uniformBuffer);
            free(alias);
            continue;
        }
        
        // Otherwise, we need to fill out the other values and create a new buffer.
        glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
        glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &indicies);

        uniformBuffer = (UniformBuffer*)calloc(1, sizeof(UniformBuffer) + size);
        assert(uniformBuffer != NULL);

        // Hash tables are intentionally oversized. 
        // This is because we don't know how many indicies are actually parts of structures. 
        // The above function returns ALL indicies, including those not a part of the buffer.
        uniformBuffer->Uniforms = HashTable_create(Uniform, indicies);
        uniformBuffer->UniformStructs = HashTable_create(UniformStruct, indicies);

        uniformIndicies = (GLint*)calloc(indicies, sizeof(GLint));
        assert(uniformIndicies != NULL);
        glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, uniformIndicies);
        
        internal_Program_buffer_uniform_parse(program, indicies, uniformIndicies, uniformBuffer);
        internal_program_uniformStruct_parse(program, indicies, uniformIndicies, uniformBuffer);

        free(uniformIndicies);

        uniformBuffer->Alias = alias;
        uniformBuffer->AliasLength = aliasLength;
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

static void internal_program_uniformStruct_parse(const GLuint program, const uint16_t uniformCount, GLint* indicies, UniformBuffer* uniformBuffer) {
    
    uint16_t ValidStructCount = 0;

    GLint* blockOffsetParams = (GLint*)malloc(uniformCount * sizeof(GLint));
    assert(blockOffsetParams != NULL);

    char* buffer = (char*)malloc(MAX_ALIAS_SIZE);
    assert(buffer != NULL);
    
    // Struct parsing variables:
    char* structName = NULL;
    char* nextStructName = NULL;
    uint16_t structMembers = 0;
    uint64_t structElements = 1;
    char* ArrayIdent = NULL;
    char* structIdent = NULL;
    bool ArrayIsFist = false;

    bool indiciesProvided = indicies? true : false ;

    if (!indiciesProvided) {
        indicies = (GLint*)malloc(uniformCount * sizeof(GLint));
        assert(indicies);

        for (uint16_t i = 0; i < uniformCount; i++) {
            indicies[i] = i;
        }
    }
    
    // Create InfoArray at the max size possible to be safe.
    UniformInformation* infoArray = (UniformInformation*)calloc(1, uniformCount * sizeof(UniformInformation));
    assert(infoArray);

    glGetActiveUniformsiv(program, uniformCount, indicies, GL_UNIFORM_OFFSET, blockOffsetParams);

    for (uint16_t i = 0; i < uniformCount; i++) {

        GLsizei length;
        GLint elements;
        GLenum type;
        uint16_t CurrentOffset = 0;

        glGetActiveUniform(program, indicies[i], MAX_ALIAS_SIZE, &length, &elements, &type, buffer);

        ArrayIdent = strchr(buffer, '[');
        ArrayIsFist = strchr(buffer, '0') == (ArrayIdent + 1) && strchr(buffer, ']') == (ArrayIdent + 2);  // the first item is always name[0] so we check that "[" is followed by a "0" then a "]"
        structIdent = strchr(buffer, '.');

        char* nameEnd = ArrayIdent ? ArrayIdent : structIdent;
        uint16_t structNameLength = ArrayIdent ? ArrayIdent - buffer + 1 : structIdent - buffer + 1;

        if (!structIdent) {
            continue;
        }

        if (nextStructName) free(nextStructName);
        nextStructName = (char*)calloc(1, structNameLength);
        assert(nextStructName);
        memcpy(nextStructName, buffer, nameEnd - buffer);

        if (!structName) {
            structName = (char*)calloc(1, structNameLength);
            assert(structName);
            memcpy(structName, buffer, nameEnd - buffer);
        }

        // found a different struct or end of uniforms.
        if (strcmp(structName, nextStructName) && structMembers != 0) {
            // do insert and reset counters.
            // TODO: element count is probably wrong here too. Fix it.
            //printf("\nstructElements:%d\n", structElements);
            UniformStruct* newStruct = internal_UniformStruct_create(structName, structIdent - buffer + 1, infoArray, structMembers, structElements / structMembers, UniformBuffer_get_shared(uniformBuffer));
            HashTable_insert(uniformBuffer->UniformStructs, structName, newStruct);
            ValidStructCount++;
            structMembers = 0;
            structElements = 1;
            structName = NULL;
        }
            
        // if it's in an array, and not the first index of the array, discard this element and increment the count of elements.
        // this will result in structElements being elements * members so we will have to divide by that later. 
        if(ArrayIdent && !ArrayIsFist){
            structElements++;
            continue;
        }

        // The current member is unique. Create info for it 
            
        // this downcast is fine as long as MaxAliasSize is < 0xffff.
        uint16_t memberNameLength = length - (structIdent - buffer);
        char* memberName = (char*)calloc(1, memberNameLength);
        assert(memberName);

        memcpy(memberName, structIdent + 1, memberNameLength - 1);

        UniformInformation info = { memberName, memberNameLength - 1, UNIFORM_TYPE_SHARED, indicies[i], type, elements, blockOffsetParams[i] };
        infoArray[structMembers] = info;

        structMembers++;
    }

    // final check to ensure the last index gets uploaded.
    if (structMembers != 0) {
        //TODO: element count is wrong here. fix it.
        //printf("\nstructElements:%d\n", structElements);
        UniformStruct* newStruct = internal_UniformStruct_create(structName, structIdent - buffer + 1, infoArray, structMembers, structElements / structMembers, UniformBuffer_get_shared(uniformBuffer));
        HashTable_insert(uniformBuffer->UniformStructs, structName, newStruct);
        ValidStructCount++;
        structMembers = 0;
        structElements = 0;
        structName = NULL;
    }

    // Resize the table to fit the actual number found.
    HashTable_resize(uniformBuffer->UniformStructs, ValidStructCount);

    if (!indiciesProvided) free(indicies);
    free(nextStructName);
    free(blockOffsetParams);
    free(buffer);
    free(infoArray);
}


static void internal_Program_buffer_uniform_parse(const GLuint program, const uint16_t uniformCount, const GLint* indicies, UniformBuffer* uniformBuffer) {

    GLint* blockOffsetParams = (GLint*)malloc(uniformCount * sizeof(GLint));
    assert(blockOffsetParams != NULL);

    char* buffer = (char*)malloc(MAX_ALIAS_SIZE);
    assert(buffer != NULL);
    
    glGetActiveUniformsiv(program, uniformCount, indicies, GL_UNIFORM_OFFSET, blockOffsetParams);

    for (uint16_t i = 0; i < uniformCount; i++) {
        
        GLsizei length;
        GLint elements;
        GLenum type;

        glGetActiveUniform(program, indicies[i], MAX_ALIAS_SIZE, &length, &elements, &type, buffer);
       
        char* alias = (char*)malloc(length + 1);
        assert(alias != NULL);
        memcpy(alias, buffer, length + 1);
        

        char* ArrayIdent = strchr(alias, '[');
        char* structIdent = strchr(alias, '.'); 
        bool ArrayIsFirst = strchr(alias, '0') == (ArrayIdent + 1) && strchr(alias, ']') == (ArrayIdent + 2);  // the first item is always name[0] so we check that "[" is followed by a "0" then a "]"

        // if its a struct, skip it.
        if (structIdent) {
            continue;
        }

        // Dirty hack to cut off array identifier.
        if (ArrayIdent) {
            *ArrayIdent = '\0';
            length = ArrayIdent - alias;
            assert(ArrayIsFirst);
        }

        UniformInformation info = {alias, length + 1, UNIFORM_TYPE_SHARED, indicies[i], type, elements, blockOffsetParams[i]};
        HashTable_insert(uniformBuffer->Uniforms, alias, internal_Uniform_create_shared(&info, UniformBuffer_get_shared(uniformBuffer)));
    }

    free(blockOffsetParams);
    free(buffer);
}

void internal_Program_uniform_parse(const GLuint program, HashTable* table) {
    // Function to parse out uniforms from a shader. Inserts the uniforms into the provided table.
    //
    //
    
    GLint uniformCount = internal_Program_uniform_count(program);
    uint64_t validUniformCount = uniformCount;
    GLsizei length;
    GLint location;
    GLint elements;
    GLenum type;

    GLint* blockOffsetParams = (GLint*)malloc(uniformCount * sizeof(GLint));
    GLuint* indicies = (GLint*)malloc(uniformCount * sizeof(GLint));
    Uniform** uniforms = (Uniform**)calloc(uniformCount, sizeof(Uniform*));
 
    assert(blockOffsetParams != NULL);
    assert(indicies != NULL);
    assert(uniforms != NULL);

    for(GLuint i = 0; i < uniformCount; i++ ) {
        indicies[i] = i;
    }

    glGetActiveUniformsiv(program, uniformCount, indicies, GL_UNIFORM_OFFSET, blockOffsetParams);
    free(indicies);

    char* buffer = (char*)malloc(MAX_ALIAS_SIZE);
    assert(buffer != NULL);

    for (GLint i = 0; i < uniformCount; i++) {

        if (blockOffsetParams[i] != -1) {
            validUniformCount--;
            continue;
        }

        glGetActiveUniform(program, i, MAX_ALIAS_SIZE, &length, &elements, &type, buffer);
        location = glGetUniformLocation(program, buffer);

        // Skip any Uniform which does not exist or is prefixed with "gl_"
        if (location == -1) {
            validUniformCount--;
            continue;
        }

        char* alias = (char*)malloc(length + 1);
        assert(alias != NULL);
        memcpy(alias, buffer, length + 1);

        char* ArrayIdent = strchr(alias, '[');
        char* structIdent = strchr(alias, '.');
        bool ArrayIsFirst = strchr(alias, '0') == (ArrayIdent + 1) && strchr(alias, ']') == (ArrayIdent + 2);  // the first item is always name[0] so we check that "[" is followed by a "0" then a "]"

        // if its a struct, skip it.
        if (structIdent) {
            validUniformCount--;
            continue;
        }

        // Dirty hack to cut off array identifier.
        if (ArrayIdent) {
            *ArrayIdent = '\0';
            length = ArrayIdent - alias;
            assert(ArrayIsFirst);
        }

        UniformInformation info = { alias, length + 1, UNIFORM_TYPE_SINGLE, location, type, elements, -1 };
        HashTable_insert(table, alias, internal_Uniform_create(&info));
    }


    HashTable_resize(table, validUniformCount);

    free(blockOffsetParams);
    free(buffer);
}
