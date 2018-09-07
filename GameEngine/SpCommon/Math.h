#ifndef _MY_MATH_
#define _MY_MATH_

#include<math.h>
#include<string>
#include"defs.h"

BEGIN_NAMESPACE_ENGINE

template<typename VType> class TmpVector2;
template<typename VType> class TmpVector3;
template<typename VType> union TmpVector4;
typedef TmpVector2<float> Vector2;
typedef TmpVector3<float> Vector3;
typedef TmpVector4<float> Vector4;

class Matrix4x4;
union Quaternion;

class CMath
{
public:
	static const float EPSILON;
	static const float PI;
	static const float TWO_PI;
	static const float HALF_PI;
	static const float QUARTER_PI;
	static const float DegToRad;
	static const float RadToDeg;
	static const float HalfDegToRad;

	inline static float Random()
	{
		static float max_rec = 1 / (float)RAND_MAX;
		return rand() * max_rec;
	}

	static Vector3 RandomVec3();

	static Vector3 RandomNormalizedVec3();

	inline static float Random(float max) { return Random() * (max); }
	inline static float Random(float min, float max) { return Random() * (max - min) + min; }
	inline static int Random(int max) { return (int)(Random((float)max) + 0.5f); }
	inline static int Random(int min, int max) { return (int)(Random((float)min, (float)max) + 0.5f); }

	inline static bool Approximately(float f0, float f1)
	{
		return abs(f0 - f1) < 1e-6;
	}

	template<typename T>
	inline static T Floor(T value)
	{
		return (T)((int)value);
	}

	template<typename T>
	inline static T Ceil(T value)
	{
		return (T)((int)(value + (1 - EPSILON)));
	}

	template<typename T>
	inline static T Abs(T value)
	{
		return value > 0 ? value : -value;
	}

	template<typename T>
	inline static T Min(T a, T b)
	{
		return (a < b ? a : b);
	}

	template<typename T>
	inline static T Max(T a, T b)
	{
		return (a > b ? a : b);
	}

	template<typename T>
	inline static T Clamp(T value, T min, T max)
	{
		return Max(Min(value, max), min);
	}

	template<typename T>
	inline static T Clamp01(T value)
	{
		return Max(Min(value, T(1)), T(0));
	}

	template<typename T>
	inline static T Lerp(T a, T b, T t)
	{
		return (b - a) * t + a;
	}

	template<typename T>
	inline static T Smooth(T a, T b, T t)
	{
		return -(b - a) * t * (t - 2) + a;
	}

	template<typename T>
	inline static T Sin(T t)
	{
		return sin(t);
	}

	template<typename T>
	inline static T Asin(T t)
	{
		return asin(t);
	}

	template<typename T>
	inline static T Cos(T t)
	{
		return cos(t);
	}

	template<typename T>
	inline static T Acos(T t)
	{
		return acos(t);
	}

	template<typename T>
	inline static T Tan(T t)
	{
		return tan(t);
	}

	template<typename T>
	inline static T Atan(T t)
	{
		return atan(t);
	}

	template<typename T>
	inline static T Pow(T base, T exponent)
	{
		return pow(base, exponent);
	}
};


template<typename VType>
class TmpVector2
{
public:
	VType x = 0;
	VType y = 0;

	TmpVector2() {}
	TmpVector2(const TmpVector3<VType>& v3) : x(v3.x), y(v3.y) {}
	TmpVector2(const TmpVector2<VType>& v2) : x(v2.x), y(v2.y) {}
	TmpVector2(VType px, VType py) : x(px), y(py) {}

	inline VType Magnitude() const { return sqrt(x*x + y*y); }

	inline VType MagnitudeSqrt() const { return x*x + y*y; }

	inline TmpVector2<VType> Normalize() const
	{
		float mag = Magnitude();
		return TmpVector2<VType>(x / mag, y / mag);
	}

	inline void Rotate(VType angle)
	{
		angle = angle / 180 * CMath::PI;
		float tx = x*cos(angle) - y*sin(angle);
		float ty = x*sin(angle) + y*cos(angle);
		x = tx;
		y = ty;
	}

	inline TmpVector2<VType> operator+(const TmpVector2<VType>& vec) const { return TmpVector2<VType>(this->x + vec.x, this->y + vec.y); }

	inline TmpVector2<VType> operator-(const TmpVector2<VType>& vec) const { return TmpVector2<VType>(this->x - vec.x, this->y - vec.y); }

