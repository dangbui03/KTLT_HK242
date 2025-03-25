#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////

// Kiểm tra có phải số chính phương không
bool isPerfectSquare(int n) {
    if (n < 0) return false; // Số âm không thể là số chính phương
    int sqrtN = static_cast<int>(sqrt(n)); // Lấy căn bậc hai của n
    return (sqrtN * sqrtN == n); // Kiểm tra nếu căn bậc hai đã làm tròn lại bình phương ra số ban đầu
}

#pragma region Position
Position::Position(int r, int c)
{
    this->r = r;
    this->c = c;
};

Position::~Position() {};

string Position::str() const
{
    return "(" + to_string(r) + "," + to_string(c) + ")";
};

int Position::getRow() const
{
    return r;
};

int Position::getCol() const
{
    return c;
};

void Position::setRow(int r)
{
    this->r = r;
};

void Position::setCol(int c)
{
    this->c = c;
};
#pragma endregion Position

#pragma region Unit
Unit::Unit(int quantity, int weight, Position pos)
{
    this->quantity = quantity;
    this->weight = weight;
    this->pos = pos;
};

Unit::~Unit() {};

Position Unit::getCurrentPosition() const
{
    return pos;
};

string Unit::str() const
{
    return "";
};
#pragma endregion Unit

// class Tank : public Unit {
// public:
//     Tank(int quantity, int weight, Position pos) : Unit(quantity, weight, pos) {}
//     int getAttackScore() override { return quantity * weight * 5; }
//     string str() const override { return "Tank:" + pos.str() + ":" + to_string(quantity); }
// };

// class Soldier : public Unit {
// public:
//     Soldier(int quantity, int weight, Position pos) : Unit(quantity, weight, pos) {}
//     int getAttackScore() override { return quantity * weight * 2; }
//     string str() const override { return "Soldier:" + pos.str() + ":" + to_string(quantity); }
// }

class Vehicle : public Unit
{
private:
    VehicleType vehicleType;

    string vehicleTypeToString(VehicleType type) const
    {
        switch (type)
        {
        case TRUCK:
            return "TRUCK";
        case MORTAR:
            return "MORTAR";
        case ANTIAIRCRAFT:
            return "ANTIAIRCRAFT";
        case ARMOREDCAR:
            return "ARMOREDCAR";
        case APC:
            return "APC";
        case ARTILLERY:
            return "ARTILLERY";
        case TANK:
            return "TANK";
        default:
            return "Unknown VehicleType";
        }
    }

public:
    Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType)
        : Unit(quantity, weight, pos), vehicleType(vehicleType)
    {
    }
    ~Vehicle() {} // Clean up memory

    int getAttackScore() override
    {
        int typeValue = vehicleType;
        int score = (typeValue * 304 + quantity * weight) / 30;
        return score; // Round to threshold if exceeds
    }

    string str() const override
    {
        return "Vehicle[" + vehicleTypeToString(vehicleType) + '=' + to_string(vehicleType) + "," + to_string(quantity) + "," + to_string(weight) + "," + pos.str() + "]";
    }
};

class Infantry : public Unit
{
    private:
        InfantryType infantryType;

        // tìm tổng các chữ số của một số
        int sumOfDigits(int num) {
            int sum = 0;
            while (num > 0) {
                sum += num % 10;
                num /= 10;
            }
            return sum;
        }

        // Tìm số cá nhân
        int getPersonalScore(int score) {
            int personalScore = sumOfDigits(score) + sumOfDigits(1975);
            while (personalScore > 9) {
                personalScore = sumOfDigits(personalScore);
            }
            return personalScore;
        }

        string infantryTypeToString(InfantryType type) const {
            switch (type) {
                case SNIPER:
                    return "SNIPER";
                case ANTIAIRCRAFTSQUAD:
                    return "ANTIAIRCRAFTSQUAD";
                case MORTARSQUAD:
                    return "MORTARSQUAD";
                case ENGINEER:
                    return "ENGINEER";
                case SPECIALFORCES:
                    return "SPECIALFORCES";
                case REGULARINFANTRY:
                    return "REGULARINFANTRY";
                default:
                    return "Unknown InfantryType";
            }
        }

    public:
        Infantry(int quantity , int weight , const Position pos , InfantryType infantryType) :
            Unit(quantity , weight , pos) , infantryType(infantryType) {}

        ~Infantry() {}

        int getAttackScore() override {
            int typeValue = infantryType;
            int score = typeValue * 56 + quantity * weight;

            if ((infantryType == SPECIALFORCES) && isPerfectSquare(weight)) {
                score += 75;
            }

            int personalScore = getPersonalScore(score);
            if (personalScore > 7) {
                quantity = (int)ceil(quantity * 1.2);
                score = typeValue * 56 + quantity * weight;
            } else if (personalScore < 4) {
                quantity = (int)ceil(quantity * 0.9);
            }
            return score;
        }

        string str() const override {
            return "Infantry[" + infantryTypeToString(infantryType) + '=' + to_string(infantryType) + "," + to_string(quantity) + "," + to_string(weight) + "," + pos.str() + "]";
        }
};

class Army {
    
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////