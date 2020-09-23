#include "utils.h"

int main(void)
{
    auto window = init(600, 600, "Chapter 2 - program 1");
    if (!window)
        return 1;

    return 0;
}
