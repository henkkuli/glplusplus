#pragma once

namespace math {
	class vec2;
	class vec3;
	class vec4;
}

#include <math.h>

// Depencies
class uniform;

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

		float &x() {
			return h[0];
		}
		float &y() {
			return h[1];
		}
		const float x() const {
			return h[0];
		}
		const float y() const {
			return h[1];
		}
	private:
		float h[2];

		friend class ::uniform;
	};
	class vec3
	{
	public:
		vec3() {
			this->h[0] = 0.0f;
			this->h[1] = 0.0f;
			this->h[2] = 0.0f;
		}
		vec3(float const &x, float const &y, float const &z) {
			this->h[0] = x;
			this->h[1] = y;
			this->h[2] = z;
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
		vec3 operator*(float const &other) const {
			return vec3(
				this->h[0] * other, 
				this->h[1] * other, 
				this->h[2] * other);
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
		vec3 const& operator-=(vec3 const& other) {
			this->h[0] -= other.h[0];
			this->h[1] -= other.h[1];
			this->h[2] -= other.h[2];

			return *this;
		}
		vec3 const& operator+=(vec3 const& other) {
			this->h[0] += other.h[0];
			this->h[1] += other.h[1];
			this->h[2] += other.h[2];

			return *this;
		}

		float &x() {
			return h[0];
		}
		float &y() {
			return h[1];
		}
		float &z() {
			return h[2];
		}
		const float x() const {
			return h[0];
		}
		const float y() const {
			return h[1];
		}
		const float z() const {
			return h[2];
		}
	private:
		float h[3];

		friend class mat4;
		friend class ::uniform;
	};
	class vec4
	{
	public:
		vec4() {
			this->h[0] = 0.0f;
			this->h[1] = 0.0f;
			this->h[2] = 0.0f;
			this->h[3] = 0.0f;
		}

		vec4(float const &x, float const &y, float const &z, float const &w) {
			this->h[0] = x;
			this->h[1] = y;
			this->h[2] = z;
			this->h[3] = w;
		}

		float &x() {
			return h[0];
		}
		float &y() {
			return h[1];
		}
		float &z() {
			return h[2];
		}
		float &w() {
			return h[3];
		}
		const float x() const {
			return h[0];
		}
		const float y() const {
			return h[1];
		}
		const float z() const {
			return h[2];
		}
		const float w() const {
			return h[3];
		}
	private:
		float h[4];

		friend class mat4;
	};
}