//#include <stdio.h>
#include <math.h>
#include <gl/glut.h>
#include <windows.h>
#include <time.h>
#include <iostream>
#include <chrono>

using namespace std;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w,int h);
void TimerFunction(int value);
void SpecialKeyboard (int key, int x, int y);
void keyboard(unsigned char key, int x, int y);
void motionfunc(int x, int y);
void Mouse (int button, int state, int x, int y);
#define WIDTH 800
#define HEIGHT 600
#define ON 1
#define OFF 0


using namespace std::chrono;
system_clock::time_point Start; //Start에 현재 시각이 저장됨
system_clock::time_point End; //End에 현재 시각이 저장됨
duration<double> dur;

int enemyNumber=10;
int timer=1;
int timeSet=0;
int noDie=0;
int speed=10;
int shot=0;
int shotZ=5;
int shotNumber=0;
int yo;
BOOL jump=FALSE;
BOOL jumpt;// 점프/
int jumpp=16;//점프
int j;

int oldx;
float xp, zp, yp;
int lx=0;
int ly=400;
int lz=1000;

float amb=0.5;
float dif=0.5;
float spec=20;

float rot;

int trap=0;
int traprot=0;

float foodx, foodz;
int foodoption;
int state=5;
typedef struct flat
{
	float r, g, b;
}flat;
struct flat flat[6];

int trapTrigger = 300;
int trapNumber = 0;

typedef struct enemy
{
	float x, y, z;
	float xm, ym, zm;
	float bx,by,bz;
	int alive;
}enemy;

struct enemy enemy[1000];
void menu(int input)
{
	if(input==1)
	{

	}
	if(input==2)
	{


	}
	if(input==3)
	{



	}
	if(input==4)
	{

	}
	glutPostRedisplay ();
}



