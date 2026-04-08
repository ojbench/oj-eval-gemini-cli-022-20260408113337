#include <bits/stdc++.h>

using namespace std;

const int N = 1e4 + 10;
const int mul = 233, Mod = 998244353;

int query(int x, int y, int z);

int A[N];

vector<int> solve_5(vector<int> indices, map<vector<int>, int>& query_results) {
    vector<int> queries;
    for (auto& p : query_results) queries.push_back(p.second);
    sort(queries.begin(), queries.end());
    
    int q_min = queries.front();
    int q_max = queries.back();
    
    vector<int> sorted_vals;
    for (int S : queries) {
        if ((q_min + q_max - S) % 2 != 0) continue;
        int x3 = (q_min + q_max - S) / 2;
        int x1 = q_min - x3;
        int x5 = q_max - x3;
        if (!(x1 < x3 && x3 < x5)) continue;
        
        for (int qC : queries) {
            int x4 = qC - x1;
            if (!(x3 < x4 && x4 < x5)) continue;
            for (int qD : queries) {
                int x2 = qD - x5;
                if (!(x1 < x2 && x2 < x3)) continue;
                
                vector<int> arr = {x1, x2, x3, x4, x5};
                vector<int> gen_queries;
                for (int i=0; i<5; ++i)
                    for (int j=i+1; j<5; ++j)
                        for (int k=j+1; k<5; ++k)
                            gen_queries.push_back(max({arr[i], arr[j], arr[k]}) + min({arr[i], arr[j], arr[k]}));
                sort(gen_queries.begin(), gen_queries.end());
                if (gen_queries == queries) {
                    sorted_vals = arr;
                    break;
                }
            }
            if (!sorted_vals.empty()) break;
        }
        if (!sorted_vals.empty()) break;
    }
    
    vector<int> p = sorted_vals;
    do {
        bool valid = true;
        for (int i=0; i<5; ++i) {
            for (int j=i+1; j<5; ++j) {
                for (int k=j+1; k<5; ++k) {
                    vector<int> trip = {indices[i], indices[j], indices[k]};
                    sort(trip.begin(), trip.end());
                    int q_val = query_results[trip];
                    int expected = max({p[i], p[j], p[k]}) + min({p[i], p[j], p[k]});
                    if (q_val != expected) {
                        valid = false;
                        break;
                    }
                }
                if (!valid) break;
            }
            if (!valid) break;
        }
        if (valid) return p;
    } while (next_permutation(p.begin(), p.end()));
    
    return {};
}

int guess(int n, int Taskid) {
    memset(A, 0, sizeof A);

    vector<int> indices = {1, 2, 3, 4, 5};
    map<vector<int>, int> query_results;
    for (int i=0; i<5; ++i) {
        for (int j=i+1; j<5; ++j) {
            for (int k=j+1; k<5; ++k) {
                vector<int> trip = {indices[i], indices[j], indices[k]};
                sort(trip.begin(), trip.end());
                query_results[trip] = query(trip[0], trip[1], trip[2]);
            }
        }
    }
    
    vector<int> first_5 = solve_5(indices, query_results);
    for (int i=0; i<5; ++i) {
        A[indices[i]] = first_5[i];
    }
    
    int u = indices[0], v = indices[1];
    if (A[u] > A[v]) swap(u, v);
    for (int i=2; i<5; ++i) {
        int idx = indices[i];
        if (A[idx] < A[u]) {
            v = u;
            u = idx;
        } else if (A[idx] < A[v]) {
            v = idx;
        }
    }
    
    int M = indices[0];
    for (int i=1; i<5; ++i) {
        if (A[indices[i]] > A[M]) {
            M = indices[i];
        }
    }
    
    vector<int> remaining;
    for (int i=6; i<=n; ++i) remaining.push_back(i);
    
    // Shuffle remaining to get O(log n) expected extra queries
    mt19937 rng(1337);
    shuffle(remaining.begin(), remaining.end(), rng);
    
    for (int i : remaining) {
        int q1 = query(u, v, i);
        if (q1 > A[u] + A[v]) {
            A[i] = q1 - A[u];
            if (A[i] > A[M]) M = i;
        } else if (q1 < A[u] + A[v]) {
            A[i] = q1 - A[v];
            v = u;
            u = i;
        } else {
            int q2 = query(i, v, M);
            A[i] = q2 - A[M];
            v = i;
        }
    }

    int ret = 0;
    for (int i = n; i; i--) ret = 1ll * (ret + A[i]) * mul % Mod;
    return ret;
}
