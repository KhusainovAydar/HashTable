//
// Created by MacBook on 03.02.18.
//
#include <bits/stdc++.h>
#include "hash_map.h"
using namespace std;

//int main() {
//    int n;
//    cin >> n;
//    HashMap<int, int> mp;
//    for (int i = 0; i < n; i++) {
//        char c;
//        cin >> c;
//        switch(c) {
//            case '+':
//                int a, b;
//                cin >> a >> b;
//                mp[a] = b;
//                break;
//            case '-':
//                int k;
//                cin >> k;
//                mp.erase(k);
//                break;
//            case '?':
//                cin >> k;
//                if (mp.find(k) == mp.end()) {
//                    cout << -1 << endl;
//                } else {
//                    cout << mp[k] << endl;
//                }
//                break;
//            case '<':
//                for (auto &t : mp) {
//                    cout << t.first << ' ' << t.second << endl;
//                }
//                break;
//            case '!':
//                mp.clear();
//                break;
//        }
//    }
//}