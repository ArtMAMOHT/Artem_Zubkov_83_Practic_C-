#include "Guide.h"

#include "Room.h"

#include <iostream>

Guide::Guide(const std::string& name) : name(name), currentRoom(nullptr) {}

void Guide::goToRoom(Room* room) {
    currentRoom = room;
    room->visit();
}

void Guide::printCurrentRoomInfo() const {
    std::cout << "Гид: " << name << '\n';

    if (currentRoom == nullptr) {
        std::cout << "Группа пока не находится ни в одной комнате.\n";
        return;
    }

    std::cout << "Текущая комната:\n";
    currentRoom->printInfo();
}

Room* Guide::getCurrentRoom() const {
    return currentRoom;
}

std::string Guide::getName() const {
    return name;
}
