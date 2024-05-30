#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <utility> 
#include <cstdlib>
#include <ctime>

using namespace std;

map<vector<vector<int>>, set<pair<vector<vector<int>>, double>>> q, r;
int goals = 0;

void init(vector<vector<int>>& arr)
{
    arr = {{1, 1, 1, 1, 1},
           {1, 0, 0, 0, 1},
           {1, 0, 2, 0, 1},
           {1, 1, 1, 0, 1},
           {1, 3, 2, 0, 1},
           {1, 0, 0, 3, 1},
           {1, 1, 1, 1, 1}};
}

bool isGoal(vector<vector<int>>& arr)
{
    for (int i = 0; i < arr.size(); i++)
    {
        for (int j = 0; j < arr[0].size(); j++)
        {
            if (arr[i][j] == 2)
                return false;
        }
    }

    return true;
}

vector<vector<int>> getChild(vector<vector<int>> arr, int fromX, int fromY, int toX, int toY) {
    switch (arr[toX][toY]) {
    case 0:
        arr[fromX][fromY] = 0;
        arr[toX][toY] = 5;
        break;

    case 1: case 3: case 4:
        arr.clear();
        break;

    case 2:
        if (fromX == toX) {
            int boxGoal = (toY - fromY) * 2 + fromY;
            switch (arr[fromX][boxGoal]) {
            case 0:
                arr[fromX][fromY] = 0;
                arr[toX][toY] = 5;
                arr[toX][boxGoal] = 2;
                break;

            case 3:
                arr[fromX][fromY] = 0;
                arr[toX][toY] = 5;
                arr[toX][boxGoal] = 4;
                break;

            case 1: case 2: case 4:
                arr.clear();
            }
        }
        else {
            int boxGoal = (toX - fromX) * 2 + fromX;
            switch (arr[boxGoal][fromY]) {
            case 0:
                arr[fromX][fromY] = 0;
                arr[toX][toY] = 5;
                arr[boxGoal][toY] = 2;
                break;

            case 3:
                arr[fromX][fromY] = 0;
                arr[toX][toY] = 5;
                arr[boxGoal][toY] = 4;
                break;

            case 1: case 2: case 4:
                arr.clear();
            }
        }
    }

    return arr;
}

vector<vector<vector<int>>> generateChildren(vector<vector<int>>& arr)
{
    int x, y;

    for (int i = 0; i < arr.size(); i++)
    {
        for (int j = 0; j < arr[0].size(); j++)
        {
            if (arr[i][j] == 5)
            {
                x = i, y = j;
                goto PersonFound;
            }
        }
    }

PersonFound:

    vector<vector<vector<int>>> children;

    children.push_back(getChild(arr, x, y, x + 1, y));
    children.push_back(getChild(arr, x, y, x - 1, y));
    children.push_back(getChild(arr, x, y, x, y + 1));
    children.push_back(getChild(arr, x, y, x, y - 1));

    for (int i = 0; i < children.size();)
    {
        if (children[i].size() == 0)
            children.erase(children.begin() + i);
        else
            i++;
    }

    return children;
}

bool isDeadLock(vector<vector<int>>& arr)
{
    for (int i = 0; i < arr.size(); i++)
    {
        for (int j = 0; j < arr[i].size(); j++)
        {
            if (arr[i][j] == 2)
            {
                if (arr[i - 1][j] == 1 || arr[i + 1][j] == 1)
                {
                    if (arr[i][j - 1] == 1 || arr[i][j + 1] == 1)
                        return true;
                }
            }
        }
    }

    return false;
}

bool equals(vector<vector<int>>& v1, vector<vector<int>>& v2) {
    if (v1.size() != v2.size()) {
        return false;
    }

    for (int i = 0; i < v1.size(); i++) {
        if (v1[i].size() != v2[i].size()) {
            return false;
        }

        for (int j = 0; j < v1[i].size(); j++) {
            if (v1[i][j] != v2[i][j]) {
                return false;
            }
        }
    }

    return true;
}

void print(vector<vector<int>>& v) {
    for (auto row : v) {
        for (int num : row) {
            cout << num << " ";
        }
        cout << endl;
    }

    cout << endl;
}

void insert_children(vector<vector<int>>& state)
{
    auto children = generateChildren(state);

    for (auto child : children)
    {
        if (isGoal(child))
        {
            goals++;
            r[state].insert({child, 100});
            q[state].insert({child, 0});
        }
        else
        {
            r[state].insert({child, 0});
            q[state].insert({child, 0});
        }
    }
}

void find_valid_states(vector<vector<int>>& state, vector<vector<vector<int>>>& valid_states)
{
    vector<vector<int>> valid_state;

    for (int i = 0; i < state.size(); i++)
    {
        for (int j = 0; j < state[i].size(); j++)
        {
            if (state[i][j] == 0)
            {
                valid_state = state;
                valid_state[i][j] = 5;
                valid_states.push_back(valid_state);
            }
        }
    }
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    vector<vector<int>> state;
    vector<vector<vector<int>>> valid_states;
    init(state);
    find_valid_states(state, valid_states);

    for (int episode = 0; episode < 300; episode++)
    {
        if (valid_states.size() == 0) {
            cout << "No solution\n";
            return 0;
        }

        state = valid_states[rand() % valid_states.size()];
        int n = 100;

        do
        {
            n--;

            if (r[state].size() == 0)
                insert_children(state);

            if (r[state].size() == 0)
                break;

            int random_index = rand() % r[state].size();
            pair<vector<vector<int>>, double> next_state;

            for (auto child : r[state])
            {
                if (!random_index)
                {
                    next_state = child;
                    break;
                }

                random_index--;
            }

            if (r[next_state.first].size() == 0)
                insert_children(next_state.first);

            double maximum = 0, y = 0.8;

            for (auto action : q[next_state.first])
                maximum = max(maximum, action.second);

            for (auto action : q[state])
            {
                if (equals(action.first, next_state.first))
                {
                    auto temp = action;
                    temp.second = next_state.second + y * maximum;
                    // Because values of Set are immutable, the modified action should be inserted 
                    // then the original one should be deleted.
                    q[state].erase(action);
                    q[state].insert(temp);
                    break;
                }
            }

            state = next_state.first;

        } while (n && !isDeadLock(state) && !isGoal(state));
    }

    if (goals == 0) {
        cout << "No solution\n";
        return 0;
    }

    while (true)
    {
        init(state);
        print(state);
        cout << "Choose a valid position(X Y): ";
        int x, y;
        cin >> x >> y;
        state[x][y] = 5;
        cout << "***********************************\n";
        print(state);

        while (!isGoal(state) && !isDeadLock(state))
        {
            double maximum = 0;

            for (auto action : q[state])
            {
                if (action.second > maximum && !equals(action.first, state))
                {
                    state = action.first;
                    maximum = action.second;
                }
            }

            if (maximum == 0)
            {
                cout << "No solution\n";
                break;
            }

            print(state);

        }
        system("pause");
        system("cls");
    }
}