#pragma once

namespace math {
	template<typename T>
	class tmat3;
	typedef tmat3<float> mat3;
	typedef tmat3<double> dmat3;

	template<typename T>
	class tmat4;
	typedef tmat4<float> mat4;
	typedef tmat4<double> dmat4;
}

#include <math.h>

#include "vec.h"

// Depencies
class uniform;

#define PI 3.14159265358979323846264338327950288419716939937510

namespace math {
	template<typename T>
	class tmat3 {
	public:
		explicit tmat3() {
			h[0] = (T)1; h[3] = (T)0; h[6] = (T)0;
			h[1] = (T)0; h[4] = (T)1; h[7] = (T)0;
			h[2] = (T)0; h[4] = (T)0; h[8] = (T)1;
		}

		explicit tmat3(T const &a1, T const &a2, T const &a3,
			T const &b1, T const &b2, T const &b3,
			T const &c1, T const &c2, T const &c3) {
			h[0] = a1;
			h[1] = b1;
			h[2] = c1;

			h[3] = a2;
			h[4] = b2;
			h[5] = c2;

			h[6] = a3;
			h[7] = b3;
			h[8] = c3;
		}

		// Cast operators
		explicit tmat3(tmat4<T> const &mat);

	private:
		T h[9];

		friend class tmat4<T>;
	};

	template<typename T>
	class tmat4 {
	public:

		explicit tmat4() {
			this->h[0] = (T)1; this->h[4] = (T)0; this->h[8] = (T)0; this->h[12] = (T)0;
			this->h[1] = (T)0; this->h[5] = (T)1; this->h[9] = (T)0; this->h[13] = (T)0;
			this->h[2] = (T)0; this->h[6] = (T)0; this->h[10] = (T)1; this->h[14] = (T)0;
			this->h[3] = (T)0; this->h[7] = (T)0; this->h[11] = (T)0; this->h[15] = (T)1;
		}
		explicit tmat4(T const &a1, T const &a2, T const &a3, T const &a4,
			T const &b1, T const &b2, T const &b3,T const &b4,
			T const &c1, T const &c2, T const &c3,  T const &c4,
			T const &d1, T const &d2, T const &d3, T const &d4) {
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

		// Cast operators
		explicit tmat4(tmat3<T> const &mat);

		tmat4 operator*(tmat4 const &other) const {
			return tmat4(
				this->h[0] * other.h[0] + this->h[4] * other.h[1] + this->h[8] * other.h[2] + this->h[12] * other.h[3],  this->h[0] * other.h[4] + this->h[4] * other.h[5] + this->h[8] * other.h[6] + this->h[12] * other.h[7],  this->h[0] * other.h[8] + this->h[4] * other.h[9] + this->h[8] * other.h[10] + this->h[12] * other.h[11],  this->h[0] * other.h[12] + this->h[4] * other.h[13] + this->h[8] * other.h[14] + this->h[12] * other.h[15], 
				this->h[1] * other.h[0] + this->h[5] * other.h[1] + this->h[9] * other.h[2] + this->h[13] * other.h[3],  this->h[1] * other.h[4] + this->h[5] * other.h[5] + this->h[9] * other.h[6] + this->h[13] * other.h[7],  this->h[1] * other.h[8] + this->h[5] * other.h[9] + this->h[9] * other.h[10] + this->h[13] * other.h[11],  this->h[1] * other.h[12] + this->h[5] * other.h[13] + this->h[9] * other.h[14] + this->h[13] * other.h[15], 
				this->h[2] * other.h[0] + this->h[6] * other.h[1] + this->h[10] * other.h[2] + this->h[14] * other.h[3],  this->h[2] * other.h[4] + this->h[6] * other.h[5] + this->h[10] * other.h[6] + this->h[14] * other.h[7],  this->h[2] * other.h[8] + this->h[6] * other.h[9] + this->h[10] * other.h[10] + this->h[14] * other.h[11],  this->h[2] * other.h[12] + this->h[6] * other.h[13] + this->h[10] * other.h[14] + this->h[14] * other.h[15], 
				this->h[3] * other.h[0] + this->h[7] * other.h[1] + this->h[11] * other.h[2] + this->h[15] * other.h[3],  this->h[3] * other.h[4] + this->h[7] * other.h[5] + this->h[11] * other.h[6] + this->h[15] * other.h[7],  this->h[3] * other.h[8] + this->h[7] * other.h[9] + this->h[11] * other.h[10] + this->h[15] * other.h[11],  this->h[3] * other.h[12] + this->h[7] * other.h[13] + this->h[11] * other.h[14] + this->h[15] * other.h[15]
			);
		}
		tvec4<T> operator*(tvec4<T> const &other) const {
			return tvec4<T>(
				this->h[0] * other.h[0] + this->h[4] * other.h[1] + this->h[8] * other.h[2] + this->h[12] * other.h[3],
				this->h[1] * other.h[0] + this->h[5] * other.h[1] + this->h[9] * other.h[2] + this->h[13] * other.h[3],
				this->h[2] * other.h[0] + this->h[6] * other.h[1] + this->h[10] * other.h[2] + this->h[14] * other.h[3],
				this->h[3] * other.h[0] + this->h[7] * other.h[1] + this->h[11] * other.h[2] + this->h[15] * other.h[3]
			);
		}
		tmat4 const& operator=(tmat4 const &value) {
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

		static tmat4 ortho(T const &left, T const &right, T const &bottom, T const &top) {
			return tmat4(
				2.0f / (right - left), (T)0, (T)0, (T)0,
				(T)0, (T)2 / (top - bottom), (T)0, (T)0,
				(T)0, (T)0, (T)-1, (T)0,
				-(right + left) / (right - left), -(top + bottom) / (top - bottom), (T)0, (T)1
				);
		}
		static tmat4 perspective(T const &fov, T const &aspect, T const &zNear, T const &zFar) {
			T range = tan((T) ((fov / 2.0f) * (PI / 180.0f))) * zNear;
			T left = -range * aspect;
			T right = range * aspect;
			T bottom = -range;
			T top = range;

			return tmat4(
				2 * zNear / (right - left), (T)0, (T)0, (T)0,
				(T)0, 2 * zNear / (top - bottom), (T)0, (T)0,
				(T)0, (T)0, -(zFar + zNear) / (zFar - zNear), -2 * zFar * zNear / (zFar - zNear),
				(T)0, (T)0, (T)-1, (T)0
				);
		}
		static tmat4 translation(T const &x, T const &y, T const &z) {
			return tmat4(
				(T)1, (T)0, (T)0, x,
				(T)0, (T)1, (T)0, y,
				(T)0, (T)0, (T)1, z,
				(T)0, (T)0, (T)0, (T)1
				);
		}
		static tmat4 translation(tvec3<T> const &vec) {
			return tmat4(
				(T)1, (T)0, (T)0, vec.x,
				(T)0, (T)1, (T)0, vec.y,
				(T)0, (T)0, (T)1, vec.z,
				(T)0, (T)0, (T)0, (T)1
				);
		}
		static tmat4 lookAt(tvec3<T> const &position, tvec3<T> const &direction, tvec3<T> const &up) {
			tvec3<T> f = (direction - position).normalize();
			tvec3<T> u = up.normalize();
			tvec3<T> s = cross(f, u).normalize();
			u = cross(s, f);

			return tmat4(
				s.x, s.y, s.z, -dot(s, position),
				u.x, u.y, u.z, -dot(u, position),
				-f.x, -f.y, -f.z, dot(f, position),
				(T)0, (T)0, (T)0, (T)1
				);
		}

		static tmat4 scale(T const &x, T const &y, T const &z) {
			return tmat4(
				x, (T)0, (T)0, (T)0,
				(T)0, y, (T)0, (T)0,
				(T)0, (T)0, z, (T)0,
				(T)0, (T)0, (T)0, (T)1
				);
		}

		static tmat4 scale(tvec3<T> const &factor) {
			return tmat4(
				factor.h[0], (T)0, (T)0, (T)0,
				(T)0, factor.h[1], (T)0, (T)0,
				(T)0, (T)0, factor.h[2], (T)0,
				(T)0, (T)0, (T)0, (T)1
				);
		}

	private:
		T h[16];

		friend class ::uniform;
		friend class tmat3<T>;
	};

	// Cast constructors
	template<typename T>
	inline tmat4<T>::tmat4(tmat3<T> const &mat) :
		tmat4(
		mat.h[0], mat.h[3], mat.h[6], (T)0,
		mat.h[1], mat.h[4], mat.h[7], (T)0,
		mat.h[2], mat.h[5], mat.h[8], (T)0,
		(T)0, (T)0, (T)0, (T)1) {
	}
	template<typename T>
	inline tmat3<T>::tmat3(tmat4<T> const &mat) :
		tmat3(
		mat.h[0], mat.h[4], mat.h[8]
		mat.h[1], mat.h[5], mat.h[9],
		mat.h[2], mat.h[6], mat.h[10]) {
	}
}

