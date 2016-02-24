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
        for (unsigned i=0; i<4; ++i)
        {
            game2048 g = gm;
            bool ok = g.make_move((Direction)i);
            if (!ok) continue;
            for (unsigned j=0; j<nb_runs; ++j)
            {
                game2048 g2 = g;
                g2.random_play();
                scores[i] += g2.score;
            }
        }
        int d = max_element(scores, scores+4) - scores;
        gm.make_move((Direction)d);

        cout << ++counter << ") move " << "UDLR"[d] << endl;
#if PROFILE
        cout << "\ttotal moves: " << game2048::Counter << endl;
#endif
    }
    gm.display();
    cout << "END" << endl;
}
