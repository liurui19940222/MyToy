#include "Math.h"
/*
#pragma region Vector3

Vector3::Vector3() :x(0), y(0), z(0) {}
Vector3::Vector3(const Vector2& v2) : x(v2.x), y(v2.y), z(0) {}
Vector3::Vector3(const Vector3& v3) : x(v3.x), y(v3.y), z(v3.z) {}
Vector3::Vector3(const Vector4& v4) : x(v4.x), y(v4.y), z(v4.z) {}
Vector3::Vector3(float px, float py, float pz) : x(px), y(py), z(pz) {}

float Vector3::Magnitude() const
{
	return sqrt(x * x + y*y + z*z);
}

float Vector3::MagnitudeSqrt() const
{
	return x * x + y*y + z*z;
}

Vector3 Vector3::Normalization() const
{
	float mag = Magnitude();
	return Vector3(x / mag, y / mag, z / mag);
}

Vector3 Vector3::operator+(const Vector3& vec) const
{
	return Vector3(x + vec.x, y + vec.y, z + vec.z);
}

void Vector3::operator+=(const Vector3& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
}

void Vector3::operator-=(const Vector3& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
}

void Vector3::operator*=(const float value)
{
	x *= value;
	y *= value;
	z *= value;
}

void Vector3::operator/=(const float value)
{
	*(this) *= (1.0f / value);
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
	float px = x * mat[0][0] + y * mat[1][0] + z * mat[2][0];
	float py = x * mat[0][1] + y * mat[1][1] + z * mat[2][1];
	float pz = x * mat[0][2] + y * mat[1][2] + z * mat[2][2];
	return Vector3(px, py, pz);
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

float Vector3::Dot(const Vector3 &vec1, const Vector3 &vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

Vector3 Vector3::Cross(const Vector3 &u, const Vector3 &v)
{
	return Vector3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

float Vector3::Angle(const Vector3 &vec1, const Vector3 &vec2)
{
	return  acos(Dot(vec1.Normalization(), vec2.Normalization()));
}

Vector3 Vector3::Projection(const Vector3 &u, const Vector3 &v)
{
	float mag = v.Magnitude();
	return v * Dot(u, v) / (mag * mag);
}

const Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::one(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::up(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::down(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::left(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::right(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::forward(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::back(0.0f, 0.0f, -1.0f);

#pragma endregion
*/
#pragma region Matrix4x4

Matrix4x4::Matrix4x4()
{
	Zero(*this);
}

Matrix4x4::Matrix4x4(float* array)
{
	if (array)
	{
		m[0].x = array[0]; m[0].y = array[4]; m[0].z = array[8]; m[0].w = array[12];
		m[1].x = array[1]; m[1].y = array[5]; m[1].z = array[9]; m[1].w = array[13];
		m[2].x = array[2]; m[2].y = array[6]; m[2].z = array[10]; m[2].w = array[14];
		m[3].x = array[3]; m[3].y = array[7]; m[3].z = array[11]; m[3].w = array[15];
	}
}

Matrix4x4::Matrix4x4(float oblique)
{
	Zero(*this);
	m[0][0] = oblique;
	m[1][1] = oblique;
	m[2][2] = oblique;
	m[3][3] = oblique;
}

Matrix4x4::Matrix4x4(float x0, float x1, float x2, float x3, float y0, float y1, float y2, float y3, float z0, float z1, float z2, float z3, float w0, float w1, float w2, float w3)
{
	m[0].x = x0; m[0].y = y0; m[0].z = z0; m[0].w = w0;
	m[1].x = x1; m[1].y = y1; m[1].z = z1; m[1].w = w1;
	m[2].x = x2; m[2].y = y2; m[2].z = z2; m[2].w = w2;
	m[3].x = x3; m[3].y = y3; m[3].z = z3; m[3].w = w3;
}

void Matrix4x4::Set(int x, int y, float value)
{
	m[x][y] = value;
}

