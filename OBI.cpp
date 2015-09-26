// OBI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "OBILoader.h"
#include "OBIModel.h"
#include "OBIUtils.h"

/* GLOBAL VARIABLES */

OBIModel* cube_model = new OBIModel();
OBIModel* monkey_model = new OBIModel();

/********************/

void Render()
{
	// clear the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	static float Factor= 0.0f;
	Factor += 2.0f;

	// render the shoe and rotate/translate/scale
	monkey_model->Rotate(Factor/2, Factor, Factor/2);
	monkey_model->Scale(1.0f,1.0f,1.0f);
	monkey_model->Translate(0.0f, 0.0f, 3.5f);
	monkey_model->Draw();

	cube_model->Rotate(Factor / 3, Factor, Factor /3);
	cube_model->Scale(1.0f, 1.0f, 1.0f);
	cube_model->Translate(1.0f, 0.0f, 3.5f);
	cube_model->Draw();
		
	//SET THE NEXT FRAME
	// show the rendering on the screen
	glutSwapBuffers();
	// post the next redisplay
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	// set OpenGL viewport (drawable area) 
	glViewport(0, 0, w, h);
}

void mouse(int button, int state, int x, int y)
{
	//	mx += x * 0.1f;
	//	my += y * 0.1f;

	// if ( button == GLUT_LEFT_BUTTON && state == GLUT_UP )
	//   {
	//	
	//   }
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		// quit when q is pressed
		exit(0);
		break;
	case 'w':
		monkey_model->SwitchWireframeFilled();	// switch from/to wireframe display mode
		break;
	default:
		break;
	}
}

void idle()
{

}

int main(int argc, char **argv)
{
	// initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(20, 20);
	glutInitWindowSize(FRAME_WIDTH, FRAME_HEIGHT);
	glutCreateWindow("AUGMENTED SHOES - preview");

	// set up GUI callback functions
	glutDisplayFunc(Render);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	//once glut has been initialized
	GLenum res = glewInit();

	// Enable Depth
	glEnable(GL_DEPTH_TEST);

	if (res != GLEW_OK)
		throw std::runtime_error("GLEW not available");

	// What version ?
	printf("GL version: %s\n", glGetString(GL_VERSION));

	// example with two models
	monkey_model->LoadModel("monkey.obj", "uv_monkey.png");
	monkey_model->Compile("m_vertex_shader.glsl","m_texture_fs.glsl");

	cube_model->LoadModel("monkey.obj", "uv_monkey.png");
	cube_model->Compile("m_vertex_shader.glsl", "m_texture_fs.glsl");
	// start GUI loop
	glutMainLoop();

    return 0;
}



