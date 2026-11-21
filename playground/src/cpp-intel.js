// C++ IntelliSense（离线版）：补全、悬停提示。
//
// 不依赖 clangd —— 在浏览器里起一个 clangd 需要 SharedWorker + JSPI + 额外几十 MB 资产，
// 对练习页来说太重了。这里用「词法扫描 + 标准库符号表」做一个够用的版本：
//
//   · 关键字、内建类型、当前文件里的变量/函数/类/宏、#include 的头文件
//   · 标准库符号（按已 include 的头文件过滤，没写 include 时全部给出）
//   · 成员补全：局部对象 . / ->，按声明里的类型查成员表
//   · 悬停：显示种类、签名和一行说明
//
// 所有数据都写在文件里，不联网、不额外加载。

const KEYWORDS = [
  'alignas', 'alignof', 'and', 'and_eq', 'asm', 'auto', 'bitand', 'bitor', 'bool', 'break',
  'case', 'catch', 'char', 'char8_t', 'char16_t', 'char32_t', 'class', 'compl', 'concept',
  'const', 'consteval', 'constexpr', 'constinit', 'const_cast', 'continue', 'co_await',
  'co_return', 'co_yield', 'decltype', 'default', 'delete', 'do', 'double', 'dynamic_cast',
  'else', 'enum', 'explicit', 'export', 'extern', 'false', 'float', 'for', 'friend', 'goto',
  'if', 'inline', 'int', 'long', 'mutable', 'namespace', 'new', 'noexcept', 'not', 'not_eq',
  'nullptr', 'operator', 'or', 'or_eq', 'override', 'private', 'protected', 'public',
  'register', 'reinterpret_cast', 'requires', 'return', 'short', 'signed', 'sizeof', 'static',
  'static_assert', 'static_cast', 'struct', 'switch', 'template', 'this', 'thread_local',
  'throw', 'true', 'try', 'typedef', 'typeid', 'typename', 'union', 'unsigned', 'using',
  'virtual', 'void', 'volatile', 'wchar_t', 'while', 'xor', 'xor_eq',
];

const BUILTIN_TYPES = [
  'int', 'long', 'short', 'char', 'bool', 'float', 'double', 'void', 'unsigned', 'signed',
  'size_t', 'std::size_t', 'long long', 'unsigned int', 'long double', 'int8_t', 'int16_t',
  'int32_t', 'int64_t', 'uint8_t', 'uint16_t', 'uint32_t', 'uint64_t', 'ssize_t',
];

