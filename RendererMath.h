#pragma once
#include "Defines.h"


struct VEC_2D
{
	float x = 0;
	float y = 0;
};
struct VEC_3D
{
	float x = 0;
	float y = 0;
	float z = 0;
};

struct VEC_4D
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;
};

struct VERTEX_2D
{
	VEC_2D pos;
	unsigned int _color = 0xFFFFFFFF;
};

struct VERTEX_3D
{
	VEC_3D pos;
	unsigned int _color = 0xFFFFFFFF;
	//VEC_4D uv;
};

struct VERTEX_4D
{
	VEC_4D pos;
	unsigned int _color = 0xFFFFFFFF;
	VEC_2D uv;

};

struct MATRIX_3D
{
	union
	{
		struct
		{
			float e11, e12, e13, e21, e22, e23, e31, e32, e33;
		};
		float mat[3][3];
	};
};

struct MATRIX_4D
{
	union
	{
		struct
		{
			float e11, e12, e13, e14, e21, e22, e23, e24, e31, e32, e33, e34, e41, e42, e43, e44;
		};
		float mat[4][4];
	};
};

bool IsEqual(float a, float b)
{
	// NOTE: Do not modify.
	return fabsf(a - b) < EPSILON;
}

bool IsZero(float a)
{
	// NOTE: Do not modify
	return (fabs(a)) < EPSILON;
}

float Max(float a, float b)
{
	// NOTE: Do not modify.
	return (a > b) ? a : b;
}

float Min(float a, float b)
{
	// NOTE: Do not modify.
	return (a < b) ? a : b;
}

float Degrees_To_Radians(float _deg)
{
	// NOTE: Do not modify.
	return _deg * PI / 180.0f;
}

float Radians_To_Degrees(float Rad)
{
	// NOTE: Do not modify.
	return Rad * 180.0f / PI;
}

unsigned int Coordinates(unsigned int x, unsigned int y, unsigned int width)
{
	return (y * width) + x;
}

float ImplicitLineEquation(VEC_4D _a, VEC_4D _b, VEC_4D _p)
{
	//(y1-y2)px + (x2-x1) + x1y2 - y1x2

	return (_a.y - _b.y) * _p.x + (_b.x - _a.x) * _p.y + ((_a.x * _b.y) - (_a.y * _b.x));
}

VEC_3D ComputeBarycentric(VEC_4D _a, VEC_4D _b, VEC_4D _c, VEC_4D _p)
{
	float maxA = ImplicitLineEquation(_b, _c, _a);
	float maxB = ImplicitLineEquation(_c, _a, _b);
	float maxC = ImplicitLineEquation(_b, _a, _c);

	float subA = ImplicitLineEquation(_b, _c, _p);
	float subB = ImplicitLineEquation(_c, _a, _p);
	float subC = ImplicitLineEquation(_b, _a, _p);

	return { (subA / maxA), (subB / maxB), (subC / maxC) };
}


//VEC_4D ComputeBarycentric(VEC_4D _a, VEC_4D _b, VEC_4D _c, VEC_4D _p)
//{
//	float maxA = ImplicitLineEquation(_b, _c, _a);
//	float maxB = ImplicitLineEquation(_c, _a, _b);
//	float maxC = ImplicitLineEquation(_b, _a, _c);
//
//	float subA = ImplicitLineEquation(_b, _c, _p);
//	float subB = ImplicitLineEquation(_c, _a, _p);
//	float subC = ImplicitLineEquation(_b, _a, _p);
//
//	return { (subA / maxA), (subB / maxB), (subC / maxC) };
//}

unsigned int ColorBlend(unsigned int a, unsigned int b, float ratio)
{
	int aA = (a & 0xFF000000) >> 24;
	int aR = (a & 0x00FF0000) >> 16;
	int aG = (a & 0x0000FF00) >> 8;
	int aB = (a & 0x000000FF);

	int bA = (b & 0xFF000000) >> 24;
	int bR = (b & 0x00FF0000) >> 16;
	int bG = (b & 0x0000FF00) >> 8;
	int bB = (b & 0x000000FF);

	int alpha = ((float)(bA - aA) * ratio) + aA;
	int red = ((float)(bR - aR) * ratio) + aR;
	int green = ((float)(bG - aG) * ratio) + aG;
	int blue = ((float)(bB - aB) * ratio) + aB;

	return (alpha << 24) | (red << 16) | (green << 8) | (blue);

}

