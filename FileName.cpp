#include<graphics.h>
#include<conio.h>
#include<iostream>
#include<stdlib.h>
#include<cmath>
#include<mmsystem.h>

#pragma comment(lib,"winmm.lib") 

extern int map1[13][13];
extern int map2[13][13];
extern int map3[13][13];
extern int map4[13][13];
extern int map5[13][13];
extern int map6[13][13];
extern int map7[13][13];
extern int map8[13][13];
extern int map9[13][13];
void reStart();
using namespace std;

bool bloodPK_bat(struct Monster monster);
bool blood_cost_bat(struct Monster monster);
int blood_cost_num_bat(struct Monster monster);
void youDead();

int (*map)[13] = map1;
int floor_count = 1;
int max_floor = 9;


struct gameRole {
	char name[100] = "";
	int blood = 400;
	int attack = 25;
	int defence = 25;
	int yellow_key = 10;
	int blue_key = 10;
	int red_key = 10;
	int grade = 0;
	int gold = 0;
	int experience = 0;
	IMAGE role_picture;
}role;

struct Monster {
	int blood;
	int attack;
	int defence;
	int gold_drop;
	int experience_drop;	
	char name[100];
	IMAGE monster_picture;
};
IMAGE img[28], role_info, battle_info;

Monster bat = { 50, 30, 20, 10, 10, "吸血蝙蝠"};
Monster mage = { 1000, 160, 110, 100, 100, "蓝法师"};
Monster red_slime = { 150, 40, 20, 15, 15, "红头怪"};
Monster green_slime = { 100, 35, 20, 12, 12, "绿头怪" };
Monster skeleton_soldier = { 400, 60, 40, 45, 45, "深渊使徒" };
Monster beast_man = { 600, 80, 80, 60, 60, "兽面人" };
Monster yanjiang = { 0, 50, 0, 0, 0,"岩浆" };

HWND hwnd;

void floorSwitchUp() {
	if (floor_count < max_floor) {
		floor_count++;
		mciSendString("stop door.mp3", 0, 0, 0);
		mciSendString("close door.mp3", 0, 0, 0);
		mciSendString("open door.mp3", 0, 0, 0);
		mciSendString("play door.mp3", 0, 0, 0);
		switch (floor_count) {
		case 1:map = map1; break;
		case 2:map = map2; break;
		case 3:map = map3; break;
		case 4:map = map4; break;
		case 5:map = map5; break;
		case 6:map = map6; break;
		case 7:map = map7; break;
		case 8:map = map8; break;
		case 9:map = map9; break;
			//case 10:map = map10; break;
		default:break;
		}
	}
}

void floorSwitchDown() {
	if (floor_count > 1) {
		floor_count--;
		mciSendString("stop door.mp3", 0, 0, 0);
		mciSendString("close door.mp3", 0, 0, 0);
		mciSendString("open door.mp3", 0, 0, 0);
		mciSendString("play door.mp3", 0, 0, 0);
		switch (floor_count) {
		case 1:map = map1; break;
		case 2:map = map2; break;
		case 3:map = map3; break;
		case 4:map = map4; break;
		case 5:map = map5; break;
		case 6:map = map6; break;
		case 7:map = map7; break;
		case 8:map = map8; break;
		case 9:map = map9; break;
			//case10:map = map10; break;
		default:break;
		}
	}
}


//加载图片
void loadResource() {
	loadimage(&role_info, "info.png", 60 * 4.5, 60 * 13);
	loadimage(&battle_info, "battle.png", 601, 262);
	for (int i = 0; i <= 27; i++) {
		char fileName[20] = "";
		sprintf_s(fileName, "%d.png", i);
		loadimage(img + i, fileName, 60, 60);
	}
}

//把整型转化为字符串
char str[20] = "";
char* intToChar(int Num) {
	sprintf_s(str, "%d", Num);
	return str;
}

