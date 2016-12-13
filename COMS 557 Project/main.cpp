#include <GLUT/glut.h>
#include <iostream>
#include <string>
#include <sstream>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "image.h"
#include "snake.h"

Snake* player;

double unitSize = 10;
double width = 640;
double height = 480;


int row = width / unitSize;
int col = height / unitSize;

int lx = row;
int ly = col;



double minX = -1.0, maxX = 1.0;
double minY = -1.0, maxY = 1.0;


int score = 0;
int score_mul = 1;

double tick = 65;
double timerMultiplier = 0.8;
double speed = 1.0;

int direction_x = 1;
int direction_y = 0;


bool showMap = false;


int snakeShape = 0;


static GLuint texName[36];


typedef enum { SPEED1, SPEED2, SPEED3,
    PERSPECTIVE_1, PERSPECTIVE_2, SNAKE1, SNAKE2, out } optionsMenu;

bool showSplashScreen = true;

int point_x, point_y;

int point_angle = 0;


int big = 0;



void loadTexture(Image* image,int k) {
    glBindTexture(GL_TEXTURE_2D, texName[k]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    
    
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 image->width, image->height,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 image->pixels);
}

Image* loadBMP(const char* filename);

void onMenu(int opcion) {
    switch(opcion) {
            
        case SPEED1:
            speed = 1.1;
            break;
        case SPEED2:
            speed = 2.2;
            break;
        case SPEED3:
            speed = 3.3;
            break;
            
        case PERSPECTIVE_1:
            showMap = false;
            break;
        case PERSPECTIVE_2:
            showMap = true;
            break;
            
        case SNAKE1:
            snakeShape = 0;
            break;
        case SNAKE2:
            snakeShape = 1;
            break;
            
        case out:
            exit(0);
            break;
    }
    glutPostRedisplay();
}

static void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_FLAT);
    
    player = new Snake(row / 2,
                       col / 2,
                       100);
    
    srand((int) time(NULL));
    
    point_x = rand() % row + 1;
    point_y = rand() % col + 1;
    
    glGenTextures(2, texName);
    Image* image;
    
    image = loadBMP("/Users/dipit/Downloads/snake-3d-master/snake.bmp");
    loadTexture(image, 0);
    
    image = loadBMP("/Users/dipit/Downloads/snake-3d-master/app.bmp");
    loadTexture(image, 1);
    
    delete image;
}


double position_x_2d(int x) {
    double wide = maxX - minX;
    double mappedX = x * (wide / row);
    
    return minX + mappedX;
}
double position_y_2d(int y) {
    double tall = maxY - minY;
    double mappedY = y * (tall / col);
    
    return minY + mappedY;
}


void drawString (void *f, const char *s, float x, float y) {
    unsigned int i;
    glRasterPos2f(x, y);
    
    for (i = 0; i < strlen (s); i++)
        glutBitmapCharacter (f, s[i]);
}

void draw3dString (void *f, const char *s, float x, float y, float z) {
    unsigned int i;
    
    glPushMatrix();
    
    glTranslatef(x, y, z);
    glScaled(0.0010, 0.0010, 0.0010);
    
    for (i = 0; i < strlen (s); i++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, s[i]);
    
    glPopMatrix();
}

void home_screen() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(4);
    
    std::stringstream ss;
    
    Image* image;
    glGenTextures(2, texName);
    
    image = loadBMP("/Users/dipit/Downloads/snake-3d-master/snake.bmp");
    loadTexture(image, 1);
    delete image;
    
    ss << "3D SNAKE";
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, ss.str().c_str(), -0.3, 0.3);
    ss.str("");
    ss.clear();
    
    ss << "COMS 557 Project";
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, ss.str().c_str(), -0.5, 0);
    ss.str("");
    ss.clear();
    
    ss << "Press [ENTER] to Play";
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, ss.str().c_str(), -1.5, -0.60);
    ss.str("");
    ss.clear();
    
}

