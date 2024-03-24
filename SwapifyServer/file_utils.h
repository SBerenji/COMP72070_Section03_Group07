
#pragma once
#include <iostream>
#include <vector>
#include <string>

std::vector<unsigned char> base64Decode(const std::string& input);

std::string base64Encode(const std::vector<unsigned char>& input);
