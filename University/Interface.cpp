#include "Interface.h"

State& State::operator= (const State& other) {
	_user = other._user;
	_exitFlag = other._exitFlag;
	_goBackFlag = other._goBackFlag;
	_menuTable = other._menuTable;
	return *this;
}

State::~State() {
	delete _user;
	delete _menuTable;
}

void State::initializeMenuTable() {
	_menuTable->insertRowBelow({ "INFO", "SCHEDULE", "TRANSCRIPT", "ENROLL", "EXIT"});
}

void MainPage::initializePage() {
	setIPageTable(getStateIPage()->getMenuTable());
}

void MainPage::executeFunction(Point* locate) {
	if (locate->X() == 0 && locate->Y() == getIPageTable()->Col() - 1) {
		State* tempState = getStateIPage();
		tempState->setExitFlag(Interface::YesNoQuestionBox(new Point(2, 2), "Do u want to exit?"));
		setStateIPage(tempState);
	}
}

void InfoPage::initializePage() {
	Table* infoPage = new Table(3, 3, 2, 4);
	infoPage->insertRowBelow({ "ID: " + getStateIPage()->User()->Id() });
	infoPage->insertRowBelow({ "NAME: " + getStateIPage()->User()->Name() });
	infoPage->insertRowBelow({ "D.O.B: " + getStateIPage()->User()->DOB() });
	infoPage->insertRowBelow({ "TELEPHONE: " + getStateIPage()->User()->Telephone() });
	infoPage->insertRowBelow({ "EMAIL: " + getStateIPage()->User()->Email() });
	infoPage->insertRowBelow({ "ADDRESS: " + getStateIPage()->User()->getAddress() });
	infoPage->insertAbove(*getStateIPage()->getMenuTable());
	setIPageTable(infoPage);
}

void InfoPage::executeFunction(Point* locate) {
	if (locate->X() == 0 && locate->Y() == getIPageTable()->Col() - 1) {
		State* tempState = getStateIPage();
		tempState->setExitFlag(Interface::YesNoQuestionBox(new Point(2, 2), "Do u want to exit?"));
		setStateIPage(tempState);
	}
	else if (locate->X() == 2 && locate->Y() == 0) {
		State* tempState = getStateIPage();
		string editString = edit(getIPageTable()->getTable()[locate->X()][locate->Y()], "NAME: ");
		if (editString != "") {
			tempState->User()->setName(editString);
		}
		setStateIPage(tempState);
	}
	else if (locate->X() == 3 && locate->Y() == 0) {
		State* tempState = getStateIPage();
		string editString = edit(getIPageTable()->getTable()[locate->X()][locate->Y()], "D.O.B: ");
		if (editString != "") {
			tempState->User()->setDOB(editString);
		}
		setStateIPage(tempState);
	}
	else if (locate->X() == 4 && locate->Y() == 0) {
		State* tempState = getStateIPage();
		string editString = edit(getIPageTable()->getTable()[locate->X()][locate->Y()], "TELEPHONE: ");
		if (editString != "") {
			tempState->User()->setTelephone(editString);
		}
		setStateIPage(tempState);
	}
	else if (locate->X() == 5 && locate->Y() == 0) {
		State* tempState = getStateIPage();
		string editString = edit(getIPageTable()->getTable()[locate->X()][locate->Y()], "EMAIL: ");
		if (editString != "") {
			tempState->User()->setEmail(editString);
		}
		setStateIPage(tempState);
	}
	else if (locate->X() == 2 && locate->Y() == 0) {
		State* tempState = getStateIPage();
		string editString = edit(getIPageTable()->getTable()[locate->X()][locate->Y()], "ADDRESS: ");
		if (editString != "") {
			tempState->User()->setAddress(editString);
		}
		setStateIPage(tempState);
	}
	else if (locate->X() == -1 && locate->Y() == -1) {
		State* tempState = getStateIPage();
		tempState->setGoBackFlag(true);
		setStateIPage(tempState);
	}
}

