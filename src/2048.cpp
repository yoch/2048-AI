#include <cstdlib>
#include <cassert>
#include <array>
#include "2048.h"

using namespace std;


#include <iostream>
#include <iomanip>

ostream& operator<<(ostream& os, const game2048& game)
{
    for (int i=0, k=60; i < 16; ++i, k-=4)
    {
        const int a = (game.board >> k) & 0xf;
        os  << right << setw(6)
            << (a ? 1 << a : 0)
            << (i%4 == 3 ? '\n' : ',');
    }
    os << "\tscore is " << game.score << endl;
    return os;
}



#define N   0xffff


static uint16_t moves_rd[N];    // right or down (normal moves)
static uint16_t moves_lu[N];    // left or up (reversed moves)
static unsigned scores_rd[N];
static unsigned scores_lu[N];


game2048::game2048() :
    board(0),
    score(0),
    nplay(0)
{
#if DEBUG
    tictac = false;
#endif
}

game2048::game2048(unsigned grid[4][4]) :
    game2048()
{
    // init the board
    for (int i=0, k=60; i < 4; ++i)
        for (int j=0; j < 4; ++j, k-=4)
        {
            if (grid[i][j])
            {
                const uint64_t val = __builtin_ctz(grid[i][j]);
                board |= val << k;
            }
        }
}

inline void game2048::_set(unsigned i, uint64_t val)
{
    board |= val << (60-i);
}

inline int game2048::_get(unsigned i, int mask) const
{
    return (board >> (63 - i)) & mask;
}

uint16_t game2048::get_col(int col) const
{
    uint16_t ret = 0;
    for (int i = col*4; i < 64; i+=16)
    {
        ret = (ret << 4) | _get(i+3, 0xf);
    }
    return ret;
}

// static function member
void game2048::set_col(uint64_t& b, int col, uint16_t val)
{
    for (int i = 60-col*4, k=12; i >= 0; i-=16, k-=4)
    {
        const uint64_t x = (val >> k) & 0xf;
        b |= x << i;
    }
}


bool game2048::move_right()
{
#if PROFILE
    Counter++;
#endif
#if DEBUG
    assert(tictac);
#endif
    uint64_t nboard = 0;
    for (int i=0, k=48; i<4; ++i, k-=16)
    {
        const uint16_t row = (board >> k) & 0xffff;
        const uint64_t mv = moves_rd[row];
        nboard |= mv << k;
        score += scores_rd[row];
    }
    if (board == nboard)
        return false;
#if DEBUG
    tictac = !tictac;
#endif
    nplay++;
    board = nboard;
    return true;
}

bool game2048::move_left()
{
#if PROFILE
    Counter++;
#endif
#if DEBUG
    assert(tictac);
#endif
    uint64_t nboard = 0;
    for (int i=0, k=48; i<4; ++i, k-=16)
    {
        const uint16_t row = (board >> k) & 0xffff;
        const uint64_t mv = moves_lu[row];
        nboard |= mv << k;
        score += scores_lu[row];
    }
    if (board == nboard)
        return false;
#if DEBUG
    tictac = !tictac;
#endif
    nplay++;
    board = nboard;
    return true;
}

bool game2048::move_up()
{
#if PROFILE
    Counter++;
#endif
#if DEBUG
    assert(tictac);
#endif
    uint64_t nboard = 0;
    for (int i=0; i<4; ++i)
    {
        const uint16_t col = get_col(i);
        const uint64_t mv = moves_lu[col];
        set_col(nboard, i, mv);
        score += scores_lu[col];
    }
    if (board == nboard)
        return false;
#if DEBUG
    tictac = !tictac;
#endif
    nplay++;
    board = nboard;
    return true;
}

bool game2048::move_down()
{
#if PROFILE
    Counter++;
#endif
#if DEBUG
    assert(tictac);
#endif
    uint64_t nboard = 0;
    for (int i=0; i<4; ++i)
    {
        const uint16_t col = get_col(i);
        const uint64_t mv = moves_rd[col];
        set_col(nboard, i, mv);
        score += scores_rd[col];
    }
    if (board == nboard)
        return false;
#if DEBUG
    tictac = !tictac;
#endif
    nplay++;
    board = nboard;
    return true;
}

