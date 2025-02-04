#include "Table.h"

Table::Table(const Table& other) {
	_row = other._row;
	_col = other._col;
	_rowGap = other._rowGap;
	_colGap = other._colGap;
	vector <vector<Point*>> emptyGrid;
	swap(_table, emptyGrid);
	for (auto it : other._table) {
		vector <Point*> store;
		for (auto it2 : it) {
			store.push_back(it2);
		}
		_table.push_back(store);
	}
	_root = other._root;
}

Table& Table::operator= (const Table& other) {
	_row = other._row;
	_col = other._col;
	_rowGap = other._rowGap;
	_colGap = other._colGap;
	vector <vector<Point*>> emptyGrid;
	swap(_table, emptyGrid);
	for (auto it : other._table) {
		vector <Point*> store;
		for (auto it2 : it) {
			store.push_back(it2);
		}
		_table.push_back(store);
	}
	_root = other._root;
	return *this;
}

Table::~Table() {
	for (auto it : _table) {
		for (auto it2 : it) {
			delete it2;
		}
	}
	delete _root;
}

void Table::createTable() {
	for (int i = 0, y = 0; i < _row; i++, y += _colGap) {
		vector <Point*> _store;
		for (int j = 0, x = 0; j < _col; j++, x += _rowGap) {
			_store.push_back(new Point(x, y));
		}
		_table.push_back(_store);
	}
}

void Table::createTable(Point* tableCoordinate) {
	for (int i = 0, y = tableCoordinate->Y(); i < _row; i++, y += _rowGap) {
		vector <Point*> _store;
		for (int j = 0, x = tableCoordinate->X(); j < _col; j++, x += _colGap) {
			_store.push_back(new Point(x, y));
		}
		_table.push_back(_store);
	}
}

void Table::wrapText(unsigned rowPos, unsigned contentLimit) {
	for (auto it : _table[rowPos]) {
		if (it->Content()[0].size() > contentLimit) {
			vector<string> wrappedContent;
			string copy = it->Content()[0];
			while (copy.size() > contentLimit) {
				string subStringToFindLastSpace = copy.substr(0, contentLimit);
				size_t spacePos = subStringToFindLastSpace.find_last_of(" ");
				if (spacePos != subStringToFindLastSpace.npos) {
					wrappedContent.push_back(copy.substr(0, spacePos));
					copy = copy.substr(spacePos + 1, copy.size() - spacePos);
				}
				else {
					wrappedContent.push_back(copy.substr(0, contentLimit));
					copy = copy.substr(contentLimit + 1, copy.size() - contentLimit);
				}
			}
			if (wrappedContent.empty() == false) wrappedContent.push_back(copy);
			it->setWrappedContent(wrappedContent);
		}
	}
}

void Table::beautifyTable() {
	for (unsigned k = 0; k < _table.size(); k++) {
		wrapText(k, 50);
	}
	for (unsigned i = 0; i < (unsigned)_col; i++) {
		unsigned max_string = 0;
		int max_x = 0;
		for (unsigned j = 0; j < _table.size(); j++) {
			if (_table[j].size() > i + 1) { // xet xem o ke ben phai co nd ko, neu ko thi skip
				for (unsigned k = 0; k < _table[j][i]->Content().size(); k++) {
					if (_table[j][i]->Content()[k].size() > max_string) {
						max_string = _table[j][i]->Content()[k].size();
					}
				}
				if (_table[j][i]->X() > max_x) max_x = _table[j][i]->X();
			}
		}
		for (unsigned k = 0; k < _table.size(); k++) {
			if (_table[k].size() > (i + 1)) {
				_table[k][i + 1]->setX(max_x + max_string + _colGap);
			}
		}
	}
	for (unsigned i = 0; i < _table.size() - 1; i++) {
		unsigned max_y = 0;
		for (unsigned j = 0; j < _table[i].size(); j++) {
			if (_table[i][j]->Content().size() - 1 > max_y) max_y = _table[i][j]->Content().size() - 1;
		}
		for (unsigned k = 0; k < _table[i + 1].size(); k++) {
			_table[i + 1][k]->setY(_table[i][0]->Y() + max_y + _rowGap);
		}
	}
}

