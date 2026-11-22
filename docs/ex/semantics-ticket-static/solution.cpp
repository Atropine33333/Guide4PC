// 参考实现：semantics-ticket-static
namespace {
int g_issued = 0;
}  // namespace

int next_ticket() {
    return ++g_issued;
}

int tickets_issued() {
    return g_issued;
}

void reset_tickets() {
    g_issued = 0;
}
