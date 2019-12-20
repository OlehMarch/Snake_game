#include "stdafx.h"
#include <stdlib.h>
#include <ctime>
#include <GL/glut.h>   //Подключение библиотеки glut.h
#include <GL/GLAux.h>
//--------------------------------------------------------------
int N = 30, M = 20;
int Scale = 25;

int w = Scale * N;
int h = Scale * M;

int dir, num = 4, koefOfSnakeLength = 0, speed = 150;

unsigned int textures[1];
//--------------------------------------------------------------
struct {
	int x;
	int y;
} s[100];
//--------------------------------------------------------------
class Fructs {
	public:
		int x, y;

		void New() {
			x = rand() % N;
			y = rand() % M;
		}

		void DrawApple() {
			glColor3f(0.0, 1.0, 0.0);
			glRectf(x * Scale, y * Scale, (x + 1) * Scale, (y + 1) * Scale);
			/*glBegin(GL_QUADS);
				//------------------------------------------//
				glTexCoord2f(x * Scale, y * Scale);
				glVertex2f((x + 1) * Scale, (y + 1) * Scale);
				//------------------------------------------//
				glTexCoord2f(x * Scale, y * Scale);
				glVertex2f((x + 1) * Scale, (y + 1) * Scale);
				//------------------------------------------//
				glTexCoord2f(x * Scale, y * Scale);
				glVertex2f((x + 1) * Scale, (y + 1) * Scale);
				//------------------------------------------//
				glTexCoord2f(x * Scale, y * Scale);
				glVertex2f((x + 1) * Scale, (y + 1) * Scale);
				//------------------------------------------//*/
		}
} m[10];
//--------------------------------------------------------------
/*void loadTextures() {
	AUX_RGBImageRec *texture1 = auxDIBImageLoadA("apple.bmp");
	glGenTextures(1, &textures[0]);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texture1->sizeX, texture1->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, texture1->data);
}*/
//--------------------------------------------------------------
void Tick() {
	//перемещение "туловища" змейки за "головой"
	for (int i = num; i > 0; --i) {
		s[i].x = s[i-1].x;
		s[i].y = s[i-1].y;
	}
	//перемещение "головы" змейки стралками клавиатуры
	if (dir == 0) {
		s[0].y += 1;
	}
	if (dir == 1) {
		s[0].x -= 1;
	}
	if (dir == 2) {
		s[0].x += 1;
	}
	if (dir == 3) {
		s[0].y -= 1;
	}
	//увеличение длины змейки при "поедании" "яблок"
	int value = 4;
	for (int i = 0; i < 10; i++) {
		if ((s[0].x == m[i].x) && (s[0].y == m[i].y)) {
			num++;
			m[i].New();
			if (num > value) {
				koefOfSnakeLength += 5;
				value = num;
			}
		}
	}
	//перенаправление змейки при нахождении на края окна
	if (s[0].x > N) {
		dir = 1;
	}
	if (s[0].x < 0) {
		dir = 2;
	}
	if (s[0].y > M) {
		dir = 3;
	}
	if (s[0].y < 0) {
		dir = 0;
	}
	//обрезка змейки при наскакивании на саму себя
	int valueMin = 2;
	for (int i = 1; i < num; i++) {
		if ((s[0].x == s[i].x) && (s[0].y == s[i].y)) {
			num = i;
			if (num == valueMin) {
				koefOfSnakeLength = -10;
			}
		}
	}
}
//--------------------------------------------------------------
void DrawField() {
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	for (int i = 0; i < w; i+=Scale) {
		glVertex2f(i, 0);
		glVertex2f(i, h);
	}
	for (int j=0; j < h; j+=Scale) {
		glVertex2f(0, j);
		glVertex2f(w, j);
	}
	glEnd();
}
//--------------------------------------------------------------
void DrawSnake() {
	glColor3f(0.9, 0.0, 0.0);
	for (int i = 0; i < num; i++) {
		glRectf(s[i].x * Scale, s[i].y * Scale, (s[i].x + 1) * Scale, (s[i].y + 1) * Scale);
	}
}
//--------------------------------------------------------------
void MyKeyboard(int key, int a, int b) {
	switch(key) {
		case 101:
			dir = 0;
			break;
		case 102:
			dir = 2;
			break;
		case 100:
			dir = 1;
			break;
		case 103:
			dir = 3;
			break;
	}
}
//--------------------------------------------------------------
void display() {

	glClear(GL_COLOR_BUFFER_BIT);	//Очищаем экран 
	glClearColor(0.98, 0.98, 0.82, 1); //Устанавливаем цвет фона
	
	for (int i = 0; i < 10; i++) {
		m[i].DrawApple();
	}

	DrawField();
	DrawSnake();

	glFlush();
}
//--------------------------------------------------------------
void timer(int = 0) {
	display();
	Tick();
	glutTimerFunc(speed - koefOfSnakeLength, timer, 0);
}
//--------------------------------------------------------------
int main(int argc, char **argv) {

	for (int i = 0; i < 10; i++) {
		m[i].New();
	}

	s[0].x = 10;
	s[0].y = 0;

/*	loadTextures();
	glEnable(GL_TEXTURE_2D);*/

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(w, h);		//Указываем размер окна
	glutInitWindowPosition(300, 200);	//Позиция окна
	glutCreateWindow("Snake. The Game");		//Имя окна
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glutDisplayFunc(display);				//Вызов функции отрисовки
	glutTimerFunc(50, timer, 0);
	glutSpecialFunc(MyKeyboard);
	glutMainLoop();
 
	return 0;
}