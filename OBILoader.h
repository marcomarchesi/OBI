#pragma once
#include "stdafx.h"

#ifndef OBI_LOADER_H
#define OBI_LOADER_H

class OBILoader
{
public:
	OBILoader(const char* filename);
	~OBILoader();

	vector<float>* GetVertices();
	int GetNumVertices() const;

	vector<int>* GetFaces();
	vector<int>* GetVerticesUVIndexes();
	int GetNumFaces() const;

	vector<float>* GetUVs();
	int GetNumUVs() const;

private:
	vector<float> vertex_vector;
	vector<float> normal_vector;
	vector<int> face_vector;
	vector<int> face_uv_vector;
	vector<int> face_n_vector;
	vector<float> uv_vector;

	void GetTriple(const string& w);	// get the v/t/n triples
	
};

#endif

