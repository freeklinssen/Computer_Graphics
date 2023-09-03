#include "SkeletalModel.h"
#include <vecmath.h>
#include <typeinfo>
#include <iostream>
#include <sstream>


#include "FL/Fl.H"

using namespace std;

void SkeletalModel::load(const char *skeletonFile, const char *meshFile, const char *attachmentsFile)
{
	loadSkeleton(skeletonFile);


	m_mesh.load(meshFile);
	m_mesh.loadAttachments(attachmentsFile, m_joints.size());

	computeBindWorldToJointTransforms();
	updateCurrentJointToWorldTransforms();

}

void SkeletalModel::draw(Matrix4f cameraMatrix, bool skeletonVisible)
{
	// draw() gets called whenever a redraw is required
	// (after an update() occurs, when the camera moves, the window is resized, etc)

	m_matrixStack.clear();
	m_matrixStack.push(cameraMatrix);

	if( skeletonVisible )
	{
		drawJoints();

		drawSkeleton();

	}
	else
	{
		cerr << "done" << endl;

		// Clear out any weird matrix we may have been using for drawing the bones and revert to the camera matrix.
		glLoadMatrixf(m_matrixStack.top());

		// Tell the mesh to draw itself.
		m_mesh.draw();
	}
}

void SkeletalModel::loadSkeleton( const char* filename )
{
	// Load the skeleton from file here.
	ifstream inFile;
	inFile.open(filename);

	vector<int> jointNumbers;
	vector< Joint* > jointList;
	string line;

	while(getline(inFile, line))
	{
		vector<string> jointString;
		istringstream iss(line);
		string num;
            
        while(iss >> num)
		{
			jointString.push_back(num);
		}

		Joint *joint = new Joint;
		joint-> transform=Matrix4f::translation((float)atof(jointString[0].c_str()),
												(float)atof(jointString[1].c_str()),
												(float)atof(jointString[2].c_str()));

		int parentIndex = (int)atof(jointString[3].c_str());
		
		jointNumbers.push_back(parentIndex);
		jointList.push_back(joint);
	}


	for(int i=1; i<jointNumbers.size(); ++i)
	{
		jointList.at(jointNumbers[i])->children.push_back(jointList[i]);
	}

	m_rootJoint = jointList.front();
	m_joints = jointList;
}

void SkeletalModel::TJH_for_joints(Joint *joint)
{
	m_matrixStack.push(joint->transform);
    
	for(int j=0; j<joint->children.size(); ++j)
	{
		TJH_for_joints(joint->children[j]);	
	}
	
	glLoadMatrixf(m_matrixStack.top());
	glutSolidSphere( 0.025f, 12, 12);
	m_matrixStack.pop();
}

void SkeletalModel::drawJoints()
{
	// Draw a sphere at each joint. You will need to add a recursive helper function to traverse the joint hierarchy.
	//
	// We recommend using glutSolidSphere( 0.025f, 12, 12 )
	// to draw a sphere of reasonable size.
	//
	// You are *not* permitted to use the OpenGL matrix stack commands
	// (glPushMatrix, glPopMatrix, glMultMatrix).
	// You should use your MatrixStack class
	// and use glLoadMatrix() before your drawing call.
	TJH_for_joints(m_rootJoint);
}


void SkeletalModel::TJH_for_bones(Joint *joint)
{

	Matrix4f translation_m = joint->transform;
	m_matrixStack.push(translation_m);
    
	for(int j=0; j<joint->children.size(); ++j)
	{
		TJH_for_joints(joint->children[j]);	
	}

	Vector3f current_point(translation_m[12], translation_m[13], translation_m[14]);
	float distance = current_point.abs();

	Matrix4f translation(1.0f, 0.0f, 0.0f, 0.0f,
					 	0.0f, 1.0f, 0.0f, 0.0f, 
					 	0.0f, 0.0f, 1.0, 0.5f,
					 	0.0f, 0.0f, 0.0f, 1.0f);

	Matrix4f scale (0.05f, 0.0f, 0.0f, 0.0f,
					 0.0f, 0.05f, 0.0f, 0.0f, 
					 0.0f, 0.0f, distance, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);

    Vector3f rnd = Vector3f(0.0f, 0.0f, 1.0f);
	Vector3f z = current_point.normalized();
	Vector3f y = Vector3f::cross(z, rnd).normalized();
	Vector3f x = Vector3f::cross(y, z).normalized();

	Matrix4f rotation (x[0], y[0], z[0], 0.0f,
						x[1], y[1], z[1], 0.0f, 
						x[2], y[2], z[2], 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);
	

	Matrix4f total_opperation =  rotation * scale * translation;

	m_matrixStack.push(total_opperation);

	glLoadMatrixf(m_matrixStack.top());
	glutSolidCube( 1.0f );
	m_matrixStack.pop();
}

