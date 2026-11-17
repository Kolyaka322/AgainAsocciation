#include <iostream>
#include <string>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include <utility>

class StringArray {
private:
    int length;
    std::string* data;

public:
    StringArray() : length(0), data(nullptr) {}

    explicit StringArray(int arrayLength) : length(0), data(nullptr) {
        if (arrayLength < 0) {
            throw std::invalid_argument("Розмір масиву не може бути від'ємним");
        }
        if (arrayLength > 0) {
            length = arrayLength;
            data = new std::string[length]{};
        }
    }

    StringArray(const std::initializer_list<std::string>& list) : length(0), data(nullptr) {
        if (list.size() > 0) {
            length = static_cast<int>(list.size());
            data = new std::string[length];
            int i = 0;
            for (const auto& item : list) {
                data[i++] = item;
            }
        }
    }

    StringArray(const StringArray& other) : length(0), data(nullptr) {
        if (other.length > 0) {
            length = other.length;
            data = new std::string[length];
            for (int i = 0; i < length; ++i) {
                data[i] = other.data[i];
            }
        }
    }

    ~StringArray() {
        delete[] data;
    }

    StringArray& operator=(const StringArray& other) {
        if (this == &other) {
            return *this;
        }
        delete[] data;
        data = nullptr;
        length = 0;

        if (other.length > 0) {
            length = other.length;
            data = new std::string[length];
            for (int i = 0; i < length; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    StringArray& operator=(const std::initializer_list<std::string>& list) {
        delete[] data;
        data = nullptr;
        length = 0;

        if (list.size() > 0) {
            length = static_cast<int>(list.size());
            data = new std::string[length];
            int i = 0;
            for (const auto& item : list) {
                data[i++] = item;
            }
        }
        return *this;
    }

    int getLength() const {
        return length;
    }

    bool isEmpty() const {
        return length == 0;
    }

    std::string& operator[](int index) {
        if (index < 0 || index >= length) {
            throw std::out_of_range("Індекс за межами діапазону StringArray");
        }
        return data[index];
    }

    const std::string& operator[](int index) const {
        if (index < 0 || index >= length) {
            throw std::out_of_range("Індекс за межами діапазону StringArray");
        }
        return data[index];
    }

    void erase() {
        delete[] data;
        data = nullptr;
        length = 0;
    }

    void reallocate(int newLength) {
        erase();
        if (newLength <= 0) {
            return;
        }
        length = newLength;
        data = new std::string[length]{};
    }

    void resize(int newLength) {
        if (newLength == length) {
            return;
        }
        if (newLength <= 0) {
            erase();
            return;
        }
        std::string* newData = new std::string[newLength]{};
        int elementsToCopy = std::min(length, newLength);
        for (int i = 0; i < elementsToCopy; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        length = newLength;
    }

    void insertBefore(const std::string& value, int index) {
        if (index < 0 || index > length) {
            throw std::out_of_range("Індекс за межами діапазону StringArray");
        }
        std::string* newData = new std::string[length + 1]{};
        for (int i = 0; i < index; ++i) {
            newData[i] = data[i];
        }
        newData[index] = value;
        for (int i = index; i < length; ++i) {
            newData[i + 1] = data[i];
        }
        delete[] data;
        data = newData;
        ++length;
    }

    void insertAtBeginning(const std::string& value) {
        insertBefore(value, 0);
    }

    void insertAtEnd(const std::string& value) {
        insertBefore(value, length);
    }

    void remove(int index) {
        if (index < 0 || index >= length) {
            throw std::out_of_range("Індекс за межами діапазону StringArray");
        }
        if (length == 1) {
            erase();
            return;
        }
        std::string* newData = new std::string[length - 1]{};
        for (int i = 0; i < index; ++i) {
            newData[i] = data[i];
        }
        for (int i = index + 1; i < length; ++i) {
            newData[i - 1] = data[i];
        }
        delete[] data;
        data = newData;
        --length;
    }

    int find(const std::string& target) const {
        for (int i = 0; i < length; ++i) {
            if (data[i] == target) {
                return i;
            }
        }
        return -1;
    }

    bool contains(const std::string& target) const {
        return find(target) != -1;
    }

    StringArray filterByPrefix(const std::string& prefix) const {
        StringArray result;
        for (int i = 0; i < length; ++i) {
            if (data[i].rfind(prefix, 0) == 0) {
                result.insertAtEnd(data[i]);
            }
        }
        return result;
    }

    std::string join(const std::string& delimiter) const {
        std::string result;
        for (int i = 0; i < length; ++i) {
            result += data[i];
            if (i < length - 1) {
                result += delimiter;
            }
        }
        return result;
    }

    void sortAscending() {
        if (length > 1) {
            std::sort(data, data + length);
        }
    }

    void sortDescending() {
        if (length > 1) {
            std::sort(data, data + length, [](const std::string& a, const std::string& b) {
                return a > b;
                });
        }
    }

    void sortByLength() {
        if (length > 1) {
            std::sort(data, data + length, [](const std::string& a, const std::string& b) {
                if (a.size() != b.size()) {
                    return a.size() < b.size();
                }
                return a < b;
                });
        }
    }

    bool operator==(const StringArray& other) const {
        if (length != other.length) {
            return false;
        }
        for (int i = 0; i < length; ++i) {
            if (data[i] != other.data[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const StringArray& other) const {
        return !(*this == other);
    }

    StringArray operator+(const StringArray& other) const {
        StringArray result(length + other.length);
        for (int i = 0; i < length; ++i) {
            result.data[i] = data[i];
        }
        for (int i = 0; i < other.length; ++i) {
            result.data[length + i] = other.data[i];
        }
        return result;
    }

    StringArray& operator+=(const StringArray& other) {
        if (other.length == 0) {
            return *this;
        }
        int oldLength = length;
        resize(length + other.length);
        for (int i = 0; i < other.length; ++i) {
            data[oldLength + i] = other.data[i];
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const StringArray& arr) {
        os << "[";
        for (int i = 0; i < arr.length; ++i) {
            os << "\"" << arr.data[i] << "\"";
            if (i < arr.length - 1) {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }
};

int main() {
    system("chcp 1251>null");
    std::cout << std::boolalpha;

    std::cout << "=== Тест 1: Конструктори та розмір ===\n";
    StringArray emptyArr;
    std::cout << "Порожній масив: довжина = " << emptyArr.getLength() << ", isEmpty = " << emptyArr.isEmpty() << "\n";

    StringArray fixedArr(3);
    std::cout << "Фіксований масив на 3 рядки: довжина = " << fixedArr.getLength() << ", isEmpty = " << fixedArr.isEmpty() << "\n";

    StringArray initArr{ "apple", "banana", "cherry", "date" };
    std::cout << "Масив зі списку ініціалізації: довжина = " << initArr.getLength() << ", isEmpty = " << initArr.isEmpty() << "\n";

    try {
        std::cout << "Спроба створити масив з розміром -5: ";
        StringArray invalidArr(-5);
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Очікуваний виняток: " << e.what() << "\n";
    }

    std::cout << "\n=== Тест 2: Індексація та очищення ===\n";
    StringArray langs{ "C++", "Python", "Rust" };
    std::cout << "Початковий масив: " << langs << "\n";
    std::cout << "Елемент [1]: " << langs[1] << "\n";
    langs[1] = "Go";
    std::cout << "Після заміни [1] на \"Go\": " << langs << "\n";

    const StringArray& constLangs = langs;
    std::cout << "Тест константного доступу: " << constLangs[2] << "\n";

    try {
        std::cout << "Спроба доступу до індексу 10: ";
        std::cout << langs[10] << "\n";
    }
    catch (const std::out_of_range& e) {
        std::cout << "Перехоплено std::out_of_range: " << e.what() << "\n";
    }

    langs.erase();
    std::cout << "Після erase(): довжина = " << langs.getLength() << ", isEmpty = " << langs.isEmpty() << "\n";

    std::cout << "\n=== Тест 3: Глибоке копіювання ===\n";
    StringArray original{ "Alpha", "Bravo", "Charlie" };
    std::cout << "Оригінал: " << original << "\n";
    StringArray copyArr = original;
    std::cout << "Створено копію copyArr\n";
    copyArr[0] = "ZULU";
    std::cout << "Змінено copyArr[0] на \"ZULU\"\n";
    std::cout << "Копія:    " << copyArr << "\n";
    std::cout << "Оригінал: " << original << " -> Не змінився! (Глибоке копіювання працює)\n";

    StringArray a, b;
    a = b = original;
    a = a;
    std::cout << "Тест самоприсвоювання (arr = arr) пройшов успішно.\n";

    std::cout << "\n=== Тест 4: Зміна розміру, вставка та видалення ===\n";
    StringArray web{ "HTML", "CSS" };
    std::cout << "Початковий: " << web << "\n";
    web.insertAtBeginning("HTTP");
    std::cout << "insertAtBeginning(\"HTTP\"): " << web << "\n";
    web.insertAtEnd("JS");
    std::cout << "insertAtEnd(\"JS\"):         " << web << "\n";
    web.insertBefore("SQL", 2);
    std::cout << "insertBefore(\"SQL\", 2):    " << web << "\n";
    web.remove(1);
    std::cout << "remove(1) (видаляємо HTML): " << web << "\n";
    web.resize(2);
    std::cout << "resize(2) (обрізаємо):     " << web << "\n";
    web.resize(4);
    std::cout << "resize(4) (розширюємо):    " << web << "\n";

    std::cout << "\n=== Тест 5: Пошук, фільтрація та join ===\n";
    StringArray fruits{ "apple", "apricot", "banana", "avocado", "blueberry" };
    std::cout << "Колекція: " << fruits << "\n";
    std::cout << "Пошук \"banana\": індекс " << fruits.find("banana") << "\n";
    std::cout << "Пошук \"orange\": індекс " << fruits.find("orange") << "\n";
    std::cout << "Містить \"avocado\": " << (fruits.contains("avocado") ? "так" : "ні") << "\n";
    std::cout << "Фільтр за префіксом \"ap\": " << fruits.filterByPrefix("ap") << "\n";
    std::cout << "Результат join(\", \"): " << fruits.join(", ") << "\n";

    std::cout << "\n=== Тест 6: Сортування рядків ===\n";
    StringArray unsorted{ "watermelon", "fig", "banana", "kiwi", "apple" };
    std::cout << "Вихідний:     " << unsorted << "\n";
    unsorted.sortAscending();
    std::cout << "За алфавітом: " << unsorted << "\n";
    unsorted.sortDescending();
    std::cout << "За спаданням: " << unsorted << "\n";
    unsorted.sortByLength();
    std::cout << "За довжиною:  " << unsorted << "\n";

    std::cout << "\n=== Тест 7: Оператори потоку та перевантаження ===\n";
    StringArray arr1{ "frontend", "backend" };
    StringArray arr2{ "devops", "qa" };
    std::cout << "arr1: " << arr1 << "\n";
    std::cout << "arr2: " << arr2 << "\n";
    StringArray arr3 = arr1 + arr2;
    std::cout << "arr3 = arr1 + arr2: " << arr3 << "\n";
    std::cout << "Чи arr1 == arr2? " << (arr1 == arr2 ? "Так" : "Ні") << "\n";
    arr1 += arr2;
    std::cout << "arr1 += arr2;\n";
    std::cout << "arr1 після +=: " << arr1 << "\n";
    std::cout << "Чи arr1 == arr3? " << (arr1 == arr3 ? "Так" : "Ні") << "\n";

    return 0;
}