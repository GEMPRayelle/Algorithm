#include <iostream>
#include <vector>

// 스왑 함수
void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

// 퀵 정렬을 수행하는 함수
void Quick_Sort(int list[], int left, int right)
{
    if (left >= right)
        return;

    pivotCount++;           // 피벗이 선택된 횟수 증가
    int pivot = list[left]; // 피벗 설정
    int i = left + 1;       // 왼쪽에서 출발하여 큰 값을 찾아가는 처음 위치
    int j = right;          // 오른쪽에서 시작하여 작은 값을 찾아가는 위치

    while (i <= j)
    {
        // i j가 서로 엇갈릴때까지 반복
        while (i <= right && list[i] < pivot)
            i++; // i가 피벗보다 크거나 같은 값을 찾을때까지 i를 증가
        while (j >= left && list[j] > pivot)
            j--; // j가 피벗보다 작거나 같은 값을 찾을때까지 j를 증가
        if (i <= j)
        {
            // i, j가 유효한 범위 내에 있으면 교환
            swap(list[i], list[j]);
            i++;
            j--; // 다음 위치 이동
        }
    }
    swap(list[left], list[j]); // 피벗을 올바른 위치로 이동

    Quick_Sort(list, left, j - 1);  // 왼쪽 부분 정렬
    Quick_Sort(list, j + 1, right); // 오른쪽 부분 정렬
}

int main()
{
    int data[12] = {6, 3, 11, 9, 12, 2, 8, 15, 18, 10, 7, 14}; // 시작 배열

    Quick_Sort(data, 0, sizeof(data) / sizeof(*data) - 1);

    for (int i = 0; i < sizeof(data) / sizeof(*data); i++)
        std::cout << data[i] << ' ';

    std::cout << std::endl;
    std::cout << "총 피벗이 선택된 횟수: " << pivotCount << std::endl;

    return 0;
}