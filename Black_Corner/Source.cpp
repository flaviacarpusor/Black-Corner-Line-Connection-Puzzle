#include <iostream>
#include <ctime>
#include <cstdlib>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define BOX_WIDTH 50
#define BOX_HEIGHT 50
#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 50

struct Element
{
	int line;
	int column;
	int piece;
}elementPrecedent, elementCurent, playerMove, firstElement, blackCorner;

bool dimensionIsSelected = false;
bool endGame = false;
bool firstMove = true;
int randomNumber;
int matrixSize;
int nrClicks;
bool currentFirstPlayer;
int beforeLineIndex = -1, beforeColumnIndex = -1;
int lineIndex, columnIndex;
string titleText;
vector<vector<int>> matrix;
vector<Element> nextPossibleElements;

int obtineXButonStop() {
	return WINDOW_WIDTH / 2 - BUTTON_WIDTH / 2;
}

int obtineYButonStop() {
	return 75;
}

int obtineLatimeButonStop() {
	return 100;
}

int obtineInaltimeButonStop() {
	return 50;
}

int getBeginMatrixX()
{
	return (WINDOW_WIDTH / 2) - ((BOX_WIDTH * matrixSize) / 2);
}

int getEndMatrixX()
{
	return (WINDOW_WIDTH / 2) + ((BOX_WIDTH * matrixSize) / 2) - BOX_WIDTH;
}

int getBeginMatrixY()
{
	return (WINDOW_HEIGHT / 2) - ((BOX_HEIGHT * matrixSize) / 2) + BOX_HEIGHT;
}

int getEndMatrixY()
{
	return (WINDOW_HEIGHT / 2) + ((BOX_HEIGHT * matrixSize) / 2);
}

int getLowerMultiple(int number)
{
	// x-ul mouse-ului, sa zicem 170 si trebuie sa returneze 150, pentru ca 150 este cel mai apropiat multiplu de 50 (box width) de 170 mai mic.
	int count = number;
	while (true)
	{
		if (count % BOX_WIDTH == 0)
			break;
		count--;
	}
	return count;
}

int getRandomNumber(int min, int max)
{
	srand(time(NULL));
	return rand() % max + min;
}

vector<vector<int>> initializareMatriceInt() {
	vector<vector<int>> matrice;
	for (int i = 0; i < matrixSize; i++)
	{
		vector<int> line;
		for (int j = 0; j < matrixSize; j++)
			line.push_back(0);
		matrice.push_back(line);
	}
	return matrice;
}

void displayElement(Element element)
{
	cout << "Element: (" << element.line << ", " << element.column << ", " << element.piece << ")" << endl;
}

void displayNextPossibleElements()
{
	cout << "\nElemente urmatoare posibile: " << endl;
	for (int i = 0; i < nextPossibleElements.size(); i++)
	{
		displayElement(nextPossibleElements[i]);
	}
	cout << "Sfarsit elemente urmatoare posibile\n";
}

void changeTitleText()
{
	if (currentFirstPlayer)
	{
		titleText = "Este randul jucatorului 1!";
	}
	else
	{
		titleText = "Este randul jucatorului 2!";
	}
}

vector<Vector2i> getButtonsPositions() {
	return {
		Vector2i(175, 225),
		Vector2i(325, 225),
		Vector2i(250, 300),
		Vector2i(175, 375),
		Vector2i(325, 375)
	};
}

void copyElement(Element& element1, Element& element2)
{
	element1.line = element2.line;
	element1.column = element2.column;
	element1.piece = element2.piece;
}

void addNextPossibleElement(int elementLine, int elementColumn)
{
	nextPossibleElements.push_back({ elementLine, elementColumn, 1 });
	nextPossibleElements.push_back({ elementLine, elementColumn, 2 });
	nextPossibleElements.push_back({ elementLine, elementColumn, 3 });
}

bool verifyMoveExitMatrix(int line, int column)
{
	if ((line >= 0 && line <= matrixSize - 1) && (column >= 0 && column <= matrixSize - 1))
		return true;
	return false;
}

bool verifyIndexesMove(int lineIndex, int columnIndex) {
	for (int i = 0; i < nextPossibleElements.size(); i++)
		if (nextPossibleElements[i].line == lineIndex && nextPossibleElements[i].column == columnIndex)
			return true;
	return false;
}



