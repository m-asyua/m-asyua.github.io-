#include "sit.h"


int sx=400,sy=200;

void sit_main(){

	point( 50, 50,  5);
	point(100, 50, 10);

	line(50, 100, 300, 100,  5);
	line(50, 150, 300, 150,  10);

	text("test1",  50, 200);
	text("test2", 150, 200);

	rect( 50,  220,  50+50, 220+50,"black");
	rect(150,  220, 150+80, 220+80,"black");
	rect(280,  220, 280+50, 220+50,"black");
	rect(290,  230, 290+30, 230+30,"white");

	imageBmp("test_24bit.bmp", 400, 50, 30,80);
	imageBmp("keshi_24bit.bmp", sx,sy,40,75);

	for(int i=0;i<200;i=i+20){
		rect(50-20+i, 380, 50-20+i+30, 380+80,"white");
		delay(200);
		imageBmp("test_24bit.bmp", 50+i, 380, 30,80);
	}

}

void shoot(int px,int py){
int i=0;
	
	for(i=0;i<50;i=i+2){
		rect(px+10, py-(i-1)*10, px+10+20, py-(i-1)*10+80,"white");
		delay(50);
		imageBmp("keshi_24bit.bmp", px+10,py-i*10,20,40);
	}
		rect(px+10+20, py-i*10, px+10+20, py-i*10+40,"white");
	
}

void KeyDown(unsigned char key){
	printf("key %c\n", key);

	rect(sx,sy,sx+40,sy+75,"white");

	if(key=='d'){ sx +=3;}
	if(key=='a'){ sx -=3;}
	if(key=='s'){ sy +=3;}
	if(key=='w'){ sy -=3;}

	if(key=='x'){ shoot(sx,sy);}

	imageBmp("keshi_24bit.bmp", sx,sy,40,75);

}



