#include <iostream>
#include <vector>
#include <map>

using namespace std;

int main()
{
    int W = 10; // 최대 무게
    map<int, pair<int, int>> items = {
        // map의 key는 아이템 순서이고 value는 pair에 무게당 가치를 저장
        {1, {5, 10}},
        {2, {4, 40}},
        {3, {6, 30}},
        {4, {3, 50}}};

    int N = items.size();                                 // 아이템의 개수
    vector<vector<int>> dp(N + 1, vector<int>(W + 1, 0)); // DP 테이블
    vector<int> keys;                                     // 키 리스트를 따로 만들어서 인덱싱에 사용
    for (auto &it : items)
        keys.push_back(it.first); // 아이템 번호 리스트

    // DP 테이블 채우기
    for (int i = 1; i <= N; i++)
    {
        int idx = keys[i - 1];
        int wt = items[idx].first;
        int val = items[idx].second;

        for (int w = 0; w <= W; w++)
        {
            if (wt <= w)
                dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - wt] + val);
            else
                dp[i][w] = dp[i - 1][w];
        }
    }

    // 결과 출력
    cout << "1-1. 최대 가치: " << dp[N][W] << "만원" << endl;

    // 선택된 아이템 출력
    cout << "1-2. 선택된 아이템: ";
    int w = W;
    for (int i = N; i >= 1; i--)
    {
        if (dp[i][w] != dp[i - 1][w])
        {
            int idx = keys[i - 1];
            cout << "Item " << idx << " ";
            w -= items[idx].first;
        }
    }
    cout << endl;

    return 0;
}