void generateNextPossibleElements(Element ep, Element ec)
{
	nextPossibleElements.clear();

	// Daca element precedent are valorile initiale atunci suntem la inceputul jocului
	if (ep.line == -1 && ep.column == -1 && ep.piece == -1)
	{
		// Verificare NORD
		if (verifyMoveExitMatrix(ec.line - 1, ec.column))
			addNextPossibleElement(ec.line - 1, ec.column);

		// Verificare EST
		if (verifyMoveExitMatrix(ec.line, ec.column + 1))
			addNextPossibleElement(ec.line, ec.column + 1);

		// Verificare SUD
		if (verifyMoveExitMatrix(ec.line + 1, ec.column))
			addNextPossibleElement(ec.line + 1, ec.column);

		// Verificare VEST
		if (verifyMoveExitMatrix(ec.line, ec.column - 1))
			addNextPossibleElement(ec.line, ec.column - 1);
	}
	else
	{
		if (ec.piece == 1)
		{
			// Elementul precedent a fost in NORD
			// Elementul urmator va fi in SUD
			if (ec.line == ep.line + 1)
				addNextPossibleElement(ec.line + 1, ec.column);

			// Elementul precedent a fost in EST
			// Elementul urmator va fi in VEST
			if (ec.column == ep.column - 1)
				addNextPossibleElement(ec.line, ec.column - 1);

			// Elementul precedent a fost in SUD
			// Elementul urmator va fi in NORD
			if (ec.line == ep.line - 1)
				addNextPossibleElement(ec.line - 1, ec.column);

			// Elementul precedent a fost in VEST
			// Elementul urmator va fi in EST
			if (ec.column == ep.column + 1)
				addNextPossibleElement(ec.line, ec.column + 1);
		}
		if (ec.piece == 2)
		{
			// Elementul precedent a fost in NORD
			// Elementul urmator va fi in EST
			if (ec.line == ep.line + 1)
				addNextPossibleElement(ec.line, ec.column + 1);

			// Elementul precedent a fost in EST
			// Elementul urmator va fi in NORD
			if (ec.column == ep.column - 1)
				addNextPossibleElement(ec.line - 1, ec.column);

			// Elementul precedent a fost in SUD
			// Elementul urmator va fi in VEST
			if (ec.line == ep.line - 1)
				addNextPossibleElement(ec.line, ec.column - 1);

			// Elementul precedent a fost in VEST
			// Elementul urmator va fi in SUD
			if (ec.column == ep.column + 1)
				addNextPossibleElement(ec.line + 1, ec.column);
		}
		if (ec.piece == 3)
		{
			// Elementul precedent a fost in NORD
			// Elementul urmator va fi in VEST
			if (ec.line == ep.line + 1)
				addNextPossibleElement(ec.line, ec.column - 1);

			// Elementul precedent a fost in EST
			// Elementul urmator va fi in SUD
			if (ec.column == ep.column - 1)
				addNextPossibleElement(ec.line + 1, ec.column);

			// Elementul precedent a fost in SUD
			// Elementul urmator va fi in EST
			if (ec.line == ep.line - 1)
				addNextPossibleElement(ec.line, ec.column + 1);

			// Elementul precedent a fost in VEST
			// Elementul urmator va fi in NORD
			if (ec.column == ep.column + 1)
				addNextPossibleElement(ec.line - 1, ec.column);
		}
	}
}

Font loadFontFromFile(string fileName)
{
	Font font;
	if (!font.loadFromFile(fileName))
	{
		cout << "Probleme la incarcarea fontului arial.ttf";
	}
	return font;
}

void drawText(RenderWindow& window, Font& font, string str, Color textColor, int x, int y, int size)
{
	Text text;
	text.setFont(font);
	text.setString(str);
	text.setCharacterSize(size);
	text.setFillColor(textColor);
	text.setPosition(x, y);
	window.draw(text);
}

void drawButton(RenderWindow& window, string str, int x, int y, int width, int height, Font& font, int size, Color textColor, Color bgColor)
{
	RectangleShape button(Vector2f(width, height));
	button.setPosition(x, y);
	button.setFillColor(bgColor);

	Text text;
	text.setFont(font);
	text.setString(str);
	text.setCharacterSize(size);
	text.setFillColor(textColor);

	FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(x + width / 2.0f, y + height / 2.0f);

	Vector2i mousePosition = Mouse::getPosition(window);

	if (mousePosition.x >= x && mousePosition.x <= x + width && mousePosition.y >= y && mousePosition.y <= y + height)
	{
		button.setFillColor(Color(200, 200, 200));
	}

	window.draw(button);
	window.draw(text);
}

