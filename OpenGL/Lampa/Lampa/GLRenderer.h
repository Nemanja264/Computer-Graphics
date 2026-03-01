#pragma once
#include <corecrt_math_defines.h>
#define TORAD(x) x*M_PI/180
#define TODEG(x) x*180/M_PI

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawAxes();
	UINT LoadTexture(char* fileName);
	void DrawEnvCube(double a);
	void DrawCylinder(double r1, double r2, double h, int nSeg, int texMode, bool bIsOpen);

	void CalculatePosition();
	void ZoomView(bool zoomIn = true);
	void RotateView(double dPitch, double dYaw);

	void DrawLampBase();
	void DrawLampArm();
	void DrawLampHead();
	void DrawLamp();

	UINT lampa, bot, top, side;
	double pitch = 0.1, yaw = 0;
	double eyeX = 10, eyeY = 10, eyeZ = 10;
	double cX = 0, cY = 10, cZ = 0;
	double upX = 0, upY = 1, upZ = 0;
	double dist = 20;

	double baseRot = 45;
	double rotSec = -90;
	double rotHead = 0;

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
