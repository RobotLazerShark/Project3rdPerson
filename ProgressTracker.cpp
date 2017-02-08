#include "P3P/ProgressTracker.hpp"
#include <P3P/Level.hpp>


ProgressTracker::ProgressTracker()
{
    //ctor
}

void ProgressTracker::checkWin()
{
    bool win = true;

    if (boxSpots.size () == 0)
    {
        win = false;
    }

    for (int i = 0, size = boxSpots.size (); i < size; i ++)
    {
        if (!boxSpots [i]->taken)
        {
            win = false;
        }
    }

    if (win)
    {
        Level::singletonInstance->clear ();
        //Load hub [TODO]
    }
}
