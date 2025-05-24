#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

// Edit Distance 알고리즘
int editDistance(const string &str1, const string &str2)
{
    int m = str1.length();
    int n = str2.length();

    // DP table Init
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    // 초기 상태 설정
    for (int i = 0; i <= m; i++)
        dp[i][0] = i; // 삭제
    for (int j = 0; j <= n; j++)
        dp[0][j] = j; // 삽입

    // DP Table 채우기
    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (str1[i - 1] == str2[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1]; // 문자가 같으면 그대로
            }
            else
            {
                dp[i][j] = min({
                    dp[i - 1][j] + 1,    // 삭제
                    dp[i][j - 1] + 1,    // 삽입
                    dp[i - 1][j - 1] + 1 // 치환
                });
            }
        }
    }

    return dp[m][n];
}

int main()
{
    string input;
    cout << "사용자 입력: ";
    cin >> input;

    // 사전 단어 목록
    string diction[] = {"definitely", "defiantly", "define"};

    string bestWord;
    int minDistance = numeric_limits<int>::max();

    // 모든 사전에 대해 Edit Distance 계산
    for (int i = 0; i < _countof(diction); i++)
    {
        int dist = editDistance(input, diction[i]);
        cout << " -> " << diction[i] << " (Edit Distance: " << dist << ")\n";

        if (dist < minDistance)
        {
            minDistance = dist;
            bestWord = diction[i];
        }
    }

    // 결과 출력
    cout << "\n(출력) " << "추천: \"" << bestWord << "\" (edit Distance: " << minDistance << ")" << endl;

    return 0;
}