void drawRectangle(RenderWindow& window, int width, int height, int x, int y, Color bgColor, Color borderColor, int thickness)
{
	RectangleShape rect(Vector2f(width, height));
	rect.setPosition(x, y);
	rect.setFillColor(bgColor);
	rect.setOutlineColor(borderColor);
	rect.setOutlineThickness(thickness);
	window.draw(rect);
}

void drawImage(RenderWindow& window, string imagePath, int x, int y, int width, int height)
{
	Texture texture;

	if (!texture.loadFromFile(imagePath))
	{
		cout << "Citirea imaginii nu a reusit" << endl;
		return;
	}

	Sprite sprite;
	sprite.setTexture(texture);
	sprite.setPosition(x, y);
	sprite.setScale(width / texture.getSize().x, height / texture.getSize().y);
	window.draw(sprite);
}

void drawMatrix(RenderWindow& window)
{
	int beginMatrixX = getBeginMatrixX();
	int endMatrixX = getEndMatrixX();
	int beginMatrixY = getBeginMatrixY();
	int endMatrixY = getEndMatrixY();

	for (int i = 0, y = beginMatrixY; i < matrixSize && y <= endMatrixY; i++, y += 50)
		for (int j = 0, x = beginMatrixX; j < matrixSize && x <= endMatrixX; j++, x += 50)
		{
			drawRectangle(window, BOX_WIDTH, BOX_HEIGHT, x, y, Color::Black, Color::White, 1);

			switch (matrix[i][j])
			{
			case 1:
				drawImage(window, "patratel1.png", x, y, 50, 50);
				break;
			case 2:
				drawImage(window, "patratel2.png", x, y, 50, 50);
				break;
			case 3:
				drawImage(window, "patratel3.png", x, y, 50, 50);
				break;
			case 4:
				drawImage(window, "black_corner.png", x, y, 50, 50);
				break;
			default:
				break;
			}
		}
}

void drawTitle(RenderWindow& window)
{
	Font arialFont = loadFontFromFile("arial.ttf");
	drawText(window, arialFont, titleText, Color::White, WINDOW_WIDTH / 2 - 140, 20, 25);
}

void drawMainWindow(RenderWindow& window)
{
	int stopButtonX = obtineXButonStop();
	int stopButtonY = obtineYButonStop();
	int stopButtonWidth = obtineLatimeButonStop();
	int stopButtonHeight = obtineInaltimeButonStop();

	Font arialFont = loadFontFromFile("arial.ttf");

	if (endGame)
	{
		drawText(window, arialFont, titleText, Color::White, WINDOW_WIDTH / 2 - 200, 20, 25);
	}
	else
	{
		drawText(window, arialFont, titleText, Color::White, WINDOW_WIDTH / 2 - 140, 20, 25);
	}

	drawButton(window, "Stop", stopButtonX, stopButtonY, stopButtonWidth, stopButtonHeight, arialFont, 17, Color::White, Color::Magenta
	);

	drawMatrix(window);
}

void drawSelectDimensionWindow(RenderWindow& window)
{
	Font arialFont = loadFontFromFile("arial.ttf");

	vector<Vector2i> buttonsPositions = getButtonsPositions();

	vector<string> buttonsTexts = { "4x4", "5x5", "6x6", "7x7", "8x8" };

	drawText(window, arialFont, "Alegeti dimensiunea matricei!", Color::White, WINDOW_WIDTH / 2 - 160, 75, 25);

	for (int i = 0; i < 5; i++)
		drawButton(window, buttonsTexts[i], buttonsPositions[i].x, buttonsPositions[i].y, BUTTON_WIDTH, BUTTON_HEIGHT, arialFont, 17, Color::White, Color::Magenta);
}

int getLineIndex(int y)
{
	int difference = y - getBeginMatrixY();

	return getLowerMultiple(difference) / BOX_HEIGHT;
}

int getColumnIndex(int x)
{
	int difference = x - getBeginMatrixX();

	return difference / BOX_WIDTH;
}

int getPiece(int nrClicks)
{
	
	return nrClicks % 3 == 0 ? 3 : nrClicks % 3;
}

