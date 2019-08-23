#include "Rasterizer.h"
#include "RasterSurface.h"

int main(int argc, char** argv)
{
	std::cout << Min_3(4, 5, 6) << " : " << Max_3(4, 5, 6) << std::endl;

	RS_Initialize(RASTER_WIDTH, RASTER_HEIGHT);

	VERTEX_4D tris[24]
	{
		//Front
		{ {-.25, -.25, .25, 1}, 0xFFFFFFFF, {0.0f,1.0f}}, //0
		{ {.25, -.25, .25, 1}, 0xFFFFFFFF, {1.0f, 1.0f}}, //1
		{ {.25, .25, .25, 1}, 0xFFFFFFFF, {1.0f, 0.0f}}, //2
		{ {-.25, .25, .25, 1}, 0xFFFFFFFF, {0.0f,0.0f}}, //3

		//Back
		{ {-.25, -.25, -.25, 1}, 0xFFFFFFFF, {1.0f, 1.0f}}, //4
		{ {.25, -.25, -.25, 1}, 0xFFFFFFFF, {0.0f, 1.0f}}, //5
		{ {.25, .25, -.25, 1}, 0xFFFFFFFF, {00.0f,0.0f}}, //6
		{ {-.25, .25, -.25, 1}, 0xFFFFFFFF, {1.0f, 0.0f}}, //7

		//Left
		{ {-.25, -.25, -.25, 1}, 0xFFFFFFFF, {0.0f, 1.0f}}, //8
		{ {-.25, -.25, .25, 1}, 0xFFFFFFFF, {1.0f,1.0f}}, //9
		{ {-.25, .25, .25, 1}, 0xFFFFFFFF, {1.0f,0.0f}}, //10
		{ {-.25, .25, -.25, 1}, 0xFFFFFFFF, {0.0f, 0.0f}}, //11

		//Right
		{ {.25, -.25, .25, 1}, 0xFFFFFFFF, {0.0f, 1.0f}}, //12
		{ {.25, -.25, -.25, 1}, 0xFFFFFFFF, {1.0f, 1.0f}}, //13
		{ {.25, .25, -.25, 1}, 0xFFFFFFFF, {1.0f,0.0f}}, //14
		{ {.25, .25, .25, 1}, 0xFFFFFFFF, {0.0f, 0.0f}}, //15

		//Top
		{ {-.25, .25, .25, 1}, 0xFFFFFFFF, {1.0f,0.0f}}, //16
		{ {.25, .25, .25, 1}, 0xFFFFFFFF, {1.0f, 1.0f}}, //17
		{ {.25, .25, -.25, 1}, 0xFFFFFFFF, {1.0f,0.0f}}, //18
		{ {-.25, .25, -.25, 1}, 0xFFFFFFFF, {0.0f, 0.0f}}, //19

		//Bottom
		{ {-.25, -.25, .25, 1}, 0xFFFFFFFF, {0.0f,1.0f}}, //20
		{ {.25, -.25, .25, 1}, 0xFFFFFFFF, {1.0f, 1.0f}}, //21
		{ {.25, -.25, -.25, 1}, 0xFFFFFFFF, {0.0f, 1.0f}}, //22
		{ {-.25, -.25, -.25, 1}, 0xFFFFFFFF, {1.0f, 1.0f}}, //23
	};

	VERTEX_4D satalite[8]
	{
		{ {-.05, -.05, .05, 1}, 0xFFFFFFFF},
		{ {.05, -.05, .05, 1}, 0xFFFFFFFF},
		{ {.05, .05, .05, 1}, 0xFFFFFFFF},
		{ {-.05, .05, .05, 1}, 0xFFFFFFFF},

		{ {-.05, -.05, -.05, 1}, 0xFFFFFFFF},
		{ {.05, -.05, -.05, 1}, 0xFFFFFFFF},
		{ {.05, .05, -.05, 1}, 0xFFFFFFFF},
		{ {-.05, .05, -.05, 1}, 0xFFFFFFFF}
	};

	VERTEX_4D grid[40]
	{
		//Outline
		{{-.5, 0, -.5, 1}, 0xFFFFFFFF},
		{{.5, 0, -.5, 1}, 0xFFFFFFFF},
		{{.5, 0, .5, 1}, 0xFFFFFFFF},
		{{-.5, 0, .5, 1}, 0xFFFFFFFF},

		//Rows - Negative
		{{-.5, 0, -.1, 1}, 0xFFFFFFFF},
		{{.5, 0, -.1, 1}, 0xFFFFFFFF},
		{ {-.5, 0, -.2, 1}, 0xFFFFFFFF},
		{{.5, 0, -.2, 1}, 0xFFFFFFFF},
		{ {-.5, 0, -.3, 1}, 0xFFFFFFFF},
		{{.5, 0, -.3, 1}, 0xFFFFFFFF},
		{ {-.5, 0,-.4, 1}, 0xFFFFFFFF},
		{{.5, 0, -.4, 1}, 0xFFFFFFFF},

		//Rows - Positive
		{{-.5, 0, .1, 1}, 0xFFFFFFFF},
		{{.5, 0, .1, 1}, 0xFFFFFFFF},
		{ {-.5, 0, .2, 1}, 0xFFFFFFFF},
		{{.5, 0, .2, 1}, 0xFFFFFFFF},
		{ {-.5, 0, .3, 1}, 0xFFFFFFFF},
		{{.5, 0, .3, 1}, 0xFFFFFFFF},
		{ {-.5, 0, .4, 1}, 0xFFFFFFFF},
		{{.5, 0, .4, 1}, 0xFFFFFFFF},

		//Center - Row
		{{-.5, 0.0, 0, 1}, 0xFFFFFFFF},
		{{.5, 0.0, 0, 1}, 0xFFFFFFFF},
		//Center Column
		{{0, 0, -.5, 1}, 0xFFFFFFFF},
		{ {0, 0, .5, 1}, 0xFFFFFFFF},

		//Columns Negative
		{ {-.1, 0, -.5, 1}, 0xFFFFFFFF},
		{ {-.1, 0, .5, 1}, 0xFFFFFFFF},
		{ {-.2, 0, -.5, 1}, 0xFFFFFFFF},
		{ {-.2, 0, .5, 1}, 0xFFFFFFFF},
		{ {-.3, 0, -.5, 1}, 0xFFFFFFFF},
		{ {-.3, 0, .5, 1}, 0xFFFFFFFF},
		{ {-.4, 0, -.5, 1}, 0xFFFFFFFF},
		{ {-.4, 0, .5, 1}, 0xFFFFFFFF},

		//Columns Positive
		{ {.1, 0, -.5, 1}, 0xFFFFFFFF},
		{ {.1, 0, .5, 1}, 0xFFFFFFFF},
		{ {.2, 0, -.5, 1}, 0xFFFFFFFF},
		{ {.2, 0, .5, 1}, 0xFFFFFFFF},
		{ {.3, 0, -.5, 1}, 0xFFFFFFFF},
		{ {.3, 0, .5, 1}, 0xFFFFFFFF},
		{ {.4, 0, -.5, 1}, 0xFFFFFFFF},
		{ {.4, 0, .5, 1}, 0xFFFFFFFF}

	};

	double startTime = GetTickCount64();
	float degree = 0;
	float degreeIncrement = .01f;

	float fov = 90.0f;
	float fovMax = 115.0f;
	float fovMin = 90.0f;
	float fovIncrement = 0.005f;
	bool forward = true;

	bool texels = false;
	bool colors = false;
	bool wireframe = true;
	bool zoom = false;

	VEC_4D translation = { 0, .25, 0, 0 };
	VEC_4D sataliteTranslation = { .5, 0, 0, 0 };
	VEC_4D view_Translation = { 0, 0, -1, 1 };

	MATRIX_4D cubeMatrix = Matrix_Identity_4D();
	MATRIX_4D sataliteMatrix = MATRIX_4D();
	MATRIX_4D gridMatrix = Matrix_Identity_4D();

	while (RS_Update(Raster, NUM_PIXELS))
	{
		Clear_Z_Buffer();
		ClearScreen();

		if (GetAsyncKeyState('0') & 0x1)
		{
			ClearScreen();
		}

		if (GetAsyncKeyState('1') & 0x1)
		{
			colors = true;
			texels = false;
			wireframe = false;
			ClearScreen();

			degreeIncrement = 1.0f;
			fovIncrement = .1f;
		}

		if (GetAsyncKeyState('2') & 0x1)
		{
			texels = true;
			colors = false;
			wireframe = false;
			ClearScreen();

			degreeIncrement = 1.0f;
			fovIncrement = .1f;
		}

		if (GetAsyncKeyState('3') & 0x1)
		{
			wireframe = true;
			texels = false;
			colors = false;
			ClearScreen();

			degreeIncrement = .01f;
			fovIncrement = .005f;
		}

		if (GetTickCount() - startTime >= 200)
		{
			startTime = 0;
			if (degree > 360)
				degree = 0.0f;
			degree += degreeIncrement;
		}
		if (!forward && fov > fovMin)
		{
			fov -= fovIncrement;
			if (fov <= fovMin)
				forward = true;
		}
		if (forward && fov < fovMax)
		{
			fov += fovIncrement;
			if (fov >= fovMax)
				forward = false;
		}


		ClearScreen();

		SV_View = Matrix_to_Matrix_Multiply_4D(Translate(view_Translation), Matrix_Rotation_4D_X(-18));
		//SV_View = Fast_Inverse(SV_View);
		SV_View = Inverse_Matrix_4D(SV_View);
		SV_Perspective = Perspective_Projection_4D(fov, .1, 10, (RASTER_WIDTH / RASTER_HEIGHT));

		//Cube
		VertexShader = VS_World;
		cubeMatrix = Matrix_to_Matrix_Multiply_4D(Translate(translation), Matrix_Rotation_4D_Y(degree));

		SV_WorldMatrix = cubeMatrix;

		if (wireframe)
		{
			colors = false;
			texels = false;

			PixelShader = PS_ChangeColor;
			SP_Color = Green;

			//Front
			Parametric(tris[0], tris[1]);
			Parametric(tris[1], tris[2]);
			Parametric(tris[2], tris[3]);
			Parametric(tris[3], tris[0]);

			//Back
			Parametric(tris[4], tris[5]);
			Parametric(tris[5], tris[6]);
			Parametric(tris[6], tris[7]);
			Parametric(tris[7], tris[4]);

			//Left
			Parametric(tris[0], tris[4]);
			Parametric(tris[4], tris[7]);
			Parametric(tris[7], tris[3]);
			Parametric(tris[3], tris[0]);

			//Right
			Parametric(tris[1], tris[5]);
			Parametric(tris[5], tris[6]);
			Parametric(tris[6], tris[2]);
			Parametric(tris[2], tris[1]);

		}
		else
		{
			if (colors)
			{
				PixelShader = PS_ChangeColor;
				SP_Color = Green;
			}
			if (texels)
				PixelShader = PS_TreeOfLife;

			//Front - Left
			FillTriangle(tris[0], tris[1], tris[2]);
			//Front - Right
			FillTriangle(tris[0], tris[2], tris[3]);

			if (colors)
			{
				PixelShader = PS_ChangeColor;
				SP_Color = Red;
			}

			//Back - Left
			FillTriangle(tris[5], tris[4], tris[7]);
			//Back - Right
			FillTriangle(tris[5], tris[7], tris[6]);

			if (colors)
			{
				PixelShader = PS_ChangeColor;
				SP_Color = Purple;
			}

			//Left - front
			FillTriangle(tris[8], tris[9], tris[10]);
			//left - back
			FillTriangle(tris[8], tris[10], tris[11]);

			if (colors)
			{
				PixelShader = PS_ChangeColor;
				SP_Color = Blue;
			}

			//Right - front
			FillTriangle(tris[12], tris[13], tris[14]);
			//Right - Back
			FillTriangle(tris[12], tris[14], tris[15]);

			if (colors)
			{
				PixelShader = PS_ChangeColor;
				SP_Color = Fuchia;
			}

			//Top - Front
			FillTriangle(tris[16], tris[17], tris[18]);
			//Top - Back
			FillTriangle(tris[16], tris[18], tris[19]);

			if (colors)
			{
				PixelShader = PS_ChangeColor;
				SP_Color = Olive;
			}

			//Top - Front
			FillTriangle(tris[20], tris[21], tris[22]);
			//Top - Back
			FillTriangle(tris[20], tris[22], tris[23]);
		}

		//Cube Satalite
		VertexShader = VS_World;

		sataliteMatrix = Matrix_to_Matrix_Multiply_4D(Translate(sataliteTranslation), Matrix_Rotation_4D_X(degree));

		//sataliteMatrix = Matrix_to_Matrix_Multiply_4D(Matrix_Rotation_4D_X(degree), Translate(sataliteTranslation));
		sataliteMatrix = Matrix_to_Matrix_Multiply_4D(sataliteMatrix, cubeMatrix);

		SV_WorldMatrix = sataliteMatrix;

		PixelShader = PS_ChangeColor;
		SP_Color = Cyan;

		Parametric(satalite[0], satalite[1]);
		Parametric(satalite[1], satalite[2]);
		Parametric(satalite[2], satalite[3]);
		Parametric(satalite[3], satalite[0]);

		Parametric(satalite[4], satalite[5]);
		Parametric(satalite[5], satalite[6]);
		Parametric(satalite[6], satalite[7]);
		Parametric(satalite[7], satalite[4]);

		Parametric(satalite[0], satalite[4]);
		Parametric(satalite[1], satalite[5]);
		Parametric(satalite[2], satalite[6]);
		Parametric(satalite[3], satalite[7]);


		{
			//Grid
			VertexShader = VS_World;
			SV_WorldMatrix = gridMatrix;

			PixelShader = PS_ChangeColor;
			SP_Color = White;

			//Outline
			Parametric(grid[0], grid[1]);
			Parametric(grid[1], grid[2]);
			Parametric(grid[2], grid[3]);
			Parametric(grid[3], grid[0]);

			//Negative Rows
			Parametric(grid[4], grid[5]);
			Parametric(grid[6], grid[7]);
			Parametric(grid[8], grid[9]);
			Parametric(grid[10], grid[11]);

			//Positive Row
			Parametric(grid[12], grid[13]);
			Parametric(grid[14], grid[15]);
			Parametric(grid[16], grid[17]);
			Parametric(grid[18], grid[19]);

			//Center Lines
			Parametric(grid[20], grid[21]);
			Parametric(grid[22], grid[23]);

			//Collumns Negative
			Parametric(grid[24], grid[25]);
			Parametric(grid[26], grid[27]);
			Parametric(grid[28], grid[29]);
			Parametric(grid[30], grid[31]);

			//Collumn Positive
			Parametric(grid[32], grid[33]);
			Parametric(grid[34], grid[35]);
			Parametric(grid[36], grid[37]);
			Parametric(grid[38], grid[39]);
		}

	};

	RS_Shutdown();

}