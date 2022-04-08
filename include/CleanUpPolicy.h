#pragma once
#include <vector>
#include <string>

using std::vector;
using std::string;

enum cleanUpPolicyType { ALL, exclude };

class CleanUpPolicy
{
private:
    cleanUpPolicyType _policy;
    vector<string> _exclude;

public:
    CleanUpPolicy(cleanUpPolicyType policy, vector<string> exclude);
    CleanUpPolicy(cleanUpPolicyType policy);
    void CleanUp(string &dir);

    ~CleanUpPolicy();
};
