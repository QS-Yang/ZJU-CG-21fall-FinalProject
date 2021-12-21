#include "ObjLoader.h"
#include <cstring>

vector<string> parser(string str) {
	vector<string> ret;
	const char* delim = "/ "; 
	char* splited = strtok(const_cast<char*>(str.c_str()), delim);
	while (splited)
	{
		ret.push_back(splited);
		splited = strtok(NULL, delim);
	}
	return ret;
}

ObjLoader::ObjLoader(string filename, Loader loader)
{
	ifstream file(filename);
	string line;
	while (getline(file, line))
	{
		if (line.substr(0,2) == "v ")
		{
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
			vector<GLint> vIndexSets;
            vector<GLint> vTextureSets;
            vector<GLint> vNormalSets;

            for(int i = 0; i < 3; i++) {
                int vindex = stoi(words[i * 3 + 1]) > 0 ? (stoi(words[i * 3 + 1]) - 1) : (stoi(words[i * 3 + 1]) + v.size() / 3);
                int vtindex = stoi(words[i * 3 + 2]) > 0 ? (stoi(words[i * 3 + 2]) - 1) : (stoi(words[i * 3 + 2]) + vt.size() / 2);
                int vnindex = stoi(words[i * 3 + 3]) > 0 ? (stoi(words[i * 3 + 3]) - 1) : (stoi(words[i * 3 + 3]) + vn.size() / 3);

                vIndexSets.push_back(vindex);
                vTextureSets.push_back(vtindex);
                vNormalSets.push_back(vnindex);
            }
			// vIndexSets.push_back(atoi(words[1].c_str())-1);
			// vIndexSets.push_back(atoi(words[4].c_str())-1);
            // vIndexSets.push_back(atoi(words[7].c_str())-1);
            // vTextureSets.push_back(atoi(words[2].c_str())-1);
            // vTextureSets.push_back(atoi(words[5].c_str())-1);
            // vTextureSets.push_back(atoi(words[8].c_str())-1);
            // vNormalSets.push_back(atoi(words[3].c_str())-1);
            // vNormalSets.push_back(atoi(words[6].c_str())-1);
            // vNormalSets.push_back(atoi(words[9].c_str())-1);
			f.push_back(vIndexSets);
            fvt.push_back(vTextureSets);
            fvn.push_back(vNormalSets);
		} else if(line.substr(0,2) == "vn"){
            vector<GLfloat> NormalVector;
            GLfloat x, y, z;
            istringstream s(line.substr(3));
            s>>x; s>>y; s>>z;
            NormalVector.push_back(x);
            NormalVector.push_back(y);
            NormalVector.push_back(z);
            vn.push_back(NormalVector);
        } else if(line.substr(0,2) == "vt"){
            vector<GLfloat> texture;
            GLfloat x, y;
            istringstream s(line.substr(3));
            s>>x; s>>y;
            texture.push_back(x);
            //存在疑问
            texture.push_back(1.0 - y);
            vt.push_back(texture);
        }
	}
	file.close();
}

Model ObjLoader::Draw(){
    GLfloat *textureArray = new GLfloat[2*v.size()]();
    GLfloat *NormalArray = new GLfloat[3*v.size()]();
    for(int i=0; i<f.size(); i++){
        for(int j=0; j<f[i].size(); j++){
            textureArray[2*f[i][j]] = vt[fvt[i][j]][0];
            textureArray[2*f[i][j]+1] = vt[fvt[i][j]][1];
        }
    }
    for(int i=0; i<f.size(); i++){
        for(int j=0; j<f[i].size(); j++){
            NormalArray[3*f[i][j]] = vn[fvn[i][j]][0];
            NormalArray[3*f[i][j]+1] = vn[fvn[i][j]][1];
            NormalArray[3*f[i][j]+2] = vn[fvn[i][j]][2];
        }
    }

    vector<GLfloat> vertices;
    for(int i=0; i<v.size(); i++){
        vertices.push_back(v[i][0]);
        vertices.push_back(v[i][1]);
        vertices.push_back(v[i][2]);
    }

    vector<GLint> indices;
    for(int i=0; i<f.size(); i++){
        indices.push_back(f[i][0]);
        indices.push_back(f[i][1]);
        indices.push_back(f[i][2]);
    }

    Model model = loader.LoadToV((float*)&vertices[0], 3*v.size(), textureArray, 2*v.size(), (int*)&indices[0], 3*f.size(), NormalArray, 3*v.size());
    return model;
}
