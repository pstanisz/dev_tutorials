# C++17 most useful features

Examples of **C++17** features.

## Char literals

Available since **C++11**, except of `u8'` added by **C++17**.

```cpp
    // 'a' in UTF-8
    const char c1 = u8'a';      // UTF-8:  a       61

    // 'a' in UTF-16
    const char16_t c2 = u'a';   // UTF-16: 61      61 00

    // 'a' in UTF-32
    const char32_t c3 = U'a';   // UTF-32: 61      61 00 00 00

    // 'a' in wide character literal
    const wchar_t c4 = L'a';    // Wide:   61      61 00 00 00
```
