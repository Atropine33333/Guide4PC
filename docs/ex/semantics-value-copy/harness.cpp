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
    report("total-basic", total(Data{1, {1, 2, 3}}) == 6 &&
                              total(Data{2, {}}) == 0 &&
                              total(Data{3, {-5, 2}}) == -3);

    {
        const Data data{4, {4, 5}};
        const int before = total(data);
        const bool unchanged = data.values.size() == 2 && data.values[0] == 4 &&
                               data.values[1] == 5;
        report("const-ref-call", before == 9 && unchanged);
    }

    {
        Data data{5, {1}};
        append_copy(data, 9);
        report("append-copy", data.values.size() == 1 && data.values[0] == 1 &&
                                  data.id == 5);
    }

    {
        Data data{6, {2}};
        append_ref(data, 9);
        const bool ok = data.values.size() == 2 && data.values[1] == 9 && data.id == 6;
        append_ref(data, 10);
        report("append-ref", ok && data.values.size() == 3 && data.values[2] == 10);
    }

    {
        Data original{7, {1, 2}};
        Data copy = original;
        copy.id = 8;
        copy.values.push_back(3);
        report("copy-independent", original.id == 7 && original.values.size() == 2 &&
                                       original.values[1] == 2);
    }

    {
        Data made = make_data(9, 4);
        const bool shape = made.id == 9 && made.values.size() == 4 &&
                           made.values[0] == 0 && made.values[3] == 3;
        Data another = made;
        another.values.push_back(100);
        report("return-copy", shape && made.values.size() == 4 &&
                                  total(make_data(1, 0)) == 0);
    }

    {
        Data data{10, {11, 12}};
        report("ref-alias",
               &first_value(data) == &data.values[0] && first_value(data) == 11);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
