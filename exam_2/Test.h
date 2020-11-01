#pragma once
#include "stdafx.h"
#include "Encryptor.h" 

using namespace std;


//<-- Обьявление класса для тестов -->//
class Tests {
private:
	struct Test { //<-- Обьявление основной структуры теста
		struct s_questions { //<-- Обьявление структуры вопросов
			char question[512];
			char options[1024];

			s_questions() : question(""), options("") {

			}
		};
		vector<s_questions> v_questions; //<-- Обьявление вектора вопросов
		char path[_MAX_PATH];
		char section[256];
		char name[256];
		char answers[256];
		int marks;
	};
	vector<Test> v_test; //<-- Обьявление основного вектора тестов

	void loadQuestions(const char* fileName, int len, Test& t) { //<-- Функция для подгрузки вопросов из файла
		FILE* f;
		fopen_s(&f, fileName, "r");
		Test::s_questions sq;
		char question[512] = "";
		char option1[256] = "";
		char option2[256] = "";
		char option3[256] = "";
		char buffer[1024] = "";

		fseek(f, (sizeof(char)*(strlen(question) + strlen(option1) + strlen(option2) + strlen(option3)) + len), 0); //<-- Установка указателя в нужное место

		fgets(question, sizeof(question), f);
		fgets(option1, sizeof(option1), f);
		fgets(option2, sizeof(option2), f);
		fgets(option3, sizeof(option3), f);

		strcpy_s(sq.question, decrypt(question));
		strcpy_s(buffer, decrypt(option1));
		strcat_s(buffer, decrypt(option2));
		strcat_s(buffer, decrypt(option3));
		strcpy_s(sq.options, buffer);

		t.v_questions.push_back(sq); //<-- Пуш заполненного вектора вопросов в временный тест

		if (_filelength(_fileno(f)) > (sizeof(char)*(strlen(question) + strlen(option1) + strlen(option2) + strlen(option3)) + len + 14)) {
			fclose(f);
			loadQuestions(fileName, (sizeof(char)*(strlen(question) + strlen(option1) + strlen(option2) + strlen(option3)) + len + 4), t);
		}
		fclose(f);
	}

