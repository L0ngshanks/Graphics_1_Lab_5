#include "Rasterizer.h"
#include "RasterSurface.h"

int main(int argc, char** argv)
{
	RS_Initialize(RASTER_WIDTH, RASTER_HEIGHT);

	VERTEX_4D square[8]
	{
		{ {-.25, -.25, .25, 1}, 0xFFFFFFFF},
		{ {.25, -.25, .25, 1}, 0xFFFFFFFF},
		{ {.25, .25, .25, 1}, 0xFFFFFFFF},
		{ {-.25, .25, .25, 1}, 0xFFFFFFFF},

		{ {-.25, -.25, -.25, 1}, 0xFFFFFFFF},
		{ {.25, -.25, -.25, 1}, 0xFFFFFFFF},
		{ {.25, .25, -.25, 1}, 0xFFFFFFFF},
		{ {-.25, .25, -.25, 1}, 0xFFFFFFFF}
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
		//Center Collumn
		{{0, 0, -.5, 1}, 0xFFFFFFFF},
		{ {0, 0, .5, 1}, 0xFFFFFFFF},

		//Collumns Negative
		{ {-.1, 0, -.5, 1}, 0xFFFFFFFF},
		{ {-.1, 0, .5, 1}, 0xFFFFFFFF},
		{ {-.2, 0, -.5, 1}, 0xFFFFFFFF},
		{ {-.2, 0, .5, 1}, 0xFFFFFFFF},
		{ {-.3, 0, -.5, 1}, 0xFFFFFFFF},
		{ {-.3, 0, .5, 1}, 0xFFFFFFFF},
		{ {-.4, 0, -.5, 1}, 0xFFFFFFFF},
		{ {-.4, 0, .5, 1}, 0xFFFFFFFF},

		//Collumns Positive
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
	float degreeIncrement = 0.075f;

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
		if (GetTickCount() - startTime >= 200)
		{
			startTime = 0;
			if (degree > 360)
				degree = 0.0f;
			degree += degreeIncrement;
		}

		if (!forward && fov > fovMin)
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

		PixelShader = PS_ChangeColor;
		SP_Color = Green;

		Parametric(square[0], square[1]);
		Parametric(square[1], square[2]);
		Parametric(square[2], square[3]);
		Parametric(square[3], square[0]);

		Parametric(square[4], square[5]);
		Parametric(square[5], square[6]);
		Parametric(square[6], square[7]);
		Parametric(square[7], square[4]);

		Parametric(square[0], square[4]);
		Parametric(square[1], square[5]);
		Parametric(square[2], square[6]);
		Parametric(square[3], square[7]);

		//Cube Satalite
		VertexShader = VS_World;

		sataliteMatrix = Matrix_to_Matrix_Multiply_4D(Translate(sataliteTranslation), Matrix_Rotation_4D_X(degree));
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