#include "Mesh.h"
#include <iostream>
#include <sstream>

using namespace std;

#define MAX_BUFFER_SIZE 300


void Mesh::load( const char* filename )
{
	// 2.1.1. load() should populate bindVertices, currentVertices, and faces
	
	// Add your code here.
	ifstream inFile;
	inFile.open(filename);

	string line;

    while(getline(inFile, line)){

        stringstream ss(line);
        Vector3f v;
        vector<unsigned> f;
        string s;
         
        ss >> s;
        ss >> v[0] >> v[1] >> v[2];

        if (s== "v") {
            bindVertices.push_back(v);
        }


        if (s=="f"){
            istringstream iss(line);
            string num;
            
            while (iss >> num){
                if (num == "f") 
				{
                    continue;  // Skip the "f" identifier
				} 

                f.push_back(std::stoi(num));
                
            } 
            faces.push_back(Tuple3u(f[0], f[1], f[2]));
        }
    }
	// make a copy of the bind vertices as the current vertices
	currentVertices = bindVertices;
}

void Mesh::draw()
{
	// Since these meshes don't have normals
	// be sure to generate a normal per triangle.
	// Notice that since we have per-triangle normals
	// rather than the analytical normals from
	// assignment 1, the appearance is "faceted".
	glBegin(GL_TRIANGLES);

	for(int i=0; i < faces.size(); ++i )
	{
		Vector3f Edge1;
		Vector3f Edge2;
		Vector3f normal;

		Edge1 = currentVertices[faces[i][1]-1] - currentVertices[faces[i][0]-1];
    	Edge2 = currentVertices[faces[i][2]-1] - currentVertices[faces[i][0]-1];
    	normal = Vector3f::cross(Edge1, Edge2).normalized();

		glNormal3d(normal[0], normal[1], normal[2]);
        glVertex3d(currentVertices[faces[i][0]-1][0], currentVertices[faces[i][0]-1][1], currentVertices[faces[i][0]-1][2]);
        glVertex3d(currentVertices[faces[i][1]-1][0], currentVertices[faces[i][1]-1][1], currentVertices[faces[i][1]-1][2]);
        glVertex3d(currentVertices[faces[i][2]-1][0], currentVertices[faces[i][2]-1][1], currentVertices[faces[i][2]-1][2]);
	}
	glEnd;
}

void Mesh::loadAttachments( const char* filename, int numJoints )
{
	// 2.2. Implement this method to load the per-vertex attachment weights
	// this method should update m_mesh.attachments

	ifstream inFile;
	inFile.open(filename);

	string line;

    while(getline(inFile, line)){

		istringstream iss(line);
        vector<float> v;
		string num;

		v.push_back(0.0f);
		
		while (iss >> num)
		{			
			v.push_back(std::stof(num));
		} 
		
		attachments.push_back(v);
    }
}
