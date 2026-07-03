class Solution {
public:
    int findMaxPathScore(vector<vector<int>>& edges, vector<bool>& online, long long k) {
        int n = online.size();

        vector<vector<pair<int, int>>> graph(n);
        vector<int> indegree(n, 0);

        int high = 0;
        for (auto &e : edges) {
            graph[e[0]].push_back({e[1], e[2]});
            indegree[e[1]]++;
            high = max(high, e[2]);
        }

        // Topological order
        queue<int> q;
        vector<int> topo;
        vector<int> deg = indegree;

        for (int i = 0; i < n; i++)
            if (deg[i] == 0)
                q.push(i);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            topo.push_back(u);

            for (auto &[v, w] : graph[u]) {
                if (--deg[v] == 0)
                    q.push(v);
            }
        }

        auto check = [&](int mid) {
            const long long INF = 4e18;
            vector<long long> dist(n, INF);
            dist[0] = 0;

            for (int u : topo) {
                if (dist[u] == INF) continue;

                for (auto &[v, w] : graph[u]) {
                    if (w < mid) continue;

                    if (v != n - 1 && !online[v]) continue;

                    if (dist[u] + w < dist[v] && dist[u] + w <= k)
                        dist[v] = dist[u] + w;
                }
            }

            return dist[n - 1] <= k;
        };

        int low = 0, ans = -1;

        while (low <= high) {
            int mid = low + (high - low) / 2;

            if (check(mid)) {
                ans = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        return ans;
    }
};