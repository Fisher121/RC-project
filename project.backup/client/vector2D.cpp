#include "vector2D.h"

vector2D::vector2D()
{
	x = 0.0f;
	y = 0.0f;
}

vector2D::vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}

vector2D& vector2D::Add(const vector2D& vec)
{
	this->x += vec.x;
	this->y += vec.y;
	return *this;
}

vector2D& vector2D::Subtract(const vector2D& vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	return *this;
}

vector2D& vector2D::Multiply(const vector2D& vec)
{
	this->x *= vec.x;
	this->y *= vec.y;
	return *this;
}

vector2D& vector2D::Divide(const vector2D& vec)
{
	this->x /= vec.x;
	this->y /= vec.y;
	return *this;
}

vector2D& operator+(vector2D& v1, vector2D v2)
{
	return v1.Add(v2);
}

vector2D& operator-(vector2D& v1, vector2D v2)
{
	return v1.Subtract(v2);
}

vector2D& operator*(vector2D& v1, vector2D v2)
{
	return v1.Multiply(v2);
}

vector2D& operator/(vector2D& v1, vector2D v2)
{
	return v1.Divide(v2);
}

vector2D& vector2D::operator+=(const vector2D& vec)
{
	return this->Add(vec);
}

vector2D& vector2D::operator-=(const vector2D& vec)
{
	return this->Subtract(vec);
}

vector2D& vector2D::operator*=(const vector2D& vec)
{
	return this->Multiply(vec);
}

vector2D& vector2D::operator/=(const vector2D& vec)
{
	return this->Divide(vec);
}

vector2D& vector2D::operator*(const int& i)
{
	this->x *= i;
	this->y *= i;
	return 	*this;
}

vector2D& vector2D::Zero()
{
	this->x = 0.0f;
	this->y = 0.0f;
	return *this;
}

std::ostream& operator<<(std::ostream& stream, const vector2D& vec)
{
	stream << "(" << vec.x << ',' << vec.y << ')';
	return stream;
}
