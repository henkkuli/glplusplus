#pragma once

namespace math {
	class vec2;
}

#include <math.h>

//#include "../uniform.h"

namespace math {
	class vec2
	{
	public:
		vec2() {
			this->h[0] = 0.0f;
			this->h[1] = 0.0f;
		}
		vec2(float const &x, float const &y) {
			this->h[0] = x;
			this->h[1] = y;
		}

		float length() const {
			return sqrt(
				this->h[0] * this->h[0] +
				this->h[1] * this->h[1]);
		}
		vec2 normalize() const {
			float l = this->length();
			return vec2(
				this->h[0] / l,
				this->h[1] / l);
		}

		vec2 operator+(vec2 const &other) const {
			return vec2(
				this->h[0] + other.h[0],
				this->h[1] + other.h[1]);
		}
		vec2 operator-(vec2 const &other) const {
			return vec2(
				this->h[0] - other.h[0],
				this->h[1] - other.h[1]);
		}
		vec2 operator-() const {
			return vec2(
				-this->h[0],
				-this->h[1]);
		}
		float operator*(vec2 const &other) const {
			return 
				this->h[0] * other.h[0] +
				this->h[1] * other.h[1];
		}
		vec2 operator*(float const &other) const {
			return vec2(
				this->h[0] * other, 
				this->h[1] * other);
		}
		vec2 const& operator=(vec2 const& value) {
			this->h[0] = value.h[0];
			this->h[1] = value.h[1];

			return *this;
		}
		vec2 const& operator-=(vec2 const& other) {
			this->h[0] -= other.h[0];
			this->h[1] -= other.h[1];

			return *this;
		}
		vec2 const& operator+=(vec2 const& other) {
			this->h[0] += other.h[0];
			this->h[1] += other.h[1];

			return *this;
		}

	private:
		float h[2];

		friend class ::uniform;
	};
}