unsigned int ColorBerp(unsigned int a, unsigned int b, unsigned int c, float ratioAlpha, float ratioBeta, float ratioGamma)
{
	// break down all 3 colors
	// final red = R1 * rationAlpha + R2 * ratioBeta + R3 * ratioGamma

	int aA = (a & 0xFF000000) >> 24;
	int aR = (a & 0x00FF0000) >> 16;
	int aG = (a & 0x0000FF00) >> 8;
	int aB = (a & 0x000000FF);

	int bA = (b & 0xFF000000) >> 24;
	int bR = (b & 0x00FF0000) >> 16;
	int bG = (b & 0x0000FF00) >> 8;
	int bB = (b & 0x000000FF);

	int cA = (c & 0xFF000000) >> 24;
	int cR = (c & 0x00FF0000) >> 16;
	int cG = (c & 0x0000FF00) >> 8;
	int cB = (c & 0x000000FF);


	int alpha = (aA * ratioAlpha) + (bA * ratioBeta) + (cA * ratioGamma);
	int red = (aR * ratioAlpha) + (bR * ratioBeta) + (cR * ratioGamma);
	int green = (aG * ratioAlpha) + (bG * ratioBeta) + (cG * ratioGamma);
	int blue = (aB * ratioAlpha) + (bB * ratioBeta) + (cB * ratioGamma);

	return (alpha << 24) | (red << 16) | (green << 8) | (blue);
}

MATRIX_3D Matrix_Identity_3D()
{
	MATRIX_3D _m = { 1, 0, 0,
					0, 1, 0,
					0, 0, 1 };
	return _m;
}

MATRIX_4D Matrix_Identity_4D()
{
	MATRIX_4D _m = { 1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1 };
	return _m;
}

MATRIX_4D Matrix_Create_Translation_4D(VEC_4D _v)
{
	MATRIX_4D _m = Matrix_Identity_4D();

	_m.e14 = _v.x;
	_m.e24 = _v.y;
	_m.e34 = _v.z;
	_m.e44 = _v.w;

	return _m;
}

VERTEX_4D Vertex_Matrix_Multipication_4D(VERTEX_4D _v, MATRIX_4D _m)
{
	VERTEX_4D temp = _v;

	temp._color = _v._color;

	temp.pos.x = (_v.pos.x * _m.mat[0][0]) + (_v.pos.y * _m.mat[1][0]) + (_v.pos.z * _m.mat[2][0]) + (_v.pos.w * _m.mat[3][0]);
	temp.pos.y = (_v.pos.x * _m.mat[0][1]) + (_v.pos.y * _m.mat[1][1]) + (_v.pos.z * _m.mat[2][1]) + (_v.pos.w * _m.mat[3][1]);
	temp.pos.z = (_v.pos.x * _m.mat[0][2]) + (_v.pos.y * _m.mat[1][2]) + (_v.pos.z * _m.mat[2][2]) + (_v.pos.w * _m.mat[3][2]);
	temp.pos.w = (_v.pos.x * _m.mat[0][3]) + (_v.pos.y * _m.mat[1][3]) + (_v.pos.z * _m.mat[2][3]) + (_v.pos.w * _m.mat[3][3]);

	return temp;
}

VERTEX_3D Vertex_Matrix_Multipication_3D(VERTEX_3D _v, MATRIX_3D _m)
{
	VERTEX_3D temp;

	temp._color = _v._color;

	temp.pos.x = (_v.pos.x * _m.mat[0][0]) + (_v.pos.y * _m.mat[1][0]) + (_v.pos.z * _m.mat[2][0]);
	temp.pos.y = (_v.pos.x * _m.mat[0][1]) + (_v.pos.y * _m.mat[1][1]) + (_v.pos.z * _m.mat[2][1]);
	temp.pos.z = (_v.pos.x * _m.mat[0][2]) + (_v.pos.y * _m.mat[1][2]) + (_v.pos.z * _m.mat[2][2]);

	return temp;
}

