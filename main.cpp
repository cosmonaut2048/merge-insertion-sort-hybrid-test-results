#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "ArrayGenerator.h"
#include "SortTester.h"

void saveResultsToFile(const std::string& filename,
                       const std::vector<int>& sizes,
                       const std::vector<long long>& times);

int main() {
    ArrayGenerator generator;
    SortTester tester{};

    // Генерация больших массивов для всех категорий.
    std::cout << "Generating test arrays..." << std::endl;
    auto randomLarge = generator.generateRandomArray(100000);
    auto reverseLarge = generator.generateReverseSortedArray(100000);
    auto almostLarge = generator.generateAlmostSortedArray(100000, 100);

    // Размеры для тестирования.
    std::vector<int> sizes;
    for (int size = 500; size <= 100000; size += 100) {
        sizes.push_back(size);
    }

    // Пороги для гибридного алгоритма.
    std::vector<int> thresholds = {5, 10, 20, 30, 50};

    // Тестирование для каждого типа массива.
    std::vector<std::string> types = {"random", "reverse", "almost"};
    std::vector<std::vector<int>> largeArrays = {randomLarge, reverseLarge, almostLarge};

    for (size_t typeIdx = 0; typeIdx < types.size(); ++typeIdx) {
        std::cout << "Testing " << types[typeIdx] << " arrays..." << std::endl;

        // Стандартный Merge Sort.
        std::vector<long long> mergeTimes;
        for (int size : sizes) {
            auto subarray = generator.getSubArray(largeArrays[typeIdx], size);
            long long time = tester.testMergeSort(subarray, 3); // 3 запуска для усреднения
            mergeTimes.push_back(time);
            std::cout << "Merge Sort - Size: " << size << ", Time: " << time << " us" << std::endl;
        }
        saveResultsToFile("merge_" + types[typeIdx] + ".csv", sizes, mergeTimes);

        // Гибридный алгоритм для разных порогов.
        for (int threshold : thresholds) {
            std::vector<long long> hybridTimes;
            for (int size : sizes) {
                auto subarray = generator.getSubArray(largeArrays[typeIdx], size);
                long long time = tester.testHybridSort(subarray, threshold, 3);
                hybridTimes.push_back(time);
                std::cout << "Hybrid(threshold=" << threshold << ") - Size: " << size
                          << ", Time: " << time << " us" << std::endl;
            }
            saveResultsToFile("hybrid_" + types[typeIdx] + "_th" +
                              std::to_string(threshold) + ".csv", sizes, hybridTimes);
        }
    }

    std::cout << "Testing complete! Results saved to CSV files." << std::endl;
    return 0;
}

void saveResultsToFile(const std::string& filename,
                       const std::vector<int>& sizes,
                       const std::vector<long long>& times) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "Size,Time(us)\n";
        for (size_t i = 0; i < sizes.size(); ++i) {
            file << sizes[i] << "," << times[i] << "\n";
        }
        file.close();
    }
}