//绘制地图
void drawMap() {
	int i, j;
	int x, y;
	for (i = 0; i <= 12; i++) {
		for (j = 0; j <= 12; j++) {
			x = 60 * j;
			y = 60 * i;
			putimage(x, y, &img[map[i][j]]);
		}
	}
	//边栏数据
	putimage(60 * 13, 0, &role_info);
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextstyle(45, 0, "华文行楷");
	outtextxy(60 * 15, 35, role.name);
	settextstyle(35, 0, "华文行楷", 0, 0, 0, FALSE, FALSE, FALSE);
	outtextxy(60 * 15 + 22, 130, intToChar(role.blood));
	outtextxy(60 * 15 + 22, 178, intToChar(role.attack));
	outtextxy(60 * 15 + 22, 225, intToChar(role.defence));
	outtextxy(60 * 15 + 22, 272, intToChar(role.experience));
	outtextxy(60 * 15 + 22, 320, intToChar(role.gold));
	settextstyle(55, 0, "黑体", 0, 0, 1000, TRUE, FALSE, FALSE);
	outtextxy(60 * 15, 395, intToChar(role.yellow_key));
	outtextxy(60 * 15, 460, intToChar(role.blue_key));
	outtextxy(60 * 15, 530, intToChar(role.red_key));
	settextstyle(47, 0, "黑体");
	outtextxy(60 * 13.5, 605, "第");
	outtextxy(60 * 16, 605, "层");
	settextstyle(55, 0, "华文行楷", 0, 0, 0, FALSE, FALSE, FALSE);
	outtextxy(60 * 15, 605, intToChar(floor_count));
	settextstyle(30, 0, "华文行楷", 0, 0, 0, FALSE, FALSE, FALSE);
	outtextxy(60 * 13, 695, "‘L’打开怪物手册");
	outtextxy(60 * 13, 735, "‘R’重新开始");
}

bool blood_cost(struct Monster monster) {
	int role_attack = role.attack - monster.defence;
	int monster_attack = monster.attack - role.defence;
	if (role_attack <= 0) return false;
	if (monster_attack < 0) monster_attack = 0;

	int role_blood = role.blood;
	int monster_blood = monster.blood;

	while (role_blood > 0 && monster_blood > 0) {

		monster_blood -= role_attack;
		if (monster_blood > 0) role_blood -= monster_attack;
	
	}
	if (role_blood <= 0) {
		return false;
	}
	else return true;
}

int blood_cost_num(struct Monster monster) {
	int sum = 0;
	int role_attack = role.attack - monster.defence;
	int monster_attack = monster.attack - role.defence;
	if (role_attack <= 0) return -1;
	if (monster_attack < 0) monster_attack = 0;
	int role_blood_cost = 0;
	int monster_blood = monster.blood;

	while (monster_blood > 0) {

		monster_blood -= role_attack;
		if (monster_blood > 0) {
			role_blood_cost += monster_attack;
		}
	}
	return role_blood_cost;
}

//战斗弹窗
void popUP(struct Monster monster) {
	putimage(100, 100, &battle_info);
	putimage(117, 137, &monster.monster_picture);
	putimage(586, 137, &role.role_picture);
	settextstyle(30, 0, "华文行楷", 0, 0, 0, FALSE, FALSE, FALSE);
	outtextxy(267, 161, intToChar(monster.blood));
	outtextxy(267, 222, intToChar(monster.attack));
	outtextxy(267, 284, intToChar(monster.defence));
	outtextxy(507, 161, intToChar(role.blood));
	outtextxy(507, 222, intToChar(role.attack));
	outtextxy(507, 284, intToChar(role.defence));
}
//战斗机制
bool bloodPK(struct Monster monster) {
	int role_attack = role.attack - monster.defence;
	int monster_attack = monster.attack - role.defence;
	if (monster_attack < 0) monster_attack = 0;
	if (blood_cost(monster)) {
		while (role.blood > 0 && monster.blood > 0) {

			mciSendString("stop attack.mp3", 0, 0, 0);
			mciSendString("close attack.mp3", 0, 0, 0);
			monster.blood -= role_attack;
			if (monster.blood > 0) role.blood -= monster_attack;
			else {
				monster.blood = 0;
				drawMap();
				popUP(monster);
				break;
			}
			mciSendString("open attack.mp3", 0, 0, 0);
			mciSendString("play attack.mp3", 0, 0, 0);			
			drawMap();
			popUP(monster);
			Sleep(340);
		}
		mciSendString("stop door.mp3", 0, 0, 0);
		mciSendString("close door.mp3", 0, 0, 0);
		role.experience += monster.experience_drop;
		role.gold += monster.gold_drop;
		mciSendString("open door.mp3", 0, 0, 0);
		mciSendString("play door.mp3", 0, 0, 0);
		Sleep(400);
		mciSendString("stop win.mp3", 0, 0, 0);
		mciSendString("close win.mp3", 0, 0, 0);
		mciSendString("open win.mp3", 0, 0, 0);
		mciSendString("play win.mp3", 0, 0, 0);
		settextstyle(45, 0, "华文行楷", 0, 0, 0, FALSE, FALSE, FALSE);
		settextcolor(BLUE);
		outtextxy(300, 300, "获得胜利！");
		Sleep(1000);
		while (_kbhit()) {
			_getch();
		}
		return true;

	}
	else return false;
}