// ---- 标准库符号表 -------------------------------------------------------
// k: 种类（用于显示）；h: 头文件；sig: 签名；d: 一句话说明
const STD = [
  // iostream
  { n: 'cout', k: '变量', h: 'iostream', sig: 'std::ostream std::cout', d: '标准输出流，用 << 依次写出内容。' },
  { n: 'cerr', k: '变量', h: 'iostream', sig: 'std::ostream std::cerr', d: '标准错误流，不受缓冲影响，适合输出错误。' },
  { n: 'cin', k: '变量', h: 'iostream', sig: 'std::istream std::cin', d: '标准输入流，用 >> 依次读入内容。' },
  { n: 'endl', k: '函数', h: 'iostream', sig: 'std::ostream& std::endl(std::ostream&)', d: '输出换行并立刻刷新缓冲区。' },
  { n: 'flush', k: '函数', h: 'iostream', sig: 'std::ostream& std::flush(std::ostream&)', d: '刷新输出缓冲区。' },
  { n: 'ws', k: '函数', h: 'iostream', sig: 'std::istream& std::ws(std::istream&)', d: '跳过输入流开头的空白字符。' },
  { n: 'ostream', k: '类型', h: 'iostream', sig: 'class std::ostream', d: '输出流类型。' },
  { n: 'istream', k: '类型', h: 'iostream', sig: 'class std::istream', d: '输入流类型。' },
  // string
  { n: 'string', k: '类型', h: 'string', sig: 'class std::string', d: '动态长度的字符序列，支持拼接、查找、截取。' },
  { n: 'to_string', k: '函数', h: 'string', sig: 'std::string std::to_string(T value)', d: '把数字转换成字符串。' },
  { n: 'stoi', k: '函数', h: 'string', sig: 'int std::stoi(const std::string& s)', d: '把字符串解析为 int；失败时抛 std::invalid_argument。' },
  { n: 'stol', k: '函数', h: 'string', sig: 'long std::stol(const std::string& s)', d: '把字符串解析为 long。' },
  { n: 'stoll', k: '函数', h: 'string', sig: 'long long std::stoll(const std::string& s)', d: '把字符串解析为 long long。' },
  { n: 'stod', k: '函数', h: 'string', sig: 'double std::stod(const std::string& s)', d: '把字符串解析为 double。' },
  { n: 'getline', k: '函数', h: 'string', sig: 'std::istream& std::getline(std::istream&, std::string&, char = \'\\n\')', d: '读取一整行（保留空格），常与 cin 搭配。' },
  { n: 'string_view', k: '类型', h: 'string_view', sig: 'class std::string_view', d: '只读的字符串视图，不拥有字符数据。' },
  // vector
  { n: 'vector', k: '类型', h: 'vector', sig: 'template<class T> class std::vector', d: '动态数组，尾部增删均摊 O(1)，随机访问 O(1)。' },
  // 容器与迭代器
  { n: 'map', k: '类型', h: 'map', sig: 'template<class K, class V> class std::map', d: '有序键值表（红黑树），按 key 有序遍历。' },
  { n: 'unordered_map', k: '类型', h: 'unordered_map', sig: 'template<class K, class V> class std::unordered_map', d: '哈希键值表，平均 O(1) 查找。' },
  { n: 'set', k: '类型', h: 'set', sig: 'template<class T> class std::set', d: '有序集合，元素唯一。' },
  { n: 'unordered_set', k: '类型', h: 'unordered_set', sig: 'template<class T> class std::unordered_set', d: '哈希集合，平均 O(1) 查找。' },
  { n: 'queue', k: '类型', h: 'queue', sig: 'template<class T> class std::queue', d: '队列（FIFO），push 进队、pop 出队。' },
  { n: 'stack', k: '类型', h: 'stack', sig: 'template<class T> class std::stack', d: '栈（LIFO），push 入栈、pop 出栈。' },
  { n: 'deque', k: '类型', h: 'deque', sig: 'template<class T> class std::deque', d: '双端队列，头尾都能 O(1) 增删。' },
  { n: 'priority_queue', k: '类型', h: 'queue', sig: 'template<class T> class std::priority_queue', d: '堆（默认大顶堆）。' },
  { n: 'pair', k: '类型', h: 'utility', sig: 'template<class A, class B> struct std::pair', d: '二元组，成员为 first / second。' },
  { n: 'make_pair', k: '函数', h: 'utility', sig: 'std::pair<A, B> std::make_pair(A, B)', d: '按实参推导类型构造 pair。' },
  { n: 'tuple', k: '类型', h: 'tuple', sig: 'template<class...> class std::tuple', d: '多元组，配合 std::get 访问。' },
  { n: 'optional', k: '类型', h: 'optional', sig: 'template<class T> class std::optional', d: '可能没有值的容器，比裸指针表达更清楚。' },
  // algorithm
  { n: 'sort', k: '函数', h: 'algorithm', sig: 'void std::sort(RandomIt first, RandomIt last[, Comp comp])', d: '对区间做升序排序，默认用 operator<。' },
  { n: 'stable_sort', k: '函数', h: 'algorithm', sig: 'void std::stable_sort(RandomIt, RandomIt[, Comp])', d: '稳定排序，相等元素保持原有顺序。' },
  { n: 'reverse', k: '函数', h: 'algorithm', sig: 'void std::reverse(BidirIt first, BidirIt last)', d: '原地反转区间。' },
  { n: 'find', k: '函数', h: 'algorithm', sig: 'It std::find(It first, It last, const T& value)', d: '线性查找第一个等于 value 的元素。' },
  { n: 'find_if', k: '函数', h: 'algorithm', sig: 'It std::find_if(It first, It last, Pred p)', d: '查找第一个满足谓词的元素。' },
  { n: 'count', k: '函数', h: 'algorithm', sig: 'long std::count(It first, It last, const T& value)', d: '统计等于 value 的元素个数。' },
  { n: 'count_if', k: '函数', h: 'algorithm', sig: 'long std::count_if(It first, It last, Pred p)', d: '统计满足谓词的元素个数。' },
  { n: 'max_element', k: '函数', h: 'algorithm', sig: 'It std::max_element(It first, It last)', d: '返回最大元素的迭代器。' },
  { n: 'min_element', k: '函数', h: 'algorithm', sig: 'It std::min_element(It first, It last)', d: '返回最小元素的迭代器。' },
  { n: 'for_each', k: '函数', h: 'algorithm', sig: 'Fn std::for_each(It first, It last, Fn f)', d: '对区间内每个元素调用 f。' },
  { n: 'fill', k: '函数', h: 'algorithm', sig: 'void std::fill(It first, It last, const T& value)', d: '把区间内所有元素设为 value。' },
  { n: 'copy', k: '函数', h: 'algorithm', sig: 'OutIt std::copy(It first, It last, OutIt out)', d: '复制区间到另一处。' },
  { n: 'unique', k: '函数', h: 'algorithm', sig: 'It std::unique(It first, It last)', d: '去除相邻重复元素（通常先 sort）。' },
  { n: 'lower_bound', k: '函数', h: 'algorithm', sig: 'It std::lower_bound(It first, It last, const T& value)', d: '有序区间中第一个不小于 value 的位置。' },
  { n: 'upper_bound', k: '函数', h: 'algorithm', sig: 'It std::upper_bound(It first, It last, const T& value)', d: '有序区间中第一个大于 value 的位置。' },
  { n: 'binary_search', k: '函数', h: 'algorithm', sig: 'bool std::binary_search(It first, It last, const T& value)', d: '有序区间二分查找，返回是否存在。' },
  { n: 'min', k: '函数', h: 'algorithm', sig: 'const T& std::min(const T& a, const T& b)', d: '返回较小者。' },
  { n: 'max', k: '函数', h: 'algorithm', sig: 'const T& std::max(const T& a, const T& b)', d: '返回较大者。' },
  { n: 'swap', k: '函数', h: 'utility', sig: 'void std::swap(T& a, T& b)', d: '交换两个变量的值。' },
  { n: 'transform', k: '函数', h: 'algorithm', sig: 'OutIt std::transform(InIt first, InIt last, OutIt out, Fn f)', d: '把区间内每个元素经 f 变换后写到 out。' },
  { n: 'accumulate', k: '函数', h: 'numeric', sig: 'T std::accumulate(It first, It last, T init)', d: '求和（numeric 头）。' },
  { n: 'iota', k: '函数', h: 'numeric', sig: 'void std::iota(It first, It last, T value)', d: '用递增序列填充区间（numeric 头）。' },
  // cmath
  { n: 'sqrt', k: '函数', h: 'cmath', sig: 'double std::sqrt(double x)', d: '平方根。' },
  { n: 'pow', k: '函数', h: 'cmath', sig: 'double std::pow(double base, double exp)', d: '幂运算。' },
  { n: 'abs', k: '函数', h: 'cstdlib', sig: 'int std::abs(int x)', d: '绝对值（浮点用 std::fabs）。' },
  { n: 'fabs', k: '函数', h: 'cmath', sig: 'double std::fabs(double x)', d: '浮点绝对值。' },
  { n: 'floor', k: '函数', h: 'cmath', sig: 'double std::floor(double x)', d: '向下取整。' },
  { n: 'ceil', k: '函数', h: 'cmath', sig: 'double std::ceil(double x)', d: '向上取整。' },
  { n: 'round', k: '函数', h: 'cmath', sig: 'double std::round(double x)', d: '四舍五入。' },
  { n: 'fmod', k: '函数', h: 'cmath', sig: 'double std::fmod(double x, double y)', d: '浮点取余。' },
  { n: 'sin', k: '函数', h: 'cmath', sig: 'double std::sin(double x)', d: '正弦（弧度）。' },
  { n: 'cos', k: '函数', h: 'cmath', sig: 'double std::cos(double x)', d: '余弦（弧度）。' },
  { n: 'tan', k: '函数', h: 'cmath', sig: 'double std::tan(double x)', d: '正切（弧度）。' },
  { n: 'atan2', k: '函数', h: 'cmath', sig: 'double std::atan2(double y, double x)', d: '四象限反正切。' },
  { n: 'log', k: '函数', h: 'cmath', sig: 'double std::log(double x)', d: '自然对数。' },
  { n: 'log10', k: '函数', h: 'cmath', sig: 'double std::log10(double x)', d: '常用对数。' },
  { n: 'exp', k: '函数', h: 'cmath', sig: 'double std::exp(double x)', d: 'e 的 x 次方。' },
  { n: 'hypot', k: '函数', h: 'cmath', sig: 'double std::hypot(double x, double y)', d: '直角三角形斜边，避免中间溢出。' },
  { n: 'isnan', k: '函数', h: 'cmath', sig: 'bool std::isnan(double x)', d: '判断是否为 NaN。' },
  { n: 'isinf', k: '函数', h: 'cmath', sig: 'bool std::isinf(double x)', d: '判断是否无穷。' },
  // cstdlib
  { n: 'exit', k: '函数', h: 'cstdlib', sig: 'void std::exit(int status)', d: '立即结束程序并返回状态码。' },
  { n: 'rand', k: '函数', h: 'cstdlib', sig: 'int std::rand()', d: '伪随机数；先 srand 播种。' },
  { n: 'srand', k: '函数', h: 'cstdlib', sig: 'void std::srand(unsigned seed)', d: '给 rand 播种。' },
  { n: 'atoi', k: '函数', h: 'cstdlib', sig: 'int std::atoi(const char* s)', d: 'C 风格字符串转 int。' },
  { n: 'atof', k: '函数', h: 'cstdlib', sig: 'double std::atof(const char* s)', d: 'C 风格字符串转 double。' },
  // fstream / sstream
  { n: 'ifstream', k: '类型', h: 'fstream', sig: 'class std::ifstream', d: '文件输入流。' },
  { n: 'ofstream', k: '类型', h: 'fstream', sig: 'class std::ofstream', d: '文件输出流。' },
  { n: 'fstream', k: '类型', h: 'fstream', sig: 'class std::fstream', d: '可读可写的文件流。' },
  { n: 'istringstream', k: '类型', h: 'sstream', sig: 'class std::istringstream', d: '从字符串读取的输入流。' },
  { n: 'ostringstream', k: '类型', h: 'sstream', sig: 'class std::ostringstream', d: '写入字符串的输出流。' },
  { n: 'stringstream', k: '类型', h: 'sstream', sig: 'class std::stringstream', d: '字符串流，可读可写。' },
  // memory
  { n: 'unique_ptr', k: '类型', h: 'memory', sig: 'template<class T> class std::unique_ptr', d: '独占所有权的智能指针。' },
  { n: 'shared_ptr', k: '类型', h: 'memory', sig: 'template<class T> class std::shared_ptr', d: '共享所有权的智能指针，引用计数。' },
  { n: 'make_unique', k: '函数', h: 'memory', sig: 'std::unique_ptr<T> std::make_unique<T>(Args&&...)', d: '构造 unique_ptr，比 new 更安全。' },
  { n: 'make_shared', k: '函数', h: 'memory', sig: 'std::shared_ptr<T> std::make_shared<T>(Args&&...)', d: '构造 shared_ptr。' },
  { n: 'move', k: '函数', h: 'utility', sig: 'T&& std::move(T&& t)', d: '把左值转换为右值引用，触发移动而非拷贝。' },
  // 字符判断
  { n: 'isdigit', k: '函数', h: 'cctype', sig: 'bool std::isdigit(int ch)', d: '是否数字字符。' },
  { n: 'isalpha', k: '函数', h: 'cctype', sig: 'bool std::isalpha(int ch)', d: '是否字母。' },
  { n: 'isalnum', k: '函数', h: 'cctype', sig: 'bool std::isalnum(int ch)', d: '是否字母或数字。' },
  { n: 'isspace', k: '函数', h: 'cctype', sig: 'bool std::isspace(int ch)', d: '是否空白字符。' },
  { n: 'toupper', k: '函数', h: 'cctype', sig: 'int std::toupper(int ch)', d: '转大写。' },
  { n: 'tolower', k: '函数', h: 'cctype', sig: 'int std::tolower(int ch)', d: '转小写。' },
  // iomanip
  { n: 'setw', k: '函数', h: 'iomanip', sig: 'std::ostream& std::setw(int n)', d: '设置下一次输出的字段宽度。' },
  { n: 'setprecision', k: '函数', h: 'iomanip', sig: 'std::ostream& std::setprecision(int n)', d: '设置浮点有效位数。' },
  { n: 'fixed', k: '函数', h: 'iomanip', sig: 'std::ostream& std::fixed(std::ostream&)', d: '固定小数点计数。' },
  { n: 'setfill', k: '函数', h: 'iomanip', sig: 'std::ostream& std::setfill(char c)', d: '设置填充字符。' },
  // limits
  { n: 'numeric_limits', k: '类型', h: 'limits', sig: 'template<class T> class std::numeric_limits', d: '查询类型的极值，如 max() / min() / is_signed。' },
  // 常量
  { n: 'INT_MAX', k: '宏', h: 'climits', sig: '常量', d: 'int 的最大值。' },
  { n: 'INT_MIN', k: '宏', h: 'climits', sig: '常量', d: 'int 的最小值。' },
  { n: 'LLONG_MAX', k: '宏', h: 'climits', sig: '常量', d: 'long long 的最大值。' },
  { n: 'NULL', k: '宏', h: 'cstddef', sig: '常量', d: '空指针常量；现代 C++ 更推荐 nullptr。' },
];

