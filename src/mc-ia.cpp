#include <iostream>
#include <algorithm>

#include "2048.h"

using namespace std;


void MC_IA(unsigned nb_runs)
{
    int counter = 0;
    // creating new board
    game2048 gm;
    while(!gm.finished())
    {
        gm.next();
        //gm.display();

        uint64_t scores[4] = {0};
        bool has_move = false;
        for (unsigned i=0; i<4; ++i)
        {
            game2048 g = gm;
            if (!g.make_move((Direction)i))
                continue;
            has_move = true;
            for (unsigned j=0; j<nb_runs; ++j)
            {
                game2048 g2 = g;
                g2.random_play();
                scores[i] += g2.score;
            }
        }
        if (!has_move) break;

        int d = max_element(scores, scores+4) - scores;
        gm.make_move((Direction)d);

        cout << ++counter << ") " << "UDLR"[d] << "\t(score: " << gm.score;
#if PROFILE
        cout << ", moves: " << game2048::Counter;
#endif
        cout << ")" << endl;
    }
    cout << gm << "END" << endl;
}
