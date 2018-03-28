#include <iostream>
#include <windows.h>
#include <time.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <conio.h>

using namespace std;
#define NX 20 //内部宽
#define NY 26 //内部高

#define DIS 5 //墙距离边界
#define WHITE FOREGROUND_RED + FOREGROUND_BLUE + FOREGROUND_GREEN

using namespace std;

bool operator!=(COORD a, COORD b)
{
	return !((a.X == b.X) && (a.Y == b.Y));
}

int scoreCheck;
int level = 0;
const int ai_level = 1;
const int win = 250;
int volume = 20000;
COORD t[2]; //tail
COORD a[3]; //cg
enum direction
{
	Left,
	Right,
	Up,
	Down
};
int heading;

//基本操作
void gotoxy(int x, int y) //位置函数
{
	COORD pos;
	pos.X = 2 * x + DIS;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void gotoxy(COORD pos) //位置函数
{

	pos.X = 2 * pos.X + DIS;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void color(int a) //颜色函数
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a | FOREGROUND_INTENSITY);
}

void pause()
{
	while (1)
	{
		_getch();
		break;
	}
}

//基本元素：
struct player //类型,状态与初始坐标
{
	int color;
	int score;
	int hp;
	int posX;
	int posY;
	bool on; //是否在棍子上

} p, ai;
struct square //类型,与初始坐标
{
	int color;

	int posX;
	int posY;
} squ[5];

bool judge()
{

	for (int i = 0; i < 5; i++)
	{
		if (squ[i].posY == p.posY && squ[i].posX == p.posX + 1)
			return 0;

		if (squ[i].posY == p.posY && squ[i].posX + 3 == p.posX - 1)
			return 0;
	}

	return 1;

} //梯子的左右时不能移动

int judgeleft()
{
	if (level >= ai_level)
	{
		for (int i = 0; i < 2; i++)
		{

			if (t[i].Y == p.posY && t[i].X == (p.posX - 1))
				return 0;
		}
	}
	return 1;
}
int judgeright()
{
	if (level >= ai_level)
	{
		for (int i = 0; i < 2; i++)
		{

			if (t[i].Y == p.posY && t[i].X == (p.posX + 1))
				return 0;
		}
	}
	return 1;
}

void setsquare(square &squ, int a)
{

	squ.posX = rand() % 17 + 1;
	squ.posY = a;
	squ.color = rand() % 16;
}
void show_score()
{
	gotoxy(NX + 3 + 8, 4);
	cout << p.score;
	scoreCheck = p.score % 26;
	if (scoreCheck == 15)
		level++;
	gotoxy(NX + 3 + 8, 2);
	cout << level;
}

void showhp()
{
	gotoxy(NX + 3, 3);
	cout << "HP: ";
	for (int i = 0; i < p.hp; i++)
	{
		color(FOREGROUND_RED);
		cout << "▆";
		color(WHITE);
	}
	for (int i = p.hp; i < 10; i++)
	{

		cout << " ";
	}
}
void p_init()
{

	p.on = 1;
	p.posX = squ[2].posX + 1;
	p.posY = squ[2].posY - 1;
	p.hp = 5;
	p.score = 0;
	showhp();
}

void s_init()
{
	for (int u = 0; u < 5; u++)
	{
		setsquare(squ[u], 6 + 5 * u);
	}
}

void ai_init()
{

	ai.posX = rand() % NX + 1;
	ai.posY = 1;
	t[0].X = ai.posX;
	t[0].Y = ai.posY;
}

void wallinit()
{
	color(3);
	gotoxy(0, 0);
	for (int i = 0; i <= NX + 1; i++)
	{

		cout << "□";
	}
}

