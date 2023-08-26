#include <climits>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <vector>
using namespace std;

int T, ks = 1;
vector<string> sstr;
string str;
string resident, party, club;
map<string, int> residents;
map<string, int> parties;
map<string, int> clubs;
int t;
int n1, n2, n3;
int x, y;
vector<vector<pair<int, pair<string, string>>>> par;
vector<vector<int>> m;
vector<int> d;
int cf, maxflow;

void bfs()
{
    d.assign(t + 1, INT_MAX);
    d[0] = 0;
    queue<int> q;
    q.push(0);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v = 1; v <= t; ++v)
            if (m[u][v] > 0 && d[v] == INT_MAX)
            {
                d[v] = d[u] + 1;
                q.push(v);
            }
    }
}

int dfs(int u, int df)
{
    if (u == t)
        return df;
    for (int v = 1; v <= t; ++v)
        if (m[u][v] > 0 && d[v] == d[u] + 1)
        {
            cf = dfs(v, min(m[u][v], df));
            if (cf > 0)
            {
                m[u][v] -= cf;
                m[v][u] += cf;
                return cf;
            }
        }
    return 0;
}

void dinic()
{
    while (true)
    {
        bfs();
        if (d[t] == INT_MAX)
            return;
        while (true)
        {
            cf = dfs(0, INT_MAX);
            if (cf == 0)
                break;
            maxflow += cf;
        }
    }
}

int main()
{
    cin >> T;
    cin.ignore();
    getline(cin, str);
    while (T--)
    {
        sstr.clear();
        residents.clear();
        parties.clear();
        clubs.clear();
        while (getline(cin, str) && str != "")
        {
            sstr.push_back(str);
            stringstream ss(str);
            ss >> resident >> party;
            residents[resident] = residents.size();
            if (parties.count(party) == 0)
                parties[party] = parties.size();
            while (ss >> club)
                if (clubs.count(club) == 0)
                    clubs[club] = clubs.size() + 1;
        }
        // number
        n1 = clubs.size();
        n2 = residents.size();
        n3 = parties.size();
        t = n1 + n2 + n3 + 1;
        m.assign(t + 1, vector<int>(t + 1, 0));
        par.assign(n1 + 1, vector<pair<int, pair<string, string>>>());

        for (auto &r : residents)
            r.second += n1 + 1;
        for (auto &r : parties)
            r.second += n1 + n2 + 1;

        for (int i = 0; i < sstr.size(); ++i)
        {
            stringstream ss(sstr[i]);
            ss >> resident >> party;
            x = residents[resident];
            y = parties[party];
            m[x][y] = 1;
            m[y][t] = (n1 + 1) / 2 - 1;
            while (ss >> club)
            {
                y = clubs[club];
                m[y][x] = 1;
                m[0][y] = 1;
                par[y].push_back({x, {resident, club}});
            }
        }

        maxflow = 0;
        dinic();

        if (ks++ != 1)
            cout << endl;
        if (maxflow != n1)
            cout << "Impossible.\n";
        else
        {
            for (int i = 1; i <= n1; ++i)
                for (auto [y, c] : par[i])
                    if (m[i][y] == 0)
                    {
                        cout << c.first << ' ' << c.second << endl;
                        break;
                    }
        }
    }
    // system("pause");
    return 0;
}
