//
// Created by admin on 03/06/2024.
//

#ifndef CATAN_BOARD_HPP
#define CATAN_BOARD_HPP

namespace mycatan {
    // singleton class represents the board
    class Board {
    public:
        // deleting copy constructor
        Board(const Board& obj)   = delete;
        static Board* getInstance();

    private:
        // static pointer which will points to the board instance
        static Board* board;
        Board();
    };

}
#endif //CATAN_BOARD_HPP
