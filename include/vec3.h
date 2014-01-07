#pragma once

namespace math {
	class vec3;
}

#include "mat4.h"

namespace math {
	class vec3
	{
	public:
		vec3() {
			this->h = new float[3];
			this->h[0] = 0.0f;
			this->h[1] = 0.0f;
			this->h[2] = 0.0f;
		}
		vec3(float const &x, float const &y, float const &z) {
			this->h = new float[3];
			this->h[0] = x;
			this->h[1] = y;
			this->h[2] = z;
		}
		virtual ~vec3() {
			delete[] this->h;
		}

		float length() const {
			return sqrt(
				this->h[0] * this->h[0] +
				this->h[1] * this->h[1] +
				this->h[2] * this->h[2]);
		}
		vec3 normalize() const {
			float l = this->length();
			return vec3(
				this->h[0] / l,
				this->h[1] / l,
				this->h[2] / l);
		}

		vec3 operator+(vec3 const &other) const {
			return vec3(
				this->h[0] + other.h[0],
				this->h[1] + other.h[1],
				this->h[2] + other.h[2]);
		}
		vec3 operator-(vec3 const &other) const {
			return vec3(
				this->h[0] - other.h[0],
				this->h[1] - other.h[1],
				this->h[2] - other.h[2]);
		}
		vec3 operator-() const {
			return vec3(
				-this->h[0],
				-this->h[1],
				-this->h[2]);
		}
		float operator*(vec3 const &other) const {
			return 
				this->h[0] * other.h[0] +
				this->h[1] * other.h[1] +
				this->h[2] * other.h[2];
		}
		vec3 operator%(vec3 const &other) const {
			return vec3(
				this->h[1] * other.h[2] - this->h[2] * other.h[1],
				this->h[2] * other.h[0] - this->h[0] * other.h[2],
				this->h[0] * other.h[1] - this->h[1] * other.h[0]);
		}
		vec3 const& operator=(vec3 const& value) {
			this->h[0] = value.h[0];
			this->h[1] = value.h[1];
			this->h[2] = value.h[2];

			return *this;
		}

	private:
		float *h;

		friend class mat4;
	};
}