VEC_3D Vertex_Negate_3D(VEC_3D _v)
{
	_v.x *= -1;
	_v.y *= -1;
	_v.z *= -1;

	return _v;
}

MATRIX_4D Translate(VEC_4D _v)
{
	MATRIX_4D temp = Matrix_Identity_4D();

	temp.e41 = _v.x;
	temp.e42 = _v.y;
	temp.e43 = _v.z;

	return temp;
};

MATRIX_4D Matrix_Rotation_4D_X(float _degree)
{
	float theta = Degrees_To_Radians(_degree);
	MATRIX_4D _m = Matrix_Identity_4D();

	_m.e11 = 1;
	_m.e22 = cosf(theta);
	_m.e23 = -1 * sinf(theta);
	_m.e32 = sinf(theta);
	_m.e33 = cosf(theta);
	_m.e44 = 1;
	return _m;
}

MATRIX_4D Matrix_Rotation_4D_Y(float _degree)
{
	float theta = Degrees_To_Radians(_degree);
	MATRIX_4D _m = Matrix_Identity_4D();

	_m.e11 = cos(theta);
	_m.e13 = sin(theta);
	_m.e22 = 1;
	_m.e31 = -1 * sin(theta);
	_m.e33 = cos(theta);
	_m.e44 = 1;

	return _m;
}

MATRIX_3D Matrix_Rotation_3D_Z(float _degree)
{
	float theta = Degrees_To_Radians(_degree);
	MATRIX_3D _m = Matrix_Identity_3D();

	_m.e11 = cos(theta);
	_m.e12 = -1 * sin(theta);
	_m.e21 = sin(theta);
	_m.e22 = cos(theta);
	_m.e33 = 1;
	return _m;
}

MATRIX_4D Matrix_Rotation_4D_Z(float _degree)
{
	float theta = Degrees_To_Radians(_degree);
	MATRIX_4D _m = Matrix_Identity_4D();

	_m.e11 = cos(theta);
	_m.e12 = -1 * sin(theta);
	_m.e21 = sin(theta);
	_m.e22 = cos(theta);
	_m.e33 = 1;
	_m.e44 = 1;

	return _m;
}

MATRIX_4D Matrix_to_Matrix_Multiply_4D(MATRIX_4D _m, MATRIX_4D _n)
{
	MATRIX_4D temp;

	temp.e11 = (_m.e11 * _n.e11) + (_m.e12 * _n.e21) + (_m.e13 * _n.e31) + (_m.e14 * _n.e41);
	temp.e12 = (_m.e11 * _n.e12) + (_m.e12 * _n.e22) + (_m.e13 * _n.e32) + (_m.e14 * _n.e42);
	temp.e13 = (_m.e11 * _n.e13) + (_m.e12 * _n.e23) + (_m.e13 * _n.e33) + (_m.e14 * _n.e43);
	temp.e14 = (_m.e11 * _n.e14) + (_m.e12 * _n.e24) + (_m.e13 * _n.e34) + (_m.e14 * _n.e44);

	temp.e21 = (_m.e21 * _n.e11) + (_m.e22 * _n.e21) + (_m.e23 * _n.e31) + (_m.e24 * _n.e41);
	temp.e22 = (_m.e21 * _n.e12) + (_m.e22 * _n.e22) + (_m.e23 * _n.e32) + (_m.e24 * _n.e42);
	temp.e23 = (_m.e21 * _n.e13) + (_m.e22 * _n.e23) + (_m.e23 * _n.e33) + (_m.e24 * _n.e43);
	temp.e24 = (_m.e21 * _n.e14) + (_m.e22 * _n.e24) + (_m.e23 * _n.e34) + (_m.e24 * _n.e44);

	temp.e31 = (_m.e31 * _n.e11) + (_m.e32 * _n.e21) + (_m.e33 * _n.e31) + (_m.e34 * _n.e41);
	temp.e32 = (_m.e31 * _n.e12) + (_m.e32 * _n.e22) + (_m.e33 * _n.e32) + (_m.e34 * _n.e42);
	temp.e33 = (_m.e31 * _n.e13) + (_m.e32 * _n.e23) + (_m.e33 * _n.e33) + (_m.e34 * _n.e43);
	temp.e34 = (_m.e31 * _n.e14) + (_m.e32 * _n.e24) + (_m.e33 * _n.e34) + (_m.e34 * _n.e44);

	temp.e41 = (_m.e41 * _n.e11) + (_m.e42 * _n.e21) + (_m.e43 * _n.e31) + (_m.e44 * _n.e41);
	temp.e42 = (_m.e41 * _n.e12) + (_m.e42 * _n.e22) + (_m.e43 * _n.e32) + (_m.e44 * _n.e42);
	temp.e43 = (_m.e41 * _n.e13) + (_m.e42 * _n.e23) + (_m.e43 * _n.e33) + (_m.e44 * _n.e43);
	temp.e44 = (_m.e41 * _n.e14) + (_m.e42 * _n.e24) + (_m.e43 * _n.e34) + (_m.e44 * _n.e44);


	return temp;
}


