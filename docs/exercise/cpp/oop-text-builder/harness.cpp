// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <iostream>
#include <sstream>
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
        TextBuilder builder;
        std::ostringstream stream;
        stream << builder;
        const bool ok = builder.empty() && builder.size() == 0 &&
                        builder.str().empty() && stream.str().empty();
        report("empty", ok);
    }

    {
        TextBuilder builder("ab");
        TextBuilder& first = builder.append("cd");
        TextBuilder& second = builder.append('!');
        const bool ok = &first == &builder && &second == &builder &&
                        builder.str() == "abcd!" && builder.size() == 5 &&
                        !builder.empty();
        builder.append("").append("x");
        report("append", ok && builder.str() == "abcd!x");
    }

    {
        TextBuilder builder("hi");
        builder += " there";
        builder += "";
        report("plus-equals", builder.str() == "hi there" && builder.size() == 8);
    }

    {
        const TextBuilder base("ab");
        const TextBuilder joined = base + "cd";
        const bool ok = joined.str() == "abcd" && base.str() == "ab" &&
                        base.size() == 2 && joined.size() == 4;
        report("plus", ok);
    }

    {
        TextBuilder builder("hello");
        builder[0] = 'H';
        const TextBuilder& read_only = builder;
        const bool ok = builder.str() == "Hello" && read_only[0] == 'H' &&
                        read_only[4] == 'o' && &read_only[1] == &builder.str()[1];
        report("index", ok);
    }

    {
        const TextBuilder a("abc");
        const TextBuilder b("abd");
        const TextBuilder c("abc");
        const TextBuilder empty;
        const bool ok = a == c && !(a == b) && a < b && !(b < a) &&
                        empty < a && !(a < c);
        report("compare", ok);
    }

    {
        const TextBuilder builder("stream");
        std::ostringstream stream;
        stream << builder << '!';
        report("stream", stream.str() == "stream!");
    }

    {
        TextBuilder builder("   ");
        builder.clear();
        const bool ok = builder.empty() && builder.size() == 0 &&
                        builder.str().empty();
        builder.append("z");
        report("clear", ok && builder.str() == "z");
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
