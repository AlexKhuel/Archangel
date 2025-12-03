#include <cstdint>
class Piece
{
public:
    static const uint8_t Pawn = 1;
    static const uint8_t Knight = 2;
    static const uint8_t Bishop = 3;
    static const uint8_t Rook = 4;
    static const uint8_t Queen = 5;
    static const uint8_t King = 6;

    static const uint8_t White = 0;
    static const uint8_t Black = 8;
};
