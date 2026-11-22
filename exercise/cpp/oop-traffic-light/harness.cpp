// Guide4PC harness appended to the student's code in the same translation unit.
#include <cstring>
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
        TrafficLight light;
        report("initial", light.current() == Light::Red && light.ticks() == 0);
    }

    {
        TrafficLight light;
        light.advance();
        const bool first = light.current() == Light::Green && light.ticks() == 1;
        light.advance();
        const bool second = light.current() == Light::Yellow && light.ticks() == 2;
        light.advance();
        const bool third = light.current() == Light::Red && light.ticks() == 3;
        report("cycle", first && second && third);
    }

    {
        TrafficLight light;
        light.advance(4);
        report("steps", light.current() == Light::Green && light.ticks() == 4 &&
                            TrafficLight::name(light.current()) != nullptr);
    }

    {
        TrafficLight light;
        light.advance(0);
        report("zero-steps", light.current() == Light::Red && light.ticks() == 0);
    }

    {
        const bool names = std::strcmp(TrafficLight::name(Light::Red), "Red") == 0 &&
                           std::strcmp(TrafficLight::name(Light::Green), "Green") == 0 &&
                           std::strcmp(TrafficLight::name(Light::Yellow), "Yellow") == 0;
        report("names", names);
    }

    {
        TrafficLight light;
        for (int i = 0; i < 300; ++i) light.advance();
        report("many", light.current() == Light::Red && light.ticks() == 300);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
