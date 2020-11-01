#include "Encryptor.h"
#include "stdafx.h"

using namespace std;

char g_seed[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890]"; //<-- Глобальная переменная сида шифровки
//<-- Реализация функций шифровки/дешифровки -->//
char* encrypt(const char* str, bool mode) {
	char seed[64] = "";
	strcpy_s(seed, g_seed);
	char buffer[256] = "";
	strcpy_s(buffer, str);
	if (mode == true) {
		for (int i = 0; buffer[i] != '\n'; i++) {
			if (buffer[i] != ' ') {
				int temp = 0;
				for (int j = 0; seed[j] != buffer[i]; j++, temp++);
				buffer[i] = seed[temp + 1];
			}
		}
	}
	else {
		for (int i = 0; buffer[i] != '\0'; i++) {
			if (buffer[i] != ' ' && buffer[i] != '\n') {
				int temp = 0;
				for (int j = 0; seed[j] != buffer[i]; j++, temp++);
				buffer[i] = seed[temp + 1];
			}
		}
	}
	return buffer;
}
char* decrypt(const char* str, bool mode) {
	char seed[65] = "";
	strcpy_s(seed, g_seed);
	char buffer[256] = "";
	strcpy_s(buffer, str);
	if (mode == true) {
		for (int i = 0; buffer[i] != '\n'; i++) {
			if (buffer[i] != ' ') {
				int temp = 0;
				for (int j = 0; seed[j] != buffer[i]; j++, temp++);
				buffer[i] = seed[temp - 1];
			}
		}
	}
	else {
		for (int i = 0; buffer[i] != '\0'; i++) {
			if (buffer[i] != ' ' && buffer[i] != '\n') {
				int temp = 0;
				for (int j = 0; seed[j] != buffer[i]; j++, temp++);
				buffer[i] = seed[temp + 1];
			}
		}
	}
	return buffer;
}