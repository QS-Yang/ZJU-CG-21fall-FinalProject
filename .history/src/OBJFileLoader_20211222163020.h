#pragma once
#include "ModelData.h"
#include "Vertex.h"
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
using namespace std;

vector<string> parser(string str, const char* delim) {
	vector<string> ret;
	char* splited = strtok(const_cast<char*>(str.c_str()), delim);
	while (splited)
	{
		ret.push_back(splited);
		splited = strtok(NULL, delim);
	}
	return ret;
}

class ObjLoader {
public:
    vector<Vertex> vertices;
    vector<glm::vec2> textures;
    vector<glm::vec3> normals;
    vector<int> indices;
    ModelData loadObj(const char* fileName) {

        ifstream file(fileName);
        string line;
        std::vector<std::vector<std::string> > f_index;
        while (getline(file, line)) {
            if (line.substr(0,2) == "v ") {
                glm::vec3 vertex;
                istringstream s(line.substr(2));
                s >> vertex.x; 
                s >> vertex.y; 
                s >> vertex.z;
                Vertex v = Vertex(vertices.size(), vertex);
                vertices.push_back(v);
            } 
            else if(line.substr(0,3) == "vt ") {
                glm::vec2 texture;
                istringstream s(line.substr(2));
                s >> texture.x; 
                s >> texture.y;
                textures.push_back(texture);
            }
            else if(line.substr(0,3) == "vn ") {
                glm::vec3 normal;
                istringstream s(line.substr(2));
                s >> normal.x; 
                s >> normal.y; 
                s >> normal.z;
                normals.push_back(normal);
            }
            else if(line.substr(0,2) == "f ") {
                vector<string> words = parser(line, " ");
                vector<string> v1 = parser(words[1],"/");
                vector<string> v2 = parser(words[2],"/");
                vector<string> v3 = parser(words[3],"/");
                f_index.push_back(v1);
                f_index.push_back(v2);
                f_index.push_back(v3);
            }    
        } 
        for(int i = 0; i < f_index.size(); i++) {
            processVertex(f_index[i]);
        }
        processVertex(v1);
        processVertex(v2);
        processVertex(v3);
        removeVertices();
        float *verticesArray = new float[vertices.size()*3];
        float *texturesArray = new float[vertices.size()*2];
        float *normalsArray = new float[vertices.size()*3];
        float furthest = convertDataToArrays(verticesArray, texturesArray, normalsArray);
        int* indicesArray = convertIndicesToArray();
        // for(int i=0; i<vertices.size()*2; i++) cout<<texturesArray[i]<<endl;
        ModelData data = ModelData(verticesArray, texturesArray, normalsArray, indicesArray, furthest, vertices.size(), indices.size());
        return data;
    }

    void processVertex(vector<string> vertex){
        cout << vertices.size() << endl;
        int index = (stoi(vertex[0])-1 >= 0) ? stoi(vertex[0])-1 : stoi(vertex[0]) + vertices.size();
        // Vertex currentVertex = vertices[index];
        int textureIndex = (stoi(vertex[1])-1 >= 0) ? stoi(vertex[1])-1 : stoi(vertex[1]) + textures.size();
        int normalIndex = (stoi(vertex[2])-1 >= 0) ? stoi(vertex[2])-1 : stoi(vertex[2]) + normals.size();
        if(!vertices[index].isSet()){
            vertices[index].setTextureIndex(textureIndex);
            vertices[index].setNormalIndex(normalIndex);
            indices.push_back(index);
        } 
        else {
            dealProcessedVertex(&vertices[index], textureIndex, normalIndex);
        }
    }

    int* convertIndicesToArray(){
        int* indicesArray = new int[indices.size()];
        for(int i=0; i<indices.size(); i++){
            indicesArray[i] = indices[i];
        }
        return indicesArray;
    }

    float convertDataToArrays(float verticesArray[], float texturesArray[], float normalsArray[]){
        float furthestPoint = 0;
        for(int i=0; i<vertices.size(); i++){
            Vertex currentVertex = vertices[i];
            if(currentVertex.length > furthestPoint){
                furthestPoint = currentVertex.length;
            }
            glm::vec3 position = currentVertex.position;
            glm::vec2 textureCoord = textures[currentVertex.textureIndex];
            glm::vec3 normalVector = normals[currentVertex.normalIndex];
            verticesArray[i * 3] = position.x;
			verticesArray[i * 3 + 1] = position.y;
			verticesArray[i * 3 + 2] = position.z;
			texturesArray[i * 2] = textureCoord.x;
			texturesArray[i * 2 + 1] = 1 - textureCoord.y;
			normalsArray[i * 3] = normalVector.x;
			normalsArray[i * 3 + 1] = normalVector.y;
			normalsArray[i * 3 + 2] = normalVector.z;
        }
        return furthestPoint;
    }

    void dealProcessedVertex(Vertex *prevVertex, int newTextureIndex, int newNormalIndex){

        if(prevVertex->hasSameTextureAndNormal(newTextureIndex, newNormalIndex)){
            indices.push_back(prevVertex->index);
        } 
        else {
            Vertex *anotherVertex = prevVertex->duplicateVertex;
            if(anotherVertex != NULL){
                dealProcessedVertex(anotherVertex, newTextureIndex, newNormalIndex);
            } 
            else {
                Vertex* dupVertex = new Vertex(vertices.size(), prevVertex->position);
                dupVertex->setTextureIndex(newTextureIndex);
                dupVertex->setNormalIndex(newNormalIndex);
                prevVertex->setDuplicateVertex(dupVertex);
                vertices.push_back(*dupVertex);
                indices.push_back(dupVertex->index);
            }
        }
    }

    void removeVertices(){
        for(int i=0; i<vertices.size(); i++){
            if(!vertices[i].isSet()){
                vertices[i].setTextureIndex(0);
                vertices[i].setNormalIndex(0);
            }
        }
    }
};