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
                
        while (getline(file, line)) {
            if (line.substr(0,2) == "v ") {
                vector<GLfloat> Point;
                GLfloat x, y, z;
                istringstream s(line.substr(2));
                s >> x; s >> y; s >> z;
                Point.push_back(x);
                Point.push_back(y);
                Point.push_back(z);
                v.push_back(Point);
                
            } else if(line.substr(0,2) == "f ") {
                vector<string> words = parser(line);
                
                // std::string s = "";
                // for(int i = 0; i < words.size(); i++) {
                //     s += " " + words[i];
                //     
                // }
                // cout << s << endl;
                // cout << "v.size() = " << v.size() << "vt.size() = " << vt.size() << "vn.size() = " << vn.size() << endl;
                
                vector<GLint> vIndexSets;
                vector<GLint> vTextureSets;
                vector<GLint> vNormalSets;
                
                for(int i = 0; i < 3; i++) {
                    //std:cout << stoi(words[i * 3 + 1]) << std::endl;
                    GLint vindex = stoi(words[i * 3 + 1]) >= 0 ? (stoi(words[i * 3 + 1]) - 1) : (stoi(words[i * 3 + 1]) + v.size());
                    GLint vtindex = stoi(words[i * 3 + 2]) >= 0 ? (stoi(words[i * 3 + 2]) - 1) : (stoi(words[i * 3 + 2]) + vt.size());
                    GLint vnindex = stoi(words[i * 3 + 3]) >= 0 ? (st
    
    
    
    
    
    
    
    }
};