string InfoPage::edit(Point*& locate, string ignoreString) {
	string editString;
	bool confirmChange = false;
	string oldContent = locate->Content();
	locate->clearPrintedContent();
	locate->setContent(ignoreString);
	cout << *locate;
	Point* editBox = new Point(locate->X() + locate->Content().size(), locate->Y());
	editString = editBox->controlConsoleInput(0, 20);
	Point* confirmBox = new Point(locate->X() + locate->Content().size() + editString.size() + 1 /*Khoang cach giua edit va confirm*/, locate->Y());
	if (editString != "") {
		confirmChange = Interface::YesNoQuestionBox(confirmBox, "Confirm? ");
	}
	if (confirmChange == false) {
		editBox->clearPrintedContent();
		locate->clearPrintedContent();
		locate->setContent(oldContent);
		cout << *locate;
		editString = "";
	}
	else {
		locate->setContent(ignoreString + editString);
	}
	delete confirmBox, editBox;
	return editString;
}

void SchedulePage::initializePage() {
	vector <string> weekday = { "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY" };
	Table* schedulePage(new Table(3, 3, 2, 2, 5, 6));
	schedulePage->getTable()[0][0]->setContent("SHIFT");
	schedulePage->getTable()[1][0]->setContent("1");
	schedulePage->getTable()[2][0]->setContent("2");
	schedulePage->getTable()[3][0]->setContent("3");
	schedulePage->getTable()[4][0]->setContent("4");
	for (int i = 1; i < 6; i++) { // 6 ngay trong tuan
		schedulePage->getTable()[0][i]->setContent(weekday[i - 1]);
		for (auto it : getStateIPage()->User()->getCourse()) {
			map<string, vector<string>> time = it->Time();
			if (time.find(weekday[i - 1]) != time.end()) {
				for (auto shiftAmount : time[weekday[i - 1]]) { // so ca hoc trong ngay
					string shift = shiftAmount;
					schedulePage->getTable()[stoi(shift)][i]->setContent(it->Name());
				}
			}
		}
	}
	schedulePage->insertAbove(*getStateIPage()->getMenuTable());
	setIPageTable(schedulePage);
}

void SchedulePage::executeFunction(Point* locate) {
	if (locate->X() == 0 && locate->Y() == getIPageTable()->getTable()[0].size() - 1) {
		State* tempState = getStateIPage();
		tempState->setExitFlag(Interface::YesNoQuestionBox(new Point(2, 2), "Do u want to exit?"));
		setStateIPage(tempState);
	}
	else if (locate->X() == -1 && locate->Y() == -1) {
		State* tempState = getStateIPage();
		tempState->setGoBackFlag(true);
		setStateIPage(tempState);
	}
}

void EnrollPage::initializePage() {
	getCourse();
	Table* enrollPage = new Table(3, 3, 2, 4);
	for (auto it : _courseStore) {
		stringstream courseInfo;
		courseInfo << "ID: " << it.first->Id() << " NAME: " << it.first->Name() << " TIME:";
		for (auto it2 : it.first->Time()) {
			courseInfo << " " + it2.first + ": SHIFT: ";
			for (auto it3 : it2.second) {
				courseInfo <<  it3 << " ";
			}
		}
		courseInfo << "SLOT: " << it.second;
		enrollPage->insertRowBelow({ courseInfo.str() });
	}
	enrollPage->insertAbove(*getStateIPage()->getMenuTable());
	if (_courseChosenStore.size() != 0) {
		enrollPage->beautifyTable();
		Table* _courseChosenTable = new Table(enrollPage->getTable().back()[0]->X(), enrollPage->getTable().back()[0]->Y() + 4, 2, 4);
		_courseChosenTable->insertRowBelow({ "CHOSEN COURSE:" });
		for (auto it : _courseChosenStore) {
			stringstream courseInfo;
			courseInfo << "ID: " << it->Id() << " NAME: " << it->Name() << " TIME:";
			for (auto it2 : it->Time()) {
				courseInfo << " " << it2.first << ": SHIFT: ";
				for (auto it3 : it2.second) {
					courseInfo <<  it3 << " " ;
				}
			}
			string warning = checkCourseHasSameTime(it);
			courseInfo << warning;
			_courseChosenTable->insertRowBelow({ courseInfo.str() });
		}
		enrollPage->insertBelow(*_courseChosenTable);
		enrollPage->insertRowBelow({ "SAVE CHANGES " });
	}
	setIPageTable(enrollPage);
}

