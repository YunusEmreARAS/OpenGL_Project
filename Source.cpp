#include <iostream>
#include <fstream>
#include <vector>

#include <glut.h>

using namespace std;

float dist = -360, angle = 0.0, _angle = 360;
int dataArray[17][21];
bool ish = false;
bool flag = true;
void read();

void keyboard(unsigned char key, int x, int y)
{
	if (key == 27)  // exit when user hits <esc>
		exit(EXIT_SUCCESS);

	if (key == 'h')
	{
		ish = !ish;
	}

	glutPostRedisplay();
}

void special(int key, int x, int y)
{
	if (key == GLUT_KEY_RIGHT)
		angle = angle - 3;

	else if (key == GLUT_KEY_LEFT)
		angle = angle + 3;

	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (double)w / (double)h, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	float xcam, zcam;
	xcam = dist * cos(angle / 180.0 * 3);
	zcam = dist * sin(angle / 180.0 * 3);
	glLoadIdentity();
	gluLookAt(xcam, 500, zcam, 0, 0, 0, 0.0, 1.0, 0.0);
	glutPostRedisplay();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create light components.
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat position[] = { 0.0f, 0.0f, _angle, 1.0f };

	// Assign created components to GL_LIGHT0.
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	if (flag) {
		_angle -= 0.1;
		if (_angle <= -360) {
			_angle = -360;
			flag = false;
		}
	}
	else {
		_angle += 0.1;
		if (_angle >= 360) {
			_angle = 360;
			flag = true;
		}
	}

	for (int z = 16; z > 0; z--)
	{
		for (int x = 0; x < 20; x++)
		{
			//encolouring
			int avgh = (dataArray[z][x] + dataArray[z][x + 1] + dataArray[z - 1][x]) / 3.0;
			if (avgh < 50 && avgh > 0)
				glColor3f(0.0, 1.0, 0.0);
			else if (avgh >= 50 && avgh < 80.0)
				glColor3f(1.0, 1.0, 0.0);
			else if (avgh >= 80)
				glColor3f(1.0, 0.0, 0.0);

			int avgh1 = (dataArray[z - 1][x] + dataArray[z][x + 1] + dataArray[z - 1][x + 1]) / 3.0;
			glPolygonMode(GL_FRONT, GL_FILL);
			if (avgh1 < 50 && avgh1 > 0)
				glColor3f(0.0, 1.0, 0.0);
			else if (avgh1 >= 50 && avgh1 < 80.0)
				glColor3f(1.0, 1.0, 0.0);
			else if (avgh1 >= 80)
				glColor3f(1.0, 0.0, 0.0);

			if (ish)
			{
				glColor3f(1.0, 1.0, 1.0);
				glPolygonMode(GL_FRONT, GL_LINE);
			}

			//drawing triangles
			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(x * 20 - 21 * 9, dataArray[z][x], z * 20 - 17 * 10);
			glVertex3f((x + 1) * 20 - 21 * 9, dataArray[z][x + 1], z * 20 - 17 * 10);
			glVertex3f(x * 20 - 21 * 9, dataArray[z - 1][x], (z - 1) * 20 - 17 * 10);
			glEnd();

			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(x * 20 - 21 * 9, dataArray[z - 1][x], (z - 1) * 20 - 17 * 10);
			glVertex3f((x + 1) * 20 - 21 * 9, dataArray[z][x + 1], z * 20 - 17 * 10);
			glVertex3f((x + 1) * 20 - 21 * 9, dataArray[z - 1][x + 1], (z - 1) * 20 - 17 * 10);
			glEnd();
		}
	}

	glutSwapBuffers();
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1); //Enable light #1
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

int main(int argc, char *argv[])
{
	read();

	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	init();
	glMatrixMode(GL_PROJECTION);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return EXIT_SUCCESS;
}

void read()
{
	ifstream data("data.txt");
	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < 21; j++)
			data >> dataArray[i][j];
	}
}