#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <limits>

using namespace std;

// 집합 구조체
struct SetItem
{
    string name;       // 집합 이름
    set<int> elements; // 집합의 원소들
    int cost;          // 가중치
};

int main()
{
    // 전체 집합
    set<int> U = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Set F 입력
    vector<SetItem> sets = {
        {"S1", {1, 2, 3, 8}, 6},
        {"S2", {1, 2, 3, 4, 8}, 10},
        {"S3", {1, 2, 3, 4}, 4},
        {"S4", {2, 3, 4, 5, 7, 8}, 12},
        {"S5", {4, 5, 6, 7}, 4},
        {"S6", {5, 6, 7, 9, 10}, 8},
        {"S7", {4, 5, 6, 7}, 4},
        {"S8", {1, 2, 4, 8}, 4},
        {"S9", {6, 9}, 3},
        {"S10", {6, 10}, 4}};

    set<int> covered;            // 현재까지 커버된 원소들을 담을 set
    vector<string> selectedSets; // 선택한 집합들의 이름을 저장할 벡터
    int totalCost = 0;           // 총 가중치

    // 전체 집합을 커버할때까지 반복
    while (covered != U)
    {
        double bestRatio = -1.0; // 비용대비 효율 최대값
        int bestIndex = -1;      // 가장 좋은 집합의 인덱스

        // 모든 집합을 순회하여 가장 효율적인 집합을 찾음
        for (int i = 0; i < sets.size(); ++i)
        {
            set<int> uncovered;
            // 아직 커버되지 않은 원소 계산
            for (int e : sets[i].elements)
            {
                if (covered.find(e) == covered.end())
                {
                    uncovered.insert(e);
                }
            }

            int uncoveredCount = uncovered.size();
            if (uncoveredCount == 0)
                continue; // 새로 커버할 원소가 없음

            // 효율: 새로 커버하는 원소 수 / 가중치
            double ratio = (double)uncoveredCount / sets[i].cost;
            if (ratio > bestRatio)
            {
                bestRatio = ratio;
                bestIndex = i;
            }
        }

        if (bestIndex == -1)
            break; // 더 이상 커버할 수 없음

        // 선택된 집합 처리
        for (int e : sets[bestIndex].elements)
        {
            covered.insert(e);
        }

        // 가중치 갱신 및 선택한 집합 저장
        totalCost += sets[bestIndex].cost;
        selectedSets.push_back(sets[bestIndex].name);
    }

    // 결과 출력
    cout << "선택된 집합: ";
    for (const auto &name : selectedSets)
    {
        cout << name << " ";
    }
    cout << "\n총 비용: " << totalCost << endl;

    return 0;
}