// ---- 成员表 ------------------------------------------------------------
// 键是去掉 std:: 与模板参数后的基础类型名
const MEMBERS = {
  string: [
    ['size', 'size_t size() const', '字符数量。'], ['length', 'size_t length() const', '同 size()。'],
    ['empty', 'bool empty() const', '是否为空。'], ['substr', 'std::string substr(size_t pos, size_t len = npos) const', '截取子串。'],
    ['find', 'size_t find(const std::string& s, size_t pos = 0) const', '查找子串，找不到返回 npos。'],
    ['rfind', 'size_t rfind(const std::string& s) const', '从右往左查找。'],
    ['append', 'std::string& append(const std::string& s)', '在末尾追加。'],
    ['push_back', 'void push_back(char c)', '追加一个字符。'],
    ['pop_back', 'void pop_back()', '删除最后一个字符。'],
    ['insert', 'std::string& insert(size_t pos, const std::string& s)', '在 pos 处插入。'],
    ['erase', 'std::string& erase(size_t pos = 0, size_t len = npos)', '删除一段字符。'],
    ['replace', 'std::string& replace(size_t pos, size_t len, const std::string& s)', '替换一段内容。'],
    ['clear', 'void clear()', '清空内容。'],
    ['c_str', 'const char* c_str() const', '转为 C 风格字符串。'],
    ['data', 'const char* data() const', '底层字符数组。'],
    ['at', 'char& at(size_t pos)', '带越界检查的访问，越界抛 out_of_range（只能用 at 的异常说明）。'],
    ['front', 'char& front()', '首字符。'], ['back', 'char& back()', '末字符。'],
    ['compare', 'int compare(const std::string& s) const', '字典序比较。'],
    ['find_first_of', 'size_t find_first_of(const char* set) const', '查找任一字符首次出现。'],
  ],
  vector: [
    ['size', 'size_type size() const', '元素个数。'], ['empty', 'bool empty() const', '是否为空。'],
    ['push_back', 'void push_back(const T& value)', '尾部追加。'], ['pop_back', 'void pop_back()', '删除尾部元素。'],
    ['emplace_back', 'T& emplace_back(Args&&... args)', '原地构造尾部元素。'],
    ['insert', 'iterator insert(iterator pos, const T& value)', '在 pos 前插入。'],
    ['erase', 'iterator erase(iterator pos)', '删除 pos 处元素。'],
    ['clear', 'void clear()', '清空所有元素。'], ['resize', 'void resize(size_type n)', '改变元素个数。'],
    ['reserve', 'void reserve(size_type n)', '预留容量，避免反复扩容。'],
    ['capacity', 'size_type capacity() const', '当前容量。'],
    ['at', 'T& at(size_type i)', '带越界检查的访问。'],
    ['front', 'T& front()', '第一个元素。'], ['back', 'T& back()', '最后一个元素。'],
    ['data', 'T* data()', '底层数组指针。'], ['assign', 'void assign(size_type n, const T& v)', '替换为 n 个 v。'],
    ['begin', 'iterator begin()', '首元素迭代器。'], ['end', 'iterator end()', '尾后迭代器。'],
  ],
  map: [
    ['size', 'size_type size() const', '键值对数量。'], ['empty', 'bool empty() const', '是否为空。'],
    ['insert', 'std::pair<iterator,bool> insert(const value_type& v)', '插入键值对。'],
    ['emplace', 'std::pair<iterator,bool> emplace(Args&&...)', '原地构造并插入。'],
    ['erase', 'size_type erase(const key_type& k)', '按键删除。'],
    ['find', 'iterator find(const key_type& k)', '查找，找不到返回 end()。'],
    ['count', 'size_type count(const key_type& k) const', '返回 0 或 1。'],
    ['contains', 'bool contains(const key_type& k) const', 'C++20：键是否存在。'],
    ['at', 'T& at(const key_type& k)', '按键访问，不存在时抛 out_of_range。'],
    ['clear', 'void clear()', '清空。'],
    ['begin', 'iterator begin()', '首元素迭代器（按 key 有序）。'], ['end', 'iterator end()', '尾后迭代器。'],
  ],
  unordered_map: [
    ['size', 'size_type size() const', '键值对数量。'], ['empty', 'bool empty() const', '是否为空。'],
    ['insert', 'std::pair<iterator,bool> insert(const value_type& v)', '插入键值对。'],
    ['emplace', 'std::pair<iterator,bool> emplace(Args&&...)', '原地构造并插入。'],
    ['erase', 'size_type erase(const key_type& k)', '按键删除。'],
    ['find', 'iterator find(const key_type& k)', '查找，找不到返回 end()。'],
    ['count', 'size_type count(const key_type& k) const', '返回 0 或 1。'],
    ['contains', 'bool contains(const key_type& k) const', 'C++20：键是否存在。'],
    ['at', 'T& at(const key_type& k)', '按键访问，不存在时抛 out_of_range。'],
    ['clear', 'void clear()', '清空。'],
  ],
  set: [
    ['size', 'size_type size() const', '元素数量。'], ['empty', 'bool empty() const', '是否为空。'],
    ['insert', 'std::pair<iterator,bool> insert(const value_type& v)', '插入元素。'],
    ['erase', 'size_type erase(const value_type& v)', '删除元素。'],
    ['find', 'iterator find(const value_type& v)', '查找，找不到返回 end()。'],
    ['count', 'size_type count(const value_type& v) const', '返回 0 或 1。'],
    ['contains', 'bool contains(const value_type& v) const', 'C++20：元素是否存在。'],
    ['clear', 'void clear()', '清空。'], ['begin', 'iterator begin()', '首元素迭代器。'],
  ],
  unordered_set: [
    ['size', 'size_type size() const', '元素数量。'], ['empty', 'bool empty() const', '是否为空。'],
    ['insert', 'std::pair<iterator,bool> insert(const value_type& v)', '插入元素。'],
    ['erase', 'size_type erase(const value_type& v)', '删除元素。'],
    ['find', 'iterator find(const value_type& v)', '查找，找不到返回 end()。'],
    ['count', 'size_type count(const value_type& v) const', '返回 0 或 1。'],
    ['contains', 'bool contains(const value_type& v) const', 'C++20：元素是否存在。'],
  ],
  queue: [
    ['push', 'void push(const T& v)', '入队。'], ['pop', 'void pop()', '出队（不返回值）。'],
    ['front', 'T& front()', '队首元素。'], ['back', 'T& back()', '队尾元素。'],
    ['size', 'size_type size() const', '元素个数。'], ['empty', 'bool empty() const', '是否为空。'],
    ['emplace', 'void emplace(Args&&...)', '原地构造并入队。'],
  ],
  stack: [
    ['push', 'void push(const T& v)', '入栈。'], ['pop', 'void pop()', '出栈（不返回值）。'],
    ['top', 'T& top()', '栈顶元素。'], ['size', 'size_type size() const', '元素个数。'],
    ['empty', 'bool empty() const', '是否为空。'], ['emplace', 'void emplace(Args&&...)', '原地构造并入栈。'],
  ],
  deque: [
    ['push_back', 'void push_back(const T& v)', '尾部入队。'], ['push_front', 'void push_front(const T& v)', '头部入队。'],
    ['pop_back', 'void pop_back()', '删除尾部。'], ['pop_front', 'void pop_front()', '删除头部。'],
    ['front', 'T& front()', '首元素。'], ['back', 'T& back()', '末元素。'],
    ['size', 'size_type size() const', '元素个数。'], ['empty', 'bool empty() const', '是否为空。'],
    ['at', 'T& at(size_type i)', '带越界检查的访问。'],
  ],
  priority_queue: [
    ['push', 'void push(const T& v)', '插入并调整堆。'], ['pop', 'void pop()', '弹出堆顶。'],
    ['top', 'const T& top() const', '堆顶元素。'], ['empty', 'bool empty() const', '是否为空。'],
    ['size', 'size_type size() const', '元素个数。'],
  ],
  ifstream: [
    ['open', 'void open(const std::string& name)', '打开文件。'], ['close', 'void close()', '关闭文件。'],
    ['is_open', 'bool is_open() const', '是否成功打开。'], ['good', 'bool good() const', '流状态是否正常。'],
    ['fail', 'bool fail() const', '是否发生失败。'], ['eof', 'bool eof() const', '是否读到文件末尾。'],
    ['getline', 'std::istream& getline(char* s, std::streamsize n)', '读一行到字符数组。'],
  ],
  ofstream: [
    ['open', 'void open(const std::string& name)', '打开文件。'], ['close', 'void close()', '关闭文件。'],
    ['is_open', 'bool is_open() const', '是否成功打开。'], ['write', 'std::ostream& write(const char* s, std::streamsize n)', '写入字节。'],
    ['flush', 'std::ostream& flush()', '刷新缓冲区。'],
  ],
  fstream: [
    ['open', 'void open(const std::string& name, std::ios::openmode mode)', '打开文件。'],
    ['close', 'void close()', '关闭文件。'], ['is_open', 'bool is_open() const', '是否成功打开。'],
  ],
  unique_ptr: [
    ['get', 'T* get() const', '取出裸指针（不释放所有权）。'], ['reset', 'void reset(T* p = nullptr)', '替换所管理对象。'],
    ['release', 'T* release()', '放弃所有权并返回裸指针。'], ['swap', 'void swap(unique_ptr& other)', '交换两个指针。'],
  ],
  shared_ptr: [
    ['get', 'T* get() const', '取出裸指针。'], ['reset', 'void reset()', '释放引用。'],
    ['use_count', 'long use_count() const', '引用计数。'], ['unique', 'bool unique() const', '是否唯一持有。'],
  ],
};

