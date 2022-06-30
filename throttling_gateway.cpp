/*
 *Throttling Gateway.
 **/
#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);


/*
 * Complete the 'droppedRequests' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts INTEGER_ARRAY requestTime as parameter.
 */

int droppedRequests(vector<int> requestTime) {
    int size = requestTime.size();
    int dropped_req = 0;
    
    unordered_map<int,int> mp;
    for (int i =0; i < size; i++) {
        if( i > 2 && requestTime[i] == requestTime[i-3]) {
            if((mp.find(requestTime[i]) == mp.end()) || mp[requestTime[i]] != i){
                //to update the last occurence of the same time value
                mp[requestTime[i]] = i;
                dropped_req++;
            }
        } else if (i > 19 && (requestTime[i] - requestTime[i-20] < 10)) {
            if ((mp.find(requestTime[i]) == mp.end()) || mp[requestTime[i]] != i) {
                //to update the last occurence of the same time value
                mp[requestTime[i]] = i;
                dropped_req++;
            }
        }  else if (i > 59 && (requestTime[i] - requestTime[i-60] < 60 )) {
            if ((mp.find(requestTime[i]) == mp.end()) || mp[requestTime[i]] != i) {
                //to update the last occurence of the same time value
                mp[requestTime[i]] = i;
                dropped_req++;
            }
        }
    } 
    return dropped_req;
}


int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string requestTime_count_temp;
    getline(cin, requestTime_count_temp);

    int requestTime_count = stoi(ltrim(rtrim(requestTime_count_temp)));

    vector<int> requestTime(requestTime_count);

    for (int i = 0; i < requestTime_count; i++) {
        string requestTime_item_temp;
        getline(cin, requestTime_item_temp);

        int requestTime_item = stoi(ltrim(rtrim(requestTime_item_temp)));

        requestTime[i] = requestTime_item;
    }

    int result = droppedRequests(requestTime);

    fout << result << "\n";

    fout.close();

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}
