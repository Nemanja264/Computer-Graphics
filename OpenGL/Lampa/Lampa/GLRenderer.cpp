#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"

//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
	CalculatePosition();
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);

	bot = LoadTexture("bot.jpg");
	top = LoadTexture("top.jpg");
	lampa = LoadTexture("lamp.jpg");
	side = LoadTexture("side.jpg");
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, cX, cY, cZ, upX, upY, upZ);

	glDisable(GL_LIGHTING);

	DrawAxes();

	glTranslated(0, 50, 0);
	DrawEnvCube(100);

	glTranslated(0, -50, 0);
	DrawLampBase();
	glTranslated(0, 1, 0);

	glRotated(baseRot, 0, 0, 1);
	DrawLampArm();

	glTranslated(0, 17, 0);
	glRotated(rotSec, 0, 0, 1);
	DrawLampArm();

	glTranslated(0, 20, -0.5);
	glRotated(rotHead, 0, 0, 1);
	DrawLampHead();

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (double)w / h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	glDeleteTextures(1, &bot);
	glDeleteTextures(1, &top);
	glDeleteTextures(1, &lampa);
	glDeleteTextures(1, &side);

	glEnable(GL_TEXTURE_2D);
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxes()
{
	glBegin(GL_LINES);
	glLineWidth(2);

	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 50);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(50, 0, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 50, 0);

	glEnd();
}

UINT CGLRenderer::LoadTexture(char* fileName)
{
	DImage img;
	img.Load(CString(fileName));
	UINT texID;
	glPixelStoref(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), 
		GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}

void CGLRenderer::DrawEnvCube(double a)
{
	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	double half = a / 2;
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

	glBindTexture(GL_TEXTURE_2D, side);
	glBegin(GL_QUADS);

	//front
	glTexCoord2d(0, 0); glVertex3f(-half, -half, half);
	glTexCoord2d(1, 0); glVertex3f(half, -half, half);
	glTexCoord2d(1, 1); glVertex3f(half, half, half);
	glTexCoord2d(0, 1); glVertex3f(-half, half, half);

	//back
	glTexCoord2d(0, 0); glVertex3f(half, -half, -half);
	glTexCoord2d(1, 0); glVertex3f(-half, -half, -half);
	glTexCoord2d(1, 1); glVertex3f(-half, half, -half);
	glTexCoord2d(0, 1); glVertex3f(half, half, -half);

	//right
	glTexCoord2d(0, 0); glVertex3f(half, -half, half);
	glTexCoord2d(1, 0); glVertex3f(half, -half, -half);
	glTexCoord2d(1, 1); glVertex3f(half, half, -half);
	glTexCoord2d(0, 1); glVertex3f(half, half, half);

	//left
	glTexCoord2d(0, 0); glVertex3f(-half, -half, -half);
	glTexCoord2d(1, 0); glVertex3f(-half, -half, half);
	glTexCoord2d(1, 1); glVertex3f(-half, half, half);
	glTexCoord2d(0, 1); glVertex3f(-half, half, -half);


	glEnd();

	//top
	glBindTexture(GL_TEXTURE_2D, top);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex3f(-half, half, half);
	glTexCoord2d(1, 0); glVertex3f(half, half, half);
	glTexCoord2d(1, 1); glVertex3f(half, half, -half);
	glTexCoord2d(0, 1); glVertex3f(-half, half, -half);
	
	glEnd();

	//bottom
	glBindTexture(GL_TEXTURE_2D, bot);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0); glVertex3f(-half, -half, -half);
	glTexCoord2d(1, 0); glVertex3f(half, -half, -half);
	glTexCoord2d(1, 1); glVertex3f(half, -half, half);
	glTexCoord2d(0, 1); glVertex3f(-half, -half, half);

	glEnd();

	glPopAttrib();
}