void monsterMenu();

bool is_end = false;

void eventKeyDown(int k, int i, int j) {
	switch (k) {
	case 1:j = j - 1; break;
	case 2:j = j + 1; break;
	case 3:i = i - 1; break;
	case 4:i = i + 1; break;
	default: break;
	}
	if (map[i][j] == 4) {
		mciSendString("stop pick.mp3", 0, 0, 0);
		mciSendString("close pick.mp3", 0, 0, 0);
		map[i][j] = 1;
		role.blue_key++;
		mciSendString("open pick.mp3", 0, 0, 0);
		mciSendString("play pick.mp3", 0, 0, 0);
	}
	else if (map[i][j] == 5) {
		mciSendString("stop pick.mp3", 0, 0, 0);
		mciSendString("close pick.mp3", 0, 0, 0);
		map[i][j] = 1;
		role.yellow_key++;
		mciSendString("open pick.mp3", 0, 0, 0);
		mciSendString("play pick.mp3", 0, 0, 0);
	}
	else if (map[i][j] == 26) {
		mciSendString("stop pick.mp3", 0, 0, 0);
		mciSendString("close pick.mp3", 0, 0, 0);
		map[i][j] = 1;
		role.red_key++;
		mciSendString("open pick.mp3", 0, 0, 0);
		mciSendString("play pick.mp3", 0, 0, 0);
	}
	else if (map[i][j] == 2 && role.blue_key > 0) {
		mciSendString("stop door.mp3", 0, 0, 0);
		mciSendString("close door.mp3", 0, 0, 0);
		map[i][j] = 1;
		role.blue_key--;
		mciSendString("open door.mp3", 0, 0, 0);
		mciSendString("play door.mp3", 0, 0, 0);
	}
	else if (map[i][j] == 3 && role.yellow_key > 0) {
		mciSendString("stop door.mp3", 0, 0, 0);
		mciSendString("close door.mp3", 0, 0, 0);
		map[i][j] = 1;
		role.yellow_key--;
		mciSendString("open door.mp3", 0, 0, 0);
		mciSendString("play door.mp3", 0, 0, 0);
	}
	else if (map[i][j] == 12 && role.red_key > 0) {
		mciSendString("stop door.mp3", 0, 0, 0);
		mciSendString("close door.mp3", 0, 0, 0);
		map[i][j] = 1;
		role.red_key--;
		mciSendString("open door.mp3", 0, 0, 0);
		mciSendString("play door.mp3", 0, 0, 0);
	}
	else if (map[i][j] == 10) {
		//int ID = MessageBox(hwnd, "遇到小蝙蝠", "是否攻击小蝙蝠", MB_YESNO);
		//if (ID == IDYES) {
		if (bloodPK_bat(bat)) {
			map[i][j] = 1;
		}
		else youDead();
		//else {
		//	int beginID = MessageBox(hwnd, "你阵亡了", "是否复活", MB_YESNO);
		//}
	//}
	}
	else if (map[i][j] == 25) {
		mciSendString("stop win.mp3", 0, 0, 0);
		mciSendString("close win.mp3", 0, 0, 0);
		mciSendString("open win.mp3", 0, 0, 0);
		mciSendString("play win.mp3", 0, 0, 0);
		int PD = MessageBox(hwnd, "胜利！\n “是”退出游戏 \n “否”继续体验地图", "恭喜你！", MB_YESNO);
		if (PD == IDYES) is_end = true;
		else is_end=false;
		while (_kbhit()) {
			_getch();
		}
	}
	else if (map[i][j] == 18) {
		if (bloodPK(mage)) {
			map[i][j] = 1;
		}
		else youDead();
	}
	else if (map[i][j] == 8) {
		if (bloodPK(red_slime)) {
			map[i][j] = 1;
		}
		else youDead();
	}
	else if (map[i][j] == 9) {
		if (bloodPK(green_slime)) {
			map[i][j] = 1;
		}
		else youDead();
	}
	else if (map[i][j] == 11) {
		if (bloodPK(skeleton_soldier)) {
			map[i][j] = 1;
		}
		else youDead();
	}
	else if (map[i][j] == 24) {
		if (bloodPK(beast_man)) {
			map[i][j] = 1;
		}
		else youDead();
	}

	else if (map[i][j] == 13) {
		mciSendString("stop pick.mp3", 0, 0, 0);
		mciSendString("close pick.mp3", 0, 0, 0);
		map[i][j] = 1;
		role.attack += 3;
		mciSendString("open pick.mp3", 0, 0, 0);
		mciSendString("play pick.mp3", 0, 0, 0);
	}
	else if (map[i][j] == 23) {
		mciSendString("stop win.mp3", 0, 0, 0);
		mciSendString("close win.mp3", 0, 0, 0);
		map[i][j] = 1;
		role.attack += 10;
		mciSendString("open win.mp3", 0, 0, 0);
		mciSendString("play win.mp3", 0, 0, 0);
	}
	else if (map[i][j] == 22) {
		mciSendString("stop win.mp3", 0, 0, 0);
		mciSendString("close win.mp3", 0, 0, 0);
		map[i][j] = 1;
		role.defence += 10;
		mciSendString("open win.mp3", 0, 0, 0);
		mciSendString("play win.mp3", 0, 0, 0);
	}
	else if (map[i][j] == 14) {
		mciSendString("stop pick.mp3", 0, 0, 0);
		mciSendString("close pick.mp3", 0, 0, 0);
		map[i][j] = 1;
		role.defence += 3;
		mciSendString("open pick.mp3", 0, 0, 0);
		mciSendString("play pick.mp3", 0, 0, 0);
	}
	else if (map[i][j] == 6) {
		mciSendString("stop pick.mp3", 0, 0, 0);
		mciSendString("close pick.mp3", 0, 0, 0);
		map[i][j] = 1;
		role.blood += 200;
		mciSendString("open pick.mp3", 0, 0, 0);
		mciSendString("play pick.mp3", 0, 0, 0);
	}
	else if (map[i][j] == 7) {
		mciSendString("stop pick.mp3", 0, 0, 0);
		mciSendString("close pick.mp3", 0, 0, 0);
		map[i][j] = 1;
		role.blood += 500;
		mciSendString("open pick.mp3", 0, 0, 0);
		mciSendString("play pick.mp3", 0, 0, 0);
	}
	else if (map[i][j] == 16) {
		floorSwitchUp();
	}
	else if (map[i][j] == 17) {
		floorSwitchDown();
	}
}