static void Map(void) {
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(0.0, 0.1, 0.7);
    glLineWidth(2);
    
    glBegin(GL_LINES);
    
    for(int i = 0; i <= row; i += 2) {
        glVertex2d(minX, position_x_2d(i));
        glVertex2f(maxX, position_x_2d(i));
        glVertex2d(position_x_2d(i), minY);
        glVertex2f(position_x_2d(i), maxY);
    }
    
    glEnd();
    
    glColor3f(0.0, 0.2, 0.4);
    glLineWidth(3);
    
    glBegin(GL_LINE_LOOP);
    glVertex2f(minX, minY);
    glVertex2f(minX, maxY);
    glVertex2f(maxX, maxY);
    glVertex2f(maxX, minY);
    glEnd();
    
    glLineWidth(2);
    
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(6);
    
    glBegin(GL_POINTS);
    glVertex2f(position_x_2d(point_x), position_y_2d(point_y));
    glEnd();
    
    glPointSize(1);
    
    
    glColor3f(0.9, 0.9, 0.0);
    glLineWidth(unitSize);
    
    glBegin(GL_LINE_STRIP);
    for (int i = player->len- 1; i >= 0; i--) {
        glVertex2f(position_x_2d(player->current_x_At(i)), position_y_2d(player->current_y_At(i)));
    }
    glEnd();
    
    glLineWidth(1);
    
    
    glColor3f(1.0, 1.0, 1.0);
    
    std::stringstream ss;
    ss << "Player Score: " << std::to_string(score);
    drawString(GLUT_BITMAP_9_BY_15, ss.str().c_str(), -0.65, -0.65);
}

