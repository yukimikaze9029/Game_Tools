#include "util.h"

void printVectorInt(vector<int>& vec)
{
    vector<int>::iterator it;
    for (it = vec.begin(); it != vec.end(); it++)
    {
        cout << *it << "\t";
    }
    cout << endl;
}