	inline TmpVector2<VType> operator*(const VType value) const { return TmpVector2<VType>(this->x * value, this->y * value); }

	inline TmpVector2<VType> operator*(const TmpVector2<VType>& vec) const { return TmpVector2<VType>(this->x * vec.x, this->y * vec.y); }

	inline TmpVector2<VType> operator/(const VType value) const { return TmpVector2<VType>(this->x / value, this->y / value); }

	inline bool operator==(const TmpVector2<VType>& vec) const { return this->x == vec.x && this->y == vec.y; }

	inline bool operator!=(const TmpVector2<VType>& vec) const { return this->x != vec.x || this->y != vec.y; }

	inline void operator+=(const TmpVector2<VType>& vec)
	{
		x += vec.x;
		y += vec.y;
	}

	inline void operator-=(const TmpVector2<VType>& vec)
	{
		x -= vec.x;
		y -= vec.y;
	}

	inline void operator*=(const VType value)
	{
		x *= value;
		y *= value;
	}

	inline void operator*=(const TmpVector2<VType>& vec)
	{
		x *= vec.x;
		y *= vec.y;
	}

	inline void operator/=(const VType value) { (*this) *= (1.0f / value); }

	inline TmpVector2<VType> operator-() const { return Vector2(-x, -y); }

	inline static float Dot(const TmpVector2<VType> &vec1, const TmpVector2<VType> &vec2) { return vec1.x * vec2.x + vec1.y * vec2.y; }

	inline static float Angle(const TmpVector2<VType> &vec1, const TmpVector2<VType> &vec2) { return  acos(Dot(vec1.Normalize(), vec2.Normalize())); }

	inline static TmpVector2<VType> Projection(const TmpVector2<VType> &u, const TmpVector2<VType> &v)
	{
		float mag = v.Magnitude();
		return v * Dot(u, v) / (mag * mag);
	}

	static const TmpVector2<float> zero;
	static const TmpVector2<float> one;
	static const TmpVector2<float> up;
	static const TmpVector2<float> down;
	static const TmpVector2<float> left;
	static const TmpVector2<float> right;
};

template<typename VType>
class TmpVector3
{
public:
	VType x;
	VType y;
	VType z;

	TmpVector3() :x(0), y(0), z(0) {}
	TmpVector3(const TmpVector2<VType>& v2) : x(v2.x), y(v2.y), z(0) {}
	TmpVector3(const TmpVector3<VType>& v3) : x(v3.x), y(v3.y), z(v3.z) {}
	TmpVector3(const TmpVector4<VType>& v4) : x(v4.x), y(v4.y), z(v4.z) {}
	TmpVector3(VType px, VType py, VType pz) : x(px), y(py), z(pz) {}

	inline VType Magnitude() const { return sqrt(x * x + y*y + z*z); }

	inline VType MagnitudeSqrt() const { return x * x + y*y + z*z; }

	inline TmpVector3<VType> Normalize() const {
		float mag = 1.0f / Magnitude();
		return Vector3(x * mag, y * mag, z * mag);
	}

	inline void MakeNormalize()
	{
		float mag = 1.0f / Magnitude();
		x *= mag; y *= mag; z *= mag;
	}

	inline TmpVector3<VType> operator+(const TmpVector3<VType>& vec) const { return TmpVector3<VType>(x + vec.x, y + vec.y, z + vec.z); }

	inline TmpVector3<VType> operator-(const TmpVector3<VType>& vec) const { return TmpVector3<VType>(x - vec.x, y - vec.y, z - vec.z); }

	inline TmpVector3<VType> operator*(const VType value) const { return TmpVector3<VType>(x * value, y * value, z * value); }

	inline TmpVector3<VType> operator/(const VType value) const { return TmpVector3<VType>(x / value, y / value, z / value); }

	inline TmpVector3<VType> operator-() const { return TmpVector3<VType>(-x, -y, -z); }

	inline void operator+=(const TmpVector3<VType>& vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}

	inline void operator-=(const TmpVector3<VType>& vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}

	inline void operator*=(const VType value) {
		x *= value;
		y *= value;
		z *= value;
	}

	inline void operator/=(const VType value) { *(this) *= (1.0f / value); }

	inline bool operator==(const TmpVector3<VType>& vec) const { return this->x == vec.x && this->y == vec.y && this->z == vec.z; }

	inline bool operator!=(const TmpVector3<VType>& vec) const { return this->x != vec.x || this->y != vec.y || this->z != vec.z; }

