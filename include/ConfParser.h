#pragma once
#include <string>


namespace ConfParser {
// configuration constants
const std::string confFile{"../directoryConf"};
const std::string pathKey{"dirPath"};
const std::string controlPolicyKey{"controlPolicyType"};
const std::string deletePolicyKey{"deletePolicyType"};
const std::string maxSizeInMb{"maxSizeInMb"};
const std::string maxNumOfContent{"maxNumOfContent"};
const std::string delAll{"ALL"};
const std::string excludeSome{"exclude"};
const std::string ignore{"{[]#//"};
const std::string endOfConf{"}"};
}