void EnrollPage::executeFunction(Point* locate) {
	if (locate->X() == 0 && locate->Y() == getIPageTable()->Col() - 1) {
		State* tempState = getStateIPage();
		tempState->setExitFlag(Interface::YesNoQuestionBox(new Point(2, 2), "Do u want to exit?"));
		setStateIPage(tempState);
	}
	else if ((locate->X() >= 1 && locate->X() <= _courseStore.size()) && 0 == locate->Y()) {
		pickCourse(locate);
		refresh();
	}
	else if ((locate->X() > _courseStore.size() && locate->X() < getIPageTable()->getTable().size() - 1) && 0 == locate->Y()) {
		deleteCourse(locate);
		refresh();
	}
	else if (locate->X() == getIPageTable()->getTable().size() - 1 && 0 == locate->Y()) {
		saveChanges(getIPageTable()->getTable()[locate->X()][0]);
		refresh();
	}
	else if (locate->X() == -1 && locate->Y() == -1) {
		State* tempState = getStateIPage();
		tempState->setGoBackFlag(true);
		setStateIPage(tempState);
	}
}

void EnrollPage::getCourse() {
	fstream file("Course.json", ios::in);
	if (file.fail()) cout << "CANNOT OPEN FILE!!!";
	else {
		vector<pair<Course*, string>> empty;
		_courseStore.swap(empty);
		json arrayCourse = json::array();
		vector <Course*> tempCourse = getStateIPage()->User()->getCourse();
		file >> arrayCourse;
		for (auto it : arrayCourse) {
			bool find = false;
			for (auto it2 : tempCourse) {
				if (it2->Id() == it["id"]) {
					find = true;
					break;
				}
			}
			if (find == false && it["slot"] > 0) {
				stringstream info;
				info << it["slot"] << "/" << it["totalSlot"];
				_courseStore.push_back(make_pair(new Course(it), info.str()));
			}
		}
	}
	file.close();
}

void EnrollPage::refresh() {
	initializePage();
}

void EnrollPage::increaseSlot(Course* chosenCourse) {
	ifstream file("Course.json", ios::in);
	if (file.fail()) cout << "CANNOT OPEN FILE!!!";
	else {
		json arrayCourse = json::array();
		file >> arrayCourse;
		for (unsigned i = 0; i < arrayCourse.size(); i++) {
			bool flagRightCourse = false;
			for (unsigned j = 0; j < arrayCourse[i]["time"].size(); j++) {
				map<string, vector<string>> time = chosenCourse->Time();
				flagRightCourse = time.find(arrayCourse[i]["time"][j]["weekday"]) != time.end() ? true : false;
				if (flagRightCourse == false) break;
				vector<string> shift;
				for (unsigned k = 0; k < arrayCourse[i]["time"][j]["shift"].size(); k++) {
					shift.push_back(arrayCourse[i]["time"][j]["shift"][k]);
				}
				flagRightCourse = chosenCourse->Time()[arrayCourse[i]["time"][j]["weekday"]] == shift ? true : false;
				if (flagRightCourse == false) break;
			}
			if (flagRightCourse == true) {
				arrayCourse[i]["slot"] = arrayCourse[i]["slot"] + 1;
			}
		}
		ofstream fileout("Course.json", ios::trunc);
		if (fileout.fail()) cout << "CANNOT OPEN FILE";
		else {
			fileout << arrayCourse.dump(4);
		}
		fileout.close();
	}
	file.close();
}

void EnrollPage::decreaseSlot(Course* chosenCourse) {
	ifstream file("Course.json", ios::in);
	if (file.fail()) cout << "CANNOT OPEN FILE!!!";
	else {
		json arrayCourse = json::array();
		file >> arrayCourse;
		for (unsigned i = 0; i < arrayCourse.size(); i++) {
			bool flagRightCourse = false;
			for (unsigned j = 0; j < arrayCourse[i]["time"].size(); j++) {
				map<string, vector<string>> time = chosenCourse->Time();
				flagRightCourse = time.find(arrayCourse[i]["time"][j]["weekday"]) != time.end() ? true : false;
				if (flagRightCourse == false) break;
				vector<string> shift;
				for (unsigned k = 0; k < arrayCourse[i]["time"][j]["shift"].size(); k++) {
					shift.push_back(arrayCourse[i]["time"][j]["shift"][k]);
				}
				flagRightCourse = chosenCourse->Time()[arrayCourse[i]["time"][j]["weekday"]] == shift ? true : false;
				if (flagRightCourse == false) break;
			}
			if (flagRightCourse == true) {
				arrayCourse[i]["slot"] = arrayCourse[i]["slot"] - 1;
			}
		}
		ofstream fileout("Course.json", ios::trunc);
		if (fileout.fail()) cout << "CANNOT OPEN FILE";
		else {
			fileout << arrayCourse.dump(4);
		}
		fileout.close();
	}
	file.close();
}