void Table::insertLeft(const Table& other) {
	Table* copy = new Table(other);
	if (_table.size() > copy->_table.size()) {
		int delta = _table.size() - copy->_table.size();
		for (int j = 0; j < delta; j++) { // them hang bi thieu
			vector<Point*> temp;
			temp.push_back(new Point(copy->_table[copy->_row - 1][0]->X(), copy->_table[copy->_row - 1][0]->Y() + _rowGap));
			for (int i = 1; i < copy->_col; i++) {
				temp.push_back(new Point(copy->_table[copy->_row - 1][i]->X() + _colGap, copy->_table[copy->_row - 1][i]->Y() + _rowGap));
			}
			copy->_table.push_back(temp);
			copy->_row++;
		}
	}
	vector <vector <Point*>> temp = this->getTable();
	copy->_col += _col;
	_table.clear();
	_rowGap = copy->_rowGap;
	_colGap = copy->_colGap;
	_row = copy->_row;
	_col = copy->_col;
	for (auto it : copy->_table) {
		_table.push_back(it);
	}
	for (unsigned i = 0; i < temp.size(); i++) {
		for (unsigned j = 0; j < temp[i].size(); j++) {
			_table[i].push_back(temp[i][j]);
		}
	}
}

void Table::insertRight(const Table& other){
	if (other._table.size() > _table.size()) {
		int delta = other._table.size() - _table.size();
		for (int j = 0; j < delta; j++) { // them hang bi thieu
			vector<Point*> temp;
			for (int i = 0; i < _col; i++) {
				temp.push_back(new Point(_table[_row - 1][i]->X(), _table[_row - 1][i]->Y() + _rowGap));
			}
			_table.push_back(temp);
			_row++;
		}
	}
	for (unsigned i = 0; i < other._table.size(); i++) {
		for (unsigned j = 0; j < other._table[i].size(); j++) {
			_table[i].push_back(other._table[i][j]);
		}
	}
	_col += other._col;
}

void Table::insertAbove(const Table& other) {
	int row = _row;
	int col = _col;
	vector <vector <Point*>> copy = this->getTable();
	_table.clear();
	_rowGap = other._rowGap;
	_colGap = other._colGap;
	_row = row + other._row;
	if (other._col > col) _col = other._col;
	else _col = col;
	for (auto it : other._table) {
		_table.push_back(it);
	}
	for (auto it2 : copy) {
		_table.push_back(it2);
	}
}

void Table::insertBelow(const Table& other) {
	for (unsigned i = 0; i < other._table.size(); i++) {
		_table.push_back(other._table[i]);
		_row++;
	}
	if (other._col > _col) _col = other._col;
}

void Table::insertRowBelow(const vector<string>& values) {
	vector <Point*> newRow;
	int x = _table.empty() != true ? _table.back()[0]->X() : _root->X();
	int y = _table.empty() != true ? _table.back()[0]->Y() : _root->Y();
	int rowGap = _table.empty() != true ? _rowGap : 0;
	for (auto it : values) {
		newRow.push_back(new Point(x++, y + rowGap, it));
	}
	_row++;
	if (values.size() > (unsigned)_col) _col = values.size();
	_table.push_back(newRow);
}

void Table::showTableContent() {
	beautifyTable();
	for (auto it : _table) {
		for (auto it2 : it) {
			it2->print();
		}
	}
}

Point* Table::moveWithinTable() {
	unsigned row = 0, col = 0;
	_table[0][0]->setPointerTo();
	bool flagContinue = true;
	while (flagContinue) {
		switch (toupper(_getch())) {
		case 'W': {
			if (row > 0 && _table[row - 1].size() > col) {
				_table[--row][col]->setPointerTo();
			}
			break;
		}
		case 'S': {
			if (row < (unsigned)Row() - 1 && _table[row + 1].size() > col) {
				_table[++row][col]->setPointerTo();
			}
			break;
		}
		case 'A': {
			if (col > 0) {
				_table[row][--col]->setPointerTo();
			}
			break;
		}
		case 'D': {
			if (col < _table[row].size() - 1) {
				_table[row][++col]->setPointerTo();
			}
			break;
		}
		case 13: {
			flagContinue = false;
			break;
		}
		case 8: {
			row = -1;
			col = -1;
			flagContinue = false;
		}
		}
	}
	return new Point(row, col);
}

void Table::deleteRow(unsigned rowPos) {
	if (rowPos < _table.size()) {
		_table.erase(_table.begin() + rowPos);
		_row--;
	}
}