#include <string>
#include <vector>
#include "Contain.hpp"
void eraceContainString(std::vector<std::string> &s) {
	for (auto itr = s.begin(); itr < s.end();) {
		bool chk = false;
		for (auto itr2 = s.begin(); itr2 < s.end(); itr2++) {
			if (itr != itr2) {
				if (contain(*itr, *itr2))chk = true;
			}
		}
		if (chk)itr = s.erase(itr);
		else itr++;
	}
}