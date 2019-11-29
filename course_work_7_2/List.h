#include "pch.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "String.h"

using namespace std;

#ifndef __LIST_H__
#define __LIST_H__

template <class Data = int> class List : public fstream 
{
private:
	class Header
	{
	public:
		int offset_head;
		int offset_tail;
		int list_size;
	};
	class Node
	{
	public:
		class Record
		{
		public:
			Data data;
			int offset_next;
			int offset_prev;
		};

		Record record;
		int offset;
		List<Data> * list;

		Node(List<Data> * list);

		Data get_data();
		int get_offset_next();
		int get_offset_prev();

		void set_data(Data data);
		void set_offset_next(int offset);
		void set_offset_prev(int offset);
		void set_offset(int offset);

		void save();
		void read_from(int offset);
	};
	
	Header header;
	String file_name;

	void purge(int offset);
	void trunc(int file_size);

	int get_offset_head();
	int get_offset_tail();
	
	void set_offset_head(int offset);
	void set_offset_tail(int offset);
	
	void inc_size();
	void dec_size();
	void zero_size();

	void save();

public:
	List(String file_name = "list.dat");
	~List();
	
	void insert_first(Data data = Data());
	void insert_last(Data data = Data());
	void insert_at(int index, Data data = Data());
	
	Data get_first();
	Data get_last();
	Data get_at(int index);
	
	void set_first(Data data = Data());
	void set_last(Data data = Data());
	void set_at(int index, Data data = Data());
	
	void remove_first();
	void remove_last();
	void remove_at(int index);
	
	void print();
	void print_info();
	void sort();
	int get_size();
	void clear();
	bool is_empty();

	class Iterator
	{
	private:
		List<Data> * list;
		int current_offset;
	
	public:
		Iterator(List<Data> * list = nullptr);
		bool is_inside();
		void operator++(int);
		void operator--(int);
		void to_head();
		void to_tail();
		void to_next();
		void to_prev();
		Data get_data();
		void set_data(Data data);
		void set_list(List<Data> * list = nullptr);
		void print_info();
	};

	friend class Iterator;
};
//assasdasdasdasd
//---------Node---------//
template <class Data> List<Data>::Node::Node(List<Data> * list) : list(list), offset(0)
{
	set_offset_next(0);
	set_offset_prev(0);
}
template <class Data> Data List<Data>::Node::get_data()
{
	return record.data;
}
template <class Data> int List<Data>::Node::get_offset_next()
{
	return record.offset_next;
}
template <class Data> int List<Data>::Node::get_offset_prev()
{
	return record.offset_prev;
}
template <class Data> void List<Data>::Node::set_data(Data data)
{
	this->record.data = data;
}
template <class Data> void List<Data>::Node::set_offset_next(int offset)
{
	record.offset_next = offset;
}
template <class Data> void List<Data>::Node::set_offset_prev(int offset)
{
	record.offset_prev = offset;
}
template <class Data> void List<Data>::Node::set_offset(int offset)
{
	this->offset = offset;
}
template <class Data> void List<Data>::Node::save()
{
	if (offset > 0)
	{
		list->seekp(offset, ios::beg);//установка указателя записи
		list->write(reinterpret_cast<char*>(&record), sizeof Record);
	}
	else
	{
		list->seekp(0, ios::end);
		offset = static_cast<int>(list->tellp());//текущее положение указателя записи
		list->write(reinterpret_cast<char*>(&record), sizeof Record);
	}
}
template <class Data> void List<Data>::Node::read_from(int offset)
{									//смещение от начала к концу
	this->offset = offset;
	list->seekg(offset, ios::beg);
	list->read(reinterpret_cast<char*>(&record), sizeof Record);
}

