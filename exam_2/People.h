#pragma once
#include "stdafx.h"
#include "Encryptor.h"

using namespace std;

int g_cur_student = 0;
//<-- ���������� ������������� ����� ��� ������������� -->//
class Person {
protected:
	char username[256];
	char password[256];

	bool virtual welcome() {
		cout << "0. ���� � ������� 1. ���������� � �������\n-> ";
		bool choice;
		cin >> choice;
		return choice;
	}
public:
	Person() :username(""), password("") {

	}

};

//<-- ���������� ������ ������������/�������� -->//
class Student : public Person {
protected:
	char FIO[256];
	char address[256];
	char phone[256];
	char marks[1024];
	vector<Student> students;

private:
	bool virtual login() {
		char name[256];
		cout << "������� �����: ";
		cin.getline(name, 256);
		cin.getline(name, 256);
		char pass[256];
		cout << "������� ������: ";
		cin.getline(pass, 256);
		strcat_s(name, "\n");
		strcat_s(pass, "\n");

		for (int i = 0; i < students.size(); i++) {
			if (strcmp(name, students[i].username) == 0 && strcmp(pass, students[i].password) == 0) {
				cout << "������ ��������!" << endl;
				sp;
				g_cur_student = i;
				return true;
			}
		}
		cout << "�������� ����� ��� ������!" << endl;
		return false;
	}
	void virtual saveToFile(const char* fileName) {
		FILE* f;
		fopen_s(&f, fileName, "w+b");

		for (int i = 0; i < students.size(); i++) {
			char buffer[256] = "";
			strcpy_s(buffer, encrypt(students[i].username));
			fwrite(buffer, strlen(students[i].username), sizeof(char), f);

			strcpy_s(buffer, encrypt(students[i].password));
			fwrite(buffer, sizeof(char), strlen(students[i].password), f);

			strcpy_s(buffer, encrypt(students[i].FIO));
			fwrite(buffer, sizeof(char), strlen(students[i].FIO), f);

			strcpy_s(buffer, encrypt(students[i].address));
			fwrite(buffer, sizeof(char), strlen(students[i].address), f);

			strcpy_s(buffer, encrypt(students[i].phone));
			fwrite(buffer, sizeof(char), strlen(students[i].phone), f);

			strcpy_s(buffer, encrypt(students[i].marks));
			fwrite(buffer, sizeof(char), strlen(students[i].marks), f);
		}

		fclose(f);
	}
public:
	Student() :Person(), FIO(""), address(""), phone(""), marks("") {

	}
	bool virtual registration() {
		Student s;
		char name[256];
		cout << "���������� �����: ";
		cin.getline(name, 256);
		cin.getline(name, 256);
		strcat_s(name, "\n");

		for (int i = 0; i < students.size(); i++) {
			if (strcmp(students[i].username, name) == 0) {
				cout << "����� ����� ��� ����������!" << endl;
				return false;
			}
		}
		strcpy_s(s.username, name);


		cout << "���������� ������: ";
		cin.getline(s.password, 256);
		strcat_s(s.password, "\n");
		cout << "������� ���� ���: ";
		cin.getline(s.FIO, 256);
		strcat_s(s.FIO, "\n");
		cout << "������� ���� ������: ";
		cin.getline(s.address, 256);
		strcat_s(s.address, "\n");
		cout << "������� ���� �������: ";
		cin.getline(s.phone, 256);
		strcat_s(s.phone, "\n");
		strcpy_s(s.marks, "\n");

		students.push_back(s);

	}
	bool virtual authentication() {
		bool choice = welcome();
		return (choice == false) ? login() : registration();
	}
	void virtual greeting() {
		cout << "������ �������\n������������ " << students[g_cur_student].FIO;
	}
	void virtual showInfo(int index) {
		cout << "�����: " << students[index].username << "������: " << students[index].password << "���: " << students[index].FIO
			<< "�����: " << students[index].address << "����� ��������: " << students[index].phone << "���������� ������� ������������: " << students[index].marks << endl;
	}
	void loadFromFile(const char* fileName, int len = 0) {
		Student s;
		FILE* f;
		fopen_s(&f, fileName, "a");
		fclose(f);
		fopen_s(&f, fileName, "r");
		if (_filelength(_fileno(f)) == 0) {
			fclose(f);
			return;
		}

		char buffer[2048] = "";
		char name[256] = "";
		char pass[256] = "";
		char fio[256] = "";
		char _address[256] = "";
		char phoneNumber[256] = "";
		char mark[1024] = "";

		fseek(f, (sizeof(char)*(strlen(name) + strlen(_address) + strlen(phoneNumber) + strlen(mark) + strlen(fio) + strlen(pass)) + len), 0);

		/*if (len != 0) {
			fgets(buffer, sizeof(name), f);
		}*/
		fgets(name, sizeof(name), f);
		fgets(pass, sizeof(pass), f);
		fgets(fio, sizeof(fio), f);
		fgets(_address, sizeof(_address), f);
		fgets(phoneNumber, sizeof(phoneNumber), f);
		fgets(mark, sizeof(mark), f);

		strcpy_s(s.username, decrypt(name));
		strcpy_s(s.password, decrypt(pass));
		strcpy_s(s.FIO, decrypt(fio));
		strcpy_s(s.address, decrypt(_address));
		strcpy_s(s.phone, decrypt(phoneNumber));
		strcpy_s(s.marks, decrypt(mark));

		students.push_back(s);

		if (_filelength(_fileno(f)) > (sizeof(char)*(strlen(name) + strlen(_address) + strlen(phoneNumber) + strlen(mark) + strlen(fio) + strlen(pass)) + len + 14)) {
			fclose(f);
			loadFromFile(fileName, (sizeof(char)*(strlen(name) + strlen(_address) + strlen(phoneNumber) + strlen(mark) + strlen(fio) + strlen(pass)) + len));
		}
		fclose(f);
	}
	void virtual end() {
		saveToFile("students.txt");
	}
	int getSize() {
		return students.size();
	}
	void virtual changeUserName(int ind, const char* nUsername) {
		strcpy_s(students[ind].username, nUsername);
		strcat_s(students[ind].username, "\n");
	}
	void virtual changePassword(int ind, const char* nPass) {
		strcpy_s(students[ind].password, nPass);
		strcat_s(students[ind].password, "\n");
	}
	void virtual changeAddress(int ind, const char* nAddress) {
		strcpy_s(students[ind].address, nAddress);
		strcat_s(students[ind].address, "\n");
	}
	void virtual changePhone(int ind, const char* nPhone) {
		strcpy_s(students[ind].phone, nPhone);
		strcat_s(students[ind].phone, "\n");
	}
	void virtual changeMarks(int ind, const char* nMarks) {
		strcpy_s(students[ind].marks, nMarks);
		strcat_s(students[ind].marks, "\n");
	}
	void virtual changeFIO(int ind, const char* nFIO) {
		strcpy_s(students[ind].FIO, nFIO);
		strcat_s(students[ind].FIO, "\n");
	}
	void deleteStud(int ind) {
		if (ind < 0 || ind >= students.size()) {
			cout << "������������ ����!" << endl;
			sp;
			return;
		}
		students.erase(students.begin() + ind);
	}
	void virtual addMark(int id, int mark) {
		char buffer[10] = "";
		_itoa_s(mark, buffer, 10);
		strcat_s(buffer, " \n");
		encrypt(buffer);
		students[id].marks[strlen(students[id].marks) - 1] = '\0';
		strcat_s(students[id].marks, buffer);
	}
};