bool game2048::make_move(Direction d)
{
    switch(d)
    {
    case UP:
        return move_up();
    case DOWN:
        return move_down();
    case LEFT:
        return move_left();
    case RIGHT:
        return move_right();
    default:
        return false;
    }
}

int game2048::nbEmpty() const
{
    const uint64_t mask = ~board;
    return __builtin_popcountll(mask & (mask >> 1) & (mask >> 2) & (mask >> 3) & 0x1111111111111111ull);
}

bool game2048::finished() const
{
    return !nbEmpty();
}

int game2048::randomEmptyIdx() const
{
    static int t[16];
    uint64_t mask = ~board;
    int j = 0;
    for(int i=15; i>=0; --i)
    {
        if ((mask & 0xf) == 0xf)
            t[j++] = i;
        mask >>= 4;
    }
    return j==0 ? -1 : t[rand()%j];
}

/*
bool game2048::isEmpty(size_t idx) const
{
    const uint64_t mask = ~board;
    return (((mask >> (60 - idx*4)) & 0xf) == 0xf);
}
*/

bool game2048::next()
{
#if DEBUG
    assert(!tictac);
#endif
    int index = randomEmptyIdx();
    if (index == -1)
        return false;
#if DEBUG
    tictac = !tictac;
#endif
    _set(index * 4, rand() % 10 == 0 ? 0x2 : 0x1);
    return true;
}


void game2048::random_play()
{
    while(!finished())
    {
        next();
        int flags = 0;
        while (flags != 0xf)
        {
            const int d = rand() % 4;
            const int v = 1 << d;
            if (flags & v)
                continue;
            if (make_move((Direction)d))
                break;
            flags |= v;
        }
    }
}



///*////////////////////////////////////////////////////////////////////

uint16_t encode(const array<unsigned, 4>& tab)
{
    const unsigned a = tab[0] == 0 ? 0 : __builtin_ctz(tab[0]),
                   b = tab[1] == 0 ? 0 : __builtin_ctz(tab[1]),
                   c = tab[2] == 0 ? 0 : __builtin_ctz(tab[2]),
                   d = tab[3] == 0 ? 0 : __builtin_ctz(tab[3]);
    return (a << 12) | (b << 8) | (c << 4) | d;
}

array<unsigned, 4> decode(uint16_t val)
{
    array<unsigned, 4> ret = {0};
    const unsigned a = (val & 0xf000) >> 12,
                   b = (val & 0x0f00) >> 8,
                   c = (val & 0x00f0) >> 4,
                   d = (val & 0x000f);
    if (a) ret[0] = 1 << a;
    if (b) ret[1] = 1 << b;
    if (c) ret[2] = 1 << c;
    if (d) ret[3] = 1 << d;
    return ret;
}

unsigned _move_left(array<unsigned, 4>& t)
{
    unsigned score = 0;
    bool last = true;
    int k = 0;
    for (int i=0; i<4; ++i)
    {
        if (t[i] != 0)
        {
            if (i != k)
                swap(t[k], t[i]);
            if (k != 0 && t[k-1] == t[k] && last)
            {
                t[k-1] += t[k];
                t[k] = 0;
                score += t[k-1];
                k -= 1;
                last = false;
            }
            else
            {
                last = true;
            }
            k++;
        }
    }
    return score;
}

unsigned _move_right(array<unsigned, 4>& t)
{
    unsigned score = 0;
    bool last = true;
    int k = 3;
    for (int i=3; i>=0; --i)
    {
        if (t[i] != 0)
        {
            if (i != k)
                swap(t[k], t[i]);
            if (k+1 < 4 && t[k+1] == t[k] && last)
            {
                t[k+1] += t[k];
                t[k] = 0;
                score += t[k+1];
                k += 1;
                last = false;
            }
            else
            {
                last = true;
            }
            k--;
        }
    }
    return score;
}

void game2048::init()
{
    // Precomputing
    for (int i=0; i<N; ++i) // all combinations
    {
        array<unsigned, 4> t = decode(i);
        array<unsigned, 4> t2;
        unsigned score;
        t2 = t;
        score = _move_right(t2);
        moves_rd[i] = encode(t2);
        scores_rd[i] = score;
        t2 = t;
        score = _move_left(t2);
        moves_lu[i] = encode(t2);
        scores_lu[i] = score;
    }
}


#if PROFILE
uint64_t game2048::Counter = 0;
#endif