MATRIX_3D Matrix_Transpose_3D(MATRIX_3D _m)
{
	MATRIX_3D temp = _m;

	_m.e11 = temp.e11;
	_m.e12 = temp.e21;
	_m.e13 = temp.e31;

	_m.e21 = temp.e12;
	_m.e22 = temp.e22;
	_m.e23 = temp.e32;

	_m.e31 = temp.e13;
	_m.e32 = temp.e23;
	_m.e33 = temp.e33;

	return _m;
}

MATRIX_4D Fast_Inverse(MATRIX_4D _m)
{
	MATRIX_3D temp = { _m.e11, _m.e12, _m.e13, _m.e21, _m.e22, _m.e23, _m.e31, _m.e32, _m.e33 };

	temp = Matrix_Transpose_3D(temp);

	VERTEX_3D v_temp = { _m.e41, _m.e42, _m.e43 };
	v_temp = Vertex_Matrix_Multipication_3D(v_temp, temp);

	v_temp.pos = Vertex_Negate_3D(v_temp.pos);

	_m.e11 = temp.e11;
	_m.e12 = temp.e12;
	_m.e13 = temp.e13;
	_m.e14 = 0;

	_m.e21 = temp.e21;
	_m.e22 = temp.e22;
	_m.e23 = temp.e23;
	_m.e24 = 0;

	_m.e31 = temp.e31;
	_m.e32 = temp.e32;
	_m.e33 = temp.e33;
	_m.e34 = 0;

	_m.e41 = v_temp.pos.x;
	_m.e42 = v_temp.pos.y;
	_m.e43 = v_temp.pos.z;
	_m.e44 = 1;

	return _m;
}

