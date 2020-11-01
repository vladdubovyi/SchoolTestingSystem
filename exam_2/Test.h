#pragma once
#include "stdafx.h"
#include "Encryptor.h" 

using namespace std;


//<-- ���������� ������ ��� ������ -->//
class Tests {
private:
	struct Test { //<-- ���������� �������� ��������� �����
		struct s_questions { //<-- ���������� ��������� ��������
			char question[512];
			char options[1024];

			s_questions() : question(""), options("") {

			}
		};
		vector<s_questions> v_questions; //<-- ���������� ������� ��������
		char path[_MAX_PATH];
		char section[256];
		char name[256];
		char answers[256];
		int marks;
	};
	vector<Test> v_test; //<-- ���������� ��������� ������� ������

	void loadQuestions(const char* fileName, int len, Test& t) { //<-- ������� ��� ��������� �������� �� �����
		FILE* f;
		fopen_s(&f, fileName, "r");
		Test::s_questions sq;
		char question[512] = "";
		char option1[256] = "";
		char option2[256] = "";
		char option3[256] = "";
		char buffer[1024] = "";

		fseek(f, (sizeof(char)*(strlen(question) + strlen(option1) + strlen(option2) + strlen(option3)) + len), 0); //<-- ��������� ��������� � ������ �����

		fgets(question, sizeof(question), f);
		fgets(option1, sizeof(option1), f);
		fgets(option2, sizeof(option2), f);
		fgets(option3, sizeof(option3), f);

		strcpy_s(sq.question, decrypt(question));
		strcpy_s(buffer, decrypt(option1));
		strcat_s(buffer, decrypt(option2));
		strcat_s(buffer, decrypt(option3));
		strcpy_s(sq.options, buffer);

		t.v_questions.push_back(sq); //<-- ��� ������������ ������� �������� � ��������� ����

		if (_filelength(_fileno(f)) > (sizeof(char)*(strlen(question) + strlen(option1) + strlen(option2) + strlen(option3)) + len + 14)) {
			fclose(f);
			loadQuestions(fileName, (sizeof(char)*(strlen(question) + strlen(option1) + strlen(option2) + strlen(option3)) + len + 4), t);
		}
		fclose(f);
	}

