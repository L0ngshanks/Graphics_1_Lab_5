#pragma once
#include "RendererMath.h"
#include "treeolife.h"

MATRIX_4D SV_WorldMatrix = Matrix_Identity_4D();

MATRIX_4D SV_View = Matrix_Identity_4D();

MATRIX_4D SV_Perspective = Matrix_Identity_4D();

unsigned int SP_Color = 0x00000000;

unsigned int SP_Texel = 0xFFFFFFFF;

float SV_Scale = 0.5f;

//Vertex Shader
void(*VertexShader)(VERTEX_4D&) = nullptr;

//Pixel Shader
void(*PixelShader)(unsigned int&, VEC_2D) = nullptr;

// VS_Shaders
void VS_World(VERTEX_4D& _v)
{
	_v = Vertex_Matrix_Multipication_4D(_v, SV_WorldMatrix);
	_v = Vertex_Matrix_Multipication_4D(_v, SV_View);
	_v = Vertex_Matrix_Multipication_4D(_v, SV_Perspective);

	_v.pos.x /= _v.pos.w;
	_v.pos.y /= _v.pos.w;
	_v.pos.z /= _v.pos.w;
}

void VS_Shrink(VERTEX_4D& _modify)
{
	_modify.pos.x *= SV_Scale;
	_modify.pos.y *= SV_Scale;
	_modify.pos.z *= SV_Scale;

}

void VS_MoveLeft(VERTEX_4D& _modify)
{
	_modify.pos.x -= 0.1f;
}

// PS_Shaders

void PS_White(unsigned int& makeWhite, VEC_2D _v)
{
	makeWhite = White;
}

void PS_Red(unsigned int& makeRed, VEC_2D _v)
{
	makeRed = Red;
}

void PS_Green(unsigned int& makeGreen, VEC_2D _v)
{
	makeGreen = Green;
}

void PS_Blue(unsigned int& makeBlue, VEC_2D _v)
{
	makeBlue = Blue;
}

void PS_ChangeColor(unsigned int& changeColor, VEC_2D _v)
{
	changeColor = SP_Color;
}

//void PS_Castle(unsigned int& _shade, VEC_2D _v)
//{
//	int x = _v.x * _4_Castle_TGA_width;
//	int y = _v.y * _4_Castle_TGA_height;
//
//	_shade = _4_Castle_TGA_pixels[Coordinates(x, y, _4_Castle_TGA_width)];
//
//	_shade = ((_shade & 0xFF000000) >> 24) |
//		((_shade & 0x00FF0000) >> 8) |
//		((_shade & 0x0000FF00) << 8) |
//		((_shade & 0x000000FF) << 24);
//
//}

void PS_TreeOfLife(unsigned int& _shade, VEC_2D _v)
{
	int x = _v.x * treeolife_width;
	int y = _v.y * treeolife_height;

	_shade = treeolife_pixels[Coordinates(x, y, treeolife_width)];

	//BGRA - >> XRGB
	_shade = ((_shade & 0xFF000000) >> 24) |
		((_shade & 0x00FF0000) >> 8) |
		((_shade & 0x0000FF00) << 8) |
		((_shade & 0x000000FF) << 24);

}