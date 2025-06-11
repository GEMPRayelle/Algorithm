#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric> //최적 종료 시간 계산을 구현하기 위해 사용
#include <iomanip>

using namespace std;

// Machine 구조체로 구현
struct Machine
{
    int id;             // 머신의 수
    int total_time = 0; // 작업 시간
    vector<int> tasks;  // 작업들 라인
};

// Optimal Solution 계산 (작업 총합 / 머신 수, 가장 큰 작업 중 더 큰 값)
int get_optimal_lower_bound(const vector<int> &tasks, int m)
{
    int total = accumulate(tasks.begin(), tasks.end(), 0);
    int max_task = *max_element(tasks.begin(), tasks.end());
    return max((total + m - 1) / m, max_task);
}

// List Scheduling
int list_scheduling(const vector<int> &tasks, int m, vector<Machine> &machines)
{
    machines = vector<Machine>(m); // m개의 머신 초기화
    for (int i = 0; i < m; ++i)
        machines[i].id = i + 1;

    // 작업을 입력 순서대로 처리
    for (int t : tasks)
    {
        // 현재까지 가장 적게 할당된 머신 찾기
        int min_idx = 0;
        for (int j = 1; j < m; ++j)
        {
            if (machines[j].total_time < machines[min_idx].total_time)
                min_idx = j;
        }
        // 해당 머신에 작업 할당
        machines[min_idx].tasks.push_back(t);
        machines[min_idx].total_time += t;
    }

    // 모든 머신 중에서 가장 늦게 끝나는 시간 구하기
    int end_time = 0;
    for (const auto &mac : machines)
        end_time = max(end_time, mac.total_time);
    return end_time;
}

// LPT Scheduling
int lpt_scheduling(vector<int> tasks, int m, vector<Machine> &machines)
{
    machines = vector<Machine>(m); // m개의 머신 초기화
    for (int i = 0; i < m; ++i)
        machines[i].id = i + 1;

    // 작업을 수행 시간 기준으로 내림차순으로 정렬 (가장 긴 작업부터 먼저 하도록)
    sort(tasks.begin(), tasks.end(), greater<int>());

    // 정렬된 작업들을 하나씩 할당
    for (int t : tasks)
    {
        // 현재까지 가장 적게 할당된 머신 찾기
        int min_idx = 0;
        for (int j = 1; j < m; ++j)
        {
            if (machines[j].total_time < machines[min_idx].total_time)
                min_idx = j;
        }
        // 해당 머신에 작업 할당
        machines[min_idx].tasks.push_back(t);
        machines[min_idx].total_time += t;
    }

    // 머신 중에서 가장 늦게 끝나는 시간 구하기
    int end_time = 0;
    for (const auto &mac : machines)
        end_time = max(end_time, mac.total_time);
    return end_time;
}

// 작업 시간 출력 함수
void print_result(const string &method, const vector<Machine> &machines, int end_time, double error_rate)
{
    cout << "=== " << method << " 결과 ===\n";
    for (const auto &mac : machines)
    {
        cout << "머신 " << mac.id << ": ";
        for (int t : mac.tasks)
            cout << t << " ";
        cout << "(총 작업 시간: " << mac.total_time << ")\n";
    }
    cout << "종료 시간: " << end_time << "\n";
    cout << "오차율: " << fixed << setprecision(2) << error_rate << "%\n\n";
}

int main()
{
    vector<int> tasks = {2, 5, 1, 7, 3, 4}; // 각 작업들의 수행시간
    int m = 3;                              // 머신의 수

    int optimal_time = get_optimal_lower_bound(tasks, m); // Optimal Solution

    vector<Machine> list_machines, lpt_machines;

    int list_end = list_scheduling(tasks, m, list_machines);
    int lpt_end = lpt_scheduling(tasks, m, lpt_machines);

    // 각 오차율 계산
    double list_error = ((double)(list_end - optimal_time) / optimal_time) * 100.0;
    double lpt_error = ((double)(lpt_end - optimal_time) / optimal_time) * 100.0;

    cout << "=== 작업 스케줄링 비교 (작업: 2, 5, 1, 7, 3, 4, 머신: 3개) ===\n";
    cout << "Optimal Solution: " << optimal_time << "\n\n";

    print_result("List Scheduling", list_machines, list_end, list_error);
    print_result("LPT Scheduling", lpt_machines, lpt_end, lpt_error);

    return 0;
}