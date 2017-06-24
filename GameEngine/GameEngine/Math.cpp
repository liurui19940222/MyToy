#include "Math.h"

#pragma region Vector2

Vector2::Vector2() :x(0), y(0) {}
Vector2::Vector2(Vector3& v3) : x(v3.x), y(v3.y) {}
Vector2::Vector2(Vector2& v2) : x(v2.x), y(v2.y) {}
Vector2::Vector2(float px, float py) : x(px), y(py) {}

float Vector2::Magnitude()
{
	return sqrt(x*x + y*y);
}

float Vector2::MagnitudeSqrt()
{
	return x*x + y*y;
}

Vector2 Vector2::Normalization()
{
	float mag = Magnitude();
	return Vector2(x / mag, y / mag);
}

void Vector2::Rotate(float angle)
{
	angle = angle / 180 * PI;
	float tx = x*cos(angle) - y*sin(angle);
	float ty = x*sin(angle) + y*cos(angle);
	x = tx;
	y = ty;
}

Vector2 Vector2::operator+(const Vector2& vec) const
{
	return Vector2(this->x + vec.x, this->y + vec.y);
}

Vector2 Vector2::operator-(const Vector2& vec) const
{
	return Vector2(this->x - vec.x, this->y - vec.y);
}

Vector2 Vector2::operator*(const float value) const
{
	return Vector2(this->x * value, this->y*value);
}

Vector2 Vector2::operator/(const float value) const
{
	return Vector2(this->x / value, this->y / value);
}

bool Vector2::operator==(const Vector2& vec) const
{
	return this->x == vec.x && this->y == vec.y;
}

