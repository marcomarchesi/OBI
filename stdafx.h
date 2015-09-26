// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <string>
#include <fstream>
#include <math.h>
#include "opencv2/opencv.hpp"
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))

using namespace std;
using namespace cv;

// Define constants
static const float FRAME_WIDTH  = 640.0f;
static const float FRAME_HEIGHT  = 480.0f;
static const float Z_NEAR = 0.1f;
static const float Z_FAR = 10000.0f;
static const float FOV = 700.0f;


