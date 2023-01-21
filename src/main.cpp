#include "Shader.h"
#include "FastNoiseLite.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/constants.hpp>


glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

int Scene = 0;

Shader renderShader;

float width = 1024.0f;
float height = 768.0f;

float x,y;

const float PI = glm::pi<float>();

int lastX = width/2;
int lastY = height/2;

int curX = lastX;
int curY = lastY;

float gtime = 0.0f;

glm::vec3 CamPosition;
glm::mat4 viewMatrix;

const float MOVE_SPEED = 1.0f;
const float MOUSE_SENSITIVITY = 0.1f;
const float PITCH_LIMIT = 1.4F;

GLuint vao, vbo;

void Init(void);
void Display(void);
void Finalize(void);
void Idle();
void move(int value);
void Keyboard(unsigned char, int x, int y);
void mouseMove(int mouseX, int mouseY);
void updateCameraVectors(glm::vec2 CamPosition);
void timer(int value);


void Init(void)
{
	
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(width, height);

    glutCreateWindow("Test");
    glewExperimental = GL_TRUE;
    if (glewInit()) {
        printf("Unable to initialize GLEW ... exiting\n");
    }
	
	glEnable(GL_DEPTH_TEST);

	glutSetCursor(GLUT_CURSOR_NONE);


	GLfloat vertices[] = {      -1.0, -1.0 ,
                            1.0, -1.0 ,
                            -1.0,  1.0 ,
						    -1.0,  1.0 ,                                         
                            1.0, -1.0 ,
						    1.0, 1.0 ,
						   
						   };

	switch(Scene) {
		case 0:
			renderShader.loadShaders("src/VertexShader.vs", "src/InfiniteSphereShader.fs");
			break;
		case 1:
			renderShader.loadShaders("src/VertexShader.vs", "src/SingleSphereShader.fs");
			break;
		case 2:
			renderShader.loadShaders("src/VertexShader.vs", "src/TorusShader.fs");
			break;
		default :
			renderShader.loadShaders("src/VertexShader.vs", "src/InfiniteSphereShader.fs");
			break;
	}


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glm::vec2 iResolution = glm::vec2(width, height);

	renderShader.use();
	
	renderShader.setUniform("uTime", gtime);
	renderShader.setUniform("iResolution", iResolution);
	renderShader.setUniform("viewMatrix", viewMatrix);


    glDrawArrays(GL_TRIANGLES, 0, 6);

    glutSwapBuffers();	
}

void timer(int value)
{
	gtime += 25;
	glutTimerFunc(25, timer, 1);
}

void move(int value)
{

	glutPostRedisplay();
	glutTimerFunc(25, move, 1);
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:
			exit(0);
	}
}

void Idle()
{
	glutPostRedisplay();
}

void Finalize(void)
{

}

void mouseMove(int x, int y) 
{
	
	int deltaX = x - lastX;
	int deltaY = y - lastY;



	lastX = x;
	lastY = y;


	if(deltaX == 0 && deltaY == 0) return;

	if(x >= width-10 || x <= 10 || y >= height-10 || y < 10)
	{
		lastX = width/2;
		lastY = height/2;

		deltaX = 0;
		deltaY = 0;

		glutWarpPointer(width/2, height/2);
	}


	curX += deltaX;
	curY += deltaY;

	glm::vec2 mPosition = glm::vec2(glm::radians((float)(curX) * MOUSE_SENSITIVITY), glm::radians((float)(-curY) * MOUSE_SENSITIVITY));

	updateCameraVectors(mPosition);
}

void updateCameraVectors(glm::vec2 mPosition)
{
    glm::vec3 mTargetPos = glm::vec3(0.0, 0.0, 0.0);
    float mRadius = 5.0;

	glm::vec3 mUp = glm::vec3(0.0, 1.0, 0.0);

    float mYaw = mPosition.x;
    float mPitch = mPosition.y;

	if (mPitch < -PITCH_LIMIT)
	{
		mPitch = -PITCH_LIMIT;
	}
	else if (mPitch > PITCH_LIMIT)
	{
		mPitch = PITCH_LIMIT;
	}

	glm::vec3 CamPos;

    CamPos.x = mTargetPos.x + mRadius * cos(mPitch) * sin(mYaw);
	CamPos.y = mTargetPos.y + mRadius * sin(mPitch);
	CamPos.z = mTargetPos.z + mRadius * cos(mPitch) * cos(mYaw);

	CamPosition = CamPos;

	viewMatrix = glm::lookAt(CamPosition, mTargetPos, mUp);

}

int main(int argc, char** argv)
{
	int argValue = 0;
	char** args = 0;
	int x;

	if(argc == 2) {
		string arg = argv[1];
		try {
			size_t pos;
			x = stoi(arg, &pos);
			if (pos < arg.size()) {
				std::cerr << "Trailing characters after number" << endl;
			}
		} catch (const std::invalid_argument& e) {
			std::cerr << "Invalid number: " << arg << endl;
			x = 0;
		} catch (const std::out_of_range& e) {
			std::cerr << "Number out of range: " << arg << endl;
			x = 0;
		}
	} else if (argc > 2){
		std::cerr << "Too many args" << endl;
		x = 0;
	} else if (argc == 1) {
		x = 0;
	} else {
		std::cerr << "Invalid number of args" << endl;
		x = 0;
	}

	if(0 > x > 2) {
		std::cerr << "Number out of range" << endl;
		x = 0;
	}

	Scene = x;
	cout << "Scene rendering: " << Scene << endl;

    glutInit(&argValue, args);
    Init();
	
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(25, move, 1);
	glutTimerFunc(25, timer, 1);
	glutPassiveMotionFunc(mouseMove);
	glutMainLoop();
	
	return 0;	
}