	void loadFromFile(const char* fileName) { //<-- Функция подгрузки теста из файла
		FILE* f;
		fopen_s(&f, fileName, "r");
		if (_filelength(_fileno(f)) == 0) { //<-- Если файл пустой, ничего не подгружаем
			fclose(f);
			return;
		}

		char buffer[2048] = "";
		char section[256] = "";
		char name[256] = "";
		char answers[256] = "";
		Test t;
		//<-- Подгрузка раздела, название и ответов -->//
		fgets(section, sizeof(section), f);
		fgets(name, sizeof(name), f);
		fgets(answers, sizeof(answers), f);
		strcpy_s(t.section, decrypt(section));
		strcpy_s(t.name, decrypt(name));
		strcpy_s(t.answers, decrypt(answers));
		strcpy_s(t.path, fileName);

		fclose(f);
		loadQuestions(fileName, (sizeof(char)*(strlen(name) + strlen(section) + strlen(answers))) + 3, t); //<-- Подгрузка вопросов

		v_test.push_back(t); //<-- Пуш заполненного теста в вектор

		fclose(f);
	}
public:
	void loadTests(const char* fileName, int len = 0) { //<-- Основная функция подгрузки теста из файла
		FILE* f;
		fopen_s(&f, fileName, "a"); //<-- Если файл не существует, создаем его
		fclose(f);
		fopen_s(&f, fileName, "r");
		if (_filelength(_fileno(f)) == 0) {
			fclose(f);
			return;
		}

		char testFileName[256] = "";
		char buffer[256] = "";

		fseek(f, (sizeof(char)*(strlen(testFileName)) + len), 0); //<-- Установка указателя на следующий файл в последовательности

		fgets(testFileName, sizeof(testFileName), f);
		testFileName[strlen(testFileName) - 1] = '\0';

		loadFromFile(testFileName); //<-- Подгрузка теста из файла

		if (_filelength(_fileno(f)) > (sizeof(char)*(strlen(testFileName)) + len + 14)) { //<-- Если не достигнут конец файла, вызываем функцию рекурсивно
			fclose(f);
			loadTests(fileName, (sizeof(char)*(strlen(testFileName)) + len) + 2);
		}
		fclose(f);
	}
	void showTest(bool mode) { //<-- Функция вывода информации по тестам
		if (mode == true) { //<-- Вывод для админа
			for (int i = 0; i < v_test.size(); i++) {
				cout << "Номер: " << i << " Раздел: " << v_test[i].section << "Название: " << v_test[i].name << "Вопросы:\n";
				for (int j = 0; j < v_test[i].v_questions.size(); j++) {
					cout << v_test[i].v_questions[j].question << v_test[i].v_questions[j].options;
				}
				cout << "Ответы: " << v_test[i].answers;
			}
		}
		else { //<-- Вывод для студентов
			for (int i = 0; i < v_test.size(); i++) {
				cout << "Раздел: " << v_test[i].section << "Название: " << v_test[i].name << "Номер: " << i << endl;
			}
		}
	}
	int beginTest(int ind) { //<-- Функция начала теста для студентов
		if (ind < 0 || ind >= v_test.size()) { //<-- Проверка на правильность индекса теста
			cout << "Неправильный ввод!" << endl;
			return -1;
		}
		cout << "Раздел: " << v_test[ind].section << "Название: " << v_test[ind].name << "Номер: " << ind << endl;

		vector<char> correctAnswer; //<-- Иниацилизация вектора правильных ответов на тест
		double sum_marks = 0;
		double answerPrice = 12.0 / v_test[ind].v_questions.size();
		int corrAnsNum = 0;
		char studentAnswer;

		for (int i = 0; v_test[ind].answers[i] != '\n'; i++) { //<-- Цикл заполнения вектора правильными ответами
			if (v_test[ind].answers[i] != ' ') correctAnswer.push_back(v_test[ind].answers[i]);
		}

		for (int i = 0; i < v_test[ind].v_questions.size(); i++) { //<-- Основной цикл для тестирования
			cout << v_test[ind].v_questions[i].question << v_test[ind].v_questions[i].options;
			cout << "Введите ответ: ";
			cin >> studentAnswer;
			if (studentAnswer == correctAnswer[i]) { //<-- Проверка на правильность ответа, добавление баллов
				sum_marks += answerPrice;
				corrAnsNum++;
			}
			sc;
		}
		sum_marks = ceil(sum_marks); //<-- Округление баллов в сторону студента
		cout << "Ваша оценка: " << sum_marks << " Правильных ответов: " << corrAnsNum << " Процент правильных ответов: " << (double)corrAnsNum / (double)v_test[ind].v_questions.size() * 100 << "%" << endl;
		sp;
		return sum_marks;
	}
	void removeTest(int ind) { //<-- Функция удаления туста
		if (ind < 0 || ind >= v_test.size()) {
			cout << "Неправильный ввод!" << endl;
			return;
		}
		if (remove(v_test[ind].path) == 0) {
			cout << "Удаление прошло успешно!";
			v_test.erase(v_test.begin() + ind);
		}
		else cout << "Что-то пошло не так..";
	}
	void saveToFile(int ind, const char* fileName, bool mode = true) { //<-- Функция сохранения измененного/добавленого теста
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
		if (mode == true) { //<-- Если тест добавлен а не изменен, записываем его путь в файл с путями к тестам
			fopen_s(&f, "tests.txt", "a");
			strcpy_s(buffer, fileName);
			strcat_s(buffer, "\n");
			fwrite(buffer, strlen(buffer), 1, f);
			fclose(f);
		}
	}
	void addTest() { //<-- Функция добавления теста
		Test t;
		Test::s_questions sq;
		char buffer[1024] = "";

		cout << "Введите категорию теста(Физика, математика): ";
		cin.ignore();
		cin.getline(buffer, 256);
		strcat_s(buffer, "\n");
		strcpy_s(t.section, buffer);

		cout << "Введите название теста(Математический Анализ, Механика): ";
		cin.getline(buffer, 256);
		strcat_s(buffer, "\n");
		strcpy_s(t.name, buffer);

		cout << "Введите правильные ответы(По порядку вопросов, через пробел): ";
		cin.getline(buffer, 256);
		strcat_s(buffer, "\n");
		strcpy_s(t.answers, buffer);

		cout << "Введите желаемое название теста(test.txt): ";
		char pathName[_MAX_PATH] = "";

		strcpy_s(buffer, "tests/");
		cin.getline(pathName, _MAX_PATH);
		strcat_s(buffer, pathName);
		strcpy_s(t.path, buffer);

		int cnt = 1;
		for (bool a = true; a; cnt++) {
			cout << "Введите " << cnt << " вопрос: ";
			if (cnt != 1) cin.ignore();
			cin.getline(buffer, 256);
			strcat_s(buffer, "\n");
			strcpy_s(sq.question, buffer);
			for (int j = 0; j < 3; j++) {
				cout << "Введите " << j << " вариант ответа(А E = mc^2): ";
				cin.getline(buffer, 256);
				strcat_s(buffer, "\n");
				strcat_s(sq.options, buffer);
			}
			t.v_questions.push_back(sq);
			strcpy_s(sq.options, "");
			cout << "1. Продолжить 0. Закончить\n-> ";
			cin >> a;
		}
		v_test.push_back(t);
		saveToFile(v_test.size() - 1, t.path);
	}
	void changeTest(int ind) { //<-- Функция изминения теста
		if (ind < 0 || ind >= v_test.size()) {
			cout << "Неправильный ввод!" << endl;
			return;
		}
		for (bool a = true; a; ) { //<-- Основной цикл для изминения теста
			sc;
			char buffer[1024] = "";
			cout << "1. Изменить категорию 2. Изменить название 3. Изменить ответы 4. Изменить вопросы 5. Изменить варианты ответов 0. Закончить\n-> ";
			short int choice;
			cin >> choice;
			switch (choice) {
			case 1: { //<-- Изменение категории
				cout << "Текущая категория: " << v_test[ind].section << endl;
				cout << "Введите новое название категории: ";
				cin.ignore();
				cin.getline(buffer, 256);
				strcat_s(buffer, "\n");
				strcpy_s(v_test[ind].section, buffer);
				break;
			}
			case 2: { //<-- Изменение названия
				cout << "Текущее название: " << v_test[ind].name << endl;
				cout << "Введите новое название: ";
				cin.ignore();
				cin.getline(buffer, 256);
				strcat_s(buffer, "\n");
				strcpy_s(v_test[ind].name, buffer);
				break;
			}
			case 3: { //<-- Изменения ответов
				cout << "Текущее ответы: " << v_test[ind].answers << endl;
				cout << "Введите новые ответы (Через пробел, в конце завершать пробелом): ";
				cin.ignore();
				cin.getline(buffer, 256);
				strcat_s(buffer, "\n");
				strcpy_s(v_test[ind].answers, buffer);
				break;
			}
			case 4: { //<-- Изменение вопросов
				cout << "Текущее вопросы: ";
				for (int i = 0; i < v_test[ind].v_questions.size(); i++) {
					cout << "Вопрос номер " << i << ": " << v_test[ind].v_questions[i].question;
				}
				cout << "Введите номер вопроса который хотите изменить: ";
				int id;
				cin >> id;
				if (id < 0 || id >= v_test[ind].v_questions.size()) {
					cout << "Вы ввели неправильный номер!";
					break;
				}
				cout << "Введите новый вопрос: ";
				cin.ignore();
				cin.getline(buffer, 256);
				strcat_s(buffer, "\n");
				strcpy_s(v_test[ind].v_questions[id].question, buffer);
				break;
			}
			case 5: { //<-- Изменение вариантов ответов
				cout << "Текущее варианты ответов: ";
				for (int i = 0; i < v_test[ind].v_questions.size(); i++) {
					cout << "Варианты для вопроса номер " << i << ":\n " << v_test[ind].v_questions[i].options;
				}
				cout << "Введите номер вопроса, варианты которого вы хотите изменить: ";
				int id;
				cin >> id;
				cin.ignore();
				if (id < 0 || id >= v_test[ind].v_questions.size()) {
					cout << "Вы ввели неправильный номер!";
					break;
				}
				char buffer_v[256] = "";
				strcpy_s(buffer, "");
				for (int i = 0; i < 3; i++) {
					cout << "Введите " << i << " вариант ответа: ";
					cin.getline(buffer_v, 256);
					strcat_s(buffer_v, "\n");
					strcat_s(buffer, buffer_v);
				}
				strcpy_s(v_test[ind].v_questions[id].options, buffer);
				break;
			}
			case 0: { //<-- Выход из программы
				cout << "Вы действительно хотите закончить и принять изменения? (0. Да 1. Нет): ";
				cin >> a;
				saveToFile(ind, v_test[ind].path, false); //<-- Сохранение изминения в файл
				sp;
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
	void saveTests() { //<-- Функция сохранения путей к новым/удаленным тестам
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
