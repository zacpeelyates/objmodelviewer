#ifndef __VECS_H__
#define __VECS_H__

	struct Vec4
	{
	public:
		float x, y, z, w;
		Vec4();
		Vec4(const float a_fInX, const float a_fInY, const float a_fInZ, const float a_fInW);
	};

	struct Vec3
	{
	public:
		float x, y, z;
		Vec3();
		Vec3(const float a_fInX, const float a_fInY, const float a_fInZ);
		Vec3(Vec4 a_oInVecFour);
	};

	struct Vec2
	{
	public:
		float x, y;
		Vec2();
		Vec2(const float a_fInX, const float a_fInY);
		Vec2(Vec4 a_oInVecFour);
		Vec2(Vec3 a_oInVecThree);
		
	};
#endif // !__VECS_H__