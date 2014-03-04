#pragma once

namespace math {
	template<typename T>
	class tvec2;
	typedef tvec2<bool> bvec2;
	typedef tvec2<int> ivec2;
	typedef tvec2<unsigned int> uvec2;
	typedef tvec2<float> vec2;
	typedef tvec2<double> dvec2;

	template<typename T>
	class tvec3;
	typedef tvec3<bool> bvec3;
	typedef tvec3<int> ivec3;
	typedef tvec3<unsigned int> uvec3;
	typedef tvec3<float> vec3;
	typedef tvec3<double> dvec3;

	template<typename T>
	class tvec4;
	typedef tvec4<bool> bvec4;
	typedef tvec4<int> ivec4;
	typedef tvec4<unsigned int> uvec4;
	typedef tvec4<float> vec4;
	typedef tvec4<double> dvec4;


	template<typename T>
	class tmat4;
}

#include <math.h>

// Depencies
class uniform;
namespace math {
	template<typename T>
	class tversor;
}

namespace math {
	template<typename T>
	class tvec2 {
	public:
		explicit tvec2() {
			this->x = (T)0;
			this->y = (T)0;
		}
		explicit tvec2(T const &x, T const &y) {
			this->x = x;
			this->y = y;
		}

		// Cast constructors
		tvec2(tvec3<T> const &vec);
		tvec2(tvec4<T> const &vec);

		// Some vector specific functions
		T length2() const {
			return
				this->x * this->x +
				this->y * this->y;
		}
		T length() const {
			return sqrt(this->length2());
		}
		tvec2 normalize() const {
			T l = this->length();
			return tvec2(
				this->x / l,
				this->y / l);
		}

		
		// Artihmetic operators
		tvec2 operator-() const {
			return tvec2(
				-this->x,
				-this->y);
		}

		tvec2 operator+(tvec2 const &other) const {
			return tvec2(
				this->x + other.x,
				this->y + other.y);
		}
		tvec2 operator-(tvec2 const &other) const {
			return tvec2(
				this->x - other.x,
				this->y - other.y);
		}
		tvec2 operator*(tvec2 const &other) const {
			return tvec2(
				this->x * other.x,
				this->y * other.y);
		}
		tvec2 operator/(tvec2 const &other) const {
			return tvec2(
				this->x / other.x,
				this->y / other.y);
		}

		// Assignment operators
		tvec2 const& operator=(tvec2 const& value) {
			this->x = value.x;
			this->y = value.y;

			return *this;
		}
		tvec2 const& operator+=(tvec2 const& other) {
			this->x += other.x;
			this->y += other.y;

			return *this;
		}
		tvec2 const& operator-=(tvec2 const& other) {
			this->x -= other.x;
			this->y -= other.y;

			return *this;
		}
		tvec2 const& operator*=(tvec2 const& other) {
			this->x *= other.x;
			this->y *= other.y;

			return *this;
		}
		tvec2 const& operator/=(tvec2 const& other) {
			this->x /= other.x;
			this->y /= other.y;

			return *this;
		}

		T x, y;

	private:
	};

	template<typename T>
	class tvec3 {
	public:
		explicit tvec3() {
			this->x = (T)0;
			this->y = (T)0;
			this->z = (T)0;
		}
		explicit tvec3(T const &x, T const &y, T const &z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		// Cast constructors
		tvec3(tvec2<T> const &vec);
		tvec3(tvec4<T> const &vec);

		// Some vector specific functions
		T length2() const {
			return
				this->x * this->x +
				this->y * this->y +
				this->z * this->z;
		}
		T length() const {
			return sqrt(this->length2());
		}
		tvec3 normalize() const {
			T l = this->length();
			return tvec3(
				this->x / l,
				this->y / l,
				this->z / l);
		}

		// Arithmetic operators
		tvec3 operator-() const {
			return tvec3(
				-this->x,
				-this->y,
				-this->z);
		}

		tvec3 operator+(tvec3 const &other) const {
			return tvec3(
				this->x + other.x,
				this->y + other.y,
				this->z + other.z);
		}
		tvec3 operator-(tvec3 const &other) const {
			return tvec3(
				this->x - other.x,
				this->y - other.y,
				this->z - other.z);
		}
		tvec3 operator*(tvec3 const &other) const {
			return tvec3(
				this->x * other.x,
				this->y * other.y,
				this->z * other.z);
		}
		tvec3 operator/(tvec3 const &other) const {
			return tvec3(
				this->x / other.x,
				this->y / other.y,
				this->z / other.z);
		}

		// Assignment operators
		tvec3 const& operator=(tvec3 const& value) {
			this->x = value.x;
			this->y = value.y;
			this->z = value.z;

			return *this;
		}
		tvec3 const& operator+=(tvec3 const& other) {
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;

			return *this;
		}
		tvec3 const& operator-=(tvec3 const& other) {
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;

			return *this;
		}
		tvec3 const& operator*=(tvec3 const& other) {
			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;

			return *this;
		}
		tvec3 const& operator/=(tvec3 const& other) {
			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;

			return *this;
		}

		T x, y, z;

	private:
	};
	template<typename T>
	class tvec4 {
	public:
		explicit tvec4() {
			this->x = (T)0;
			this->y = (T)0;
			this->z = (T)0;
			this->w = (T)0;
		}

