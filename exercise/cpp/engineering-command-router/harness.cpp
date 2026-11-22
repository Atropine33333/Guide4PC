// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <iostream>
#include <string>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char *id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

}  // namespace

int main() {
    {
        CommandRouter router;
        report("empty", router.size() == 0 && !router.has("x") &&
                            router.dispatch("x", "a") == "unknown");
    }

    {
        CommandRouter router;
        router.add("echo", [](const std::string &argument) { return argument; });
        const bool ok = router.size() == 1 && router.has("echo") &&
                        router.dispatch("echo", "hello") == "hello" &&
                        router.dispatch("echo", "") == "";
        report("echo", ok);
    }

    {
        CommandRouter router;
        router.add("cmd", [](const std::string &) { return std::string("first"); });
        router.add("cmd", [](const std::string &) { return std::string("second"); });
        report("replace", router.size() == 1 &&
                              router.dispatch("cmd", "") == "second");
    }

    {
        CommandRouter router;
        router.add("up", [](const std::string &s) { return "UP:" + s; });
        router.add("down", [](const std::string &s) { return "DOWN:" + s; });
        router.add("empty", [](const std::string &) { return std::string("[]"); });
        const bool ok = router.size() == 3 && router.has("up") &&
                        router.has("down") && router.has("empty") &&
                        router.dispatch("up", "1") == "UP:1" &&
                        router.dispatch("down", "2") == "DOWN:2" &&
                        router.dispatch("empty", "") == "[]" &&
                        router.dispatch("UP", "1") == "unknown";
        report("many", ok);
    }

    {
        CommandRouter router;
        int calls = 0;
        router.add("count", [&calls](const std::string &argument) {
            ++calls;
            return argument + std::to_string(calls);
        });
        const bool ok = router.dispatch("count", "x") == "x1" &&
                        router.dispatch("count", "y") == "y2" && calls == 2;
        report("capture", ok);
    }

    {
        CommandRouter router;
        router.add("ping", [](const std::string &) { return std::string("pong"); });
        const CommandRouter &view = router;
        report("const", view.has("ping") && view.dispatch("ping", "") == "pong");
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
