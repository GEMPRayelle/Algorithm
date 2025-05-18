#include <iostream>
#include <climits>
using namespace std;

const int N = 4; // 4개의 행렬
int m[N][N];     // 행렬 곱셈 최소 연산 수 저장할 배열
int s[N][N];     // m[N][N]을 최소화하는 k값 저장하는 배열 (최적의 분할 위치)

// 곱셈 순서를 출력
void printOptimalParens(int i, int j)
{
    if (i == j)
        cout << "A" << i + 1; // 행렬이 하나일 경우
    else
    {
        cout << "(";
        printOptimalParens(i, s[i][j]);
        cout << " x ";
        printOptimalParens(s[i][j] + 1, j);
        cout << ")";
    }
}

int main()
{
    // 행렬 저장: A1(10x20), A2(20x5), A3(5x15), A4(15x30)
    int p[N + 1] = {10, 20, 5, 15, 30};

    // initialization
    for (int i = 0; i < N; i++)
        m[i][i] = 0;
    // 체인 길이 l
    for (int l = 2; l <= N; l++)
    {
        for (int i = 0; i <= N - l; i++)
        {
            int j = i + l - 1;
            m[i][j] = INT_MAX; // 무한대로 초기화
            for (int k = i; k < j; k++)
            {
                // 연산 수 = 왼쪽 + 오른쪽 + 현재 곱셈
                int cost = m[i][k] + m[k + 1][j] + p[i] * p[k + 1] * p[j + 1];
                if (cost < m[i][j])
                {
                    m[i][j] = cost; // 갱신
                    s[i][j] = k;    // 분할 위치 저장
                }
            }
        }
    }

    // DP 테이블 출력
    cout << "Chained Matrix Multiplication:" << endl;
    cout << "===================================" << endl;
    cout << "M\t";
    for (int j = 0; j < N; j++)
    {
        cout << j + 1 << "\t";
    }
    cout << endl;

    for (int i = 0; i < N; i++)
    {
        cout << i + 1 << "\t"; // 행
        for (int j = 0; j < N; j++)
        {
            if (i > j)
                cout << "0\t"; // 비어있는 부분은 0
            else
                cout << m[i][j] << "\t"; // 최소 연산 수
        }
        cout << endl;
    }

    // 결과 출력
    cout << "\nFinal Solution : " << m[0][N - 1] << endl;
    cout << "Implicit Order for Matrix Multiplication: ";
    printOptimalParens(0, N - 1);
    cout << endl;

    return 0;
}