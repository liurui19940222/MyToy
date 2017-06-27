#ifndef _MY_MATH_
#define _MY_MATH_

#include<math.h>
#include<string>
#define PI 3.141592653

class Vector3;
class Vector4;
class Matrix4x4;

class Vector2
{
public:
	float x;
	float y;

	Vector2();
	Vector2(Vector3& v3);
	Vector2(Vector2& v2);
	Vector2(float px, float py);

	float Magnitude();

	float MagnitudeSqrt();

	Vector2 Normalization();

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

	static float Dot(Vector2 &vec1, Vector2 &vec2);

	static float Angle(Vector2 &vec1, Vector2 &vec2);

	static Vector2 Projection(Vector2 &u, Vector2 &v);

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
	Vector3(Vector2& v2);
	Vector3(Vector3& v3);
	Vector3(Vector4& v4);
	Vector3(float px, float py, float pz);

	float Magnitude();

	float MagnitudeSqrt();

	Vector3 Normalization();

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

	static float Dot(Vector3 &vec1, Vector3 &vec2);

	static Vector3 Cross(Vector3 &vec1, Vector3 &vec2);

	static float Angle(Vector3 &vec1, Vector3 &vec2);

	static Vector3 Projection(Vector3 &u, Vector3 &v);

	static Vector3 Zero();

	static Vector3 One();

	static Vector3 Up();

	static Vector3 Left();

	static Vector3 Right();

	static Vector3 Forward();
};

class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;

	Vector4();
	Vector4(Vector3& v);
	Vector4(Vector4& v);
	Vector4(float px, float py, float pz);
	Vector4(float px, float py, float pz, float pw);
};

class Matrix4x4
{
public:
	Matrix4x4();

	Matrix4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);

	void Set(int x, int y, float value);

	float Get(int x, int y);

	Vector3 Multiply(Vector3& v3);

	Vector4 Multiply(Vector4& v4);

	Matrix4x4 Multiply(Matrix4x4& matrix4x4);

	Matrix4x4 Multiply(float value);

	Matrix4x4 operator*(Matrix4x4& matrix);

	Matrix4x4 Identity();

	Matrix4x4 Transpose();

	Vector3 EulerAngles();

	static Matrix4x4 Rotate(float pitch, float yaw, float roll);

private:
	float m[4][4];
};

class ParameterizedLine2D
{
public:
	Vector2 p0;
	Vector2 p1;
	Vector2 v;

	ParameterizedLine2D();
	ParameterizedLine2D(Vector2 p0, Vector2 p1);
	Vector2 GetPoint(float t);

	static bool Interset(ParameterizedLine2D* line1, ParameterizedLine2D* line2, Vector2* intersetPoint);
};

class ParameterizedLine3D
{
public:
	Vector3 p0;
	Vector3 p1;
	Vector3 v;

	ParameterizedLine3D();
	ParameterizedLine3D(Vector3 p0, Vector3 p1);
	Vector3 GetPoint(float t);
};

class Plane3D
{
public:
	Vector3 normal;
	Vector3 p0;

	Plane3D();
	Plane3D(Vector3 normal, Vector3 p0, bool isNormalize);
	float PointWithPlane(Vector3* p);
	bool IntersetLine(ParameterizedLine3D* line, Vector3* intersetPoint);
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
