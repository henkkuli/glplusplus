#pragma once

namespace math {
	template<typename T>
	class tversor;
	typedef tversor<float> versor;
}

#include "vec.h"
#include "mat.h"

namespace math {
	template<typename T>
	class tversor {
	public:
		explicit tversor() {
			h[0] = 1;
			h[1] = 0;
			h[2] = 0;
			h[3] = 0;
		}

		explicit tversor(T const angle, tvec3<T> const &axis) {
			h[0] = cos(angle/2);
			const T s = sin(angle/2);
			h[1] = s * axis.x;
			h[2] = s * axis.y;
			h[3] = s * axis.z;
		}

		explicit tversor(T const q0, T const q1, T const q2, T const q3) {
			h[0] = q0;
			h[1] = q1;
			h[2] = q2;
			h[3] = q3;
		}

		tversor operator*(tversor const &other) const {
			return tversor(
				other.h[0] * h[0] - other.h[1] * h[1] - other.h[2] * h[2] - other.h[3] * h[3],
				other.h[0] * h[1] + other.h[1] * h[0] - other.h[2] * h[3] + other.h[3] * h[2],
				other.h[0] * h[2] + other.h[1] * h[3] + other.h[2] * h[0] - other.h[3] * h[1],
				other.h[0] * h[3] - other.h[1] * h[2] + other.h[2] * h[1] + other.h[3] * h[0]
				);
		}

		tversor const& operator=(tversor const &value) {
			for (unsigned char i = 0; i < 4; i++)
				h[i] = value.h[i];
			return *this;
		}

		tmat3<T> getMatrix() const {
			const T w = h[0];
			const T x = h[1];
			const T y = h[2];
			const T z = h[3];
			return tmat3<T>(
				1 - 2 * y*y - 2 * z*z, 2 * x*y - 2 * w*z, 2 * x*z + 2 * w*y,
				2 * x*y + 2 * w*z, 1 - 2 * x*x - 2 * z*z, 2 * y*z - 2 * w*x,
				2 * x*z - 2 * w*y, 2 * y*z + 2 * w*x, 1 - 2 * x*x - 2 * y*y
				);
		}

	private:
		T h[4];
	};
}