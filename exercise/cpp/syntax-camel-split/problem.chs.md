给定一个不含空格的驼峰标识符，把它拆成小写单词，单词之间用一个空格分隔。

拆分规则：

1. 连续的大写字母视为一段缩写；当缩写后面紧跟小写字母时，这段缩写的最后一个大写字母属于下一个单词。
2. 在小写字母或数字之后出现大写字母，开始一个新单词。
3. 数字留在它前面的单词里。
4. 所有字母都转成小写。

几个例子：

| 输入 | 输出 |
| --- | --- |
| `camelCase` | `camel case` |
| `HTTPServer` | `http server` |
| `XMLHttpRequest` | `xml http request` |
| `getHTTPResponseCode` | `get http response code` |
| `parse2Json` | `parse2 json` |
| `JSON2XML` | `json2 xml` |

**输入**：

- 一行一个标识符，只含 ASCII 字母和数字，首字符是字母，长度 `1` 到 `60`。

**输出**：

- 一行，拆分后的单词，以单个空格分隔。

### 示例

输入：

```text
getHTTPResponseCode
```

输出：

```text
get http response code
```

### 约束

- 输入只有一个单词，没有空格和标点。
- 输出不能有多余的前导、尾随空格或连续空格。
- 注意 `HTTPServer`：`S` 既是缩写 `HTTPS` 的结尾，也是单词 `Server` 的开头，但输出中 `http` 和 `server` 是两个单词。
