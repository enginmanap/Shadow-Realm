#pragma once

#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <math.h>

namespace Jormag {

	#define PI 3.141592654
	#define DEGREES_RADIANS PI / 180.0f
	#define RADIANS_DEGREES 180.0f / PI

	struct Vector2 {
		float x, y;

		Vector2() {
			x = 0.0f;
			y = 0.0f;
		}
		Vector2(float x, float y) {
			this->x = x;
			this->y = y;
		}

		float Magnitude() {
			return (float)sqrt(x * x + y * y);
		}

		Vector2 Normalise() {
			float mag = Magnitude();
			return Vector2(x / mag, y / mag);
		}

		// *NOTE* FOLLOWING FOUND ON THE INTERNET
		Vector2& operator +=(const Vector2& rhs) {
			x += rhs.x;
			y += rhs.y;

			return *this;
		}

		Vector2& operator -=(const Vector2& rhs) {
			x -= rhs.x;
			y -= rhs.y;
		}

		Vector2 operator -() const {
			return Vector2(-x, -y);
		}
	};

	inline Vector2 operator +(const Vector2& lhs, const Vector2& rhs) {
		return Vector2(rhs.x + lhs.x, rhs.y + lhs.y);
	}

	inline Vector2 operator -(const Vector2& lhs, const Vector2& rhs) {
		return Vector2(rhs.x - lhs.x, rhs.y - lhs.y);
	}

	inline Vector2 operator *(const Vector2& lhs, const Vector2& rhs) {
		return Vector2(rhs.x * lhs.x, rhs.y * lhs.y);
	}

	inline Vector2 operator *(const Vector2& lhs, const float& rhs) {
		return Vector2(rhs * lhs.x, rhs * lhs.y);
	}

	inline Vector2 Rotate(Vector2& vector, float angle) {
		float radAngle = (float)(angle * DEGREES_RADIANS);
		return Vector2((float)(vector.x * cos(radAngle) - vector.y * sin(radAngle)), (float)(vector.x * sin(radAngle) + vector.y * cos(radAngle)));
	}

	inline Vector2 Lerp(Vector2& start, Vector2& end, float time) {
		if (time <= 0.0f)
			return start;

		if (time > 1.0f)
			return end;

		Vector2 dir = (end - start).Normalise();
		float mag = (end - start).Magnitude();

		return start + dir * mag * time;
	}

	const Vector2 V2_ZERO = { 0.0f, 0.0f };
	const Vector2 V2_ONE = { 1.0f, 1.0f };
	const Vector2 V2_UP = { 0.0f, 1.0f };
	const Vector2 V2_RIGHT = { 1.0f, 0.0f };
}

#endif