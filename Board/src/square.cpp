#include "../square.h"

namespace Board {
    Square::Square()
    {
    }

    std::ostream& operator << (std::ostream& stream, Square const& obj)
    {
        stream << obj.toString();

        return stream;
    }
}
