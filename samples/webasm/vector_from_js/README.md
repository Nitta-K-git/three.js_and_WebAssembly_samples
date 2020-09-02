# jsのvector形をC++に渡す



- [embind: Cannot pass vector of strings from JS to C++ · Issue #8933 · emscripten-core/emscripten](https://github.com/emscripten-core/emscripten/issues/8933)

C++でバインド定義した型をjs側でも使用する

二次元配列も扱える

```cpp
register_vector<int>("VectorInt");
register_vector<std::vector<int>>("VVInt");
```

```javascript
const v = new Module.VectorInt();
const v2 = new Module.VectorInt();
v.push_back(10);
v.push_back(1);
v.push_back(3);
v2.push_back(4);
v2.push_back(8);

const vv = new Module.VVInt();
vv.push_back(v);
vv.push_back(v2);
```

