#include <string>
#include <iostream>
#include <thread>

using namespace std;

void task1(string msg) {
  cout << "Task1 says: " << msg << endl;
}

int main() {
  thread t1(task1, "Hello");

  t1.join();
  return 0;

}
