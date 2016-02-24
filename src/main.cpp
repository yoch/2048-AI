#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "2048.h"


using namespace std;

void MC_IA();


int main()
{
    srand(time(NULL));

    game2048::init();

    MC_IA();

/*
    game2048 gm;

    char c;
    gm.next();
    while(!gm.finished())
    {
        bool ok = false;

        gm.display();

        cout << "your move (u/d/r/l): ";
        cin >> c;
        switch(c)
        {
        case 'u':
            ok = gm.move_up();
            break;
        case 'd':
            ok = gm.move_down();
            break;
        case 'l':
            ok = gm.move_left();
            break;
        case 'r':
            ok = gm.move_right();
            break;
        default:
            break;
        }

        if (!ok)
            continue;

        gm.next();
    }
*/

    return 0;
}