void SkeletalModel::drawSkeleton( )
{
	// Draw boxes between the joints. You will need to add a recursive helper function to traverse the joint hierarchy.
	
	TJH_for_joints(m_rootJoint);
}

void SkeletalModel::setJointTransform(int jointIndex, float rX, float rY, float rZ)
{
	// Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.

	Matrix4f new_matrix (cos(rZ)*cos(rY) , cos(rZ)*sin(rY)*sin(rX) - sin(rZ)*cos(rX)  , cos(rZ)*sin(rY)*cos(rX) + sin(rZ)*sin(rX), m_joints[jointIndex]-> transform[12],
						 sin(rZ)*cos(rY) , sin(rZ)*sin(rY)*sin(rX) + cos(rZ)*cos(rX) , sin(rZ)*sin(rY)*cos(rX) - cos(rZ)*sin(rY),  m_joints[jointIndex]-> transform[13], 
						 sin(rY)         , cos(rY)*sin(rX)                           , cos(rY) * cos(rX)                        ,  m_joints[jointIndex]-> transform[14],
						 0.0f            , 0.0f                                      , 0.0f                                     ,  1.0f);

	
	m_joints.at(jointIndex) -> transform = new_matrix;

}



void SkeletalModel::TJH_for_BindWorldToJointTransforms(Joint *joint)
{
	m_matrixStack.push(joint->transform);
	joint->bindWorldToJointTransform = m_matrixStack.top().inverse();

	for(int j=0; j<joint->children.size(); ++j)
	{
		TJH_for_BindWorldToJointTransforms(joint->children[j]);	
	}
	m_matrixStack.pop();
}

void SkeletalModel::computeBindWorldToJointTransforms()
{
	// 2.3.1. Implement this method to compute a per-joint transform from
	// world-space to joint space in the BIND POSE.
	//
	// Note that this needs to be computed only once since there is only
	// a single bind pose.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
	m_matrixStack.clear();
	TJH_for_BindWorldToJointTransforms(m_rootJoint);
}

void SkeletalModel::TJH_for_CurrentJointToWorldTransforms(Joint *joint)
{
	m_matrixStack.push(joint->transform);
	joint->currentJointToWorldTransform = m_matrixStack.top();

	for(int j=0; j<joint->children.size(); ++j)
	{
		TJH_for_CurrentJointToWorldTransforms(joint->children[j]);	
	}	
	m_matrixStack.pop();
}

void SkeletalModel::updateCurrentJointToWorldTransforms()
{
	// 2.3.2. Implement this method to compute a per-joint transform from
	// joint space to world space in the CURRENT POSE.
	//
	// The current pose is defined by the rotations you've applied to the
	// joints and hence needs to be *updated* every time the joint angles change.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
	m_matrixStack.clear();
	TJH_for_CurrentJointToWorldTransforms(m_rootJoint);
}


void SkeletalModel::updateMesh()
{
	// 2.3.2. This is the core of SSD.
	// Implement this method to update the vertices of the mesh
	// given the current state of the skeleton.
	// You will need both the bind pose world --> joint transforms.
	// and the current joint --> world transforms.
	vector<Vector3f> new_vertices;
	Vector3f tmp;

	for(int i=0; i<m_mesh.currentVertices.size(); ++i)
	{
		Vector4f new_vertice = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
		float weight; 

		for(int j=0; j<m_mesh.attachments[i].size();j++)
		{
			weight = m_mesh.attachments[i][j];
			new_vertice = new_vertice + weight *
			m_joints[j] -> currentJointToWorldTransform *
			m_joints[j] -> bindWorldToJointTransform *
			Vector4f(m_mesh.bindVertices[i][0], m_mesh.bindVertices[i][1], m_mesh.bindVertices[i][2], 0.0f);



		}

		new_vertices.push_back(Vector3f(new_vertice[0], new_vertice[1], new_vertice[2]));

	}


	m_mesh.currentVertices = new_vertices;


}