//处理键盘事件
void keyDown() {
	char userKey;
	int i = 1, j = 1;
	//找到骑士位置
	for (i = 1; i < 12; i++) {
		for (j = 1; j < 12; j++) {
			if (map[i][j] == 15 || map[i][j] == 19 || map[i][j] == 20 || map[i][j] == 21) {
				break;
			}
		}
		if (map[i][j] == 15 || map[i][j] == 19 || map[i][j] == 20 || map[i][j] == 21) {
			break;
		}
	}

	userKey = _getch();


	switch (userKey) {
		//左
	case'A':
	case'a':
		map[i][j] = 20;
		drawMap();
		if (map[i][j - 1] == 1) {
			mciSendString("stop walk.mp3", 0, 0, 0);
			mciSendString("close walk.mp3", 0, 0, 0);
			map[i][j] = 1;
			map[i][j - 1] = 20;
			role.blood -= 50;
			mciSendString("open walk.mp3", 0, 0, 0);
			mciSendString("play walk.mp3 from 3", 0, 0, 0);
		}
		else if (map[i][j - 1] == 27) {
			mciSendString("stop walk.mp3", 0, 0, 0);
			mciSendString("close walk.mp3", 0, 0, 0);
			map[i][j] = 1;
			map[i][j - 2] = 20;
			mciSendString("open walk.mp3", 0, 0, 0);
			mciSendString("play walk.mp3 from 3", 0, 0, 0);
		}
		else if(i != 0)eventKeyDown(1, i, j);
		break;


		//右
	case'D':
	case'd':
		map[i][j] = 19;
		drawMap();
		if (map[i][j + 1] == 1) {
			mciSendString("stop walk.mp3", 0, 0, 0);
			mciSendString("close walk.mp3", 0, 0, 0);
			map[i][j] = 1;
			map[i][j + 1] = 19;
			mciSendString("open walk.mp3", 0, 0, 0);
			mciSendString("play walk.mp3", 0, 0, 0);
		}
		else if (map[i][j + 1] == 27) {
			mciSendString("stop walk.mp3", 0, 0, 0);
			mciSendString("close walk.mp3", 0, 0, 0);
			map[i][j] = 1;
			map[i][j + 2] = 19;
			role.blood -= 50;
			mciSendString("open walk.mp3", 0, 0, 0);
			mciSendString("play walk.mp3", 0, 0, 0);
		}
		else if (i != 0)eventKeyDown(2, i, j);
		break;


		//上
	case'W':
	case'w':
		map[i][j] = 21;
		drawMap();
		if (map[i - 1][j] == 1) {
			mciSendString("stop walk.mp3", 0, 0, 0);
			mciSendString("close walk.mp3", 0, 0, 0);
			map[i][j] = 1;
			map[i - 1][j] = 21;
			mciSendString("open walk.mp3", 0, 0, 0);
			mciSendString("play walk.mp3", 0, 0, 0);
		}
		else if (i != 0)eventKeyDown(3, i, j);
		break;


		//下
	case'S':
	case's':
		map[i][j] = 15;
		drawMap();
		if (map[i + 1][j] == 1) {
			mciSendString("stop walk.mp3", 0, 0, 0);
			mciSendString("close walk.mp3", 0, 0, 0);
			map[i][j] = 1;
			map[i + 1][j] = 15;
			mciSendString("open walk.mp3", 0, 0, 0);
			mciSendString("play walk.mp3", 0, 0, 0);

		}
		else if (i != 0)eventKeyDown(4, i, j);
		break;

		//图鉴
	case'L':
	case'l':
	
		mciSendString("stop door.mp3", 0, 0, 0);
		mciSendString("close door.mp3", 0, 0, 0);		
		mciSendString("open door.mp3", 0, 0, 0);
		mciSendString("play door.mp3", 0, 0, 0);
		cleardevice();
		monsterMenu();
		mciSendString("stop 1.mp3", 0, 0, 0);
		mciSendString("close 1.mp3", 0, 0, 0);
		mciSendString("open 2.mp3", 0, 0, 0);
		mciSendString("play 2.mp3 repeat from 0", 0, 0, 0);
		while (_getch() != 'l' && _getch() != 'L') {
			;
		}	
		drawMap();

		mciSendString("stop door.mp3", 0, 0, 0);
		mciSendString("close door.mp3", 0, 0, 0);
		mciSendString("open door.mp3", 0, 0, 0);
		mciSendString("play door.mp3", 0, 0, 0);
		mciSendString("stop 2.mp3", 0, 0, 0);
		mciSendString("close 2.mp3", 0, 0, 0);
		mciSendString("open 1.mp3", 0, 0, 0);
		mciSendString("play 1.mp3 repeat from 0", 0, 0, 0);
		break;

		//退出
	case'r':
	case'R':
		//is_end = true;
		mciSendString("stop win.mp3", 0, 0, 0);
		mciSendString("close win.mp3", 0, 0, 0);
		reStart();
		role.blood = 400;
		role.attack = 25;
		role.defence = 25;
		role.yellow_key = 0;
		role.blue_key = 0;
		role.red_key = 0;
		role.grade = 0;
		role.gold = 0;
		role.experience = 0;
		map = map1;
		floor_count = 1;
		mciSendString("open win.mp3", 0, 0, 0);
		mciSendString("play win.mp3", 0, 0, 0);
		break;

	case'P':
	case'p':
		mciSendString("stop win.mp3", 0, 0, 0);
		mciSendString("close win.mp3", 0, 0, 0);
		role.blood += 100;
		role.experience -= 50;
		role.gold -= 50;
		mciSendString("open win.mp3", 0, 0, 0);
		mciSendString("play win.mp3", 0, 0, 0);
		break;

	default:break;
	}
}

