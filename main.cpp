#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include <functional>

int ucounter = 25; // update counter
bool rain;
bool carv=false;
bool day = true;

float move_cloud=0;
float move_dust=0;
void *currentfont;

// Car Movement
static float car1Speed = 0.5f;
static float tempCar1Speed;
static float car2Speed = 0.5f;
static float tempCar2Speed;

void dummy(void){} // does nothing

void drawPixelInt(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void setFont(void *font){
	currentfont=font;
}

void drawstring(float x,float y,float z,char *string)
{
	char *c;
	glRasterPos3f(x,y,z);

	for(c=string;*c!='\0';c++)
	{
		glutBitmapCharacter(currentfont,*c);
	}
}

// vertical straight
void twoIntVertS(int pT, int pB, int fixedP) {
    glBegin(GL_LINES);
    glVertex2i(fixedP, pT);
    glVertex2i(fixedP, pB);
    glEnd();
}

// horizontal straight
void twoIntHorzS(int pT, int pB, int fixedP) {
    glBegin(GL_LINES);
    glVertex2i(pB, fixedP);
    glVertex2i(pT, fixedP);
    glEnd();
}

void quadHorzInt(int pT, int pB, int fixedP, int fixedQ) {
    glBegin(GL_QUADS);
    glVertex2i(pB, fixedP);
    glVertex2i(pT, fixedP);
    glVertex2i(pT, fixedQ);
    glVertex2i(pB, fixedQ);
    glEnd();
}

void quadHorzInt2(int v0, int v01, int v1, int v11, int v2, int v21, int v3, int v31) {
    glBegin(GL_QUADS);
    glVertex2i(v0, v01);
    glVertex2i(v1, v11);
    glVertex2i(v2, v21);
    glVertex2i(v3, v31);
    glEnd();
}

void triangleNS(int v0, int v01, int v1, int v11, int v2, int v21) {
    glBegin(GL_TRIANGLES);
    glVertex2i(v0, v01);
    glVertex2i(v1, v11);
    glVertex2i(v2, v21);
    glEnd();
}

void quadHorzFloat(float pT, float pB, float fixedP, float fixedQ) {
    glBegin(GL_QUADS);
    glVertex2f(pB, fixedP);
    glVertex2f(pT, fixedP);
    glVertex2f(pT, fixedQ);
    glVertex2f(pB, fixedQ);
    glEnd();
}

void keyboard(unsigned char , int , int );
void update(int);
void draw_circle(float x, float y, float radius);

void plane() {
    static float planeTFactX = -180.0f; // plane translation factor X
    static float planeTFactY = 0.0f; // plane translation factor Y

    if(planeTFactX >= 900) {
         planeTFactX = -180.0f;
         planeTFactY = 0.0f;

    } else {
        planeTFactX += 0.1f;
        planeTFactY += 0.005f;
    }

    // Main Body
    glColor3ub(115, 110, 109);

    glPushMatrix();
    glTranslatef(planeTFactX, planeTFactY, 0);
    quadHorzInt2(30, 700, 100, 700, 100, 715, 35, 715);

    // Head
    triangleNS(100, 700, 110, 707, 100, 715);

    // Radar
    quadHorzInt2(37, 715, 57, 715, 40, 730, 20, 730);

    // Left wing
    quadHorzInt2(70, 715, 80, 715, 60, 735, 50, 735);

    // Right wing
    quadHorzInt2(70, 700, 80, 700, 60, 680, 50, 680);

    // Windows
    glColor3ub(255,255,255);
    int planeWindowX = 95;
    for (int i = 0; i < 8; i++) {
        drawPixelInt(planeWindowX, 710);
        planeWindowX -= 5;
    }

    glPopMatrix();
    glutPostRedisplay();

}
void roadBar(int heightLoc, int height){
    int startingLoc =0, endingLoc= 40;

    for(int i = 0; i < 20; i++){
        if(i % 2 == 0){
            glColor3ub(1,1,1);  // black color
        }else{
            glColor3ub(217, 217, 217);  // white color
        }

        quadHorzInt(startingLoc, endingLoc, heightLoc, heightLoc + height);

        startingLoc = endingLoc;
        endingLoc = endingLoc + 40;
    }
}

template <class T>
void pedestrian(T func, T func2) { // Takes two function as param

}

void DrawMainRoad(){
    // Main Road
    glColor3f(0.2,0.2,0.2);
    quadHorzInt(0, 800, 110, 330); // body

    glColor3ub(201,195,200);
    quadHorzInt(0, 800, 315, 420); // Footpath

    glColor3b(0,0,0);
    quadHorzInt(0,800,419,420);//footpath upper border

    // road divider
    glColor3f(1.0, 1.0, 1.0);
    int dividerLine= 0;
    for(int i = 0; i < 200; i++){
        twoIntHorzS(dividerLine + 40, dividerLine + 20, 210);
        dividerLine += 40;
    }

    // top Road Side Bar
    roadBar(305, 10);
    // bottom Road Side Bar
    roadBar(100, 10);
}

void drawCloud(int r, int g, int b){
    // Cloud 1
    glColor3ub(r, g, b);
    draw_circle(100 + move_cloud, 730, 33);
    draw_circle(55 + move_cloud, 730, 23);
    draw_circle(145 + move_cloud, 730, 23);

    // Cloud 2
    draw_circle(400 + move_cloud, 730, 33);
    draw_circle(355 + move_cloud, 730, 23);
    draw_circle(445 + move_cloud, 730, 23);
}

void drawSun(){
    glColor3f(255, 255, 0); // sun color
    draw_circle(300,760,30);
}

void drawMoon(){
    glColor3f(255, 255, 255); // moon color
    draw_circle(300,760,20);
}

void DrawCity(){
    // Draw Building two
    glColor3ub(153, 51, 51);
    quadHorzInt(120,250,420,630);

    int buildingY1=590,buildingY2=593;
    for(int i=0;i<3;i++){
        // floor of building 2
        glColor3ub(153, 153, 102);
        quadHorzInt(120,250,buildingY1,buildingY2);
        buildingY1=buildingY1-55;
        buildingY2=buildingY2-55;
    }

//building 2 windows
     int b2y1=495,b2y2=520;
    int b2x1=130,b2x2=160;
    for(int j=1;j<7;j++){


        glColor3ub(153,53,53);
        quadHorzInt(b2x1,b2x2,b2y1,b2y2);

        b2x1=b2x1+85;
        b2x2=b2x2+85;
        if(j%2==0){
            b2x1=130;
            b2x2=160;
            b2y1=b2y1+50;
            b2y2=b2y2+50;
        }
    }

   //gate building 2

    glColor3ub(201,195,200);
    quadHorzInt(170,200,420,460);

    //gate_lines
    glColor3ub(0, 10, 0);
    quadHorzInt(177,180,420,460);
    quadHorzInt(190,193,420,460);

    // Draw Building One

    //triangle head
    glColor3ub(236, 238, 58);
    triangleNS(5,580,80,625,155,580);

   //building 1 body
    glColor3ub(201, 195, 200);
    quadHorzInt(10,150,420,580);

    // gate of building one
    glColor3ub(88, 86, 85);
    quadHorzInt(62,92,420,468);
    //vertical black
    glColor3ub(0, 10, 0);
    quadHorzInt(65,67,420,468);
    quadHorzInt(88,90,420,468);
    //horizontal black
    quadHorzInt(62,92,440,443);
    quadHorzInt(62,92,445,448);

//windows
    int b1y1=470,b1y2=500;
    int b1x1=15,b1x2=52;
    for(int i=1;i<5;i++){
        glColor3ub(242, 242, 242);
        quadHorzInt(b1x1,b1x2,b1y1,b1y2);

        b1x1=b1x1+85;
        b1x2=b1x2+85;
        if(i%2==0){
            b1x1=15;
            b1x2=52;
            b1y1=b1y1+65;
            b1y2=b1y2+65;
        }
    }

    // Draw Building three
    glColor3ub(102, 51, 0);
    quadHorzInt(210,350,420,560);

     //flag
    glColor3ub(0, 106, 77);
    quadHorzInt(315,365,570,610);

    glColor3f(255, 0, 0);
    draw_circle(340,590,10);

    //flag stand
    glColor3ub(0, 0, 128);
    quadHorzInt(310,315,560,615);

    //building 3 gate
    glColor3ub(88, 86, 85);
    quadHorzInt(250,310,420,460);
    //horizontal black
    glColor3ub(0, 10, 0);
    quadHorzInt(250,310,440,443);
    quadHorzInt(250,310,430,433);
    quadHorzInt(290,293,420,460);

    //windows
    int b3y1=470,b3y2=500;
    int b3x1=220,b3x2=255;
    for(int j=1;j<5;j++){

        glColor3ub(242, 242, 242);
        quadHorzInt(b3x1,b3x2,b3y1,b3y2);

        b3x1=b3x1+85;
        b3x2=b3x2+85;
        if(j%2==0){
            b3x1=220;
            b3x2=255;
            b3y1=b3y1+45;
            b3y2=b3y2+45;
        }
    }
}

// Rain
void rainfunc(){
    int x=0;
    int y=778;
    static float a=-760.0f;

    if(a<=-768){
         a=-760.0f;
    }
    else{
        a-=0.5f;

    }

    glColor3ub(255,255,255);
    glPushMatrix();
    glTranslatef(0.0f,a,0.0f);
    glBegin(GL_LINES);

    for(int i=500;i>=0;i--)
    {
        for(int j=0;j<=i;j++)
        {
            glVertex3i(x,y,0);
            glVertex3i(x+3,y+10,0);
            x+=rand()%1050;
        }
        y+=rand()%15;
        x=0;

    }
    glEnd();
    // ucounter =100;
    glPopMatrix();
    glutPostRedisplay();
}

// Bank
template <class T>
void DrawBank(T func1, T func2){

    glColor3ub(153,53,53);
    quadHorzInt(580, 730, 420, 580); // Main body

    glColor3ub(34, 10, 60);
    quadHorzInt(550, 580, 420, 600); // bank right pillar
    quadHorzInt(730, 760, 420, 600); // bank left pillar

    glColor3ub(255, 255, 255);
    twoIntHorzS(550, 760, 580);
    twoIntHorzS(550, 760, 579);
    twoIntHorzS(550, 760, 578);
    glColor3ub(153, 226, 57);
    draw_circle(655,580,30);

    // windows
    glColor3ub(255, 255, 255);
    int winStart = 590, winEnd = 630, winH1 = 440, winH2 = 480;
    for(int i = 0; i < 6; i++) {
        quadHorzInt(winStart, winEnd, winH1, winH2);
        winStart += 45;
        winEnd += 45;
        if (i == 2) {
            winStart = 590, winEnd = 630;
            winH1 += 80;
            winH2 += 80;
        }
    }

    glColor3ub(0, 0, 0);
    setFont(GLUT_BITMAP_HELVETICA_12);
    drawstring(637.0,590.0,0.0,"BANK BD");
}

void otherCircle(GLfloat x, GLfloat y,GLfloat z, GLfloat radius,int r,int g,int b)
{
	int i;
	int triangleAmount = 20; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * 3.1416;
    int counter=0;
    glColor3ub(r,g,b);
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(x, y,z); // center of circle
		for(i = 0; i <= triangleAmount;i++) {
			counter+=1;
			glVertex3f(
		            x + (radius * cos(i *  twicePi / triangleAmount)),
			    y + (radius * sin(i * twicePi / triangleAmount)),z
			);
		}
	glEnd();
}

void carBase(int r, int g, int b) {
    glColor3ub(r, g, b);
    quadHorzInt(20, 220, 140, 180); // lower part

    quadHorzInt2(40, 180, 200, 180, 160, 210, 80, 210); // body higher part

    glColor3ub(0,0,0);
    quadHorzInt2(115, 180, 190, 180, 155, 205, 115, 205); // car window front
    quadHorzInt2(45, 180, 110, 180, 110, 205, 80, 205); // car window back
    quadHorzInt(115, 135, 170, 175); // handle front
    quadHorzInt(45, 65, 170, 175); // handle back
    otherCircle(30.0f,160.0f,0.0f,19.0f, r, g, b); // back bumper
    otherCircle(210.0f,160.0f,0.0f,19.0f, r, g, b); // front bumper

    // Light
    glColor3ub(250,250,0);
    quadHorzInt(225, 230, 155, 165);

    // car light
    if(day == false)
    {
        glColor3ub(200,200,200);
        glBegin(GL_QUADS);
        glVertex3i(230,155,0);
        glVertex3i(280,155,0);
        glVertex3i(280,195,0);
        glVertex3i(230,165,0);
        glEnd();
    }

    otherCircle(60.0f,140.0f,0.0f,18.0f,0,0,0);
    otherCircle(60.0f,140.0f,0.0f,10.0f,137,137,137);
    otherCircle(170.0f,140.0f,0.0f,18.0f,0,0,0);
    otherCircle(170.0f,140.0f,0.0f,10.0f,137,137,137);
}

void cars() {
    static float car1Position = -400.0f;
    static float car2Position = -1200.0f;

    if(car1Position >= 1324){
         car1Position = -400.0f;
    }
    else {
        car1Position += car1Speed;
    }

    if(car2Position >= 1324){
         car2Position = -1200.0f;
    }
    else {
        car2Position += car2Speed;
    }

    glPushMatrix();
    glTranslatef(car1Position, 0, 0);
    carBase(255, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(car2Position, 0, 0);
    carBase(0, 0, 255);
    glPopMatrix();

    glutPostRedisplay();
}

void commonStuff(){
    DrawCity();
    DrawMainRoad();
    DrawBank(dummy, dummy);
    plane();
    if(carv){

    cars();
    }
    if(rain){
        rainfunc();
    }
}
void daymode(){
    drawSun();
    drawCloud(255, 255, 255);
    glClearColor(0.0,0.7,1.5,0.0);
    commonStuff();
}

void nightmode(){
    drawMoon();
    drawCloud(232, 225, 224);
    glClearColor(0.0,0.0,0.0,0.0);
    commonStuff();
}

int flag=2;

// welcome screen
void startscreen(void){
    setFont(GLUT_BITMAP_HELVETICA_18);
    glClearColor(0.000, 0.000, 0.000,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void display(){
    if(day){
        daymode();
    }
    else{
        nightmode();
    }
}

void myDisplay(void){
    glClear (GL_COLOR_BUFFER_BIT);

    if(flag>1){
        display();
    }

    glFlush ();
    glutSwapBuffers();
}

void myInit (void){
    glClearColor(0.0,0.7,1.5,0.0);  // sky color
    glColor3f(0.0f, 0.0f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 800.0, 100.0, 800.0);
}
void handleMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
		     day=false;
		}
	}
	else
        if (state == GLUT_DOWN)
		{
		     day=true;
		}
	glutPostRedisplay();
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (800, 680);
    glutInitWindowPosition (280,0);
    glutCreateWindow ("");
    glutDisplayFunc(myDisplay);
    glutTimerFunc(ucounter, update, 100); // Add a timer
    glutKeyboardFunc(keyboard); // keyboard handler
    glutMouseFunc(handleMouse);//mouse handler
    myInit ();
    glutMainLoop();
}

// Keyboard handler
void keyboard(unsigned char key, int x, int y){
    if(key==13){
        flag++;
        myDisplay();
    }
    if(key=='h'){
        flag=1;
        myDisplay();
    }
    if(key=='r') {
        rain=true;
    }
    if(key=='s'){
        rain=false;
    }
    if(key=='d'){
        day=true;
    }
    if(key=='n'){
        day=false;
    }
    // Car appear
    if(key=='c')
    {
        carv=true;
        sndPlaySound("TrafficSound.wav",SND_ASYNC|SND_LOOP);
    }
    // Car disappear
    if(key=='v')
    {
        carv=false;
        sndPlaySound(NULL, SND_ASYNC|SND_LOOP);
    }
    // Stops all car
    if (key == '.') {
        sndPlaySound(NULL, SND_ASYNC|SND_LOOP);

        if (car2Speed != 0.0f) {
            tempCar1Speed = car1Speed;
            tempCar2Speed = car2Speed;
        }

        car1Speed = 0.0f;
        car2Speed = 0.0f;
    }
    if (key == ',') {
        car1Speed = tempCar1Speed;
        car2Speed = tempCar2Speed;
        sndPlaySound("TrafficSound.wav",SND_ASYNC|SND_LOOP);
    }

}
void update(int value) {
    move_cloud = move_cloud + .2;

	glutPostRedisplay(); // Tell GLUT that the display has changed
	glutTimerFunc(ucounter, update, 0);   // Tell GLUT to call update again in 25 milliseconds
}

void draw_circle(float x, float y, float radius) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(x, y, 0.0f);
    static const int circle_points = 100;
    static const float angle = 2.0f * 3.1416f / circle_points;

    glBegin(GL_POLYGON);
    double angle1=0.0;
    glVertex2d(radius * cos(0.0) , radius * sin(0.0));
    int i;

    for (i=0; i<circle_points; i++)
    {
        glVertex2d(radius * cos(angle1), radius *sin(angle1));
        angle1 += angle;
    }
    glEnd();
    glPopMatrix();
}
