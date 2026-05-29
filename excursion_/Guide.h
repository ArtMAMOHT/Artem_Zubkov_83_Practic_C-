#pragma once

#include <string>

class Room;

class Guide {
public:
    explicit Guide(const std::string& name);

    void goToRoom(Room* room);
    void printCurrentRoomInfo() const;
    Room* getCurrentRoom() const;
    std::string getName() const;

private:
    std::string name;
    Room* currentRoom;
};
