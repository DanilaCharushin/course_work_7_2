#include "pch.h"
#include "List.h"
#include "Complex.h"
#include "Matrix.h"
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <ctime>
#include <typeinfo>

using namespace std;

typedef Matrix CurrentType;
const int WIDTH = 10;

enum COLOR { 
	BLACK = 0, 
	GREY = 8, 
	BLUE = 9, 
	GREEN = 10, 
	TURQUOISE = 11, 
	RED = 12, 
	PINK = 13, 
	YELLOW = 14, 
	WHITE = 15,  
	STANDART = 7 
};

template<int text_color = 7>
ostream& color(ostream &text)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text_color); //(WORD)((bg << 4) | txt)
	return text;
}

void print_main_menu()
{
	cout << color<YELLOW> << "(7.2)" << color << "D-linked list in bin file\n";
	cout << "------------------------------\n";
	cout << "-------------" << color<YELLOW> << "MENU" << color << "-------------\n";
	cout << color<GREEN> << "---INSERT:" << endl;
	cout << setw(WIDTH) << color << left << "rand" << color<PINK> << right << "(0)" << endl;
	cout << setw(WIDTH) << color << left << "first" << color<PINK> << right << "(1)" << endl;
	cout << setw(WIDTH) << color << left << "last" << color<PINK> << right << "(2)" << endl;
	cout << setw(WIDTH) << color << left << "at" << color<PINK> << right << "(3)" << endl;
	cout << color<GREEN> << "---REMOVE:" << endl;
	cout << setw(WIDTH) << color << left << "first" << color<PINK> << right << "(4)" << endl;
	cout << setw(WIDTH) << color << left << "last" << color<PINK> << right << "(5)" << endl;
	cout << setw(WIDTH) << color << left << "at" << color<PINK> << right << "(6)" << endl;
	cout << color<GREEN> << "---GET:" << endl;
	cout << setw(WIDTH) << color << left << "first" << color<PINK> << right << "(7)" << endl;
	cout << setw(WIDTH) << color << left << "last" << color<PINK> << right << "(8)" << endl;
	cout << setw(WIDTH) << color << left << "at" << color<PINK> << right << "(9)" << endl;
	cout << color<GREEN> << "---SET:" << endl;
	cout << setw(WIDTH) << color << left << "first" << color<PINK> << right << "(10)" << endl;
	cout << setw(WIDTH) << color << left << "last" << color<PINK> << right << "(11)" << endl;
	cout << setw(WIDTH) << color << left << "at" << color<PINK> << right << "(12)" << endl;
	cout << color<GREEN> << "---PRINT:" << endl;
	cout << setw(WIDTH) << color << left << "list" << color<PINK> << right << "(13)" << endl;
	cout << setw(WIDTH) << color << left << "info" << color<PINK> << right << "(14)" << endl;
	cout << setw(WIDTH) << color << left << "size" << color<PINK> << right << "(15)" << endl;
	cout << color<GREEN> << "---OTHER:" << endl;
	cout << setw(WIDTH) << color << left << "sort" << color<PINK> << right << "(16)" << endl;
	cout << setw(WIDTH) << color << left << "is empty" << color<PINK> << right << "(17)" << endl;
	cout << setw(WIDTH) << color << left << "clear" << color<PINK> << right << "(18)" << endl;
	cout << setw(WIDTH) << color << left << "iterator" << color<PINK> << right << "(19)" << endl;
	cout << setw(WIDTH) << left << color<GREEN> << "---EXIT" << color<PINK> << right << "(~)\n" << color;
	cout << "------------------------------\n";
	cout << "------------------------------\n               ";
}

