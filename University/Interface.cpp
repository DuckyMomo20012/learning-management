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
	_menuTable->getTable()[0][0]->setContent("INFO");
	_menuTable->getTable()[0][1]->setContent("SHCEDULE");
	_menuTable->getTable()[0][2]->setContent("TRANSCRIPT");
	_menuTable->getTable()[0][3]->setContent("ENROLL");
	_menuTable->getTable()[0][4]->setContent("EXIT");
	_menuTable->beautifyTable();
}

void MainPage::initializePage() {
	Table* test = getStateIPage()->getMenuTable();
	setIPageTable(test);
}

void MainPage::executeFunction(Point* locate) {
	if (locate->X() == 0 && locate->Y() == getIPageTable()->Col() - 1) {
		State* tempState = getStateIPage();
		tempState->setExitFlag(Interface::YesNoQuestionBox(new Point(2, 2), "Do u want to exit?"));
		setStateIPage(tempState);
	}
}

void InfoPage::initializePage() {
	system("cls");
	Table* infoPage = new Table(3, 3, 6, 1, 2, 4);
	infoPage->getTable()[0][0]->setContent("ID: " + getStateIPage()->User()->Id());
	infoPage->getTable()[1][0]->setContent("NAME: " + getStateIPage()->User()->Name());
	infoPage->getTable()[2][0]->setContent("D.O.B: " + getStateIPage()->User()->DOB());
	infoPage->getTable()[3][0]->setContent("TELEPHONE: " + getStateIPage()->User()->Telephone());
	infoPage->getTable()[4][0]->setContent("EMAIL: " + getStateIPage()->User()->Email());
	infoPage->getTable()[5][0]->setContent("ADDRESS: " + getStateIPage()->User()->getAddress());
	infoPage->insertAbove(*getStateIPage()->getMenuTable());
	infoPage->beautifyTable();
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
	system("cls");
	vector <string> weekday = { "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY" };
	Table* schedulePage(new Table(3, 3, 5, 6, 2, 2));
	schedulePage->getTable()[0][0]->setContent("SHIFT");
	schedulePage->getTable()[1][0]->setContent("1");
	schedulePage->getTable()[2][0]->setContent("2");
	schedulePage->getTable()[3][0]->setContent("3");
	schedulePage->getTable()[4][0]->setContent("4");
	if(getStateIPage()->User()->getCourse().empty()==false)
	{
		for (int i = 1; i < 6; i++) {
			schedulePage->getTable()[0][i]->setContent(weekday[i - 1]);
			for (auto it : getStateIPage()->User()->getCourse()) {
				map<string, int> time = it->Time();
				if (time.find(weekday[i - 1]) != time.end()) {
					int shift = it->Time()[weekday[i - 1]];
					schedulePage->getTable()[shift][i]->setContent(it->Name());
				}
			}
		}
	}
	else
	{
		for (int i = 1; i < 6; i++) {
			schedulePage->getTable()[0][i]->setContent(weekday[i - 1]);
		}
		int k = 1;
		for (int i = 1; i < 5; i++)
			schedulePage->getTable()[i][k]->setContent("");
		k++;
		}
	schedulePage->insertAbove(*getStateIPage()->getMenuTable());
	schedulePage->beautifyTable();
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
json readJson(string path,json input)
{
	input = json::array();
	ifstream file(path, ios::in);
	if (file.fail()) cout << "Cannot open file!" << endl;
	else {

		file >> input;
	}
	file.close();
	return input;
}
void writeJson(string path, json output)
{
	ofstream fileo(path, ios::trunc || ios::out);
	fileo << output.dump(4);
	fileo.close();
}
void EnrollPage::initializePage()
{
	system("cls");
	//Doc file Course json
	json course;
	course = readJson("Course.json", course);
	// Dem so coures trong Course.json
	auto countCourse = course.size();
	//Doc Student.json
	json student;
	student = readJson("Student.json", student);
	//Check thu tu cua student
	State* tempState = getStateIPage();
	int I = 0;
	int J = 0;
	for (int i = 0; i < student.size(); i++)
	{
		if (student[i]["id"] == tempState->User()->Id())
		{
			I = i;
		}
	}
	delete tempState;

	//Tao Table
	Table* enrollPage = new Table(3, 3, countCourse, 2, 2, 2);
	if (student[I]["course"].size() == 0)
	{
		for (int i = 0; i < countCourse; i++)
		{

			enrollPage->getTable()[i][0]->setContent("\u25A0");
			enrollPage->getTable()[i][1]->setContent(course[i]["name"]);
		}
	}
	else
	{
		for (int i = 0; i < countCourse; i++)
		{

			enrollPage->getTable()[i][0]->setContent("\u25A0");
			enrollPage->getTable()[i][1]->setContent(course[i]["name"]);
		}
	}
	enrollPage->insertAbove(*getStateIPage()->getMenuTable());
	enrollPage->beautifyTable();
	setIPageTable(enrollPage);
}
void EnrollPage::executeFunction(Point* locate)
{
	//Doc student.json
	json student;
	student = readJson("Student.json", student);
	//Doc file course.json
	json course;
	course = readJson("Course.json", course);
	//
	State* tempState = getStateIPage();
	int I = 0;
	int J = 0;
	for (int i = 0; i < student.size(); i++)
	{
		if (student[i]["id"] == tempState->User()->Id())
		{
			I = i;
		}
	}
	delete tempState;
	vector<string>courseName;//Luu ten mon hoc
	for (int i = 0; i < course.size(); i++)
	{
		courseName.push_back(course[i]["name"]);
	}
	vector<int> mark;// luu thu tu cua mon hoc trong course
	for (int i = 0; i < student[I]["course"].size(); i++)
	{
		for (int j = 0; j < courseName.size(); j++)
		{
			if (student[I]["course"][i]["name"] == courseName[j]);
			{
				mark.push_back(i);
			}
		}
	}
	//define mark[0]=DSTT
	//mark[1]=OOP
	//
	if (locate->X() == 0 && locate->Y() == getIPageTable()->getTable()[0].size() - 1) 
	{
		State* tempState = getStateIPage();
		tempState->setExitFlag(Interface::YesNoQuestionBox(new Point(2, 2), "Do u want to exit?"));
		setStateIPage(tempState);
	}
	if (locate->X() == 1 && locate->Y() == 0) {
		State* tempState = getStateIPage();
		if (locate->Content() != "\xfb")
		{
			string editstring = edit(getIPageTable()->getTable()[locate->X()][locate->Y()]);
			if (editstring == "\xfb")
			{
				student[I]["course"].push_back(course[0]);
			}
		}
		if (locate->Content() == "\xfb")
		{
			string editstring = edit(getIPageTable()->getTable()[locate->X()][locate->Y()]);
			if (editstring != "\xfb")
			{
				student[I]["course"].erase(student[I]["course"].begin() + mark[0]);
			}
		}
		setStateIPage(tempState);
	}
	
	if (locate->X() == 2 && locate->Y() == 0) {
		State* tempState = getStateIPage();
		if (locate->Content() != "\xfb")
		{
			string editstring = edit(getIPageTable()->getTable()[locate->X()][locate->Y()]);
			if (editstring == "\xfb")
			{
						student[I]["course"].push_back(course[1]);
			}
		}
		if (locate->Content() == "\xfb")
		{
			string editstring = edit(getIPageTable()->getTable()[locate->X()][locate->Y()]);
			if (editstring != "\xfb")
			{
				student[I]["course"].erase(student[I]["course"].begin() + mark[1]);
			}
		}
		setStateIPage(tempState);
	}
	//Luu file
	writeJson("Student.json", student);
}
string EnrollPage::edit(Point*& locate)
{
	string editString="\xfb";
	bool confirmChange = false;
	string oldContent = "\u25A0";
	locate->clearPrintedContent();
	Point* tickBox = new Point(locate->X() + locate->Content().size() + editString.size() + 1 /*Khoang cach giua edit va confirm*/, locate->Y());
	if (editString != "") {
		confirmChange = Interface::YesNoQuestionBox(tickBox, "Confirm Tick? ");
	}
	if (confirmChange == false) {
		locate->clearPrintedContent();
		locate->setContent(oldContent);
		cout << *locate;
		editString = "";
	}
	else {
		locate->setContent("\xfb");
	}
	delete tickBox;
	return editString;
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
		// transcript page
	}
	else if (locate->X() == 0 && locate->Y() == 3) {
		newPage = new EnrollPage(state);
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


