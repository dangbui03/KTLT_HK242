#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////

#pragma region Position
Position::Position(int r, int c) {
    this->r = r;
    this->c = c;
};

Position::~Position() {};

string Position::str() const {
    return "(" + to_string(r) + "," + to_string(c) + ")";
};

int Position::getRow() const {
    return r;
};

int Position::getCol() const {
    return c;
};

void Position::setRow(int r) {
    this->r = r;
};

void Position::setCol(int c) {
    this->c = c;
};
#pragma endregion Position

#pragma region Unit
Unit:: Unit(int quantity, int weight, Position pos) {
    this->quantity = quantity;
    this->weight = weight;
    this->pos = pos;
};

Unit::~Unit() {};

int Unit::getAttackScore() {
    return 0;
};

Position Unit::getCurrentPosition() const {
    return pos;
};

string Unit::str() const {
    return "";
};
#pragma endregion Unit

class Tank : public Unit {
public:
    Tank(int quantity, int weight, Position pos) : Unit(quantity, weight, pos) {}
    int getAttackScore() override { return quantity * weight * 5; }
    string str() const override { return "Tank:" + pos.str() + ":" + to_string(quantity); }
};

class Soldier : public Unit {
public:
    Soldier(int quantity, int weight, Position pos) : Unit(quantity, weight, pos) {}
    int getAttackScore() override { return quantity * weight * 2; }
    string str() const override { return "Soldier:" + pos.str() + ":" + to_string(quantity); }
};

class Vehicle : public Unit{
    private:
        VehicleType vehicleType;
    public:
        Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType) {
            this->vehicleType = vehicleType;
            Unit(quantity, weight, pos);
        }
        ~Vehicle() { }  // Clean up memory

        int getAttackScore() { }
};
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////