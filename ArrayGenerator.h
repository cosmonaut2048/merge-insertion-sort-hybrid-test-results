#ifndef ARRAY_GENERATOR_H
#define ARRAY_GENERATOR_H

#include <vector>
#include <random>
#include <algorithm>

class ArrayGenerator {
private:
    std::random_device rd;
    std::mt19937 gen;

public:
    ArrayGenerator() : gen(rd()) {}

    // Случайные значения в диапазоне:
    std::vector<int> generateRandomArray(int size, int minVal = 0, int maxVal = 6000) {
        std::vector<int> arr(size);
        std::uniform_int_distribution<int> dist(minVal, maxVal);

        for (int i = 0; i < size; ++i) {
            arr[i] = dist(gen);
        }
        return arr;
    }

    // Обратно отсортированный массив:
    std::vector<int> generateReverseSortedArray(int size) {
        std::vector<int> arr(size);
        for (int i = 0; i < size; ++i) {
            arr[i] = size - i;
        }
        return arr;
    }

    // Почти отсортированный массив:
    std::vector<int> generateAlmostSortedArray(int size, int numSwaps = 10) {
        std::vector<int> arr(size);
        for (int i = 0; i < size; ++i) {
            arr[i] = i + 1;
        }

        std::uniform_int_distribution<int> dist(0, size - 1);
        for (int i = 0; i < numSwaps; ++i) {
            int idx1 = dist(gen);
            int idx2 = dist(gen);
            std::swap(arr[idx1], arr[idx2]);
        }
        return arr;
    }

    std::vector<int> getSubArray(const std::vector<int>& source, int size) {
        return std::vector<int>(source.begin(), source.begin() + size);
    }
};

#endif