MATRIX_4D Inverse_Matrix_4D(MATRIX_4D _m)
{
	float s0 = _m.mat[0][0] * _m.mat[1][1] - _m.mat[1][0] * _m.mat[0][1];
	float s1 = _m.mat[0][0] * _m.mat[1][2] - _m.mat[1][0] * _m.mat[0][2];
	float s2 = _m.mat[0][0] * _m.mat[1][3] - _m.mat[1][0] * _m.mat[0][3];
	float s3 = _m.mat[0][1] * _m.mat[1][2] - _m.mat[1][1] * _m.mat[0][2];
	float s4 = _m.mat[0][1] * _m.mat[1][3] - _m.mat[1][1] * _m.mat[0][3];
	float s5 = _m.mat[0][2] * _m.mat[1][3] - _m.mat[1][2] * _m.mat[0][3];

	float c5 = _m.mat[2][2] * _m.mat[3][3] - _m.mat[3][2] * _m.mat[2][3];
	float c4 = _m.mat[2][1] * _m.mat[3][3] - _m.mat[3][1] * _m.mat[2][3];
	float c3 = _m.mat[2][1] * _m.mat[3][2] - _m.mat[3][1] * _m.mat[2][2];
	float c2 = _m.mat[2][0] * _m.mat[3][3] - _m.mat[3][0] * _m.mat[2][3];
	float c1 = _m.mat[2][0] * _m.mat[3][2] - _m.mat[3][0] * _m.mat[2][2];
	float c0 = _m.mat[2][0] * _m.mat[3][1] - _m.mat[3][0] * _m.mat[2][1];

	// Should check for 0 determinant
	float invdet = 1.0 / (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);

	if (IsZero(invdet))
		return _m;
	MATRIX_4D temp = Matrix_Identity_4D();

	temp.mat[0][0] = (_m.mat[1][1] * c5 - _m.mat[1][2] * c4 + _m.mat[1][3] * c3) * invdet;
	temp.mat[0][1] = (-_m.mat[0][1] * c5 + _m.mat[0][2] * c4 - _m.mat[0][3] * c3) * invdet;
	temp.mat[0][2] = (_m.mat[3][1] * s5 - _m.mat[3][2] * s4 + _m.mat[3][3] * s3) * invdet;
	temp.mat[0][3] = (-_m.mat[2][1] * s5 + _m.mat[2][2] * s4 - _m.mat[2][3] * s3) * invdet;

	temp.mat[1][0] = (-_m.mat[1][0] * c5 + _m.mat[1][2] * c2 - _m.mat[1][3] * c1) * invdet;
	temp.mat[1][1] = (_m.mat[0][0] * c5 - _m.mat[0][2] * c2 + _m.mat[0][3] * c1) * invdet;
	temp.mat[1][2] = (-_m.mat[3][0] * s5 + _m.mat[3][2] * s2 - _m.mat[3][3] * s1) * invdet;
	temp.mat[1][3] = (_m.mat[2][0] * s5 - _m.mat[2][2] * s2 + _m.mat[2][3] * s1) * invdet;

	temp.mat[2][0] = (_m.mat[1][0] * c4 - _m.mat[1][1] * c2 + _m.mat[1][3] * c0) * invdet;
	temp.mat[2][1] = (-_m.mat[0][0] * c4 + _m.mat[0][1] * c2 - _m.mat[0][3] * c0) * invdet;
	temp.mat[2][2] = (_m.mat[3][0] * s4 - _m.mat[3][1] * s2 + _m.mat[3][3] * s0) * invdet;
	temp.mat[2][3] = (-_m.mat[2][0] * s4 + _m.mat[2][1] * s2 - _m.mat[2][3] * s0) * invdet;

	temp.mat[3][0] = (-_m.mat[1][0] * c3 + _m.mat[1][1] * c1 - _m.mat[1][2] * c0) * invdet;
	temp.mat[3][1] = (_m.mat[0][0] * c3 - _m.mat[0][1] * c1 + _m.mat[0][2] * c0) * invdet;
	temp.mat[3][2] = (-_m.mat[3][0] * s3 + _m.mat[3][1] * s1 - _m.mat[3][2] * s0) * invdet;
	temp.mat[3][3] = (_m.mat[2][0] * s3 - _m.mat[2][1] * s1 + _m.mat[2][2] * s0) * invdet;

	return temp;
}

MATRIX_4D Perspective_Projection_4D(float _fov, float _np, float _fp, float _ar)
{
	MATRIX_4D temp = Matrix_Identity_4D();

	float yScale = 1.0f / tanf(Degrees_To_Radians(.5 * _fov));
	temp.e11 = yScale * _ar;
	temp.e22 = yScale;
	temp.e33 = _fp / (_fp - _np);
	temp.e34 = 1;
	temp.e43 = -(_fp * _np) / (_fp - _np);
	temp.e44 = 0;

	return temp;
}