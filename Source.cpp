#include "Rasterizer.h"
#include "RasterSurface.h"

int main(int argc, char** argv)
{
	std::cout << Min_3(4, 5, 6) << " : " << Max_3(4, 5, 6) << std::endl;

	RS_Initialize(RASTER_WIDTH, RASTER_HEIGHT);

	VERTEX_4D tris[8]
	{
		{ {-.25, -.25, .25, 1}, 0xFFFFFFFF}, //a
		{ {.25, -.25, .25, 1}, 0xFFFFFFFF}, //b
		{ {.25, .25, .25, 1}, 0xFFFFFFFF}, //c
		{ {-.25, .25, .25, 1}, 0xFFFFFFFF}, //d

		{ {-.25, -.25, -.25, 1}, 0xFFFFFFFF}, //e
		{ {.25, -.25, -.25, 1}, 0xFFFFFFFF}, //f
		{ {.25, .25, -.25, 1}, 0xFFFFFFFF}, //g
		{ {-.25, .25, -.25, 1}, 0xFFFFFFFF} //h
	};

	//VERTEX_4D satalite[8]
	//{
	//	{ {-.05, -.05, .05, 1}, 0xFFFFFFFF},
	//	{ {.05, -.05, .05, 1}, 0xFFFFFFFF},
	//	{ {.05, .05, .05, 1}, 0xFFFFFFFF},
	//	{ {-.05, .05, .05, 1}, 0xFFFFFFFF},

	//	{ {-.05, -.05, -.05, 1}, 0xFFFFFFFF},
	//	{ {.05, -.05, -.05, 1}, 0xFFFFFFFF},
	//	{ {.05, .05, -.05, 1}, 0xFFFFFFFF},
	//	{ {-.05, .05, -.05, 1}, 0xFFFFFFFF}
	//};

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
	float degreeIncrement = 1.0f;

	float fov = 90.0f;
	float fovMax = 115.0f;
	float fovMin = 90.0f;
	float fovIncrement = 0.05f;
	bool forward = true;


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

		if (GetTickCount() - startTime >= 0)
		{
			startTime = 0;
			if (degree > 360)
				degree = 0.0f;
			degree += degreeIncrement;
		}

		/*if (!forward && fov > fovMin)
		{
			fov -= .015f;
			if (fov <= fovMin)
				forward = true;
		}
		if (forward && fov < fovMax)
		{
			fov += .015f;
			if (fov >= fovMax)
				forward = false;
		}*/

		ClearScreen();

		SV_View = Matrix_to_Matrix_Multiply_4D(Translate(view_Translation), Matrix_Rotation_4D_X(-18));
		//SV_View = Fast_Inverse(SV_View);
		SV_View = Inverse_Matrix_4D(SV_View);
		SV_Perspective = Perspective_Projection_4D(fov, .1, 10, (RASTER_WIDTH / RASTER_HEIGHT));

		//Cube
		VertexShader = VS_World;
		cubeMatrix = Matrix_to_Matrix_Multiply_4D(Translate(translation), Matrix_Rotation_4D_Y(degree));

		SV_WorldMatrix = cubeMatrix;

		PixelShader = PS_ChangeColor;
		SP_Color = Green;

		//Front - Left
		FillTriangle(tris[0], tris[1], tris[2]);
		//Front - Right
		FillTriangle(tris[0], tris[2], tris[3]);

		PixelShader = PS_ChangeColor;
		SP_Color = Red;

		//Back - Left
		FillTriangle(tris[4], tris[5], tris[6]);
		//Back - Right
		FillTriangle(tris[4], tris[6], tris[7]);

		PixelShader = PS_ChangeColor;
		SP_Color = Purple;

		//Left - front
		FillTriangle(tris[4], tris[0], tris[3]);
		//left - back
		FillTriangle(tris[4], tris[7], tris[3]);

		PixelShader = PS_ChangeColor;
		SP_Color = Blue;

		//Right - front
		FillTriangle(tris[1], tris[5], tris[6]);
		//Right - Back
		FillTriangle(tris[1], tris[2], tris[6]);

		PixelShader = PS_ChangeColor;
		SP_Color = Fuchia;

		//Top - Front
		FillTriangle(tris[3], tris[2], tris[6]);
		//Top - Back
		FillTriangle(tris[3], tris[7], tris[6]);

		PixelShader = PS_ChangeColor;
		SP_Color = Olive;

		//Top - Front
		FillTriangle(tris[0], tris[1], tris[5]);
		//Top - Back
		FillTriangle(tris[0], tris[4], tris[5]);

		//Cube Satalite
		VertexShader = VS_World;

		//sataliteMatrix = Matrix_to_Matrix_Multiply_4D(Translate(sataliteTranslation), Matrix_Rotation_4D_X(degree));
		//sataliteMatrix = Matrix_to_Matrix_Multiply_4D(sataliteMatrix, cubeMatrix);

		//SV_WorldMatrix = sataliteMatrix;

		//PixelShader = PS_ChangeColor;
		//SP_Color = Cyan;

		//Parametric(satalite[0], satalite[1]);
		//Parametric(satalite[1], satalite[2]);
		//Parametric(satalite[2], satalite[3]);
		//Parametric(satalite[3], satalite[0]);

		//Parametric(satalite[4], satalite[5]);
		//Parametric(satalite[5], satalite[6]);
		//Parametric(satalite[6], satalite[7]);
		//Parametric(satalite[7], satalite[4]);

		//Parametric(satalite[0], satalite[4]);
		//Parametric(satalite[1], satalite[5]);
		//Parametric(satalite[2], satalite[6]);
		//Parametric(satalite[3], satalite[7]);


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

	};

	RS_Shutdown();

}