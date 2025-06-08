#include <iostream>
#include <vector>
#include <set>
#include <ctime>

using namespace std;

const int MIN_VERTICES = 10; // 정점의 최소 갯수
const int MAX_VERTICES = 20; // 정점의 최대 갯수
const int MIN_EDGES = 20;    // 간선의 최소 갯수
const int MAX_EDGES = 30;    // 간선의 최대 갯수
const int NUM_TEST = 10;     // 테스트 횟수

// 무작위 정수 생성
int randomInt(int minVal, int maxVal)
{
    return rand() % (maxVal - minVal + 1) + minVal;
}

// 무방향 무가중치 그래프 생성
vector<vector<int>> GenerateGraph(int &V, int &E)
{
    V = randomInt(MIN_VERTICES, MAX_VERTICES); // 정점의 수를 랜덤으로 확정
    E = randomInt(MIN_EDGES, MAX_EDGES);       // 간선의 수를 랜덤으로 확정

    vector<vector<int>> graph(V); // 그래프 초기화
    set<pair<int, int>> edgeSet;  // 간선을 저장하기 위한 Set

    // 간선의 수만큼 반복
    while (edgeSet.size() < (int)E)
    {
        int u = randomInt(0, V - 1); // 정점 u선택
        int v = randomInt(0, V - 1); // 정점 v선택
        if (u != v)
        { // 자기 자신으로의 간선 제외
            // Undirected Graph라 작은쪽을 first로 저장해 중복 처리
            pair<int, int> edge = (u < v) ? make_pair(u, v) : make_pair(v, u);
            if (edgeSet.find(edge) == edgeSet.end()) // 동일한 간선이 추가되지 않았을시
            {
                edgeSet.insert(edge);  // 간선을 Set에 추가
                graph[u].push_back(v); // 그래프에 간선 추가
                graph[v].push_back(u);
            }
        }
    }

    return graph;
}

// Vertex Priority Selection
int vertexPriorityCover(const vector<vector<int>> &graph)
{
    int V = graph.size();
    vector<bool> covered(V, false); // 정점이 커버되었는지 여부
    vector<bool> edgeCovered(V * V, false);
    int coverSize = 0;

    set<pair<int, int>> remainingEdges; // 아직 커버되지 않은 간선 집합

    // 모든 간선을 집합에 추가
    for (int u = 0; u < V; ++u)
    {
        for (int v : graph[u])
        {
            if (u < v)
                remainingEdges.insert({u, v});
        }
    }

    while (!remainingEdges.empty())
    {
        // 각 정점의 degree 계산
        vector<int> degrees(V, 0);
        for (auto &e : remainingEdges)
        {
            degrees[e.first]++;
            degrees[e.second]++;
        }

        // 최대 degree 정점 선택
        int maxDeg = -1, selected = -1;
        for (int i = 0; i < V; ++i)
        {
            if (!covered[i] && degrees[i] > maxDeg)
            {
                maxDeg = degrees[i];
                selected = i;
            }
        }

        // 해당 정점을 커버에 추가
        covered[selected] = true;
        coverSize++;

        // 선택된 정점이 포함된 간선 제거
        for (auto it = remainingEdges.begin(); it != remainingEdges.end();)
        {
            if (it->first == selected || it->second == selected)
                it = remainingEdges.erase(it);
            else
                ++it;
        }
    }

    return coverSize;
}

// Maximal Matching
int maximalMatchingCover(const vector<vector<int>> &graph)
{
    int V = graph.size();
    vector<bool> used(V, false); // 정점이 matching에 사용되었는지 여부
    set<pair<int, int>> remainingEdges;

    // 모든 간선을 집합에 추가
    for (int u = 0; u < V; ++u)
    {
        for (int v : graph[u])
        {
            if (u < v)
                remainingEdges.insert({u, v});
        }
    }

    set<int> coverSet;

    while (!remainingEdges.empty())
    {
        auto edge = *remainingEdges.begin(); // 하나의 간선을 선택
        remainingEdges.erase(edge);
        int u = edge.first;
        int v = edge.second;

        // 두 정점 모두 아직 사용되지 않은 경우만 선택
        if (!used[u] && !used[v])
        {
            used[u] = used[v] = true; // 둘다 사용 처리
            coverSet.insert(u);
            coverSet.insert(v);

            // 이 정점들과 연결된 간선 제거
            for (auto it = remainingEdges.begin(); it != remainingEdges.end();)
            {
                if (it->first == u || it->second == u || it->first == v || it->second == v)
                    it = remainingEdges.erase(it);
                else
                    ++it;
            }
        }
    }

    return coverSet.size();
}

// 실험 실행
int main()
{
    srand(time(0));

    int totalVPC = 0; // Vertex Priority Selection 방식 결과
    int totalMMC = 0; // Maximal Matching 방식 결과 누적합

    for (int i = 0; i < NUM_TEST; ++i)
    {
        int V, E;
        auto graph = GenerateGraph(V, E);
        int vpc = vertexPriorityCover(graph);
        int mmc = maximalMatchingCover(graph);

        cout << "[Test " << i + 1 << "] Vertex Count: " << V << ", Edge Count: " << E << "\n";
        cout << "  Vertex Priority Cover size: " << vpc << "\n";
        cout << "  Maximal Matching Cover size: " << mmc << "\n\n";

        totalVPC += vpc;
        totalMMC += mmc;
    }

    cout << "=== 평균 결과 ===\n";
    cout << "Vertex Priority Cover 평균 크기: " << totalVPC / (double)NUM_TEST << "\n";
    cout << "Maximal Matching Cover 평균 크기: " << totalMMC / (double)NUM_TEST << "\n";

    if (totalVPC < totalMMC)
        cout << "Vertex Priority 방식이 평균적으로 더 우수.\n";
    else if (totalVPC > totalMMC)
        cout << "Maximal Matching 방식이 평균적으로 더 우수.\n";
    else
        cout << "두 방식이 평균적으로 동일.\n";

    return 0;
}