// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <cstddef>
#include <iostream>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char* id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

}  // namespace

int main() {
    {
        const int before_alive = Tracker::alive();
        const int before_constructions = Tracker::constructions();
        const int before_destructions = Tracker::destructions();
        {
            Tracker direct(7);
            report("tracker", direct.value() == 7 &&
                                  Tracker::alive() == before_alive + 1 &&
                                  Tracker::constructions() ==
                                      before_constructions + 1);
        }
        report("tracker-destroyed",
               Tracker::alive() == before_alive &&
                   Tracker::destructions() == before_destructions + 1);
    }

    {
        const int before_alive = Tracker::alive();
        const int before_constructions = Tracker::constructions();
        const int before_destructions = Tracker::destructions();
        {
            TrackerBuffer buffer(3);
            const bool first = buffer.push(10);
            const bool second = buffer.push(20);
            const bool third = buffer.push(30);
            report("push", first && second && third && buffer.size() == 3 &&
                               Tracker::alive() == before_alive + 3 &&
                               Tracker::constructions() ==
                                   before_constructions + 3);
        }
        report("destruction",
               Tracker::alive() == before_alive &&
                   Tracker::destructions() == before_destructions + 3);
    }

    {
        TrackerBuffer buffer(2);
        buffer.push(1);
        buffer.push(2);
        const bool extra = buffer.push(3);
        report("full", !extra && buffer.size() == 2);
    }

    {
        TrackerBuffer buffer(4);
        for (int i = 0; i < 4; ++i) buffer.push(i * 11 - 5);
        const TrackerBuffer& view = buffer;
        report("values", buffer.size() == 4 && buffer[0].value() == -5 &&
                             buffer[1].value() == 6 &&
                             buffer[2].value() == 17 &&
                             buffer[3].value() == 28 &&
                             view[2].value() == 17);
    }

    {
        TrackerBuffer zero(0);
        TrackerBuffer one(1);
        const bool zero_push = zero.push(1);
        const bool one_push = one.push(2);
        report("edge", !zero_push && zero.size() == 0 && one_push &&
                           one.size() == 1 && one[0].value() == 2);
    }

    {
        const int before_alive = Tracker::alive();
        {
            TrackerBuffer first(2);
            TrackerBuffer second(3);
            first.push(1);
            second.push(2);
            second.push(3);
            report("multiple", Tracker::alive() == before_alive + 3 &&
                                   first.size() == 1 && second.size() == 2);
        }
        report("multiple-destroyed", Tracker::alive() == before_alive);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
