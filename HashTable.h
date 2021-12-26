#pragma once
#include <string>
#include <list>

struct Student {
	unsigned age;
	unsigned weight;

	Student(unsigned age, unsigned weight) : age(age), weight(weight) {};

	bool operator==(Student other) { return age == other.age && weight == other.weight; };
	bool operator!=(Student other) { return age != other.age || weight != other.weight; };
};

typedef std::string Key;
typedef Student Value;
typedef std::list<std::pair<Key, Value>> TableLine;

class HashTable {
private:
	const double _MAX_LOAD = 0.9;

	size_t _buffer_size  = 4;
	size_t _values_count = 0;

	TableLine* _data;

public:
	HashTable() : HashTable(4) {};
	HashTable(size_t buf_size);
	~HashTable();

	HashTable(const HashTable& b);
	HashTable(HashTable&& b);

	HashTable& operator=(const HashTable& b);
	HashTable& operator=(HashTable&& b);

	// Обменивает значения двух хэш-таблиц.
	void swap(HashTable& b);

	// Очищает контейнер.
	void clear();
	// Удаляет элемент по заданному ключу.
	bool erase(const Key& k);
	// Вставка в контейнер. Возвращаемое значение - успешность вставки.
	bool insert(const Key& k, const Value& v);

	// Проверка наличия значения по заданному ключу.
	bool contains(const Key& k) const;

	// Возвращает значение по ключу. Небезопасный метод.
	// В случае отсутствия ключа в контейнере, следует вставить в контейнер
	// значение, созданное конструктором по умолчанию и вернуть ссылку на него. 
	Value& operator[](const Key& k);
	const Value& operator[](const Key& k) const;

	// Возвращает значение по ключу. Бросает исключение при неудаче.
	Value& at(const Key& k);
	const Value& at(const Key& k) const;

	size_t size() const;
	bool empty() const;

	friend bool operator==(const HashTable& a, const HashTable& b);
	friend bool operator!=(const HashTable& a, const HashTable& b);

private:
	size_t hash(const Key& key, size_t size) const;
	void redistribute();
};

bool operator==(const HashTable& a, const HashTable& b);
bool operator!=(const HashTable& a, const HashTable& b);
