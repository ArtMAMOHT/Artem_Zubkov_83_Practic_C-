#include "Guide.h"
#include "Room.h"

#include <iostream>
#include <windows.h>

const int ROOM_COUNT = 5;
const int GROUP_SIZE = 10;  // сколько человек в группе на экскурсии

void setupRussianConsole() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

void printMenu() {
    std::cout << "\n===== Экскурсия по офису Яндекса =====\n";
    std::cout << "1. Посетить комнату\n";
    std::cout << "2. Информация о текущей комнате\n";
    std::cout << "3. Показать все посещённые комнаты\n";
    std::cout << "4. Самая посещаемая комната\n";
    std::cout << "5. Выход\n";
    std::cout << "Выберите пункт: ";
}

void printRoomList(Room* rooms[], int count) {
    std::cout << "\nДоступные комнаты:\n";
    for (int i = 0; i < count; ++i) {
        std::cout << (i + 1) << ". " << rooms[i]->getName()
                  << " (" << rooms[i]->getType()
                  << ", вместимость " << rooms[i]->getCapacity() << ")\n";
    }
}

void visitRoom(Guide& guide, Room* rooms[], int count) {
    printRoomList(rooms, count);

    std::cout << "\nВведите номер комнаты: ";
    int choice = 0;
    std::cin >> choice;

    if (choice < 1 || choice > count) {
        std::cout << "Неверный номер комнаты.\n";
        return;
    }

    Room* selectedRoom = rooms[choice - 1];

    // Доп. задание: в серверную нельзя провести слишком большую группу.
    if (selectedRoom->getName() == "Серверная"
        && GROUP_SIZE > selectedRoom->getCapacity()) {
        std::cout << "В серверную нельзя провести группу из "
                  << GROUP_SIZE << " человек.\n";
        std::cout << "Максимальная вместимость: "
                  << selectedRoom->getCapacity() << ".\n";
        return;
    }

    guide.goToRoom(selectedRoom);
    std::cout << "Гид " << guide.getName()
              << " провёл группу в комнату: "
              << selectedRoom->getName() << '\n';
}

void printVisitedRooms(Room* rooms[], int count) {
    bool hasVisited = false;
    int totalVisits = 0;

    for (int i = 0; i < count; ++i) {
        totalVisits += rooms[i]->getVisitCount();
        if (rooms[i]->getVisitCount() > 0) {
            hasVisited = true;
        }
    }

    if (!hasVisited) {
        std::cout << "Пока не посещено ни одной комнаты.\n";
        return;
    }

    std::cout << "Посещённые комнаты:\n";
    for (int i = 0; i < count; ++i) {
        if (rooms[i]->getVisitCount() > 0) {
            std::cout << rooms[i]->getName()
                      << " — тип: " << rooms[i]->getType()
                      << ", вместимость: " << rooms[i]->getCapacity()
                      << ", посещений: " << rooms[i]->getVisitCount()
                      << '\n';
        }
    }

    std::cout << "Общее количество посещений: " << totalVisits << '\n';
}

void printMostVisitedRoom(Room* rooms[], int count) {
    Room* bestRoom = nullptr;
    int maxVisits = 0;

    for (int i = 0; i < count; ++i) {
        if (rooms[i]->getVisitCount() > maxVisits) {
            maxVisits = rooms[i]->getVisitCount();
            bestRoom = rooms[i];
        }
    }

    if (bestRoom == nullptr) {
        std::cout << "Пока нет посещённых комнат.\n";
        return;
    }

    std::cout << "Самая посещаемая комната: " << bestRoom->getName() << '\n';
    std::cout << "Количество посещений: " << bestRoom->getVisitCount() << '\n';
}

int main() {
    setupRussianConsole();

    Room* rooms[ROOM_COUNT];
    rooms[0] = new Room("Переговорка Толстой", "переговорная", 12);
    rooms[1] = new Room("Кухня 3 этаж", "кухня", 20);
    rooms[2] = new Room("Open space Backend", "рабочая зона", 40);
    rooms[3] = new Room("Зона отдыха", "отдых", 15);
    rooms[4] = new Room("Серверная", "техническая", 5);

    Guide guide("Алексей");

    int choice = 0;
    do {
        printMenu();
        std::cin >> choice;

        switch (choice) {
        case 1:
            visitRoom(guide, rooms, ROOM_COUNT);
            break;
        case 2:
            guide.printCurrentRoomInfo();
            break;
        case 3:
            printVisitedRooms(rooms, ROOM_COUNT);
            break;
        case 4:
            printMostVisitedRoom(rooms, ROOM_COUNT);
            break;
        case 5:
            std::cout << "Экскурсия завершена. До встречи!\n";
            break;
        default:
            std::cout << "Такого пункта нет. Попробуйте ещё раз.\n";
            break;
        }
    } while (choice != 5);

    for (int i = 0; i < ROOM_COUNT; ++i) {
        delete rooms[i];
        rooms[i] = nullptr;
    }

    return 0;
}
