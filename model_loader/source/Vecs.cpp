//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: Vecs.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 30/09/21
// Last Edited:  30/09/21
// Brief: Collection of function implementations for Vector types used to store dimensional data
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Vecs.h"

Vec4::Vec4()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

Vec4::Vec4(const float a_fInX, const float a_fInY, const float a_fInZ, const float a_fInW)
{
	this->x = a_fInX;
	this->y = a_fInY;
	this->z = a_fInZ;
	this->w = a_fInW;
}

Vec3::Vec3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vec3::Vec3(float a_fInX, float a_fInY, float a_fInZ)
{
	this->x = a_fInX;
	this->y = a_fInY;
	this->z = a_fInZ;
}

Vec3::Vec3(Vec4 a_oInVecFour)
{
	x = a_oInVecFour.x;
	y = a_oInVecFour.y;
	z = a_oInVecFour.z;
}


Vec2::Vec2()
{
	x = 0.0f;
	y = 0.0f;
}

Vec2::Vec2(const float a_fInX, const float a_fInY)
{
	x = a_fInX;
	y = a_fInY;
}

Vec2::Vec2(Vec4 a_oInVecFour)
{
	x = a_oInVecFour.x;
	y = a_oInVecFour.y;
}

Vec2::Vec2(Vec3 a_oInVecThree)
{
	x = a_oInVecThree.x;
	y = a_oInVecThree.y;
} 

