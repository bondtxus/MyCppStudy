#include <iostream>
#include <thread>
#include <map>
#include <string>
#include <chrono>
using namespace std::chrono_literals;


void RefreshForcast(std::map<std::string, int> forcastMap) {
  while (true) {
    for (auto& item : forcastMap) {
      item.second++;
      std::cout << item.first << " - " << item.second << std::endl;
    }
    std::this_thread::sleep_for(2000ms);
  }
}


int main()
{

  std::map<std::string, int> forcastMap = {
    {"New York", 15},
    {"Mumbai", 20},
    {"Berlin", 18}
  };

  std::thread bgWorker(RefreshForcast, forcastMap);
  
  system("pause>nul");

}