void EnrollPage::pickCourse(Point* locate) {
	Course* chosenCourse = _courseStore[locate->X() - 1].first;
	if (_courseChosenStore.empty() == true) {
		_courseChosenStore.push_back(chosenCourse);
		decreaseSlot(chosenCourse);
	}
	else {
		_courseChosenStore.push_back(chosenCourse);
		decreaseSlot(chosenCourse);
	}
}

void EnrollPage::deleteCourse(Point* locate) {
	bool confirmChange = false;
	Point* confirmBox = new Point
	(getIPageTable()->getTable()[locate->X()][locate->Y()]->X() + getIPageTable()->getTable()[locate->X()][locate->Y()]->Content().size() + 1 /*Khoang cach giua edit va confirm*/, getIPageTable()->getTable()[locate->X()][locate->Y()]->Y());
	confirmChange = Interface::YesNoQuestionBox(confirmBox, "Confirm remove this course? ");
	if (confirmChange == true) {
		increaseSlot(_courseChosenStore[locate->X() - _courseStore.size() - 2]);
		_courseChosenStore.erase(_courseChosenStore.begin() + (locate->X() - _courseStore.size() - 2)); // loai dong chosen course 
		getIPageTable()->deleteRow(locate->X());
	}
	delete confirmBox;
}

void EnrollPage::saveChanges(Point* locate) {
	bool confirmChange = false;
	Point* confirmBox = new Point(locate->X() + locate->Content().size() + 1 /*Khoang cach giua edit va confirm*/, locate->Y());
	confirmChange = Interface::YesNoQuestionBox(confirmBox, "Confirm? ");
	if (confirmChange == true) {
		for (auto it : _courseChosenStore) {
			getStateIPage()->User()->pushBackCourse(it);
		}
		_courseChosenStore.clear();
	}
	delete confirmBox;
}

string EnrollPage::checkCourseHasSameTime(Course* course) {
	bool flagCheckChosenCourse = false;
	bool flagCheckStudentCourse = false;
	int countChosenCourse = 0; // tai duyet mang co chua chinh no
	int countStudentCourse = 0; // tai duyet mang co chua chinh no
	for (auto it : _courseChosenStore) {
		bool flagSameTime = course->checkSameTime(it);
		if (flagSameTime == true) {
			countChosenCourse++;
			if (countChosenCourse > 1) {
				flagCheckChosenCourse = true;
				break;
			}
		}
	}
	for (auto it : getStateIPage()->User()->getCourse()) {
		bool flagSameTime = course->checkSameTime(it);
		if (flagSameTime == true) {
			countStudentCourse++;
			if (countStudentCourse > 1) {
				flagCheckStudentCourse = true;
				break;
			}
		}
	}
	stringstream warning;
	if (flagCheckChosenCourse == true) { warning << "TRUNG GIO HOC VOI CAC KHOA DA CHON "; }
	if (flagCheckStudentCourse == true) { warning << "TRUNG GIO HOC VOI CAC KHOA TRONG TKB "; }
	return warning.str();
}

void TranscriptPage::initializePage() {
	Table* TranscriptPage = new Table(3, 3, 2, 4);
	TranscriptPage->insertRowBelow({ "Course", "ProcessPoint", "MidPoint", "FinalPoint" });
	for (auto it : getStateIPage()->User()->getCourse()) {
		vector<string> coursePoint;
		coursePoint.push_back(it->Name() + " ");
		for (auto it2 : it->Point()) {
			stringstream point;
			for (auto it3 : it2) {
				point << it3 << " ";
			}
			coursePoint.push_back(point.str());
		}
		TranscriptPage->insertRowBelow({ coursePoint });
	}
	TranscriptPage->insertAbove(*getStateIPage()->getMenuTable());
	setIPageTable(TranscriptPage);
}

