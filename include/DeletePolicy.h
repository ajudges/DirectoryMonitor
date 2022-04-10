#pragma once
#include <vector>
#include <string>

using std::vector;
using std::string;

enum deletePolicyType { ALL, exclude };

// Policy to manage cleaning up a directory
class DeletePolicy
{
private:
    deletePolicyType _policy;
    vector<string> _exclude;

public:
    DeletePolicy(deletePolicyType policy, vector<string> exclude); // if some files to exclude
    DeletePolicy(deletePolicyType policy); // if delete policy is ALL
    void CleanUp(string &dir); // delete files in directory

    ~DeletePolicy();
};