		explicit tvec4(T const &x, T const &y, T const &z, T const &w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		// Cast constructors
		tvec4(tvec2<T> const &vec);
		tvec4(tvec3<T> const &vec);

		// Some vector specific functions
		T length2() const {
			return
				this->x * this->x +
				this->y * this->y +
				this->z * this->z +
				this->w * this->w;
		}
		T length() const {
			return sqrt(this->length2());
		}
		tvec4 normalize() const {
			T l = this->length();
			return tvec4(
				this->x / l,
				this->y / l,
				this->z / l,
				this->w / l);
		}

		// Arithmetic operators
		tvec4 operator-() const {
			return tvec4(
				-this->x,
				-this->y,
				-this->z,
				-this->w);
		}

		tvec4 operator+(tvec4 const &other) const {
			return tvec4(
				this->x + other.x,
				this->y + other.y,
				this->z + other.z,
				this->w + other.w);
		}
		tvec4 operator-(tvec4 const &other) const {
			return tvec4(
				this->x - other.x,
				this->y - other.y,
				this->z - other.z,
				this->w - other.w);
		}
		tvec4 operator*(tvec4 const &other) const {
			return tvec4(
				this->x * other.x,
				this->y * other.y,
				this->z * other.z,
				this->w * other.w);
		}
		tvec4 operator/(tvec4 const &other) const {
			return tvec4(
				this->x / other.x,
				this->y / other.y,
				this->z / other.z,
				this->w / other.w);
		}

		// Assignment operators
		tvec4 const& operator=(tvec4 const& value) {
			this->x = value.x;
			this->y = value.y;
			this->z = value.z;
			this->w = value.w;

			return *this;
		}
		tvec4 const& operator+=(tvec4 const& other) {
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			this->w += value.w;

			return *this;
		}
		tvec4 const& operator-=(tvec4 const& other) {
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			this->w -= value.w;

			return *this;
		}
		tvec4 const& operator*=(tvec4 const& other) {
			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
			this->w *= value.w;

			return *this;
		}
		tvec4 const& operator/=(tvec4 const& other) {
			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;
			this->w /= value.w;

			return *this;
		}

		T x, y, z, w;

	private:
	};

	// Constructors for type casting
	template<typename T>
	inline tvec2<T>::tvec2(tvec3<T> const &vec) {
		this->x = vec.x;
		this->y = vec.y;
	}
	template<typename T>
	inline tvec2<T>::tvec2(tvec4<T> const &vec) {
		this->x = vec.x;
		this->y = vec.y;
	}

	template<typename T>
	inline tvec3<T>::tvec3(tvec2<T> const &vec) {
		this->x = vec.x;
		this->y = vec.y;
		this->z = (T)0;
	}
	template<typename T>
	inline tvec3<T>::tvec3(tvec4<T> const &vec) {
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
	}

	template<typename T>
	inline tvec4<T>::tvec4(tvec2<T> const &vec) {
		this->x = vec.x;
		this->y = vec.y;
		this->z = (T)0;
		this->w = (T)0;
	}
	template<typename T>
	inline tvec4<T>::tvec4(tvec3<T> const &vec) {
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
		this->w = (T)0;
	}

	// Dot and cross products
	template<typename T>
	T dot(tvec2<T> const &a, tvec2<T> const &b) {
		return a.x*b.x + a.y*b.y;
	}
	template<typename T>
	T dot(tvec3<T> const &a, tvec3<T> const &b) {
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}
	template<typename T>
	T dot(tvec4<T> const &a, tvec4<T> const &b) {
		return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
	}

	template<typename T>
	tvec3<T> cross(tvec3<T> const &a, tvec3<T> const &b) {
		return tvec3<T>(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
	}

	// Arithmetic operators by number
	template<typename T>
	tvec2<T> operator*(tvec2<T> const &vec, T const val) {
		return tvec2<T>(
			vec.x * val,
			vec.y * val);
	}
	template<typename T>
	tvec2<T> operator*(T const val, tvec2<T> const &vec) {
		return tvec2<T>(
			vec.x * val,
			vec.y * val);
	}
	template<typename T>
	tvec3<T> operator*(tvec3<T> const &vec, T const val) {
		return tvec3<T>(
			vec.x * val,
			vec.y * val,
			vec.z * val);
	}
	template<typename T>
	tvec3<T> operator*(T const val, tvec3<T> const &vec) {
		return tvec3<T>(
			vec.x * val,
			vec.y * val,
			vec.z * val);
	}
	template<typename T>
	tvec4<T> operator*(tvec4<T> const &vec, T const val) {
		return tvec4<T>(
			vec.x * val,
			vec.y * val,
			vec.z * val,
			vec.w * val);
	}
	template<typename T>
	tvec4<T> operator*(T const val, tvec4<T> const &vec) {
		return tvec4<T>(
			vec.x * val,
			vec.y * val,
			vec.z * val,
			vec.w * val);
	}
}