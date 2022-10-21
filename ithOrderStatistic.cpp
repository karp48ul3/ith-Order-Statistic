#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <climits>
#include <exception>

template<typename T>
concept Comparable = requires (T a, T b) { a <=> b; };

template<typename T>
requires Comparable<T>
T sort_5element_subVector(typename std::vector<T>::iterator iter, size_t n) {
    std::sort(iter, iter + n);
    return *(iter + (n + 1) / 2 - 1);
}

template<typename T>
requires Comparable<T>
T select(std::vector<T>& vec, int l, int r) {
    if (r-l > 4) {
        // vector of medians
        std::vector<T> vec_medians;
        // divide to 5 element subarrays and take median of each
        for (int i = l; i < r+1; i += 5) {
            if (r+1 - i >= 5) {
                // sort subarrays
                auto median = sort_5element_subVector<T>(vec.begin()+ i, 5);
                // push back median of a subarray
                vec_medians.push_back(median);
            }
            else {
                auto median = sort_5element_subVector<T>(vec.begin() + i, r - i);
                vec_medians.push_back(median);
            }
        }
        // call select recursivelly on vector of medians
        return select(vec_medians, 0, vec_medians.size()-1);
    }
    else {
        // sort vector of at most 5 elements
        std::sort(vec.begin()+l, vec.begin() +r+1);
        // return its median
        return *(vec.begin()+l + ((r-l + 2) / 2 - 1));

    }
}

template<typename T>
requires Comparable<T>
int partition(std::vector<T>& vec, int l, int r, int ith, T median) {
    // find index of a median and swap median with last element
    for (int i = l; i <= r; i++) {
        if (vec[i] == median) {
            T lastElement = vec[r];
            vec[r] = median;
            vec[i] = lastElement;
            break;
        }
    }

    // if left equals right return it
    if (l == r) {
        return l;
    }
    int i = l;
    // partition subvector
    for (int j= l; j <= r-1;) {
        if (vec[j] < median) {
            T temp = vec[j];
            vec[j] = vec[i];
            vec[i] = temp;
            i++;
            j++;
        }
        else {
            j++;
        }
    }

    // insert last element - median between left and right partition boundary
    T temp1 = vec[i];
    vec[i] = median;
    vec[r] = temp1;

    // return index of a median
    return i;
}

template<typename T>
requires Comparable<T>
T ithOrderStatistics(std::vector<T> & inputVec, int ithElement) {
    // Initialize return value to 0
    T atIthElem = static_cast<T>(0);

    // Check if ithElement is in allowed range
    if (!(ithElement > 0 && ithElement <= inputVec.size())) {
        std::cout << "Ith order statistics has wrong value" << std::endl;
        throw std::runtime_error("exit");
    }

    // l - left end of an array, r - right end of an array
    int l = 0, r = inputVec.size() - 1;

    int medianIndex = 1;

    bool found = false;

    // main loop
    while (l < r) {
        // selects median of medians
        auto median = select(inputVec, l, r);

        // partitionof subvector
        medianIndex = partition(inputVec, l, r, ithElement, median);

        // check if ith order ( here 5) is lower than the left of median partitioned subvector
        if (ithElement < medianIndex - l + 1) {
            //if so we narrow down searching to left subarray
            r = medianIndex - 1;
        }
        else if (ithElement > medianIndex - l + 1) {
            // narrow down searching to right subvector
            ithElement = ithElement - (medianIndex - l + 1);
            l = medianIndex + 1;
        }
        else {
            // ith order is a median
            atIthElem = inputVec[medianIndex];
            found = true;
            break;
        }
    }

    if (!found && l == r) {
        atIthElem = inputVec[l];
    }

    return atIthElem;
}


int main()
{
    
    int ithElement = 23;
    //std::vector<int> inputVec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

    // Input vector
    std::vector<int> inputVec{ 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 , 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29};
    
    // Call 5-th order statistic function on input vector
    auto orderStatistic = ithOrderStatistics(inputVec, ithElement);

    // print ith order statistic to console
    std::cout << "i-th Order Statistic: "<< orderStatistic << std::endl;
}



