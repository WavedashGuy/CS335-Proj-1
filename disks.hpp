///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the alternating disks
// problem.
//
// As provided, this header has four functions marked with TODO comments.
// You need to write in your own implementation of these functions.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once



#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>
#include <functional>
#include <iostream>

enum disk_color { DISK_LIGHT, DISK_DARK };

class disk_state {
private:
    std::vector<disk_color> _colors;

public:
    disk_state(size_t light_count) // generates light_count # of light & dark disks
        : _colors(light_count * 2, DISK_LIGHT) { // total count, initializes all as disk light

        assert(light_count > 0);

        for (size_t i = 1; i < _colors.size(); i += 2) {
            _colors[i] = DISK_DARK;
        }
    }

    bool operator== (const disk_state& rhs) const {
        return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
    }

    size_t total_count() const {
        return _colors.size();
    }

    size_t light_count() const {
        return total_count() / 2;
    }

    size_t dark_count() const {
        return light_count();
    }

    bool is_index(size_t i) const {
        return (i < total_count());
    }

    disk_color get(size_t index) const {
        assert(is_index(index));
        return _colors[index];
    }

    void swap(size_t left_index) {
        assert(is_index(left_index));
        auto right_index = left_index + 1;
        assert(is_index(right_index));
        std::swap(_colors[left_index], _colors[right_index]);
    }

    std::string to_string() const {
        std::stringstream ss;
        bool first = true;
        for (auto color : _colors) {
            if (!first) {
                ss << " ";
            }

            if (color == DISK_LIGHT) {
                ss << "L";
            }
            else {
                ss << "D";
            }

            first = false;
        }
        return ss.str();
    }

    // Return true when this disk_state is in alternating format. That means
    // that the first disk at index 0 is light, the second disk at index 1
    // is dark, and so on for the entire row of disks.
    bool is_initialized() const {
        for (size_t i = 0; i < total_count(); i++) {     //check each position before function
            if (i % 2 == 0) {                              //check even position --> should be light
                if (_colors[i] == DISK_DARK) {
                    return false;
                }
            }
            else {                                      //check odd position --> should be dark
                if (_colors[i] == DISK_LIGHT) {
                    return false;
                }
            }
        }

        return true;
    }

    // Return true when this disk_state is fully sorted, with all light disks on
    // the left (low indices) and all dark disks on the right (high indices).
    bool is_sorted() const {

        for (size_t i = 0; i < light_count(); i++) {
            if (get(i) == 1)
                return false;
        }

        for (size_t i = light_count(); i < total_count(); i++) {
            if (get(i) == 0)
                return false;
        }

        return true;
    }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
    disk_state _after;
    unsigned _swap_count;

public:

    sorted_disks(const disk_state& after, unsigned swap_count)
        : _after(after), _swap_count(swap_count) { }

    sorted_disks(disk_state&& after, unsigned swap_count)
        : _after(after), _swap_count(swap_count) { }

    const disk_state& after() const {
        return _after;
    }
    
    unsigned swap_count() const {
        return _swap_count;
    }
};

// Algorithm that sorts disks using the alternate algorithm.
sorted_disks sort_alternate(const disk_state& before) {
    
    /*
    DISK_LIGHT == 0
    DISK_DARK == 1
    */

    /* initialization */
    bool isSorted = false;
    size_t numOfSwap = 0;
    disk_state state = before;
    size_t loopCount = 0;

    while (isSorted == false) {

        /* compares swapCheck w/ numOfSwap at the end. if numOfSwap is unchanged,
        then no swaps were performed this loop. hence, the group will be
        confirmed as sorted.*/
        size_t swapCheck = numOfSwap;

        size_t currentIndex = 0;
        
        while (currentIndex + loopCount < state.total_count() - loopCount) {
            disk_color tLeft = state.get(currentIndex + loopCount);
            disk_color tRight = state.get(currentIndex + loopCount + 1);

            if (tLeft == 1 && tRight == 0) {
                state.swap(currentIndex + loopCount);
                numOfSwap++;
            }

            currentIndex+=2;
        }

        if (swapCheck == numOfSwap && loopCount != 0)
            isSorted = true;
        else
            loopCount++;
    }

    return sorted_disks(disk_state(state), numOfSwap);
}


// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {
    /*
    DISK_LIGHT == 0
    DISK_DARK == 1
    */

    /* initialization */
    bool isSorted = false;
    size_t numOfSwap = 0;
    disk_state state = before;

    while (isSorted == false) {

        /* compares swapCheck w/ numOfSwap at the end. if numOfSwap is unchanged,
        then no swaps were performed this loop. hence, the group will be 
        confirmed as sorted.*/
        size_t swapCheck = numOfSwap;
        
        size_t loopCount = 0;

        /* left to right sort */
        while (loopCount < state.total_count() - 1) {
            disk_color tLeft = state.get(loopCount);
            disk_color tRight = state.get(loopCount + 1);

            if (tLeft == 1 && tRight == 0) {
                state.swap(loopCount);
                numOfSwap++;
            }
            loopCount++;
        }

        /* right to left sort */
        while (loopCount > 1) {
            disk_color tRight = state.get(loopCount);
            disk_color tLeft = state.get(loopCount - 1);

            if (tLeft == 1 && tRight == 0) {
                state.swap(loopCount - 1);
                numOfSwap++;
            }
            loopCount--;
        }

        if (swapCheck == numOfSwap)
            isSorted = true;
    }

    return sorted_disks(disk_state(state), numOfSwap);
}
