#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>

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

// 무작위 연결 그래프 생성
void generateRandomConnectedGraph()
{
    srand(time(0));

    // 먼저 spanning tree로 연결
    vector<bool> visited(VERTEX_COUNT + 1, false);
    visited[1] = true; // 1번 정점부터 시작
    for (int i = 2; i <= VERTEX_COUNT; ++i)
    {
        int from = rand() % (i - 1) + 1; // 1부터 i-1 사이에서 랜덤 선택
        int weight = rand() % 20 + 1;    // 1 ~ 20 사이의 랜덤한 가중치 설정
        // 양 방향 간선 추가 (사실상 undirected)
        adj[from].push_back({i, weight});
        adj[i].push_back({from, weight});
    }

    // 랜덤하게 추가 edge를 넣기
    int extraEdges = rand() % 10 + 5; // 추가 간선 수
    for (int i = 0; i < extraEdges; ++i)
    {
        int u = rand() % VERTEX_COUNT + 1;
        int v = rand() % VERTEX_COUNT + 1;
        if (u != v)
        {
            // 중복 간선 방지
            bool exists = false;
            for (const auto &e : adj[u])
            {
                if (e.to == v)
                {
                    exists = true;
                    break;
                }
            }
            // 존재하지 않는 경우에만 추가
            if (!exists)
            {
                int weight = rand() % 10 + 1;
                adj[u].push_back({v, weight});
                adj[v].push_back({u, weight});
            }
        }
    }
}

// 다익스트라 알고리즘
void dijkstra(int start, vector<int> &dist, vector<int> &prev)
{
    dist.assign(VERTEX_COUNT + 1, INF); // 거리 배열 초기화
    prev.assign(VERTEX_COUNT + 1, -1);  // 이전 정점의 정보 초기화
    dist[start] = 0;                    // 시작 정점까지 거리를 0으로 초기화

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq; // 우선순위 큐 사용
    pq.push({0, start});                                                  // 시작 정점 큐에 추가

    // 우선순위 큐로 최소 비용 정점이 큐에서 먼저 나오도록 함
    while (!pq.empty())
    {
        pair<int, int> current = pq.top();
        pq.pop();
        int cost = current.first;
        int u = current.second;

        // 현재 경로가 더 오래된 정보면 스킵
        if (cost > dist[u])
            continue;

        for (const auto &edge : adj[u])
        {
            int v = edge.to;
            int w = edge.weight;
            // 더 짧은 경로를 찾았을 경우 갱신
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

// 경로 출력
void printPaths(int start, const vector<int> &dist, const vector<int> &prev)
{
    for (int v = 1; v <= VERTEX_COUNT; ++v)
    {
        if (v == start)
            continue; // 자기 자신은 제외

        // 경로 추적을 위한 리스트
        vector<int> path;
        for (int cur = v; cur != -1; cur = prev[cur])
        {
            path.push_back(cur);
        }
        reverse(path.begin(), path.end()); // 거꾸로 저장되었기에 reverse

        for (size_t i = 0; i < path.size(); ++i)
        {
            cout << path[i];
            if (i != path.size() - 1)
                cout << " -> ";
        }
        cout << " (Cost: " << dist[v] << ")\n";
    }
}

// 메인 함수
int main()
{
    generateRandomConnectedGraph(); // 랜덤 그래프 생성

    int start = 1; // 시작점
    vector<int> dist, prev;

    dijkstra(start, dist, prev);   // 다익스트라 알고리즘 수행
    printPaths(start, dist, prev); // 출력

    return 0;
}