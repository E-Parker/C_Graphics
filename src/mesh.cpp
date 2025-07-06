#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "bool.h"
#include "cStringUtilities.h"

#include "camera.h"
#include "mesh.h"
#include "material.h"
#include "renderable.h"
#include "par_shapes.h"

const uint32_t LINE_BUFFER_SIZE = 512;
const uint32_t MAX_FILESIZE = 0xffff; 
const uint32_t MAX_ITERATIONS = 0xffff;


void StaticMesh_Destory() {
    
    // Free the first mesh the normal way, since it contains the original reference to the vbo, tbo, and nbo.
    FreeMesh(&meshRenders[0]);

    // Delete all other meshes with the subMesh method to avoid freeing the same location twice.
    for (int i = 1; i < MaterialCount; i++) {
        FreeSubMesh(&meshRenders[i]);
    }
    
    delete[] materials;
    materials = NULL;

    delete[] meshRenders;
    meshRenders = NULL;
}

void StaticMesh::SetMaterial(Material* material, uint32_t index) {

    if(this == NULL || index >= MaterialCount) {
        return;
    }

    materials[index] = material;
}



void StaticMesh::Draw() const {
    /* function to draw a mesh on screen. */

    if (this == NULL) {
        return;
    }

    // run a draw call for each material.
    for (uint32_t i = 0; i < MaterialCount; i++) {
        DrawRenderable(&meshRenders[i], materials[i], &Transform);
    }
}

Vector2 Vector2FromString(const std::string data) {

	std::stringstream dataStream(data);
	std::string segment;
	std::vector<std::string> segmentList;

	while(std::getline(dataStream, segment, ' ')) {
        if (!segment.empty()) {
            segmentList.push_back(segment);
        }
	}

    Vector2 vector{0.0f, 0.0f};

	vector.x = std::stof(segmentList[0]);
	vector.y = std::stof(segmentList[1]);

	return vector;
	
}

Vector3 Vector3FromString(const std::string data) {

	std::stringstream dataStream(data);
	std::string segment;
	std::vector<std::string> segmentList;

	while(std::getline(dataStream, segment, ' ')) {
        if (!segment.empty()) {
   		    segmentList.push_back(segment);
        }
	}

    Vector3 vector{0.0f, 0.0f, 0.0f};

	vector.x = std::stof(segmentList[0]);
	vector.y = std::stof(segmentList[1]);
	vector.z = std::stof(segmentList[2]);

	return vector;
}

void parseFace(std::vector<uint32_t>* vi, std::vector<uint32_t>* ti, std::vector<uint32_t>* ni, std::vector<std::string> segmentList) {
    
    // For each of the three points in the face,
    for (int i = 0; i < segmentList.size(); i++) {
        std::stringstream subDataStream(segmentList[i]);
        std::string subSegment;
        std::vector<std::string> subSegmentList;

        while (std::getline(subDataStream, subSegment, '/')) {
            subSegmentList.push_back(subSegment);
        }

        // for each subset, get the int stored there.
        // This kind of sucks but it means that if there's a missing index nothing is added for that list.
        // TODO: add support for models that lack UV or Normal data.
        for (int k = 0; k < subSegmentList.size(); k++) {
            switch (k) {
            case 0:	vi->push_back(std::stoi(subSegmentList[0]) - 1); break;
            case 1:	ti->push_back(std::stoi(subSegmentList[1]) - 1); break;
            case 2:	ni->push_back(std::stoi(subSegmentList[2]) - 1); break;
            default: break;
            }
        }
    }
}

int parseFaceIndicies(std::vector<uint32_t>* vi, std::vector<uint32_t>* ti, std::vector<uint32_t>* ni, const std::string data) {
	/* This function parses an incoming wavefront file face data and added the indicies to the corresponding lists. 
	Faces are stored as f n/n/n n/n/n n/n/n where the subsets are the indicies of the vertex, texture coordinate, and normal. */

	std::stringstream dataStream(data);
	std::string segment;
	std::vector<std::string> segmentList;

	while(std::getline(dataStream, segment, ' ')) {
   		segmentList.push_back(segment);
	}

    if (segmentList.size() == 3) {
        parseFace(vi, ti, ni, segmentList);
        return 3;
    }

    if (segmentList.size() == 4) {
        std::vector<std::string> firstTriangle { segmentList[0], segmentList[1], segmentList[2] };
        std::vector<std::string> secondTriangle{ segmentList[2], segmentList[3], segmentList[0] };

        parseFace(vi, ti, ni, firstTriangle);
        parseFace(vi, ti, ni, secondTriangle);
    }
    return 6;
}

