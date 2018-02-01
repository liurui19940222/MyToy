#include "Math.h"
#include<assert.h>
#include<Windows.h>

USING_NAMESPACE_ENGINE

const Vector2 Vector2::zero(0.0f, 0.0f);
const Vector2 Vector2::one(1.0f, 1.0f);
const Vector2 Vector2::up(0.0f, 1.0f);
const Vector2 Vector2::down(0.0f, -1.0f);
const Vector2 Vector2::left(-1.0f, 0.0f);
const Vector2 Vector2::right(1.0f, 0.0f);

const Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::one(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::up(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::down(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::left(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::right(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::forward(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::back(0.0f, 0.0f, -1.0f);

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

Matrix4x4 Matrix4x4::Rotate(const Vector3& eulerAngles)
{
	Matrix4x4 m;
	Rotate(m, eulerAngles.x, eulerAngles.y, eulerAngles.z);
	return m;
}

Matrix4x4 Matrix4x4::Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4x4 m;
	Ortho(m, left, right, bottom, top, zNear, zFar);
	return m;
}

Matrix4x4 Matrix4x4::Perspective(float fov, float aspect, float _near, float _far)
{
	Matrix4x4 m;
	Perspective(m, fov, aspect, _near, _far);
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

Quaternion Matrix4x4::ToQuaternion(Matrix4x4& R)
{
	Quaternion q;
	float trace = R[0][0] + R[1][1] + R[2][2];
	if (trace > 0.0f)
	{
		float s = sqrt(trace + 1.0f);
		q[3] = s * 0.5f;
		float t = 0.5f / s;
		q[0] = (R[1][2] - R[2][1]) * t;
		q[1] = (R[2][0] - R[0][2]) * t;
		q[2] = (R[0][1] - R[1][0]) * t;
	}
	else
	{
		int i = 0;
		if (R[1][1] > R[0][0]) i = 1;
		if (R[2][2] > R[i][i]) i = 2;
		static const int next[3] = { 1, 2, 0 };
		int j = next[i];
		int k = next[j];
		float s = sqrt((R[i][i] - (R[j][j] + R[k][k])) + 1.0f);
		q[i] = s * 0.5f;
		float t;
		if (s != 0.0f) t = 0.5f / s;
		else t = s;
		q[3] = (R[j][k] - R[k][j]) * t;
		q[j] = (R[i][j] + R[j][i]) * t;
		q[k] = (R[i][k] + R[k][i]) * t;
	}
	return q;
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

void Matrix4x4::MakeRotate(const Vector3& eulerAngles)
{
	Rotate(*this, eulerAngles.x, eulerAngles.y, eulerAngles.z);
}

void Matrix4x4::MakeRotate(const Quaternion& q)
{
	Rotate(*this, q);
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

void Matrix4x4::MakePerspective(float fov, float aspect, float _near, float _far)
{
	Perspective(*this, fov, aspect, _near, _far);
}

void Matrix4x4::MakeLookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	LookAt(*this, eye, center, up);
}

Quaternion Matrix4x4::ToQuaternion()
{
	return ToQuaternion(*this);
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
	mat = y * x * z;
}

void Matrix4x4::Rotate(Matrix4x4& mat, const Quaternion& q)
{
	mat.MakeIdentity();
	mat[0][0] = 1 - 2 * q.y * q.y - 2 * q.z * q.z;	mat[0][1] = 2 * q.x * q.y + 2 * q.z * q.w;		mat[0][2] = 2 * q.x * q.z - 2 * q.y * q.w;
	mat[1][0] = 2 * q.x * q.y - 2 * q.z * q.w;		mat[1][1] = 1 - 2 * q.x * q.x - 2 * q.z * q.z;	mat[1][2] = 2 * q.y * q.z + 2 * q.x * q.w;
	mat[2][0] = 2 * q.x * q.z + 2 * q.y * q.w;		mat[2][1] = 2 * q.y * q.z - 2 * q.x * q.w;		mat[2][2] = 1 - 2 * q.x * q.x - 2 * q.y * q.y;
}

void Matrix4x4::RotateUVN(Matrix4x4& mat, const Vector3& targetPos, const Vector3& selfPos)
{
	Vector3 n = (targetPos - selfPos).Normalize();
	Vector3 u = Vector3::Cross(Vector3(0, 1, 0), n).Normalize();
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

void Matrix4x4::Perspective(Matrix4x4& mat, float fov, float aspect, float _near, float _far)
{
	mat.MakeZero();
	float range = tan(CMath::DegToRad * (fov / 2.0f)) * _near;
	float left = -range * aspect;
	float right = range * aspect;
	float bottom = -range;
	float top = range;

	mat[0][0] = (2.0f * _near) / (right - left);
	mat[1][1] = (2.0f * _near) / (top - bottom);
	mat[2][2] = -(_far + _near) / (_far - _near);
	mat[2][3] = -1.0f;
	mat[3][2] = -(2.0f * _far * _near) / (_far - _near);
}

void Matrix4x4::LookAt(Matrix4x4& mat, const Vector3& eye, const Vector3& center, const Vector3& up)
{
	Vector3 f = (center - eye).Normalize();
	Vector3 u = up.Normalize();
	Vector3 s = Vector3::Cross(f, u).Normalize();
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

const float CMath::EPSILON = FLT_EPSILON;
const float CMath::PI = 3.141592653f;
const float CMath::TWO_PI = PI * 2.0f;
const float CMath::HALF_PI = PI * 0.5f;
const float CMath::QUARTER_PI = PI * 0.25f;
const float CMath::DegToRad = PI / 180.0f;
const float CMath::RadToDeg = 180.0f / PI;
const float CMath::HalfDegToRad = 0.5f * DegToRad;

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

#pragma region Quaternion

const float Quaternion::_next[3] = { 1, 2, 0 };
const float Quaternion::negativeFlip = -0.0001f;
const float Quaternion::positiveFlip = CMath::TWO_PI - 0.0001f;

Quaternion::Quaternion() : x(0), y(0), z(0), w(0) {}
Quaternion::Quaternion(const Vector3& vec) : x(vec.x), y(vec.y), z(vec.z), w(0) {}
Quaternion::Quaternion(float px, float py, float pz, float pw) : x(px), y(py), z(pz), w(pw) {}
float& Quaternion::operator[](int index) { return m[index]; }
bool Quaternion::operator==(const Quaternion& q) const { return x == q.x && y == q.y && z == q.z && w == q.w; }
bool Quaternion::operator!=(const Quaternion& q) const { return x != q.x || y != q.y || z != q.z || w != q.w; }
Vector3 Quaternion::operator*(const Vector3& vec) const { return Multiply(vec); }
Quaternion Quaternion::operator*(const Quaternion& vec) const { return Multiply(vec); }
Quaternion Quaternion::operator-() { return Quaternion(-x, -y, -z, w); }

void Quaternion::MakeEuler(const Vector3& euler)
{
	float x = euler.x * CMath::HalfDegToRad;
	float y = euler.y * CMath::HalfDegToRad;
	float z = euler.z * CMath::HalfDegToRad;

	float sinX = sin(x);
	float cosX = cos(x);
	float sinY = sin(y);
	float cosY = cos(y);
	float sinZ = sin(z);
	float cosZ = cos(z);

	this->w = cosY * cosX * cosZ + sinY * sinX * sinZ;
	this->x = cosY * sinX * cosZ + sinY * cosX * sinZ;
	this->y = sinY * cosX * cosZ - cosY * sinX * sinZ;
	this->z = cosY * cosX * sinZ - sinY * sinX * cosZ;
}

void Quaternion::MakeNormalize()
{
	float n = this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w;

	if (n != 1 && n > 0)
	{
		n = 1 / sqrt(n);
		this->x = this->x * n;
		this->y = this->y * n;
		this->z = this->z * n;
		this->w = this->w * n;
	}
}

void Quaternion::MakeIdentity()
{
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}

Quaternion Quaternion::AngleAxis(const Vector3& axis, float angle)
{
	Vector3 normAxis = axis.Normalize();
	angle = angle * CMath::HalfDegToRad;
	float s = sin(angle);
	float w = cos(angle);
	float x = normAxis.x * s;
	float y = normAxis.y * s;
	float z = normAxis.z * s;
	return Quaternion(x, y, z, w);
}

Quaternion Quaternion::Euler(const Vector3& euler)
{
	Quaternion q;
	q.MakeEuler(euler);
	return q;
}

Quaternion Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	t = CMath::Clamp01(t);
	Quaternion q;
	if (Quaternion::Dot(q1, q2) < 0)
	{
		q.x = q1.x + t * (-q2.x - q1.x);
		q.y = q1.y + t * (-q2.y - q1.y);
		q.z = q1.z + t * (-q2.z - q1.z);
		q.w = q1.w + t * (-q2.w - q1.w);
	}
	else
	{
		q.x = q1.x + (q2.x - q1.x) * t;
		q.y = q1.y + (q2.y - q1.y) * t;
		q.z = q1.z + (q2.z - q1.z) * t;
		q.w = q1.w + (q2.w - q1.w) * t;
	}
	q.MakeNormalize();
	return q;
}

Quaternion Quaternion::UnclampedSlerp(const Quaternion& from, const Quaternion& to, float t)
{
	float cosAngle = Quaternion::Dot(from, to);
	Quaternion target = to;
	if (cosAngle < 0.0f)
	{
		cosAngle = -cosAngle;
		target = Quaternion(-to.x, -to.y, -to.z, -to.w);
	}

	float t1(0.0f), t2(0.0f);

	if (cosAngle < 0.95)
	{
		float angle = acos(cosAngle);
		float sinAngle = sin(angle);
		float invSinAngle = 1.0f / sinAngle;
		t1 = sin((1.0f - t) * angle) * invSinAngle;
		t2 = sin(t * angle) * invSinAngle;
		return Quaternion(from.x * t1 + target.x * t2, from.y * t1 + target.y * t2, from.z * t1 + target.z * t2, from.w * t1 + target.w * t2);
	}
	else
	{
		return Quaternion::Lerp(from, target, t);
	}
}

Quaternion Quaternion::Slerp(const Quaternion& from, const Quaternion& to, float t)
{
	return UnclampedSlerp(from, to, CMath::Clamp01(t));
}

Quaternion Quaternion::LookRotation(const Vector3& pForward, const Vector3& pUp)
{
	float mag = pForward.Magnitude();
	assert(mag >= 1e-6);

	Vector3 forward = pForward / mag;
	Vector3 right = Vector3::Cross(pUp, forward);
	right.MakeNormalize();
	Vector3 up = Vector3::Cross(forward, right);
	right = Vector3::Cross(up, forward);

	float t = right.x + up.y + forward.z;

	if (t > 0.0f)
	{
		float x, y, z, w;
		t = t + 1.0f;
		float s = 0.5f / sqrt(t);
		w = s * t;
		x = (up.z - forward.y) * s;
		y = (forward.x - right.z) * s;
		z = (right.y - up.x) * s;

		Quaternion ret = Quaternion(x, y, z, w);
		ret.MakeNormalize();
		return ret;
	}
	else
	{
		float rot[3][3] =
		{
			{ right.x, up.x, forward.x },
			{ right.y, up.y, forward.y },
			{ right.z, up.z, forward.z },
		};

		float q[3] = { 0, 0, 0 };
		int i = 0;

		if (up.y > right.x)
		{
			i = 1;
		}

		if (forward.z > rot[i][i])
		{
			i = 2;
		}

		int j = (int)_next[i];
		int k = (int)_next[j];

		float t = rot[i][i] - rot[j][j] - rot[k][k] + 1;
		float s = 0.5f / sqrt(t);
		q[i] = s * t;
		float w = (rot[k][j] - rot[j][k]) * s;
		q[j] = (rot[j][i] + rot[i][j]) * s;
		q[k] = (rot[k][i] + rot[i][k]) * s;

		Quaternion ret = Quaternion(q[0], q[1], q[2], w);
		ret.MakeNormalize();
		return ret;
	}
}

Quaternion Quaternion::Normalize() const
{
	Quaternion q = *this;
	q.MakeNormalize();
	return q;
}

float Quaternion::Angle(const Quaternion& a, const Quaternion& b)
{
	float dot = Quaternion::Dot(a, b);
	if (dot < 0.0f) dot = -dot;
	return CMath::Acos(CMath::Min(dot, 1.0f)) * 2.0f * 57.29578f;
}

float Quaternion::Dot(const Quaternion& a, const Quaternion& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Vector3 Quaternion::ToEulerAngles() const
{
	//from http://www.geometrictools.com/Documentation/EulerAngles.pdf
	//ZXYË³Ðò
	float x = this->x;
	float y = this->y;
	float z = this->z;
	float w = this->w;

	float check = 2 * (y * z - w * x);

	if (check < 0.999)
	{
		if (check > -0.999)
		{
			Vector3 v = Vector3(-asin(check),
			atan2(2 * (x * z + w * y), 1 - 2 * (x * x + y * y)),
			atan2(2 * (x * y + w * z), 1 - 2 * (x * x + z * z)));
			SanitizeEuler(v);
			return v * CMath::RadToDeg;
		}
		else
		{
			Vector3 v = Vector3(CMath::HALF_PI, atan2(2 * (x * y - w * z), 1 - 2 * (y * y + z * z)), 0);
			SanitizeEuler(v);
			return v * CMath::RadToDeg;
		}
	}
	else
	{
		Vector3 v = Vector3(-CMath::HALF_PI, atan2(-2 * (x * y - w * z), 1 - 2 * (y * y + z * z)), 0);
		SanitizeEuler(v);
		return v * CMath::RadToDeg;
	}
}

void Quaternion::SanitizeEuler(Vector3& euler)
{
	if (euler.x < negativeFlip)
	{
		euler.x = euler.x + CMath::TWO_PI;
	}
	else if (euler.x > positiveFlip)
	{
		euler.x = euler.x - CMath::TWO_PI;
	}
	if (euler.y < negativeFlip)
	{
		euler.y = euler.y + CMath::TWO_PI;
	}
	else if (euler.y > positiveFlip)
	{
		euler.y = euler.y - CMath::TWO_PI;
	}
	if (euler.z < negativeFlip)
	{
		euler.z = euler.z + CMath::TWO_PI;
	}
	else if (euler.z > positiveFlip)
	{
		euler.z = euler.z + CMath::TWO_PI;
	}
}

Vector3 Quaternion::Multiply(const Vector3& point) const
{
	Vector3 vec;

	float num = this->x * 2;
	float num2 = this->y * 2;
	float num3 = this->z * 2;
	float num4 = this->x * num;
	float num5 = this->y * num2;
	float num6 = this->z * num3;
	float num7 = this->x * num2;
	float num8 = this->x * num3;
	float num9 = this->y * num3;
	float num10 = this->w * num;
	float num11 = this->w * num2;
	float num12 = this->w * num3;

	vec.x = (((1 - (num5 + num6)) * point.x) + ((num7 - num12) * point.y)) + ((num8 + num11) * point.z);
	vec.y = (((num7 + num12) * point.x) + ((1 - (num4 + num6)) * point.y)) + ((num9 - num10) * point.z);
	vec.z = (((num8 - num11) * point.x) + ((num9 + num10) * point.y)) + ((1 - (num4 + num5)) * point.z);

	return vec;
}

Quaternion Quaternion::Multiply(const Quaternion& q) const
{
	return Quaternion((((this->w * q.x) + (this->x * q.w)) + (this->y * q.z)) - (this->z * q.y), 
		(((this->w * q.y) + (this->y * q.w)) + (this->z * q.x)) - (this->x * q.z), 
		(((this->w * q.z) + (this->z * q.w)) + (this->x * q.y)) - (this->y * q.x), 
		(((this->w * q.w) - (this->x * q.x)) - (this->y * q.y)) - (this->z * q.z));
}

void Quaternion::ToAngleAxis(const Quaternion& q, Vector3* outVec, float* outAngle) const
{
	float angle = 2.0f * CMath::Acos(this->w);

	if (CMath::Approximately(angle, 0.0f))
	{
		*outAngle = angle * 57.29578f;
		*outVec = Vector3(1.0f, 0.0f, 0.0f);
	}

	float div = 1.0f / sqrt(1.0f - sqrt(this->w));
	*outAngle = angle * 57.29578f;
	*outVec = Vector3(q.x * div, q.y * div, q.z * div);
}

const Quaternion Quaternion::identity(0.0f, 0.0f, 0.0f, 1.0f);

#pragma endregion