//---------List---------//
//private methods (inner)
template <class Data> void List<Data>::purge(int offset)//очистка
{
	seekp(0, ios::end);
	int moved_node_offset = static_cast<int>(tellg()) - sizeof Node::Record;

	if (offset == moved_node_offset)
	{
		trunc(offset);//обрезать
		return;
	}

	if (offset < sizeof(Header))
	{
		trunc(sizeof(Header));
		return;
	}

	Node moved_node(this);
	moved_node.read_from(moved_node_offset);
	moved_node.set_offset(offset);
	moved_node.save();

	int prev_offset = moved_node.get_offset_prev();
	if (prev_offset == 0)
		set_offset_head(offset);
	else
	{
		Node prev_node(this);
		prev_node.read_from(prev_offset);
		prev_node.set_offset_next(offset);
		prev_node.save();
	}

	int next_offset = moved_node.get_offset_next();
	if (next_offset == 0)
		set_offset_tail(offset);
	else
	{
		Node next_node(this);
		next_node.read_from(next_offset);
		next_node.set_offset_prev(offset);
		next_node.save();
	}
	save();
	trunc(moved_node_offset);
}
template <class Data> void List<Data>::trunc(int file_size)
{
	const int buffer_size = 1024l * 1024l;
	String temp_file_name = "$" + file_name;
	fstream tmp_stream(temp_file_name.to_char(), fstream::out | fstream::binary);

	seekg(0, ios::beg);

	char * buffer = new char[buffer_size];

	while (file_size > 0)
	{
		int bytes_to_read = buffer_size;
		if (file_size < bytes_to_read)
			bytes_to_read = file_size;

		read(buffer, bytes_to_read);
		tmp_stream.write(buffer, bytes_to_read);

		file_size -= bytes_to_read;
	}

	close();
	tmp_stream.close();

	remove(file_name.to_char());
	rename(temp_file_name.to_char(), file_name.to_char());

	open(file_name.to_char(), fstream::in | fstream::out | fstream::binary);

	delete buffer;
}
template <class Data> int List<Data>::get_offset_head()
{
	return header.offset_head;
}
template <class Data> int List<Data>::get_offset_tail()
{
	return header.offset_tail;
}
template <class Data> void List<Data>::set_offset_head(int offset)
{
	header.offset_head = offset;
}
template <class Data> void List<Data>::set_offset_tail(int offset)
{
	header.offset_tail = offset;
}
template <class Data> void List<Data>::inc_size()
{
	header.list_size++;
}
template <class Data> void List<Data>::dec_size()
{
	header.list_size;
}
template <class Data> void List<Data>::zero_size()
{
	header.list_size = 0;
}
template <class Data> void List<Data>::save()
{
	seekp(0, ios::beg);
	write(reinterpret_cast<char*>(&header), sizeof Header);
}

//basic methods
template <class Data> List<Data>::List(String file_name)
{
	this->file_name = file_name;

	open(file_name.to_char(), fstream::in | fstream::out | fstream::binary);

	if (!is_open())
	{
		open(file_name.to_char(), fstream::out);
		close();
		open(file_name.to_char(), fstream::in | fstream::out | fstream::binary);
	}

	if (is_open())
	{
		seekg(0, ios::end);
		int file_size = tellg();
		seekg(0, ios::beg);

		if (file_size == 0)
		{
			set_offset_head(0);
			set_offset_tail(0);
			zero_size();
			write(reinterpret_cast<char*>(&header), sizeof Header);
		}
		else
			read(reinterpret_cast<char*>(&header), sizeof Header);
	}
}
template <class Data> List<Data>::~List()
{
	close();
}

//public methods (UI)
template <class Data> void List<Data>::insert_first(Data data)
{
	Node new_node(this);
	new_node.set_data(data);
	new_node.save();

	inc_size();
	
	if (is_empty())
	{
		set_offset_head(new_node.offset);
		set_offset_tail(new_node.offset);
		save();
		return;
	}

	Node first_node(this);
	first_node.read_from(get_offset_head());
	first_node.set_offset_prev(new_node.offset);
	first_node.save();

	new_node.set_offset_next(first_node.offset);
	new_node.save();

	set_offset_head(new_node.offset);
	save();
}
template <class Data> void List<Data>::insert_last(Data data)
{
	Node new_node(this);
	new_node.set_data(data);
	new_node.save();

	inc_size();

	if (is_empty())
	{
		set_offset_head(new_node.offset);
		set_offset_tail(new_node.offset);
		save();
		return;
	}

	Node last_node(this);
	last_node.read_from(get_offset_tail());
	last_node.set_offset_next(new_node.offset);
	last_node.save();

	new_node.set_offset_prev(last_node.offset);
	new_node.save();

	set_offset_tail(new_node.offset);
	save();
}
template <class Data> void List<Data>::insert_at(int index, Data data)
{
	if (index < 0)
		throw range_error("Negative index");

	Node prev_node(this);
	Node next_node(this);
	int next_offset = get_offset_head();
	next_node.read_from(next_offset);

	for (int i = 0; i <= index && next_offset != 0; i++)
	{
		if (i == index)
			break;

		if (next_offset == 0)
			throw range_error("Index out of range");

		next_offset = next_node.get_offset_next();
		prev_node = next_node;

		if (next_offset != 0)
			next_node.read_from(next_offset);
	}

	if (prev_node.offset == 0)
	{
		insert_first(data);
		return;
	}

	if (next_offset == 0)
	{
		insert_last(data);
		return;
	}

	Node new_node(this);
	new_node.set_data(data);
	new_node.save();

	next_node.set_offset_prev(new_node.offset);
	next_node.save();

	prev_node.set_offset_next(new_node.offset);
	prev_node.save();

	new_node.set_offset_next(next_node.offset);
	new_node.set_offset_prev(prev_node.offset);
	new_node.save();
	inc_size();

	save();
}

