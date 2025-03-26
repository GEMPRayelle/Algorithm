#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>

int static randomPivotCount = 0;
int static medianPivotCount = 0;
// 스왑 함수
void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

// 랜덤 피벗 선택 (3-way partitioning 적용)
void Random_Quick_Sort(std::vector<int> &list, int left, int right)
{
    if (left >= right)
        return;

    randomPivotCount++;
    int index = left + rand() % (right - left + 1); // 피벗을 랜덤으로 설정
    int pivot = list[index];
    swap(list[left], list[index]); // 피벗과 left 자리 바꿈

    int i = left + 1; // 왼쪽에서 출발하여 큰 값을 찾아가는 처음 위치
    int j = right;    // 오른쪽에서 시작하여 작은 값을 찾아가는 위치
    int k = left + 1; // 중복된 피벗을 처리하기 위한 변수

    while (k <= j)
    {
        if (list[k] < pivot)
        {
            // list[k]가 피벗보다 작으면 왼쪽부분에 위치시킴
            swap(list[i], list[k]);
            // 위치를 바꾼후 i를 오른쪽으로 이동
            i++; // 작은 값의 끝
            k++; // 다음 요소 확인을 위해 한 칸 이동
        }
        else if (list[k] > pivot)
        {
            // list[k]가 피벗보다 크면 오른쪽부분에 위치시킴
            swap(list[k], list[j]);
            j--; // j는 큰 값의 시작
        }
        else
            // list[k]가 피벗과 같으면 중복된 피벗이라 그대로 둠
            k++; // 중복된 피벗을 건너뛰고 다음 원소 검사
    }

    swap(list[left], list[j]); // 피벗을 올바른 위치로 이동

    Random_Quick_Sort(list, left, j - 1);  // 왼쪽 부분 정렬
    Random_Quick_Sort(list, j + 1, right); // 오른쪽 부분 정렬
}

// Median 방식 피벗 선택 (3-way partitioning 적용)
void Median_Quick_Sort(std::vector<int> &list, int left, int right)
{
    if (left >= right)
        return;

    medianPivotCount++;                                                                                            // 피벗이 선택된 횟수 증가
    int mid = left + (right - left) / 2;                                                                           // 벡터의 중앙 인덱스를 찾음
    int pivot = std::max(std::min(list[left], list[mid]), std::min(std::max(list[left], list[mid]), list[right])); // 피벗 선정 방식
    // left mid right 세 값중 하나를 피벗으로 선정

    int index = (list[left] == pivot) ? left : (list[mid] == pivot ? mid : right); // 피벗의 인덱스
    swap(list[left], list[index]);                                                 // 피벗과 left 자리 바꿈

    int i = left + 1; // 왼쪽에서 출발하여 큰 값을 찾아가는 처음 위치
    int j = right;    // 오른쪽에서 시작하여 작은 값을 찾아가는 위치
    int k = left + 1;

    while (k <= j)
    {
        if (list[k] < pivot)
        {
            swap(list[i], list[k]);
            i++;
            k++;
        }
        else if (list[k] > pivot)
        {
            swap(list[k], list[j]);
            j--;
        }
        else
            k++;
    }

    swap(list[left], list[j]); // 피벗을 올바른 위치로 이동

    Median_Quick_Sort(list, left, j - 1);  // 왼쪽 부분 정렬
    Median_Quick_Sort(list, j + 1, right); // 오른쪽 부분 정렬
}

int main()
{
    srand(time(0));
    std::ifstream file("daegu_weather_2024.txt");
    std::vector<int> temperature; // 각 날씨의 온도들을 담을 벡터
    std::string date;
    int temp;

    while (file >> date >> temp)
    {
        temperature.push_back(temp);
    }
    file.close();

    std::vector<int> tempRandom = temperature;
    std::vector<int> tempMedian = temperature;

    Random_Quick_Sort(tempRandom, 0, tempRandom.size() - 1);
    Median_Quick_Sort(tempMedian, 0, tempMedian.size() - 1);

    std::cout << "랜덤 피벗 선택 횟수: " << randomPivotCount << std::endl;
    std::cout << "중앙값 피벗 선택 횟수: " << medianPivotCount << std::endl;

    return 0;
}