// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>

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
        CircuitBreaker breaker(3, 5);
        report("initial", breaker.state() == CircuitBreaker::State::Closed &&
                              breaker.allow() && breaker.failures() == 0);
    }

    {
        CircuitBreaker breaker(3, 5);
        breaker.record(true);
        breaker.record(false);
        breaker.record(false);
        const bool still_closed =
            breaker.state() == CircuitBreaker::State::Closed &&
            breaker.allow() && breaker.failures() == 2;
        breaker.record(false);
        const bool tripped = breaker.state() == CircuitBreaker::State::Open &&
                             !breaker.allow() && breaker.failures() == 3;
        report("trip", still_closed && tripped);
    }

    {
        CircuitBreaker breaker(3, 5);
        breaker.record(false);
        breaker.record(false);
        breaker.record(true);
        report("success-resets", breaker.state() == CircuitBreaker::State::Closed &&
                                     breaker.failures() == 0 && breaker.allow());
    }

    {
        CircuitBreaker breaker(3, 5);
        for (int i = 0; i < 3; ++i) breaker.record(false);
        for (int i = 0; i < 4; ++i) breaker.tick();
        const bool waiting = breaker.state() == CircuitBreaker::State::Open &&
                             !breaker.allow();
        breaker.tick();
        const bool half = breaker.state() == CircuitBreaker::State::HalfOpen &&
                          breaker.allow();
        breaker.tick();
        report("cooldown", waiting && half &&
                               breaker.state() == CircuitBreaker::State::HalfOpen);
    }

    {
        CircuitBreaker breaker(1, 0);
        breaker.record(false);
        const bool half = breaker.state() == CircuitBreaker::State::HalfOpen &&
                          breaker.allow();
        breaker.record(true);
        report("zero-cooldown", half &&
                                   breaker.state() == CircuitBreaker::State::Closed &&
                                   breaker.failures() == 0);
    }

    {
        CircuitBreaker breaker(1, 2);
        breaker.record(false);
        const bool opened = breaker.state() == CircuitBreaker::State::Open;
        breaker.tick();
        breaker.tick();
        const bool half = breaker.state() == CircuitBreaker::State::HalfOpen;
        breaker.record(false);
        const bool reopened = breaker.state() == CircuitBreaker::State::Open &&
                              !breaker.allow();
        breaker.tick();
        breaker.tick();
        breaker.record(true);
        report("half-open", opened && half && reopened &&
                                breaker.state() == CircuitBreaker::State::Closed);
    }

    {
        CircuitBreaker breaker(2, 3);
        breaker.record(false);
        breaker.tick();
        const bool unchanged = breaker.state() == CircuitBreaker::State::Closed &&
                               breaker.failures() == 1;
        breaker.record(false);
        breaker.record(true);
        breaker.record(false);
        report("ignored", unchanged &&
                              breaker.state() == CircuitBreaker::State::Open &&
                              !breaker.allow() && breaker.failures() == 2);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
