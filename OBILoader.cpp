#include "stdafx.h"
#include "OBILoader.h"

/************************************************/
/* Load OBJ models								*/
/************************************************/

//constructor loads .OBJ model
OBILoader::OBILoader(const char* filename)
{
	string word;

	int vertex_counter = 0;
	int normal_counter = 0;
	int uv_counter = 0;
	int face_counter = 0;

	printf("Loading %s...\n", filename);
	fstream pModelFile;
	pModelFile.open(filename);

	if (!pModelFile.is_open())
	{
		cout << "Failed to open file!" << endl;
		exit(1);
	}

	/********************************************************/
	// parsing data to get numbers of vertices,normals,faces//
	/********************************************************/
	do
		pModelFile >> word;
	while (word != "v");

	// count vertices and copy to vertex_vector
	do {
		if (word == "v") {	// v x y z
			vertex_counter++;
			pModelFile >> word;
			vertex_vector.push_back(stof(word, nullptr));
			pModelFile >> word;
			vertex_vector.push_back(stof(word, nullptr));
			pModelFile >> word;
			vertex_vector.push_back(stof(word, nullptr));
			pModelFile >> word;
		}
	} while ((word != "vt") && (word != "vn"));

	// count UVs
	do {
		if (word == "vt") { // vt u v
			uv_counter++;
			pModelFile >> word;
			uv_vector.push_back(stof(word, nullptr));
			pModelFile >> word;
			uv_vector.push_back(stof(word, nullptr));
			pModelFile >> word;
		}
	} while (word != "vn");

	// count normals
	do {
		if (word == "vn") {
			normal_counter++;
		}
		pModelFile >> word;
	} while (word != "f");

	// count faces
	while (word == "f") {
		pModelFile >> word;
		GetTriple(word);
		pModelFile >> word;
		GetTriple(word);
		pModelFile >> word;
		GetTriple(word);
		pModelFile >> word;
		face_counter++;
	}

	printf("Model has been loaded. ");
	printf("It has %i vertices and %i faces\n", GetNumVertices(), GetNumFaces());
}

void OBILoader::GetTriple(const string& w)
{
	string delimiter = "/";
	string token;
	int post = 0;
	int posn = 0;
	token = w.substr(0, post = w.find(delimiter));
	face_vector.push_back(stoi(token, nullptr) - 1);
	token = w.substr(post+1, posn = token.find(delimiter));
	face_uv_vector.push_back(stoi(token, nullptr) - 1);
	token = w.substr(posn + 1, token.find(delimiter));
	face_n_vector.push_back(stoi(token, nullptr) - 1);
}

vector<int>* OBILoader::GetVerticesUVIndexes()
{
	return &face_uv_vector;
}

vector<float>* OBILoader::GetVertices() {

	return &vertex_vector;
}

int OBILoader::GetNumVertices() const {

	return vertex_vector.size() / 3;
}

vector<int>* OBILoader::GetFaces() {

	return &face_vector;
}

int OBILoader::GetNumFaces() const{

	return face_vector.size() / 3;
}

vector<float>* OBILoader::GetUVs() {
	return &uv_vector;
}

int OBILoader::GetNumUVs() const {
	return uv_vector.size() / 2;
}

OBILoader::~OBILoader()
{
}



