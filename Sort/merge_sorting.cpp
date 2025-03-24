#include <iostream>
#include <vector>

void Merge(std::vector<int> &arr, int start, int mid, int end)
{
    int leftL = mid - start + 1; // 앞 부분 벡터의 크기
    int rightL = end - mid;      // 뒷 부분 벡터의 크기

    std::vector<int> Left(leftL), Right(rightL); // 앞 부분과 뒷 부분을 저장할 임시 벡터

    // 원본 벡터들의 원소들을 복사
    for (int i = 0; i < leftL; i++)
        Left[i] = arr[start + i]; // arr[start ~ mid]의 원소들을 복사
    for (int j = 0; j < rightL; j++)
        Right[j] = arr[mid + 1 + j]; // arr[mid+1 ~ end]의 원소들을 복사

    int i = 0, j = 0; // Left,Right의 인덱스
    int p = start;    // 원본 벡터에서 병합된 데이터를 저장할 위치

    while (i < leftL && j < rightL)
    {
        // 더 작은 값을 원본 벡터에 복사
        if (Left[i] <= Right[j])
        {
            arr[p] = Left[i];
            i++;
        }
        else
        {
            arr[p] = Right[j];
            j++;
        }
        p++;
    }
    // 한쪽 배열이 끝날 경우

    // 아직 남아있는 원소들을 그대로 원본 벡터에 복사
    while (i < leftL)
    {
        arr[p] = Left[i];
        i++;
        p++;
    }
    while (j < rightL)
    {
        arr[p] = Right[j];
        j++;
        p++;
    }
}

// 시작과 끝 index를 받아줌
void MergeSort(std::vector<int> &arr, int start, int end)
{

    if (start < end)
    {
        int mid = (start + end) / 2;  // 반으로 나누기 위한 middle index
        MergeSort(arr, start, mid);   // 앞부분 Recursive call
        MergeSort(arr, mid + 1, end); // 뒷부분 Recursive call
        Merge(arr, start, mid, end);  // 앞부분과 뒷부분을 합병
    }
}

// 벡터 출력 함수
void PrintVector(std::vector<int> &arr)
{
    for (int i = 0; i < arr.size(); i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
}

int main()
{
    std::vector<int> arr = {37, 10, 22, 30, 35, 13, 25, 24};

    std::cout << "Unordered vector: ";
    PrintVector(arr);

    MergeSort(arr, 0, arr.size() - 1);

    std::cout << "Merge Sort: ";
    PrintVector(arr);
    return 0;
