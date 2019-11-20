#include <iostream>
#include <stdio.h>
#include <vector>
#include <unistd.h>

#include <GL/glew.h>  // needs to be included before any other OpenGL package
#include <GL/glut.h>

#include "config.h"
#include "math3d.h"
#include "game_of_life.h"

#define WIDTH 1000
#define HEIGHT 1100

int FRAME_COUNT = 0;
int DURATION = 20;
GLuint VBO;
GameOfLifeBoard GOL_BOARD;

// start ffmpeg telling it to expect raw rgba 720p-60hz frames
// -i - tells it to read frames from stdin
const char* cmd = "ffmpeg -r 15 -f rawvideo -pix_fmt rgba -s 1000x1100 -i - "
				"-threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip output.mp4";

// open pipe to ffmpeg's stdin in binary write mode
FILE* ffmpeg = popen(cmd, "w");

int* buffer = new int[WIDTH*HEIGHT];

void drawBoard(GameOfLifeBoard board) {
	for(size_t i {0}; i < board._rows; i++) {
		for(size_t j {0}; j < board._columns; j++) {
			if(board._board[i][j]) {
			float x1 = -1.0f + 2.0f/board._columns * j;
			float y1 = -1.0f + 2.0f/board._rows * i;
			float x2 = -1.0f + 2.0f/board._columns * (j+1);
			float y2 = -1.0f + 2.0f/board._rows * (i+1);
			glRectd(x1, y1, x2, y2);
			}
		}
	}
}

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GOL_BOARD.doGameCycle();
	drawBoard(GOL_BOARD);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();

	if(FRAME_COUNT < DURATION * 15) {
		std::cout << FRAME_COUNT << std::endl;
		glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		fwrite(buffer, sizeof(int)*WIDTH*HEIGHT, 1, ffmpeg);
	} else if (FRAME_COUNT == DURATION * 30) {
		pclose(ffmpeg);
		std::cout << "Done!" << endl;
		exit(0);
	}
	
	//std::cout << "Frame: " << ::FRAME_COUNT << std::endl;
	FRAME_COUNT++;
	//usleep(20000);
}

static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
}

void initGlut(int &argc, char** &argv) {
	// This call initializes GLUT. The parameters can be provided directly from the command line and include
	// useful options such as '-sync' and '-gldebug' which disable the asynchronous nature of X and 
	// automatically checks for GL errors and displays them (respectively).
	glutInit(&argc, argv);

	// GLUT_DOUBLE enables double buffering (drawing to a background buffer while another buffer is
	// displayed) and the color buffer where most rendering ends up (i.e. the screen).
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);

	// Sets window geometries and properties.
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL CMake: Window");

	// Defines the display function of glut
	InitializeGlutCallbacks();
}

static void CreateVertexBuffer()
{
	vector3 vertices[6];
    vertices[0] = vector3(-1.0f, -1.0f, 0.0f);
    vertices[1] = vector3(0.0f, -1.0f, 0.0f);
    vertices[2] = vector3(0.0f, 0.0f, 0.0f);
    vertices[3] = vector3(-1.0f, -1.0f, 0.0f);
    vertices[4] = vector3(-1.0f, 0.0f, 0.0f);
    vertices[5] = vector3(0.0f, 0.0f, 0.0f);

 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

int main(int argc, char** argv)
{


	initGlut(argc, argv);

	// Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }

	// set color of the glClear() method
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// set up game of life board
	GOL_BOARD.setGliderGun(0,1);
	GOL_BOARD.setPentadecathlon(20,10);
	GOL_BOARD.setPentadecathlon(40,30);
	GOL_BOARD.setPentadecathlon(60,50);
	// GOL_BOARD.writeBoardToFile("../board.txt");
	// GOL_BOARD.readBoardFromFile("../board.txt");
	GOL_BOARD.setBoard(GOL_BOARD + GOL_BOARD.mirrorAtMajorDiagonal());
	GOL_BOARD.print();

    glutMainLoop();

    return 0;
}