void inputRoleName() {
	char s[100];
	InputBox(s, 100, "请输入你的名字：", "欢迎来到魔塔", "不超过三个字");
	sprintf_s(role.name, s);
}

void drawMonsterMenu(struct Monster monster, int y) {
	putimage(0, y, &monster.monster_picture);
	settextstyle(28, 0, "黑体", 0, 0, 0, FALSE, FALSE, FALSE);
	outtextxy(120, y + 11, "名称");
	outtextxy(120, y + 57, "血量");
	outtextxy(420, y + 11, "攻击");
	outtextxy(420, y + 57, "防御");
	outtextxy(720, y + 11, "金-经");
	outtextxy(720, y + 57, "损失");
	settextstyle(40, 0, "华文行楷", 0, 0, 0, FALSE, FALSE, FALSE);
	outtextxy(220, y + 11, monster.name);
	outtextxy(220, y + 54, intToChar(monster.blood));
	outtextxy(520, y + 11, intToChar(monster.attack));
	outtextxy(520, y + 54, intToChar(monster.defence));
	char temp[50] = "";
	sprintf_s(temp, "%d-%d", monster.gold_drop, monster.experience_drop);
	outtextxy(820, y + 11, temp);
	if (blood_cost_num(monster) != -1) {
		if(monster.blood != 50 && monster.blood != 0)outtextxy(820, y + 54, intToChar(blood_cost_num(monster)));
		else if(monster.blood == 50) outtextxy(820, y + 54, intToChar(blood_cost_num_bat(monster)));
		else if (monster.blood == 0) outtextxy(820, y + 54, intToChar(50 - role.defence));
	}
	else outtextxy(820, y + 54, "? ? ?");
}

