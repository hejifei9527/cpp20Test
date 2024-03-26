#include <iostream>
#include <unordered_map>
#include <thread>
#include <vector>
#include <queue>
#include <map>
#include <fstream>

std::unordered_map<int, int> mp;

using DS = std::map<int, int>;
std::vector<std::queue<DS>> vecDataQueue;

const int threadSize = 5;

void genData() {
    int cnt = 0;
    while(true) {
        int threadId = cnt%threadSize;
        DS data;
        data.insert(std::make_pair(cnt, cnt*1));
        data.insert(std::make_pair(cnt+1, cnt*2));
        data.insert(std::make_pair(cnt+2, cnt*3));
        vecDataQueue[threadId].push(data);
        cnt++;
    }
}

void task(int tid) {
    while (true) {
        if (vecDataQueue[tid].empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            continue;
        }

        auto data = vecDataQueue[tid].front();
        std::string s;
        for (auto &[k,v]: data) {
            s += std::to_string(v) + ",";
        }
        vecDataQueue[tid].pop();

        std::cout << "thread[" << tid << "]: data=" << s << std::endl;
    }
}

std::string getCurTime() {
    time_t timep;
    struct tm *p;

    time(&timep); //获取从1970至今过了多少秒，存入time_t类型的timep
    p = localtime(&timep);//用localtime将秒数转化为struct tm结构体
    char s[20];
    sprintf(s, "%d/%d/%d %02d:%02d:%02d\n", 1900 + p->tm_year, 1+ p->tm_mon, p->tm_mday,p->tm_hour, p->tm_min, p->tm_sec);//2019/2/28 14:18:31
    return s;
}

uint32_t hash(std::string key) {
    const int p = 16777619;
    uint32_t hash = 2166136261;
    for (int idx = 0; idx < key.size(); ++idx) {
        hash = (hash ^ key[idx]) * p;
    }
    hash += hash << 13;
    hash ^= hash >> 7;
    hash += hash << 3;
    hash ^= hash >> 17;
    hash += hash << 5;
    return hash;
}

int main() {
    for (int i = 0; i < 10; i++) {
        std::cout << i << ' ' << hash(std::to_string(i)) << std::endl;
    }
//    for(int i = 0; i < threadSize; i++) {
//        vecDataQueue.emplace_back();
//    }
//
//    std::vector<std::thread> vecThreads;
//
//    std::thread dataThread(genData);
//
//    for(int i = 0; i < threadSize; i++) {
//        std::thread t(task, i);
//        vecThreads.push_back(std::move(t));
//    }
//
//    dataThread.join();
//    for(auto & thread: vecThreads) {
//        thread.join();
//    }
    return 0;
}
