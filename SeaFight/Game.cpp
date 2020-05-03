#include <iostream>
#include <ctime>
#include <vector>
#include <iomanip>
#include <Windows.h>

enum FieldLabel
{

	LABEL_EMPTY,
	LABEL_SHIP,
	LABEL_HIT,
	LABEL_MISS,
	LABEL_BUFFER

};

struct Ship
{

	Ship()
	{

		Length = 0;
		X = 0;
		Y = 0;
		Direction = 0;		

	}

	//Ship(int length, int x, int y, int direction, std::vector <std::vector <int> > shipCoordinates)
	//{

	//	ShipCoordinates = shipCoordinates;
	//	Length = length;
	//	X = x;
	//	Y = y;
	//	Direction = direction;

	//}

	
	bool IsLive = true;	
	int Length;
	int X;
	int Y;
	int Direction;
	std::vector <std::vector <int> > ShipCoordinates;	
	
	enum ShipDirection
	{

		DIRECTION_TOP = 1,
		DIRECTION_RIGHT,
		DIRECTION_BOTTOM,
		DIRECTION_LEFT

	};

	// Проверка жив ли корабль
	void CheckIsLive(int playingField[][10]) 
	{

		for (int i = 0; i < Length; ++i)
		{

			if (playingField[ShipCoordinates[0][i]][ShipCoordinates[1][i]] == 0)
			{
				return;
			}
				
		}

		IsLive = false;
		Buffer(playingField);

		std::cout << "Ship destroyed!\n\n";		

	}

	// Создание буфферной зоны вокруг потопленного коробля
	void Buffer(int playingField[][10])  
	{

		int checkArray[8][2] =
		{
			{0, 1},  {1, 1},   {1, 0},  {1, -1},
			{0, -1}, {-1, -1}, {-1, 0}, {-1, 1}
		};
		
		for (int i = 0; i < Length; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				if ((ShipCoordinates[0][i] + checkArray[j][0]) >= 0 && (ShipCoordinates[1][i] + checkArray[j][1]) >= 0 && (ShipCoordinates[0][i] + checkArray[j][0]) < 10 && (ShipCoordinates[1][i] + checkArray[j][1]) < 10)
				{
					if (playingField[ShipCoordinates[0][i] + checkArray[j][0]][ShipCoordinates[1][i] + checkArray[j][1]] == FieldLabel::LABEL_EMPTY)
					{
						playingField[ShipCoordinates[0][i] + checkArray[j][0]][ShipCoordinates[1][i] + checkArray[j][1]] = FieldLabel::LABEL_BUFFER;
					}
				}
			}
		}

	}

};

// Вывод поля на экран
void printField(int playingField[][10])
{
	std::cout << "\n      0     1     2     3     4     5     6     7     8     9";
	std::cout << "\n   -------------------------------------------------------------\n";
	for (int i = 0; i < 10; ++i)
	{
		std::cout << " " << i;
		for (int j = 0; j < 10; ++j)
		{
			std::cout <<" | ";
			if (playingField[j][i] == FieldLabel::LABEL_EMPTY)
			{
				std::cout << "   ";
			}
			else if (playingField[j][i] == FieldLabel::LABEL_SHIP)
			{
				std::cout << "[ ]";
			}
			else if (playingField[j][i] == FieldLabel::LABEL_HIT)
			{
				std::cout << " X ";
			}
			else if (playingField[j][i] == FieldLabel::LABEL_MISS)
			{
				std::cout << " o ";
			}
			else if (playingField[j][i] == FieldLabel::LABEL_BUFFER)
			{
				std::cout << " . ";
			}
		}
		std::cout << " |\n   -------------------------------------------------------------\n";
	}

}

