#pragma once
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glew32mx.lib")

#include "Angel.h"
#include "mat.h"
#include "constraint.h"

#include <iostream>
#include <vector>
#include <GL/freeglut.h>
#include <GL/glew.h>

using namespace std;

#define w_len 500

GLuint program;
GLuint vShaderID;
GLuint fShaderID;

// Vertex Array & Buufer
// Triangle
GLuint VAID_triangle;
GLuint VBID_triangle;
// Point
GLuint VAID_point;
GLuint VBID_point;

vec4 vertices[3] = { 
	vec4(2.0, 0.0, 0.0, 1.0), 
	vec4(-2.0, 0.0, 0.0, 1.0), 
	vec4(0.0, 4.0, 0.0, 1.0) 
};
vector<vec4> tri_points;
vector<vec4> tri_normals;

// Rigid body
vec4 point = vec4(-30.0, 0.0, 0.0, 1.0);
vec4 tri_point = vec4(100.0, 30.0, -20.0, 1.0);
vec4 vp = vec4(0.0, 0.0, 0.0, 0.0);
vec4 vt = vec4(0.01, 0.01, 0.01, 0.0);
vec4 post_point, vp2, post_tri_point, vt2;

//mat4 tri_pos = Scale(0.0,0.0,0.0);
mat4 tri_pos = RotateY(0.0);


void setView()
{
	vec4 eye (0.0, 0.0, 100.0, 0.0);
	vec4 at ( 0.0, 0.0, -1.0, 0.0 );
	vec4 up ( 0.0, 1.0, 0.0, 0.0 );

	mat4 viewMat = Angel::LookAt( eye, at, up );
	glUniformMatrix4fv( glGetUniformLocation( program, "mView" ),
		1, GL_TRUE, viewMat );
}

void setProjection()
{
	mat4 projectionMat = Angel::Perspective( 80.0f, 1.0, 0.1f, 200.0f );
	glUniformMatrix4fv( glGetUniformLocation( program, "mProjection" ),
		1, GLU_TRUE, projectionMat );
}

void setModel()
{
	// Local to World
	mat4 modelMat = Angel::Scale( 1.0, 1.0, 1.0 );
	glUniformMatrix4fv( glGetUniformLocation( program, "mModel" ),
		1, GL_TRUE, modelMat );
}

void setColor( float r, float g, float b )
{
	vec4 color (r, g, b, 1.0);
	glUniform4fv(glGetUniformLocation(program,"color"),1,color);
}


