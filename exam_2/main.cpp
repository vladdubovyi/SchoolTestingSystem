#include "stdafx.h"
#include "Test.h"
#include "Encryptor.h"
#include "People.h"

using namespace std;

int main() {
	setlocale(LC_ALL, "rus");

	//<-- Иниацилизация обьектов классов -->//
	Admin adm;
	Student stud;
	Tests ts;

	//<-- Подгрузка информации о студентах и тестах -->//
	_mkdir("tests/");
	stud.loadFromFile("students.txt");
	ts.loadTests("tests.txt");

	//<-- Основной цикл программы -->//
	for (bool a = true; a; ) {
		short int choice;
		cout << "Добро пожаловать в систему тестирования!\nКем вы явяетесь: 1. Тестируемый 2. Администратор 0. Выйти\n-> ";
		cin >> choice;
		switch (choice) {
			//<-- Операция входа для студента -->//
		case 1: {
			if (stud.authentication() == true) { //<-- Аунтентификация студента
				for (bool b = true; b; ) { //<-- Цикл меню студента при успешном входе
					sc;
					stud.greeting();//<-- Приветствие студента
					cout << "Доступные действия: 1. Просмотреть личную информацию 2. Сдать тестирование 0. Выйти\n-> ";
					short int choice2;
					cin >> choice2;
					switch (choice2) {
					case 1: {
						stud.showInfo(g_cur_student); //<-- Вывод личной информации студента
						sp;
						sc;
						break;
					}
					case 2: {
						ts.showTest(false); //<-- Вывод списка доступных тестов
						cout << "Введите номер теста, который хотите сдать: ";
						int test_num;
						cin >> test_num;
						int mark = ts.beginTest(test_num); //<-- Запуск функции сдачи теста, если указаный индекс неверный, функция возвращает -1
						sp;
						sc;
						if (mark != -1) {
							stud.addMark(g_cur_student, mark); //<-- Добавление оценки студенту
						}
						break;
					}
					case 0: {
						b = false;
						sc;
						break;
					}
					default: {
						cout << "Неправильный ввод!" << endl;
						break;
					}
					}
				}
			}
			else { //<-- Перезапуск основного цикла если аунтентификация прошла неуспешно
				sp;
				sc;
				break;
			}
			break;
		}
				//<-- Операция входа для админа -->//
		case 2: {
			if (adm.authentication() == true) { //<-- Аунтентификация админа
				for (bool c = true; c; ) { //<-- Цикл меню админа при успешном входе
					sc;
					adm.greeting(); //<-- Приветствие админа
					cout << "Доступные действия: 1. Работа с тестируемыми 2. Работа с тестами 3. Изменить логин 4. Изменить пароль 0. Выйти\n-> ";
					short int choice3;
					cin >> choice3;
					switch (choice3) {
					case 1: { //<-- Работа с тестируемыми
						for (bool c_1 = true; c_1; ) { //<-- Цикл для роботы с студентами
							cout << "1. Посмотреть информацию тестируемых 2. Добавить тестируемового 3. Изменить информацию тестируемого 4. Удалить тестируемого 0. Выйти\n-> ";
							short int choice3_1;
							cin >> choice3_1;
							switch (choice3_1) {
							case 1: { //<-- Вывод информации о студентах
								adm.showStudentInfo(stud); //<-- Вывод данных о студентах
								sp;
								sc;
								break;
							}
							case 2: { //<-- Добавление студента
								stud.registration(); //<-- Регистрация нового студента
								sc;
								break;
							}
							case 3: { //<-- Изменение информации
								adm.changeStudentInfo(stud); //<-- Изменение данных определенного студента
								break;
							}
							case 4: { //<-- Удаление студента
								int ind;
								cout << "Введите номер тестируемого: ";
								cin >> ind;
								stud.deleteStud(ind); //<-- Функция удаления студента
								sc;
								break;
							}
							case 0: { //<-- Выход
								c_1 = false;
								sc;
								break;
							}
							default: {
								cout << "Неправильный ввод!" << endl;
								sp;
								sc;
								break;
							}
							}
						}
						break;
					}
					case 2: { //<-- Работа с тестами
						for (bool c_2 = true; c; ) { //<-- Цикл для работы с тестами
							sc;
							cout << "1. Просмотреть тесты 2. Добавить тест 3. Удалить тест 4. Изменить тест 0. Выйти\n-> ";
							short int choice3_1;
							cin >> choice3_1;
							switch (choice3_1) {
							case 1: {
								ts.showTest(true); //<-- Функция вывода тестов с ответами
								sp;
								break;
							}
							case 2: { //<-- Добавление теста 
								ts.addTest();
								break;
							}
							case 3: { //<-- Удаление теста
								ts.showTest(false); //<-- Функция вывода тестов с краткой информацией
								cout << "Введите номер теста который хотите удалить: ";
								int ind;
								cin >> ind;
								ts.removeTest(ind); //<-- Функция удаления теста
								sp;
								sc;
								break;
							}
							case 4: { //<-- Изменение теста
								ts.showTest(false); //<-- Функция вывода тестов с краткой информацией
								cout << "Введите номер теста который хотите изменить: ";
								int ind;
								cin >> ind;
								ts.changeTest(ind); //<-- Функция меню изменений в тесте
								break;
							}
							case 0: { //<-- Выход
								c = false;
								sc;
								break;
							}
							default: {
								cout << "Неправильный ввод!" << endl;
								sp;
								sc;
								break;
							}
							}
						}
						break;
					}
					case 3: { //<-- Изменение логина
						char buffer[256] = "";
						cout << "Введите новый логин: ";
						cin.ignore();
						cin.getline(buffer, 256);
						adm.changeUserName(buffer);
						break;
					}
					case 4: { //<-- Изменение пароля
						char buffer[256] = "";
						cout << "Введите новый пароль: ";
						cin.ignore();
						cin.getline(buffer, 256);
						adm.changePassword(buffer);
						break;
					}
					case 0: { //<-- Выход
						c = false;
						sc;
						break;
					}
					default: {
						cout << "Неправильный ввод!" << endl;
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
		case 0: //<-- Выход из программы
			stud.end(); //<-- Сохранение данных о студентах в файл
			ts.saveTests(); //<-- Сохранение путей к тестам в файл
			a = false;
			break;
		default:
			cout << "Неправильный ввод!" << endl;
			sp;
			sc;
			break;
		}
	}

	return 0;
}