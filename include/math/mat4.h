#pragma once

namespace math {
	class mat4;
}

#include <math.h>

#include "vec4.h"
#include "vec3.h"

#define PI 3.14159265358979323846264338327950288419716939937510

namespace math {
	class mat4
	{
	public:

		mat4() {
			this->h[ 0] = 1.0f; this->h[ 4] = 0.0f; this->h[ 8] = 0.0f; this->h[12] = 0.0f;
			this->h[ 1] = 0.0f; this->h[ 5] = 1.0f; this->h[ 9] = 0.0f; this->h[13] = 0.0f;
			this->h[ 2] = 0.0f; this->h[ 6] = 0.0f; this->h[10] = 1.0f; this->h[14] = 0.0f;
			this->h[ 3] = 0.0f; this->h[ 7] = 0.0f; this->h[11] = 0.0f; this->h[15] = 1.0f;
		}
		mat4(float const &a1,float const &a2,float const &a3,float const &a4,
			float const &b1,float const &b2,float const &b3,float const &b4,
			float const &c1,float const &c2,float const &c3,float const &c4,
			float const &d1,float const &d2,float const &d3,float const &d4) {
				this->h[ 0] = a1;
				this->h[ 1] = b1;
				this->h[ 2] = c1;
				this->h[ 3] = d1;

				this->h[ 4] = a2;
				this->h[ 5] = b2;
				this->h[ 6] = c2;
				this->h[ 7] = d2;

				this->h[ 8] = a3;
				this->h[ 9] = b3;
				this->h[10] = c3;
				this->h[11] = d3;

				this->h[12] = a4;
				this->h[13] = b4;
				this->h[14] = c4;
				this->h[15] = d4;
		}

		mat4 operator*(mat4 const &other) const {
			return mat4(
				this->h[0] * other.h[0] + this->h[4] * other.h[1] + this->h[8] * other.h[2] + this->h[12] * other.h[3],  this->h[0] * other.h[4] + this->h[4] * other.h[5] + this->h[8] * other.h[6] + this->h[12] * other.h[7],  this->h[0] * other.h[8] + this->h[4] * other.h[9] + this->h[8] * other.h[10] + this->h[12] * other.h[11],  this->h[0] * other.h[12] + this->h[4] * other.h[13] + this->h[8] * other.h[14] + this->h[12] * other.h[15], 
				this->h[1] * other.h[0] + this->h[5] * other.h[1] + this->h[9] * other.h[2] + this->h[13] * other.h[3],  this->h[1] * other.h[4] + this->h[5] * other.h[5] + this->h[9] * other.h[6] + this->h[13] * other.h[7],  this->h[1] * other.h[8] + this->h[5] * other.h[9] + this->h[9] * other.h[10] + this->h[13] * other.h[11],  this->h[1] * other.h[12] + this->h[5] * other.h[13] + this->h[9] * other.h[14] + this->h[13] * other.h[15], 
				this->h[2] * other.h[0] + this->h[6] * other.h[1] + this->h[10] * other.h[2] + this->h[14] * other.h[3],  this->h[2] * other.h[4] + this->h[6] * other.h[5] + this->h[10] * other.h[6] + this->h[14] * other.h[7],  this->h[2] * other.h[8] + this->h[6] * other.h[9] + this->h[10] * other.h[10] + this->h[14] * other.h[11],  this->h[2] * other.h[12] + this->h[6] * other.h[13] + this->h[10] * other.h[14] + this->h[14] * other.h[15], 
				this->h[3] * other.h[0] + this->h[7] * other.h[1] + this->h[11] * other.h[2] + this->h[15] * other.h[3],  this->h[3] * other.h[4] + this->h[7] * other.h[5] + this->h[11] * other.h[6] + this->h[15] * other.h[7],  this->h[3] * other.h[8] + this->h[7] * other.h[9] + this->h[11] * other.h[10] + this->h[15] * other.h[11],  this->h[3] * other.h[12] + this->h[7] * other.h[13] + this->h[11] * other.h[14] + this->h[15] * other.h[15]
			);
		}
		vec4 operator*(vec4 const &other) const {
			return vec4(
				this->h[0] * other.h[0] + this->h[4] * other.h[1] + this->h[8] * other.h[2] + this->h[12] * other.h[3],
				this->h[1] * other.h[0] + this->h[5] * other.h[1] + this->h[9] * other.h[2] + this->h[13] * other.h[3],
				this->h[2] * other.h[0] + this->h[6] * other.h[1] + this->h[10] * other.h[2] + this->h[14] * other.h[3],
				this->h[3] * other.h[0] + this->h[7] * other.h[1] + this->h[11] * other.h[2] + this->h[15] * other.h[3]
			);
		}
		mat4 const& operator=(mat4 const &value) {
			this->h[0] = value.h[0];
			this->h[1] = value.h[1];
			this->h[2] = value.h[2];
			this->h[3] = value.h[3];

			this->h[4] = value.h[4];
			this->h[5] = value.h[5];
			this->h[6] = value.h[6];
			this->h[7] = value.h[7];

			this->h[8] = value.h[8];
			this->h[9] = value.h[9];
			this->h[10] = value.h[10];
			this->h[11] = value.h[11];

			this->h[12] = value.h[12];
			this->h[13] = value.h[13];
			this->h[14] = value.h[14];
			this->h[15] = value.h[15];
			return *this;
		}

		static mat4 ortho(float const &left, float const &right, float const &bottom, float const &top) {
			return mat4(
				2.0f / (right - left), 0.0f, 0.0f, 0.0f,
				0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
				0.0f, 0.0f, -1.0f, 0.0f,
				-(right + left) / (right - left), -(top + bottom) / (top - bottom), 0.0f, 1.0f
				);
		}
		static mat4 perspective(float const &fov, float const &aspect, float const &zNear, float const &zFar) {
			float range = tan((fov / 2.0f)*(PI/180.0)) * zNear;
			float left = -range * aspect;
			float right = range * aspect;
			float bottom = -range;
			float top = range;

			return mat4(
				2 * zNear / (right - left), 0.0f, 0.0f, 0.0f,
				0.0f, 2 * zNear / (top - bottom), 0.0f, 0.0f,
				0.0f, 0.0f, -(zFar + zNear) / (zFar - zNear), -2 * zFar * zNear / (zFar - zNear),
				0.0f, 0.0f, -1.0f, 0.0f
				);
		}
		static mat4 translation(float const &x, float const &y, float const &z) {
			return mat4(
				1.0f, 0.0f, 0.0f, x,
				0.0f, 1.0f, 0.0f, y,
				0.0f, 0.0f, 1.0f, z,
				0.0f, 0.0f, 0.0f, 1.0f
				);
		}
		static mat4 lookAt(vec3 const &position, vec3 const &direction, vec3 const &up) {
			vec3 f = (direction - position).normalize();
			vec3 u = up.normalize();
			vec3 s = (f % u).normalize();
			u = s % f;

			return mat4(
				s.h[0], s.h[1], s.h[2], -(s * position),
				u.h[0], u.h[1], u.h[2], -(u * position),
				-f.h[0], -f.h[1], -f.h[2], (f * position),
				0.0f, 0.0f, 0.0f, 1.0f
				);
		}

	private:
		float h[16];

		friend class ::uniform;
	};
}

