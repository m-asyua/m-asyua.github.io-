#include <GL/glut.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


#define WIDTH  640
#define HEIGHT 480


void sit_main();
void KeyDown(unsigned char);


void keyboard(unsigned char key, int x, int y){
	KeyDown(key);
}

void delay(int time){
	Sleep(time);

/*
	pid_t ret = fork();
    if (ret == -1)
        printf("fork() failed");
    if (ret == 0){
    	//child
    	Sleep(time);
    } else {
    	//parent
    	return;
    }
*/

}

void text(char* str,int x, int y){
	glColor4f(0.0f,0.0f,0.0f,1.0f);
	glRasterPos2f(x, y);
	for(int i = 0; i < strlen(str); i++){   // 一文字ずつ描画
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	}
}	

// 
// https://w.atwiki.jp/opengl/pages/67.html
// を参考
//
class BMP{
	public:
		unsigned long sizeX,sizeY;
		char *Data;
		GLuint texture;
		BMP(char *FileName);
		bool Load(char *filename);
		void TexSet();
};

BMP::BMP(char *FileName){
	Load(FileName);
	TexSet();
}

bool BMP::Load(char *FileName) {

	FILE *File;
	unsigned long size,i;
	unsigned short int planes,bpp;
	char temp;
	
	if((File = fopen(FileName, "rb")) == NULL){
		printf("file open error");
		return false;
	}

	
	fseek(File,  18,SEEK_CUR);

	fread(&sizeX, 4, 1, File);
	fread(&sizeY, 4, 1, File);
	fread(&planes,2, 1, File);
	fread(&bpp,   2, 1, File);

	
	if(bpp != 24){        // 24bppでなければエラー
		printf("24 bit error");
		return false;
	}

	fseek(File, 24, SEEK_CUR);

	size = sizeX * sizeY * 3;
	Data = (char *) malloc(size);

	if( (i = fread(Data, size, 1, File)) != 1) {
		printf("read data err");
		return false;
	}

	for (i=0;i<size;i+=3) {   // bgrからrgb入替
		temp = Data[i];
		Data[i] = Data[i+2];
		Data[i+2] = temp;
	}

	return true;
}


void BMP::TexSet(){
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
	glBindTexture(GL_TEXTURE_2D, 0);
}




/*
void display(void){
}
*/

/*
void idle(void){
	glutPostRedisplay();
}
*/

//void my_init(){
//	glClearColor(0.0, 0.0, 0.0, 1.0);
//	glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
//	bmp = new BMP("sample.bmp");
//}

void rect(int x1,int y1,int x2, int y2,char *ColorName){

	if(strcmp(ColorName,"Black")==0 || strcmp(ColorName,"black")==0 ){
		glColor4f(0.0f,0.0f,0.0f,1.0f);
	}else if(strcmp(ColorName,"White")==0 || strcmp(ColorName,"white")==0){
		glColor4f(1.0f,1.0f,1.0f,1.0f);
	}else{
		printf("color is not defined. use black");
		glColor4f(0.0f,0.0f,0.0f,1.0f);
	}

	glBegin(GL_QUADS);
	glVertex2i(x1,y1);
	glVertex2i(x2,y1);
	glVertex2i(x2,y2);
	glVertex2i(x1,y2);
	glEnd();
}


void point(int x,int y,float size){

	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex2i(x,y);
	glEnd();
		glFlush();


}

void line(double x1, double y1, double x2, double y2, double size){

	double x_a, x_b, y_a, y_b;
	int i = 0;
	int j = 0;
	
	if(x1 > x2){
		x_a = x2;
		x_b = x1;
	}else{
		x_a = x1;
		x_b = x2;
	}

	if(y1 > y2){
		y_a = y2;
		y_b = y1;
	}else{
		y_a = y1;
		y_b = y2;
	}

	double p1 = (y_b-y_a)/(x_b-x_a);
	
	for(i=x_a;i<x_b;i++){
		j =(int)( p1*(double)i) + y_a - (int)(x_a*p1);
		point(i,j,size);
	}
}


BMP *bmp;

void imageBmp(char *FileName, int x, int y, int width, int height){

	bmp = new BMP(FileName);

	glColor4f(1.0f,1.0f,1.0f,0.7f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0.0, WIDTH, HEIGHT, 0.0, -1.0, 1.0);

	glEnable(GL_TEXTURE_2D);     //テクスチャ有効
	glBindTexture( GL_TEXTURE_2D, bmp->texture );
	glEnable(GL_ALPHA_TEST);    //アルファテスト開始
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f); glVertex2d(x , y+height);       //左下
		glTexCoord2f(0.0f, 1.0f); glVertex2d(x ,  y);             //左上
		glTexCoord2f(1.0f, 1.0f); glVertex2d( x+width ,  y);      //右上
		glTexCoord2f(1.0f, 0.0f); glVertex2d( x+width , y+height);//右下
	glEnd();
	glDisable(GL_ALPHA_TEST);    //アルファテスト終了
	glDisable(GL_TEXTURE_2D);    //テクスチャ無効
	glutSwapBuffers();

}


int display_flg = 0;

void display(void){

	if(display_flg == 0){

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f(0.0f,0.0f,0.0f,1.0f);
	
		sit_main();

		display_flg = 1;
	}

}




int sit_init(int argc, char *argv[]){

	glutInitWindowPosition(100,100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow("sit test");
	glutDisplayFunc(display);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
//	my_init();
	
	glutKeyboardFunc(keyboard); // キーボード（コールバック）
	
	glutMainLoop();

	return 0;
}

int main (int argc, char *argv[]) {

	sit_init(argc, argv);

	return 0;

}
