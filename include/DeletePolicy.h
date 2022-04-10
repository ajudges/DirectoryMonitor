#pragma once
#include <vector>
#include <string>

using std::vector;
using std::string;

enum deletePolicyType { ALL, exclude };

class DeletePolicy
{
private:
    deletePolicyType _policy;
    vector<string> _exclude;

public:
    DeletePolicy(deletePolicyType policy, vector<string> exclude);
    DeletePolicy(deletePolicyType policy);
    void CleanUp(string &dir);

    ~DeletePolicy();
};
