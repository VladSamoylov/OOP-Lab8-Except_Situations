#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

class Exception {
protected:
	char error[255];
public:
	virtual void Error() const noexcept = 0;
};

class ExceptionDivbyZero : public Exception {
private:
	int div;
public:
	ExceptionDivbyZero(const char* er) { strcpy_s(this->error, er); this->div = 1; }
	void Error() const noexcept override { cerr << this->error << endl; }
	int SetDefault() noexcept { return this->div; }
};

class ExceptionFileStream : public Exception {
public:
	ExceptionFileStream(const char* er) { strcpy_s(this->error, er); }
	void Error() const noexcept override { cerr << this->error << endl; }
};

class ExceptionStream : public Exception {
public:
	ExceptionStream(const char* er) { strcpy_s(this->error, er); }
	void Error() const noexcept override { cerr << this->error << endl; }
	template <class T> void Check(T& x) {
		try {
			cin.clear();
			cin.ignore(666, '\n');
			cout << "Re-enter:";
			cin >> x;
			if (cin.fail()) throw ExceptionStream("Error: <Input data>");
		}
		catch (ExceptionStream& er) {
			er.Error();
			er.Check(x);
		}		
	}
};

class ExceptionAlloc : public Exception {
public:
	ExceptionAlloc(const char* er) { strcpy_s(this->error, er); }
	void Error() const noexcept override { cerr << this->error << endl; }	
};

class RealNumber {
private:
	double* mas;
	int n;
public:
	RealNumber();
	RealNumber(const int&);
	RealNumber(const RealNumber&);
	void Generate();
	RealNumber operator=(const RealNumber&);
	RealNumber operator+(const RealNumber&);
	RealNumber operator-(const RealNumber&);
	double operator*(const RealNumber&);
	RealNumber operator<<(int);
	friend RealNumber operator>>(RealNumber&, int);
	double& operator[](const int&);
	void Show();
	void SaveToFile(const string&);
	void LoadFromFile(const string&);
	void* operator new(size_t);
	void* operator new[](size_t);
	void operator delete(void* p);
	void operator delete[](void* p);
	~RealNumber();
};

/**
 * @brief Конструктор класу RealNumber за замовченням
*/
RealNumber::RealNumber() {

	this->mas = nullptr;
	this->n = 0;
}

/**
 * @brief Параметричний конструктор класу RealNumber
 * @param n Кількість чисел дійсного типу в масиві
*/
RealNumber::RealNumber(const int& n) {

	this->n = n;
	try {
		this->mas = new double[this->n];
	}
	catch (ExceptionAlloc& er) {
		er.Error();
		abort();
	}
	catch (bad_alloc) {
		cout << "error\n";
		abort();
	}
	for (int i = 0; i < this->n; i++) {
		this->mas[i] = (rand() % 21) - 10;
	}
}

/**
 * @brief Конструктор копіювання класу RealNumber
 * @param rn Об'єкт для копіювання
*/
RealNumber::RealNumber(const RealNumber& rn) {

	this->n = rn.n;
	try {
		this->mas = new double[this->n];
	}
	catch (ExceptionAlloc& er) {
		er.Error();
		abort();
	}
	for (int i = 0; i < rn.n; i++) {
		this->mas[i] = rn.mas[i];
	}
}

/**
 * @brief Метод, який генерує новий масив дійсного типу
*/
void RealNumber::Generate() {

	if (this->mas != nullptr) delete[] this->mas;
	this->n = (rand() % 10) + 1;
	try {
		this->mas = new double[this->n];
	}
	catch (ExceptionAlloc& er) {
		er.Error();
		abort();
	}
	for (int i = 0; i < this->n; i++) {
		this->mas[i] = (rand() % 21) - 10;
	}
}

/**
 * @brief Метод, який відображає зміст масиву
*/
void RealNumber::Show() {

	cout << "{";
	for (int i = 0; i < this->n; i++) {
		cout << setw(4) << right << this->mas[i];
	}
	cout << " }" << endl;
}

/**
 * @brief Деструктор
*/
RealNumber::~RealNumber() {
	
	delete[] this->mas;
}

/**
 * @brief Перевантаження бінарного оператору =
 * @param rn Об'єкт правого аргументу
 * @return Повернення поточного об'єкту
*/
RealNumber RealNumber::operator=(const RealNumber& rn) {

	if (this->n != rn.n) {
		if (this->mas != nullptr) delete[] this->mas;
		this->n = rn.n;
		try {
			this->mas = new double[this->n];
		}
		catch (ExceptionAlloc& er) {
			er.Error();
			abort();
		}
	}
	for (int i = 0; i < rn.n; i++) {
		this->mas[i] = rn.mas[i];
	}
	return *this;
}

