#include <bangtal.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
//#define _CRT_SECURE_NO_WARNINGS
//char buf[256];

TimerID timer1;
TimerID timer2;

SceneID scene1;
ObjectID startButton,restartButton,endButton,level1,level2,level3;
ObjectID s[17];
int X[6] = { 0,0,199,398,597,0 };
int Y[6] = { 0,597,398,199,0,0 };
int YX[6][6] = { {-1,-1,-1,-1,-1,-1}, {-1,1,2,3,4,-1}, {-1,5,6,7,8,-1}, {-1,9,10,11,12,-1}, {-1,13,14,15,0,-1}, {-1,-1,-1,-1,-1,-1} }; 
int i,j,cx,cy,snum=1,success=0;
int r_num,x0=4,y0=4,res_num=0;
int dif=0;
float t=0;
void setS();

void move1(int num1, int x1, int y1,int x2,int y2) {
	YX[y2][x2] = num1;
	YX[y1][x1] = 0;
	locateObject(s[num1], scene1, X[x2], Y[y2]);
	locateObject(s[0], scene1, X[x1], Y[y1]);
}

void move2(int x1, int y1, int x2, int y2) {
	YX[y1][x1] = YX[y2][x2];
	YX[y2][x2] = 0;
	locateObject(s[YX[y1][x1]], scene1, X[x1], Y[y1]);
	locateObject(s[0], scene1, X[x2], Y[y2]);
}

void moveS(int num) {
	for (i = 1; i <= 4; i++) {  
		for (j = 1; j <= 4; j++) {
			if (YX[i][j] == num) {
				cy = i;
				cx = j;
			}
		}
	}
	if (YX[cy - 1][cx] == 0) {
		move1(num, cx, cy, cx, cy - 1);
	}
	else if (YX[cy + 1][cx] == 0) {
		move1(num, cx, cy, cx, cy + 1);
	}
	else if (YX[cy][cx + 1] == 0) {
		move1(num, cx , cy, cx + 1, cy);
	}
	else if (YX[cy][cx - 1] == 0) {
		move1(num, cx , cy, cx - 1 , cy);
	}

}
void arrangeS() {
	for (i = 1; i <= 4; i++) {
		for (j = 1; j <= 4; j++) {
			locateObject(s[YX[i][j]], scene1, X[j], Y[i]);
			showObject(s[YX[i][j]]);
		}
	}
	for (i = 0; i <= dif; i++) { 
		r_num = rand() % 4;
		if (r_num == 0) {
			if (YX[y0 - 1][x0] > 0) {
				move2(x0, y0, x0, y0 - 1);
				y0--;
			}
			else {
				i--;
			}
		}
		else if (r_num == 1) {
			if (YX[y0][x0 + 1] > 0) {
				move2(x0, y0, x0 + 1, y0);
				x0++;
			}
			else {
				i--;
			}
		}
		else if (r_num == 2) {
			if (YX[y0][x0 - 1] > 0) {
				move2(x0, y0, x0 - 1, y0);
				x0--;
			}
			else {
				i--;
			}
		}
		else {
			if (YX[y0 + 1][x0] > 0) {
				move2(x0, y0, x0, y0 + 1);
				y0++;
			}
			else {
				i--;
			}
		}
	}
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == level1) {
		if (dif == 0) {
			showObject(startButton);
		}
		showMessage("난이도 1 (다음 판부터 적용됩니다.)");
		dif = 10;
	}
	else if (object == level2) {
		if (dif == 0) {
			showObject(startButton);
		}
		showMessage("난이도 2 (다음 판부터 적용됩니다.)");
		dif = 50;
	}
	else if (object == level3) {
		if (dif == 0) {
			showObject(startButton);
		}
		showMessage("난이도 3 (다음 판부터 적용됩니다.)");
		dif = 200;
	}
	

	if (object == startButton) {
		timer1 = createTimer(1.0f);
		timer2 = createTimer(t);
		showTimer(timer2);
		startTimer(timer1);
		setSceneImage(scene1, "Images/back1.png");
		hideObject(startButton);
		showObject(endButton);
		showObject(restartButton);
		arrangeS();

	}
	if (object == restartButton) {
		for (i = 1; i <= 4; i++) {
			for (j = 1; j <= 4; j++) {
				res_num++;
				YX[i][j] = res_num;
			}
		}
		YX[4][4] = 0;
		res_num = 0;
		x0 = 4;
		y0 = 4;
		t = 0;
		setTimer(timer1, 1.0f);
		startTimer(timer1);
		arrangeS();
	}
	if (object == endButton) {
		endGame();
	}
	for (int k = 1; k <= 16; k++) {
		if (object == s[k]) {
			moveS(k);
		}
	}
	for (i = 1; i <= 4; i++) {
		for (j = 1; j <= 4; j++) {
			if (YX[i][j] == snum) {
				success++;
			}
			snum++;
		}
	}
	if (success >= 15 && object!=level1 && object!=level2 && object!=level3) {
		stopTimer(timer1);
		showMessage("성공");
		setSceneImage(scene1, "Images/back2.png");
		for (i = 0; i <= 16; i++) {
			hideObject(s[i]);
		}
	}
	snum = 1;
	success = 0;
}

void timerCallback(TimerID timer) {
	if (timer == timer1) {
		t++;
		setTimer(timer1, 1.0f);
		startTimer(timer1);
		setTimer(timer2, t);
	}
}


int main() {
	//printf("%d", t);
	//sprintf_s(buf, "%d", t);
	//showMessage(buf);

	srand(time(NULL));
	setTimerCallback(timerCallback);
	scene1 = createScene("퍼즐 맞추기", "Images/back2.png");
	endButton = createObject("Images/end.png");
	restartButton = createObject("Images/restart.png");
	level1 = createObject("Images/level1.png");
	level2 = createObject("Images/level2.png");
	level3 = createObject("Images/level3.png");
	locateObject(level1, scene1, 800, 500);
	scaleObject(level1, 0.1f);
	locateObject(level2, scene1, 800, 400);
	scaleObject(level2, 0.1f);
	locateObject(level3, scene1, 800, 300);
	scaleObject(level3, 0.1f);
	locateObject(endButton, scene1, 800, 80);
	locateObject(restartButton, scene1, 800, 40);
	showObject(level1);
	showObject(level2);
	showObject(level3);
	startButton = createObject("Images/start.png");
	locateObject(startButton, scene1, 368, 100);
	setS();
	setMouseCallback(mouseCallback);
	showMessage("숫자를 눌러 난이도를 선택해주세요. (난이도는 섞는 횟수를 반영합니다.)");
	startGame(scene1);
}


void setS() {

	s[0] = createObject("Images/h.png");
	s[1] = createObject("Images/1.jpg");
	s[2] = createObject("Images/2.png");
	s[3] = createObject("Images/3.png");
	s[4] = createObject("Images/4.png");
	s[5] = createObject("Images/5.png");
	s[6] = createObject("Images/6.png");
	s[7] = createObject("Images/7.png");
	s[8] = createObject("Images/8.png");
	s[9] = createObject("Images/9.png");
	s[10] = createObject("Images/10.png");
	s[11] = createObject("Images/11.png");
	s[12] = createObject("Images/12.png");
	s[13] = createObject("Images/13.png");
	s[14] = createObject("Images/14.png");
	s[15] = createObject("Images/15.png");
	s[16] = createObject("Images/16.png");
	
}