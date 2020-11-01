#include "stdafx.h"
#include "Test.h"
#include "Encryptor.h"
#include "People.h"

using namespace std;

int main() {
	setlocale(LC_ALL, "rus");

	//<-- ������������� �������� ������� -->//
	Admin adm;
	Student stud;
	Tests ts;

	//<-- ��������� ���������� � ��������� � ������ -->//
	_mkdir("tests/");
	stud.loadFromFile("students.txt");
	ts.loadTests("tests.txt");

	//<-- �������� ���� ��������� -->//
	for (bool a = true; a; ) {
		short int choice;
		cout << "����� ���������� � ������� ������������!\n��� �� ��������: 1. ����������� 2. ������������� 0. �����\n-> ";
		cin >> choice;
		switch (choice) {
			//<-- �������� ����� ��� �������� -->//
		case 1: {
			if (stud.authentication() == true) { //<-- ��������������� ��������
				for (bool b = true; b; ) { //<-- ���� ���� �������� ��� �������� �����
					sc;
					stud.greeting();//<-- ����������� ��������
					cout << "��������� ��������: 1. ����������� ������ ���������� 2. ����� ������������ 0. �����\n-> ";
					short int choice2;
					cin >> choice2;
					switch (choice2) {
					case 1: {
						stud.showInfo(g_cur_student); //<-- ����� ������ ���������� ��������
						sp;
						sc;
						break;
					}
					case 2: {
						ts.showTest(false); //<-- ����� ������ ��������� ������
						cout << "������� ����� �����, ������� ������ �����: ";
						int test_num;
						cin >> test_num;
						int mark = ts.beginTest(test_num); //<-- ������ ������� ����� �����, ���� �������� ������ ��������, ������� ���������� -1
						sp;
						sc;
						if (mark != -1) {
							stud.addMark(g_cur_student, mark); //<-- ���������� ������ ��������
						}
						break;
					}
					case 0: {
						b = false;
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
			else { //<-- ���������� ��������� ����� ���� ��������������� ������ ���������
				sp;
				sc;
				break;
			}
			break;
		}
				//<-- �������� ����� ��� ������ -->//
		case 2: {
			if (adm.authentication() == true) { //<-- ��������������� ������
				for (bool c = true; c; ) { //<-- ���� ���� ������ ��� �������� �����
					sc;
					adm.greeting(); //<-- ����������� ������
					cout << "��������� ��������: 1. ������ � ������������ 2. ������ � ������� 3. �������� ����� 4. �������� ������ 0. �����\n-> ";
					short int choice3;
					cin >> choice3;
					switch (choice3) {
					case 1: { //<-- ������ � ������������
						for (bool c_1 = true; c_1; ) { //<-- ���� ��� ������ � ����������
							cout << "1. ���������� ���������� ����������� 2. �������� �������������� 3. �������� ���������� ������������ 4. ������� ������������ 0. �����\n-> ";
							short int choice3_1;
							cin >> choice3_1;
							switch (choice3_1) {
							case 1: { //<-- ����� ���������� � ���������
								adm.showStudentInfo(stud); //<-- ����� ������ � ���������
								sp;
								sc;
								break;
							}
							case 2: { //<-- ���������� ��������
								stud.registration(); //<-- ����������� ������ ��������
								sc;
								break;
							}
							case 3: { //<-- ��������� ����������
								adm.changeStudentInfo(stud); //<-- ��������� ������ ������������� ��������
								break;
							}
							case 4: { //<-- �������� ��������
								int ind;
								cout << "������� ����� ������������: ";
								cin >> ind;
								stud.deleteStud(ind); //<-- ������� �������� ��������
								sc;
								break;
							}
							case 0: { //<-- �����
								c_1 = false;
								sc;
								break;
							}
							default: {
								cout << "������������ ����!" << endl;
								sp;
								sc;
								break;
							}
							}
						}
						break;
					}
					case 2: { //<-- ������ � �������
						for (bool c_2 = true; c; ) { //<-- ���� ��� ������ � �������
							sc;
							cout << "1. ����������� ����� 2. �������� ���� 3. ������� ���� 4. �������� ���� 0. �����\n-> ";
							short int choice3_1;
							cin >> choice3_1;
							switch (choice3_1) {
							case 1: {
								ts.showTest(true); //<-- ������� ������ ������ � ��������
								sp;
								break;
							}
							case 2: { //<-- ���������� ����� 
								ts.addTest();
								break;
							}
							case 3: { //<-- �������� �����
								ts.showTest(false); //<-- ������� ������ ������ � ������� �����������
								cout << "������� ����� ����� ������� ������ �������: ";
								int ind;
								cin >> ind;
								ts.removeTest(ind); //<-- ������� �������� �����
								sp;
								sc;
								break;
							}
							case 4: { //<-- ��������� �����
								ts.showTest(false); //<-- ������� ������ ������ � ������� �����������
								cout << "������� ����� ����� ������� ������ ��������: ";
								int ind;
								cin >> ind;
								ts.changeTest(ind); //<-- ������� ���� ��������� � �����
								break;
							}
							case 0: { //<-- �����
								c = false;
								sc;
								break;
							}
							default: {
								cout << "������������ ����!" << endl;
								sp;
								sc;
								break;
							}
							}
						}
						break;
					}
					case 3: { //<-- ��������� ������
						char buffer[256] = "";
						cout << "������� ����� �����: ";
						cin.ignore();
						cin.getline(buffer, 256);
						adm.changeUserName(buffer);
						break;
					}
					case 4: { //<-- ��������� ������
						char buffer[256] = "";
						cout << "������� ����� ������: ";
						cin.ignore();
						cin.getline(buffer, 256);
						adm.changePassword(buffer);
						break;
					}
					case 0: { //<-- �����
						c = false;
						sc;
						break;
					}
					default: {
						cout << "������������ ����!" << endl;
						sp;
						sc;
						break;
					}
					}
				}
			}
			else {
				sp;
				sc;
				break;
			}
			break;
		}
		case 0: //<-- ����� �� ���������
			stud.end(); //<-- ���������� ������ � ��������� � ����
			ts.saveTests(); //<-- ���������� ����� � ������ � ����
			a = false;
			break;
		default:
			cout << "������������ ����!" << endl;
			sp;
			sc;
			break;
		}
	}

	return 0;
}