void init()
{
	wallinit();

	for (int i = 1; i <= NY + 1; i++)
	{
		gotoxy(0, i);
		cout << "□";
		gotoxy(NX + 1, i);
		cout << "□";
	}
	level = 0;
	s_init();
	p_init();
	gotoxy(NX + 3, 2);

	cout << "Level :";
	gotoxy(NX + 3, 4);
	cout << "Power : ";
	gotoxy(NX + 3, 5);
	cout << "p暂停，q退出";

	gotoxy(NX + 3, 6);

	cout << "规则：";
	gotoxy(NX + 3, 7);
	cout << "碰到顶部伤害：2";
	gotoxy(NX + 3, 8);
	cout << "碰到蛇伤害:1 仅蛇头具有伤害";
	gotoxy(NX + 3, 9);
	cout << "到底部死亡";
	gotoxy(NX + 3, 10);
	cout << "在特定的积木上会有特别的效果";
	gotoxy(NX + 3, 11);
	cout << "速度会随着level增加而增加";

	PlaySound(TEXT("./music.wav"), GetModuleHandle(0), SND_FILENAME | SND_ASYNC | SND_LOOP);
	waveOutSetVolume(0, volume);
	ai_init();
}
void restart()
{
	if (p.score > win)
	{
		color(FOREGROUND_RED);
		gotoxy(5, 15);
		cout << "Congratulation!!You have got enough power.You Win";
	}
	else
	{
		color(WHITE);
		gotoxy(5, 15);
		cout << "Sorry.You have not got enough power. You Lose";
	}

	gotoxy(5, 16);
	cout << "Y再来一次 否则退出";
	char a = getchar();
	cin.get();

	if (a == 'y' || a == 'Y')
	{

		system("cls");
		init();
	}
	else
	{

		gotoxy(5, 17);

		exit(0);
	}
}

//对player操作：：
void pcs(struct player p) //清除player
{
	gotoxy(p.posX, p.posY);
	cout << ' ';
}

int ison(struct player &p)
{
	for (int i = 0; i < 5; i++)
		if ((p.posY + 1 == squ[i].posY) && p.posX >= squ[i].posX && p.posX <= (squ[i].posX + 3))
		{

			p.on = 1;
			p.posY--;

			break;
		}
		else
		{
			p.on = 0;
		}

	return p.on;
}

int hurt()
{
	if (p.posY >= 1 + NY)
	{
		p.hp = 0;
		showhp();
	}

	if (p.posY < 1)
	{
		p.hp -= 2;
		Beep(100, 100);
		Sleep(100);
		showhp();
	}

	return p.hp;
}

void p_action(char &c)
{

	switch (c)
	{
	case 'A':
	case 'a':
		if (p.posX > 1 && judge() && judgeleft())
			p.posX--;
		break;
	case 'D':
	case 'd':
		if (p.posX < NX && judge() && judgeright())
			p.posX++;
		break;
	case 'P':
	case 'p':
		gotoxy(p.posX, p.posY);
		cout << "★";
		while (!_kbhit())
			;
		cout << "\b  ";
		break;
	case 'q':
	case 'Q':
		restart();
	}
}

//对square操作

void cs(square &squ) //擦除square
{

	int x, y, i;
	for (i = 0; i < 4; i++)
	{
		x = squ.posX + i;
		y = squ.posY;
		gotoxy(x, y);
		cout << " ";
	}
}

bool check(square squ) //检查是否到顶？
{
	if (squ.posY == 1)
	{
		cs(squ);
		return 1;
	}
	else
		return 0;
}

void pr(square squ) //打印图形
{

	color(squ.color);
	for (int i = 0; i < 4; i++)
	{

		gotoxy(squ.posX + i, squ.posY);
		cout << "■";
	}
}

int getScore()
{
	int j = 1;
	if ((p.posY + 1) == squ[j].posY && p.posX >= squ[j].posX && p.posX <= (squ[j].posX + 3))
	{
		p.score++;
		Beep(5000, 50);
		show_score();

		return 0;
	}
	else if (squ[j].posY != 2 && ((ai.posY + 1) != squ[j].posY || ai.posX < squ[j].posX || ai.posX > (squ[j].posX + 3)))
	{
		gotoxy(squ[j].posX + 1, squ[j].posY - 1);
		cout << "Power";
	}
	return 1;
}
void s_action()
{
	if (ison(p))
	{
		gotoxy(p.posX, p.posY);
		cout << "★";
	}
	if (ison(ai) && level >= ai_level)
	{
		gotoxy(ai.posX, ai.posY);
		cout << "⊙";
	}

	for (int i = 0; i < 5; i++)
	{
		cs(squ[i]);

		squ[i].posY--;
	}

	for (int i = 0; i < 5; i++)
	{
		if (check(squ[i]))
		{
			setsquare(squ[i], 26);
			p.score += 1;

			if ((!(p.score % 15)) && p.hp < 5)
			{
				p.hp++;
				showhp();
			}
			show_score();
		}
	}

	for (int i = 0; i < 5; i++)
	{
		pr(squ[i]);
	}
	if (getScore())
		Sleep(150 - level * 8);
	else
		Sleep(50);
}

