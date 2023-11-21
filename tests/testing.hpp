#include <stdbool.h>

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#ifdef NDEBUG
#define assert(condition) ((void)0)
#else
#undef assert
#define assert(condition)                                    \
    {                                                        \
        if (!(condition)) {                                  \
            testing::CURRENT_FUNC_FAILS.push_back(__LINE__); \
        }                                                    \
    }
#endif

namespace testing {
    namespace colors {
        const std::string COLOR_RED = "\033[31m";
        const std::string COLOR_GREEN = "\033[32m";
        const std::string COLOR_CYAN = "\033[36m";
        const std::string NO_COLOR = "\033[0m";
    }  // namespace colors

    static std::string CURRENT_FUNC_NAME = "";
    static std::vector<int> CURRENT_FUNC_FAILS;
    static bool FAIL_ANYWAYS;

    int test(std::function<void()> func) {
        CURRENT_FUNC_FAILS = {};
        try {
            func();
        }
        catch (const std::exception &e) {
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

    int run_tests(std::vector<std::function<void()>> funcs) {
        int exit_code = 0;
        for (auto func : funcs) {
            exit_code |= test(func);
        }
        return exit_code;
    }
}  // namespace testing