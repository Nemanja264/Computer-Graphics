#pragma once

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
	void CalculatePosition();
	void ZoomView(bool zoomIn = true);
	void RotateView(double dPitch, double dYaw);
	void DrawEnvCube(double a);
	UINT LoadTexture(char* fileName);
	void DrawSphere(double r, int nSeg, double texU, double texV, double texR);
	void DrawCone(double r, double h, int nSeg, double texU, double texV, double texR);
	void DrawLegSegment(double r, double h, int nSeg);
	void DrawLeg();
	void DrawSpider();
	void DrawSpiderBody();

public:
	double eyeX=0, eyeY=0, eyeZ=0;
	double upx=0, upy=1, upz=0;
	double cx=0, cy=10, cz=0;
	double dist = 20;
	double pitch = 0, yaw = 0;
	UINT front, bot, back, top, left, right, spider;

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
