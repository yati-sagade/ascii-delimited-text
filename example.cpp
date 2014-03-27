/** Compile using
 *
 *      g++ -std=c++11 -Wall example.cpp adt.cpp -o example
 *
**/

#include "adt.hpp"
using namespace adt;

int main(int argc, char *argv[])
{
    Record r1, r2, r3;

    r1 << "this" << "is" << "a" << "record";

    r2 << "this" << "is" << 1.0D << "more";

    r3 << 3 << "rd" << "one";

    std::ofstream out;
    out.open("test.dat");

    out << r1 << r2 << r3;

    out.close();

    // Read the records and display them
    std::ifstream in;
    in.open("test.dat");

    Record r;
    while(in >> r)
    {
        std::string s;
        while (!r.empty())
        {
            r >> s;
            std::cout << s << "|";
        }
        std::cout << std::endl;
    }

    return 0;
}


    





