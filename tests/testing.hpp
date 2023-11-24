#include <stdbool.h>

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifdef NDEBUG
#define assert(condition) ((void)0)
#else
#undef assert
#define assert(condition)                                                                                            \
    {                                                                                                                \
        if (!(condition)) {                                                                                          \
            testing::CURRENT_FUNC_FAILS.push_back(__LINE__);                                                         \
            std::stringstream text;                                                                                  \
            text << "[Line " << __LINE__ << "] Condition failed: " << #condition;                                    \
            testing::CAPTURED_VARS.reserve(testing::CAPTURED_VARS.size() + testing::TEMP_CAPTURED_VARS.size());      \
            testing::CAPTURED_VARS.insert(                                                                           \
                testing::CAPTURED_VARS.end(), testing::TEMP_CAPTURED_VARS.begin(), testing::TEMP_CAPTURED_VARS.end() \
            );                                                                                                       \
            testing::CAPTURED_VARS.push_back(text.str());                                                            \
        }                                                                                                            \
        testing::TEMP_CAPTURED_VARS.clear();                                                                         \
    }
#endif

#define CAPTURE_VARS(...) testing::capture_vars(__FILE__, __func__, __LINE__, #__VA_ARGS__, __VA_ARGS__)

namespace testing {
    namespace colors {
        const std::string COLOR_RED = "\033[31m";
        const std::string COLOR_GREEN = "\033[32m";
        const std::string COLOR_CYAN = "\033[36m";
        const std::string NO_COLOR = "\033[0m";
    }  // namespace colors

    static std::string CURRENT_FUNC_NAME = "";
    static std::vector<int> CURRENT_FUNC_FAILS;
    static std::vector<std::string> CAPTURED_VARS, TEMP_CAPTURED_VARS;
    static bool FAIL_ANYWAYS;

    int test(std::function<void()> func) {
        try {
            func();
        }
        catch (const std::exception& e) {
        }

        std::cout << "  " << colors::COLOR_CYAN << "•" << colors::NO_COLOR << " " << CURRENT_FUNC_NAME << " [";

        if (CURRENT_FUNC_FAILS.size() > 0 || FAIL_ANYWAYS) {
            std::cout << colors::COLOR_RED << "FAILED" << colors::NO_COLOR << "]";

            if (CURRENT_FUNC_FAILS.size() > 0) {
                sort(CURRENT_FUNC_FAILS.begin(), CURRENT_FUNC_FAILS.end());
                CURRENT_FUNC_FAILS.erase(
                    unique(CURRENT_FUNC_FAILS.begin(), CURRENT_FUNC_FAILS.end()), CURRENT_FUNC_FAILS.end()
                );

                std::cout << " (" << (CURRENT_FUNC_FAILS.size() == 1 ? "Line: " : "Lines: ");
                for (int i = 0; i < CURRENT_FUNC_FAILS.size(); ++i) {
                    std::cout << (i == 0 ? "" : ", ") << CURRENT_FUNC_FAILS[i];
                }

                std::cout << ")";
            }
            std::cout << std::endl;

            for (auto captured_var : CAPTURED_VARS) {
                std::cout << "    " << colors::COLOR_RED << "-" << colors::NO_COLOR << " " << captured_var << std::endl;
            }

            return 1;
        }
        else {
            std::cout << colors::COLOR_GREEN << "PASSED" << colors::NO_COLOR << "]" << std::endl;
            return 0;
        }
    }

    std::function<void()> at_least_n_of_m(std::function<void()> func, int m, int n = 1) {
        return [func, m, n]() {
            int valid = 0;
            for (int t = 0, len = CURRENT_FUNC_FAILS.size(); t < m && valid < n; ++t) {
                func();
                valid += CURRENT_FUNC_FAILS.size() == len;
                len = CURRENT_FUNC_FAILS.size();
            }
            if (valid == n) {
                CURRENT_FUNC_FAILS.clear();
            }
            else {
                FAIL_ANYWAYS = true;
            }
        };
    }

    void setup_test() {
        CURRENT_FUNC_FAILS = {};
        CAPTURED_VARS = TEMP_CAPTURED_VARS = {};
        FAIL_ANYWAYS = false;
    }

    int run_tests(std::vector<std::function<void()>> funcs) {
        int exit_code = 0;
        for (auto func : funcs) {
            setup_test();
            exit_code |= test(func);
        }
        return exit_code;
    }

    template <typename T>
    std::stringstream capture_var(const std::string& name, const T& arg) {
        std::stringstream ss;
        ss << name << " = " << arg;
        return ss;
    }

    template <typename T1, typename... T2>
    std::stringstream capture_var(const std::string& names, const T1& arg, const T2&... args) {
        std::stringstream ss;
        ss << names.substr(0, names.find(',')) << " = " << arg << " ; ";
        ss << capture_var(names.substr(names.find(',') + 2), args...).str();
        return ss;
    }

    template <typename T1, typename... T2>
    void capture_vars(
        const std::string& filename, const std::string& func, const int line, const std::string& names, const T1& arg,
        const T2&... args
    ) {
        std::stringstream ss;
        ss << "[Line " << line << "] " << capture_var(names, arg, args...).str();
        TEMP_CAPTURED_VARS.push_back(ss.str());
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
        os << "[";
        for (int i = 0; i < v.size(); ++i) {
            os << (i > 0 ? ", " : "") << v[i];
        }
        os << "]";
        return os;
    }
}  // namespace testing