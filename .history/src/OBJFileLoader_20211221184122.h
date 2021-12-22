#pragma once
#include "ModelData.h"
#include "Vertex.h"
#include <vector>
#include <glm/glm.hpp>

class ObjLoader {
public:
    ObjLoader();
    ~ObjLoader();
    
    ModelData loadObj(const char* fileName) {

        ifstream file(fileName);
        string line;
        std::vector<Vertex> vertices;
        std::vector<glm::vec2> textures;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        while (getline(file, line)) {
            if (line.substr(0,2) == "v ") {
                glm::vec3 vertex;
                istringstream s(line.substr(2));
                s >> vertex.x; 
                s >> vertex.y; 
                s >> vertex.z;
                vertices.push_back(vertex);
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
                vn.push_back(normal);
            }
            else if(line.substr(0,2) == "f ") {
                std::vector<std::string> words = parser(line);
                std::vector<Vertex> vertexs;
                std::vector<glm::vec2> textures;
                std::vector<glm::vec3> normals;
                for(int i = 0; i < 3; i++) {
                    Vertex vertex;
                    glm::vec3 vindex = v[stoi(words[i * 3 + 1]) - 1];
                    glm::vec2 tindex = vt[stoi(words[i * 3 + 2]) - 1];
                    glm::vec3 nindex = vn[stoi(words[i * 3 + 3]) - 1];
                    vertex.Position = vindex;
                    vertex.TexCoords = tindex;
                    vertex.Normal = nindex;
                    vertexs.push_back(vertex);
                }
                indices.push_back(vertices.size());
                vertices.insert(vertices.end(), vertexs.begin(), vertexs.end());
                textures.insert(textures.end(), vertexs.begin(), vertexs.end());
                normals.insert(normals.end(), vertexs.begin(), vertexs.end());
            }    
    
    }
};