void initializare()
{
	currentFirstPlayer = true;
	elementPrecedent = { -1, -1, -1 };
	elementCurent = { -1, -1, -1 };

	int randomNumber = getRandomNumber(1, 4);

	switch (randomNumber)
	{
	case 1:
		blackCorner = { 0, 0, 4 };
		elementCurent = { matrixSize - 1, matrixSize - 1, 1 };
		break;
	case 2:
		blackCorner = { 0, matrixSize - 1, 4 };
		elementCurent = { matrixSize - 1, 0, 1 };
		break;
	case 3:
		blackCorner = { matrixSize - 1,  matrixSize - 1, 4 };
		elementCurent = { 0, 0, 1 };
		break;
	case 4:
		blackCorner = { matrixSize - 1, 0, 4 };
		elementCurent = { 0, matrixSize - 1, 1 };
		break;
	default:
		blackCorner = { matrixSize - 1,  matrixSize - 1, 4 };
		elementCurent = { 0, 0, 1 };
		break;
	}

	changeTitleText();
	generateNextPossibleElements(elementPrecedent, elementCurent);
	matrix = initializareMatriceInt();
	matrix[elementCurent.line][elementCurent.column] = elementCurent.piece;
	matrix[blackCorner.line][blackCorner.column] = blackCorner.piece;
}

void afisareElement(Element element)
{
	cout << "Element: (" << element.line << ", " << element.column << ", " << element.piece << ")" << endl;
}

void actualizareElementPrecedent(Element& element1, Element& element2)
{
	element1.line = element2.line;
	element1.column = element2.column;
	element1.piece = element2.piece;
}

bool verificareMacarOMutarePosibilaBuna()
{
	for (int i = 0; i < nextPossibleElements.size(); i++)
		if (
			(nextPossibleElements[i].line >= 0 && nextPossibleElements[i].line < matrixSize) &&
			(nextPossibleElements[i].column >= 0 && nextPossibleElements[i].column < matrixSize)
			)
			return true;
	return false;
}

bool verificareCastig()
{
	// Cazul cand am ajuns la colt negru, oprim jocul, jucatorul ce a facut mutarea a castigat
	if (playerMove.line == blackCorner.line && playerMove.column == blackCorner.column)
	{
		string player = currentFirstPlayer ? "1" : "2";
		titleText = "Ai ajuns la coltul negru, ai castigat!";
		endGame = true;
		return true;
	}
	return false;
}

bool verificarePierdere()
{
	cout << "\n-----------------------------------------------------------\n";
	cout << "VERIFICARE PIERDERE" << endl << endl;

	generateNextPossibleElements(elementCurent, playerMove);

	displayNextPossibleElements();

	cout << "Element elementCurent: ";
	afisareElement(elementCurent);
	cout << "Element playerMove: ";
	afisareElement(playerMove);

	cout << "\n-----------------------------------------------------------\n";

	if (!verificareMacarOMutarePosibilaBuna())
	{
		string player = currentFirstPlayer ? "2" : "1";
		titleText = "Ai pierdut, a castigat jocul jucatorul " + player + "!";
		endGame = true;
		return true;
	}
	return false;
}