float Matrix4x4::Get(int x, int y)
{
	return m[x][y];
}

Vector3 Matrix4x4::Multiply(const Vector3& v3)
{
	float x = 0, y = 0, z = 0;
	x = m[0][0] * v3.x + m[1][0] * v3.y + m[2][0] * v3.z;
	y = m[0][1] * v3.x + m[1][1] * v3.y + m[2][1] * v3.z;
	z = m[0][2] * v3.x + m[1][2] * v3.y + m[2][2] * v3.z;
	return Vector3(x, y, z);
}

Vector4 Matrix4x4::Multiply(const Vector4& v)
{
	return Vector4(
		m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w,
		m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w,
		m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w,
		m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w
	);
}

Matrix4x4 Matrix4x4::Multiply(Matrix4x4& mat)
{
	Matrix4x4 temp;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			temp[j][i] = mat[j][0] * m[0][i] + mat[j][1] * m[1][i] + mat[j][2] * m[2][i] + mat[j][3] * m[3][i];

	return temp;
}

Matrix4x4 Matrix4x4::Multiply(float value)
{
	return Matrix4x4(m[0][0] * value, m[1][0] * value, m[2][0] * value, m[3][0] * value,
		m[0][1] * value, m[1][1] * value, m[2][1] * value, m[3][1] * value,
		m[0][2] * value, m[1][2] * value, m[2][2] * value, m[3][2] * value,
		m[0][3] * value, m[1][3] * value, m[2][3] * value, m[3][3] * value);
}

Matrix4x4 Matrix4x4::operator+(Matrix4x4& matrix)
{
	return Matrix4x4(m[0][0] + matrix[0][0], m[1][0] + matrix[1][0], m[2][0] + matrix[2][0], m[3][0] + matrix[3][0],
		m[0][1] + matrix[0][1], m[1][1] + matrix[1][1], m[2][1] + matrix[2][1], m[3][1] + matrix[3][1],
		m[0][2] + matrix[0][2], m[1][2] + matrix[1][2], m[2][2] + matrix[2][2], m[3][2] + matrix[3][2],
		m[0][3] + matrix[0][3], m[1][3] + matrix[1][3], m[2][3] + matrix[2][3], m[3][3] + matrix[3][3]);
}

Matrix4x4 Matrix4x4::operator-(Matrix4x4& matrix)
{
	return Matrix4x4(m[0][0] - matrix[0][0], m[1][0] - matrix[1][0], m[2][0] - matrix[2][0], m[3][0] - matrix[3][0],
		m[0][1] - matrix[0][1], m[1][1] - matrix[1][1], m[2][1] - matrix[2][1], m[3][1] - matrix[3][1],
		m[0][2] - matrix[0][2], m[1][2] - matrix[1][2], m[2][2] - matrix[2][2], m[3][2] - matrix[3][2],
		m[0][3] - matrix[0][3], m[1][3] - matrix[1][3], m[2][3] - matrix[2][3], m[3][3] - matrix[3][3]);
}

void Matrix4x4::operator+=(Matrix4x4& matrix)
{
	m[0][0] += matrix[0][0]; m[1][0] += matrix[1][0]; m[2][0] += matrix[2][0]; m[3][0] += matrix[3][0];
	m[0][1] += matrix[0][1]; m[1][1] += matrix[1][1]; m[2][1] += matrix[2][1]; m[3][1] += matrix[3][1];
	m[0][2] += matrix[0][2]; m[1][2] += matrix[1][2]; m[2][2] += matrix[2][2]; m[3][2] += matrix[3][2];
	m[0][3] += matrix[0][3]; m[1][3] += matrix[1][3]; m[2][3] += matrix[2][3]; m[3][3] += matrix[3][3];
}