void InitProgram()
{
	program = InitShader( "vshader0.glsl", "fshader0.glsl", &vShaderID, &fShaderID );
	glUseProgram( program );

	/*** Initialize triangle ***/
	// make triangle
	vec4 u = vertices[2] - vertices[0];
	vec4 v = vertices[1] - vertices[0];
	vec3 normal = normalize(cross(u,v));

	tri_normals.push_back(vec4(normal, 0)); tri_points.push_back(vertices[0]);
	tri_normals.push_back(vec4(normal, 0)); tri_points.push_back(vertices[1]);
	tri_normals.push_back(vec4(normal, 0)); tri_points.push_back(vertices[2]);

	glGenVertexArrays(1, &VAID_triangle);
	glBindVertexArray(VAID_triangle);

	glGenBuffers(1, &VBID_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, VBID_triangle);

	glBufferData(GL_ARRAY_BUFFER,sizeof(vec4)*tri_points.size()*2,0,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,tri_points.size()*sizeof(vec4),tri_points.data());
	glBufferSubData(GL_ARRAY_BUFFER,tri_points.size()*sizeof(vec4),tri_normals.size()*sizeof(vec4), tri_normals.data());

	GLuint vPosition = glGetAttribLocation(program,"vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition,4,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program,"vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal,4,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(tri_points.size()*sizeof(vec4)));
	/************************/

	/*** Initialize point ***/
	glGenVertexArrays(1, &VAID_point);
	glBindVertexArray(VAID_point);

	glGenBuffers(1, &VBID_point);
	glBindBuffer(GL_ARRAY_BUFFER, VBID_point);

	glBufferData(GL_ARRAY_BUFFER,sizeof(vec4),0,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vec4),point);

	vPosition = glGetAttribLocation(program,"vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition,4,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));

	vNormal = glGetAttribLocation(program,"vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal,4,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(sizeof(vec4)));
	/************************/

	// set view, projection, model
	setView();
	setProjection();
	setModel();
}

void display()
{
	glClearColor (0.1, 0.1, 0.1, 1.0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glUseProgram(program);

	/*** draw a point ***/ // -9.2 < x < 9.2
	setColor(1.0, 0.0, 0.0);
	mat4 Transformation = Translate(0.0, 0.0, 0.0);
	glUniformMatrix4fv( glGetUniformLocation( program, "mModel" ),
		1, GL_TRUE, Transformation );
	glBindVertexArray(VAID_point);
	glBindBuffer(GL_ARRAY_BUFFER, VBID_point);
	glDrawArrays(GL_POINTS, 0, 1);
	/*********************/

	/*** draw triangle ***/
	setColor( 1.0, 1.0, 0.0 );
	tri_pos = Translate( tri_point.x, tri_point.y, tri_point.z );
	glUniformMatrix4fv( glGetUniformLocation( program, "mModel" ),
		1, GL_TRUE, tri_pos );
	glBindVertexArray(VAID_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, VBID_triangle);
	glDrawArrays(GL_TRIANGLES, 0, tri_points.size());
	/*********************/


	// draw & swap buffers
	glutSwapBuffers();
}

void idle()
{
	// TODO : caculate behind
	applyConstraint(point, vp, tri_point, vt, 
		&post_point, &vp2, &post_tri_point, &vt2);

	point = post_point;
	vp = vp2;
	tri_point = post_tri_point; 
	vt = vt2;

	//cout << "vp " << vp << endl;
	//cout << "vt " << vt << endl;
	//cout << "point " << point << endl;
	//cout << "tri_point " << tri_point << endl;

	glutPostRedisplay();
}

void EndProgram()
{
	// destroy shaders
	glUseProgram (0);
	glDetachShader( program, vShaderID );
	glDetachShader( program, fShaderID );
	glDeleteShader( vShaderID );
	glDeleteShader( fShaderID );
	glDeleteProgram( program );

	// destroy buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VAID_triangle);
	glDeleteBuffers(1, &VBID_triangle);
}

void reshape( int width, int height )
{
	
	double dWidth, dHeight;

	if( width >= height ) {
		dWidth = height;
		dHeight = height;
	} else {
		dWidth= width;
		dHeight = width;
	}

	glViewport( ( width - dWidth ) / 2.0, ( height - dHeight ) / 2.0, dWidth, dHeight );
}

void keyboard( unsigned char key, int x, int y )
{
	if(key==27) exit(1);

//	if(key==' ') is_tracked = !is_tracked;

	switch( key )
	{
	case 27 : exit(0); EndProgram(); break;
	}

	glutPostRedisplay( );
}

int main(int argc, char** argv)
{
	// GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	// create window
	int win_w = w_len, win_h = w_len;

	// main window
	glutInitWindowSize (win_w,win_h);
//	glutInitWindowPosition(100, 100);
	glutCreateWindow("step0");

	// set global callbacks
//	atexit(EndProgram);
	glutCloseFunc(EndProgram); // only available in FREEGLUT

	//////////////////////////////////////////////
	glewExperimental = GL_TRUE;
	//////////////////////////////////////////////

	// initialize program
	glewInit();
	InitProgram();

	glutDisplayFunc (display);
//	glutReshapeFunc (reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);

	// enter to main loop
	glutMainLoop();

	return 1;
}
