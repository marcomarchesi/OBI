#pragma once
#include "stdafx.h"
#include "OBIMath3D.h"

#ifndef OBI_MODEL_H
#define OBI_MODEL_H

class OBIModel
{
public:
	OBIModel();
	~OBIModel();
	GLuint Link();
	void Compile(const char* vs_filename, const char* fs_filename);
	void Draw();
	void SwitchWireframeFilled();

	void Rotate(float psi, float theta, float phi);
	void Scale(float scaleX, float scaleY, float scaleZ);
	void Translate(float x, float y, float z);

	Matrix4f& GetGLFromMatrix4f();

	void SetTex(const char* filename);
	Mat GetTex();

	void LoadModel(const char* filename, const char* tex_filename);

	bool GetDisplayMode();

private:
	
	GLuint gVAO;
	GLuint gVBO;
	GLuint gIBO;
	GLuint gTex;

	GLuint ShaderProgram;
	// OpenGL attributes and uniforms
	GLuint gVert;
	GLuint gWorldLocation;
	GLuint gUV;

	// internal values for the object
	float m_Translation[3];
	float m_Scale[3];
	float m_Rotation[3];

	Matrix4f WTransform;
	Matrix4f WPTransform;

	int NumFaces;
	int NumVertices;
	int NumUVs;
	Mat tex_image;

	//swtich from/to wireframe
	bool IsWireframe;
};

#endif