template <class Data> Data List<Data>::get_first()
{
	if (is_empty())
		throw invalid_argument("List is empty");

	Node first_node(this);
	first_node.read_from(get_offset_head());
	return first_node.get_data();
}
template <class Data> Data List<Data>::get_last()
{
	if (is_empty())
		throw invalid_argument("List is empty");

	Node last_node(this);
	last_node.read_from(get_offset_tail());
	return last_node.get_data();
}
template <class Data> Data List<Data>::get_at(int index)
{
	if (is_empty())
		throw invalid_argument("List is empty");

	if (index < 0)
		throw range_error("Negative index");

	Node node(this);
	int next_offset = get_offset_head();
	for (int i = 0; i <= index && next_offset != 0; i++)
	{
		node.read_from(next_offset);
		if (i == index)
			return node.get_data();
		next_offset = node.get_offset_next();
	}
	throw range_error("Index out of range");
}

template <class Data> void List<Data>::set_first(Data data)
{
	if (is_empty())
		throw invalid_argument("List is empty");

	Node first_node(this);
	first_node.read_from(get_offset_head());
	first_node.set_data(data);
}
template <class Data> void List<Data>::set_last(Data data)
{
	if (is_empty())
		throw invalid_argument("List is empty");

	Node last_node(this);
	last_node.read_from(get_offset_tail());
	last_node.set_data(data);
}
template <class Data> void List<Data>::set_at(int index, Data data)
{
	if (is_empty())
		throw invalid_argument("List is empty");

	if (index < 0)
		throw range_error("Negative index");

	Node node(this);
	int next_offset = get_offset_head();
	for (int i = 0; i <= index && next_offset != 0; i++)
	{
		node.read_from(next_offset);
		if (i == index)
			return node.set_data(data);
		next_offset = node.get_offset_next();
	}
	throw range_error("Index out of range");
}

template <class Data> void List<Data>::remove_first()
{
	if (is_empty())
		throw invalid_argument("List is empty");

	Node first_node(this);
	first_node.read_from(get_offset_head());
	int next_offset = first_node.get_offset_next();

	dec_size();
	// Если удаляется единственный узел
	if (next_offset == 0)
	{
		set_offset_head(0);
		set_offset_tail(0);
		save();
		purge(first_node.offset);
		return;
	}

	Node next_node(this);
	next_node.read_from(next_offset);
	next_node.set_offset_prev(0);
	next_node.save();

	set_offset_head(next_offset);
	save();
	purge(first_node.offset);
}
template <class Data> void List<Data>::remove_last()
{
	if (is_empty())
		throw invalid_argument("List is empty");

	Node last_node(this);
	last_node.read_from(get_offset_tail());
	int prev_offset = last_node.get_offset_prev();

	dec_size();

	// Если удаляется единственный узел
	if (prev_offset == 0)
	{
		set_offset_head(0);
		set_offset_tail(0);
		save();
		purge(last_node.offset);
		return;
	}

	Node prev_node(this);
	prev_node.read_from(prev_offset);
	prev_node.set_offset_next(0);
	prev_node.save();

	set_offset_tail(prev_offset);
	save();
	purge(last_node.offset);
}
template <class Data> void List<Data>::remove_at(int index)
{
	if (is_empty())
		throw invalid_argument("List is empty");

	if (index < 0)
		throw range_error("Negative index");

	if (index == 0)
	{
		remove_first();
		return;
	}

	Node prev_node(this);
	Node current_node(this);
	int current_offset = get_offset_head();
	current_node.read_from(current_offset);

	for (int i = 0; i <= index && current_offset != 0; i++)
	{
		if (i == index)
			break;

		if (current_offset == 0)
			throw range_error("Index out of range");

		current_offset = current_node.get_offset_next();
		prev_node = current_node;

		if (current_offset != 0)
			current_node.read_from(current_offset);
	}

	if (current_offset == 0)
	{
		remove_last();
		return;
	}

	Node next_node(this);
	next_node.read_from(current_node.get_offset_next());
	next_node.set_offset_prev(prev_node.offset);
	next_node.save();

	prev_node.set_offset_next(next_node.offset);
	prev_node.save();

	purge(current_offset);
	dec_size();
	save();
}

