#ifndef XAUDIO2VECTOR_H
#define XAUDIO2VECTOR_H

#include <math.h>

static constexpr float OX_EPSILON = 0.000001f;	// Œë·.

// 3¬•ªfloat
struct XAudio2Vec {
	float x, y, z;

	XAudio2Vec() {}
	XAudio2Vec(float x, float y, float z) : x(x), y(y), z(z) {}
	~XAudio2Vec() {}

	XAudio2Vec operator +(const XAudio2Vec &r) const {
		return XAudio2Vec(x + r.x, y + r.y, z + r.z);
	}

	XAudio2Vec operator -(const XAudio2Vec &r) const {
		return XAudio2Vec(x - r.x, y - r.y, z - r.z);
	}

	XAudio2Vec operator -() const {
		return XAudio2Vec(x * -1.0f, y * -1.0f, z * -1.0f);
	}

	XAudio2Vec operator *(const XAudio2Vec &r) const {
		return XAudio2Vec(x * r.x, y * r.y, z * r.z);
	}

	XAudio2Vec operator /(const XAudio2Vec &r) const {
		return XAudio2Vec(x / r.x, y / r.y, z / r.z);
	}

	XAudio2Vec operator *(float r) const {
		return XAudio2Vec(x * r, y * r, z * r);
	}

	XAudio2Vec operator /(float r) const {
		return XAudio2Vec(x / r, y / r, z / r);
	}

	friend XAudio2Vec operator *(float l, const XAudio2Vec &r) {
		return XAudio2Vec(r.x * l, r.y * l, r.z * l);
	}

	friend XAudio2Vec operator /(float l, const XAudio2Vec &r) {
		return XAudio2Vec(r.x / l, r.y / l, r.z / l);
	}

	float dot(const XAudio2Vec &r) const {
		return x * r.x + y * r.y + z * r.z;
	}

	XAudio2Vec cross(const XAudio2Vec &r) const {
		return XAudio2Vec(y * r.z - z * r.y, z * r.x - x * r.z, x * r.y - y * r.x);
	}

	float length() const {
		return sqrtf(lengthSq());
	}

	float lengthSq() const {
		return x * x + y * y + z * z;
	}

	void norm() {
		const float len = length();
		if (len > 0.0f) {
			x /= len;
			y /= len;
			z /= len;
		}
	}

	XAudio2Vec getNorm() const {
		const float len = length();
		if (len > 0.0f) {
			return XAudio2Vec(x / len, y / len, z / len);
		}
		return XAudio2Vec(0.0f, 0.0f, 0.0f);
	}
};

#endif // XAUDIO2VECTOR_H.