const KIND_ORDER = { '变量': 0, '函数': 1, '类型': 2, '宏': 3, '关键字': 4, '成员': 5 };

function maskSource(src) {
  let out = '';
  let i = 0;
  while (i < src.length) {
    const c = src[i];
    if (c === '/' && src[i + 1] === '/') {
      while (i < src.length && src[i] !== '\n') { out += ' '; i++; }
    } else if (c === '/' && src[i + 1] === '*') {
      out += '  '; i += 2;
      while (i < src.length && !(src[i] === '*' && src[i + 1] === '/')) { out += src[i] === '\n' ? '\n' : ' '; i++; }
      if (i < src.length) { out += '  '; i += 2; }
    } else if (c === '"' || c === "'") {
      const q = c; out += ' '; i++;
      while (i < src.length && src[i] !== q) { if (src[i] === '\\') { out += ' '; i++; } if (i < src.length) { out += src[i] === '\n' ? '\n' : ' '; i++; } }
      if (i < src.length) { out += ' '; i++; }
    } else { out += c; i++; }
  }
  return out;
}

function stripType(t) {
  return t.replace(/\b(const|constexpr|static|volatile|inline|mutable|typename|struct|class)\b/g, ' ')
    .replace(/[&*]/g, ' ').replace(/\s+/g, ' ').trim();
}