void Matrix4x4::operator-=(Matrix4x4& matrix)
{
	m[0][0] -= matrix[0][0]; m[1][0] -= matrix[1][0]; m[2][0] -= matrix[2][0]; m[3][0] -= matrix[3][0];
	m[0][1] -= matrix[0][1]; m[1][1] -= matrix[1][1]; m[2][1] -= matrix[2][1]; m[3][1] -= matrix[3][1];
	m[0][2] -= matrix[0][2]; m[1][2] -= matrix[1][2]; m[2][2] -= matrix[2][2]; m[3][2] -= matrix[3][2];
	m[0][3] -= matrix[0][3]; m[1][3] -= matrix[1][3]; m[2][3] -= matrix[2][3]; m[3][3] -= matrix[3][3];
}

Matrix4x4 Matrix4x4::operator*(Matrix4x4& matrix)
{
	return Multiply(matrix);
}

Vector3 Matrix4x4::operator*(const Vector3& v)
{
	return Multiply(v);
}

Vector4 Matrix4x4::operator*(const Vector4& v)
{
	return Multiply(v);
}

Matrix4x4 Matrix4x4::operator*(float value)
{
	return Multiply(value);
}

Matrix4x4 Matrix4x4::operator/(float value)
{
	return Multiply(1.0f / value);
}

Vector4& Matrix4x4::operator[](size_t index)
{
	return m[index];
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
	Matrix4x4 m;
	Rotate(m, pitch, yaw, roll);
	return m;
}

Matrix4x4 Matrix4x4::Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4x4 m;
	Ortho(m, left, right, bottom, top, zNear, zFar);
	return m;
}

Matrix4x4 Matrix4x4::Perspective(float fov, float aspect, float near, float far)
{
	Matrix4x4 m;
	Perspective(m, fov, aspect, near, far);
	return m;
}

Matrix4x4 Matrix4x4::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	Matrix4x4 m;
	LookAt(m, eye, center, up);
	return m;
}

Matrix4x4 Matrix4x4::Lerp(Matrix4x4& a, Matrix4x4& b, float t)
{
	Matrix4x4 m;
	Lerp(m, a, b, t);
	return m;
}

Matrix4x4 Matrix4x4::RotateUVN(const Vector3& targetPos, const Vector3& selfPos)
{
	Matrix4x4 m;
	RotateUVN(m, targetPos, selfPos);
	return m;
}

Matrix4x4 Matrix4x4::Translate(const Vector3& translate)
{
	Matrix4x4 m;
	Translate(m, translate);
	return m;
}

Matrix4x4 Matrix4x4::Scale(const Vector3& scale)
{
	Matrix4x4 m;
	Scale(m, scale);
	return m;
}

Matrix4x4 Matrix4x4::Transpose()
{
	return Matrix4x4(m[0][0], m[0][1], m[0][2], m[0][3],
		m[1][0], m[1][1], m[1][2], m[1][3],
		m[2][0], m[2][1], m[2][2], m[2][3],
		m[3][0], m[3][1], m[3][2], m[3][3]);
}