/**
 * @brief Перевантаження бінарного оператору +
 * @param rn Об'єкт правого аргументу
 * @return Повернення тимчасового об'єкту
*/
RealNumber RealNumber::operator+(const RealNumber& rn) {

	if (this->n > rn.n) {
		RealNumber tmp(*this);
		for (int i = 0; i < rn.n; i++) {
			tmp.mas[i] += rn.mas[i];
		}
		return tmp;
	}
	else if (this->n < rn.n) {
		RealNumber tmp(rn);
		for (int i = 0; i < this->n; i++) {
			tmp.mas[i] += this->mas[i];
		}
		return tmp;
	}
	else {
		RealNumber tmp(this->n);
		for (int i = 0; i < tmp.n; i++) {
			tmp.mas[i] = this->mas[i] + rn.mas[i];
		}
		return tmp;
	}
}

/**
 * @brief Перевантаження бінарного оператору -
 * @param rn Об'єкт правого аргументу
 * @return Повернення тимчасового об'єкту
*/
RealNumber RealNumber::operator-(const RealNumber& rn) {

	if (this->n > rn.n) {
		RealNumber tmp(*this);
		for (int i = 0; i < rn.n; i++) {
			tmp.mas[i] -= rn.mas[i];
		}
		return tmp;
	}
	else if (this->n < rn.n) {
		RealNumber tmp(rn);
		for (int i = 0; i < this->n; i++) {
			tmp.mas[i] = this->mas[i] - tmp.mas[i];
		}
		for (int j = this->n; j < rn.n; j++) {
			tmp.mas[j] = 0 - tmp.mas[j];
		}
		return tmp;
	}
	else {
		RealNumber tmp(this->n);
		for (int i = 0; i < tmp.n; i++) {
			tmp.mas[i] = this->mas[i] - rn.mas[i];
		}
		return tmp;
	}
}

/**
 * @brief Перевантаження бінарного оператору *
 * @param rn Об'єкт правого аргументу
 * @return Результату скалярного множення двох об'єктів
*/
double RealNumber::operator*(const RealNumber& rn) {

	double scalar_product = 0.0;
	if (this->n > rn.n) {
		for (int i = 0; i < rn.n; i++) {
			scalar_product += this->mas[i] * rn.mas[i];
		}
	}
	else if (this->n < rn.n) {
		for (int i = 0; i < this->n; i++) {
			scalar_product += this->mas[i] * rn.mas[i];
		}
	}
	else {
		for (int i = 0; i < this->n; i++) {
			scalar_product += this->mas[i] * rn.mas[i];
		}
	}

	return scalar_product;
}

/**
 * @brief Перевантаження оператору =
 * @param index Елемент для отримання за індексом
 * @return Значення елементу за індексом
*/
double& RealNumber::operator[](const int& index) {

	if (index < 0 || index >= this->n) throw (this->n - 1);
	else return this->mas[index];
}

/**
 * @brief Перевантаження оператору <<
 * @param notused Не використовується
 * @return Повернення поточного об'єкту
*/
RealNumber RealNumber::operator<<(int notused) {

	if (this->mas == nullptr) return *this;
	double tmp = this->mas[0];
	for (int i = 0; i < this->n - 1; i++) {
		this->mas[i] = this->mas[i + 1];
	}
	this->mas[this->n - 1] = tmp;

	return *this;
}

/**
 * @brief Дружня функція, яка перевантажує оператор >>
 * @param rn Об'єкт лівого аргументу
 * @param notused Не використовується
 * @return Повернення об'єкту
*/
RealNumber operator>>(RealNumber& rn, int notused) {

	if (rn.mas == nullptr) return rn;
	double tmp = rn.mas[rn.n - 1];
	for (int i = rn.n - 1; i > 0; i--) {
		rn.mas[i] = rn.mas[i - 1];
	}
	rn.mas[0] = tmp;

	return rn;
}

/**
 * @brief Метод, який зберігає масив до файлу
 * @param name Ім'я файлу
*/
void RealNumber::SaveToFile(const string& name) {

	ofstream fout;
	try {
		fout.open(name, ios::out | ios::trunc);

		if (!fout) throw ExceptionFileStream("Error: <Cannot opened or created file>");
		if (!fout.is_open()) throw ExceptionFileStream("Error: <File is not opened or created>");

		fout << this->n << " ";
		for (int i = 0; i < this->n; i++) {
			fout << this->mas[i] << " ";
		} fout << "\n";

		if (fout.fail()) throw ExceptionFileStream("Error: <File write operation failed>");
		
		fout.close();
	}	
	catch (ExceptionFileStream& er) {
		er.Error();
	}
}

