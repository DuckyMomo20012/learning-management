#include "Point.h"

Point::Point(const Point& other) {
	_x = other._x;
	_y = other._y;
	_content = other._content;
}

Point& Point::operator= (const Point& other) {
	_x = other._x;
	_y = other._y;
	_content = other._content;
	return *this;
}

void Point::goTo(int x, int y) {
	HANDLE hConsoleOutput;
	COORD Cursor;
	Cursor.X = x;
	Cursor.Y = y;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, Cursor); // DI CHUYEN 
}

void Point::goTo() {
	HANDLE hConsoleOutput;
	COORD Cursor;
	Cursor.X = _x;
	Cursor.Y = _y;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, Cursor); // DI CHUYEN 
}

void Point::setPointerTo() {
	goTo(_x - 1, _y);
}

string Point::controlConsoleInput(unsigned max_size) {
	goTo(_x, _y);
	while (1) {
		char type = _getch();
		if ((_content.size() <= max_size /* Check co bo dau "=" ko*/) && ((type >= 48 && type <= 57) || (type >= 64 && type <= 90) || (type >= 97 && type <= 122))) {
			goTo(_x + _content.size(), _y);
			cout << type;
			_content.push_back(type);
		}
		else if (8 == type && _content.size() > 0) {
			_content.pop_back();
			goTo(_x + _content.size(), _y);
			cout << " ";
			goTo(_x + _content.size(), _y);
		}
		else if (27 == type) {
			clearPrintedContent();
			_content.clear();
			break;
		}
		else if (13 == type) {
			break;
		}
	}
	return _content;
}

void Point::clearPrintedContent() {
	for (unsigned i = 0; i < _content.size(); i++) {
		goTo(_x + i, _y);
		cout << " ";
	}
	goTo(_x, _y);
	_content.clear();
}

ostream& operator<<(ostream& out, Point& object) {
	object.goTo();
	out << object.Content();
	return out;
}

void Point::operator>> (string content) {
	setContent(content);
}