	void loadFromFile(const char* fileName) { //<-- ������� ��������� ����� �� �����
		FILE* f;
		fopen_s(&f, fileName, "r");
		if (_filelength(_fileno(f)) == 0) { //<-- ���� ���� ������, ������ �� ����������
			fclose(f);
			return;
		}

		char buffer[2048] = "";
		char section[256] = "";
		char name[256] = "";
		char answers[256] = "";
		Test t;
		//<-- ��������� �������, �������� � ������� -->//
		fgets(section, sizeof(section), f);
		fgets(name, sizeof(name), f);
		fgets(answers, sizeof(answers), f);
		strcpy_s(t.section, decrypt(section));
		strcpy_s(t.name, decrypt(name));
		strcpy_s(t.answers, decrypt(answers));
		strcpy_s(t.path, fileName);

		fclose(f);
		loadQuestions(fileName, (sizeof(char)*(strlen(name) + strlen(section) + strlen(answers))) + 3, t); //<-- ��������� ��������

		v_test.push_back(t); //<-- ��� ������������ ����� � ������

		fclose(f);
	}
public:
	void loadTests(const char* fileName, int len = 0) { //<-- �������� ������� ��������� ����� �� �����
		FILE* f;
		fopen_s(&f, fileName, "a"); //<-- ���� ���� �� ����������, ������� ���
		fclose(f);
		fopen_s(&f, fileName, "r");
		if (_filelength(_fileno(f)) == 0) {
			fclose(f);
			return;
		}

		char testFileName[256] = "";
		char buffer[256] = "";

		fseek(f, (sizeof(char)*(strlen(testFileName)) + len), 0); //<-- ��������� ��������� �� ��������� ���� � ������������������

		fgets(testFileName, sizeof(testFileName), f);
		testFileName[strlen(testFileName) - 1] = '\0';

		loadFromFile(testFileName); //<-- ��������� ����� �� �����

		if (_filelength(_fileno(f)) > (sizeof(char)*(strlen(testFileName)) + len + 14)) { //<-- ���� �� ��������� ����� �����, �������� ������� ����������
			fclose(f);
			loadTests(fileName, (sizeof(char)*(strlen(testFileName)) + len) + 2);
		}
		fclose(f);
	}
	void showTest(bool mode) { //<-- ������� ������ ���������� �� ������
		if (mode == true) { //<-- ����� ��� ������
			for (int i = 0; i < v_test.size(); i++) {
				cout << "�����: " << i << " ������: " << v_test[i].section << "��������: " << v_test[i].name << "�������:\n";
				for (int j = 0; j < v_test[i].v_questions.size(); j++) {
					cout << v_test[i].v_questions[j].question << v_test[i].v_questions[j].options;
				}
				cout << "������: " << v_test[i].answers;
			}
		}
		else { //<-- ����� ��� ���������
			for (int i = 0; i < v_test.size(); i++) {
				cout << "������: " << v_test[i].section << "��������: " << v_test[i].name << "�����: " << i << endl;
			}
		}
	}
	int beginTest(int ind) { //<-- ������� ������ ����� ��� ���������
		if (ind < 0 || ind >= v_test.size()) { //<-- �������� �� ������������ ������� �����
			cout << "������������ ����!" << endl;
			return -1;
		}
		cout << "������: " << v_test[ind].section << "��������: " << v_test[ind].name << "�����: " << ind << endl;

		vector<char> correctAnswer; //<-- ������������� ������� ���������� ������� �� ����
		double sum_marks = 0;
		double answerPrice = 12.0 / v_test[ind].v_questions.size();
		int corrAnsNum = 0;
		char studentAnswer;

		for (int i = 0; v_test[ind].answers[i] != '\n'; i++) { //<-- ���� ���������� ������� ����������� ��������
			if (v_test[ind].answers[i] != ' ') correctAnswer.push_back(v_test[ind].answers[i]);
		}

		for (int i = 0; i < v_test[ind].v_questions.size(); i++) { //<-- �������� ���� ��� ������������
			cout << v_test[ind].v_questions[i].question << v_test[ind].v_questions[i].options;
			cout << "������� �����: ";
			cin >> studentAnswer;
			if (studentAnswer == correctAnswer[i]) { //<-- �������� �� ������������ ������, ���������� ������
				sum_marks += answerPrice;
				corrAnsNum++;
			}
			sc;
		}
		sum_marks = ceil(sum_marks); //<-- ���������� ������ � ������� ��������
		cout << "���� ������: " << sum_marks << " ���������� �������: " << corrAnsNum << " ������� ���������� �������: " << (double)corrAnsNum / (double)v_test[ind].v_questions.size() * 100 << "%" << endl;
		sp;
		return sum_marks;
	}
	void removeTest(int ind) { //<-- ������� �������� �����
		if (ind < 0 || ind >= v_test.size()) {
			cout << "������������ ����!" << endl;
			return;
		}
		if (remove(v_test[ind].path) == 0) {
			cout << "�������� ������ �������!";
			v_test.erase(v_test.begin() + ind);
		}
		else cout << "���-�� ����� �� ���..";
	}
	void saveToFile(int ind, const char* fileName, bool mode = true) { //<-- ������� ���������� �����������/����������� �����
		FILE* f;
		fopen_s(&f, fileName, "w+");
		char buffer[1024] = "";
		strcpy_s(buffer, encrypt(v_test[ind].section));
		fwrite(buffer, strlen(buffer), 1, f);

		strcpy_s(buffer, encrypt(v_test[ind].name));
		fwrite(buffer, strlen(buffer), 1, f);

		strcpy_s(buffer, encrypt(v_test[ind].answers));
		fwrite(buffer, strlen(buffer), 1, f);

		for (int i = 0; i < v_test[ind].v_questions.size(); i++) {
			strcpy_s(buffer, encrypt(v_test[ind].v_questions[i].question, false));
			fwrite(buffer, strlen(buffer), 1, f);
			strcpy_s(buffer, encrypt(v_test[ind].v_questions[i].options, false));
			fwrite(buffer, strlen(buffer), 1, f);
		}

		strcpy_s(v_test[ind].path, fileName);
		fclose(f);
		if (mode == true) { //<-- ���� ���� �������� � �� �������, ���������� ��� ���� � ���� � ������ � ������
			fopen_s(&f, "tests.txt", "a");
			strcpy_s(buffer, fileName);
			strcat_s(buffer, "\n");
			fwrite(buffer, strlen(buffer), 1, f);
			fclose(f);
		}
	}
	void addTest() { //<-- ������� ���������� �����
		Test t;
		Test::s_questions sq;
		char buffer[1024] = "";

		cout << "������� ��������� �����(������, ����������): ";
		cin.ignore();
		cin.getline(buffer, 256);
		strcat_s(buffer, "\n");
		strcpy_s(t.section, buffer);

		cout << "������� �������� �����(�������������� ������, ��������): ";
		cin.getline(buffer, 256);
		strcat_s(buffer, "\n");
		strcpy_s(t.name, buffer);

		cout << "������� ���������� ������(�� ������� ��������, ����� ������): ";
		cin.getline(buffer, 256);
		strcat_s(buffer, "\n");
		strcpy_s(t.answers, buffer);

		cout << "������� �������� �������� �����(test.txt): ";
		char pathName[_MAX_PATH] = "";

		strcpy_s(buffer, "tests/");
		cin.getline(pathName, _MAX_PATH);
		strcat_s(buffer, pathName);
		strcpy_s(t.path, buffer);

		int cnt = 1;
		for (bool a = true; a; cnt++) {
			cout << "������� " << cnt << " ������: ";
			if (cnt != 1) cin.ignore();
			cin.getline(buffer, 256);
			strcat_s(buffer, "\n");
			strcpy_s(sq.question, buffer);
			for (int j = 0; j < 3; j++) {
				cout << "������� " << j << " ������� ������(� E = mc^2): ";
				cin.getline(buffer, 256);
				strcat_s(buffer, "\n");
				strcat_s(sq.options, buffer);
			}
			t.v_questions.push_back(sq);
			strcpy_s(sq.options, "");
			cout << "1. ���������� 0. ���������\n-> ";
			cin >> a;
		}
		v_test.push_back(t);
		saveToFile(v_test.size() - 1, t.path);
	}
	void changeTest(int ind) { //<-- ������� ��������� �����
		if (ind < 0 || ind >= v_test.size()) {
			cout << "������������ ����!" << endl;
			return;
		}
		for (bool a = true; a; ) { //<-- �������� ���� ��� ��������� �����
			sc;
			char buffer[1024] = "";
			cout << "1. �������� ��������� 2. �������� �������� 3. �������� ������ 4. �������� ������� 5. �������� �������� ������� 0. ���������\n-> ";
			short int choice;
			cin >> choice;
			switch (choice) {
			case 1: { //<-- ��������� ���������
				cout << "������� ���������: " << v_test[ind].section << endl;
				cout << "������� ����� �������� ���������: ";
				cin.ignore();
				cin.getline(buffer, 256);
				strcat_s(buffer, "\n");
				strcpy_s(v_test[ind].section, buffer);
				break;
			}
			case 2: { //<-- ��������� ��������
				cout << "������� ��������: " << v_test[ind].name << endl;
				cout << "������� ����� ��������: ";
				cin.ignore();
				cin.getline(buffer, 256);
				strcat_s(buffer, "\n");
				strcpy_s(v_test[ind].name, buffer);
				break;
			}
			case 3: { //<-- ��������� �������
				cout << "������� ������: " << v_test[ind].answers << endl;
				cout << "������� ����� ������ (����� ������, � ����� ��������� ��������): ";
				cin.ignore();
				cin.getline(buffer, 256);
				strcat_s(buffer, "\n");
				strcpy_s(v_test[ind].answers, buffer);
				break;
			}
			case 4: { //<-- ��������� ��������
				cout << "������� �������: ";
				for (int i = 0; i < v_test[ind].v_questions.size(); i++) {
					cout << "������ ����� " << i << ": " << v_test[ind].v_questions[i].question;
				}
				cout << "������� ����� ������� ������� ������ ��������: ";
				int id;
				cin >> id;
				if (id < 0 || id >= v_test[ind].v_questions.size()) {
					cout << "�� ����� ������������ �����!";
					break;
				}
				cout << "������� ����� ������: ";
				cin.ignore();
				cin.getline(buffer, 256);
				strcat_s(buffer, "\n");
				strcpy_s(v_test[ind].v_questions[id].question, buffer);
				break;
			}
			case 5: { //<-- ��������� ��������� �������
				cout << "������� �������� �������: ";
				for (int i = 0; i < v_test[ind].v_questions.size(); i++) {
					cout << "�������� ��� ������� ����� " << i << ":\n " << v_test[ind].v_questions[i].options;
				}
				cout << "������� ����� �������, �������� �������� �� ������ ��������: ";
				int id;
				cin >> id;
				cin.ignore();
				if (id < 0 || id >= v_test[ind].v_questions.size()) {
					cout << "�� ����� ������������ �����!";
					break;
				}
				char buffer_v[256] = "";
				strcpy_s(buffer, "");
				for (int i = 0; i < 3; i++) {
					cout << "������� " << i << " ������� ������: ";
					cin.getline(buffer_v, 256);
					strcat_s(buffer_v, "\n");
					strcat_s(buffer, buffer_v);
				}
				strcpy_s(v_test[ind].v_questions[id].options, buffer);
				break;
			}
			case 0: { //<-- ����� �� ���������
				cout << "�� ������������� ������ ��������� � ������� ���������? (0. �� 1. ���): ";
				cin >> a;
				saveToFile(ind, v_test[ind].path, false); //<-- ���������� ��������� � ����
				sp;
				sc;
				break;
			}
			default: {
				cout << "������������ ����!" << endl;
				break;
			}
			}
		}
	}
	void saveTests() { //<-- ������� ���������� ����� � �����/��������� ������
		FILE* f;
		fopen_s(&f, "tests.txt", "w+");
		for (int i = 0; i < v_test.size(); i++) {
			char buffer[_MAX_PATH] = "";
			strcpy_s(buffer, v_test[i].path);
			strcat_s(buffer, "\n");
			fwrite(buffer, strlen(buffer), 1, f);
		}
		fclose(f);
	}
};