/**
 * @brief Метод, який зчитує дані з файлу до масиву
 * @param name Ім'я файлу
*/
void RealNumber::LoadFromFile(const string& name) {

	try {
		ifstream fin(name, ios::in);

		if (!fin) throw ExceptionFileStream("Error: <Cannot opened or read file>");
		if (!fin) throw ExceptionFileStream("Error: <File is not opened or read>");

		fin >> this->n;
		if (this->mas != nullptr) delete[] this->mas;
		try {
			this->mas = new double[this->n];
		}
		catch (ExceptionAlloc& er) {
			er.Error();
			abort();
		}
		for (int i = 0; i < this->n; i++) {
			fin >> this->mas[i];
		}

		if (!fin.good()) throw ExceptionFileStream("Error: <File read operation failed>");
		if (fin.fail() && !fin.eof()) throw ExceptionFileStream("Error: <File read operation failed>");
		if (fin.bad()) throw 1;

		fin.close();
	}
	catch (ExceptionFileStream& er) {
		er.Error();
	}
	catch (int&) {
		cerr << "Error: <Critical error of read operation>" << endl;
		exit(1);
	}
	catch (...) {
		cerr << "Error: <Unknown error>" << endl;
	}
}

/**
 * @brief Функція, яка перевіряє правильність введення даних користувачем
 * @param x Данні для перевірки
*/
template <class T>
void CheckData(T& x) {

	cin.ignore(666, '\n');
	while (true) {
		if (cin.fail()) {
			cin.clear();
			cin.ignore(666, '\n');
			cerr << "You enter wrong data! Please enter correct int data: \n";
			cin >> x;
			cin.ignore(666, '\n');
		}
		else break;
	}
}

/**
 * @brief Перевантаження оператора виділення пам'яті
 * @param size Розмір блоку пам'яті
 * @return Вказівник адреси початку блоку
*/
void* RealNumber::operator new(size_t size) {

	void* p;
	p = malloc(size);
	if (!p) { throw ExceptionAlloc("Error: <Allocation of dynamic memory>"); } //bad_alloc ba;  throw ba;
}

/**
 * @brief Перевантаження оператора виділення пам'яті
 * @param size Розмір блоку пам'яті
 * @return Вказівник адреси початку блоку
*/
void* RealNumber::operator new[](size_t size) {
	
	void* p;
	p = malloc(size);
	if (!p) { throw ExceptionAlloc("Error: <Allocation of dynamic memory>"); } //bad_alloc ba; throw ba; }	
}

/**
 * @brief Перевантаження оператору вивільнення пам'яті
 * @param p Вказівник на блок пам'яті
*/
void RealNumber::operator delete(void* p) {
	
	free(p);
}

/**
 * @brief Перевантаження оператору вивільнення пам'яті
 * @param p Вказівник на блок пам'яті
*/
void RealNumber::operator delete[](void* p) {

	free(p);
}


int main() {

	srand(static_cast<unsigned int>(time(nullptr)));
	int work = 1;
	RealNumber rn1(2), rn2;

	int a, b;
	
	while (work != 0) {
		cout << "cin a and b: ";
		try {
			cin >> a >> b;
			if (cin.fail()) throw ExceptionStream("Error: <Input data>");
		}
		catch (ExceptionStream& e) {
			e.Error();
			cerr << "a-"; e.Check(a);
			cerr << "b-"; e.Check(b);
		}
		try {
			if (b == 0) throw ExceptionDivbyZero("Error: <Division by Zero>");
		}
		catch (ExceptionDivbyZero& e) {
			e.Error();
			b = e.SetDefault();
		}
		cout << a << " " << b << endl;
		cout << "a / b = " << static_cast<float>(a) / static_cast<float>(b) << endl << endl;

		cout << "rn1: "; rn1.Show();
		cout << "rn1 save to file...\n"; rn1.SaveToFile("work.txt");
		cout << "rn2 load from file...\n"; rn2.LoadFromFile("work.txt");
		cout << "rn2: "; rn2.Show();
		
		try {
			cout << "\nGet 4 element of rn2[4]: " << rn2[4] << endl;
		}
		catch (int& index) {
			cerr << "Error <Current index don't find>" << endl;
			cout << "Return the element of maximum masive index [" << index << "] = " << rn2[index] << endl;
		}		

		if (work != 0) {
			cout << "\nRetry? 1 - yes / 0 - no" << endl;
			cin >> work;
			CheckData(work);
			rn1.Generate();
			rn2.Generate();
		}
	}	

	return 0;
}