bool Vector2::operator!=(const Vector2& vec) const
{
	return this->x != vec.x || this->y != vec.y;
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

float Vector2::Dot(Vector2 &vec1, Vector2 &vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

float Vector2::Angle(Vector2 &vec1, Vector2 &vec2)
{
	Vector2 nor1 = vec1.Normalization();
	Vector2 nor2 = vec2.Normalization();
	return  acos(Dot(nor1, nor2));
}

Vector2 Vector2::Projection(Vector2 &u, Vector2 &v)
{
	float mag = v.Magnitude();
	return v * Dot(u, v) / (mag * mag);
}

Vector2 Vector2::Zero()
{
	return Vector2(0, 0);
}

Vector2 Vector2::One()
{
	return Vector2(1, 1);
}

Vector2 Vector2::Up()
{
	return Vector2(0, 1);
}

Vector2 Vector2::Left()
{
	return Vector2(-1, 0);
}

#pragma endregion

#pragma region Vector3

Vector3::Vector3() :x(0), y(0), z(0) {}
Vector3::Vector3(Vector2& v2) : x(v2.x), y(v2.y), z(0) {}
Vector3::Vector3(Vector3& v3) : x(v3.x), y(v3.y), z(v3.z) {}
Vector3::Vector3(Vector4& v4) : x(v4.x), y(v4.y), z(v4.z) {}
Vector3::Vector3(float px, float py, float pz) : x(px), y(py), z(pz) {}

float Vector3::Magnitude()
{
	return sqrt(x * x + y*y + z*z);
}

float Vector3::MagnitudeSqrt()
{
	return x * x + y*y + z*z;
}

Vector3 Vector3::Normalization()
{
	float mag = Magnitude();
	return Vector3(x / mag, y / mag, z / mag);
}

Vector3 Vector3::operator+(const Vector3& vec) const
{
	return Vector3(x + vec.x, y + vec.y, z + vec.z);
}

Vector3 Vector3::operator-(const Vector3& vec) const
{
	return Vector3(x - vec.x, y - vec.y, z - vec.z);
}

Vector3 Vector3::operator*(const float value) const
{
	return Vector3(x * value, y * value, z * value);
}

Vector3 Vector3::operator/(const float value) const
{
	return Vector3(x / value, y / value, z / value);
}

bool Vector3::operator==(const Vector3& vec) const
{
	return this->x == vec.x && this->y == vec.y && this->z == vec.z;
}

bool Vector3::operator!=(const Vector3& vec) const
{
	return this->x != vec.x || this->y != vec.y || this->z != vec.z;
}

Vector3 Vector3::operator*(Matrix4x4& mat) const
{
	return mat.Multiply(Vector3(x, y, z));
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

float Vector3::Dot(Vector3 &vec1, Vector3 &vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

Vector3 Vector3::Cross(Vector3 &u, Vector3 &v)
{
	return Vector3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

float Vector3::Angle(Vector3 &vec1, Vector3 &vec2)
{
	Vector3 nor1 = vec1.Normalization();
	Vector3 nor2 = vec2.Normalization();
	return  acos(Dot(nor1, nor2));
}

Vector3 Vector3::Projection(Vector3 &u, Vector3 &v)
{
	float mag = v.Magnitude();
	return v * Dot(u, v) / (mag * mag);
}

Vector3 Vector3::Zero()
{
	return Vector3(0, 0, 0);
}

Vector3 Vector3::One()
{
	return Vector3(1, 1, 1);
}

Vector3 Vector3::Up()
{
	return Vector3(0, 1, 0);
}

Vector3 Vector3::Left()
{
	return Vector3(-1, 0, 0);
}

Vector3 Vector3::Right()
{
	return Vector3(1, 0, 0);
}

Vector3 Vector3::Forward()
{
	return Vector3(0, 0, 1);
}

#pragma endregion

#pragma region Vector4

Vector4::Vector4() :x(0), y(0), z(0), w(1) { }

Vector4::Vector4(Vector3& v) : x(v.x), y(v.y), z(v.z), w(1) { }

Vector4::Vector4(Vector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) { }

Vector4::Vector4(float px, float py, float pz) : x(px), y(py), z(pz), w(1) { }

Vector4::Vector4(float px, float py, float pz, float pw) : x(px), y(py), z(pz), w(pw) { }

#pragma endregion

#pragma region Matrix4x4

Matrix4x4::Matrix4x4()
{
	memset(m, 0, sizeof(float) * 16);
}

Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
	m[0][0] = m00; m[1][0] = m10; m[2][0] = m20; m[3][0] = m30;
	m[0][1] = m01; m[1][1] = m11; m[2][1] = m21; m[3][1] = m31;
	m[0][2] = m02; m[1][2] = m12; m[2][2] = m22; m[3][2] = m32;
	m[0][3] = m03; m[1][3] = m13; m[2][3] = m23; m[3][3] = m33;
}

void Matrix4x4::Set(int x, int y, float value)
{
	m[x][y] = value;
}

float Matrix4x4::Get(int x, int y)
{
	return m[x][y];
}

Vector3 Matrix4x4::Multiply(Vector3& v3)
{

	float x = 0, y = 0, z = 0;
	x = v3.x * m[0][0] + v3.y * m[0][1] + v3.z * m[0][2] + m[0][3];
	y = v3.x * m[1][0] + v3.y * m[1][1] + v3.z * m[1][2] + m[1][3];
	z = v3.x * m[2][0] + v3.y * m[2][1] + v3.z * m[2][2] + m[2][3];
	return Vector3(x, y, z);
}

Vector4 Matrix4x4::Multiply(Vector4& v)
{
	return Vector4(
		m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
		m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
		m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
		m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
		);
}

Matrix4x4 Matrix4x4::Multiply(Matrix4x4& mat)
{
	Matrix4x4 temp;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			temp.Set(j, i, mat.Get(0, i) * m[j][0]+ mat.Get(1, i) * m[j][1] + mat.Get(2, i) * m[j][2] + mat.Get(3, i) * m[j][3]);

	return temp;
}

Matrix4x4 Matrix4x4::Multiply(float value)
{
	return Matrix4x4(m[0][0] * value, m[1][0] * value, m[2][0] * value, m[3][0] * value,
		m[0][1] * value, m[1][1] * value, m[2][1] * value, m[3][1] * value,
		m[0][2] * value, m[1][2] * value, m[2][2] * value, m[3][2] * value,
		m[0][3] * value, m[1][3] * value, m[2][3] * value, m[3][3] * value);
}

Matrix4x4 Matrix4x4::operator*(Matrix4x4& matrix)
{
	return Multiply(matrix);
}

Matrix4x4 Matrix4x4::Identity()
{
	return Matrix4x4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::Rotate(float pitch, float yaw, float roll)
{
	Matrix4x4 x, y, z;
	float sin_x = sin(pitch * PI / 180.0f);
	float sin_y = sin(yaw * PI / 180.0f);
	float sin_z = sin(roll * PI / 180.0f);
	float cos_x = cos(pitch * PI / 180.0f);
	float cos_y = cos(yaw * PI / 180.0f);
	float cos_z = cos(roll * PI / 180.0f);
	x.Set(0, 0, 1); x.Set(1, 0, 0); x.Set(2, 0, 0); x.Set(3, 0, 0);
	x.Set(0, 1, 0); x.Set(1, 1, cos_x); x.Set(2, 1, -sin_x); x.Set(3, 1, 0);
	x.Set(0, 2, 0); x.Set(1, 2, sin_x); x.Set(2, 2, cos_x); x.Set(3, 2, 0);
	x.Set(0, 3, 0); x.Set(1, 3, 0); x.Set(2, 3, 0); x.Set(3, 3, 1);

	y.Set(0, 0, cos_y); y.Set(1, 0, 0); y.Set(2, 0, sin_y); y.Set(3, 0, 0);
	y.Set(0, 1, 0); y.Set(1, 1, 1); y.Set(2, 1, 0); y.Set(3, 1, 0);
	y.Set(0, 2, -sin_y); y.Set(1, 2, 0); y.Set(2, 2, cos_y); y.Set(3, 2, 0);
	y.Set(0, 3, 0); y.Set(1, 3, 0); y.Set(2, 3, 0); y.Set(3, 3, 1);

	z.Set(0, 0, cos_z); z.Set(1, 0, -sin_z); z.Set(2, 0, 0); z.Set(3, 0, 0);
	z.Set(0, 1, sin_z); z.Set(1, 1, cos_z); z.Set(2, 1, 0); z.Set(3, 1, 0);
	z.Set(0, 2, 0); z.Set(1, 2, 0); z.Set(2, 2, 1); z.Set(3, 2, 0);
	z.Set(0, 3, 0); z.Set(1, 3, 0); z.Set(2, 3, 0); z.Set(3, 3, 1);

	return (z * x * y).Transpose();
}

Matrix4x4 Matrix4x4::Transpose()
{
	return Matrix4x4(m[0][0], m[1][0], m[2][0], m[3][0],
		m[0][1], m[1][1], m[2][1], m[3][1],
		m[0][2], m[1][2], m[2][2], m[3][2],
		m[0][3], m[1][3], m[2][3], m[3][3]);
}

Vector3 Matrix4x4::EulerAngles()
{
	Vector3 euler;

	float h = 0, p = 0, b = 0;
	float sp = -Get(1, 2);
	if (sp <= -1.0f)
	{
		p = -1.570796f;
	}
	else if (sp >= 1.0f)
	{
		p = 1.570796f;
	}
	else
	{
		p = asin(sp);
	}
	if (sp > 0.9999f)
	{
		b = 0;
		h = atan2(-Get(2, 0), Get(0, 0));
	}
	else
	{
		h = atan2(Get(0, 2), Get(2, 2));
		b = atan2(Get(1, 0), Get(1, 1));
	}

	euler.y = CMath::RadToDeg(h);
	euler.x = CMath::RadToDeg(p);
	euler.z = CMath::RadToDeg(b);
	if (euler.x < 0) euler.x = 360 + euler.x;
	if (euler.y < 0) euler.y = 360 + euler.y;
	if (euler.z < 0) euler.z = 360 + euler.z;
	return euler;
}

#pragma endregion

#pragma region ParameterizedLine2D

ParameterizedLine2D::ParameterizedLine2D()
{
}

ParameterizedLine2D::ParameterizedLine2D(Vector2 p0, Vector2 p1)
{
	this->p0 = p0;
	this->p1 = p1;
	this->v = Vector2(p1.x - p0.x, p1.y - p0.y);
}

Vector2 ParameterizedLine2D::GetPoint(float t)
{
	return Vector2(p0.x + v.x * t, p0.y + v.y * t);
}

bool ParameterizedLine2D::Interset(ParameterizedLine2D* line0, ParameterizedLine2D* line1, Vector2* intersetPoint)
{
	if (line1->v.x * line0->v.y == line1->v.y * line0->v.x)
		return false;

	float t0 = (line1->v.y * (line1->p0.x - line0->p0.x) - line1->v.x * (line1->p0.y - line0->p0.y)) / (line0->v.x * line1->v.y - line0->v.y * line1->v.x);
	float t1 = (line0->v.y * (line0->p0.x - line1->p0.x) - line0->v.x * (line0->p0.y - line1->p0.y)) / (line1->v.x * line0->v.y - line1->v.y * line0->v.x);

	if (t0 < 0 || t0 > 1 || t1 < 0 || t1 > 1)
		return false;

	if (intersetPoint != nullptr)
	{
		intersetPoint->x = line0->p0.x + line0->v.x * t0;
		intersetPoint->y = line0->p0.y + line0->v.y * t0;
	}
	return true;
}

#pragma endregion

#pragma region ParameterizedLine3D

ParameterizedLine3D::ParameterizedLine3D()
{
}

ParameterizedLine3D::ParameterizedLine3D(Vector3 p0, Vector3 p1)
{
	this->p0 = p0;
	this->p1 = p1;
	this->v = Vector3(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z);
}

Vector3 ParameterizedLine3D::GetPoint(float t)
{
	return Vector3(p0.x + v.x * t, p0.y + v.y * t, p0.z + v.z * t);
}

#pragma endregion

#pragma region Plane3D

Plane3D::Plane3D()
{
}

Plane3D::Plane3D(Vector3 normal, Vector3 p0, bool isNormalize)
{
	this->p0.x = p0.x;
	this->p0.y = p0.y;
	this->p0.z = p0.z;
	if (isNormalize)
	{
		float mag = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z + normal.z);
		this->normal.x = normal.x / mag;
		this->normal.y = normal.y / mag;
		this->normal.z = normal.z / mag;
	}
	else
	{
		this->normal.x = normal.x;
		this->normal.y = normal.y;
		this->normal.z = normal.z;
	}
}

float Plane3D::PointWithPlane(Vector3* p)
{
	Vector3 v(p->x - this->p0.x, p->y - this->p0.y, p->z - this->p0.z);
	return normal.x * v.x + normal.y * v.y + normal.z * v.z;
}

bool Plane3D::IntersetLine(ParameterizedLine3D* line, Vector3* intersetPoint)
{
	float dot = (line->v.x * normal.x + line->v.y * normal.y + line->v.z * normal.z);
	if (dot == 0)
		return false;

	float t = -(normal.x * line->p0.x + normal.y * line->p0.y + normal.z * line->p0.z -
		normal.x * p0.x - normal.y * p0.y - normal.z * p0.z) / dot;

	if (t < 0 || t > 1)
		return false;

	if (intersetPoint != nullptr)
	{
		intersetPoint->x = line->p0.x + line->v.x * t;
		intersetPoint->y = line->p0.y + line->v.y * t;
		intersetPoint->z = line->p0.z + line->v.z * t;
	}
	return true;
}

#pragma endregion

#pragma region Math

CMath::CMath() { }

CMath::~CMath() { }

float CMath::DegToRad(float angle)
{
	return PI / 180 * angle;
}

float CMath::RadToDeg(float radian)
{
	return 180 / PI * radian;
}

float CMath::Random()
{
	static float max_rec = 1 / (float)RAND_MAX;
	return rand() * max_rec;
}

float CMath::Random(float max)
{
	return Random() * (max);
}

float CMath::Random(float min, float max)
{
	return Random() * (max - min) + min;
}

int CMath::Random(int max)
{
	return (int)(Random((float)max) + 0.5f);
}

int CMath::Random(int min, int max)
{
	return (int)(Random((float)min, (float)max) + 0.5f);
}

void CMath::EualrAnglesToUVN(const Vector3* eualrAngles, Vector3* u, Vector3* v, Vector3* n)
{
	float phi = eualrAngles->y;
	float theta = eualrAngles->z;

	float sin_phi = sin(CMath::DegToRad(phi));
	float cos_phi = cos(CMath::DegToRad(phi));
	float sin_theta = sin(CMath::DegToRad(theta));
	float cos_theta = cos(CMath::DegToRad(theta));

	float r = sin_phi;

	n->x = cos_theta * r;
	n->y = sin_theta * r;
	n->z = cos_phi;

	v->x = 0;
	v->y = 1;
	v->z = 0;

	*n = (*n).Normalization();
	*u = Vector3::Cross(*v, *n);
	*v = Vector3::Cross(*n, *u);
}

#pragma endregion