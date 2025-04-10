#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <set>
#include <algorithm>

using namespace std;

const int VERTEX_COUNT = 10; // 정점의 수
const int MAX_WEIGHT = 20;   // 최대 가중치

// 간선 구조체 정의
struct Edge
{
    int u, v, weight;
    bool operator<(const Edge &other) const
    {
        return weight < other.weight;
    }
};

// 유니온 파인드 클래스 (그래프 연결성 보장)
class Union
{
    vector<int> parent;

public:
    Union(int n) : parent(n)
    {
        for (int i = 0; i < n; i++)
            parent[i] = -1; // 그래프 초기화
    }

    int set_find(int x)
    {
        if (parent[x] == -1)
            return x;
        while (parent[x] != -1)
            x = parent[x];
        return x;
        // return parent[x] = find(parent[x]);
    }

    bool set_union(int x, int y)
    {
        int rx = set_find(x);
        int ry = set_find(y);
        if (rx != ry)
        {
            parent[rx] = ry;
            return true;
        }
        return false;
    }
};

// 간선 추가 함수 (undirect)
void addEdge(vector<vector<int>> &matrix, int u, int v, int weight)
{
    matrix[u][v] = weight;
    matrix[v][u] = weight;
}

// 연결 그래프 생성 (인접 행렬)
vector<vector<int>> generateConnectedGraph(int vertexCount)
{
    cout << "Random Matrix Generation!!\n";

    vector<vector<int>> matrix(vertexCount, vector<int>(vertexCount, 0));
    Union uf(vertexCount);
    set<pair<int, int>> used_edges; // 두개의 벡터를 쌍으로 처리하기 위해 pair사용

    int i = 1;
    int edge_count = 0;
    while (edge_count < vertexCount - 1)
    {
        int u = rand() % vertexCount;
        int v = rand() % vertexCount;

        if (u == v || used_edges.count({min(u, v), max(u, v)}))
            continue;

        if (uf.set_union(u, v))
        {
            int weight = rand() % MAX_WEIGHT + 1;
            addEdge(matrix, u, v, weight);
            used_edges.insert({min(u, v), max(u, v)});

            cout << i << ". random edge: " << "(" << u << "," << v << "), " << "(" << v << "," << u << "), weight: " << weight << "\n";

            i++;
            edge_count++;
        }
    }

    cout << endl;
    cout << "Number of Vertices: " << vertexCount << endl;
    cout << "Number of Edge: " << edge_count << "\n"
         << endl;

    return matrix;
}

// Kruskal 알고리즘 구현
vector<Edge> kruskalMST(const vector<vector<int>> &matrix)
{
    int n = matrix.size();
    vector<Edge> edges;

    // 인접 행렬에서 간선 리스트 추출 (중복 방지)
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            if (matrix[i][j] > 0)
            {
                edges.push_back({i, j, matrix[i][j]});
            }
        }
    }

    // 가중치 기준으로 정렬
    sort(edges.begin(), edges.end());

    // MST 구성
    Union uf(n);
    vector<Edge> mst;

    for (const auto &edge : edges)
    {
        if (uf.set_union(edge.u, edge.v))
        {
            mst.push_back(edge);
        }
        else
        {
            // 사이클이 생길경우 간선 출력
            cout << "Edge (" << edge.u << ", " << edge.v << ") with weight "
                 << edge.weight << " is excluded because it creates a cycle.\n";
        }
    }

    return mst;
}

// 인접 행렬 출력
void print_Graph(const vector<vector<int>> &matrix)
{
    cout << "Adjacency Matrix:\n";
    for (int i = 0; i < matrix.size(); ++i)
    {
        for (int j = 0; j < matrix[i].size(); ++j)
        {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

// MST 출력 함수
void print_MST(const vector<Edge> &mst)
{
    cout << "\nKruskal MST Result:\n";
    int totalWeight = 0;
    for (const auto &edge : mst)
    {
        cout << edge.u << " - " << edge.v << " (weight: " << edge.weight << ")\n";
        totalWeight += edge.weight;
    }
    cout << "Total Weight of MST: " << totalWeight << "\n";
}

// 메인 함수
int main()
{
    srand(time(0));

    vector<vector<int>> graph = generateConnectedGraph(VERTEX_COUNT);
    print_Graph(graph);

    // Kruskal 알고리즘 적용
    vector<Edge> mst = kruskalMST(graph);
    print_MST(mst);

    return 0;
}