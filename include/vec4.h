#pragma once

namespace math {
	class vec4;
}

#include "mat4.h"

namespace math {
	class vec4
	{
	public:
		vec4(float const &x, float const &y, float const &z, float const &w) {
			this->h = new float[4];
			this->h[0] = x;
			this->h[1] = y;
			this->h[2] = z;
			this->h[3] = w;
		}
		virtual ~vec4() {
			delete[] this->h;
		}

	private:
		float *h;

		friend class mat4;
	};
}
