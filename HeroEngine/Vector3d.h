#pragma once
#include <string>
#include <iostream>

class Vector3d {

public:
	float x, y, z, w;  // 3 dimension value and 4th value for matrix calculation

	Vector3d* Normalize();
	Vector3d* Add(const Vector3d& vector);
	Vector3d* Multiple(float value);
	Vector3d* Sub(const Vector3d& vector);
	Vector3d* Divide(float value);

	Vector3d* CrossProduct(const Vector3d& vector);

	float DotProduct(const Vector3d& vector) const;
	float DotProduct() const;
	float GetDistance(const Vector3d& vector) const;
	float GetLength() const;

	static Vector3d Normalize(const Vector3d& vector);
	static Vector3d Sub(const Vector3d& A, const Vector3d& B);
	static Vector3d Add(const Vector3d& A, const Vector3d& B);
	static Vector3d CrossProduct(const Vector3d& A, const Vector3d& B);
	static Vector3d Multiple(const Vector3d& A, float value);
	static float DotProduct(const Vector3d& A, const Vector3d& B);
	static Vector3d IntersectPlane(const Vector3d& planeP, const Vector3d& planeN, const Vector3d& lineStart, const Vector3d& lineEnd);

	void Transform(float x, float y, float z);
	void SetTo(float x, float y, float z, float w = 1.0f);

	std::string ToString(std::string title = "") const;

	Vector3d();
	Vector3d(const Vector3d& other);
	Vector3d(float x, float y, float z, float w=1.0);
	~Vector3d();

	void print(std::string title = "") const;
};