Matrix4x4 Matrix4x4::Inverse()
{
	float SubFactor00 = this->m[2][2] * this->m[3][3] - this->m[3][2] * this->m[2][3];
	float SubFactor01 = this->m[2][1] * this->m[3][3] - this->m[3][1] * this->m[2][3];
	float SubFactor02 = this->m[2][1] * this->m[3][2] - this->m[3][1] * this->m[2][2];
	float SubFactor03 = this->m[2][0] * this->m[3][3] - this->m[3][0] * this->m[2][3];
	float SubFactor04 = this->m[2][0] * this->m[3][2] - this->m[3][0] * this->m[2][2];
	float SubFactor05 = this->m[2][0] * this->m[3][1] - this->m[3][0] * this->m[2][1];
	float SubFactor06 = this->m[1][2] * this->m[3][3] - this->m[3][2] * this->m[1][3];
	float SubFactor07 = this->m[1][1] * this->m[3][3] - this->m[3][1] * this->m[1][3];
	float SubFactor08 = this->m[1][1] * this->m[3][2] - this->m[3][1] * this->m[1][2];
	float SubFactor09 = this->m[1][0] * this->m[3][3] - this->m[3][0] * this->m[1][3];
	float SubFactor10 = this->m[1][0] * this->m[3][2] - this->m[3][0] * this->m[1][2];
	float SubFactor11 = this->m[1][1] * this->m[3][3] - this->m[3][1] * this->m[1][3];
	float SubFactor12 = this->m[1][0] * this->m[3][1] - this->m[3][0] * this->m[1][1];
	float SubFactor13 = this->m[1][2] * this->m[2][3] - this->m[2][2] * this->m[1][3];
	float SubFactor14 = this->m[1][1] * this->m[2][3] - this->m[2][1] * this->m[1][3];
	float SubFactor15 = this->m[1][1] * this->m[2][2] - this->m[2][1] * this->m[1][2];
	float SubFactor16 = this->m[1][0] * this->m[2][3] - this->m[2][0] * this->m[1][3];
	float SubFactor17 = this->m[1][0] * this->m[2][2] - this->m[2][0] * this->m[1][2];
	float SubFactor18 = this->m[1][0] * this->m[2][1] - this->m[2][0] * this->m[1][1];

	Matrix4x4 Inverse(
		+this->m[1][1] * SubFactor00 - this->m[1][2] * SubFactor01 + this->m[1][3] * SubFactor02,
		-this->m[1][0] * SubFactor00 + this->m[1][2] * SubFactor03 - this->m[1][3] * SubFactor04,
		+this->m[1][0] * SubFactor01 - this->m[1][1] * SubFactor03 + this->m[1][3] * SubFactor05,
		-this->m[1][0] * SubFactor02 + this->m[1][1] * SubFactor04 - this->m[1][2] * SubFactor05,

		-this->m[0][1] * SubFactor00 + this->m[0][2] * SubFactor01 - this->m[0][3] * SubFactor02,
		+this->m[0][0] * SubFactor00 - this->m[0][2] * SubFactor03 + this->m[0][3] * SubFactor04,
		-this->m[0][0] * SubFactor01 + this->m[0][1] * SubFactor03 - this->m[0][3] * SubFactor05,
		+this->m[0][0] * SubFactor02 - this->m[0][1] * SubFactor04 + this->m[0][2] * SubFactor05,

		+this->m[0][1] * SubFactor06 - this->m[0][2] * SubFactor07 + this->m[0][3] * SubFactor08,
		-this->m[0][0] * SubFactor06 + this->m[0][2] * SubFactor09 - this->m[0][3] * SubFactor10,
		+this->m[0][0] * SubFactor11 - this->m[0][1] * SubFactor09 + this->m[0][3] * SubFactor12,
		-this->m[0][0] * SubFactor08 + this->m[0][1] * SubFactor10 - this->m[0][2] * SubFactor12,

		-this->m[0][1] * SubFactor13 + this->m[0][2] * SubFactor14 - this->m[0][3] * SubFactor15,
		+this->m[0][0] * SubFactor13 - this->m[0][2] * SubFactor16 + this->m[0][3] * SubFactor17,
		-this->m[0][0] * SubFactor14 + this->m[0][1] * SubFactor16 - this->m[0][3] * SubFactor18,
		+this->m[0][0] * SubFactor15 - this->m[0][1] * SubFactor17 + this->m[0][2] * SubFactor18);

	float Determinant =
		+this->m[0][0] * Inverse[0][0]
		+ this->m[0][1] * Inverse[1][0]
		+ this->m[0][2] * Inverse[2][0]
		+ this->m[0][3] * Inverse[3][0];

	Inverse = Inverse / Determinant;
	return Inverse;
}