GLvoid drawScene( GLvoid )
{

	if(GetAsyncKeyState('D') & 0x8000)
	{
		if((zp-speed*sin(rot/60))>-800&&(zp-speed*sin(rot/60)<800))

			zp-=speed*sin(rot/60);

		if((xp-speed*cos(rot/60))>-800&&(xp-speed*cos(rot/60)<800))

			xp-=speed*cos(rot/60);
	}
	if(GetAsyncKeyState('A') & 0x8000)
	{
		if((zp+speed*sin(rot/60))>-800&&(zp+speed*sin(rot/60)<800))

			zp+=speed*sin(rot/60);

		if((xp+speed*cos(rot/60))>-800&&(xp+speed*cos(rot/60)<800))

			xp+=speed*cos(rot/60);
	}
	if(GetAsyncKeyState('W') & 0x8000)
	{
		if((zp+speed*cos(rot/60))<800&&(zp+speed*cos(rot/60))>-800)

			zp+=speed*cos(rot/60);

		if((xp-speed*sin(rot/60))>-800&&(xp-speed*sin(rot/60)<800))

			xp-=speed*sin(rot/60);
	}
	if(GetAsyncKeyState('S') & 0x8000)
	{
		if((zp-speed*cos(rot/60))>-800&&(zp-speed*cos(rot/60)<800))

			zp-=speed*cos(rot/60);

		if((xp+speed*sin(rot/60))>-800&&(xp+speed*sin(rot/60)<800))

			xp+=speed*sin(rot/60);
	}

	if(trap==1&&traprot<45)
	{
		traprot++;

	}
	if(trap==0&&traprot>0)
	{
		traprot--;

	}



	if(foodx+xp>=-25&&foodx+xp<=25&&foodz+zp>=-25&&foodz+zp<=25&&yp<=25)
	{
		if(foodoption==1){
			speed=20;
			timeSet=timer;

		}
		if(foodoption==2){
			noDie=ON;
			timeSet=timer;
		}
		if(foodoption==3){
			shotNumber=5;

		}

		state=foodoption;
		foodx=rand()%1300-750;
		foodz=rand()%1300-750;
		foodoption=rand()%5;


	}


	for(int i=0;i<enemyNumber;i++){
		if(noDie!=ON){
			if(enemy[i].x+xp>=-30&&enemy[i].x+xp<=25&&enemy[i].z+zp>=-25&&enemy[i].z+zp<=25&&enemy[i].y+yp==0)
			{
				printf("D");
				//exit(0);
			}

			//적 발사체 충돌체크
			if(enemy[i].bx+enemy[i].x+xp>=-30 && enemy[i].bx+enemy[i].x+xp<=25 && enemy[i].bz+zp+enemy[i].z>=-25 && enemy[i].bz+zp+enemy[i].z<=25 && enemy[i].y+yp==0 )
			{
				printf("D");
				//exit(0);
			}
		}

		//주인공 발사체 파괴 충돌체크
		if(enemy[i].x+xp>=30 && enemy[i].x+xp<=25 && enemy[i].z+zp>=-25+shotZ && enemy[i].z+zp<=25+shotZ && enemy[i].y+yp==0)
		{

			enemy[i].alive=0;
		}

		enemy[i].x+=enemy[i].xm; //적 이동
		enemy[i].z+=enemy[i].zm;
		enemy[i].bx+=enemy[i].xm; //적 발사체
		enemy[i].bz+=enemy[i].zm;

		if(enemy[i].x>=800||enemy[i].x<=-800)enemy[i].xm=-enemy[i].xm;


		if(enemy[i].z>=800||enemy[i].z<=-800)enemy[i].zm=-enemy[i].zm;

	}

	if(jump)//점프부분
	{
		yp=yp+jumpp;
		jumpp--;
		if(j==33)
		{
			jump=FALSE;
			jumpt=FALSE;
			jumpp=16;
			j=0;
			yp=yo;

		}
		j++;
	}


	if((timer-timeSet)>=700){ //7초가 지나면
		speed=10;  //원래속도로
		noDie=OFF;  //무적풀림
		shot=OFF; //발사체 ㄴㄴ

	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat lightPos[] = {lx+xp,ly,lz+zp,1};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos); //위치 설정


	glEnable (GL_DEPTH_TEST);
	glEnable (GL_CULL_FACE);
	glShadeModel (GL_SMOOTH); 
	glEnable(GL_COLOR_MATERIAL);
	glEnable (GL_LIGHTING);
	glEnable(GL_LIGHT0); //0번 조명 사용
	glPushMatrix();

	glRotatef (traprot, 0.0f, 0.0f, 1.0f);
	if(trap==1)
	{
		if(xp+5<800)
			xp+=7;
	}


	glPushMatrix();

	glColor3f (1.0,1.0,1.0);

	if(shot==ON && shotNumber>0){
		glPushMatrix();
		//		if(timer-timeSet<5)
		glTranslatef(xp,0,shotZ+zp);
		//if(timer-timeSet>=5)
		//glTranslatef(0,0,shotZ);
		//printf("%d\n",timer-timeSet);
		glTranslatef(0, -800+15, 0);
		glutSolidSphere(15,10,10);
		glPopMatrix();
		shotZ-=5;
	}
	else
		shot=OFF;




	glTranslatef(0, -800+15+yp, 0);//이동


	//glRotatef (-rot, 0.0f, 1.0f, 0.0f);
	switch (state)
	{
	case 1:
		{
			glColor3f (1.0, 0.0, 0.0);
			break;
		}
	case 2:
		{
			glColor3f (0.0, 1.0, 0.0);
			break;
		}
	case 3:
		{
			glColor3f (0.0, 0.0, 1.0);
			break;
		}
	case 4:
		{
			glColor3f (0.0, 1.0, 1.0);
			break;
		}
	case 0:
		{
			glColor3f (1.0, 1.0, 0.0);
			break;
		}
	case 5:
		{
			glColor3f (1.0, 1.0, 1.0);
			break;
		}
	}
	glutSolidSphere(30,20,20);

	glPopMatrix();


	glPushMatrix();

	//glTranslatef(xp, 0, zp);
	//glRotatef (rot, 0.0f, 1.0f, 0.0f);
	glRotatef (rot, 0.0f, 1.0f, 0.0f);


	for(int i=0;i<enemyNumber;i++){

		if(enemy[i].alive==1){
			if(timer%500>=0 && timer%500<=150 && i%5==0){  //5초마다 발사체 발사 (1.5초지속)
				glPushMatrix();
				glColor3f(1,0,0);


				glTranslatef(enemy[i].bx,0,enemy[i].bz);
				glTranslatef(enemy[i].x+xp+enemy[i].xm, enemy[i].y-800+25, enemy[i].z+zp+enemy[i].zm);
				glutSolidSphere(15,10,10);
				glPopMatrix();
			}
			else{ //발사체초기화
				enemy[i].bx=0;
				enemy[i].by=0;
				enemy[i].bz=0;
			}


			glPushMatrix();
			glColor3f (0.0, 0.0, 0.0);
			glTranslatef(enemy[i].x+xp+enemy[i].xm, enemy[i].y-800+25, enemy[i].z+zp+enemy[i].zm);//이동
			glutSolidSphere(20,10,10);

			glPopMatrix();

		}
	}


	glPushMatrix();


	glTranslatef(foodx+xp, -800+25, foodz+zp);//이동
	switch (foodoption)
	{
	case 1:
		{
			glColor3f (1.0, 0.0, 0.0);
			break;
		}
	case 2:
		{
			glColor3f (0.0, 1.0, 0.0);
			break;
		}
	case 3:
		{
			glColor3f (0.0, 0.0, 1.0);
			break;
		}
	case 4:
		{
			glColor3f (0.0, 1.0, 1.0);
			break;
		}
	case 0:
		{
			glColor3f (1.0, 1.0, 0.0);
			break;
		}

	}

	glutSolidCube(50);

	glPopMatrix();

	glBegin (GL_QUADS);

	glColor3f (flat[0].r, flat[0].g, flat[0].b); // 제일 앞면
	glVertex3f (-800.0+xp, -800.0, 800.0+zp);
	glVertex3f (-800.0+xp, 800.0, 800.0+zp);
	glVertex3f (800.0+xp, 800.0, 800.0+zp);
	glVertex3f (800.0+xp, -800.0, 800.0+zp);

	glColor3f (flat[1].r, flat[1].g, flat[1].b); // 오른쪽 면
	glVertex3f (800.0+xp, -800.0, 800.0+zp);
	glVertex3f (800.0+xp, 800.0, 800.0+zp);
	glVertex3f (800.0+xp, 800.0, -800.0+zp);
	glVertex3f (800.0+xp,-800.0, -800.0+zp);

	glColor3f (flat[2].r, flat[2].g, flat[2].b); //제일 뒷면
	glVertex3f (-800.0+xp, 800.0, -800+zp);
	glVertex3f (-800.0+xp, -800.0, -800+zp);
	glVertex3f (800+xp, -800.0, -800+zp);
	glVertex3f (800+xp, 800.0, -800+zp);

	glColor3f (flat[3].r, flat[3].g, flat[3].b); //왼쪽면
	glVertex3f (-800.0+xp, 800.0, 800.0+zp);
	glVertex3f (-800.0+xp, -800.0, 800.0+zp);
	glVertex3f (-800.0+xp, -800.0, -800+zp);
	glVertex3f (-800.0+xp, 800.0, -800+zp);

	glColor3f (flat[4].r, flat[4].g, flat[4].b); // 바닥
	glVertex3f (-800.0+xp, -800.0, 800.0+zp);
	glVertex3f (800.0+xp, -800.0, 800.0+zp);
	glVertex3f (800.0+xp, -800.0, -800.0+zp);
	glVertex3f (-800.0+xp, -800.0, -800.0+zp);

	glColor3f (flat[5].r, flat[5].g, flat[5].b); // 천장
	glVertex3f (800.0+xp, 800.0, 800.0+zp);
	glVertex3f (-800.0+xp, 800.0, 800.0+zp);
	glVertex3f (-800.0+xp, 800.0, -800.0+zp);
	glVertex3f (800.0+xp, 800.0, -800.0+zp);

	//glColor3f (flat[4].r, flat[4].g, flat[4].b); // 바닥
	//glVertex3f (-800.0+xp, -800.0, -1800+zp);
	//glVertex3f (-800.0+xp, -800.0, -800.0+zp);
	//glVertex3f (1800+xp, -800.0, -800.0+zp);
	//glVertex3f (1800+xp, -800.0, -1800+zp);

	//glColor3f (flat[1].r, flat[1].g, flat[1].b); // 오른쪽 면
	//glVertex3f (1800+xp, 800.0, -1800+zp);
	//glVertex3f (1800+xp, -800.0, -1800+zp);
	//glVertex3f (1800+xp, -800.0, -800.0+zp);
	//glVertex3f (1800+xp,800.0, -800.0+zp);

	//glColor3f (flat[1].r, flat[1].g, flat[1].b); // 오른쪽 면
	//glVertex3f (800.0+xp, 800.0, -800.0+zp);
	//glVertex3f (800.0+xp, -800.0, -800.0+zp);
	//glVertex3f (1800+xp, -800.0, -800.0+zp);
	//glVertex3f (1800+xp,800.0, -800.0+zp);

	//glColor3f (flat[5].r, flat[5].g, flat[5].b); // 천장
	//glVertex3f (-800.0+xp, 800.0, -800.0+zp);
	//glVertex3f (-800.0+xp, 800.0, -1800.0+zp);
	//glVertex3f (1800.0+xp, 800.0, -1800.0+zp);
	//glVertex3f (1800.0+xp, 800.0, -800.0+zp);


	glEnd ();

	glColor3f (0.0, 0.0, 0.0);





	glPopMatrix();
	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);
	glDisable (GL_LIGHTING);
	glDisable(GL_LIGHT0); //0번 조명 사용
	glDisable (GL_CULL_FACE);


	glDisable (GL_DEPTH_TEST);


	glutSwapBuffers ();
}

