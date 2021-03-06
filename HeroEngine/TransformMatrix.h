#pragma once
#include "Vector3d.h"
#include <cmath>
#include <string>

class TransformMatrix
{
	public:
		float matrix[4][4] = { 0 };

		TransformMatrix();
		TransformMatrix(float other[4][4]);
		TransformMatrix(const TransformMatrix& other);
		~TransformMatrix();

		void print(std::string title = "");
		void FillWithZero();
		void DiagonalMatrix();
		Vector3d Multiply(const Vector3d& vector);
		void MultiplyRef(Vector3d& vector);
		void Multiply(const TransformMatrix& other);
		void MakeRotationX(float angleRad);
		void MakeRotationY(float angleRad);
		void MakeRotationZ(float angleRad);
		void MakeTranslation(float x, float y, float z);
		void MakeProjection(float dFovRad, float dAspectRatio, float dNear, float dFar);
		void QuickInverse(const TransformMatrix &other);
		void PointAt(const Vector3d& pos, const Vector3d& target,const Vector3d& up);
		

};

