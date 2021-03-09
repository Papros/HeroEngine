#include "TransformMatrix.h"
#include <iostream>

TransformMatrix::TransformMatrix()
{

}

TransformMatrix::TransformMatrix(float other[4][4])
{
    for (int a = 0; a < 4; a++)
        for (int b = 0; b < 4; b++)
            this->matrix[a][b] = other[a][b];
}

TransformMatrix::TransformMatrix(const TransformMatrix& other)
{
    for (int a = 0; a < 4; a++)
        for (int b = 0; b < 4; b++)
            this->matrix[a][b] = other.matrix[a][b];
}

TransformMatrix::~TransformMatrix()
{
}

void TransformMatrix::print(std::string title)
{
    if (!title.empty()) std::cout << "Matrix: " << title << std::endl;
    for (int a = 0; a < 4; a++) {

        for (int b = 0; b < 4; b++) {
            std::cout << "[ " << matrix[a][b] << " ]";
        }
        std::cout << std::endl;
    }
}

void TransformMatrix::FillWithZero()
{
    for (int a = 0; a < 4; a++)
        for (int b = 0; b < 4; b++)
            this->matrix[a][b] = 0.0f;
}

void TransformMatrix::DiagonalMatrix()
{
    this->matrix[0][0] = 1.0f;
    this->matrix[1][1] = 1.0f;
    this->matrix[2][2] = 1.0f;
    this->matrix[3][3] = 1.0f;
}

Vector3d TransformMatrix::Multiply(const Vector3d& vector)
{
    float x, y, z, w;
    x = vector.x * matrix[0][0] + vector.y * matrix[1][0] + vector.z * matrix[2][0] + vector.w * matrix[3][0];
    y = vector.x * matrix[0][1] + vector.y * matrix[1][1] + vector.z * matrix[2][1] + vector.w * matrix[3][1];
    z = vector.x * matrix[0][2] + vector.y * matrix[1][2] + vector.z * matrix[2][2] + vector.w * matrix[3][2];
    w = vector.x * matrix[0][3] + vector.y * matrix[1][3] + vector.z * matrix[2][3] + vector.w * matrix[3][3];
    
    if (w != 0.0f) {
        x /= w;
        y /= w;
        z /= w;
        w /= w;
    }

    return Vector3d(x,y,z,w);
}

void TransformMatrix::MultiplyRef(Vector3d& vector)
{
    float x, y, z, w;
    x = vector.x * matrix[0][0] + vector.y * matrix[1][0] + vector.z * matrix[2][0] + vector.w * matrix[3][0];
    y = vector.x * matrix[0][1] + vector.y * matrix[1][1] + vector.z * matrix[2][1] + vector.w * matrix[3][1];
    z = vector.x * matrix[0][2] + vector.y * matrix[1][2] + vector.z * matrix[2][2] + vector.w * matrix[3][2];
    w = vector.x * matrix[0][3] + vector.y * matrix[1][3] + vector.z * matrix[2][3] + vector.w * matrix[3][3];

    vector.SetTo(x, y, z);
}

void TransformMatrix::Multiply(const TransformMatrix& other)
{
    float mat[4][4] = { 0.0f };

	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			mat[r][c] = matrix[r][0] * other.matrix[0][c] + matrix[r][1] * other.matrix[1][c] + matrix[r][2] * other.matrix[2][c] + matrix[r][3] * other.matrix[3][c];
    
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            matrix[r][c] = mat[r][c];
}



void TransformMatrix::MakeRotationX(float angleRad)
{
    matrix[0][0] = 1.0f;
    matrix[1][1] = cosf(angleRad);
    matrix[1][2] = sinf(angleRad);
    matrix[2][1] = -sinf(angleRad);
    matrix[2][2] = cosf(angleRad);
    matrix[3][3] = 1.0f;
}