void TranscriptPage::executeFunction(Point* locate) {
	if (locate->X() == 0 && locate->Y() == getIPageTable()->getTable()[0].size() - 1) {
		State* tempState = getStateIPage();
		tempState->setExitFlag(Interface::YesNoQuestionBox(new Point(2, 2), "Do u want to exit?"));
		setStateIPage(tempState);
	}
	else if (locate->X() == -1 && locate->Y() == -1) {
		State* tempState = getStateIPage();
		tempState->setGoBackFlag(true);
		setStateIPage(tempState);
	}
}

IPage* Factory::clone(Point* locate, State* state) {
	IPage* newPage = NULL;
	if (locate->X() == 0 && locate->Y() == 0) {
		newPage = new InfoPage(state);
	}
	else if (locate->X() == 0 && locate->Y() == 1) {
		newPage = new SchedulePage(state);
	}
	else if (locate->X() == 0 && locate->Y() == 2) {
		newPage = new TranscriptPage(state);
	}
	else if (locate->X() == 0 && locate->Y() == 3) {
		newPage = EnrollPage::instance(state);
	}
	return newPage;
}

void Caretaker::push_back(IPage* other) {
	if (_history.empty())  _history.push_back(other);
	else if (other != _history.back()) _history.push_back(other);
}

void Caretaker::pop_back() {
	if (_history.size() > 1) _history.pop_back();
}

void Interface::resizeConsole(int width, int height) {
	HWND console = GetConsoleWindow();
	RECT r;
	LONG style = GetWindowLong(console, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(console, GWL_STYLE, style); // LAM MO NUT MAXIMIZE
	GetWindowRect(console, &r); // THAY DOI KICH CO CMD
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}

void Interface::login() {
	Point textBox(20, 10);
	Point loginTextBox(29, 10);
	Point errorTextBox(29, 11);
	bool flag = false;
	ifstream file("Student.json", ios::in);
	if (file.fail()) cout << "Cannot open file!" << endl;
	else {
		textBox >> "NHAP ID:";
		cout << textBox;
		json allStudent = json::array();
		file >> allStudent;
		while (flag == false) {
			string id = loginTextBox.controlConsoleInput(0, 20);
			for (unsigned i = 0; i < allStudent.size(); i++) {
				if (allStudent[i]["id"] == id) {
					flag = true;
					if (!errorTextBox.isEmpty()) errorTextBox.clearPrintedContent();
					State* temp = new State();
					temp->setStudent(new Student(allStudent[i]));
					setState(temp);
					break;
				}
				else {
					loginTextBox.clearPrintedContent();
					errorTextBox >> "Can't find this id";
					cout << errorTextBox;
				}
			}
		}
	}
	file.close();
	system("cls");
}

bool Interface::YesNoQuestionBox(Point* locate, string sentence) {
	locate->setContent(sentence + " ");
	cout << *locate;
	Table confirmTable(locate->X() + locate->Content().size(), locate->Y(), 1, 2, 1, 2);
	confirmTable.getTable()[0][0]->setContent("Y");
	confirmTable.getTable()[0][1]->setContent("N");
	cout << *confirmTable.getTable()[0][0];
	cout << *confirmTable.getTable()[0][1];
	Point* confirm = confirmTable.moveWithinTable();
	locate->clearPrintedContent();
	confirmTable.getTable()[0][0]->clearPrintedContent();
	confirmTable.getTable()[0][1]->clearPrintedContent();
	if (0 == confirm->Y()) return true;
	else return false;
}

void Interface::run() {
	login();
	_care.push_back(new MainPage(getState()));
	while (getState()->ExitFlag() == false) {
		system("cls");
		getCare().getCurrentPage()->getIPageTable()->showTableContent();
		Point* locate = getCare().getCurrentPage()->getIPageTable()->moveWithinTable();
		IPage* newPage = Factory::clone(locate, getState());
		if (newPage != NULL && newPage != getCare().getCurrentPage()) {
			_care.push_back(newPage);
		}
		else _care.getCurrentPage()->executeFunction(locate);
		if (getState()->GoBackFlag() == true) {
			_care.pop_back();
			getState()->setGoBackFlag(false);
		}
	}
	system("cls");
}