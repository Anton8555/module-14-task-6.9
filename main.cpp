#include <iostream>

// definitions of codes on the playing field
#define empty 0
#define ship 1
#define missed 2
#define hit 3

// data global
const int n = 10;
int field1[n][n];
int field2[n][n];
int x,y;
int x1,y1,x2,y2;


/*!
 * @brief Output of one given playing field.
 * @param field - given playing field.
 */
void fieldShow(int field[][n])
{
    std::cout
            << "+---------------+\n"
            << "|   0123456789  |\n"
            << "|  +----------+ |\n";
    for(int y=0; y<n; y++) {
        std::cout << "| " << y << '|';

        for (int x = 0; x < n; x++) {
            int cod = field[y][x];
            std::cout
                    << (cod == empty ? ' ' :
                        cod == ship ? 'S' :
                        cod == missed ? '.' :
                        cod == hit ? 'X' : '_');
        }

        std::cout << "| |\n";
    }
    std::cout
            << "|  +----------+ |\n"
            << "+---------------+\n";
}

/*!
 * @brief Output two playing fields next to each other with signatures of the players' numbers.
 */
void fieldsShow()
{
    std::cout
            << "+-----------------------------+\n"
            << "|    Player #1     Player #2  |\n"
            << "|   0123456789    0123456789  |\n"
            << "|  +----------+  +----------+ |\n";
    for(int y=0; y<n; y++) {
        std::cout << "| " << y << '|';

        for(int x=0; x<n; x++) {
            int cod = field1[y][x];
            std::cout
                    << (cod == empty ? ' ' :
                        cod == ship ? 'S' :
                        cod == missed ? '.' :
                        cod == hit ? 'X' : '_');
        }

        std::cout << "| " << y << '|';

        for(int x=0; x<n; x++) {
            int cod = field2[y][x];
            std::cout
                    << (cod == empty ? ' ' :
                        cod == ship ? 'S' :
                        cod == missed ? '.' :
                        cod == hit ? 'X' : '_');
        }

        std::cout << "| |\n";
    }
    std::cout
            << "|  +----------+  +----------+ |\n"
            << "+-----------------------------+\n";
}

/*!
 * @brief Input of one pairs of coordinates with check on an admissible range.
 */
void inputXY()
{
    while( true ) {
        //std::cout << ">";
        std::cin >> x >> y;
        if( (x>=0) && (x<n) && (y>=0) && (y<n) )
            break;
        std::cout << "Input Error! Try again.\n";
    }
}

/*!
 * @brief Input of two pairs of coordinates with checking according to the conditions of the game.
 * @param field - the playing field for which the coordinates are entered.
 * @param ship_deck - ship length.
 * @return Return true if the input is valid, false otherwise.
 */
bool inputXY2(int field[][n], int ship_deck) {
    // input of two pairs of coordinates
    // 1st pair of coordinates
    inputXY(); x1 = x; y1 = y;
    // 2nd pair of coordinates
    inputXY(); x2 = x; y2 = y;

    // ships should only be vertical or horizontal.
    if ((x1 == x2) && (y1 == y2) || (x1 != x2) && (y1 != y2))
        return false;

    if (x1 == x2) {
        if (y1 > y2)
            std::swap(y1, y2);
        // the coordinates must match the length of the ship.
        if((y2-y1+1)!=ship_deck)
            return false;
        // there should not be other ships in the new location of the ship.
        for (int i = y1; i <= y2; i++)
            if (field[i][x1] == ship)
                return false;
    } else {
        if (x1 > x2)
            std::swap(x1, x2);
        // the coordinates must match the length of the ship.
        if((x2-x1+1)!=ship_deck)
            return false;
        // there should not be other ships in the new location of the ship.
        for (int i = x1; i <= x2; i++)
            if (field[y1][i] == ship)
                return false;
    }

    return true;
}

/*!
 * @brief Installation of ships on a given playing field with a given number and number of decks of the ship.
 * @param field - given playing field.
 * @param ship_count - A given number of ships.
 * @param ship_deck - The given deck of ships.
 */
void setShip(int field[][n], int ship_count, int ship_deck) {
    for(int i=0; i<ship_count; i++) {
        // field output
        fieldShow(field);

        // input two pairs of coordinates
        while( !inputXY2(field, ship_deck) )
            std::cout << "Input Error! Try again.\n";

        // work with the array: installation of the ship.
        if (x1 == x2)
            for (int i = y1; i <= y2; i++)
                field[i][x1] = ship;
        else
            for (int i = x1; i <= x2; i++)
                field[y1][i] = ship;
    }
}

/*!
 * @brief Installation of ships on a given playing field.
 * @param field - given playing field.
 */
void inputField(int field[][n])
{
    // 1-deck ship
    std::cout << "Enter the coordinates of four single-deck ships (x, y):\n";
    for(int i=0; i<4; i++) {
        fieldShow(field);
        while( true ) {
            inputXY();
            if( field[y][x]==0 ) break;
            std::cout << "Input Error! Try again.\n";
        }
        field[y][x] = ship;
    }

    // 2-deck ship
    std::cout << "Enter the coordinates of the three double-deck ships (x1, y1, x2, y2):\n";
    setShip(field, 3, 2);

    // 3-deck ship
    std::cout << "Enter the coordinates of two three-deck ships (x1, y1, x2, y2):\n";
    setShip(field, 2, 3);

    // 4-deck ship
    std::cout << "Enter the coordinates of one four-deck ship (x1, y1, x2, y2):\n";
    setShip(field, 1, 4);
}

/*!
 * @brief Counting ships not sunk on a given playing field.
 * @param field - given playing field.
 * @return Returns the number of ships not sunk.
 */
int countShip(int field[][n]) {
    int count = 0;

    for(int y=0; y<n; y++)
        for(int x=0; x<n; x++)
            if( field[y][x] == ship )
                count++;

    return count;
}

/*!
 * @brief The function of implementing the gameplay with one player on a given playing field.\n
 * The function will stop allowing the player to shoot on the first miss.
 * @param field - given playing field.
 * @return Returns true if there are still ships not sunk, false otherwise.
 */
bool step(int field[][n]) {
    while( true ) {
        fieldsShow();

        inputXY();

        int cod = field[y][x];
        if (cod == ship) {
            field[y][x] = hit;
            std::cout << "Hit!\n";
            if (countShip(field) == 0)
                return false;
        } else //if( (cod==empty) || (cod==missed) || (cod==hit) )
        {
            if (cod == empty)
                field[y][x] = missed;
            std::cout << "Missed!\n";
            return true;
        }
    }
}



int main() {
    // data input
    // field #1
    std::cout << "\nEnter data field #1.\n";
    inputField(field1);
    // field #2
    std::cout << "\nEnter data field #2.\n";
    inputField(field2);

    // a game
    while( true ) {
        std::cout << "\nPlayer Turn #1.\n";
        if( !step(field2) ) {
            std::cout << "\nPlayer #1 won.\n";
            break;
        }

        std::cout << "\nPlayer Turn #2.\n";
        if( !step(field1) ) {
            std::cout << "\nPlayer #2 won.\n";
            break;
        }
    }

    return 0;
}