Vector3 Matrix4x4::EulerAngles()
{
	Vector3 euler;

	float h = 0, p = 0, b = 0;
	float sp = -m[1][2];
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
		h = atan2(-m[2][0], m[0][0]);
	}
	else
	{
		h = atan2(m[0][2], m[2][2]);
		b = atan2(m[2][0], m[1][1]);
	}

	euler.y = CMath::RadToDeg * h;
	euler.x = CMath::RadToDeg * p;
	euler.z = CMath::RadToDeg * b;
	if (euler.x < 0) euler.x = 360 + euler.x;
	if (euler.y < 0) euler.y = 360 + euler.y;
	if (euler.z < 0) euler.z = 360 + euler.z;
	return euler;
}

void Matrix4x4::MakeIdentity()
{
	Identity(*this);
}

void Matrix4x4::MakeZero()
{
	Zero(*this);
}

void Matrix4x4::MakeRotate(float pitch, float yaw, float roll)
{
	Rotate(*this, pitch, yaw, roll);
}

void Matrix4x4::MakeRotateUVN(const Vector3& targetPos, const Vector3& selfPos)
{
	RotateUVN(*this, targetPos, selfPos);
}

void Matrix4x4::MakeTranslate(const Vector3& translate)
{
	Translate(*this, translate);
}

void Matrix4x4::MakeScale(const Vector3& scale)
{
	Scale(*this, scale);
}

void Matrix4x4::MakeOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Ortho(*this, left, right, bottom, top, zNear, zFar);
}

void Matrix4x4::MakePerspective(float fov, float aspect, float near, float far)
{
	Perspective(*this, fov, aspect, near, far);
}

void Matrix4x4::MakeLookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	LookAt(*this, eye, center, up);
}

void Matrix4x4::Zero(Matrix4x4& mat)
{
	memset(&mat, 0, sizeof(float) * 16);
}

void Matrix4x4::Identity(Matrix4x4& mat)
{
	mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = 0; mat[0][3] = 0;
	mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0; mat[1][3] = 0;
	mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1; mat[2][3] = 0;
	mat[3][0] = 0; mat[3][1] = 0; mat[3][2] = 0; mat[3][3] = 1;
}

void Matrix4x4::Rotate(Matrix4x4& mat, float pitch, float yaw, float roll)
{
	static Matrix4x4 x, y, z;
	float sin_x = sin(pitch * CMath::DegToRad);
	float sin_y = sin(yaw * CMath::DegToRad);
	float sin_z = sin(roll * CMath::DegToRad);
	float cos_x = cos(pitch * CMath::DegToRad);
	float cos_y = cos(yaw * CMath::DegToRad);
	float cos_z = cos(roll * CMath::DegToRad);
	x[0][0] = 1; x[0][1] = 0;		x[0][2] = 0;		x[0][3] = 0;
	x[1][0] = 0; x[1][1] = cos_x;	x[1][2] = sin_x;	x[1][3] = 0;
	x[2][0] = 0; x[2][1] = -sin_x;	x[2][2] = cos_x;	x[2][3] = 0;
	x[3][0] = 0; x[3][1] = 0;		x[3][2] = 0;		x[3][3] = 1;

	y[0][0] = cos_y;	y[0][1] = 0; y[0][2] = -sin_y;	y[0][3] = 0;
	y[1][0] = 0;		y[1][1] = 1; y[1][2] = 0;		y[1][3] = 0;
	y[2][0] = sin_y;	y[2][1] = 0; y[2][2] = cos_y;	y[2][3] = 0;
	y[3][0] = 0;		y[3][1] = 0; y[3][2] = 0;		y[3][3] = 1;

	z[0][0] = cos_z;	z[0][1] = sin_z;	z[0][2] = 0; z[0][3] = 0;
	z[1][0] = -sin_z;	z[1][1] = cos_z;	z[1][2] = 0; z[1][3] = 0;
	z[2][0] = 0;		z[2][1] = 0;		z[2][2] = 1; z[2][3] = 0;
	z[3][0] = 0;		z[3][1] = 0;		z[3][2] = 0; z[3][3] = 1;
	mat = z * x * y;
}