template <class Data> void List<Data>::print()
{
	if (is_empty())
		throw invalid_argument("List is empty");

	Node node(this);
	int next_offset = get_offset_head();
	int i = 0;
	while (next_offset != 0)
	{
		node.read_from(next_offset);
		cout << i++ << ".\t" << node.get_data() << endl;
		next_offset = node.get_offset_next();
	}
}
template <class Data> void List<Data>::print_info()
{
	cout << "List size: " << get_size() << endl;
	cout << "Pointer put: " << tellp() << endl;
	cout << "Pointer get: " << tellg() << endl;
	cout << "Head offset: " << get_offset_head() << endl;
	cout << "Tail offset: " << get_offset_tail() << endl;
	int offset = tellg();
	seekg(0, ios::end);
	cout << "File size: " << static_cast<int>(tellg()) << " bytes" << endl;
	seekg(offset, ios::beg);
}
template <class Data> void List<Data>::sort()
{
	if (is_empty())
		throw invalid_argument("List is empty");

	Node node1(this);
	node1.read_from(get_offset_head());
	int offset1 = node1.get_offset_next();
	while (offset1 != 0)
	{
		Node node2(this);
		node2.read_from(node1.get_offset_next());
		int offset2 = node2.offset;
			
		while (offset2 != 0)
		{
			if (!(node1.get_data() < node2.get_data()))
			{
				Data tmp = node1.get_data();
				node1.set_data(node2.get_data());
				node2.set_data(tmp);
				node1.save();
				node2.save();
			}
			offset2 = node2.get_offset_next();
			node2.read_from(offset2);
		}
		offset1 = node1.get_offset_next();
		node1.read_from(offset1);
	}
	save();
	/*
	int newHead;
	int newTail;
	merge_sort(header.offset_head, newHead, newTail);
	set_head(newHead);
	set_tail(newTail);
	save();*/
}
template <class Data> int List<Data>::get_size()
{
	return header.list_size;
}
template <class Data> void List<Data>::clear()
{
	String temp_file_name = "$";
	temp_file_name += file_name;
	fstream tmp_stream(temp_file_name.to_char(), fstream::out | fstream::binary);
	close();
	tmp_stream.close();
	remove(file_name.to_char());
	rename(temp_file_name.to_char(), file_name.to_char());
	open(file_name.to_char(), fstream::in | fstream::out | fstream::binary);
	set_offset_head(0);
	set_offset_tail(0);
	zero_size();
	save();
}
template <class Data> bool List<Data>::is_empty()
{
	return get_offset_head() == 0 ? true : false;
}

//-------Iterator-------//

template <class Data> List<Data>::Iterator::Iterator(List<Data> * list) : list(list), current_offset(0) {};

template <class Data> void List<Data>::Iterator::operator++(int)
{
	to_next();
}
template <class Data> void List<Data>::Iterator::operator--(int)
{
	to_prev();
}

template <class Data> void List<Data>::Iterator::to_head()
{
	if (list)
	{
		Node node(list);
		node.read_from(current_offset);
		if (node.offset == list->get_offset_head())
			throw range_error("Iterator is on the head");
		else
			current_offset = list->get_offset_head();
	}
	else
		throw invalid_argument("Iterator has no list");
}
template <class Data> void List<Data>::Iterator::to_tail()
{
	if (list)
	{
		Node node(list);
		node.read_from(current_offset);
		if (node.offset == list->get_offset_tail())
			throw range_error("Iterator is on the tail");
		else
			current_offset = list->get_offset_tail();
	}
	else
		throw invalid_argument("Iterator has no list");
}
template <class Data> void List<Data>::Iterator::to_next()
{
	if (is_inside())
	{
		Node node(list);
		node.read_from(current_offset);
		if (node.offset == list->get_offset_tail())
			throw range_error("Iterator is on the tail");
		else
			current_offset = node.get_offset_next();
	}
	else
		throw invalid_argument("Iterator is not inside the list");
}
template <class Data> void List<Data>::Iterator::to_prev()
{
	if (is_inside())
	{
		Node node(list);
		node.read_from(current_offset);
		if (node.offset == list->get_offset_head())
			throw range_error("Iterator is on the head");
		else
			current_offset = node.get_offset_prev();
	}
	else
		throw invalid_argument("Iterator is not inside the list");
}

template <class Data> Data List<Data>::Iterator::get_data()
{
	if (is_inside())
	{
		Node node(list);
		node.read_from(current_offset);
		return node.get_data();
	}
	else
		throw invalid_argument("Iterator is not inside the list");
}
template <class Data> void List<Data>::Iterator::set_data(Data data)
{
	if (is_inside())
	{
		Node node(list);
		node.read_from(current_offset);
		node.set_data(data);
		node.save();
	}
	else
		throw invalid_argument("Iterator is not inside the list");
}

template <class Data> bool List<Data>::Iterator::is_inside()
{
	return current_offset && list ? true : false;
}
template <class Data> void List<Data>::Iterator::set_list(List<Data> * list)
{
	this->list = list;
	current_offset = 0;
}
template <class Data> void List<Data>::Iterator::print_info()
{
	Node node(list);
	node.read_from(current_offset);
	cout << "List: " << list << endl;
	cout << "Current offset: " << current_offset << endl;
	cout << "Next offset: " << node.get_offset_next() << endl;
	cout << "Prev offset: " << node.get_offset_prev() << endl;
}

#endif