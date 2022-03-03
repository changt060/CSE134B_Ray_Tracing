#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "variables.h"
#include "math.h"

using namespace glm;

const float pie = 3.14159265; // For portability across platforms

class Ray {
	public:
		vec3 dir;
		vec3 pos;
		Ray(vec3 position, vec3 direction) {
			this->dir = normalize(direction);
			this->pos = position;
		}
		vec3 getPoint(float t) {
			return pos + t * dir;
		}
};
class Intersection {
		
public:
	float distance;
	vec3 position;
	vec3 normal;
	vec3 direction;
	Primitive* primitive;
	Intersection intersect(Ray ray, Primitive* obj) {
		if (obj->type == 3) {
			//Triangle

		}
		else if (obj->type == 0) {
			//Sphere;
			Intersection hit;
			hit.primitive = obj;
			vec3 e = ray.pos;
			vec3 ctr = obj->center;
			vec3 d = ray.dir;
			float a = dot(d, d); // basically = 1
			float b = dot(2.0f * d, (e - ctr));
			float c = dot((e - ctr), (e - ctr)) - (obj->radius * obj->radius);
			if ((b * b) - (4.0f * a * c) < 0) { // no intersrction
				hit.distance = std::numeric_limits<float>::infinity();
				return hit;
			}
				float dee = sqrt((b * b) - (4.0f * a * c));
			float t0 = (-b - dee) / (2.0f * a);
			float t1 = (-b + dee) / (2.0f * a);
			if (t0 > 0 && t1 >0) { // 
				hit.distance = t0;
			} else if (t0 == t1) {
				hit.distance = t0;
			} else if (t0 < 0 && t1 > 0) {
				hit.distance = t1;
			} else if (t0 < 0 && t1 < 0) { // no intersection
				hit.distance = std::numeric_limits<float>::infinity();
			}
			return hit;
		}
	}
	Intersection findIntersection(Ray ray, vector<Primitive*> scene) {
		float min_distance = std::numeric_limits<float>::infinity(); //start at infinity(ish)
		Intersection hit;
		for (Primitive* obj : scene) {
			Intersection temp = intersect(ray, obj);
			if (temp.distance < min_distance) {
				min_distance = temp.distance;
				hit = temp;
			}
		}
		return hit;
	}
};

class Camera{
public:
	vec3 upvector;
	vec3 lookat;
	vec3 lookfrom;
	float fovy;
	Camera(vec3 upvector, vec3 lookat, vec3 lookfrom, float fovy) {
		this->upvector = upvector;
		this->lookat = lookat;
		this->lookfrom = lookfrom;
		this->fovy = fovy;
	}
	//This function shoots out a ray for each pixel.
	Ray* generateRay(float i, float j) {

		vec3 w = normalize(lookfrom - lookat); // c = normalize(eye - center)
		vec3 b = normalize(upvector);
		vec3 u = normalize(cross(b, w));
		vec3 v = cross(w, u);

		float theta = (fovy * pie / 180.0) / 2; // in radians
		float alpha = (width/height)*tan(theta) * (j - (width / 2.0f)) / (width / 2.0f);
		float beta = tan(theta) * (i - (height / 2.0f)) / (height / 2.0f);
		vec3 direction = normalize(alpha * u + beta * v - w);
		Ray* ray = new Ray(lookfrom, direction);
		return ray;
	}
};
#endif
