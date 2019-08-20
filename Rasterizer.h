#pragma once
#include "Shaders.h"

unsigned int Raster[NUM_PIXELS];

void ClearScreen()
{
	for (int i = 0; i < NUM_PIXELS; ++i)
	{
		Raster[i] = Black;
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

void PlotPixel(VERTEX_2D _p)
{
	assert(_p.pos.x < RASTER_WIDTH);
	assert(_p.pos.y < RASTER_HEIGHT);
	assert(_p.pos.x >= 0);
	assert(_p.pos.y >= 0);

	Raster[(int)_p.pos.y * RASTER_WIDTH + (int)_p.pos.x] = _p._color;
}


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

		//unsigned int _color = ColorBlend(_a, _b, ratio);

		if (PixelShader)
		{
			VEC_2D uv;
			for(int y = 0; i < RASTER_WIDTH; ++i)
				for (int x = 0; y < RASTER_HEIGHT; ++y)
				{
					uv.x = x / RASTER_WIDTH;
					uv.y = y / RASTER_HEIGHT;
				}
			//float u = _a.uv.x * bary.
			//PixelShader(ColorBerp(_a._color, _b._color, _c._color, bary.x, bary.y, bary.z));
			//PixelShader(_color, uv);
		}

		//PlotPixel(currX + 0.5f, currY + 0.5f, _color);
	}

#endif
}

void FillTriange(VERTEX_4D _a, VERTEX_4D _b, VERTEX_4D _c)
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
#elif 1
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

#endif
}