function baseType(t) {
  const s = stripType(t).replace(/^std::/, '');
  const name = s.split('<')[0].trim().split('::').pop();
  return name || '';
}

const RESERVED = new Set(KEYWORDS);

function parseDoc(text) {
  const masked = maskSource(text);
  const includes = new Set();
  for (const m of masked.matchAll(/#\s*include\s*[<"]([^>"]+)[>"]/g)) includes.add(m[1]);

  const macros = new Set();
  for (const m of masked.matchAll(/#\s*define\s+([A-Za-z_]\w*)/g)) macros.add(m[1]);

  const vars = new Map();   // name -> {type, sig, kind}
  const funcs = new Map();  // name -> {sig, ret}
  const classes = new Map(); // name -> {kind, members:[{n,sig,d}]}

  // 类 / 结构体
  for (const m of masked.matchAll(/\b(class|struct)\s+([A-Za-z_]\w*)\s*(?::[^{;]*)?\{/g)) {
    const name = m[2];
    const start = m.index + m[0].length - 1;
    let depth = 0, end = start;
    for (; end < masked.length; end++) {
      if (masked[end] === '{') depth++;
      else if (masked[end] === '}') { depth--; if (depth === 0) break; }
    }
    const body = masked.slice(start + 1, end);
    const members = [];
    for (const mm of body.matchAll(/(?:^|[;{}\n])\s*([A-Za-z_][\w:<>,\s\*&]*?)\s+([A-Za-z_]\w*)\s*(\(([^;{}()]*)\))?\s*[;{=]/g)) {
      const type = stripType(mm[1]);
      if (!type || RESERVED.has(type) || /^(return|else|public|private|protected)$/.test(mm[1].trim())) continue;
      members.push({ n: mm[2], sig: (mm[3] ? mm[3] : ''), d: mm[3] ? '成员函数' : '成员变量：' + type });
    }
    classes.set(name, { kind: m[1], members });
  }

  // 函数
  for (const m of masked.matchAll(/(?:^|[;}\n])\s*(?:static\s+|inline\s+|constexpr\s+)*([A-Za-z_][\w:<>,\s\*&]*?)\s+([A-Za-z_]\w*)\s*\(([^;{}]*)\)\s*(?:const\s*)?[;{]/g)) {
    const ret = stripType(m[1]);
    const name = m[2];
    if (!ret || RESERVED.has(name) || /^(return|else|if|for|while|switch)$/.test(ret)) continue;
    if (funcs.has(name) || vars.has(name) || classes.has(name)) continue;
    funcs.set(name, { ret, sig: ret + ' ' + name + '(' + m[3].trim() + ')' });
  }

  // 变量（含成员、参数、局部）
  for (const m of masked.matchAll(/(?:^|[;{}(,\n]\s*)(?:const\s+|constexpr\s+|static\s+)*([A-Za-z_][\w:<>,\s\*&]*?)\s+([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*(?:[=;,)])/g)) {
    const type = stripType(m[1]);
    const name = m[2];
    if (!type || RESERVED.has(name)) continue;
    if (/^(return|else|if|for|while|switch|do|case|new|delete|using|namespace|typedef|template)$/.test(type)) continue;
    if (/\b(return|throw|delete|new|using|typedef|namespace|template)\b/.test(m[1])) continue;
    if (vars.has(name) || funcs.has(name) || classes.has(name)) continue;
    vars.set(name, { type, base: baseType(type), sig: type + ' ' + name });
  }

  return { includes, macros, vars, funcs, classes };
}

function stdFor(parsed) {
  if (!parsed || parsed.includes.size === 0) return STD;
  const list = [];
  for (const s of STD) {
    if (parsed.includes.has(s.h)) list.push(s);
  }
  // 常见组合：iostream 往往也想要 string / vector 等常用项
  return list;
}

/* 按我们自己的顺序排：CodeMirror 默认按 label 字母序，会把 count 排在 cout 前面。 */
function withRank(options) {
  for (let i = 0; i < options.length; ++i) {
    options[i].sortText = String(i).padStart(4, '0') + options[i].label;
  }
  return options;
}

function optionFor(name, kind, detail, info, boost) {
  return { label: name, type: kind, detail: detail || '', info: info || '', boost: boost || 0 };
}

function generalOptions(parsed, prefix) {
  const out = [];
  const seen = new Set();
  const add = (opt) => { if (!seen.has(opt.label)) { seen.add(opt.label); out.push(opt); } };

  if (parsed) {
    for (const [name, v] of parsed.vars) add(optionFor(name, '变量', v.sig, '当前文件中声明的变量。', 4));
    for (const [name, f] of parsed.funcs) add(optionFor(name, '函数', f.sig, '当前文件中声明的函数。', 3));
    for (const [name, c] of parsed.classes) add(optionFor(name, '类型', c.kind + ' ' + name, '当前文件中声明的' + (c.kind === 'struct' ? '结构体' : '类') + '。', 3));
    for (const name of parsed.macros) add(optionFor(name, '宏', '#define ' + name, '当前文件中的宏。', 2));
    for (const s of stdFor(parsed)) add(optionFor('std::' + s.n, s.k, s.sig, s.d + '\\n（头文件 <' + s.h + '>）', 1));
  }
  for (const k of KEYWORDS) add(optionFor(k, '关键字', 'C++ 关键字', '', 0));
  for (const t of BUILTIN_TYPES) add(optionFor(t, '类型', t, '内建类型。', 0));

  out.sort((a, b) => (KIND_ORDER[a.type] || 9) - (KIND_ORDER[b.type] || 9));
  return withRank(out);
}

function memberOptions(parsed, objName, prefix) {
  const v = parsed && parsed.vars.get(objName);
  if (!v) return null;
  const base = v.base;
  const list = MEMBERS[base];
  if (list) {
    return { from: base, options: withRank(list.map(([n, sig, d]) => optionFor(n, '成员', sig, d, 0))) };
  }
  const cls = parsed && parsed.classes.get(base);
  if (cls) {
    return { from: base, options: withRank(cls.members.map((m) => optionFor(m.n, '成员', m.sig, m.d, 0))) };
  }
  return { from: base, options: [] };
}

export function cppCompletionSource(context) {
  const { state, pos, explicit } = context;
  const line = state.doc.lineAt(pos);
  const before = state.sliceDoc(line.from, pos);

  const parsed = parseDoc(state.doc.toString());

  // 成员：obj. 或 obj->
  const memberMatch = before.match(/([A-Za-z_]\w*)\s*(?:\.|->)\s*([A-Za-z_]\w*)?$/);
  if (memberMatch) {
    const res = memberOptions(parsed, memberMatch[1], memberMatch[2] || '');
    if (res) {
      const partial = memberMatch[2] || '';
      return {
        from: pos - partial.length,
        options: res.options.map((o) => ({ ...o, info: (parsed.vars.get(memberMatch[1]).base) + '.' + o.label })),
        validFor: /^[A-Za-z_]\w*$/,
      };
    }
    return null;
  }

  // 命名空间：std::
  const nsMatch = before.match(/([A-Za-z_]\w*)\s*::\s*([A-Za-z_]\w*)?$/);
  if (nsMatch && nsMatch[1] === 'std') {
    const partial = nsMatch[2] || '';
    const options = [];
    const seen = new Set();
    for (const s of stdFor(parsed)) {
      if (seen.has(s.n)) continue;
      seen.add(s.n);
      options.push(optionFor(s.n, s.k, s.sig, s.d + '\\n（头文件 <' + s.h + '>）', 0));
    }
    return { from: pos - partial.length, options: withRank(options), validFor: /^[A-Za-z_]\w*$/ };
  }

  const word = context.matchBefore(/[A-Za-z_]\w*/);
  if (!explicit && (!word || word.from === word.to)) return null;

  const options = generalOptions(parsed, word ? word.text : '');
  return {
    from: word ? word.from : pos,
    options,
    validFor: /^[A-Za-z_]\w*$/,
  };
}

// ---- 悬停 --------------------------------------------------------------
function hoverCard(title, kind, sig, doc) {
  const dom = document.createElement('div');
  dom.className = 'ex-hover';
  const head = document.createElement('div');
  head.className = 'ex-hover-head';
  const kindEl = document.createElement('span');
  kindEl.className = 'ex-hover-kind';
  kindEl.textContent = kind;
  const nameEl = document.createElement('code');
  nameEl.textContent = title;
  head.appendChild(kindEl);
  head.appendChild(nameEl);
  dom.appendChild(head);
  if (sig && sig !== title) {
    const sigEl = document.createElement('div');
    sigEl.className = 'ex-hover-sig';
    sigEl.textContent = sig;
    dom.appendChild(sigEl);
  }
  if (doc) {
    const docEl = document.createElement('div');
    docEl.className = 'ex-hover-doc';
    docEl.textContent = doc;
    dom.appendChild(docEl);
  }
  return { dom };
}

export function cppHover(view, pos) {
  const word = view.state.wordAt(pos);
  if (!word) return null;
  const text = view.state.sliceDoc(word.from, word.to);
  if (!text || /^\d+$/.test(text)) return null;

  const parsed = parseDoc(view.state.doc.toString());
  const from = word.from;
  const to = word.to;

  const local = parsed.vars.get(text);
  if (local) {
    return { pos: from, end: to, create: () => hoverCard(text, '变量', local.sig, '当前文件中声明的变量，类型为 ' + local.type + '。') };
  }
  const fn = parsed.funcs.get(text);
  if (fn) {
    return { pos: from, end: to, create: () => hoverCard(text, '函数', fn.sig, '当前文件中声明的函数。') };
  }
  const cls = parsed.classes.get(text);
  if (cls) {
    return { pos: from, end: to, create: () => hoverCard(text, '类型', cls.kind + ' ' + text, (cls.kind === 'struct' ? '结构体' : '类') + '，共 ' + cls.members.length + ' 个成员。') };
  }
  const std = STD.find((s) => s.n === text);
  if (std) {
    return { pos: from, end: to, create: () => hoverCard('std::' + text, std.k, std.sig, std.d + '（头文件 <' + std.h + '>）') };
  }
  if (KEYWORDS.indexOf(text) >= 0) {
    return { pos: from, end: to, create: () => hoverCard(text, '关键字', 'C++ 关键字', '') };
  }
  if (BUILTIN_TYPES.indexOf(text) >= 0) {
    return { pos: from, end: to, create: () => hoverCard(text, '类型', text, '内建类型。') };
  }
  if (parsed.macros.has(text)) {
    return { pos: from, end: to, create: () => hoverCard(text, '宏', '#define ' + text, '当前文件中的宏。') };
  }
  return null;
}
