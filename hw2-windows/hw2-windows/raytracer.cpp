﻿/******************************************************************************/
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
#include <conio.h>
#include <algorithm> 

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
vec3 ComputeLight(vec3 direction, vec3 lightcolor, vec3 normal, vec3 halfvec, vec3 mydiffuse,vec3 myspecular,float myshininess) {

	float nDotL = dot(normal, direction);
	vec3 lambert = mydiffuse * lightcolor * std::max(nDotL, 0.0f);

	float nDotH = dot(normal, halfvec);
	vec3 phong = myspecular * lightcolor * pow(std::max(nDotH, 0.0f), myshininess);

	vec3 retval = lambert + phong;
	return retval;
}

RGBQUAD findColor(Intersection hit) { // findColor for dummies
	//cout << 1000000 << "\n";
	//RGBQUAD r = new RGBQUAD(Color.Indigo);
	RGBQUAD red = { 0, 0, 255, 0 };
	RGBQUAD green = { 0.0f, 255.0f, 0.0f, 0.0f };
	RGBQUAD blue = { 255.0f, 0.0f, 0.0f, 0.0f };
	RGBQUAD black = { 0.0f, 0.0f, 0.0f, 0.0f };
	RGBQUAD white = { 255.0f, 255.0f, 255.0f, 0.0f };
	if (hit.distance == std::numeric_limits<float>::infinity()) { // no intersection
		return black;
	}
	else { // intersection
		if (hit.primitive->type == 0) {
			vec3 norm = normalize(hit.position - hit.primitive->center);
			//cout << hit.position[0] << ", " << hit.position[1] << ", " << hit.position[2] << "\n";
			vec3 finalcolor;
			vec3 eyepos = eyeinit;
			//vec4 myvertexCalc = modelview * myvertex;
			vec3 mypos = hit.position;
			vec3 eyedirn = normalize(eyepos - mypos);

			// Compute normal, needed for shading. 
			vec3 normal = mat3(transpose(inverse(modelview))) * norm;
			normal = normalize(normal);


			finalcolor = hit.primitive->ambient + hit.primitive->emission;
			// Add up all the lights
			for (int i = 0; i < lightpos.size(); i++) {
				vec3 direction;
				vec3 lightposn = vec3(lightpos[i][0],lightpos[i][1],lightpos[i][2]);
				if (attenuation[0] == 1) { // directional light
					direction = normalize(lightposn);
				}
				else {
					//ec3 position = lightpos[i] / lightposn[i].w;
					direction = normalize(lightpos[i]-mypos); // no attenuation
				}
				vec3 halfAng = normalize(direction + eyedirn);
				vec3 curCol = ComputeLight(direction, lightcol[i], normal, halfAng, hit.primitive->diffuse, hit.primitive->specular, hit.primitive->shininess);
				finalcolor = finalcolor + curCol;
			}
			//color += vec3(norm[0], norm[1], norm[3]);
			//for (int i = 0; i < lightpos.size(); i++) {
				//color += hit.primitive->diffuse;
				//color += hit.primitive->specular;
			//}
			RGBQUAD finalcol = { finalcolor[2]*255.0f, finalcolor[1] * 255.0f, finalcolor[0] * 255.0f, 0 };
				//color += hit.primitive.am
				//for each light in the scene :
				//	color += kd term
				//	color += ks term
			return finalcol;
		}
		else {
			return red;
		}
	}
}

void init() {
	maxdepth = 5;
	attenuation = vec3(1.0, 0.0, 0.0);
	filename = "raytrace.png";
}
void Clear()
{
	system("cls");
	//clrscr(); // including header file : conio.h
}
void progressBar(float percentage) {
	cout << "[";
	for (int g = 0; g < floor(percentage / 10); g++) {
		cout << "|";
	}
	for (int g = 0; g < floor(100 - percentage) / 10; g++) {
		cout << ".";
	}
	cout << "]";
	cout << percentage << "%" << "\n";
}
int main(int argc, char* argv[]) {

  init();
  Intersection* intersect = new Intersection();
  readfile(argv[1]);
  FreeImage_Initialise();
  FIBITMAP* image = FreeImage_Allocate(width, height, 24);
  //int pix = width * height;
  //BYTE* pixels = new BYTE[3 * pix];
  //FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, width, height, width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	Camera* cam = new Camera(upinit, center, eyeinit, fovy);
	modelview = Transform::lookAt(eyeinit, center, upinit);
	mat4 transf = modelview;
	int percentage = 0;
	for (int i = 0; i < height; i++) {
		percentage = ((float)i / height) * 100; // calculates percentage rendered based on height
		progressBar(percentage);
		for (int j = 0; j < width; j++) {
			Ray* ray = cam->generateRay((float)(i + 0.5f), (float)(j + 0.5f));
			Intersection hit = intersect->findIntersection(*ray, primitives, transf);
			RGBQUAD color = findColor(hit);
			FreeImage_SetPixelColor(image, j, i, &color);
		}
		Clear(); // clears console terminal
	}
	FreeImage_Save(FIF_PNG, image, "sc1.png", 0);
	//FreeImage_Save(FIF_PNG, image, filename.c_str(), 0);

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