StaticMesh* CreateStaticMeshFromWavefront(const char* path) {
    /* Parse an obj file and load a mesh from it. */ 
    

    FILE* file = fopen(path, "r");
    
    // Return null if the file could not be found.
    if (!file) {
        printf("Mesh load error: source file \"%s\" not found or inaccessible.", path);
        return NULL;
    }
    
    // Get the raw file as a c-string to compile
    char* buffer = (char*)malloc(MAX_FILESIZE);  
    fgets(buffer, MAX_FILESIZE, file);
    int errorCode = ferror(file);
    fclose(file);
    file = NULL;

    // Return null if read error.
    if (errorCode) {
        printf("Mesh load error: Something went wrong reading file, \"%s\". Discarding Mesh.", path);
        free(buffer);
        return NULL;
    }

    char* bufferEnd = FindBufferEnd(buffer);
    
    // Return null if the buffer was not large enough.
    if(!bufferEnd) {
        printf("Mesh load error: source file \"%s\" was to big to fit in a buffer. (%u),", path, MAX_FILESIZE);
        free(buffer);
        return NULL;
    }
    
    char lineBuffer[LINE_BUFFER_SIZE];
    char identifyer[2] = { '\0', '\0' };
    uint32_t lineCount = 0;
    uint32_t materialCount = 0;

    List surfaceSplitIndecies = List_Create(uint32_t, 128);
	List vi = List_Create(uint32_t, 128); 
	List ti = List_Create(uint32_t, 128); 
	List ni = List_Create(uint32_t, 128); 
	
	List vertexList = List_Create(vec3, 128);
	List normalList = List_Create(vec3, 128);
	List tCoordList = List_Create(vec2, 128);

    stream.seekp(0);
    uint32_t iteration = 0;
    uint32_t indiciesParced = 0;
	
    while (!stream.eof()) {
        iteration++;
    	// Copy the line into the buffer:
    	stream.getline(lineBuffer, LINE_BUFFER_SIZE);       // Get the current line from the file.
    	memcpy(identifyer, lineBuffer, 2 * sizeof(char));   // Copy the first two characters from the line buffer.
		
		// Quick hack to get it as a string.
		std::string lineBufferString = std::string(lineBuffer);

    	switch (identifyer[0]) {
    	case '#':   // comment line, skip this one.
            break;

        case 'o':   // object name.
            ObjectName = lineBufferString.substr(2);
            break;

        case 's':   // use smooth shading or not.
                  
            break;

        case 'u':   // material.
            
            if (identifyer[1] != 's') {
                break;
            }

            materialCount++;
            surfaceSplitIndecies.push_back(indiciesParced);
            indiciesParced = 0;
            break;

    	case 'f':
    	    //handle face stuff here.
            indiciesParced += parseFaceIndicies(&vi, &ti, &ni, lineBufferString.substr(2));
            break;

        // handle all vertex cases.
    	case 'v':
    	    switch (identifyer[1]) {
    	    case ' ':   // vertex
                vertexList.push_back(Vector3FromString(lineBufferString.substr(2)));
				break;
    	    case 't':   // tCoord
                tCoordList.push_back(Vector2FromString(lineBufferString.substr(3)));
				break;
    	    case 'n':   // normal
                normalList.push_back(Vector3FromString(lineBufferString.substr(3)));
                break;
    	    }
        default:
            break;
        }
	}

    std::cout << iteration << std::endl;
    
    if (surfaceSplitIndecies.empty()) {
        surfaceSplitIndecies.push_back(indiciesParced);
    }
    else {
        surfaceSplitIndecies[0] = indiciesParced;
    }

    // Create a new mesh from the index data which follows the standards of OpenGL.

    // Assert that all index list are the same size.
    assert(vi.size() == ni.size() && ni.size() == ti.size());


    //std::vector<uint32_t> occurances(vi.size(), 0);
    //std::vector<uint32_t> matchingTcoord(vi.size(), 0);
    //
    //for (uint32_t i = 0; i < vi.size(); i++) {
    //    occurances[vi[i]]++;
    //}

    std::vector<Vector3> normalArray(vertexList.size(), Vector3{ 0.0f, 1.0f, 0.0f });
    std::vector<Vector2> tCoordArray(vertexList.size(), Vector2{ 0.0f, 0.0f });


    for (uint32_t i = 0; i < vi.size(); i++) {
        normalArray[vi[i]] = normalList[ni[i]];
        tCoordArray[vi[i]] = tCoordList[ti[i]];
    }

    StaticMesh* newMesh = new StaticMesh((materialCount == 0) ? 1 : materialCount, MatrixIdentity());
    SetAlias(newMesh, ObjectName.c_str());

    // upload the first mesh containing the actual vertex, normal and tChoord buffers. 
    UploadMesh(&(newMesh->meshRenders[0]), &vi[0], &vertexList[0], &normalArray[0], &tCoordArray[0], surfaceSplitIndecies[0], vertexList.size());
    
    //starting at the first material split, upload a sub-mesh referencing the buffers from the first mesh.
    uint32_t currentMaterialElementIndex = 0;

    for (uint32_t i = 1; i < materialCount; i++) {
        // Copy the vbo, tbo, and nbo from the first mesh which holds all the data.
        currentMaterialElementIndex += surfaceSplitIndecies[i];
        UploadSubMesh(&newMesh->meshRenders[i], &newMesh->meshRenders[0], &vi[currentMaterialElementIndex], surfaceSplitIndecies[i]);
    }
	return newMesh;
}