void monsterMenu() {

	int t = 0;
	int is_used[40] = { 0 };
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 13; j++) {
			int y = 140 * t + 20;

			switch (map[i][j]) {
			case 8:
				if (is_used[map[i][j]] == 0) {
					drawMonsterMenu(red_slime, y);
					t++;
					is_used[map[i][j]] = 1;			
				}
			break;

			case 9:
				if (is_used[map[i][j]] == 0) {
					drawMonsterMenu(green_slime, y);
					t++;
					is_used[map[i][j]] = 1;
				}
				break;
			case 10:
				if (is_used[map[i][j]] == 0) {
					drawMonsterMenu(bat, y);
					t++;
					is_used[map[i][j]] = 1;
				}
				break;
			case 11:
				if (is_used[map[i][j]] == 0) {
					drawMonsterMenu(skeleton_soldier, y);
					t++;
					is_used[map[i][j]] = 1;
				}
				break;
			case 18:
				if (is_used[map[i][j]] == 0) {
					drawMonsterMenu(mage, y);
					t++;
					is_used[map[i][j]] = 1;
				}
				break;

			case 24:
				if (is_used[map[i][j]] == 0) {
					drawMonsterMenu(beast_man, y);
					t++;
					is_used[map[i][j]] = 1;
				}
				break;

			case 27:
				if (is_used[map[i][j]] == 0) {
					drawMonsterMenu(yanjiang, y);
					t++;
					is_used[map[i][j]] = 1;
				}
				break;

			default:break;
			}
		}
	}
}