void SetupRC(){

	enemy[0].xm=5;
	GLfloat ambientLight[] = {amb, amb, amb, 1.0};
	GLfloat DiffuseLight[] = {dif, dif, dif, 1.0f};
	GLfloat SpecularLight[] = {1, 1, 1.0, spec};
	GLfloat Shine[] = {1, 0, 1.0, spec};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight); //Diffuse설정Ambient와 동일한 값으로 줘도 되고 다른 값을 줘도 된다.
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight); //Specular 설정


	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	//glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, ambientLight);
	//glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, ambientLight);//저장된 값으로 주변광을 설정한다.



	glMaterialfv (GL_FRONT, GL_SPECULAR, SpecularLight);
	glMateriali(GL_FRONT, GL_SHININESS, spec);

	srand(time(NULL));
	for(int i=0; i<6; i++)
	{
		flat[i].r=(rand()%10)*0.1;
		flat[i].g=(rand()%10)*0.1;
		flat[i].b=(rand()%10)*0.1;
	}
	foodoption=rand()%5;
	foodx=rand()%750;
	foodz=rand()%750;
	for(int i=0;i<100;i++){

		enemy[i].x=rand()%800;
		enemy[i].z=rand()%800;

		enemy[i].xm=rand()%7;
		enemy[i].zm=rand()%7;

		enemy[i].alive=1;

		if(i%10){
			enemy[i].xm=rand()%15;
			enemy[i].zm=rand()%15;

		}
	}

}

