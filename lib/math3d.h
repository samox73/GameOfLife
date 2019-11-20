#include <cmath>
#include <math.h>
#include <iostream>

#define ToRad(x) (float)(((x) * M_PI / 180.0f))
#define ToDeg(x) (float)(((x) / M_PI * 180.0f))

struct vector3 {
	float _x, _y, _z;

	vector3() {
		_x = 0;
		_y = 0;
		_z = 0;
	}

	vector3(float x, float y, float z=0) {
		_x = x;
		_y = y;
		_z = z;
	}

	vector3(const vector3 &vec);				//copy constructor
    vector3 operator+(const vector3 &vec);		//addition
    vector3 &operator+=(const vector3 &vec);	//assigning new result to the vector
	vector3 operator-(const vector3 &vec);		//substraction
    vector3 &operator-=(const vector3 &vec);	//assigning new result to the vector
    friend vector3 operator*(vector3 vec, float value);				//multiplication
    friend vector3 operator*(float value, vector3 vec);				//multiplication
    friend float operator*(const vector3 vec1, const vector3 vec2);		//scalar dot_product
    vector3 &operator*=(float value);			//assigning new result to the vector.
    friend vector3 operator/(vector3 vec, float value);				//division
    vector3 &operator/=(float value);  			//assigning new result to the vector
    vector3 &operator=(const vector3 &vec);
	
    float get_x();								//return x
    float get_y();								//return y
    float get_z();								//return z
    void print();									//print value of vectors
	friend std::ostream& operator<<(std::ostream& os, const vector3& vec);

};

vector3::vector3(const vector3 &vec) {
	_x = vec._x;
	_y = vec._y;
	_z = vec._z;
}

vector3 vector3::operator+(const vector3 &v) {
	return vector3(_x + v._x, _y + v._y, _z + v._z);
}

vector3 &vector3::operator+=(const vector3 &v) {
	_x += v._x;
	_y += v._y;
	_z += v._z;
	return *this;
}

vector3 vector3::operator-(const vector3 &v) {
	return vector3(_x - v._x, _y - v._y, _z - v._z);
}

vector3 &vector3::operator-=(const vector3 &v) {
	_x -= v._x;
	_y -= v._y;
	_z -= v._z;
	return *this;
}

vector3 operator*(vector3 vec, float scalar) {
	return vector3(scalar * vec._x, scalar * vec._y, scalar * vec._z);
}

vector3 operator*(float scalar, vector3 vec) {
	return vector3(scalar * vec._x, scalar * vec._y, scalar * vec._z);
}

float operator*(vector3 vec1, vector3 vec2) {
	return vec1._x*vec2._x + vec1._y*vec2._y + vec1._z*vec2._z;
}

vector3 &vector3::operator*=(float scalar) {
	_x *= scalar;
	_y *= scalar;
	_z *= scalar;
	return *this;
}

vector3 operator/(vector3 vec, float scalar) {
	return vector3(vec._x / scalar, vec._y / scalar, vec._z / scalar);
}

vector3 &vector3::operator/=(float scalar) {
	_x /= scalar;
	_y /= scalar;
	_z /= scalar;
	return *this;
}

vector3 &vector3::operator=(const vector3 &vec) {
    _x=vec._x;
    _y=vec._y;
    _z=vec._z;
    return *this;
}

void vector3::print() {
	std::cout << _x << " " << _y << " " << _z << std::endl;
}

std::ostream& operator<<(std::ostream& os, const vector3& v)
{
    os << v._x << ' ' << v._y << ' ' << v._z;
    return os;
}

float vector3::get_x() {
	return _x;
}

float vector3::get_y() {
	return _y;
}

float vector3::get_z() {
	return _z;
}

// ---------------------------------------------------------

vector3 outer_product(vector3 vec1, vector3 vec2) {
	return vector3(vec1._y*vec2._z - vec1._z*vec2._y, vec1._z*vec2._x - vec1._x*vec2._z, vec1._x*vec2._y - vec1._y*vec2._x);
}

float norm(vector3 vec) {
	return sqrt(vec*vec);
}

vector3 normalize(vector3 vec){
	return vec / norm(vec);
}

float distance(vector3 vec1, vector3 vec2) {
	return norm(vec2 - vec1);
}