int main() {

	mciSendString("open 1.mp3", 0, 0, 0);
	mciSendString("play 1.mp3 repeat from 0" , 0, 0, 0);
	loadResource();
	loadimage(&bat.monster_picture, "10.png", 100, 100);
	loadimage(&red_slime.monster_picture, "8.png", 100, 100);
	loadimage(&green_slime.monster_picture, "9.png", 100, 100);
	loadimage(&skeleton_soldier.monster_picture, "11.png", 100, 100);
	loadimage(&mage.monster_picture, "18.png", 100, 100);
	loadimage(&role.role_picture, "15.png", 100, 100);
	loadimage(&beast_man.monster_picture, "24.png", 100, 100);
	loadimage(&yanjiang.monster_picture, "27.png", 100, 100);
	initgraph(60 * 17.5, 60 * 13);

	inputRoleName();

	while (1) {
		drawMap();
		keyDown();
		if (is_end)return 0;
	}
	mciSendString("stop 1.mp3", 0, 0, 0);
	mciSendString("close 1.mp3", 0, 0, 0);

	closegraph();
	return 0;
}

int blood_cost_num_bat(struct Monster monster) {
	int sum = 0;
	int role_attack = role.attack - monster.defence;
	int monster_attack = monster.attack - role.defence;
	if (role_attack <= 0) return -1;
	if (monster_attack < 0) monster_attack = 0;
	int role_blood_cost = 0;
	int monster_blood = monster.blood;

	while (monster_blood > 0) {

		monster_blood -= role_attack;
		if (monster_blood > 0) {
			role_blood_cost += monster_attack;
			if (sum == 3) {
				sum = 0;
				role_blood_cost += 10;

			}
			else {
				sum++;
			}
		}
	}
	return role_blood_cost;
}

bool blood_cost_bat(struct Monster monster) {
	int sum = 0;
	int role_attack = role.attack - monster.defence;
	int monster_attack = monster.attack - role.defence;
	if (role_attack <= 0) return false;
	if (monster_attack < 0) monster_attack = 0;

	int role_blood = role.blood;
	int monster_blood = monster.blood;

	while (role_blood > 0 && monster_blood > 0) {

		monster_blood -= role_attack;
		if (monster_blood > 0) {
			role_blood -= monster_attack;
			if (sum == 3) {
				sum = 0;
				role_blood -= 10;
			}
			else {
				sum++;
			}
		}
	}
	if (role_blood <= 0) {
		return false;
	}
	else return true;
}

bool bloodPK_bat(struct Monster monster) {
	int sum = 0;
	int role_attack = role.attack - monster.defence;
	int monster_attack = monster.attack - role.defence;
	if (monster_attack < 0) monster_attack = 0;
	if (blood_cost_bat(monster)) {
		while (role.blood > 0 && monster.blood > 0) {

			mciSendString("stop attack.mp3", 0, 0, 0);
			mciSendString("close attack.mp3", 0, 0, 0);
			monster.blood -= role_attack;
			if (monster.blood > 0) {
				role.blood -= monster_attack;
				if (sum == 3) {
					sum = 0;
					role.blood -= 10;
				}
				else {
					sum++;
				}
			}
			else {
				monster.blood = 0;
				drawMap();
				popUP(monster);
				break;
			}
			mciSendString("open attack.mp3", 0, 0, 0);
			mciSendString("play attack.mp3", 0, 0, 0);
			drawMap();
			popUP(monster);
			Sleep(340);
		}
		mciSendString("stop door.mp3", 0, 0, 0);
		mciSendString("close door.mp3", 0, 0, 0);
		role.experience += monster.experience_drop;
		role.gold += monster.gold_drop;
		mciSendString("open door.mp3", 0, 0, 0);
		mciSendString("play door.mp3", 0, 0, 0);
		Sleep(400);
		mciSendString("stop win.mp3", 0, 0, 0);
		mciSendString("close win.mp3", 0, 0, 0);
		mciSendString("open win.mp3", 0, 0, 0);
		mciSendString("play win.mp3", 0, 0, 0);
		settextstyle(45, 0, "华文行楷", 0, 0, 0, FALSE, FALSE, FALSE);
		settextcolor(BLUE);
		outtextxy(300, 300, "获得胜利！");
		Sleep(1000);
		while (_kbhit()) {
			_getch();
		}
		return true;

	}
	else return false;
}

void youDead() {
	int beginID = MessageBox(hwnd, "是否复活", "胜败乃兵家常事", MB_YESNO);
	if (beginID == IDYES) {
		reStart();
	}
	else is_end = true;
}