// Проверка на возможность размещения корабля
bool placementCheck(int playingField[][10], Ship &ship) 
{	

	std::vector<std::vector<int> > shipCoordinates(2, (std::vector<int>(ship.Length, 0))); // Координаты текущего корабля
	int count = 0;

	int checkArray[8][2] =
	{
		{0, 1},  {1, 1},   {1, 0},  {1, -1},
		{0, -1}, {-1, -1}, {-1, 0}, {-1, 1}
	};

	switch (ship.Direction)
	{


	case ship.ShipDirection::DIRECTION_TOP:

		for (int i = ship.Y; i > (ship.Y - ship.Length); --i)
		{
			if (i < 0 || i > 9)
			{
				return false;
			}
			else if (playingField[ship.X][i] == 1)
			{
				return false;
			}

			for (int j = 0; j < 8; ++j)
			{
				if (playingField[ship.X + checkArray[j][0]][i + checkArray[j][1]] >= 0 && playingField[ship.X + checkArray[j][0]][i + checkArray[j][1]] < 10)
				{
					if (playingField[ship.X + checkArray[j][0]][i + checkArray[j][1]] == 1)
					{
						return false;
					}
				}
			}

			shipCoordinates[0][count] = ship.X;
			shipCoordinates[1][count] = i;
			++count;

		}

		break;

	case ship.ShipDirection::DIRECTION_RIGHT:

		for (int i = ship.X; i < (ship.X + ship.Length); ++i)
		{
			if (i < 0 || i > 9)
			{
				return false;
			}
			else if (playingField[i][ship.Y] == 1)
			{
				return false;
			}	

			for (int j = 0; j < 8; ++j)
			{
				if (playingField[i + checkArray[j][0]][ship.Y + checkArray[j][1]] >= 0 && playingField[ship.X + checkArray[j][0]][i + checkArray[j][1]] < 10)
				{
					if (playingField[i + checkArray[j][0]][ship.Y + checkArray[j][1]] == 1)
					{
						return false;
					}
				}
			}

			shipCoordinates[0][count] = i;
			shipCoordinates[1][count] = ship.Y;
			++count;

		}		

		break;

	case ship.ShipDirection::DIRECTION_BOTTOM:

		for (int i = ship.Y; i < (ship.Y + ship.Length); ++i)
		{
			if (i < 0 || i > 9) // Проверка на выход за поле
			{
				return false;
			}
			else if (playingField[ship.X][i] == 1) // Проверка на пересечение с уже установленными кораблями
			{
				return false;
			}

			for (int j = 0; j < 8; ++j) // Проверка на соприкосновение с другими короблями
			{
				if (playingField[ship.X + checkArray[j][0]][i + checkArray[j][1]] >= 0 && playingField[ship.X + checkArray[j][0]][i + checkArray[j][1]] < 10)
				{
					if (playingField[ship.X + checkArray[j][0]][i + checkArray[j][1]] == 1)
					{
						return false;
					}
				}
			}

			shipCoordinates[0][count] = ship.X; // Запись координат в массив
			shipCoordinates[1][count] = i;
			++count;

		}

		break;

	case ship.ShipDirection::DIRECTION_LEFT:

		for (int i = ship.X; i > (ship.X - ship.Length); --i)
		{
			if (i < 0 || i > 9)
			{
				return false;
			}
			else if (playingField[i][ship.Y] == 1)
			{
				return false;
			}

			for (int j = 0; j < 8; ++j)
			{
				if (playingField[i + checkArray[j][0]][ship.Y + checkArray[j][1]] >= 0 && playingField[ship.X + checkArray[j][0]][i + checkArray[j][1]] < 10)
				{
					if (playingField[i + checkArray[j][0]][ship.Y + checkArray[j][1]] == 1)
					{
						return false;
					}
				}
			}

			shipCoordinates[0][count] = i;
			shipCoordinates[1][count] = ship.Y;
			++count;

		}

		

		break;
	}

	ship.ShipCoordinates = shipCoordinates;

	return true;

}

//Рандомная генерация кораблей
void shipGeneration(int playingField[][10], Ship &ship, int length)
{

	while (true)
	{
		ship.X = rand() % 10;
		ship.Y = rand() % 10;
		ship.Direction = (rand() % 4) + 1;
		ship.Length = length;

		if (placementCheck(playingField, ship))
		{
			break;
		}

	}

}

// Запись коробля в массив 
void placingShipOnField(int playingField[][10], Ship &ship)
{

	for (int i = 0; i < ship.Length; ++i)
	{
		playingField[ship.ShipCoordinates[0][i]][ship.ShipCoordinates[1][i]] = 1;
	}	

}

