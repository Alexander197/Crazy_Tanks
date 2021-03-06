#include "stdafx.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;
HWND hwnd = GetConsoleWindow();
HDC hdc = GetWindowDC(hwnd);

const int RED RGB(255, 0, 0);
const int BLK RGB(0, 0, 0);
const int GRN RGB(0, 255, 0);
const int BLE RGB(0, 0, 255);
const int BLT RGB(0, 50, 0);
const int WHT RGB(255, 255, 255);

const int SHIFT_X = 300;
const int SHIFT_Y = 100;
const int AMOUNT_OF_WALLS = 20;
const int ENEMIES = 10;
const int HEALTH = 3;

const int TANK_SIZE = 10;
const int MIN_DIST = 150;

const HPEN REDPEN = CreatePen(PS_SOLID, 3, RED);
const HPEN BLKPEN = CreatePen(PS_SOLID, 3, BLK);
const HPEN GRNPEN = CreatePen(PS_SOLID, 3, GRN);
const HPEN BLEPEN = CreatePen(PS_SOLID, 3, BLE);
const HPEN BULLET = CreatePen(PS_SOLID, 4, BLT);

bool Up()
{
	SHORT state = GetAsyncKeyState(VK_UP);
	return (state) != 0;
}
bool Right()
{
	SHORT state = GetAsyncKeyState(VK_RIGHT);
	return (state) != 0;
}
bool Down()
{
	SHORT state = GetAsyncKeyState(VK_DOWN);
	return (state) != 0;
}
bool Left()
{
	SHORT state = GetAsyncKeyState(VK_LEFT);
	return (state) != 0;
}
bool Space()
{
	SHORT state = GetAsyncKeyState(VK_SPACE);
	return (state) != 0;
}
bool drawLine(HPEN pen, int bngX, int bngY, int endX, int endY)
{
	SelectObject(hdc, pen);
	MoveToEx(hdc, bngX, bngY, 0);
	return LineTo(hdc, endX, endY);
}
bool drawRect(HPEN pen, int bngX, int bngY, int endX, int endY)
{
	SelectObject(hdc, pen);
	return Rectangle(hdc, bngX, bngY, endX, endY);
}
void field(HPEN pen, int x, int y)
{
	RECT rect;
	GetWindowRect(hwnd, &rect);
	drawRect(pen, x, y, rect.right - x, rect.bottom - y);
}
void tank(HPEN pen, int x, int y, char anglPos)
{
	switch (anglPos)
	{
	case 1:
		drawRect(pen, x - TANK_SIZE, y - TANK_SIZE, x + TANK_SIZE, y + TANK_SIZE);
		drawLine(pen, x, y, x, y - (TANK_SIZE + TANK_SIZE*0.5));
		break;
	case 2:
		drawRect(pen, x - TANK_SIZE, y - TANK_SIZE, x + TANK_SIZE, y + TANK_SIZE);
		drawLine(pen, x, y, x + (TANK_SIZE + TANK_SIZE*0.5), y);
		break;
	case 3:
		drawRect(pen, x - TANK_SIZE, y - TANK_SIZE, x + TANK_SIZE, y + TANK_SIZE);
		drawLine(pen, x, y, x, y + (TANK_SIZE + TANK_SIZE*0.5));
		break;
	case 4:
		drawRect(pen, x - TANK_SIZE, y - TANK_SIZE, x + TANK_SIZE, y + TANK_SIZE);
		drawLine(pen, x, y, x - (TANK_SIZE + TANK_SIZE*0.5), y);
		break;
	default:
		break;
	}
}
void getWalls(bool **Map, int ***wallPos, int amountOfWalls, int fieldLength, int fieldWidth)
{
	int rndLength;
	int rndWidth;
	int rndPosX;
	int rndPosY;
	bool toggle;
	bool ver;
	for (int i = 0; i < amountOfWalls; i++)
	{
		rndLength = rand() % 50 + 50;
		rndWidth = rand() % 11 + 10;
		if (!i)
		{
			toggle = rand() % 2;
			if (!toggle)
			{

				rndPosX = SHIFT_X + rand() % (fieldLength - rndLength);
				rndPosY = SHIFT_Y + rand() % (fieldWidth - rndWidth);
				wallPos[i][0][0] = rndPosX;
				wallPos[i][0][1] = rndPosX + rndLength - 1;
				wallPos[i][1][0] = rndPosY;
				wallPos[i][1][1] = rndPosY + rndWidth - 1;
			}
			else if (toggle)
			{
				rndPosX = SHIFT_X + rand() % (fieldLength - rndWidth);
				rndPosY = SHIFT_Y + rand() % (fieldWidth - rndLength);
				wallPos[i][0][0] = rndPosX;
				wallPos[i][0][1] = rndPosX + rndWidth - 1;
				wallPos[i][1][0] = rndPosY;
				wallPos[i][1][1] = rndPosY + rndLength - 1;
			}
		}
		else if (i)
		{
			int a = 50000;
			while (a--)
			{
				ver = 0;
				toggle = rand() % 2;
				if (!toggle)
				{

					rndPosX = SHIFT_X + rand() % (fieldLength - rndLength);
					rndPosY = SHIFT_Y + rand() % (fieldWidth - rndWidth);
					wallPos[i][0][0] = rndPosX;
					wallPos[i][0][1] = rndPosX + rndLength - 1;
					wallPos[i][1][0] = rndPosY;
					wallPos[i][1][1] = rndPosY + rndWidth - 1;
				}
				else if (toggle)
				{
					rndPosX = SHIFT_X + rand() % (fieldLength - rndWidth);
					rndPosY = SHIFT_Y + rand() % (fieldWidth - rndLength);
					wallPos[i][0][0] = rndPosX;
					wallPos[i][0][1] = rndPosX + rndWidth - 1;
					wallPos[i][1][0] = rndPosY;
					wallPos[i][1][1] = rndPosY + rndLength - 1;
				}
				for (int j = 0; j < i; j++)
				{
					if (abs((wallPos[j][0][0] + wallPos[j][0][1]) / 2 - (wallPos[i][0][0] + wallPos[i][0][1]) / 2) < MIN_DIST && abs((wallPos[j][1][0] + wallPos[j][1][1]) / 2 - (wallPos[i][1][0] + wallPos[i][1][1]) / 2) < MIN_DIST)
					{
						ver = 1;
						break;
					}
				}
				if (!ver)
					break;
			}
		}
	}
	for (int i = 0; i < fieldLength; i++)
		for (int j = 0; j < fieldWidth; j++)
			Map[i][j] = 0;
	for (int i = 0; i < amountOfWalls; i++)
	{
		for (int j = 0; j < wallPos[i][0][1] - wallPos[i][0][0] + 1; j++)
			for (int k = 0; k < wallPos[i][1][1] - wallPos[i][1][0] + 1; k++)
				Map[wallPos[i][0][0] - SHIFT_X + j][wallPos[i][1][0] - SHIFT_Y + k] = 1;
	}
	for (int j = 0; j < fieldLength; j++)
		for (int k = 0; k < TANK_SIZE; k++)
		{
			Map[j][k] = 1;
			Map[j][fieldWidth - 1 - k] = 1;
		}
	for (int j = 0; j < fieldWidth; j++)
		for (int k = 0; k < TANK_SIZE; k++)
		{
			Map[k][j] = 1;
			Map[fieldLength - 1 - k][j] = 1;
		}


	/*
	for (int j=0; j < wallPos[i][0][1] - wallPos[i][0][0] + 1; j++)
	Map[wallPos[i][0][0] - SHIFT_X + j][wallPos[i][1][1] - SHIFT_Y] = 1;
	for (int j=0; j < wallPos[i][1][1] - wallPos[i][1][0] + 1; j++)
	Map[wallPos[i][0][0] - SHIFT_X][wallPos[i][1][0] - SHIFT_Y + j] = 1;
	for (int j=0; j < wallPos[i][1][1] - wallPos[i][1][0] + 1; j++)
	Map[wallPos[i][0][1] - SHIFT_X][wallPos[i][1][0] - SHIFT_Y + j] = 1;
	*/

}
void enemyPos(COORD *enemy, bool **Map, int fieldLength, int fieldWidth, int amountOfEnemies)
{
	bool ver;
	for (int i = 0; i < amountOfEnemies; i++)
	{
		if (!i)
		{
			while (1)
			{
				ver = 0;
				enemy[i].X = SHIFT_X + TANK_SIZE + rand() % (fieldLength - TANK_SIZE);
				enemy[i].Y = SHIFT_Y + TANK_SIZE + rand() % (fieldWidth / 2);
				for (int x = -TANK_SIZE; x < TANK_SIZE; x++)
					for (int y = -TANK_SIZE; y < TANK_SIZE; y++)
						if (Map[enemy[i].X + x - SHIFT_X][enemy[i].Y + y - SHIFT_Y])
						{
							ver = 1;
							goto a;
						}
			a:
				if (!ver)
					break;
			}
		}
		else if (i)
		{
			while (1)
			{
				ver = 0;
				enemy[i].X = SHIFT_X + TANK_SIZE + rand() % (fieldLength - TANK_SIZE);
				enemy[i].Y = SHIFT_Y + TANK_SIZE + rand() % (fieldWidth / 2);
				for (int x = -TANK_SIZE; x < TANK_SIZE; x++)
					for (int y = -TANK_SIZE; y < TANK_SIZE; y++)
						if (Map[enemy[i].X + x - SHIFT_X][enemy[i].Y + y - SHIFT_Y])
						{
							ver = 1;
							goto b;
						}
				for (int j = 0; j < i; j++)
					if (abs(enemy[j].X - enemy[i].X)<5 * TANK_SIZE && abs(enemy[j].Y - enemy[i].Y)<5 * TANK_SIZE)
					{
						ver = 1;
						goto b;
					}
			b:
				if (!ver)
					break;
			}
		}
	}
}
COORD playerPos(bool **Map, int fieldLength, int fieldWidth)
{
	bool ver;
	COORD player;
	while (1)
	{
		ver = 0;
		player.X = SHIFT_X + TANK_SIZE + rand() % (fieldLength - TANK_SIZE);
		player.Y = SHIFT_Y + 3 * fieldWidth / 4 - 1 + rand() % (fieldWidth / 4 - 1);
		for (int x = -TANK_SIZE; x < TANK_SIZE; x++)
			for (int y = -TANK_SIZE; y < TANK_SIZE; y++)
				if (Map[player.X + x - SHIFT_X][player.Y + y - SHIFT_Y])
				{
					ver = 1;
					goto c;
				}
	c:
		if (!ver)
			break;
	}
	return player;
}
int PewPew(bool **Map, int ***wallPos, COORD *enemy, COORD player, int x, int y, char angPosPl, char angPos, char *rndAngPos, bool owner, bool *enemyStatus, int Helth)
{
	COORD bulletB;
	COORD bulletE;
	int bulletLength = 5;
	for (int i = 0; i < 1000; i += 9)
	{
		switch (angPos)
		{
		case 1:
			bulletB.X = x;
			bulletB.Y = y - (TANK_SIZE + 1) - i;
			bulletE.X = x;
			bulletE.Y = y - (TANK_SIZE + 1) - bulletLength - i;
			break;
		case 2:
			bulletB.X = x + (TANK_SIZE + 1) + i;
			bulletB.Y = y;
			bulletE.X = x + (TANK_SIZE + 1) + bulletLength + i;
			bulletE.Y = y;
			break;
		case 3:
			bulletB.X = x;
			bulletB.Y = y + (TANK_SIZE + 1) + i;
			bulletE.X = x;
			bulletE.Y = y + (TANK_SIZE + 1) + bulletLength + i;
			break;
		case 4:
			bulletB.X = x - (TANK_SIZE + 1) - i;
			bulletB.Y = y;
			bulletE.X = x - (TANK_SIZE + 1) - bulletLength - i;
			bulletE.Y = y;
			break;
		default:
			break;
		}
		field(GRNPEN, SHIFT_X, SHIFT_Y);
		for (int i = 0; i < AMOUNT_OF_WALLS; i++)
			drawRect(BLKPEN, wallPos[i][0][0], wallPos[i][1][0], wallPos[i][0][1], wallPos[i][1][1]);

		for (int i = 0; i < ENEMIES; i++)
		{
			tank(BLEPEN, enemy[i].X, enemy[i].Y, rndAngPos[i]);
		}
		tank(REDPEN, player.X, player.Y, angPosPl);
		drawLine(BULLET, bulletB.X, bulletB.Y, bulletE.X, bulletE.Y);
		Sleep(1);
		if (Map[bulletE.X - SHIFT_X][bulletE.Y - SHIFT_Y])
			return Helth;
		if (owner)
		{
			for (int i = 0; i < ENEMIES; i++)
				for (int x = -TANK_SIZE; x < TANK_SIZE; x++)
					for (int y = -TANK_SIZE; y < TANK_SIZE; y++)
						if (enemy[i].X + x == bulletE.X && enemy[i].Y + y == bulletE.Y)
						{
							enemyStatus[i] = 1;
							enemy[i] = { 0,0 };
							return Helth;
						}
		}
		else if (!owner)
		{
			for (int i = 0; i < ENEMIES; i++)
				for (int x = -TANK_SIZE; x < TANK_SIZE; x++)
					for (int y = -TANK_SIZE; y < TANK_SIZE; y++)
						if (enemy[i].X + x == bulletE.X && enemy[i].Y + y == bulletE.Y)
						{
							return Helth;
						}

			for (int x = -TANK_SIZE; x < TANK_SIZE; x++)
				for (int y = -TANK_SIZE; y < TANK_SIZE; y++)
					if (player.X + x == bulletE.X && player.Y + y == bulletE.Y)
					{

						return Helth - 1;
					}
		}
	}
}
int main(void)
{
	srand(time(0));
	setlocale(LC_ALL, "rus");
	cout << ("Разверните окно на весь экран и нажмите любую клавишу");
	_getch();
	LOGFONT font;
	font.lfHeight = 46;
	font.lfWidth = 0;
	font.lfEscapement = 0;
	font.lfOrientation = 0;
	font.lfWeight = 100;
	font.lfItalic = 0;
	font.lfUnderline = 0;
	font.lfStrikeOut = 0;
	font.lfCharSet = RUSSIAN_CHARSET;
	font.lfOutPrecision = 0;
	font.lfClipPrecision = 0;
	font.lfQuality = 0;
	font.lfPitchAndFamily = 0;
	HFONT hfont = CreateFontIndirect(&font);

	WCHAR  hah[100];
	int strLength1 = 1;
	int strLength2 = 1;

	RECT rect;
	GetWindowRect(hwnd, &rect);
	int fieldLength = rect.right - 2 * SHIFT_X;
	int fieldWidth = rect.bottom - 2 * SHIFT_Y;

	int Score = 0;
	int Health = HEALTH;

	bool *enemyStatus = new bool[ENEMIES];
	for (int i = 0; i < ENEMIES; i++)
	{
		enemyStatus[i] = 0;
	}



	COORD player;
	COORD *enemy = new COORD[ENEMIES];
	int *rndMove = new int[ENEMIES];
	char *rndAngPos = new char[ENEMIES];
	char angPosPl = 1;

	bool **Map = new bool*[fieldLength];
	for (int i = 0; i < fieldLength; i++)
		Map[i] = new bool[fieldWidth];

	int ***wallPos = new int**[AMOUNT_OF_WALLS];
	for (int i = 0; i < AMOUNT_OF_WALLS; i++)
		wallPos[i] = new int*[2];
	for (int i = 0; i < AMOUNT_OF_WALLS; i++)
		for (int j = 0; j < 2; j++)
			wallPos[i][j] = new int[2];

	getWalls(Map, wallPos, AMOUNT_OF_WALLS, fieldLength, fieldWidth);
	enemyPos(enemy, Map, fieldLength, fieldWidth, ENEMIES);
	player = playerPos(Map, fieldLength, fieldWidth);

	for (int i = 0; i < ENEMIES; i++)
	{
		rndMove[i] = 0;
	}


	int plDel = 0;
	int start;
	system("cls");
	for (int c = 0;; c++)
	{
		start = clock();
		Score = 0;
		field(GRNPEN, SHIFT_X, SHIFT_Y);
		for (int i = 0; i < AMOUNT_OF_WALLS; i++)
			drawRect(BLKPEN, wallPos[i][0][0], wallPos[i][1][0], wallPos[i][0][1], wallPos[i][1][1]);

		for (int i = 0; i < ENEMIES; i++)
		{
			if (!enemyStatus[i])
			{
				if (!rndMove[i])
				{
					rndAngPos[i] = 1 + rand() % 4;
					rndMove[i] = rand() % 20 + 25;
				}
			}
		}
		for (int i = 0; i < ENEMIES; i++)
		{
			if (!enemyStatus[i])
			{
				if (rndAngPos[i] == 1)
				{
					enemy[i].Y -= 4;
					if (Map[enemy[i].X - SHIFT_X][enemy[i].Y - SHIFT_Y - (TANK_SIZE / 2 + 2)])
						enemy[i].Y += 4;
				}
				else if (rndAngPos[i] == 2)
				{
					enemy[i].X += 4;
					if (Map[enemy[i].X - SHIFT_X + (TANK_SIZE / 2 + 2)][enemy[i].Y - SHIFT_Y])
						enemy[i].X -= 4;
				}
				else if (rndAngPos[i] == 3)
				{
					enemy[i].Y += 4;
					if (Map[enemy[i].X - SHIFT_X][enemy[i].Y - SHIFT_Y + (TANK_SIZE / 2 + 2)])
						enemy[i].Y -= 4;
				}
				else if (rndAngPos[i] == 4)
				{
					enemy[i].X -= 4;
					if (Map[enemy[i].X - SHIFT_X - (TANK_SIZE / 2 + 2)][enemy[i].Y - SHIFT_Y])
						enemy[i].X += 4;
				}
				rndMove[i]--;
			}
		}
		if (Up())
		{
			player.Y -= TANK_SIZE / 2 + 1;
			if (Map[player.X - SHIFT_X][player.Y - SHIFT_Y - (TANK_SIZE / 2 + 2)])
				player.Y += TANK_SIZE / 2 + 1;
			angPosPl = 1;
		}
		else if (Right())
		{
			player.X += TANK_SIZE / 2 + 1;
			if (Map[player.X - SHIFT_X + (TANK_SIZE / 2 + 2)][player.Y - SHIFT_Y])
				player.X -= TANK_SIZE / 2 + 1;
			angPosPl = 2;
		}
		else if (Down())
		{
			player.Y += TANK_SIZE / 2 + 1;
			if (Map[player.X - SHIFT_X][player.Y - SHIFT_Y + (TANK_SIZE / 2 + 2)])
				player.Y -= TANK_SIZE / 2 + 1;
			angPosPl = 3;
		}
		else if (Left())
		{
			player.X -= TANK_SIZE / 2 + 1;
			if (Map[player.X - SHIFT_X - (TANK_SIZE / 2 + 2)][player.Y - SHIFT_Y])
				player.X += TANK_SIZE / 2 + 1;
			angPosPl = 4;
		}

		for (int i = 0; i < ENEMIES; i++)
		{
			if (abs(enemy[i].X - player.X) < TANK_SIZE / 2 && enemy[i].Y > player.Y &&rndAngPos[i] == 1)
				Health = PewPew(Map, wallPos, enemy, player, enemy[i].X, enemy[i].Y, angPosPl, rndAngPos[i], rndAngPos, 0, enemyStatus, Health);
			else if (abs(enemy[i].X - player.X) < TANK_SIZE / 2 && enemy[i].Y < player.Y &&rndAngPos[i] == 3)
				Health = PewPew(Map, wallPos, enemy, player, enemy[i].X, enemy[i].Y, angPosPl, rndAngPos[i], rndAngPos, 0, enemyStatus, Health);
			else if (abs(enemy[i].Y - player.Y) < TANK_SIZE / 2 && enemy[i].X > player.X &&rndAngPos[i] == 4)
				Health = PewPew(Map, wallPos, enemy, player, enemy[i].X, enemy[i].Y, angPosPl, rndAngPos[i], rndAngPos, 0, enemyStatus, Health);
			else if (abs(enemy[i].Y - player.Y) < TANK_SIZE / 2 && enemy[i].X < player.X &&rndAngPos[i] == 2)
				Health = PewPew(Map, wallPos, enemy, player, enemy[i].X, enemy[i].Y, angPosPl, rndAngPos[i], rndAngPos, 0, enemyStatus, Health);
		}

		if (Space() && !plDel)
		{
			PewPew(Map, wallPos, enemy, player, player.X, player.Y, angPosPl, angPosPl, rndAngPos, 1, enemyStatus, Health);
			plDel = 11;
		}
		for (int i = 0; i < ENEMIES; i++)
		{
			Score += enemyStatus[i];
		}

		for (int i = 0; i < ENEMIES; i++)
		{
			{
				tank(BLEPEN, enemy[i].X, enemy[i].Y, rndAngPos[i]);
			}
		}
		tank(REDPEN, player.X, player.Y, angPosPl);

		if (start / CLK_TCK > 9)
			strLength1 = 2;
		else if (start / CLK_TCK > 99)
			strLength1 = 3;
		else if (start / CLK_TCK > 999)
			strLength1 = 4;

		if (Score > 9)
			strLength2 = 2;
		else if (Score > 99)
			strLength2 = 3;

		SelectObject(hdc, hfont);
		SetTextColor(hdc, WHT);
		SetBkColor(hdc, RGB(0, 0, 0));
		wsprintf(hah, TEXT("Time: %d"), start / CLK_TCK);
		TextOut(hdc, 25, 70, hah, 7 + strLength1);
		wsprintf(hah, TEXT("Your Health: %d"), Health);
		TextOut(hdc, 25, 210, hah, 15);
		wsprintf(hah, TEXT("Your Score: %d"), Score);
		TextOut(hdc, 25, 350, hah, 13 + strLength2);

		Sleep(30);

		if (plDel)
			plDel--;
		if (!c % 33)
		{
			c = 0;
			InvalidateRect(hwnd, 0, TRUE);
			UpdateWindow(hwnd);
		}
		if (!Health || Score == ENEMIES)break;
	}
	InvalidateRect(hwnd, 0, TRUE);
	UpdateWindow(hwnd);
	Sleep(100);
	font.lfHeight = 144;
	hfont = CreateFontIndirect(&font);
	SelectObject(hdc, hfont);
	SetTextColor(hdc, WHT);
	SetBkColor(hdc, RGB(0, 0, 0));
	if (!Health)
	{
		TextOut(hdc, 350, 300, TEXT("Game Over"), 10);
	}
	else if (Score == ENEMIES)
	{
		TextOut(hdc, 470, 300, TEXT("Victory"), 8);
	}
	return 0;
}