static void drawPerspective(void) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    GLfloat ambientLight[] = {0.5f, 0.4f, 0.4f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
    
    GLfloat directedLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat directedLightPos[] = {-10.0f, 15.0f, 20.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
    glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);
    
    
    glColor3f(0, 0.0, 0.7);
    glLineWidth(1);
    
    glBegin(GL_LINES);
    
    for(double i = 0; i <= row; i += 2) {
        glVertex2d(minX, position_x_2d(i));
        glVertex2f(maxX, position_x_2d(i));
        glVertex2d(position_x_2d(i), minY);
        glVertex2f(position_x_2d(i), maxY);
    }
    
    glEnd();
    
    
    glColor3f(0.7, 0.0, 0.7);
    
    glPushMatrix();
    glTranslated(maxX + 0.05, 0.0, 0.0);
    glScaled(1.0, 1.0 * 41, 1.0);
    glutSolidCube(0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(minX - 0.05, 0.0, 0.0);
    glScaled(1.0, 1.0 * 41, 1.0);
    glutSolidCube(0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(0.0, minY - 0.05, 0.0);
    glScaled(1.0 * 43, 1.0, 1.0);
    glutSolidCube(0.05);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(0.0, maxY + 0.05, 0.0);
    glScaled(1.0 * 43, 1.0, 1.0);
    glutSolidCube(0.05);
    glPopMatrix();
    
    glEnable(GL_TEXTURE_2D);
    
    glColor3f(1.5, 0.0, 1.0);
    
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    
    glBindTexture(GL_TEXTURE_2D, texName[1]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    
    glPushMatrix();
    glTranslated(position_x_2d(point_x), position_y_2d(point_y), 0.025);
    glRotated(point_angle, 0.3, 1.0, 0.0);
    glutSolidCube(0.05);
    glPopMatrix();
    
    glColor3f(0.8, 1.0, 0.0);
    
    glBindTexture(GL_TEXTURE_2D, texName[0]);
    
    for (int i = player->len - 1; i >= 0; i--) {
        glPushMatrix();
        glTranslated(position_x_2d(player->current_x_At(i)), position_y_2d(player->current_y_At(i)), 0.025);
        
        if (snakeShape == 0) {
            glutSolidSphere(0.05, 10, 10);
        } else {
            glutSolidCube(0.05);
        }
        
        glPopMatrix();
    }
    
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
    
    
    glColor3f(1.0, 1.0, 1.0);
    
    std::stringstream ss;
    ss << "Score: " << std::to_string(score);
    draw3dString(GLUT_STROKE_MONO_ROMAN, ss.str().c_str(), -0.85, -0.85, 0.0);
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
}

static void display(void) {
    double snakeX, snakeY;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (showSplashScreen) {
        
        gluLookAt(0.0, 0.0, 2.0,
                  0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0);
        
        home_screen();
        
    } else if (showMap) {
        
        gluLookAt(0.0, 0.0, 2.0,
                  0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0);
        
        Map();
        
    } else {
        
        snakeX = position_x_2d(player->x());
        snakeY = position_y_2d(player->y());
        
        snakeY = snakeY <= -0.5 ? -0.5 : snakeY;
        snakeY = snakeY >= 1.5 ? 1.5 : snakeY;
        
        gluLookAt(0.0, -2.0, 1,
                  0.0, snakeY, 0.0,
                  0.0, 0.5, 0.0);
        
        drawPerspective();
        
    }
    
    
    glutSwapBuffers();
}

bool snakeHits(float x, float y) {
    double nx, ny;
    
    nx = player->x();
    ny = player->y();
    
    return nx == x && ny == y;
}

void resetGame() {
    player->reset();
    direction_x = 1;
    direction_y = 0;
}

void myTimer(int arg) {
    int nx, ny;
    
    if (showSplashScreen) {
        glutTimerFunc(tick / speed, myTimer, 1);
        return;
    }
    
    if (direction_x == 1 && player->x() >= row) {
        direction_x = 0;
        direction_y = 1;
    } else if (direction_x == -1 && player->x() <= 0) {
        direction_x = 0;
        direction_y = -1;
    } else if (direction_y == 1 && player->y() >= col) {
        direction_y = 0;
        direction_x = -1;
    } else if (direction_y == -1 && player->y() <= 0) {
        direction_y = 0;
        direction_x = 1;
    }
    
    nx = player->x() + direction_x;
    ny = player->y() + direction_y;
    
    point_angle = (point_angle >= 360) ? 0 : point_angle + 5;
    
    
    if (big == 1 || snakeHits(point_x, point_y)) {
        
        score += (1 * score_mul * speed);
        
        if (!player->full()) {
            player->grab_point();
        } else {
            resetGame();
            
            score_mul++;
            
            tick *= timerMultiplier;
        }
        
        point_x = rand() % row + 1;
        point_y = rand() % col + 1;
        
        big = 0;
    }
    
    if (!player->move_direction_to(direction_x, direction_y)){
        resetGame();
        score = 0;
        score_mul = 1;
        speed = 1.0;
        timerMultiplier = 0.8;
    }
    
    glutPostRedisplay();
    glutTimerFunc(tick / speed, myTimer, 1);
}

void play_keyboard(unsigned char ke, int directionX, int directionY) {
    switch (ke) {
            
        case 'i': case 'I':
            if (direction_y != -1) { direction_x = 0; direction_y = 1; }
            break;
        case 'K': case 'k':
            if (direction_y != 1) { direction_x = 0; direction_y = -1; }
            break;
        case 'J': case 'j':
            if (direction_x != 1) { direction_x = -1; direction_y = 0; }
            break;
        case 'L': case 'l':
            if (direction_x != -1) { direction_x = 1; direction_y = 0; }
            break;
            
            
        case 'm': case 'M':
            showMap = !showMap;
            break;
            
        case 'v':
            speed *= 1.4;
            break;
            
        case 'V':
            speed *= 0.7;
            break;
            
        case 13:
            showSplashScreen = false;
            break;
            
            
        case 27: case 'e': case 'E':
            exit(-1);
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(150, 150);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Snake 3D COMS 557");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(play_keyboard);
    glutTimerFunc(2000, myTimer, 1);
    
    glutMainLoop();
    return EXIT_SUCCESS;
}
