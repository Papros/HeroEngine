#include "Vector3d.h"
#include <cmath>

Vector3d* Vector3d::Normalize()
{
	float len = this->GetLength();
	this->x /= len;
	this->y /= len;
	this->z /= len;
	return this;
}

Vector3d* Vector3d::Add(const Vector3d& vector)
{
	this->x += vector.x;
	this->y += vector.y;
	this->z += vector.z;
	return this;
}

Vector3d* Vector3d::Multiple(float value)
{
	this->x *= value;
	this->y *= value;
	this->z *= value;
	return this;
}

Vector3d* Vector3d::Sub(const Vector3d& vector)
{
	this->x -= vector.x;
	this->y -= vector.y;
	this->z -= vector.z;
	return this;
}

Vector3d* Vector3d::Divide(float value)
{
	if (value == 0.0f) {
		std::cout << " Divide vector by zero" << std::endl;
		return this;
	}
	this->x /= value;
	this->y /= value;
	this->z /= value;
	return this;
}

Vector3d* Vector3d::CrossProduct(const Vector3d& vector)
{
	float n_x = y * vector.z - z * vector.y;
	float n_y = z * vector.x - x * vector.z;
	float n_z = x * vector.y - y * vector.x;

	this->x = n_x;
	this->y = n_y;
	this->z = n_z;

	this->Normalize();
	return this;
}

float Vector3d::DotProduct(const Vector3d& vector) const
{
	return (x * vector.x + y * vector.y + z * vector.z);
}

float Vector3d::DotProduct() const
{
	return (x * x + y * y + z * z);
}

float Vector3d::GetDistance(const Vector3d& vector) const
{
	float a = vector.x - x, b = vector.y - y, c = vector.z - z;

	return sqrtf(a*a + b*b +c*c);
}

float Vector3d::GetLength() const
{
	return sqrtf(this->DotProduct());
}

Vector3d Vector3d::Normalize(const Vector3d& vector)
{
	float len = vector.GetLength();
	return Vector3d(vector.x / len, vector.y / len, vector.z / len);
}

Vector3d Vector3d::Sub(const Vector3d& A, const Vector3d& B)
{
	return Vector3d(A.x - B.x, A.y - B.y, A.z - B.z);
}

Vector3d Vector3d::Add(const Vector3d& A, const Vector3d& B)
{
	return Vector3d(A.x + B.x, A.y + B.y, A.z + B.z);
}

Vector3d Vector3d::CrossProduct(const Vector3d& A, const Vector3d& B)
{
	float n_x = A.y * B.z - A.z * B.y;
	float n_y = A.z * B.x - A.x * B.z;
	float n_z = A.x * B.y - A.y * B.x;
	return *Vector3d(n_x, n_y, n_z).Normalize();
}

Vector3d Vector3d::Multiple(const Vector3d& A, float value)
{
	return Vector3d(A.x*value, A.y*value, A.z*value);
}

float Vector3d::DotProduct(const Vector3d& A, const Vector3d& B)
{
	return A.DotProduct(B);
}

void Vector3d::Transform(float x, float y, float z)
{
	this->x += x;
	this->y += y;
	this->z += z;
}

void Vector3d::SetTo(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector3d Vector3d::IntersectPlane(const Vector3d& planeP, const Vector3d& planeN, const Vector3d& lineStart, const Vector3d& lineEnd)
{
	Vector3d plane_n = Vector3d::Normalize(planeN);
	float plane_d = -plane_n.DotProduct(planeP);
	float ad = lineStart.DotProduct(plane_n);
	float bd = lineEnd.DotProduct(plane_n);
	float t = (-plane_d - ad) / (bd - ad);
	Vector3d lineStartToEnd = Vector3d::Sub(lineEnd, lineStart);
	return Vector3d::Add(lineStart, Vector3d::Multiple(lineStartToEnd, t) );
}

std::string Vector3d::ToString(std::string title) const
{
	return (title!=""? title+" = " : "" ) + "[ "+std::to_string(x)+" , "+ std::to_string(y) +" , "+ std::to_string(z)+" ]";
}

Vector3d::Vector3d()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->w = 1.0f;
}

Vector3d::Vector3d(const Vector3d& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = other.w;
}

Vector3d::Vector3d(float x, float y, float z, float w )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector3d::~Vector3d()
{
}

void Vector3d::print(std::string title) const
{
	std::cout << (title) << ":> " << x << ":" << y<<":" << z << std::endl;
}
