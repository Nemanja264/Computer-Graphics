#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
#include <corecrt_math_defines.h>
#define TORAD(x) x*M_PI/180.00
#define TODEG(x) x*180.00/M_PI

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
	right = LoadTexture("right.jpg");
	left = LoadTexture("left.jpg");
	front = LoadTexture("front.jpg");
	back = LoadTexture("back.jpg");
	spider = LoadTexture("spider.png");

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, cx, cy, cz, upx, upy, upz);

	glDisable(GL_LIGHTING);
	
	DrawAxes();

	glTranslated(0, 50, 0);

	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	DrawEnvCube(100);

	glPopAttrib();

	glTranslated(0, -46, 0);
	DrawSpider();

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (double)w / (double)h, 0.1, 2000);
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
	glDeleteTextures(1, &right);
	glDeleteTextures(1, &left);
	glDeleteTextures(1, &front);
	glDeleteTextures(1, &spider);

	glEnable(GL_TEXTURE_2D);
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

UINT CGLRenderer::LoadTexture(char* fileName)
{
	DImage img;
	UINT texID;
	img.Load(CString(fileName));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(),
		GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
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

void CGLRenderer::CalculatePosition()
{
	double a = TORAD(pitch);
	double b = TORAD(yaw);

	eyeX = cx + dist * cos(a) * cos(b);
	eyeY = cy + dist * sin(a);
	eyeZ = cz + dist * cos(a) * sin(b);

	upx = 0; upy = 1; upz = 0;
}

void CGLRenderer::ZoomView(bool zoomIn)
{
	int sensitivity = 3;

	dist += zoomIn ? -sensitivity : sensitivity;

	dist = min(50, dist);
	dist = max(8, dist);

	CalculatePosition();
}

void CGLRenderer::RotateView(double dPitch, double dYaw)
{
	pitch += dPitch;
	yaw += dYaw;

	//double t = (-50 - cy / dist);
	double t = (- cy / dist);

	t = max(-1, t);
	t = min(1, t);

	double minPitch = TODEG(asin(t));

	pitch = max(pitch, minPitch);
	pitch = min(90, pitch);

	CalculatePosition();
}

void CGLRenderer::DrawEnvCube(double a)
{
	double half = a / 2;
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, right);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0); 
	glVertex3f(half, -half, half);
	glTexCoord2f(1, 0);
	glVertex3f(half, -half, -half);
	glTexCoord2f(1, 1);
	glVertex3f(half, half, -half);
	glTexCoord2f(0, 1);
	glVertex3f(half, half, half);

	glEnd();


	glBindTexture(GL_TEXTURE_2D, left);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex3f(-half, -half, -half);
	glTexCoord2f(1, 0);
	glVertex3f(-half, -half, half);
	glTexCoord2f(1, 1);
	glVertex3f(-half, half, half);
	glTexCoord2f(0, 1);
	glVertex3f(-half, half, -half);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, front);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex3f(-half, -half, half);
	glTexCoord2f(1, 0);
	glVertex3f(half, -half, half);
	glTexCoord2f(1, 1);
	glVertex3f(half, half, half);
	glTexCoord2f(0, 1);
	glVertex3f(-half, half, half);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, back);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex3f(half, -half, -half);
	glTexCoord2f(1, 0);
	glVertex3f(-half, -half, -half);
	glTexCoord2f(1, 1);
	glVertex3f(-half, half, -half);
	glTexCoord2f(0, 1);
	glVertex3f(half, half, -half);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, top);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex3f(-half, half, half);
	glTexCoord2f(1, 0);
	glVertex3f(half, half, half);
	glTexCoord2f(1, 1);
	glVertex3f(half, half, -half);
	glTexCoord2f(0, 1);
	glVertex3f(-half, half, -half);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, bot);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex3f(-half, -half, -half);
	glTexCoord2f(1, 0);
	glVertex3f(half, -half, -half);
	glTexCoord2f(1, 1);
	glVertex3f(half, -half, half);
	glTexCoord2f(0, 1);
	glVertex3f(-half, -half, half);

	glEnd();
}