void Matrix4x4::RotateUVN(Matrix4x4& mat, const Vector3& targetPos, const Vector3& selfPos)
{
	Vector3 n = (targetPos - selfPos).Normalization();
	Vector3 u = Vector3::Cross(Vector3(0, 1, 0), n).Normalization();
	Vector3 v = Vector3::Cross(n, u);
	mat.MakeIdentity();
	mat[0][0] = u.x; mat[0][1] = u.y; mat[0][2] = u.z;
	mat[1][0] = v.x; mat[1][1] = v.y; mat[1][2] = v.z;
	mat[2][0] = n.x; mat[2][1] = n.y; mat[2][2] = n.z;
}

void Matrix4x4::Translate(Matrix4x4& mat, const Vector3& translate)
{
	mat.MakeIdentity();
	mat[3][0] = translate.x;
	mat[3][1] = translate.y;
	mat[3][2] = translate.z;
}

void Matrix4x4::Scale(Matrix4x4& mat, const Vector3& scale)
{
	mat.MakeIdentity();
	mat[0][0] = scale.x;
	mat[1][1] = scale.y;
	mat[2][2] = scale.z;
}

void Matrix4x4::Ortho(Matrix4x4& mat, float left, float right, float bottom, float top, float zNear, float zFar)
{
	mat.MakeIdentity();
	mat[0][0] = 2 / (right - left);
	mat[1][1] = 2 / (top - bottom);
	mat[2][2] = -2 / (zFar - zNear);
	mat[3][0] = -(right + left) / (right - left);
	mat[3][1] = -(top + bottom) / (top - bottom);
	mat[3][2] = -(zFar + zNear) / (zFar - zNear);
}

void Matrix4x4::Perspective(Matrix4x4& mat, float fov, float aspect, float near, float far)
{
	mat.MakeZero();
	float range = tan(CMath::DegToRad * (fov / 2.0f)) * near;
	float left = -range * aspect;
	float right = range * aspect;
	float bottom = -range;
	float top = range;

	mat[0][0] = (2.0f * near) / (right - left);
	mat[1][1] = (2.0f * near) / (top - bottom);
	mat[2][2] = -(far + near) / (far - near);
	mat[2][3] = -1.0f;
	mat[3][2] = -(2.0f * far * near) / (far - near);
}

void Matrix4x4::LookAt(Matrix4x4& mat, const Vector3& eye, const Vector3& center, const Vector3& up)
{
	Vector3 f = (center - eye).Normalization();
	Vector3 u = up.Normalization();
	Vector3 s = Vector3::Cross(f, u).Normalization();
	u = Vector3::Cross(s, f);

	mat.MakeIdentity();
	mat[0][0] = s.x; mat[1][0] = s.y; mat[2][0] = s.z; mat[3][0] = -Vector3::Dot(s, eye);
	mat[0][1] = u.x; mat[1][1] = u.y; mat[2][1] = u.z; mat[3][1] = -Vector3::Dot(u, eye);
	mat[0][2] = -f.x; mat[1][2] = -f.y; mat[2][2] = -f.z; mat[3][2] = Vector3::Dot(f, eye);
}

void Matrix4x4::GetUVN(Matrix4x4& mat, Vector3* u, Vector3* v, Vector3* n)
{
	u->x = mat[0][0]; u->y = mat[0][1]; u->z = mat[0][2];
	v->x = mat[1][0]; v->y = mat[1][1]; v->z = mat[1][2];
	n->x = mat[2][0]; n->y = mat[2][1]; n->z = mat[2][2];
}

void Matrix4x4::GetUVN(Matrix4x4& mat, const Vector3& scale, Vector3* u, Vector3* v, Vector3* n)
{
	float rx = 1.0f / scale.x, ry = 1.0f / scale.y, rz = 1.0f / scale.z;
	u->x = mat[0][0] * rx; u->y = mat[0][1] * rx; u->z = mat[0][2] * rx;
	v->x = mat[1][0] * ry; v->y = mat[1][1] * ry; v->z = mat[1][2] * ry;
	n->x = mat[2][0] * rz; n->y = mat[2][1] * rz; n->z = mat[2][2] * rz;
}

