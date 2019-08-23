#pragma once
#include "Shaders.h"

unsigned int Raster[NUM_PIXELS];

float* z_buffer = new float[NUM_PIXELS];


void ClearScreen()
{
	for (int i = 0; i < NUM_PIXELS; ++i)
	{
		Raster[i] = Black;
	}
}

void Clear_Z_Buffer()
{
	for (int i = 0; i < NUM_PIXELS; ++i)
	{
		z_buffer[i] = 1;
	}
}

void PlotPixel(int _x, int _y, unsigned int _color)
{
	assert(_x < RASTER_WIDTH);
	assert(_y < RASTER_HEIGHT);
	assert(_x >= 0);
	assert(_y >= 0);

	Raster[_y * RASTER_WIDTH + _x] = _color;
}

//void PlotPixel(VERTEX_2D _p)
//{
//	assert(_p.pos.x < RASTER_WIDTH);
//	assert(_p.pos.y < RASTER_HEIGHT);
//	assert(_p.pos.x >= 0);
//	assert(_p.pos.y >= 0);
//
//	Raster[(int)_p.pos.y * RASTER_WIDTH + (int)_p.pos.x] = _p._color;
//}

void Parametric(VERTEX_4D _a, VERTEX_4D _b)
{
#if 0
	//begin end ratio
	float deltaX = fabsf(_a.pos.x - _b.pos.x);
	float deltaY = fabsf(_a.pos.y - _b.pos.y);

	int totalPixel = static_cast<int>(fmaxf(deltaX, deltaY));

	for (int i = 0; i < totalPixel; ++i)
	{
		float ratio = i / static_cast<float>(totalPixel);
		// x = (B - A)*R + A
		float currX = (_b.pos.x - _a.pos.x) * ratio + _a.pos.x;
		float currY = (_b.pos.y - _a.pos.y) * ratio + _a.pos.y;

		VERTEX_3D copyColor;
		copyColor._color = _a._color;
		if (PIXEL_SHADER_3D)
			PIXEL_SHADER_3D(copyColor._color);

		PlotPixel(currX + 0.5f, currY + 0.5f, copyColor._color);
	}
#elif 1
	if (VertexShader)
	{
		VertexShader(_a);
		VertexShader(_b);
	}

	_a.pos.x = NDC_to_Screen_X(_a.pos.x);
	_a.pos.y = NDC_to_Screen_Y(_a.pos.y);

	_b.pos.x = NDC_to_Screen_X(_b.pos.x);
	_b.pos.y = NDC_to_Screen_Y(_b.pos.y);

	//begin end ratio
	float deltaX = fabsf(_a.pos.x - _b.pos.x);
	float deltaY = fabsf(_a.pos.y - _b.pos.y);

	int totalPixel = static_cast<int>(fmaxf(deltaX, deltaY));

	for (int i = 0; i < totalPixel; ++i)
	{
		float ratio = i / static_cast<float>(totalPixel);
		// x = (B - A)*R + A
		float currX = (_b.pos.x - _a.pos.x) * ratio + _a.pos.x;
		float currY = (_b.pos.y - _a.pos.y) * ratio + _a.pos.y;

		float currZ = (_b.pos.z - _a.pos.z) * ratio + _a.pos.z;

		unsigned int _color = ColorBlend(_a._color, _b._color, ratio);

		unsigned int zBufferIndex = Coordinates(currX, currY, RASTER_WIDTH);
		if (currZ < z_buffer[zBufferIndex])
		{
			if (PixelShader)
			{
				VEC_2D uv;
				PixelShader(_color, uv);
			}
			z_buffer[zBufferIndex] = currZ;

		PlotPixel(currX + 0.5f, currY + 0.5f, _color);
		}

	}

#endif
}

