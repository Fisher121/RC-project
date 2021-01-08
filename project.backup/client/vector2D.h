#pragma once
#include <iostream>


class vector2D {
public:
	float x;
	float y;

	vector2D();
	vector2D(float x, float y);

	vector2D& Add(const vector2D& vec);
	vector2D& Subtract(const vector2D& vec);
	vector2D& Multiply(const vector2D& vec);
	vector2D& Divide(const vector2D& vec);

	friend vector2D& operator+(vector2D& v1,vector2D v2);
	friend vector2D& operator-(vector2D& v1, vector2D v2);
	friend vector2D& operator*(vector2D& v1, vector2D v2);
	friend vector2D& operator/(vector2D& v1, vector2D v2);

	vector2D& operator+=(const vector2D& vec);
	vector2D& operator-=(const vector2D& vec);
	vector2D& operator*=(const vector2D& vec);
	vector2D& operator/=(const vector2D& vec);

	vector2D& operator*(const int& i);
	vector2D& Zero();

	friend std::ostream& operator<<(std::ostream& stream, const vector2D& vec);


};