// Ручное размещение кораблей
void manualShipPlacement(int playingField[][10], Ship shipsArray[])
{

	for (int i = 0; i < 4; ++i)
	{

		while (true)
		{
			system("cls");
			printField(playingField);
			std::cout << "\n\n\nEnter the coordinates of the " << i + 1 << " single-deck ship (X, Y): ";
			std::cin >> shipsArray[i].X >> shipsArray[i].Y;
			shipsArray[i].Direction = 1;
			shipsArray[i].Length = 1;

			if (placementCheck(playingField, shipsArray[i]))
			{
				break;
			}

			std::cout << "\nIt is impossible to put the ship in such coordinates. Re-enter.\n";

			system("pause");

		}

		placingShipOnField(playingField, shipsArray[i]);

	}

	for (int i = 4; i < 7; ++i)
	{

		while (true)
		{
			system("cls");
			printField(playingField);
			std::cout << "\n\n\nEnter the initial coordinates of the " << i - 3 << " double-decker ship (X, Y): ";
			std::cin >> shipsArray[i].X >> shipsArray[i].Y;
			std::cout << "\nChoose ship direction: \n1.Top\n2.Right\n3.Bottom\n4.Left\n";
			std::cin >> shipsArray[i].Direction;
			shipsArray[i].Length = 2;

			if (placementCheck(playingField, shipsArray[i]))
			{
				break;
			}

			std::cout << "\nIt is impossible to put the ship in such coordinates. Re-enter.\n";

			system("pause");

		}

		placingShipOnField(playingField, shipsArray[i]);

	}

	for (int i = 7; i < 9; ++i)
	{

		while (true)
		{
			system("cls");
			printField(playingField);
			std::cout << "\n\n\nEnter the initial coordinates of the " << i - 6 << " three-deck ship (X, Y): ";
			std::cin >> shipsArray[i].X >> shipsArray[i].Y;
			std::cout << "\nChoose ship direction: \n1.Top\n2.Right\n3.Bottom\n4.Left\n";
			std::cin >> shipsArray[i].Direction;
			shipsArray[i].Length = 3;

			if (placementCheck(playingField, shipsArray[i]))
			{
				break;
			}

			std::cout << "\nIt is impossible to put the ship in such coordinates. Re-enter.\n";
			
			system("pause");

		}

		placingShipOnField(playingField, shipsArray[i]);

	}

	while (true)
	{
		system("cls");
		printField(playingField);
		std::cout << "\n\n\nEnter the initial coordinates of the four-deck ship (X, Y): ";
		std::cin >> shipsArray[9].X >> shipsArray[9].Y;
		std::cout << "\nChoose ship direction: \n1.Top\n2.Right\n3.Bottom\n4.Left\n";
		std::cin >> shipsArray[9].Direction;
		shipsArray[9].Length = 4;

		if (placementCheck(playingField, shipsArray[9]))
		{
			break;
		}

		std::cout << "\nIt is impossible to put the ship in such coordinates. Re-enter.\n";

		system("pause");

	}

	placingShipOnField(playingField, shipsArray[9]);

}

// Автоматическое размещение кораблей
void automaticShipPlacement(int playingField[][10], Ship shipsArray[])
{

	for (int i = 0; i < 4; ++i)
	{
		
		shipGeneration(playingField, shipsArray[i], 1);
		placingShipOnField(playingField, shipsArray[i]);

	}

	for (int i = 4; i < 7; ++i)
	{

		shipGeneration(playingField, shipsArray[i], 2);
		placingShipOnField(playingField, shipsArray[i]);

	}

	for (int i = 7; i < 9; ++i)
	{

		shipGeneration(playingField, shipsArray[i], 3);
		placingShipOnField(playingField, shipsArray[i]);

	}
	
	shipGeneration(playingField, shipsArray[9], 4);
	placingShipOnField(playingField, shipsArray[9]);

}