void CGLRenderer::DrawCylinder(double r1, double r2, double h, int nSeg, int texMode, bool bIsOpen)
{
	double step = 2 * M_PI / nSeg;
	glEnable(GL_TEXTURE_2D);

	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, lampa);
	glBegin(GL_QUAD_STRIP);

	double v0, v1;
	if (texMode == 1)
	{
		v0 = 0;
		v1 = 0.5;
	}
	else
	{
		v0 = 0.5;
		v1 = 1;
	}

	for (int i = 0; i <= nSeg; i++)
	{
		double angle = i * step;
		double x = cos(angle);
		double z = sin(angle);

		double u = (double)i / nSeg;

		glTexCoord2d(u, v0);
		glVertex3f(r1 * x, 0, r1 * z);
		glTexCoord2d(u, v1);
		glVertex3f(r2 * x, h, r2 * z);
	}
	glEnd();

	if (!bIsOpen)
	{
		glBindTexture(GL_TEXTURE_2D, lampa);
		glBegin(GL_TRIANGLE_FAN);
		glTexCoord2d(0.5, 0.25);
		glVertex3d(0, 0, 0);
		for (int i = 0; i <= nSeg; i++)
		{
			double angle = i * step;
			double x = r1 * cos(angle);
			double z = r1 * sin(angle);

			double u = (x / r1 + 1) * 0.5;
			double v = (z / r1 + 1) * 0.25;
			glTexCoord2d(u, v);
			glVertex3f(x, 0, z);
		}
		glEnd();


		glBindTexture(GL_TEXTURE_2D, lampa);
		glBegin(GL_TRIANGLE_FAN);
		glTexCoord2d(0.5, 0.25);
		glVertex3d(0, h, 0);
		for (int i = 0; i <= nSeg; i++)
		{
			double angle = i * step;
			double x = r2 * cos(angle);
			double z = r2 * sin(angle);

			double u = (x / r2 + 1) * 0.5;
			double v = (z / r2 + 1) * 0.25;

			glTexCoord2d(u, v);
			glVertex3f(x, h, z);
		}
		glEnd();
	}
}

void CGLRenderer::CalculatePosition()
{
	double a = pitch;
	double b = yaw;

	eyeX = cX + dist * cos(a) * cos(b);
	eyeY = cY + dist * sin(a);
	eyeZ = cZ + dist * cos(a) * sin(b);

	upX = 0; upY = 1; upZ = 0;
}

void CGLRenderer::ZoomView(bool zoomIn)
{
	double sensitivity = 3;

	dist += zoomIn ? -sensitivity : sensitivity;

	dist = max(5, dist);
	dist = min(100, dist);

	CalculatePosition();
}

void CGLRenderer::RotateView(double dPitch, double dYaw)
{
	pitch += dPitch;
	yaw += dYaw;

	pitch = max(0.1, pitch);
	pitch = min(1.5, pitch);

	CalculatePosition();
}


void CGLRenderer::DrawLampBase()
{
	DrawCylinder(8, 7, 2, 50, 0, false);
}

void CGLRenderer::DrawLampArm()
{
	glPushMatrix();
	{
		glTranslated(0, 0, -1);
		glRotated(90, 1, 0, 0);
		DrawCylinder(3, 3, 2, 50, 0, false);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslated(0, 2.5, 0);
		DrawCylinder(1, 1, 16, 50, 0, false);
	}
	glPopMatrix();
}

void CGLRenderer::DrawLampHead()
{
	glPushMatrix();
	{
		glRotated(90, 1, 0, 0);
		DrawCylinder(2, 2, 1, 50, 0, false);
	}
	glPopMatrix();
	
	glTranslated(-4, 3, 0);
	glRotated(-90, 0, 0, 1);

	DrawCylinder(2, 3, 2, 50, 0, false);
	
	glTranslated(0, 2, 0);

	DrawCylinder(3, 3, 5, 50, 0, false);
	
	glTranslated(0, 5, 0);
	DrawCylinder(3, 6, 5, 50, 0, true);
	
}

void CGLRenderer::DrawLamp()
{
	glPushMatrix();

	glPopMatrix();
}