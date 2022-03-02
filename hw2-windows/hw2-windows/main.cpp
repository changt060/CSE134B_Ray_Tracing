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

using namespace std ; 

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h" 
#include "readfile.h" // prototypes for readfile.cpp  




void saveScreenshot(string fname) {
  int pix = w * h;
  BYTE *pixels = new BYTE[3*pix];	
  glReadBuffer(GL_FRONT);
  glReadPixels(0,0,w,h,GL_BGR,GL_UNSIGNED_BYTE, pixels);

  FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);

  std::cout << "Saving screenshot: " << fname << "\n";

  FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
  delete pixels;
}



void init() {
	maxdepth = 5;
	attenuation = vec3(1.0, 0.0, 0.0);
	filename = "raytrace.png";
}

int main(int argc, char* argv[]) {

  FreeImage_Initialise();
  //glutInit(&argc, argv);
  //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  //glutCreateWindow("HW2: Scene Viewer");


  init();
  readfile(argv[1]);
  saveScreenshot(filename);

  //glutDisplayFunc(display);
  //glutSpecialFunc(specialKey);
  //glutKeyboardFunc(keyboard);
  //glutReshapeFunc(reshape);
  //glutReshapeWindow(w, h);
  //glutMainLoop();
  FreeImage_DeInitialise();
  return 0;
}
