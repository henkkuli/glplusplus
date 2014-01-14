#pragma once

namespace math {
	class vec4;
}

#include <math.h>

//#include "../uniform.h"

namespace math {
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

	private:
		float h[4];

		friend class mat4;
	};
}