//联合函数

int k = 1; //下落声音判定

void show(char &c)
{
	pcs(p);
	p_action(c);

	k--;
	if (!ison(p))
	{
		p.posY++;
		k = 1;
	}

	if (hurt() > 0)
	{
		gotoxy(p.posX, p.posY);
		cout << "★";
		if (!k)
			Beep(500, 10);
	}
	else
	{

		restart();
	}
}

void ai_action()
{

	if (ai.posX < p.posX && (ai.posX != t[0].X - 1) && (ai.posX != t[1].X - 1) && ai.posX < NX)
		ai.posX++;
	else if (ai.posX > 1)
	{
		ai.posX--;
	}
}

void ai_hurt()
{
	if (ai.posX == p.posX && p.posY == ai.posY)
	{
		p.hp -= 1;
		showhp();
		gotoxy(ai.posX, ai.posY - 1);
		cout << "Duang~";

		Beep(800, 300);

		gotoxy(ai.posX, ai.posY - 1);
		cout << "      ";

		ai_init();
	}
}

void ai_show()
{

	if (level >= ai_level)
	{

		ai_hurt();
		pcs(ai);
		for (int i = 0; i < 2; i++)
		{
			gotoxy(t[i].X, t[i].Y);
			cout << "  ";
		}

		if ((t[1].X != t[0].X || t[1].Y != t[0].Y) && (t[1].X != ai.posX || t[1].Y != ai.posY))
		{
			t[1].X = t[0].X;
			t[1].Y = t[0].Y;
		}
		if (t[0].X != ai.posX || t[0].Y != ai.posY)
		{
			t[0].X = ai.posX;
			t[0].Y = ai.posY;
		}

		if (ai.posX == p.posX && p.posY == ai.posY)
		{
			p.hp -= 1;
			showhp();
			gotoxy(ai.posX, ai.posY - 1);
			cout << "Duang~";
			Beep(100, 100);
			Sleep(100);

			gotoxy(ai.posX, ai.posY - 1);
			cout << "      ";

			ai_init();
		}

		if (!ison(ai))
		{
			ai.posY++;
			if (ai.posY >= 1 + NY)
			{
				ai_init();
				p.score++;
			}
		}
		else
		{
			ai_action();
			t[0].Y--;
			t[1].Y--;
		}
		gotoxy(ai.posX, ai.posY);
		cout << "⊙";
		for (int i = 0; i < 2; i++)
		{
			gotoxy(t[i].X, t[i].Y);
			cout << "〇";
		}

		ai_hurt();
	}
}

int showarrow()
{

	COORD arrow = {13, 20};

	char a = _getch();
	gotoxy(arrow.X, arrow.Y);
	cout << "  ";
	while (a != '\r')
	{

		gotoxy(arrow.X, arrow.Y);
		cout << "  ";
		switch (a)
		{
		case 's':
		case 'S':
			if (arrow.Y < 24)
			{
				arrow.Y += 2;
			}
			break;
		case 'W':
		case 'w':
			if (arrow.Y > 20)
			{
				arrow.Y -= 2;
			}
		}

		gotoxy(arrow.X, arrow.Y);
		cout << "-> ";
		a = _getch();
	}
	system("cls");
	return arrow.Y;
}

void logo()
{
	cout << "          〇                    〇                               〇    " << endl;
	cout << "       〇    〇              〇〇〇〇〇〇〇                     〇〇  " << endl;
	cout << "     〇    〇   〇          〇                            〇〇〇〇〇〇〇〇 " << endl;
	cout << "   〇    〇〇〇    〇                              〇     〇            〇" << endl;
	cout << "             〇        〇〇〇                      〇             " << endl;
	cout << "           〇          〇  〇 〇〇〇〇〇〇     〇〇〇〇〇      〇    〇  " << endl;
	cout << "      〇〇〇〇〇〇     〇  〇          〇      〇  〇  〇      〇    〇" << endl;
	cout << "      〇        〇     〇〇〇        〇        〇〇〇〇〇      〇    〇" << endl;
	cout << "      〇   〇   〇                〇               〇          〇〇〇〇" << endl;
	cout << "      〇   〇   〇            〇                   〇  〇      〇" << endl;
	cout << "        〇    〇             〇            〇  〇〇〇〇〇      〇      〇" << endl;
	cout << "      〇          〇           〇〇〇〇〇〇            〇      〇〇〇〇" << endl;

} //不好看

