# Exercise the compatibility layer in the first standard where
# std::result_of is no longer available. OF 0.12.1 already selects C++20 on
# supported Linux compilers. Project Generator ignores this file on Windows,
# where the generated Visual Studio project supplies the dialect.
MAC_OS_CPP_VER = -std=c++20
