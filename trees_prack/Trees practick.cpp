#include "Treap.hpp"

#include <iostream>
#include <windows.h>

void setupRussianConsole() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

void printVector(const std::vector<int>& values) {
    std::cout << "[";
    for (std::size_t i = 0; i < values.size(); ++i) {
        std::cout << values[i];
        if (i + 1 < values.size()) {
            std::cout << ", ";
        }
    }
    std::cout << "]\n";
}

int main() {
    setupRussianConsole();

    Treap tree;

    std::cout << "Декартово дерево\n";

    int data[] = {50, 30, 70, 20, 40, 60, 80, 20, 90};
    for (int value : data) {
        bool ok = tree.insert(value);
        std::cout << "insert(" << value << ") -> "
                  << (ok ? "добавлено" : "уже есть") << '\n';
    }

    std::cout << "\nРазмер: " << tree.size() << '\n';
    std::cout << "Элементы по порядку: ";
    printVector(tree.values());

    std::cout << "\ncontains(40) -> " << std::boolalpha << tree.contains(40) << '\n';
    std::cout << "contains(25) -> " << tree.contains(25) << '\n';

    std::cout << "\nremove(30) -> " << tree.remove(30) << '\n';
    std::cout << "remove(100) -> " << tree.remove(100) << '\n';

    std::cout << "\nПосле удаления:\n";
    std::cout << "Размер: " << tree.size() << '\n';
    std::cout << "Элементы: ";
    printVector(tree.values());

    std::cout << "\nempty() -> " << tree.empty() << '\n';

    return 0;
}