int menus()
{

	color(2);
	gotoxy(14, NY / 2 - 2);
	cout << "贪吃蛇【改】";
	color(9);

	gotoxy(NX / 3 + 2, NY / 2);
	cout << "操控说明：";
	gotoxy(NX / 3 + 2, NY / 2 + 1);
	cout << "WASD分别控制方向的上下左右";

	color(13);
	gotoxy(15, 20);
	cout << "Start";
	gotoxy(15, 22);
	cout << "Set";
	gotoxy(15, 24);
	cout << "Quit";
	gotoxy(13, 20);
	cout << "-> ";

	return showarrow();
}

void a_init()
{
	for (int i = 0; i < 3; i++)
	{
		a[i].X = 4 - i;
		a[i].Y = 2;
		gotoxy(a[i]);
		cout << "〇";
	}
	heading = Right;
}

void goheading()
{
	switch (heading)
	{
	case Up:
		a[0].Y--;
		break;
	case Down:
		a[0].Y++;
		break;
	case Left:
		a[0].X--;
		break;
	case Right:
		a[0].X++;
	}
}

void cgaction(int &n)
{

	char b;
	if (_kbhit())
		b = _getch();
	else
		b = ' ';

	gotoxy(a[2]);
	cout << " ";

	if (a[2] != a[1] && a[0] != a[1])
	{
		a[2] = a[1];
		a[1] = a[0];
	}

	switch (b)
	{
	case 'A':
	case 'a':
		if (a[0].X > 1 && heading != Right)
		{
			a[0].X--;
			heading = Left;
		}
		else
			goheading();

		break;
	case 'D':
	case 'd':
		if (a[0].X < NX && heading != Left)
		{
			a[0].X++;
			heading = Right;
		}
		else
			goheading();
		break;
	case 'w':
	case 'W':
		if (a[0].Y > 1 && heading != Down)
		{
			a[0].Y--;
			heading = Up;
		}
		else
			goheading();

		break;
	case 's':
	case 'S':
		if (a[0].Y < NY && heading != Up)
		{
			a[0].Y++;
			heading = Down;
		}
		else
			goheading();
		break;
	default:
		goheading();
	}

	if (a[0].Y > NY || a[0].Y < 1 || a[0].X > NX || a[0].X < 1)
	{
		gotoxy(5, 15);
		cout << "Lost.Try again";
		gotoxy(a[1]);
		cout << " ";
		gotoxy(a[2]);
		cout << " ";
		while (!_kbhit())
			;
		gotoxy(5, 15);
		cout << "              ";
		a_init();
		n = 15;
	}

	for (int i = 0; i < 3; i++)
	{
		gotoxy(a[i]);
		cout << "〇";
	}

	Sleep(200);
}

COORD star = {NX - 3, NY - 3};

void declare()
{
	system("cls");
	gotoxy(NX / 3, NY / 3);
	cout << "游戏说明：";
	gotoxy(NX / 3, NY / 3 + 1);
	cout << "你操控的星星必须获得足够的能量来逃离贪吃蛇的追击";
	gotoxy(NX / 3, NY / 3 + 2);
	cout << "若游戏结束时你有 " << win << "的power，则逃脱成功~  ↖(^ω^)↗加油~~";

	gotoxy(NX / 3, NY / 3 + 5);
	cout << "规则：";
	gotoxy(NX / 3, NY / 3 + 6);
	cout << "碰到顶部伤害：2";
	gotoxy(NX / 3, NY / 3 + 7);
	cout << "碰到蛇伤害： 1【仅蛇头具有伤害力】";
	gotoxy(NX / 3, NY / 3 + 8);
	cout << "到底部死亡";
	gotoxy(NX / 3, NY / 3 + 9);
	cout << "在特定的积木上会有特别的效果";
	gotoxy(NX / 3, NY / 3 + 10);
	cout << "速度会随着level增加而增加";
	gotoxy(NX / 3, NY / 3 + 11);
	cout << "p暂停，q重新开始";

	pause();
	system("cls");
}