void FillTriangle(VERTEX_4D _a, VERTEX_4D _b, VERTEX_4D _c)
{
#if 0
	for (int y = 0; y < RASTER_HEIGHT; ++y)
	{
		for (int x = 0; x < RASTER_WIDTH; ++x)
		{
			// ?? determine if x & y are in the triangle
			VEC_4D bary = ComputeBarycentric(_a.pos, _b.pos, _c.pos, { x * 1.0f,y * 1.0f });
			if (bary.x > 0 && bary.y < 1 &&
				bary.y > 0 && bary.y < 1 &&
				bary.z > 0 && bary.z < 1)
			{
				PlotPixel(x, y, ColorBerp(_a, _b, _c, bary.x, bary.y, bary.z));
			}
		}
	}
#elif 0 //Brute force
	if (VertexShader)
	{
		VertexShader(_a);
		VertexShader(_b);
		VertexShader(_c);
	}

	_a.pos.x = NDC_to_Screen_X(_a.pos.x);
	_a.pos.y = NDC_to_Screen_Y(_a.pos.y);

	_b.pos.x = NDC_to_Screen_X(_b.pos.x);
	_b.pos.y = NDC_to_Screen_Y(_b.pos.y);

	_c.pos.x = NDC_to_Screen_X(_c.pos.x);
	_c.pos.y = NDC_to_Screen_Y(_c.pos.y);

	for (int y = 0; y < RASTER_HEIGHT; ++y)
	{
		for (int x = 0; x < RASTER_WIDTH; ++x)
		{
			// ?? determine if x & y are in the triangle
			VEC_3D bary = ComputeBarycentric({ _a.pos.x, _a.pos.y }, { _b.pos.x, _b.pos.y }, { _c.pos.x, _c.pos.y }, { x * 1.0f, y * 1.0f });
			if (bary.x > 0 && bary.x < 1 &&
				bary.y > 0 && bary.y < 1 &&
				bary.z > 0 && bary.z < 1)
			{
				//unsigned int _color = _a._color;
				unsigned int _color = ColorBerp(_a._color, _b._color, _c._color, bary.x, bary.y, bary.z);

				if (PixelShader)
				{
					//PixelShader();
					VEC_2D uv;
					uv.x = _a.uv.x * bary.x + _b.uv.x * bary.y + _c.uv.x * bary.z;
					uv.y = _a.uv.y * bary.x + _b.uv.y * bary.y + _c.uv.y * bary.z;

					PixelShader(_color, uv);
				}

				VERTEX_2D pixel_to_print;
				//PlotPixel(x, y, ColorBerp(_a._color, _b._color, _c._color, bary.x, bary.y, bary.z));
				PlotPixel({ { (float)x, (float)y }, _color });

			}
		}
	}
#elif 1 //Better Brute
	if (VertexShader)
	{
		VertexShader(_a);
		VertexShader(_b);
		VertexShader(_c);
	}

	_a.pos.x = NDC_to_Screen_X(_a.pos.x);
	_a.pos.y = NDC_to_Screen_Y(_a.pos.y);

	_b.pos.x = NDC_to_Screen_X(_b.pos.x);
	_b.pos.y = NDC_to_Screen_Y(_b.pos.y);

	_c.pos.x = NDC_to_Screen_X(_c.pos.x);
	_c.pos.y = NDC_to_Screen_Y(_c.pos.y);

	int startX = Min_3(_a.pos.x, _b.pos.x, _c.pos.x);
	int startY = Min_3(_a.pos.y, _b.pos.y, _c.pos.y);

	int endX = Max_3(_a.pos.x, _b.pos.x, _c.pos.x);
	int endY = Max_3(_a.pos.y, _b.pos.y, _c.pos.y);

	for (int startY = 0; startY < endY; ++startY)
	{
		for (int startX = 0; startX < endX; ++startX)
		{
			int currX = startX;
			int currY = startY;

			// ?? determine if x & y are in the triangle
			VEC_3D bary = ComputeBarycentric({ _a.pos.x, _a.pos.y }, { _b.pos.x, _b.pos.y }, { _c.pos.x, _c.pos.y }, { currX * 1.0f, currY * 1.0f });

			if (bary.x > 0 && bary.x < 1 &&
				bary.y > 0 && bary.y < 1 &&
				bary.z > 0 && bary.z < 1)
			{
				//unsigned int _color = _a._color;
				unsigned int _color = ColorBerp(_a._color, _b._color, _c._color, bary.x, bary.y, bary.z);

				float zBufferRatio = (_a.pos.z * bary.x) + (_b.pos.z * bary.y) + (_c.pos.z * bary.z);

				unsigned int buffer_index = Coordinates(currX, currY, RASTER_WIDTH);
				if (zBufferRatio < z_buffer[buffer_index])
				{
					if (PixelShader)
					{
						VEC_2D uv;
						uv.x = _a.uv.x * bary.x + _b.uv.x * bary.y + _c.uv.x * bary.z;
						uv.y = _a.uv.y * bary.x + _b.uv.y * bary.y + _c.uv.y * bary.z;

						PixelShader(_color, uv);
						//PixelShader();
					}

					z_buffer[buffer_index] = zBufferRatio;

					//PlotPixel(x, y, ColorBerp(_a._color, _b._color, _c._color, bary.x, bary.y, bary.z));
					//PlotPixel({ { (float)currX, (float)currY }, _color });
					PlotPixel(currX, currY, _color);
				}


			}
		}
	}

#elif 0 //Lari's Parametric Fill
	if (VertexShader)
	{
		VertexShader(_a);
		VertexShader(_b);
		VertexShader(_c);
	}

	VERTEX_4D sortArray[3] = { _a, _b, _c };
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; j++)
		{
			if (sortArray[i].pos.y > sortArray[j].pos.y)
			{
				VERTEX_4D temp = sortArray[i];
				sortArray[i] = sortArray[j];
				sortArray[j] = temp;
			}
		}
	VERTEX_4D top = sortArray[2];
	VERTEX_4D middle = sortArray[1];
	VERTEX_4D bottom = sortArray[0];

	float directionOfFill = ImplicitLineEquation(_a.pos, _b.pos, _c.pos);


#endif
}