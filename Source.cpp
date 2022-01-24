#include <iostream>
#include <numbers>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <map>
#include <algorithm>
#include <ctime>
#include <clocale> 
#include <windows.h>

using namespace std;

map<pair<double, double>, int>const_count;

double Stirling(int N, int K) {
    if (N == K) return 1;
    else
        if (K == 0) return 0;
        else
            return Stirling(N - 1, K - 1) + K * Stirling(N - 1, K);

}

int calcCountClasses(const int& s) {
    double upper = s / 25.0, lower = s / 28.0;
    vector<int>ans;
    for (int i = ceil(lower); i <= floor(upper); ++i)
        ans.push_back(i);
    pair<int, int>mmin = { 0,0 };
    for (int i = 0; i < ans.size(); ++i) {
        int l = ans[i] * 25, u = ans[i] * 28;
        int m = min(s - l, u - s);
        if (mmin.second < m)
            mmin = { i,m };
    }
    if (ans.empty()) {
        cout << " ол-во классов по верхней границе\n";
        return ceil(upper);
    }
    return ans[mmin.first];
}

void setClasses(const bool& flag, const int& count_classes, map<pair<double, double>, int>templ_classes) {//true - перераспределение; false - копирование
    if (flag) {
        int min_c = floor(double(count_classes) / templ_classes.size());
        int remains = count_classes - min_c * templ_classes.size();//остаток
        for (auto& it : templ_classes) it.second += min_c;
        vector<int>mem;
        while (remains != 0) {
            int tmp = rand() % templ_classes.size();
            for (auto it : mem)
                if (it == tmp) continue;
            mem.push_back(tmp);
            auto it = templ_classes.begin();
            advance(it, tmp);
            it->second++;
            --remains;
        }
        for (auto it : templ_classes) const_count[it.first] = it.second;
    }
    else
        for (auto it : const_count) templ_classes[it.first] = it.second;
}

int main() {
    setlocale(LC_ALL, "Russian");
    setlocale(LC_NUMERIC, "C");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    //srand(time(0));
    ifstream file("cord.txt");
    vector<int>numbers;
    vector<pair<double, double>>schools;
    int sum = 0;
    map<pair<double, double>, int>templ_classes;
    map<pair<double, double>, pair<vector<int>, int>>ans;
    while (!file.eof()) {
        string str;
        getline(file, str);
        size_t t2 = str.find(',');
        if (str[0] != '!') {
            size_t t = str.find(';');
            auto tmp = stoi(str.substr(t + 2));
            numbers.push_back(tmp);
            sum += tmp;
        }
        else {
            auto tmp1 = stod(str.substr(1, t2 - 1));
            auto tmp2 = stod(str.substr(t2 + 2));
            templ_classes[{tmp1, tmp2}] = 0;
            schools.push_back({ tmp1,tmp2 });
            ans[{tmp1, tmp2}];
        }
    }

    auto count = calcCountClasses(sum);
    setClasses(true, count, templ_classes);

    sort(numbers.begin(), numbers.end());
    
    vector<int>tmp;
    bool flag;
    while (!numbers.empty()) {
        flag = false;
        for (const auto &it : schools) {
            if (ans[it].second + numbers.back() <= const_count[it] * 28) {
                ans[it].first.push_back(numbers.back());
                ans[it].second += numbers.back();
                numbers.pop_back();                
                flag = true;
                break;
            }
        }
        if (!flag) {
            tmp.push_back(numbers.back());
            numbers.pop_back();
        }
    }
    cout <<"¬сего детей = " << sum << endl;
    cout << (tmp.empty() ? "YES\n" : "NO\n");
	return 0;
}