// Проверка массива кораблей на жизнь 
bool checkShipsIsLive(Ship ships[], int battleField[][10])
{

	bool gameOver = true;

	for (int i = 0; i < 10; ++i)
	{
		if (ships[i].IsLive == true)
		{			
			ships[i].CheckIsLive(battleField);
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		if (ships[i].IsLive == true)
		{
			gameOver = false;
		}
	}


	return gameOver;

}

// Рандомный выстрел
bool randomFight(int field[][10], int battleField[][10], Ship ships[], bool &gameOver)
{

	bool moveTransition = true;

	while (true)
	{		

		system("cls");
		printField(battleField);
		std::cout << "Computer turn";
		//Sleep(1000);
		int x = rand() % 10;
		int y = rand() % 10;

		if (battleField[x][y] == FieldLabel::LABEL_EMPTY || field[x][y] == FieldLabel::LABEL_BUFFER)
		{

			if (field[x][y] == FieldLabel::LABEL_EMPTY)
			{
				system("cls");
				battleField[x][y] = FieldLabel::LABEL_MISS;
				printField(battleField);
				std::cout << "\n\nComputer miss\n\n";
				//system("pause"); 
				break;
			}
			else if (field[x][y] == FieldLabel::LABEL_SHIP)
			{
				system("cls");
				battleField[x][y] = FieldLabel::LABEL_HIT;
				printField(battleField);
				std::cout << "\n\nYour ship is damaged!\n\n";				

				if (checkShipsIsLive(ships, battleField))
				{
					gameOver = true;
				}

				//system("pause");				
				moveTransition = false;
				break;
			}

		}

	}

	return moveTransition;

}

// Ручной выстрел
bool manualFight(int field[][10], int battleField[][10], Ship ships[], bool &gameOver)
{

	bool moveTransition = true;

	int x = 0;
	int y = 0;

	while (true)
	{
		system("cls");
		printField(battleField);
		std::cout << "\n\n\nEnter the coordinates of the shoot (X, Y): ";
		std::cin >> x >> y;			

		if (battleField[x][y] == FieldLabel::LABEL_EMPTY || field[x][y] == FieldLabel::LABEL_BUFFER)
		{
			system("cls");

			if (field[x][y] == FieldLabel::LABEL_EMPTY)
			{
				battleField[x][y] = FieldLabel::LABEL_MISS;
				printField(battleField);
				std::cout << "\nYou miss!\n\n";
				system("pause");
				break;
			}
			else if (field[x][y] == FieldLabel::LABEL_SHIP)
			{
				battleField[x][y] = FieldLabel::LABEL_HIT;
				printField(battleField);				
				std::cout << "\nYou hit an enemy ship!\n\n";

				if (checkShipsIsLive(ships, battleField))
				{
					gameOver = true;
				}

				system("pause");
				moveTransition = false;
				break;
			}

		}

		std::cout << "\nYou have already shot at the given coordinates. Enter others coordinates.\n";

		system("pause");		

	}

	return moveTransition;

}




int  main()
{	
	bool gameOver = false;
	srand(time(NULL));		
	
	Ship playerShips[10];
	int playerField[10][10] = { 0 };
	int playerBattleField[10][10] = { 0 };

	Ship computerShips[10];
	int computerField[10][10] = { 0 };
	int computerBattleField[10][10] = { 0 };
	
	automaticShipPlacement(computerField, computerShips);	

	std::cout << "*********************************************************************************\n";
	std::cout << "\t\t\tWELCOME TO Sea FIGHT v0.03\n";
	std::cout << "*********************************************************************************\n\n";

	int menu = 0;

	while (true)
	{		
		std::cout << "\nMENU:\n\n";
		std::cout << "1. Automatic Ship Placement\n";
		std::cout << "2. Manual Ship Placement\n";
		std::cout << "3. Exit\n";
		std::cin >> menu;

		if (menu > 0 && menu < 4)
		{
			break;
		}
		system("cls");
	}

	switch (menu)
	{
	case 1:
		automaticShipPlacement(playerField, playerShips);
		system("cls");
		printField(playerField);
		system("pause");
		break;
	case 2:
		manualShipPlacement(playerField, playerShips);
		break;
	case 3:
		std::cout << "\nGoodbye!\n\n";
		system("pause");
		return 0;
	}

	while (true)
	{					

		while (true)
		{
			if (manualFight(computerField, computerBattleField, computerShips, gameOver))
			{
				break;
			}

			if (gameOver)
			{
				std::cout << "\n\n****************************************************\n";
				std::cout << "\t\tYou wins!\n\t\tGame over!\n";
				std::cout << "****************************************************\n\n";
				system("pause");
				return 0;
			}

		}

		//while (true)
		//{	
		//	if (randomFight(computerField, computerBattleField, computerShips, gameOver))
		//	{
		//		break;
		//	}

		//	if (gameOver)
		//	{
		//		std::cout << "\n\n****************************************************\n";
		//		std::cout << "\t\tYou wins!\n\t\tGame over!\n";
		//		std::cout << "****************************************************\n\n";
		//		system("pause");
		//		return 0;
		//	}

		//}

		while (true)
		{
			if (randomFight(playerField, playerBattleField, playerShips, gameOver))
			{
				break;
			}

			if (gameOver)
			{
				std::cout << "\n\n****************************************************\n";
				std::cout << "\t\tComputer wins!\n\t\tGame over!\n";
				std::cout << "****************************************************\n\n";
				system("pause");
				return 0;
			}

		}
		
	}

}