//par_shapes_mesh* tmp = parMesh;
//parMesh = par_shapes_weld(parMesh, 0.01, 0);
//par_shapes_free_mesh(tmp);
//par_shapes_compute_normals(parMesh);


StaticMesh* CreateStaticMeshFromRawData(const uint32_t* indeciesArray, const  Vector3* vertexBufferArray, const  Vector3* normalBufferArray, const  Vector2* tCoordArray, const  size_t indecies, const  size_t vertecies) {
    StaticMesh* newMesh = new StaticMesh(1, MatrixIdentity());
    UploadMesh(&(newMesh->meshRenders[0]), indeciesArray, vertexBufferArray, normalBufferArray, tCoordArray, indecies, vertecies);
    return newMesh;
}


StaticMesh* CreateStaticMeshPrimativeCone(int slices, int stacks) {
    par_shapes_mesh* parMesh = par_shapes_create_cone(slices, stacks);
    StaticMesh* newMesh = new StaticMesh(1, MatrixIdentity());
    UploadMesh(&(newMesh->meshRenders[0]), parMesh->triangles, (Vector3*)parMesh->points, (Vector3*)parMesh->normals, (Vector2*)parMesh->tcoords, parMesh->ntriangles * 3, parMesh->npoints);
    par_shapes_free_mesh(parMesh);
    return newMesh;
}


StaticMesh* CreateStaticMeshPrimativeCylinder(int slices, int stacks) {
    par_shapes_mesh* parMesh = par_shapes_create_cylinder(slices, stacks);
    StaticMesh* newMesh = new StaticMesh(1, MatrixIdentity());
    UploadMesh(&(newMesh->meshRenders[0]), parMesh->triangles, (Vector3*)parMesh->points, (Vector3*)parMesh->normals, (Vector2*)parMesh->tcoords, parMesh->ntriangles * 3, parMesh->npoints);
    par_shapes_free_mesh(parMesh);
    return newMesh;
}


StaticMesh* CreateStaticMeshPrimativeTorus(int slices, int stacks, float radius) {
    par_shapes_mesh* parMesh = par_shapes_create_torus(slices, stacks, radius);
    StaticMesh* newMesh = new StaticMesh(1, MatrixIdentity());
    UploadMesh(&(newMesh->meshRenders[0]), parMesh->triangles, (Vector3*)parMesh->points, (Vector3*)parMesh->normals, (Vector2*)parMesh->tcoords, parMesh->ntriangles * 3, parMesh->npoints);
    par_shapes_free_mesh(parMesh);
    return newMesh;
}


StaticMesh* CreateStaticMeshPrimativePlane(int slices, int stacks) {
    par_shapes_mesh* parMesh = par_shapes_create_plane(slices, stacks);
    StaticMesh* newMesh = new StaticMesh(1, MatrixIdentity());
    UploadMesh(&(newMesh->meshRenders[0]), parMesh->triangles, (Vector3*)parMesh->points, (Vector3*)parMesh->normals, (Vector2*)parMesh->tcoords, parMesh->ntriangles * 3, parMesh->npoints);
    par_shapes_free_mesh(parMesh);
    return newMesh;
}


StaticMesh* CreateStaticMeshPrimativeSphere(int subdivisions) {
    par_shapes_mesh* parMesh = par_shapes_create_subdivided_sphere(subdivisions);
    StaticMesh* newMesh = new StaticMesh(1, MatrixIdentity());
    Vector2* tCoord = new Vector2[parMesh->npoints]{ {0.0f, 0.0f} };
    UploadMesh(&(newMesh->meshRenders[0]), parMesh->triangles, (Vector3*)parMesh->points, (Vector3*)parMesh->normals, tCoord, parMesh->ntriangles * 3, parMesh->npoints);
    par_shapes_free_mesh(parMesh);
    delete[] tCoord;
    return newMesh;
}


StaticMesh* CreateStaticMeshFromGraphicsLibraryTransmissionFormat(const char* Path) {
    return NULL; 
}


StaticMesh* CreateStaticMeshFromGraphicsLibraryBinaryTransmissionFormat(const char* Path) {
    return NULL;
}


