#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Primitive {
	public:
		int type;
		glm::vec3 center; //sphere
		float radius; // sphere
		glm::vec3 v1; // triangle
		glm::vec3 v2; // triangle
		glm::vec3 v3; // triangle
		//glm::vec3 ambient;
		//glm::vec3 diffuse;
		//glm::vec3 specular;
		//glm::vec3 emmision;
		Primitive() {}
};

class Sphere : public Primitive {
	public:
		Sphere(float x, float y, float z, float radius) {
			this->center = glm::vec3(x, y, z);
			this->radius = radius;
			this->type = 0;
		}
};

class Triangle : public Primitive {
	public:
		Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
			this->type = 3;
			this->v1 = v1;
			this->v2 = v2;
			this->v3 = v3;
		}
};
#endif
