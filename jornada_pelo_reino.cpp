#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXB = 8; 

struct Bits {
    unsigned long long b[MAXB];

    Bits() {
        memset(b, 0, sizeof(b));
    }
};

struct Event {
    ll dist;
    int r1, r2;
    Bits mask;
};

struct CompareEvent {
    bool operator()(const Event& a, const Event& b) const {
        return a.dist > b.dist;
    }
};

int R, C, N;
int totalCells;
vector<int> V, reachR, reachC;

int sizeTree;
vector<Bits> tree;

priority_queue<Event, vector<Event>, CompareEvent> pq;

int targetR, targetC;

bool hasAny(const Bits& a, const Bits& mask) {
    for (int i = 0; i < MAXB; i++) {
        if (a.b[i] & mask.b[i]) return true;
    }
    return false;
}

void pull(int node) {
    for (int i = 0; i < MAXB; i++) {
        tree[node].b[i] = tree[node * 2].b[i] | tree[node * 2 + 1].b[i];
    }
}

unsigned long long makeRangeMask(int l, int r) {
   
    if (l == 0 && r == 63) return ~0ULL;

    int len = r - l + 1;
    return ((1ULL << len) - 1ULL) << l;
}

Bits buildColumnMask(int c1, int c2) {
    Bits mask;

    int b1 = c1 / 64;
    int b2 = c2 / 64;

    for (int b = b1; b <= b2; b++) {
        int left = 0;
        int right = 63;

        if (b == b1) left = c1 % 64;
        if (b == b2) right = c2 % 64;

        mask.b[b] = makeRangeMask(left, right);
    }

    return mask;
}

void addEvent(int r, int c, ll currentDist) {
    int idx = r * C + c;

    ll newDist = currentDist + V[idx];

    int rr = reachR[idx];
    int cc = reachC[idx];

    int r1 = max(0, r - rr);
    int r2 = min(R - 1, r + rr);

    int c1 = max(0, c - cc);
    int c2 = min(C - 1, c + cc);

    Event ev;
    ev.dist = newDist;
    ev.r1 = r1;
    ev.r2 = r2;
    ev.mask = buildColumnMask(c1, c2);

    pq.push(ev);
}

void extractCells(int node, int left, int right, const Event& ev, ll& answer) {
    if (answer != -1) return;

    if (right < ev.r1 || ev.r2 < left) return;

    if (!hasAny(tree[node], ev.mask)) return;

    if (left == right) {
        int row = left;

        if (row >= R) return;

        for (int block = 0; block < MAXB; block++) {
            unsigned long long bits = tree[node].b[block] & ev.mask.b[block];

            tree[node].b[block] &= ~bits;

            while (bits) {
                unsigned long long bit = bits & -bits;
                int offset = __builtin_ctzll(bits);
                int col = block * 64 + offset;

                if (col >= C) break;

                if (row == targetR && col == targetC) {
                    answer = ev.dist;
                    return;
                }

                addEvent(row, col, ev.dist);

                bits ^= bit;
            }
        }

        return;
    }

    int mid = (left + right) / 2;

    extractCells(node * 2, left, mid, ev, answer);

    if (answer == -1) {
        extractCells(node * 2 + 1, mid + 1, right, ev, answer);
    }

    if (answer == -1) {
        pull(node);
    }
}

ll shortestPath(pair<int, int> source, pair<int, int> target) {
    int sr = source.first;
    int sc = source.second;

    targetR = target.first;
    targetC = target.second;

    if (source == target) return 0;

    while (!pq.empty()) pq.pop();

    sizeTree = 1;
    while (sizeTree < R) sizeTree *= 2;

    tree.assign(2 * sizeTree, Bits());

    Bits fullMask;

    for (int block = 0; block < MAXB; block++) {
        int startCol = block * 64;
        int remaining = C - startCol;

        if (remaining <= 0) {
            fullMask.b[block] = 0;
        } else if (remaining >= 64) {
            fullMask.b[block] = ~0ULL;
        } else {
            fullMask.b[block] = (1ULL << remaining) - 1ULL;
        }
    }

    for (int r = 0; r < R; r++) {
        tree[sizeTree + r] = fullMask;
    }

    // A origem já está visitada com custo 0
    int sourceBlock = sc / 64;
    int sourceOffset = sc % 64;
    tree[sizeTree + sr].b[sourceBlock] &= ~(1ULL << sourceOffset);

    for (int node = sizeTree - 1; node >= 1; node--) {
        pull(node);
    }

    addEvent(sr, sc, 0);

    while (!pq.empty()) {
        Event ev = pq.top();
        pq.pop();

        ll answer = -1;

        extractCells(1, 0, sizeTree - 1, ev, answer);

        if (answer != -1) {
            return answer;
        }
    }

    return -1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> R >> C >> N;

    totalCells = R * C;

    V.resize(totalCells);
    reachR.resize(totalCells);
    reachC.resize(totalCells);

    for (int i = 0; i < totalCells; i++) {
        cin >> V[i];
    }

    for (int i = 0; i < totalCells; i++) {
        cin >> reachR[i];
    }

    for (int i = 0; i < totalCells; i++) {
        cin >> reachC[i];
    }

    vector<pair<int, int>> provinces(N);

    for (int i = 0; i < N; i++) {
        int r, c;
        cin >> r >> c;

        r--;
        c--;

        provinces[i] = {r, c};
    }

    vector<ll> answers;

    for (int i = 0; i < N - 1; i++) {
        ll ans = shortestPath(provinces[i], provinces[i + 1]);
        answers.push_back(ans);
    }

    for (int i = 0; i < (int)answers.size(); i++) {
        if (i) cout << ' ';
        cout << answers[i];
    }

    cout << '\n';

    return 0;
}