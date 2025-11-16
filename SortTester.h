#ifndef SORT_TESTER_H
#define SORT_TESTER_H

#include <vector>
#include <chrono>
#include <functional>
#include <iostream>
#include "SortingAlgorithms.h"

class SortTester {
private:
    SortingAlgorithms sorter;

public:
    long long testSort(std::vector<int> arr,
                       std::function<void(std::vector<int>&, int, int)> sortFunc,
                       int runs = 5) {
        long long totalTime = 0;

        for (int i = 0; i < runs; ++i) {
            std::vector<int> testArr = arr;

            auto start = std::chrono::high_resolution_clock::now();
            sortFunc(testArr, 0, testArr.size() - 1);
            auto elapsed = std::chrono::high_resolution_clock::now() - start;

            if (!sorter.isSorted(testArr)) {
                std::cerr << "Error: Array not sorted correctly!" << std::endl;
                return -1;
            }

            totalTime += std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        }

        return totalTime / runs; // Среднее время в микросекундах.
    }

    // Тестирование стандартного Merge Sort.
    long long testMergeSort(std::vector<int> arr, int runs = 5) {
        auto mergeSortFunc = [this](std::vector<int>& arr, int left, int right) {
            sorter.mergeSort(arr, left, right);
        };
        return testSort(arr, mergeSortFunc, runs);
    }

    // Тестирование гибридного алгоритма.
    long long testHybridSort(std::vector<int> arr, int threshold, int runs = 5) {
        auto hybridSortFunc = [this, threshold](std::vector<int>& arr, int left, int right) {
            sorter.hybridSort(arr, left, right, threshold);
        };
        return testSort(arr, hybridSortFunc, runs);
    }
};

#endif