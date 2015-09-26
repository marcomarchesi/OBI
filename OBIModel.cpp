/******************************************************/
/*
/* OBI - A tiny OBJ model importer and viewer in OpenGL
/*
/* by Marco Marchesi 2015 - MIT license
/*
/******************************************************/

#include "stdafx.h"
#include "OBIModel.h"
#include "OBILoader.h"
#include "OBIUtils.h"
#include "OBIMath3D.h"


void AddShader(GLuint& program, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);
	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}
	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);
	/***********************/
	/* DEBUG SHADER COMPILING */
	/************************/
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}
	glAttachShader(program, ShaderObj);
}

OBIModel::OBIModel()
{
	//SetTex("giraffe.jpg");
	NumFaces = 0;
	NumVertices = 0;
	NumUVs = 0;
	IsWireframe = false;
	

	// Initialization
	m_Translation[0] = 0.0f;
	m_Translation[1] = 0.0f;
	m_Translation[2] = 0.0f;
	m_Scale[0] = 1.0f;
	m_Scale[1] = 1.0f;
	m_Scale[2] = 1.0f;
	m_Rotation[0] = 0.0f;
	m_Rotation[1] = 0.0f;
	m_Rotation[2] = 0.0f;

}

OBIModel::~OBIModel()
{
}


GLuint OBIModel::Link()
{
	return gVAO;
}

bool OBIModel::GetDisplayMode() {

	return IsWireframe;
}

Matrix4f& OBIModel::GetGLFromMatrix4f()
{

	Matrix4f ScaleTransform, RotateTransform, TranslationTransform, PerspectiveTransform;

	ScaleTransform.InitScaleTransform(m_Scale[0], m_Scale[1], m_Scale[2]);
	RotateTransform.InitRotateTransform(m_Rotation[0], m_Rotation[1], m_Rotation[2]);
	TranslationTransform.InitTranslationTransform(m_Translation[0], m_Translation[1], m_Translation[2]);

	PerspectiveTransform.InitPerspectiveProjectionTransform();

	WTransform = TranslationTransform * RotateTransform * ScaleTransform;	//apply transformations
	WPTransform = PerspectiveTransform * WTransform;		// apply perspective transformation


	return WPTransform;

}

void OBIModel::Draw()
{
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)GetGLFromMatrix4f());


	glUseProgram(ShaderProgram);

	glActiveTexture(GL_TEXTURE0);
	if (!tex_image.empty()) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_image.cols, tex_image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, tex_image.ptr());
	}
	
	// bind the VAO
	glBindVertexArray(gVAO);
	glDrawElements(GL_TRIANGLES, NumFaces * 3, GL_UNSIGNED_INT, 0);
	// unbind the VAO
	glBindVertexArray(0);
	//glUseProgram(0);  NOT WORKING IN THIS WAY
}

void OBIModel::SwitchWireframeFilled()
{
	if (IsWireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		this->Compile("m_vertex_shader.glsl", "m_texture_fs.glsl");
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		this->Compile("m_vertex_shader.glsl", "m_wireframe_fs.glsl");
	}
	IsWireframe = !IsWireframe;
}

void OBIModel::SetTex(const char* filename)
{
	tex_image = imread(filename, 1);
	flip(tex_image, tex_image, 0);
}


Mat OBIModel::GetTex()
{
	return tex_image;
}

void OBIModel::LoadModel(const char* filename,const char* tex_filename)
{

	OBILoader* loader = new OBILoader(filename);
	NumVertices = loader->GetNumVertices();
	NumFaces = loader->GetNumFaces();
	NumUVs = loader->GetNumUVs();
	printf("Loaded model with:\n");
	printf("%i vertices\n", NumVertices);
	printf("%i faces\n", NumFaces);
	printf("%i UVs\n", NumUVs);

	vector<float>* pVertexPtr = loader->GetVertices();
	//vector<int>* pIndexPtr = loader->GetFaces(); NOT WORKING WITH UVs
	vector<int>* pFacePtr = loader->GetFaces();
	vector<int>* pIndexPtr = loader->GetVerticesUVIndexes();
	vector<float>* pUVPtr = loader->GetUVs();

	printf("Faces are:\n");
	unsigned int* gIndexBufferData = new unsigned int[NumFaces * 3];
	for (int i = 0; i < NumFaces * 3; ++i) {
		gIndexBufferData[i] = (unsigned int)pIndexPtr->at(i);
		//printf("%i\n", gIndexBufferData[i]);
	}

	GLfloat* gVertexBufferData = new GLfloat[NumUVs * 5];
	int index = 0;
	int vj = 0;
	for (int i = 0; i < NumUVs * 5; i += 5)
	{
		int uv_idx = 0;
		for (int j = 0; j < NumFaces * 3;++j) {
			int face_idx = (int)pFacePtr->at(j);
			uv_idx = gIndexBufferData[j];
			if (uv_idx == index) {
				vj = face_idx;
				break;
			}	
		}
		gVertexBufferData[i] = (GLfloat)pVertexPtr->at(3 * vj);
		gVertexBufferData[i + 1] = (GLfloat)pVertexPtr->at((3 * vj) + 1);
		gVertexBufferData[i + 2] = (GLfloat)pVertexPtr->at((3 * vj) + 2);

		gVertexBufferData[i + 3] = (GLfloat)pUVPtr->at(2 * uv_idx);
		gVertexBufferData[i + 4] = (GLfloat)pUVPtr->at((2 * uv_idx) + 1);
		index++;
	}


	// make and bind the VAO
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	glGenTextures(1, &gTex);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, gTex);
	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	tex_image = imread(tex_filename, 1);
	flip(tex_image, tex_image, 0);

	glGenBuffers(1, &gVBO);
	// the buffer will contain an array of vertices
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, NumUVs * 5 * sizeof(GLfloat), gVertexBufferData, GL_STATIC_DRAW);


	glGenBuffers(1, &gIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumFaces * 3 * sizeof(unsigned int), gIndexBufferData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // 0 == position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);

	glEnableVertexAttribArray(1); // 1 == texCoord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

	//release data arrays
	delete[] gVertexBufferData;
	delete[] gIndexBufferData;

}


void OBIModel::Rotate(float psi, float theta, float phi)
{
	m_Rotation[0] = psi;
	m_Rotation[1] = theta;
	m_Rotation[2] = phi;
}

void OBIModel::Scale(float scaleX,float scaleY,float scaleZ)
{
	m_Scale[0] = scaleX;
	m_Scale[1] = scaleY;
	m_Scale[2] = scaleZ;
}

void OBIModel::Translate(float x, float y, float z)
{
	m_Translation[0] = x;
	m_Translation[1] = y;
	m_Translation[2] = z;
}


void OBIModel::Compile(const char* vs_filename,const char* fs_filename) {

	ShaderProgram = glCreateProgram();
	// check shader program
	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	// LOAD SHADERS
	string vertex_shader, fragment_shader;

	if (!OBIUtils::ReadFile(fs_filename, fragment_shader))
		exit(1);
	if (!OBIUtils::ReadFile(vs_filename, vertex_shader))
		exit(1);

	AddShader(ShaderProgram,vertex_shader.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram,fragment_shader.c_str(), GL_FRAGMENT_SHADER);

	glLinkProgram(ShaderProgram);
	glUseProgram(ShaderProgram);
	//UPDATE TRANSFORMATION MATRIX
	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	//UPDATE VERTICES
	gVert = glGetAttribLocation(ShaderProgram, "vert");
	gUV = glGetUniformLocation(ShaderProgram, "tex");

	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, GetGLFromMatrix4f());
}






