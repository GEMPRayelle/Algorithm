#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

int Selection_Sorting(std::vector<int> &arr, int left, int right, int k)
{
    if (left == right)
        return arr[left];
    int p = (left + right) / 2;
    int high = left + 1;
    int low = right;

    std::swap(arr[p], arr[left]); // 피벗과 left와 위치 교환
    while (high <= low)
    {
        while ((high <= right) && (arr[high] <= arr[left]))
            high++;
        while ((low >= left) && (arr[low] >= arr[left]))
            low--;
        if (high > low)
            break;
        std::swap(arr[low], arr[high]);
    }

    std::swap(arr[left], arr[low]);

    int s = low - left;
    if (k < s)
        Selection_Sorting(arr, left, low - 1, k); // Small 그룹에서 찾기
    else if (k == s + 1)
        return arr[low]; // 피벗 = k번째 작은 숫자
    else
        Selection_Sorting(arr, low + 1, right, k - (s + 1)); // large 그룹에서 찾기
}

void PrintVector(std::vector<int> &arr)
{
    for (int i = 0; i < arr.size(); i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
}

int main()
{

    std::vector<int> testArr{31, 8, 48, 73, 11, 3, 29, 29, 65, 15};
    int k = 3;
    int result = Selection_Sorting(testArr, 0, testArr.size() - 1, k);

    PrintVector(testArr);
    std::cout << "선택된 3번째 작은 수: " << result << std::endl;

    return 0;
}
