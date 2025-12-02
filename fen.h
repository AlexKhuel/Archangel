#include "board.h"
#include <string>

class FEN
{
public:
    void fenToBoard(Board &board, std::string fenString);
};