#ifndef _MY_MATH_
#define _MY_MATH_

#include<math.h>
#include<string>
#define PI 3.141592653

class Vector3;
union Vector4;
class Matrix4x4;

class Vector2
{
public:
	float x;
	float y;

	Vector2();
	Vector2(const Vector3& v3);
	Vector2(const Vector2& v2);
	Vector2(float px, float py);

	float Magnitude() const;

	float MagnitudeSqrt() const;

	Vector2 Normalization() const;

	void Rotate(float angle);

	Vector2 operator+(const Vector2& vec) const;

	Vector2 operator-(const Vector2& vec) const;

	Vector2 operator*(const float value) const;

	Vector2 operator/(const float value) const;

	bool operator==(const Vector2& vec) const;

	bool operator!=(const Vector2& vec) const;

	void operator+=(const Vector2& vec);

	void operator-=(const Vector2& vec);

	void operator*=(const float value);

	void operator/=(const float value);

	Vector2 operator-() const;

	static float Dot(const Vector2 &vec1, const Vector2 &vec2);

	static float Angle(const Vector2 &vec1, const Vector2 &vec2);

	static Vector2 Projection(const Vector2 &u, const Vector2 &v);

	static Vector2 Zero();

	static Vector2 One();

	static Vector2 Up();

	static Vector2 Left();
};

class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3();
	Vector3(const Vector2& v2);
	Vector3(const Vector3& v3);
	Vector3(const Vector4& v4);
	Vector3(float px, float py, float pz);

	float Magnitude() const;

	float MagnitudeSqrt() const;

	Vector3 Normalization() const;

	Vector3 operator+(const Vector3& vec) const;

	Vector3 operator-(const Vector3& vec) const;

	Vector3 operator*(const float value) const;

	Vector3 operator/(const float value) const;

	Vector3 operator*(Matrix4x4& mat) const;

	Vector3 operator-() const;

	void operator+=(const Vector3& vec);

	void operator-=(const Vector3& vec);

	void operator*=(const float value);

	void operator/=(const float value);

	bool operator==(const Vector3& vec) const;

	bool operator!=(const Vector3& vec) const;

	static float Dot(const Vector3 &vec1, const Vector3 &vec2);

	static Vector3 Cross(const Vector3 &vec1, const Vector3 &vec2);

	static float Angle(const Vector3 &vec1, const Vector3 &vec2);

	static Vector3 Projection(const Vector3 &u, const Vector3 &v);

	static Vector3 Zero();

	static Vector3 One();

	static Vector3 Up();

	static Vector3 Left();

	static Vector3 Right();

	static Vector3 Forward();
};

union Vector4
{
	float m[4];
	struct
	{
		float x;
		float y;
		float z;
		float w;
	};

	float& operator[](size_t);

	float const & operator[](size_t) const;

	Vector4();
	Vector4(const Vector3& v);
	Vector4(const Vector4& v);
	Vector4(float px, float py, float pz);
	Vector4(float px, float py, float pz, float pw);
};

class Matrix4x4
{
public:
	Matrix4x4();

	Matrix4x4(float x0, float x1, float x2, float x3, float y0, float y1, float y2, float y3, float z0, float z1, float z2, float z3, float w0, float w1, float w2, float w3);

	void Set(int x, int y, float value);

	float Get(int x, int y);

	Vector3 Multiply(const Vector3& v3);

	Vector4 Multiply(const Vector4& v4);

	Matrix4x4 Multiply(Matrix4x4& matrix4x4);

	Matrix4x4 Multiply(float value);

	Matrix4x4 operator*(Matrix4x4& matrix);

	Vector3 operator*(Vector3& v);

	Matrix4x4 operator*(float value);

	Matrix4x4 operator/(float value);

	Vector4& operator[](size_t index);

	Matrix4x4 Transpose();

	Matrix4x4 Inverse();

	Vector3 EulerAngles();

	void MakeIdentity();

	void MakeZero();

	void MakeRotate(float pitch, float yaw, float roll);

	void MakeRotateUVN(const Vector3& targetPos, const Vector3& selfPos);

	void MakeTranslate(const Vector3& translate);

	void MakeScale(const Vector3& scale);

	void MakeOrtho(float left, float right, float bottom, float top, float zNear, float zFar);

	void MakePerspective(float fov, float aspect, float near, float far);

	void MakeLookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

	static Matrix4x4 Identity();

	static Matrix4x4 Rotate(float pitch, float yaw, float roll);

	static Matrix4x4 RotateUVN(const Vector3& targetPos, const Vector3& selfPos);

	static Matrix4x4 Translate(const Vector3& translate);

	static Matrix4x4 Scale(const Vector3& scale);

	static Matrix4x4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar);

	static Matrix4x4 Perspective(float fov, float aspect, float znear, float zfar);

	static Matrix4x4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

	static void Zero(Matrix4x4& mat);

	static void Identity(Matrix4x4& mat);

	static void Rotate(Matrix4x4& mat, float pitch, float yaw, float roll);

	static void RotateUVN(Matrix4x4& mat, const Vector3& targetPos, const Vector3& selfPos);

	static void Translate(Matrix4x4& mat, const Vector3& translate);

	static void Scale(Matrix4x4& mat, const Vector3& scale);

	static void Ortho(Matrix4x4& mat, float left, float right, float bottom, float top, float zNear, float zFar);

	static void Perspective(Matrix4x4& mat, float fov, float aspect, float near, float far);

	static void LookAt(Matrix4x4& mat, const Vector3& eye, const Vector3& center, const Vector3& up);

	static void GetUVN(Matrix4x4& mat, Vector3* u, Vector3* v, Vector3* n);

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

class CMath
{
public:
	CMath();
	~CMath();

	static float DegToRad(float angle);
	static float RadToDeg(float radian);
	static float Random();
	static float Random(float max);
	static float Random(float min, float max);
	static int Random(int max);
	static int Random(int min, int max);
	static void EualrAnglesToUVN(const Vector3* eualrAngles, Vector3* u, Vector3* v, Vector3* n);

	template<typename T>
	static T Min(T a, T b)
	{
		return (a < b ? a : b);
	}

	template<typename T>
	static T Max(T a, T b)
	{
		return (a > b ? a : b);
	}

	template<typename T>
	static T Clamp(T value, T min, T max)
	{
		return Max(Min(value, max), min);
	}
};

#endif
