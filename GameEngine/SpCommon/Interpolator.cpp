#include"Interpolator.h"

USING_NAMESPACE_ENGINE;

float InterpolateHelper::Interpolate(float a, float b, float t)
{
	return CMath::Lerp(a, b, t);
}

Color InterpolateHelper::Interpolate(const Color& a, const Color& b, float t)
{
	return Color(CMath::Lerp(a.r, b.r, t), CMath::Lerp(a.g, b.g, t), CMath::Lerp(a.b, b.b, t), CMath::Lerp(a.a, b.a, t));
}

Vector2 InterpolateHelper::Interpolate(const Vector2& a, const Vector2& b, float t)
{
	return Vector2(CMath::Lerp(a.x, b.x, t), CMath::Lerp(a.y, b.y, t));
}

Vector3 InterpolateHelper::Interpolate(const Vector3& a, const Vector3& b, float t)
{
	return Vector3(CMath::Lerp(a.x, b.x, t), CMath::Lerp(a.y, b.y, t), CMath::Lerp(a.z, b.z, t));
}

Vector4 InterpolateHelper::Interpolate(const Vector4& a, const Vector4& b, float t)
{
	return Vector4(CMath::Lerp(a.x, b.x, t), CMath::Lerp(a.y, b.y, t), CMath::Lerp(a.z, b.z, t), CMath::Lerp(a.w, b.w, t));
}

Matrix4x4 InterpolateHelper::Interpolate(Matrix4x4& a, Matrix4x4& b, float t)
{
	Matrix4x4 mat;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mat[i][j] = CMath::Lerp(a[i][j], b[i][j], t);
		}
	}
	return mat;
}

Quaternion InterpolateHelper::Interpolate(const Quaternion& a, const Quaternion& b, float t)
{
	return Quaternion::Slerp(a, b, t);
}