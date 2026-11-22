// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <cstddef>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

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
    static_assert(std::is_nothrow_move_constructible_v<Resource>);
    static_assert(std::is_nothrow_move_assignable_v<Resource>);

    {
        const bool before = Resource::live() == 0;
        bool ok = before;
        {
            Resource resource(7);
            if (resource.value() != 7 || Resource::live() != 1) ok = false;
        }
        if (Resource::live() != 0) ok = false;
        report("construction", ok);
    }

    {
        const int copies_before = Resource::copies();
        bool ok = true;
        {
            Resource original(5);
            Resource copy(original);
            if (copy.value() != 5 || Resource::live() != 2) ok = false;
            copy.set(9);
            if (original.value() != 5 || copy.value() != 9) ok = false;
            if (Resource::copies() - copies_before != 1) ok = false;
        }
        report("copy-construct", ok && Resource::live() == 0);
    }

    {
        const int moves_before = Resource::moves();
        bool ok = true;
        {
            Resource source(6);
            Resource target(std::move(source));
            if (target.value() != 6 || source.value() != -1) ok = false;
            if (Resource::live() != 1) ok = false;
            if (Resource::moves() - moves_before != 1) ok = false;
        }
        report("move-construct", ok && Resource::live() == 0);
    }

    {
        const int copies_before = Resource::copies();
        bool ok = true;
        {
            Resource left(1);
            Resource right(2);
            right = left;
            if (right.value() != 1 || left.value() != 1) ok = false;
            right.set(8);
            if (left.value() != 1 || right.value() != 8) ok = false;
            if (Resource::live() != 2) ok = false;
            if (Resource::copies() - copies_before != 1) ok = false;
        }
        report("copy-assign", ok && Resource::live() == 0);
    }

    {
        const int moves_before = Resource::moves();
        bool ok = true;
        {
            Resource source(3);
            Resource target(4);
            target = std::move(source);
            if (target.value() != 3 || source.value() != -1) ok = false;
            if (Resource::live() != 1) ok = false;
            if (Resource::moves() - moves_before != 1) ok = false;
        }
        report("move-assign", ok && Resource::live() == 0);
    }

    {
        const int copies_before = Resource::copies();
        bool ok = true;
        {
            Resource resource(5);
            Resource &alias = resource;
            resource = alias;
            if (resource.value() != 5) ok = false;
            if (Resource::copies() != copies_before) ok = false;
        }
        report("self-assign", ok && Resource::live() == 0);
    }

    {
        const int moves_before = Resource::moves();
        bool ok = true;
        {
            std::vector<Resource> items;
            items.push_back(Resource(1));
            items.push_back(Resource(2));
            items.push_back(Resource(3));
            if (items.size() != 3 || Resource::live() != 3) ok = false;
            for (std::size_t i = 0; i < items.size(); ++i) {
                if (items[i].value() != static_cast<int>(i + 1)) ok = false;
            }
            if (Resource::moves() - moves_before < 3) ok = false;
            items.clear();
            if (Resource::live() != 0) ok = false;
        }
        report("vector-move", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