GLvoid Reshape(int w,int h)
{

	//glViewport(0,0,w,h);
	glMatrixMode (GL_PROJECTION);

	glLoadIdentity();
	gluPerspective (60.0f, w/h, 100.0, 4000.0);
	//glFrustum (0, 600, 0, 800, 1, 0);
	gluLookAt (0.0, 0.0, 1800,   0.0, 0.0, 1.0,   0.0, 1.0, 0.0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 바탕색을 'green' 로 지정
	glMatrixMode (GL_MODELVIEW);
	glutSwapBuffers ();

}
/*void TimerFunction (int value)
{
xp += xm;//움직이기
yp += ym;

glutPostRedisplay (); // 화면 재 출력
glutTimerFunc(100, TimerFunction, 1); // 타이머함수 재 설정
}*/
void main(int argc,char*argv[])
{
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//glutInitDisplayMode (GLUT_RGB);
	glutInitWindowPosition( 100, 100 );
	glutInitWindowSize ( 800, 600 );
	glutCreateWindow( "Example2" );

	SetupRC();

	glutDisplayFunc( drawScene );
	glutReshapeFunc (Reshape);
	glutMotionFunc(motionfunc);
	glutMouseFunc (Mouse);
	glutTimerFunc (100, TimerFunction, 1);
	GLint menuID=glutCreateMenu(menu);
	//glutSpecialFunc (SpecialKeyboard);
	glutKeyboardFunc(keyboard);
	glutAddMenuEntry("culling", 1);
	glutAddMenuEntry("depth", 2);
	glutAddMenuEntry("top", 3);
	glutAddMenuEntry("shading", 4);
	srand(time(NULL));
	/*for(int i=0; i<15; i++)
	{
	torus[i].xp=-1000;
	}*/

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	Start = system_clock::now();
	
	




	glutMainLoop();
}
void motionfunc(int x, int y)
{
	if(oldx<x)
	{
		//glRotatef (x*0.01, 0, 1, 0);

		rot+=6;
		if(rot==360||rot==-360)
		{
			rot=0;
		}

	}
	else
	{
		//glRotatef (-x*0.01, 0, 1, 0);
		rot-=6;
		if(rot==360||rot==-360)
		{
			rot=0;
		}

	}
	oldx=x;
	glutPostRedisplay ();
}
void TimerFunction(int value)
{
	End = system_clock::now();
	dur = End - Start;

	cout <<dur.count()<< endl;

	if (trapTrigger == 0){
		trapNumber = rand() % 300;
		trapTrigger = rand() % 3000;
	}
	else
		trapTrigger--;

	if (trapNumber <= 0)
		trap = 0;
	else{
		trap = 1;
		trapNumber--;
	}
	/*cout << trapTrigger << " " << trapNumber << endl;*/


	timer++;
	glutTimerFunc (10, TimerFunction, 1);
	glutPostRedisplay ();
	if(timer%1000==0) 
		enemyNumber++;

	
};

void keyboard(unsigned char key, int x, int y)
{
	if(key==32)
	{
		if(jumpt==FALSE)
		{
			yo=0;
			jumpt=TRUE;
			jump=TRUE;
		}
	}


	if(key == 'x')
	{
		glRotatef (1, 1.0f, 0, 0);
	}
	if(key == 'X')
	{
		glTranslatef(-5, 0, 0);
	}
	if(key == 'y')
	{
		glRotatef (1, 0, 1, 0);
	}
	if(key == 'Y')
	{
		glRotatef (1, 0, -1, 0);
	}
	if(key=='z')
	{
		glTranslatef(0, 0, 5);
	}

	if(key=='Z')
	{
		glTranslatef(0, 0, -5);
	}
	if(key=='j')
	{
		lz+=10;
	}
	if(key=='J')
	{
		lz-=10;
	}
	if(key=='t')
	{
		if(trap==0)
		{
			trap=1;
		}
		else
		{
			trap=0;
		}
	}
	if(key==27)
		exit(0);


	glutPostRedisplay ();
}




void Mouse (int button, int state, int x, int y){
	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		shotZ=-5;
		shot=ON;
		timeSet=timer;
		//shotNumber--;

	}
}