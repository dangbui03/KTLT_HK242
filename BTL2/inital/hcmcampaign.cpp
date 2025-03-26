#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////

// Kiểm tra có phải số chính phương không
bool isPerfectSquare(int n)
{
    if (n < 0)
        return false;                      // Số âm không thể là số chính phương
    int sqrtN = static_cast<int>(sqrt(n)); // Lấy căn bậc hai của n
    return (sqrtN * sqrtN == n);           // Kiểm tra nếu căn bậc hai đã làm tròn lại bình phương ra số ban đầu
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
        if (score > 1000)
            score = 1000;
        else if (score < 0)
            score = 0;
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
    int sumOfDigits(int num)
    {
        int sum = 0;
        while (num > 0)
        {
            sum += num % 10;
            num /= 10;
        }
        return sum;
    }

    // Tìm số cá nhân
    int getPersonalScore(int score)
    {
        int personalScore = sumOfDigits(score) + sumOfDigits(1975);
        while (personalScore > 9)
        {
            personalScore = sumOfDigits(personalScore);
        }
        return personalScore;
    }

    string infantryTypeToString(InfantryType type) const
    {
        switch (type)
        {
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
    Infantry(int quantity, int weight, const Position pos, InfantryType infantryType) : Unit(quantity, weight, pos), infantryType(infantryType) {}

    ~Infantry() {}

    int getAttackScore() override
    {
        int typeValue = infantryType;
        int score = typeValue * 56 + quantity * weight;

        if ((infantryType == SPECIALFORCES) && isPerfectSquare(weight))
        {
            score += 75;
        }

        int personalScore = getPersonalScore(score);
        if (personalScore > 7)
        {
            quantity = (int)ceil(quantity * 1.2);
            score = typeValue * 56 + quantity * weight;
        }
        else if (personalScore < 4)
        {
            quantity = (int)ceil(quantity * 0.9);
        }

        if (score > 500)
            score = 500;
        else if (score < 0)
            score = 0;
        return score;
    }

    string str() const override
    {
        return "Infantry[" + infantryTypeToString(infantryType) + '=' + to_string(infantryType) + "," + to_string(quantity) + "," + to_string(weight) + "," + pos.str() + "]";
    }
};

Army::Army(Unit **unitArray, int size, string name, BattleField *battleField) : LF(0), EXP(0), name(name), unitList(nullptr), battleField(battleField)
{
    for (int i = 0; i < size; i++)
    {
        Vehicle *vehicle = dynamic_cast<Vehicle *>(unitArray[i]);
        if (vehicle != nullptr)
        {
            LF += vehicle->getAttackScore();
        }
        else
        {
            Infantry *infantry = dynamic_cast<Infantry *>(unitArray[i]);
            if (infantry != nullptr)
            {
                EXP += infantry->getAttackScore();
            }
        }
    }
}

Army::~Army()
{
    delete unitList;
    delete battleField;
}

class LiberationArmy : public Army
{
public:
    LiberationArmy(const Unit **unitArray, int size, string name, BattleField *battleField) : Army(const_cast<Unit **>(unitArray), size, name, battleField) {}
    void fight(Army *enemy, bool defense = false) override
    {
        if (defense)
        {
            this->LF = LF * 1.3;
            this->EXP = EXP * 1.3;
        }
        else
        {
            this->LF = LF * 1.5;
            this->EXP = EXP * 1.5;
        }
    }
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////