void print_iterator_menu()
{
	cout << "------------------------------\n";
	cout << "------------" << color<YELLOW> << "ITERATOR" << color << "------------\n";
	cout << setw(WIDTH) << left << "---TO HEAD ";
	cout << color<TURQUOISE> << right << "(1)\n" << color;
	cout << setw(WIDTH) << left << "---TO TAIL ";
	cout << color<TURQUOISE> << right << "(2)\n" << color;
	cout << setw(WIDTH) << left << "---TO NEXT ";
	cout << color<TURQUOISE> << right << "(3)\n" << color;
	cout << setw(WIDTH) << left << "---TO PREV ";
	cout << color<TURQUOISE> << right << "(4)\n" << color;
	cout << setw(WIDTH) << left << "---SET DATA ";
	cout << color<TURQUOISE> << right << "(5)\n" << color;
	cout << setw(WIDTH) << left << "---PRINT INFO ";
	cout << color<TURQUOISE> << right << "(6)\n" << color;
	cout << setw(WIDTH) << left << "---MENU ";
	cout << color<TURQUOISE> << right << "(~)\n" << color;
	cout << "------------------------------\n";
	cout << "------------------------------\n             ";
}
void wait()
{
	getchar();
	getchar();
}

int main()
{
	//размер окна консоли
	system("mode con cols=80 lines=80");
	srand(time(0));

	String file_name = typeid(CurrentType).name(); 
	file_name += ".bin";
	List<CurrentType> list(file_name);
	List<CurrentType>::Iterator iterator(&list);

	bool running = true;
	while (running)
	{
		system("cls");
		print_main_menu();
		int key;
		cin >> key;
		switch (key)
		{
			case 0:
			{
				list.insert_last(CurrentType(rand() % 100));
				list.insert_last(CurrentType(rand() % 100));
				list.insert_last(CurrentType(rand() % 100));
				list.insert_last(CurrentType(rand() % 100));
				list.insert_last(CurrentType(rand() % 100));
				list.insert_last(CurrentType(rand() % 100));
				list.insert_last(CurrentType(rand() % 100));
				list.insert_last(CurrentType(rand() % 100));
				list.insert_last(CurrentType(rand() % 100));
				list.insert_last(CurrentType(rand() % 100));
				break;
			}
			case 1:
			{	
				CurrentType data;
				cout << "Data: ";
				cin >> data;
				list.insert_first(data);
				break;
			}
			case 2:
			{
				CurrentType data;
				cout << "Data: ";
				cin >> data;
				list.insert_last(data);
				break;
			}
			case 3:
			{
				CurrentType data;
				int index;
				cout << "Index, data: ";
				cin >> index >> data;
				try {
					list.insert_at(index, data);
				}
				catch (const range_error &ex) {
					cout << color<RED> << ex.what() << color << endl;
					wait();
				}
				break;
			}
			case 4:
			{
				try {
					list.remove_first();
				}
				catch (const exception &ex) {
					cout << color<RED> << ex.what() << color << endl;
					wait();
				}
				break;
			}
			case 5:
			{
				try {
					list.remove_last();
				}
				catch (const exception &ex) {
					cout << color<RED> << ex.what() << color << endl;
					wait();
				}
				break;
			}
			case 6:
			{
				int index;
				cout << "Index: ";
				cin >> index;
				try {
					list.remove_at(index);
				}
				catch (const exception &ex) {
					cout << color<RED> << ex.what() << color << endl;
					wait();
				}
				break;
			}
			case 7:
			{
				try {
					cout << "First: " << list.get_first() << endl;
				}
				catch (const exception &ex) {
					cout << color<RED> << ex.what() << color << endl;
				}
				wait();
				break;
			}
			case 8:
			{
				try {
					cout << "Last: " << list.get_last() << endl;
				}
				catch (const exception &ex) {
					cout << color<RED> << ex.what() << color << endl;
				}
				wait();
				break;
			}
			case 9:
			{
				try {
					int index;
					cout << "Index: ";
					cin >> index;
					cout << "Data by index " << index << ": " << list.get_at(index) << endl;
				}
				catch (const exception &ex) {
					cout << color<RED> << ex.what() << color << endl;
				}
				wait();
				break;
			}
			case 10:
			{
				try {
					CurrentType data;
					cout << "Data: ";
					cin >> data;
					list.set_first(data);
				}
				catch (const exception &ex) {
					cout << color<RED> << ex.what() << color << endl;
					wait();
				}
				break;
			}
			case 11:
			{
				try {
					CurrentType data;
					cout << "Data: ";
					cin >> data;
					list.set_last(data);
				}
				catch (const exception &ex) {
					cout << color<RED> << ex.what() << color << endl;
					wait();
				}
				break;
			}
			case 12:
			{
				try {
					int index;
					CurrentType data;
					cout << "Index, data: ";
					cin >> index >> data;
					list.set_at(index, data);
				}
				catch (const exception &ex) {
					cout << color<RED> << ex.what() << color << endl;
					wait();
				}
				break;
			}
			case 13:
			{
				system("cls");
				cout << "--------------------------------\n";
				cout << "--------------" << color<YELLOW> << "LIST" << color << "--------------\n";
				try {
					list.print();
					cout << "------------------------------\n";
					cout << "------------------------------\n";
				}
				catch (const exception &ex) {
					cout << color<RED> << ex.what() << color << endl;
				}
				wait();
				break;
			}
			case 14:
			{
				system("cls");
				cout << "------------------------------\n";
				cout << "--------------" << color<YELLOW> << "INFO" << color << "--------------\n";
				try {
					list.print_info();
					cout << "------------------------------\n";
					cout << "------------------------------\n";
				}
				catch (const exception &ex) {
					cout << color<RED> << ex.what() << color << endl;
				}
				wait();
				break;
			}
			case 15:
			{
				cout << "List size: " << list.get_size() << endl;
				wait();
				break;
			}
			case 16:
			{
				try {
					list.sort();
				}
				catch (const exception &ex) {
					cout << color<RED> << ex.what() << color << endl;
					wait();
				}
				break;
			}
			case 17:
			{
				if (list.is_empty())
					cout << "List is empty\n";
				else
					cout << "List is not empty\n";
				wait();
				break;
			}
			case 18:
			{
				list.clear();
				break;
			}
			case 19:
			{
				bool running_iterator = true;
				while (running_iterator)
				{
					system("cls");
					cout << "------------------------------\n";
					cout << "-------------" << color<YELLOW> << "LIST" << color << "-------------\n";
					try {
						list.print();
						cout << "----------------------------\n";	
						cout << "----------------------------\n"; 
						try {
							cout << "Data by iterator: " << color<WHITE> << iterator.get_data() << color << endl;
						}
						catch (const exception &ex) {
							cout << color<RED> << ex.what() << color << endl;
						}
					}
					catch (const exception &ex) {
						cout << color<RED> << ex.what() << color << endl;
						wait();
						break;
					}
					print_iterator_menu();
					int c;
					cin >> c;

					switch (c)
					{
						case 1:
						{
							try {
								iterator.to_head();
							}
							catch (const range_error &ex) {
								cout << color<RED> << ex.what() << color << endl;
								wait();
							}
							catch (const invalid_argument &ex) {
								cout << color<RED> << ex.what() << color << endl;
								wait();
								running_iterator = false;
							}
							break;
						}
						case 2:
						{
							try {
								iterator.to_tail();
							}
							catch (const range_error &ex) {
								cout << color<RED> << ex.what() << color << endl;
								wait();
							}
							catch (const invalid_argument &ex) {
								cout << color<RED> << ex.what() << color << endl;
								wait();
								running_iterator = false;
							}
							break;
						}
						case 3:
						{
							try {
								iterator++;
							}
							catch (const range_error &ex) {
								cout << color<RED> << ex.what() << color << endl;
								wait();
							}
							catch (const invalid_argument &ex) {
								cout << color<RED> << ex.what() << color << endl;
								wait();
								running_iterator = false;
							}
							break;
						}
						case 4:
						{
							try {
								iterator--;
							}
							catch (const range_error &ex) {
								cout << color<RED> << ex.what() << color << endl;
								wait();
							}
							catch (const invalid_argument &ex) {
								cout << color<RED> << ex.what() << color << endl;
								wait();
								running_iterator = false;
							}
							break;
						}
						case 5:
						{
							try {
								CurrentType data;
								cout << "Data: ";
								cin >> data;
								iterator.set_data(data);
							}
							catch (const invalid_argument &ex) {
								cout << color<RED> << ex.what() << color << endl;
								wait();
								running_iterator = false;
							}
							break;
						}
						case 6:
						{
							iterator.print_info();
							wait();
							break;
						}
						default:
							running_iterator = false;
							break;
					}
				}
				break;
			}
			default:  
				running = false;  
				break;
		}
	}

	return 0;
}
