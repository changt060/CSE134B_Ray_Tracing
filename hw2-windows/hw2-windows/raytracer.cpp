/******************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi  */
/* Extends HW 1 to deal with shading, more transforms and multiple objects    */
/******************************************************************************/

// You shouldn't have to edit this file at all!

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Transform.h"
#include <FreeImage.h>

using namespace std; 

// Main variables in the program.  
#define MAINPROGRAM 
//#include "variables.h" 
#include "readfile.h" // prototypes for readfile.cpp  
#include "Camera.h"
#include "Sampler.cpp"



void saveScreenshot(string fname) {
  int pix = width * height;
  BYTE *pixels = new BYTE[3*pix];
  glReadBuffer(GL_FRONT);
  glReadPixels(0,0,width,height,GL_BGR,GL_UNSIGNED_BYTE, pixels);

  FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, width, height, width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);

  std::cout << "Saving screenshot: " << fname << "\n";

  FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
  delete pixels;
}

RGBQUAD findColor(Intersection hit) { // findColor for dummies
	//cout << 1000000 << "\n";
	RGBQUAD red = { 0.0f, 0.0f, 255.0f, 0.0f };
	RGBQUAD green = { 0.0f, 255.0f, 0.0f, 0.0f };
	RGBQUAD blue = { 255.0f, 0.0f, 0.0f, 0.0f };
	RGBQUAD black = { 0.0f, 0.0f, 0.0f, 0.0f };
	RGBQUAD white = { 255.0f, 255.0f, 255.0f, 0.0f };
	if (hit.distance == std::numeric_limits<float>::infinity()) { // no intersection
		return black;
	}
	else { // intersection
		return red;
	}
}

void init() {
	maxdepth = 5;
	attenuation = vec3(1.0, 0.0, 0.0);
	filename = "raytrace.png";
}

int main(int argc, char* argv[]) {
	vec3 eyeinit(0.0f, 0.0f, 5.0f); // Initial eye position, also for resets
	vec3 upinit(0.0f, 1.0f, 0.0f); // Initial up position, also for resets
	vec3 center(0.0f, 0.0f, 0.0f);
	float fovy = 90.0f;
	width = 640;
	height = 480;
	FreeImage_Initialise();
  //glutInit(&argc, argv);
  //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  //glutCreateWindow("HW2: Scene Viewer");

  FIBITMAP* image = FreeImage_Allocate(width, height, 24);
  init();
  //readfile(argv[1]);
  Sphere* sph = new Sphere(0.0f, 0.0f, -3.0f, 3.0f);
  vec3 tri1(-1.0f, 1.0f, -5.0f);
  vec3 tri2(1.0f, 1.0f, -5.0f);
  vec3 tri3(0.0f, -1.0f, -5.0f);
  Triangle* tri = new Triangle(tri1, tri2, tri3);
  vector<Primitive*> scene;
  Intersection* intersect = new Intersection();
	
	scene.push_back(tri);
	//scene.push_back(sph);
 
	Camera* cam = new Camera(upinit, center, eyeinit, fovy);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Ray* ray = cam->generateRay((float)(i+0.5f), (float)(j+0.5f));
			Intersection hit = intersect->findIntersection(*ray, scene);
			//cout << hit.distance;/////////////////////////////////
			RGBQUAD color = findColor(hit);
			FreeImage_SetPixelColor(image, j, i, &color);
		}
	}
	FreeImage_Save(FIF_PNG, image, "bla.png", 0);
  //saveScreenshot(filename);

  //glutDisplayFunc(display);
  //glutSpecialFunc(specialKey);
  //glutKeyboardFunc(keyboard);
  //glutReshapeFunc(reshape);
  //glutReshapeWindow(w, h);
  //glutMainLoop();
	FreeImage_DeInitialise();
	return 0;
}
