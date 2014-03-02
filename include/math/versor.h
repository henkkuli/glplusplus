#pragma once

namespace math {
	class versor;
}

#include "vec.h"
#include "mat.h"

namespace math {
	class versor {
	public:
		explicit versor() {
			h[0] = 1;
			h[1] = 0;
			h[2] = 0;
			h[3] = 0;
		}

		explicit versor(float const angle, vec3 const &axis) {
			h[0] = cosf(angle/2);
			const float sin = sinf(angle/2);
			h[1] = sin * axis.h[0];
			h[2] = sin * axis.h[1];
			h[3] = sin * axis.h[2];
		}

		explicit versor(float const q0, float const q1, float const q2, float const q3) {
			h[0] = q0;
			h[1] = q1;
			h[2] = q2;
			h[3] = q3;
		}

		versor operator*(versor const &other) const {
			return versor(
				other.h[0] * h[0] - other.h[1] * h[1] - other.h[2] * h[2] - other.h[3] * h[3],
				other.h[0] * h[1] + other.h[1] * h[0] - other.h[2] * h[3] + other.h[3] * h[2],
				other.h[0] * h[2] + other.h[1] * h[3] + other.h[2] * h[0] - other.h[3] * h[1],
				other.h[0] * h[3] - other.h[1] * h[2] + other.h[2] * h[1] + other.h[3] * h[0]
				);
		}

		versor const& operator=(versor const &value) {
			for (unsigned char i = 0; i < 4; i++)
				h[i] = value.h[i];
			return *this;
		}

		mat3 getMatrix() const {
			const float w = h[0];
			const float x = h[1];
			const float y = h[2];
			const float z = h[3];
			return mat3(
				1 - 2 * y*y - 2 * z*z, 2 * x*y - 2 * w*z, 2 * x*z + 2 * w*y,
				2 * x*y + 2 * w*z, 1 - 2 * x*x - 2 * z*z, 2 * y*z - 2 * w*x,
				2 * x*z - 2 * w*y, 2 * y*z + 2 * w*x, 1 - 2 * x*x - 2 * y*y
				);
		}

	private:
		float h[4];
	};
}