void TransformMatrix::MakeRotationY(float angleRad)
{
    matrix[0][0] = cosf(angleRad);
    matrix[0][2] = sinf(angleRad);
    matrix[2][0] = -sinf(angleRad);
    matrix[1][1] = 1.0f;
    matrix[2][2] = cosf(angleRad);
    matrix[3][3] = 1.0f;
}

void TransformMatrix::MakeRotationZ(float angleRad)
{
    matrix[0][0] = cosf(angleRad);
    matrix[0][1] = sinf(angleRad);
    matrix[1][0] = -sinf(angleRad);
    matrix[1][1] = cosf(angleRad);
    matrix[2][2] = 1.0f;
    matrix[3][3] = 1.0f;
}

void TransformMatrix::MakeTranslation(float x, float y, float z)
{
    matrix[0][0] = 1.0f;
    matrix[1][1] = 1.0f;
    matrix[2][2] = 1.0f;
    matrix[3][3] = 1.0f;

    matrix[3][0] = x;
    matrix[3][1] = y;
    matrix[3][2] = z;
}

void TransformMatrix::MakeProjection(float FovDeg, float AspectRatio, float Near, float Far)
{
    float Fov = 1.0f / tanf(FovDeg * 0.5f);
  
    matrix[0][0] = AspectRatio * Fov;
    matrix[1][1] = Fov;
    matrix[2][2] = Far / (Far - Near);
    matrix[3][2] = (-Far * Near) / (Far - Near);
    matrix[2][3] = 1.0f;
    matrix[3][3] = 0.0f;
}

void TransformMatrix::QuickInverse(const TransformMatrix& other)
{
    matrix[0][0] = other.matrix[0][0]; matrix[0][1] = other.matrix[1][0]; matrix[0][2] = other.matrix[2][0]; matrix[0][3] = 0.0f;
    matrix[1][0] = other.matrix[0][1]; matrix[1][1] = other.matrix[1][1]; matrix[1][2] = other.matrix[2][1]; matrix[1][3] = 0.0f;
    matrix[2][0] = other.matrix[0][2]; matrix[2][1] = other.matrix[1][2]; matrix[2][2] = other.matrix[2][2]; matrix[2][3] = 0.0f;

    matrix[3][0] = -(other.matrix[3][0] * matrix[0][0] + other.matrix[3][1] * matrix[1][0] + other.matrix[3][2] * matrix[2][0]);
    matrix[3][1] = -(other.matrix[3][0] * matrix[0][1] + other.matrix[3][1] * matrix[1][1] + other.matrix[3][2] * matrix[2][1]);
    matrix[3][2] = -(other.matrix[3][0] * matrix[0][2] + other.matrix[3][1] * matrix[1][2] + other.matrix[3][2] * matrix[2][2]);
    matrix[3][3] = 1.0f;
}

void TransformMatrix::PointAt(const Vector3d& pos, const Vector3d& target, const Vector3d& up)
{
    // New Forward direction
    Vector3d newForward = *(Vector3d::Sub(target,pos).Normalize());
    
    // new Up direction
    Vector3d a = Vector3d::Multiple(newForward, Vector3d::DotProduct(up, newForward) );
    
    Vector3d newUp = *Vector3d::Sub(up, a).Normalize(); 
   
    // New Right direction
    Vector3d newRight = Vector3d::CrossProduct(newUp, newForward);
   
    matrix[0][0] = newRight.x;
    matrix[1][0] = newUp.x;
    matrix[2][0] = newForward.x;
    matrix[3][0] = pos.x;

    matrix[0][1] = newRight.y;
    matrix[1][1] = newUp.y;
    matrix[2][1] = newForward.y;
    matrix[3][1] = pos.y;

    matrix[0][2] = newRight.z;
    matrix[1][2] = newUp.z;
    matrix[2][2] = newForward.z;
    matrix[3][2] = pos.z;

    matrix[0][3] = 0.0f;
    matrix[1][3] = 0.0f;
    matrix[2][3] = 0.0f;
    matrix[3][3] = 1.0f;
}
