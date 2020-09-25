#include <iostream>
#include <emscripten/bind.h>
#include <emscripten/val.h>

using namespace emscripten;
using namespace std;

int main()
{
    cout << "Hello World" << endl;
}

void hello(){
    cout << "hello" << endl;
}

int add(int a, int b){
    return a+b;
}

EMSCRIPTEN_BINDINGS(module) {
	emscripten::function("hello", &hello);
	emscripten::function("add", &add);
}