	inline static VType Dot(const TmpVector3<VType> &vec1, const TmpVector3<VType> &vec2) { return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z; }

	inline static TmpVector3<VType> Cross(const TmpVector3<VType> &u, const TmpVector3<VType> &v) { return TmpVector3<VType>(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x); }

	inline static VType Angle(const TmpVector3<VType> &vec1, const TmpVector3<VType> &vec2) { return  acos(Dot(vec1.Normalize(), vec2.Normalize())) * CMath::RadToDeg; }

	inline static TmpVector3<VType> Projection(const TmpVector3<VType> &u, const TmpVector3<VType> &v) {
		float mag = v.Magnitude();
		return v * Dot(u, v) / (mag * mag);
	}

	inline static bool Approximately(const TmpVector3<VType>& a, const TmpVector3<VType>& b)
	{
		return CMath::Approximately(a.x, b.x) && CMath::Approximately(a.y, b.y) && CMath::Approximately(a.z, b.z);
	}

	static const TmpVector3<float> zero;
	static const TmpVector3<float> one;
	static const TmpVector3<float> up;
	static const TmpVector3<float> down;
	static const TmpVector3<float> left;
	static const TmpVector3<float> right;
	static const TmpVector3<float> forward;
	static const TmpVector3<float> back;
};

template<typename VType>
union TmpVector4
{
	VType m[4];
	struct
	{
		VType x;
		VType y;
		VType z;
		VType w;
	};
	struct
	{
		VType left;
		VType top;
		VType right;
		VType bottom;
	};

	VType& operator[](size_t index) { return m[index]; }

	TmpVector4() :x(0), y(0), z(0), w(0) {}
	TmpVector4(VType px, VType py, VType pz, VType pw) : x(px), y(py), z(pz), w(pw) {}
	TmpVector4(const TmpVector3<VType>& v3, VType pw) :x(v3.x), y(v3.y), z(v3.z), w(pw) {}
	inline bool equaltozero() const { return x == VType(0) && y == VType(0) && z == VType(0) && w == VType(0); }

	static const TmpVector4<float> zero;
};

class Matrix4x4
{
public:
	Matrix4x4();

	Matrix4x4(float* array);

	Matrix4x4(float oblique);

	Matrix4x4(float x0, float x1, float x2, float x3, float y0, float y1, float y2, float y3, float z0, float z1, float z2, float z3, float w0, float w1, float w2, float w3);

	void Set(int x, int y, float value);

	float Get(int x, int y);

	Vector3 Multiply(const Vector3& v3);

	Vector4 Multiply(const Vector4& v4);

	inline Matrix4x4 Multiply(Matrix4x4& mat);

	Matrix4x4 Multiply(float value);

	Matrix4x4 operator+(Matrix4x4& matrix);

	Matrix4x4 operator-(Matrix4x4& matrix);

	void operator+=(Matrix4x4& matrix);

	void operator-=(Matrix4x4& matrix);

	inline Matrix4x4 operator*(Matrix4x4& matrix) { return Multiply(matrix); }

	inline Vector3 operator*(const Vector3& v) { return Multiply(v); }

	inline Vector4 operator*(const Vector4& v) { return Multiply(v); }

	inline Matrix4x4 operator*(float value) { return Multiply(value); }

	inline Matrix4x4 operator/(float value) { return Multiply(1.0f / value); }

	inline Vector4& operator[](size_t index) { return m[index]; }

	Matrix4x4 Transpose();

	Matrix4x4 Inverse();

	void MakeIdentity();

	void MakeZero();

	void MakeRotate(float pitch, float yaw, float roll);

	void MakeRotate(const Vector3& eulerAngles);

	void MakeRotate(const Quaternion& q);

	void MakeRotateUVN(const Vector3& targetPos, const Vector3& selfPos);

	void MakeTranslate(const Vector3& translate);

	void MakeScale(const Vector3& scale);

	void MakeOrtho(float left, float right, float bottom, float top, float zNear, float zFar);

	void MakePerspective(float fov, float aspect, float near, float far);

	void MakeLookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

	Quaternion ToQuaternion();

	static Matrix4x4 Identity();

	static Matrix4x4 Rotate(float pitch, float yaw, float roll);

	static Matrix4x4 Rotate(const Vector3& eulerAngles);

	static Matrix4x4 RotateUVN(const Vector3& targetPos, const Vector3& selfPos);