//<-- ���������� ������ ������ -->//
class Admin : public Person {
public:
	Admin() :Person() {

	}
	bool virtual authentication() {
		bool choice = welcome();
		return (choice == false) ? login() : registration();
	}
	void virtual greeting() {
		cout << "������ �������\n������������ " << username << endl;
	}
	void virtual showStudentInfo(Student s) {
		for (int i = 0; i < s.getSize(); i++) {
			cout << "����������� � " << i << endl;
			s.showInfo(i);
			cout << endl;
		}
	}
	void virtual changeUserName(const char* nUserName) {
		FILE* f;
		fopen_s(&f, "admin.txt", "w+");
		char buffer[256] = "";

		strcpy_s(buffer, nUserName);
		strcat_s(buffer, "\n");
		strcpy_s(username, buffer);
		strcpy_s(buffer, encrypt(buffer));

		fwrite(buffer, strlen(buffer), 1, f);

		strcpy_s(buffer, encrypt(password));
		fwrite(buffer, strlen(password), 1, f);

		fclose(f);
	}
	void virtual changePassword(const char* nPass) {
		FILE* f;
		fopen_s(&f, "admin.txt", "w+");
		char buffer[256] = "";

		strcpy_s(buffer, encrypt(username));
		fwrite(buffer, strlen(username), 1, f);

		strcpy_s(buffer, nPass);
		strcat_s(buffer, "\n");
		strcpy_s(password, buffer);
		strcpy_s(buffer, encrypt(buffer));
		fwrite(buffer, strlen(buffer), 1, f);

		fclose(f);
	}
	void changeStudentInfo(Student& stud) {
		int ind;
		short int choice3_2;
		char buffer[1024] = "";
		cout << "������� ����� ������������: ";
		cin >> ind;
		if (ind >= stud.getSize() || ind < 0) {
			cout << "������������� ����!" << endl;
			sp;
			sc;
			return;
		}
		if (stud.getSize() == 0) {
			cout << "� ������� ��� �� ���������������� �� ������ ������������!" << endl;
			sp;
			sc;
			return;
		}
		for (bool d3 = true; d3; ) {
			stud.showInfo(ind);
			cout << "��� �� ������ ��������? \n1. ��� 2. ����� 3. ����� �������� 4. ������ 5. ����� 6. ������ 0. �����\n-> ";

			cin >> choice3_2;
			switch (choice3_2) {
			case 1:
				cout << "������� ����� ���: ";
				cin.ignore();
				cin.getline(buffer, 256);
				stud.changeFIO(ind, buffer);
				sc;
				break;
			case 2:
				cout << "������� ����� �����: ";
				cin.ignore();
				cin.getline(buffer, 256);
				stud.changeAddress(ind, buffer);
				sc;
				break;
			case 3:
				cout << "������� ����� ����� ��������: ";
				cin.ignore();
				cin.getline(buffer, 256);
				stud.changePhone(ind, buffer);
				sc;
				break;
			case 4:
				cout << "������� ����� ������: ";
				cin.ignore();
				cin.getline(buffer, 256);
				stud.changeMarks(ind, buffer);
				sc;
				break;
			case 5:
				cout << "������� ����� �����: ";
				cin.ignore();
				cin.getline(buffer, 256);
				stud.changeUserName(ind, buffer);
				sc;
				break;
			case 6:
				cout << "������� ����� ������: ";
				cin.ignore();
				cin.getline(buffer, 256);
				stud.changePassword(ind, buffer);
				sc;
				break;
			case 0:
				d3 = false;
				sc;
				break;
			default:
				cout << "������������ ����!" << endl;
				sp;
				sc;
				break;
			}
		}
	}
private:
	bool virtual login() {
		FILE* f;
		fopen_s(&f, "admin.txt", "a"); // ���� ����� ���, �� ��� �������
		fclose(f);
		fopen_s(&f, "admin.txt", "r");
		if (_filelength(_fileno(f)) == 0) {
			fclose(f);
			cout << "�� ����� ������ �� �������!" << endl;
			return false;
		}
		char buffer_n[256] = "";
		char buffer_p[256] = "";
		fgets(buffer_n, sizeof(buffer_n), f);
		fgets(buffer_p, sizeof(buffer_p), f);
		fclose(f);

		char name[256];
		cout << "������� �����: ";
		cin.getline(name, 256);
		cin.getline(name, 256);
		char pass[256];
		cout << "������� ������: ";
		cin.getline(pass, 256);


		strcat_s(name, "\n");
		strcat_s(pass, "\n");


		if (strcmp(name, decrypt(buffer_n)) == 0 && strcmp(pass, decrypt(buffer_p)) == 0) {
			strcpy_s(username, name);
			strcpy_s(password, pass);
			cout << "������ ��������!" << endl;
			return true;
		}
		else {
			cout << "������������ ����� ��� ������!" << endl;
			return false;
		}
	}
	bool virtual registration() {
		FILE* f;
		fopen_s(&f, "admin.txt", "a"); // ���� ����� ���, �� ��� �������
		fclose(f);
		fopen_s(&f, "admin.txt", "r");

		if (_filelength(_fileno(f)) != 0) {
			cout << "������� ������ �������������� ��� �������! � ������� ����� ���� ������ ���� �������������\n";
			return false;
		}
		fclose(f);
		fopen_s(&f, "admin.txt", "w+");
		cout << "���������� �����: ";
		cin.getline(username, 256);
		cin.getline(username, 256);
		cout << "���������� ������: ";
		cin.getline(password, 256);

		char buffer[256];
		strcat_s(username, "\n");
		strcpy_s(buffer, encrypt(username));
		fwrite(buffer, strlen(buffer), 1, f);

		strcat_s(password, "\n");
		strcpy_s(buffer, encrypt(password));
		fwrite(buffer, strlen(buffer), 1, f);
		fclose(f);
	}
};