void cg()
{

	wallinit();

	for (int i = 1; i <= NY + 1; i++)
	{
		gotoxy(0, i);
		cout << "□";
		gotoxy(NX + 1, i);
		cout << "□";
	}
	gotoxy(1, NY + 1);
	for (int i = 1; i <= NX; i++)
	{
		cout << "□";
	}

	int n = 15;
	a_init();
	gotoxy(star);
	cout << "★";
	while (n--)
	{
		cgaction(n);
	}
	gotoxy(star);
	cout << " ";
	gotoxy(NX - 3, NY - 4);
	cout << "★";
	Sleep(150);
	gotoxy(NX - 5, NY - 4);
	cout << "Σ(°o°；)";
	Beep(1000, 200);
	Sleep(200);
	gotoxy(NX - 3, NY - 3);
	cout << "★";
	pause();
	gotoxy(NX - 8, NY - 5);
	cout << "有蛇！！快逃！！";
	Sleep(200);
	while (1)
		if (_kbhit())
			if (_kbhit())
			{
				_getch();
				gotoxy(NX - 5, NY - 4);
				cout << "           ";
				gotoxy(NX - 8, NY - 5);
				cout << "                ";
				break;
			}
	int k = 3;
	while (k)
	{

		cgaction(n);

		gotoxy(star);
		cout << " ";
		star.Y++;
		if (star.Y == NY + 1)
		{
			star.Y = NY - 3;
			k--;
			Beep(1200, 100);
		}
		gotoxy(star);
		cout << "★";
	}
	gotoxy(star);
	cout << " ";
	gotoxy(star.X, NY + 1);
	cout << " ";
	Beep(150, 200);
	Beep(100, 80);
	gotoxy(a[0].X, a[0].Y + 1);
	cout << "!!!";
	pause();

	pause();
	gotoxy(a[0].X, a[0].Y + 1);
	cout << "   ";
	gotoxy(a[0].X, a[0].Y + 2);
	cout << "    ";
	while (a[0].Y != NY)
	{
		gotoxy(a[2]);
		cout << " ";
		a[2] = a[1];
		a[1] = a[0];
		if (a[0].X < star.X)
		{
			a[0].X++;
		}
		else if (a[0].X > star.X)
		{
			a[0].X--;
		}
		else
			a[0].Y++;
		for (int i = 0; i < 3; i++)
		{
			gotoxy(a[i]);
			cout << "〇";
		}
		Sleep(200);
	}
	gotoxy(a[2]);
	cout << " ";
	Sleep(200);
	gotoxy(a[1]);
	cout << " ";
	Sleep(200);
	gotoxy(a[0]);
	cout << " ";
	Sleep(200);

	pause();
	gotoxy(NX / 3, NY / 3);
	cout << "选择操作角色 ：";
	gotoxy(NX / 3, NY / 3 + 1);
	cout << "1.★";
	gotoxy(NX / 3, NY / 3 + 3);
	cout << "2.〇〇〇<待开发>";
	gotoxy(NX / 3 - 3, NY / 3 + 1);
	cout << "-> ";
	char q = ' ';
	while (q != '\r')
		q = _getch();
	declare();
	gotoxy(NX / 3 + 5, NY / 3 + 3);
	cout << "Ready ?";
	pause();
	gotoxy(NX / 3 + 5, NY / 3 + 4);
	cout << "GO!!!";
	Sleep(300);
	system("cls");
}

void before_begain()
{
	system("mode con cols=80 lines=35");
	//调整系统Console控制台显示的宽度和高度，高度为line，宽度为col
	srand(time(0));
	int i = menus();

	while (i != 20)
	{
		system("cls");
		if (i == 22) //set
		{
			gotoxy(15, 19);
			cout << "背景音乐音量：";
			gotoxy(15, 20);
			cout << "Off";
			gotoxy(15, 22);
			cout << "Low";
			gotoxy(15, 24);
			cout << "Loud";
			gotoxy(13, 20);
			cout << "-> ";
			switch (showarrow())
			{
			case 20:
				volume = 15000 * 0;
				break;
			case 22:
				volume = 15000;
				break;
			case 24:
				volume = 30000;
			}

			i = menus();
		}
		else if (i == 24)
		{

			exit(0);
		}
	}

	cg();

	init();
}

int main()
{
	before_begain();

	char c = ' ';

	while (1)
	{
		wallinit();
		if (_kbhit())
			c = _getch();
		else
			c = ' ';

		if (level >= ai_level)
			ai_show();
		show(c);
		s_action();
	}

	return 0;
}