	static Matrix4x4 Translate(const Vector3& translate);

	static Matrix4x4 Scale(const Vector3& scale);

	static Matrix4x4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar);

	static Matrix4x4 Perspective(float fov, float aspect, float znear, float zfar);

	static Matrix4x4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

	static Matrix4x4 Lerp(Matrix4x4& a, Matrix4x4& b, float t);

	static Quaternion ToQuaternion(Matrix4x4& mat);

	static void Zero(Matrix4x4& result);

	static void Identity(Matrix4x4& result);

	static void Multiply(Matrix4x4& result, Matrix4x4& a, Matrix4x4& b);

	static void Rotate(Matrix4x4& result, float pitch, float yaw, float roll);

	static void Rotate(Matrix4x4& result, const Quaternion& q);

	static void RotateUVN(Matrix4x4& result, const Vector3& targetPos, const Vector3& selfPos);

	static void Translate(Matrix4x4& result, const Vector3& translate);

	static void Scale(Matrix4x4& result, const Vector3& scale);

	static void Ortho(Matrix4x4& result, float left, float right, float bottom, float top, float zNear, float zFar);

	static void Perspective(Matrix4x4& result, float fov, float aspect, float near, float far);

	static void LookAt(Matrix4x4& result, const Vector3& eye, const Vector3& center, const Vector3& up);

	static void GetUVN(Matrix4x4& result, Vector3* u, Vector3* v, Vector3* n);

	static void GetUVN(Matrix4x4& result, const Vector3& scale, Vector3* u, Vector3* v, Vector3* n);

	static void GetPosition(Matrix4x4& result, Vector3* position);

	static void Lerp(Matrix4x4& result, Matrix4x4& a, Matrix4x4& b, float t);

	static const Matrix4x4 identity;

private:
	Vector4 m[4];
};

class ParameterizedLine2D
{
public:
	Vector2 p0;
	Vector2 p1;
	Vector2 v;

	ParameterizedLine2D();
	ParameterizedLine2D(const Vector2& p0, const Vector2& p1);
	Vector2 GetPoint(float t);

	static bool Interset(const ParameterizedLine2D& line0, const ParameterizedLine2D& line1, Vector2* intersetPoint);
};

class ParameterizedLine3D
{
public:
	Vector3 p0;
	Vector3 p1;
	Vector3 v;

	ParameterizedLine3D();
	ParameterizedLine3D(const Vector3& p0, const Vector3& p1);
	Vector3 GetPoint(float t) const;
};

class Plane3D
{
public:
	Vector3 normal;
	Vector3 p0;

	Plane3D();
	Plane3D(const Vector3& normal, const Vector3& p0, bool isNormalize);
	float PointWithPlane(const Vector3& p) const;
	bool IntersetLine(const ParameterizedLine3D& line, Vector3* intersetPoint) const;
};

struct STriangle
{
	Vector3 verties[3];
};


union Quaternion
{
public:
	float m[4];
	struct {
		float x, y, z, w;
	};
	Quaternion();
	Quaternion(const Vector3& vec);
	Quaternion(float px, float py, float pz, float pw);
	float& operator[](int index);
	bool operator==(const Quaternion& q) const;
	bool operator!=(const Quaternion& q) const;
	Vector3 operator*(const Vector3& vec) const;
	Quaternion operator*(const Quaternion& vec) const;
	Quaternion operator-();

	void MakeEuler(const Vector3& euler);
	void MakeNormalize();
	void MakeIdentity();
	void ToAngleAxis(const Quaternion& q, Vector3* outVec, float* outAngle) const;
	Quaternion Normalize() const;
	Vector3 ToEulerAngles() const;
	Vector3 Multiply(const Vector3& vec) const;
	Quaternion Multiply(const Quaternion& q) const;

	static Quaternion AngleAxis(const Vector3& axis, float angle);
	static Quaternion Euler(const Vector3& euler);
	static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t);
	static Quaternion UnclampedSlerp(const Quaternion& a, const Quaternion& b, float t);
	static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t);
	static Quaternion LookRotation(const Vector3& forward, const Vector3& up);
	static float Angle(const Quaternion& a, const Quaternion& b);
	static float Dot(const Quaternion& a, const Quaternion& b);
	static void SanitizeEuler(Vector3& euler);

	static const Quaternion identity;
private:
	static const float _next[3];
	static const float negativeFlip;
	static const float positiveFlip;
};

END_NAMESPACE_ENGINE

#endif
