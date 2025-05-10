#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>

using namespace std;

const int VERTEX_COUNT = 10; // 정점의 수
const int INF = 10000;       // 무한의 가중치

// 간선 구조체 정의
struct Edge
{
    int to,     // 도착 정점
        weight; // 가중치
};

// 인접 리스트 방식 사용
vector<vector<Edge>> adj(VERTEX_COUNT + 1);

// 인접 리스트를 인접 행렬로 변환
void convertToMatrix(vector<vector<int>> &matrix)
{
    matrix.assign(VERTEX_COUNT + 1, vector<int>(VERTEX_COUNT + 1, INF));
    for (int i = 1; i <= VERTEX_COUNT; ++i)
    {
        matrix[i][i] = 0;
        for (const auto &edge : adj[i])
        {
            matrix[i][edge.to] = edge.weight;
        }
    }
}

// connected directed weighted graph를 생성
void generateRandomConnectedGraph()
{
    srand((unsigned int)time(0));
    adj.assign(VERTEX_COUNT + 1, vector<Edge>());

    // 연결성 보장을 위한 트리 형태 간선 추가
    for (int i = 2; i <= VERTEX_COUNT; ++i)
    {
        int from = rand() % (i - 1) + 1;
        int weight = rand() % 20 + 1;
        adj[from].push_back({i, weight}); // 방향성 있음
    }

    // 추가 간선
    int extraEdges = rand() % 10 + 10;
    for (int i = 0; i < extraEdges; ++i)
    {
        int u = rand() % VERTEX_COUNT + 1;
        int v = rand() % VERTEX_COUNT + 1;
        if (u != v)
        {
            bool exists = false;
            for (const auto &e : adj[u])
            {
                if (e.to == v)
                {
                    exists = true;
                    break;
                }
            }
            if (!exists)
            {
                int weight = rand() % 20 + 1;
                adj[u].push_back({v, weight}); // 방향성 유지
            }
        }
    }
}

// 다익스트라 알고리즘
void dijkstra(int start, vector<int> &dist)
{
    dist.assign(VERTEX_COUNT + 1, INF);
    dist[start] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, start});

    while (!pq.empty())
    {
        pair<int, int> current = pq.top();
        pq.pop();
        int cost = current.first;
        int u = current.second;
        if (cost > dist[u])
            continue;

        for (const auto &edge : adj[u])
        {
            int v = edge.to, w = edge.weight;
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
}

// Floyd 알고리즘
void Floyd(vector<vector<int>> &matrix)
{
    for (int k = 1; k <= VERTEX_COUNT; ++k)
    {
        for (int i = 1; i <= VERTEX_COUNT; ++i)
        {
            for (int j = 1; j <= VERTEX_COUNT; ++j)
            {
                if (matrix[i][k] < INF && matrix[k][j] < INF)
                {
                    matrix[i][j] = min(matrix[i][j], matrix[i][k] + matrix[k][j]);
                }
            }
        }
    }
}

// 메인 함수
int main()
{
    generateRandomConnectedGraph(); // 랜덤 그래프 생성

    float totalTime = 0.0;

    // Dijkstra
    {
        // 모든 정점을 시작점으로 수행
        for (int start = 1; start <= VERTEX_COUNT; ++start)
        {
            vector<int> dist;

            auto begin = chrono::high_resolution_clock::now();

            dijkstra(start, dist);

            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> duration = end - begin;

            totalTime += duration.count();

            cout << "시작 정점: " << start << ", 실행 시간: " << duration.count() << " ms" << endl;
        }

        cout << "Dijkstra 전체 실행 시간 합계: " << totalTime << " ms" << endl;
    }

    // Floyd
    {
        vector<vector<int>> matrix;
        auto floydStart = chrono::high_resolution_clock::now();

        convertToMatrix(matrix);
        Floyd(matrix);

        auto floydEnd = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> floydDuration = floydEnd - floydStart;

        cout << "Floyd 전체 실행 시간: " << floydDuration.count() << " ms\n";
    }

    return 0;
}
