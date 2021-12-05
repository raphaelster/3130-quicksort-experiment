#include <iostream>
#include <vector>
#include <chrono>
#include <functional>
#include <random>
#include <algorithm>
#include <cmath>
#include <cassert>

using namespace std;


class Timer {
    public:

    void start() {
        startTime = std::chrono::system_clock::now();
    }

    double stop() {
        std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - startTime;

        return elapsed.count();
    }

    private:
    std::chrono::system_clock::time_point startTime;

};

void swap(vector<int>& arr, int aIdx, int bIdx) {
    int tmp = arr.at(aIdx);
    arr[aIdx] = arr.at(bIdx);
    arr[bIdx] = tmp;
}

void _quicksort(vector<int>& in, int lo, int hi, std::function<int(int, int)> randFunc) {
    if (hi - lo <= 1) return;
    if (hi - lo == 2) {
        if (in.at(lo) > in.at(hi-1)) swap(in, lo, hi-1);
        return;
    }

    int pivot = randFunc(lo, hi);
    int pivotValue = in.at(pivot);

    swap(in, pivot, hi-1);
    //in.swap(in.begin() + pivot, in.begin() + (hi-1));

    pivot = hi-1;

    for (int i = pivot - 1; i >= lo; i--) {
        if (in.at(i) > pivotValue) {
            swap(in, i, pivot-1);
            swap(in, pivot-1, pivot);
            pivot--;
        }
    }

    //std::cout<<"["<<lo<<", "<<hi<<"] -> ["<<lo<<", "<<pivot<<"] & ["<<pivot+1<<", "<<hi<<"]\n";
    _quicksort(in, lo, pivot, randFunc);
    _quicksort(in, pivot+1, hi, randFunc);
}


void quicksort(vector<int>& in, std::function<int(int lo, int hi)> randFunc) {

    _quicksort(in, 0, in.size(), randFunc);
}

double timeQuicksort(const vector<int>& in, std::function<int(int lo, int hi)> randFunc) {
    vector<int> outList;

    for (int i : in) outList.push_back(i);

    Timer timer;
    timer.start();

    quicksort(outList, randFunc);

    double out = timer.stop();

    for (int i=1; i<outList.size(); i++) assert(outList.at(i-1) <= outList.at(i));
    return out;
}

struct RandomFunc {
    std::default_random_engine engine;

    RandomFunc(int seed)
        : engine(seed) {}

    int operator()(int lo, int hi) {

        return std::abs((int) engine()) % (hi - lo) + lo;
    }
};

struct ListStatistics {
    double mean;
    double variance;
    double standardDeviation;

    ListStatistics(std::vector<double> list) {
        double sum = 0.0;
        for (double d : list) sum += d;
        mean = sum / list.size();

        variance = 0.0;
        for (double d : list) variance += std::pow(d - mean, 2);

        standardDeviation = std::sqrt(variance);
    }

    friend std::ostream& operator<<(std::ostream& o, const ListStatistics& ls) {
        o<<"mean: "<<ls.mean<<"\nvariance: "<<ls.variance<<"\nstandard deviation:"<<ls.standardDeviation<<std::endl;
        return o;
    }
};

vector<double> runQuicksortTrials(int listSize, int listMin, int listMax, std::function<int(int hi, int lo)> randFunc, int numTrials) {
    vector<int> list;

    for (int i=0; i<listSize;  i++) list.push_back(randFunc(listMin, listMax));

    vector<double> out;

    for (int i=0; i<numTrials; i++) out.push_back(timeQuicksort(list, randFunc));

    return out;
}


int main() {

    RandomFunc random(1234);

    const int BASE_LIST_SIZE = 500000;
    const int MIN_LIST_ELEMENT = 0;
    const int MAX_LIST_ELEMENT = 1000;
    const int NUM_TRIALS = 100;

    vector<double> beforeTest = runQuicksortTrials( BASE_LIST_SIZE, MIN_LIST_ELEMENT, MAX_LIST_ELEMENT, random, NUM_TRIALS);
    for (double d : beforeTest) std::cout<<d<<std::endl;
    ListStatistics beforeTestStats(beforeTest);
    std::cout<<"beforeTestStats:\n"<<beforeTestStats<<std::endl;

    vector<double> afterTest = runQuicksortTrials(2*BASE_LIST_SIZE, MIN_LIST_ELEMENT, MAX_LIST_ELEMENT, random, NUM_TRIALS);
    for (double d : afterTest) std::cout<<d<<std::endl;
    ListStatistics afterTestStats(afterTest);
    std::cout<<"afterTestStats:\n"<<afterTestStats<<std::endl;



    return 0;
    //return timeQuicksort(list, random);

    //return 0;
}
