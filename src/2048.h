#include <cstdint>
#include <ostream>


enum Direction {UP=0, DOWN=1, LEFT=2, RIGHT=3};


struct game2048
{
    static void init();
    static void set_col(uint64_t& b, int col, uint16_t val);

#if PROFILE
    static uint64_t Counter;
#endif

    uint64_t board;
    uint32_t score;
    uint32_t nplay;

#if DEBUG
    bool tictac;
#endif

    game2048();
    game2048(unsigned grid[4][4]);

    inline void _set(unsigned i, uint64_t val);
    inline int _get(unsigned i, int mask) const;
    uint16_t get_col(int col) const;

    bool move_right();
    bool move_left();
    bool move_up();
    bool move_down();
    bool make_move(Direction d);
    bool next();
    void random_play();

    //bool isEmpty(size_t idx) const;
    int nbEmpty() const;
    bool finished() const;
    int randomEmptyIdx() const;
};

std::ostream& operator<<(std::ostream& os, const game2048& game);
