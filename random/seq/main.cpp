#pragma GCC optimize("Ofast")
#pragma GCC arch("native")

#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"

#include <bits/stdc++.h>
using namespace std;

typedef uint64_t u64;
typedef unsigned char uchar;
typedef long double fll;

#define all(v) (v).begin(), (v).end()
#define for_range(type, variable, limit) for (type variable = 0; variable < (limit); variable++)
#define amalloc(type, element_count) (static_cast<type*>(malloc(sizeof(type) * element_count)))
#define check_malloc(m, rvalue) do { if ((m) == NULL) return(rvalue); } while (0);

#ifdef LOCAL
    #define dbg(x) (debugln(#x, x))
#else
    #define dbg(x) (x)
#endif

const fll EPS = 1e-9;
const u64 INF = 1e9;
const u64 MOD = 1e9 + 7;

template <typename T>
T debugln(string_view str, T&& expr) {
    cerr << str << " = " << expr << endl;
    
    return expr;
}

template <typename T>
vector<T> debugln(string_view str, vector<T>&& expr) {
    cerr << str << " = ";
    for_range(int, i, expr.size()) cerr << expr[i] << " ";
    cerr << "\n";
}

inline u64 ceil_div(u64 a, u64 b) {return a / b + a % b == 0 ? 0 : 1;}
inline u64 factorial(u64 a) {u64 n = 1; for_range(u64, i, a - 1) { n = n * (i + 1) % MOD;} return n;}
inline u64 nth_prime(u64 n) {
    const u64 P[549] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541,547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,641,643,647,653,659,661,673,677,683,691,701,709,719,727,733,739,743,751,757,761,769,773,787,797,809,811,821,823,827,829,839,853,857,859,863,877,881,883,887,907,911,919,929,937,941,947,953,967,971,977,983,991,997,1009,1013,1019,1021,1031,1033,1039,1049,1051,1061,1063,1069,1087,1091,1093,1097,1103,1109,1117,1123,1129,1151,1153,1163,1171,1181,1187,1193,1201,1213,1217,1223,1229,1231,1237,1249,1259,1277,1279,1283,1289,1291,1297,1301,1303,1307,1319,1321,1327,1361,1367,1373,1381,1399,1409,1423,1427,1429,1433,1439,1447,1451,1453,1459,1471,1481,1483,1487,1489,1493,1499,1511,1523,1531,1543,1549,1553,1559,1567,1571,1579,1583,1597,1601,1607,1609,1613,1619,1621,1627,1637,1657,1663,1667,1669,1693,1697,1699,1709,1721,1723,1733,1741,1747,1753,1759,1777,1783,1787,1789,1801,1811,1823,1831,1847,1861,1867,1871,1873,1877,1879,1889,1901,1907,1913,1931,1933,1949,1951,1973,1979,1987,1993,1997,1999,2003,2011,2017,2027,2029,2039,2053,2063,2069,2081,2083,2087,2089,2099,2111,2113,2129,2131,2137,2141,2143,2153,2161,2179,2203,2207,2213,2221,2237,2239,2243,2251,2267,2269,2273,2281,2287,2293,2297,2309,2311,2333,2339,2341,2347,2351,2357,2371,2377,2381,2383,2389,2393,2399,2411,2417,2423,2437,2441,2447,2459,2467,2473,2477,2503,2521,2531,2539,2543,2549,2551,2557,2579,2591,2593,2609,2617,2621,2633,2647,2657,2659,2663,2671,2677,2683,2687,2689,2693,2699,2707,2711,2713,2719,2729,2731,2741,2749,2753,2767,2777,2789,2791,2797,2801,2803,2819,2833,2837,2843,2851,2857,2861,2879,2887,2897,2903,2909,2917,2927,2939,2953,2957,2963,2969,2971,2999,3001,4003,4007,4013,4019,4021,4027,4049,4051,4057,4073,4079,4091,4093,4099,4111,4127,4129,4133,4139,4153,4157,4159,4177,4201,4211,4217,4219,4229,4231,4241,4243,4253,4259,4261,4271,4273,4283,4289,4297,4327,4337,4339,4349,4357,4363,4373,4391,4397,4409,4421,4423,4441,4447,4451,4457,4463,4481,4483,4493,4507,4513,4517,4519,4523,4547,4549,4561,4567,4583,4591,4597,4603,4621,4637,4639,4643,4649,4651,4657,4663,4673,4679,4691,4703,4721,4723,4729,4733,4751,4759,4783,4787,4789,4793,4799,4801,4813,4817,4831,4861,4871,4877,4889,4903,4909,4919,4931,4933,4937,4943,4951,4957,4967,4969,4973,4987,4993,4999};

    if (n <= 549) return P[n - 1];

    auto L = logf(static_cast<float>(n));
    auto l = n * static_cast<u64>((L + logf(L)) + 1);

    auto a = vector<bool>(l, true);

    a[0] = false; a[1] = false;

    u64 p = 0; u64 f = 0;

    for (u64 i = 2; i < l; i++) {        
        if (!a[i]) continue;
        
        f += 1; p = i;
        if (f == n) break;

        for (u64 j = i * i; j < l; j += i) a[j] = false;
    }

    return p;
}

template <typename T>
inline size_t binary_search_index(vector<T> v, T data) {
    auto it = lower_bound(v.begin(), v.end(), data);

    if (it == v.end() || *it != data) {
        return -1;
    } else {
        size_t index = distance(v.begin(), it);
        return index;
    }   
}

template <typename T>
inline T rcin() {
    T t;
    cin >> t;
    return t;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

    map<int, vector<int>> numbers;

    int nc = rcin<int>();

    auto ns = amalloc(int, nc);
    check_malloc(ns, 1);

    for_range(int, i, nc) {
        int x = rcin<int>();

        if (numbers.find(x) != numbers.end())
            numbers.insert({x, vector<int>()});
        
        numbers[x].push_back(i);
        ns[i] = x;
    }

    int tc = rcin<int>();

    for_range(int, i, tc) {
        int in = rcin<int>() - 1;
        int n = ns[in];

        int l = 0, r = 0;

        auto nss = numbers[n];
        auto ion = binary_search_index<int>(nss, in);

        if (ion != 0)
            l = nss[ion - 1] + 1;

        if (ion != nss.size() - 1)
            r = nss[ion + 1] + 1;

        
        cout << l << " " << r << endl;
    }

    free(ns);
}