void Matrix4x4::GetPosition(Matrix4x4& mat, Vector3* position)
{
	position->x = mat[3][0];
	position->y = mat[3][1];
	position->z = mat[3][2];
}

void Matrix4x4::Lerp(Matrix4x4& mat, Matrix4x4& a, Matrix4x4& b, float t)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mat[i][j] = CMath::Lerp(a[i][j], b[i][j], t);
		}
	}
}

#pragma endregion

#pragma region ParameterizedLine2D

ParameterizedLine2D::ParameterizedLine2D()
{
}

ParameterizedLine2D::ParameterizedLine2D(const Vector2& p0, const Vector2& p1)
{
	this->p0 = p0;
	this->p1 = p1;
	this->v = Vector2(p1.x - p0.x, p1.y - p0.y);
}

Vector2 ParameterizedLine2D::GetPoint(float t)
{
	return Vector2(p0.x + v.x * t, p0.y + v.y * t);
}

bool ParameterizedLine2D::Interset(const ParameterizedLine2D& line0, const ParameterizedLine2D& line1, Vector2* intersetPoint)
{
	if (line1.v.x * line0.v.y == line1.v.y * line0.v.x)
		return false;

	float t0 = (line1.v.y * (line1.p0.x - line0.p0.x) - line1.v.x * (line1.p0.y - line0.p0.y)) / (line0.v.x * line1.v.y - line0.v.y * line1.v.x);
	float t1 = (line0.v.y * (line0.p0.x - line1.p0.x) - line0.v.x * (line0.p0.y - line1.p0.y)) / (line1.v.x * line0.v.y - line1.v.y * line0.v.x);

	if (t0 < 0 || t0 > 1 || t1 < 0 || t1 > 1)
		return false;

	if (intersetPoint != nullptr)
	{
		intersetPoint->x = line0.p0.x + line0.v.x * t0;
		intersetPoint->y = line0.p0.y + line0.v.y * t0;
	}
	return true;
}

#pragma endregion

#pragma region ParameterizedLine3D

ParameterizedLine3D::ParameterizedLine3D()
{
}

ParameterizedLine3D::ParameterizedLine3D(const Vector3& p0, const Vector3& p1)
{
	this->p0 = p0;
	this->p1 = p1;
	this->v = Vector3(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z);
}

Vector3 ParameterizedLine3D::GetPoint(float t) const
{
	return Vector3(p0.x + v.x * t, p0.y + v.y * t, p0.z + v.z * t);
}

#pragma endregion

#pragma region Plane3D

Plane3D::Plane3D()
{
}

Plane3D::Plane3D(const Vector3& normal, const Vector3& p0, bool isNormalize)
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

float Plane3D::PointWithPlane(const Vector3& p) const
{
	Vector3 v(p.x - this->p0.x, p.y - this->p0.y, p.z - this->p0.z);
	return normal.x * v.x + normal.y * v.y + normal.z * v.z;
}

bool Plane3D::IntersetLine(const ParameterizedLine3D& line, Vector3* intersetPoint) const
{
	float dot = (line.v.x * normal.x + line.v.y * normal.y + line.v.z * normal.z);
	if (dot == 0)
		return false;

	float t = -(normal.x * line.p0.x + normal.y * line.p0.y + normal.z * line.p0.z -
		normal.x * p0.x - normal.y * p0.y - normal.z * p0.z) / dot;

	if (t < 0 || t > 1)
		return false;

	if (intersetPoint != nullptr)
	{
		intersetPoint->x = line.p0.x + line.v.x * t;
		intersetPoint->y = line.p0.y + line.v.y * t;
		intersetPoint->z = line.p0.z + line.v.z * t;
	}
	return true;
}

#pragma endregion

#pragma region Math

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

#pragma endregion