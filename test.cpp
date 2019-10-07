#include <iostream>
#include <map>
#include <string>

int main()
{
    std::map<std::string, std::string> myMap;
    myMap.insert_or_assign("a", "apple"     );
    myMap.insert_or_assign("b", "bannana"   );
    myMap.insert_or_assign("c", "cherry"    );
    myMap.insert_or_assign("c", "clementine");

    for (const auto &pair : myMap) {
        std::cout << pair.first << " : " << pair.second << '\n';
    }
}