void CGLRenderer::DrawSphere(double r, int nSeg, double texU,double texV, double texR)
{
	double alpha = M_PI / nSeg;
	double beta = 2 * M_PI / (2 * nSeg);

	glColor3f(1, 1, 1);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, spider);
	for (int i = 0; i < nSeg; i++)
	{
		double a0 = -M_PI/2.0 + alpha * i;
		double a1 = a0 + alpha;

		glBegin(GL_QUAD_STRIP);
		
		for (int j = 0; j <= 2*nSeg; j++)
		{
			double b = beta * j;

			double x0 = r * cos(a0) * cos(b);
			double y0 = r * sin(a0);
			double z0 = r * cos(a0) * sin(b);

			double x1 = r * cos(a1) * cos(b);
			double y1 = r * sin(a1) ;
			double z1 = r * cos(a1) * sin(b);

			double tx0 = texR * (x0 / r) + texU;
			double ty0 = texR * (z0 / r) + texV;

			double tx1 = texR * (x1 / r) + texU;
			double ty1 = texR * (z1 / r) + texV;

			glTexCoord2d(tx0, ty0);
			glVertex3d(x0, y0, z0);

			glTexCoord2d(tx1, ty1);
			glVertex3d(x1, y1, z1);

		}
		glEnd();
	}
}

void CGLRenderer::DrawCone(double r, double h, int nSeg, double texU, double texV, double texR)
{
	double alpha = 2 * M_PI / nSeg;

	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, spider);

	glBegin(GL_TRIANGLE_FAN);
	

	glTexCoord2d(texU, texV);
	glVertex3f(0, h, 0);

	for (int i = 0; i <= nSeg; i++)
	{
		double angle = i * alpha;

		double x = r * cos(angle);
		double z = r * sin(angle);

		double u = texU + texR * x / r;
		double v = texU + texR * z / r;
		glTexCoord2d(u, v);
		glVertex3f(x, 0, z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2d(texU, texV);
	glVertex3f(0, 0, 0);

	for (int i = 0; i <= nSeg; i++)
	{
		double angle = i * alpha;

		double x = r * cos(angle);
		double z = r * sin(angle);

		double u = texU + texR * x / r;
		double v = texU + texR * z / r;
		glTexCoord2d(u, v);
		glVertex3f(x, 0, z);
	}

	glEnd();
}

void CGLRenderer::DrawLegSegment(double r, double h, int nSeg)
{
	DrawSphere(r, 2 * nSeg, 0.25, 0.25, 0.24);
	DrawCone(r, h, nSeg, 0.75, 0.75, 0.24);
}

void CGLRenderer::DrawLeg()
{
	glPushMatrix();
	{
		DrawLegSegment(1, 10, 5);

		glTranslated(0, 10, 0);
		glRotated(85, 1, 0, 0);
		DrawLegSegment(1, 15, 5);
	}
	glPopMatrix();
}

void CGLRenderer::DrawSpiderBody()
{
	glPushMatrix();
	glScaled(1, 0.5, 1);
	DrawSphere(2, 10, 0.75, 0.25, 0.24);
	glPopMatrix();

	glTranslated(4, 0, 0);
	glPushMatrix();
	glScaled(1, 0.5, 1);
	DrawSphere(3, 10, 0.25, 0.25, 0.24);
	glPopMatrix();

	glTranslated(6, 0, 0);
	glPushMatrix();
	glScaled(1, 0.8, 1);
	DrawSphere(5, 10, 0.25, 0.25, 0.24);
	glPopMatrix();
}

void CGLRenderer::DrawSpider()
{
	DrawSpiderBody();
	glTranslated(-6, 0, 0);
	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		{
			glRotatef(i * 30 - 45, 0.0, 1.0, 0.0);
			glRotatef(45, 1.0, 0.0, 0.0);
			DrawLeg();
		}
		glPopMatrix();
	}

	glRotated(180, 0, 1, 0);
	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		{
			glRotatef(i * 30 - 45, 0.0, 1.0, 0.0);
			glRotatef(45, 1.0, 0.0, 0.0);
			DrawLeg();
		}
		glPopMatrix();
	}
}