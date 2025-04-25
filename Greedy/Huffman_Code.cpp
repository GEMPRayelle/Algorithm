#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <string>

using namespace std;

// 허프만 트리의 노드 구조체
struct Node
{
    char c;             // 문자
    int frequency;      // 빈도수
    Node *left, *right; // 왼쪽 자식노드

    Node(char c, int f) : c(c), frequency(f), left(nullptr), right(nullptr) {}
};

// 우선순위 큐에서 빈도수가 낮은 순서대로 정렬하기 위한 비교 함수
struct Compare
{
    bool operator()(Node *a, Node *b)
    {
        return a->frequency > b->frequency; // 빈도수 오름차순 정렬
    }
};

void buildFrequencyTable(const string &filename, unordered_map<char, int> &freq)
{
    ifstream file(filename);
    char ch;
    while (file.get(ch))
    {
        if (ch == '\n' || ch == '\0' || ch == ' ')
            continue; // <- 여기에 걸러냄
        freq[ch]++;
    }
    file.close();
}

// 허프만 트리를 생성
Node *buildHuffmanTree(const unordered_map<char, int> &frequency)
{
    priority_queue<Node *, vector<Node *>, Compare> pq;

    // 각 문자를 노드로 변환하여 우선순위 큐에 삽입
    for (auto &pair : frequency)
    {
        pq.push(new Node(pair.first, pair.second));
    }

    // 큐에서 두개의 가장 노드를 꺼내어 하나의 부모 노드로 병합
    while (pq.size() > 1)
    {
        Node *l = pq.top();
        pq.pop();
        Node *r = pq.top();
        pq.pop();
        Node *merged = new Node('\0', l->frequency + r->frequency); // 내부 노드는 문자 없음
        merged->left = l;
        merged->right = r;
        pq.push(merged);
    }

    return pq.top(); // 루트 노트 반환
}

// 허프만 코드 생성
void generateCodes(Node *root, const string &code, unordered_map<char, string> &huffmanCode)
{
    if (!root)
        return;

    // 단말 노드일 경우 해당 문자에 대한 허프만 코드 저장
    if (!root->left && !root->right)
        huffmanCode[root->c] = code;

    // 왼쪽은 0 오른쪽은 1로 지정
    generateCodes(root->left, code + "0", huffmanCode);
    generateCodes(root->right, code + "1", huffmanCode);
}

// 원본 txt파일을 허프만 코드로 인코딩
void encodeFile(const string &inFile, const string &outFile, const unordered_map<char, string> &huffmanCode)
{
    ifstream in(inFile);
    ofstream out(outFile, ios::binary);
    string bitString;
    char c;

    // 파일을 문자 단위로 읽어서 해당 허프만 코드를 이어 붙임
    while (in.get(c))
    {
        if (c == ' ' || c == '\n' || c == '\0')
            continue; // 제외
        bitString += huffmanCode.at(c);
    }

    // 8의 배수를 기준으로 padding추가
    while (bitString.size() % 8 != 0)
        bitString += '0'; // padding

    // 8비트 단위로 나눠서 바이너리 파일로 저장
    for (size_t i = 0; i < bitString.size(); i += 8)
    {
        bitset<8> bits(bitString.substr(i, 8));
        out.put(static_cast<char>(bits.to_ulong()));
    }

    in.close();
    out.close();
}

// 인코딩된 파일을 디코딩하여 복원
void decodeFile(const string &encFile, const string &decFile, Node *root)
{
    ifstream in(encFile, ios::binary);
    ofstream out(decFile);
    string bitString;
    char byte;

    // 파일을 바이트단위로 읽음
    while (in.get(byte))
    {
        bitString += bitset<8>(byte).to_string();
    }

    // 허프만 트리를 따라가며 bit를 디코딩
    Node *current = root;
    for (char bit : bitString)
    {
        current = (bit == '0') ? current->left : current->right;
        if (!current->left && !current->right)
        {
            out.put(current->c); // 단말 노드 도착시 문자 출력
            current = root;      // 루트로 귀환
        }
    }

    in.close();
    out.close();
}

// 두 파일이 같은지를 비교
bool filesAreEqual(const string &file1, const string &file2)
{
    ifstream f1(file1), f2(file2);
    char c1, c2;

    while (true)
    {
        // f1에서 특수문자 제외
        while (f1.get(c1) && (c1 == ' ' || c1 == '\n' || c1 == '\0'))
            ;
        // f2는 인코딩-디코딩된 파일이므로 특수문자 없음
        while (f2.get(c2) && (c2 == ' ' || c2 == '\n' || c2 == '\0'))
            ;

        if (f1.eof() && f2.eof())
            return true;
        if (f1.eof() || f2.eof() || c1 != c2)
            return false;
    }
}

int main()
{
    unordered_map<char, int> frequency;      // 문자별 빈도수
    unordered_map<char, string> huffmanCode; // 문자별 허프만 코드
    int totalChars = 0;

    buildFrequencyTable("sample.txt", frequency); // txt파일에 빈도수 계산

    cout << "문자 빈도수:\n";
    for (auto &p : frequency)
    {
        if (p.first == '\n')
            cout << "\\n: ";
        else if (p.first == ' ')
            cout << "' ': ";
        else
            cout << p.first << ": ";
        cout << p.second << '\n';
    }

    Node *root = buildHuffmanTree(frequency); // 허프만 트리 생성
    generateCodes(root, "", huffmanCode);     // 허프만 코드 생성

    encodeFile("sample.txt", "sample.enc", huffmanCode);
    decodeFile("sample.enc", "sample.dec", root);

    // 원본 파일과 디코딩 파일 비교
    cout << "\n압축 성공 여부: " << (filesAreEqual("sample.txt", "sample.dec") ? "일치" : "불일치") << '\n';

    // 압축률 계산
    int originalBits = 0;
    int compressedBits = 0;
    for (auto &p : frequency)
    {
        originalBits += p.second * 8;                             // ASCII = 8bit
        compressedBits += p.second * huffmanCode[p.first].size(); // 허프만 코드 길이
    }

    double ratio = 100.0 * (originalBits - compressedBits) / originalBits;
    cout << "원래 크기: " << originalBits << " bits\n";
    cout << "압축된 크기: " << compressedBits << " bits\n";
    cout << "압축률: " << ratio << "%\n";

    return 0;
}