void procesareEvenimente(RenderWindow& window)
{
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::MouseButtonPressed)
			if (event.mouseButton.button == Mouse::Left)
			{
				int mouseButtonX = event.mouseButton.x;
				int mouseButtonY = event.mouseButton.y;

				cout << "\n-----------------------------------------------------------\n";
				cout << "PROCESS EVENTS" << endl << endl;

				cout << "Am apasat click 1 la coord: " << mouseButtonX << " " << mouseButtonY << endl;

				displayNextPossibleElements();
				cout << "Element elementPrecedent: ";
				afisareElement(elementPrecedent);
				cout << "Element elementCurent: ";
				afisareElement(elementCurent);
				cout << "Element playerMove: ";
				afisareElement(playerMove);

				cout << "getBeginMatrixX: " << getBeginMatrixX() << endl;
				cout << "getEndMatrixX + BOX_WIDTH: " << getEndMatrixX() + BOX_WIDTH << endl;
				cout << "getBeginMatrixY: " << getBeginMatrixY() << endl;
				cout << "getEndMatrixY + BOX_HEIGHT: " << getEndMatrixY() + BOX_HEIGHT << endl;

				cout << "lineIndex: " << getLineIndex(mouseButtonY) << endl;
				cout << "columnIndex: " << getColumnIndex(mouseButtonX) << endl;

				cout << "\n-----------------------------------------------------------\n";

				if (!endGame)
				{
					if (!dimensionIsSelected)
					{
						vector<Vector2i> buttonsPositions = getButtonsPositions();

						for (int i = 0; i < buttonsPositions.size(); i++)
							if ((mouseButtonX > buttonsPositions[i].x) && (mouseButtonX < (buttonsPositions[i].x + BUTTON_WIDTH)) &&
								(mouseButtonY > buttonsPositions[i].y) && (mouseButtonY < (buttonsPositions[i].y + BUTTON_HEIGHT)))
							{
								dimensionIsSelected = true;
								matrixSize = 4 + i;
								initializare();
							}
					}
					else
					{
						int stopButtonX = obtineXButonStop();
						int stopButtonY = obtineYButonStop();
						int stopButtonWidth = obtineLatimeButonStop();
						int stopButtonHeight = obtineInaltimeButonStop();

						if ((mouseButtonX > stopButtonX) && (mouseButtonX < stopButtonX + stopButtonWidth) &&
							(mouseButtonY > stopButtonY) && (mouseButtonY < stopButtonY + stopButtonHeight))
						{
							cout << "Am dat click pe STOP" << endl;
							if (nrClicks > 0)
							{
								if (!verificareCastig() && !verificarePierdere())
								{
									copyElement(elementPrecedent, elementCurent);
									copyElement(elementCurent, playerMove);
									generateNextPossibleElements(elementPrecedent, elementCurent);

									currentFirstPlayer = !currentFirstPlayer;
									firstMove = false;
									changeTitleText();
									nrClicks = 0;

									while (matrix[nextPossibleElements[0].line][nextPossibleElements[0].column] != 0
										&& nextPossibleElements[0].line != blackCorner.line
										&& nextPossibleElements[0].column != blackCorner.column
										)
									{
										playerMove = { nextPossibleElements[0].line, nextPossibleElements[0].column, matrix[nextPossibleElements[0].line][nextPossibleElements[0].column] };

										cout << "\n========================================\n";
										afisareElement(playerMove);
										cout << "\n========================================\n";

										if (verificareCastig())
											break;

										if (verificarePierdere())
											break;

										copyElement(elementPrecedent, elementCurent);
										copyElement(elementCurent, playerMove);
									}
								}
							}
						}

						int beginMatrixX = getBeginMatrixX();
						int endMatrixX = getEndMatrixX() + BOX_WIDTH;
						int beginMatrixY = getBeginMatrixY();
						int endMatrixY = getEndMatrixY() + BOX_HEIGHT;

						if ((mouseButtonX > beginMatrixX) && (mouseButtonX < endMatrixX) &&
							(mouseButtonY > beginMatrixY) && (mouseButtonY < endMatrixY))
						{

							lineIndex = getLineIndex(mouseButtonY);
							columnIndex = getColumnIndex(mouseButtonX);

							cout << lineIndex << " " << columnIndex << '\n';


							if (verifyIndexesMove(lineIndex, columnIndex))
							{
								if (lineIndex == beforeLineIndex && columnIndex == beforeColumnIndex)
								{
									nrClicks++;
								}
								else
								{
									if (beforeLineIndex != -1 && beforeColumnIndex != -1 && firstMove == true)
										matrix[beforeLineIndex][beforeColumnIndex] = 0;
									nrClicks = 1;
								}

								cout << nrClicks << '\n';

								matrix[lineIndex][columnIndex] = getPiece(nrClicks);

								playerMove = { lineIndex, columnIndex, getPiece(nrClicks) };

								cout << "Dau click in matrice, pe linia " << lineIndex << " si coloana " << columnIndex << endl;

								beforeLineIndex = lineIndex;
								beforeColumnIndex = columnIndex;
							}
						}
					}
				}
			}

		if (event.type == Event::Closed)
			window.close();
	}
}

int main()
{
	/* ADAUGARE SUNET 
	SoundBuffer buffer;

	if (!buffer.loadFromFile("sound.wav"))
	{
		cout << "Nu s-a reusit citirea sound.wav!" << endl;
		return 0;
	}

	Sound sound;

	sound.setBuffer(buffer);
	sound.setLoop(true);
	sound.play();*/

	RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Colt negru!");

	while (window.isOpen())
	{
		window.clear();

		procesareEvenimente(window);

		if (!dimensionIsSelected)
		{
			drawSelectDimensionWindow(window);
		}
		else
		{
			drawMainWindow(window);
		}

		window.display();
	}

	return 0;
}