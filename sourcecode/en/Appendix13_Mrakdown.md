# Appendix 13: Markdown

> This appendix introduces what Markdown is and how to use it. Markdown is a lightweight markup language that lets you write documents in plain text and then convert them to clean, formatted HTML. It is much simpler than Word and much nicer to look at than Notepad. This appendix covers only the most basic syntax. For more detailed tutorials, please visit `markdown.com.cn`.

## 13.1 What Is Markdown?

**Markdown** is a plain‑text markup language. Instead of using your mouse to click a "bold" button, you just add a few symbols around your text. For example:

```markdown
This is **bold** text, and this is *italic* text.
```

Markdown files usually have the `.md` extension. You can open and edit them with any text editor (Notepad, VS Code). After writing, you can convert the file to HTML, PDF, Word, or other formats.

**What can you do with Markdown?**
- Write README files (the project descriptions you see on GitHub are almost all Markdown)
- Write blog posts (many static blog systems such as Hexo, Hugo, and VuePress use Markdown)
- Take notes (Obsidian, Typora, and Notion all support Markdown)
- Write technical documentation (many open source projects use Markdown for their docs)
- In fact, all the content of this site was written in Markdown

Markdown's biggest feature is that it is **plain text** – you can open it on any computer without worrying about layout breaking, unlike `.docx` files. The syntax is extremely simple; you can learn it in ten minutes.

## 13.2 Basic Syntax

Below is the most commonly used Markdown syntax. For a more complete reference, see the "Basic Syntax" section on [markdown.com.cn](https://markdown.com.cn).

### 13.2.1 Headings

Use `#` to indicate a heading. The number of `#` symbols indicates the heading level. Always put a space after the `#`.

```markdown
# Heading 1
## Heading 2
### Heading 3
#### Heading 4
```

### 13.2.2 Paragraphs and Line Breaks

Separate paragraphs with a blank line. If you only want a line break (without starting a new paragraph), add two spaces at the end of the line, then press Enter.

```markdown
This is the first paragraph.

This is the second paragraph.

This is the first line··(two spaces)
This is the second line of the same paragraph.
```

### 13.2.3 Emphasis

```markdown
*italic* or _italic_
**bold** or __bold__
***bold and italic*** or ___bold and italic___
```

### 13.2.4 Lists

**Unordered lists**: Use `-`, `*`, or `+`, followed by a space.

```markdown
- Apple
- Banana
- Orange
```

**Ordered lists**: Use a number followed by a period, then a space. The numbers do not have to be sequential – Markdown will renumber them automatically.

```markdown
1. First item
2. Second item
3. Third item
```

### 13.2.5 Links

```markdown
[link text](https://example.com)
```

### 13.2.6 Images

```markdown
![alt text](image-url)
```

### 13.2.7 Blockquotes

Use `>` to indicate a blockquote. You can nest them (`>>`).

```markdown
> This is a blockquote.
> 
> > This is a nested blockquote.
```

### 13.2.8 Code

**Inline code**: Wrap the code with backticks (the key above Tab, to the left of `1`).

```markdown
This is the `printf("hello")` function.
```

**Code blocks**: Wrap the code with three backticks. You can specify a language for syntax highlighting.

~~~markdown
```python
def hello():
    print("Hello, world!")
```
~~~

### 13.2.9 Horizontal Rules

Use three or more `-`, `*`, or `_` on a line by themselves.

```markdown
---
```

### 13.2.10 Tables

Use `|` to separate cells and `-` to separate the header from the body.

```markdown
| Name  | Age | City     |
|-------|-----|----------|
| Zhang San | 25  | Beijing  |
| Li Si     | 30  | Shanghai |
```

### 13.2.11 Task Lists

Use `- [ ]` for incomplete tasks and `- [x]` for completed tasks.

```markdown
- [x] Finished chapter 1
- [ ] Finished chapter 2
- [ ] Get someone to proofread
```

## 13.3 Extended Syntax

Standard Markdown is fairly basic. **Extended syntax** adds more useful features: strikethrough, automatic links, footnotes, definition lists, Emoji, highlighting, subscripts, superscripts, and more. However, not all Markdown renderers support extended syntax.

Examples of common extended syntax:

```markdown
~~strikethrough~~

:joy: (Emoji – `:joy:` gives 😂)

Here is a footnote.[^1]

[^1]: The footnote content.
```

For a detailed explanation of extended syntax, see the "Extended Syntax" section on `markdown.com.cn`.

## 13.4 What Tools Can You Use?

- **VS Code**: Install the `Markdown All in One` extension. You can preview, generate tables of contents, and export to HTML.
- **Typora**: A dedicated Markdown editor that is "what you see is what you get" – you type the syntax, and it shows the rendered result in real time. Clean interface, good for beginners.
- **Obsidian**: A knowledge management tool that supports Markdown. Great for building a personal note archive.
- **Any text editor**: Even Notepad works, though you will not get preview.

## 13.5 A Final Thought

Markdown is not a replacement for Word – for complex layouts, exact margins, or academic references, you should still use Word or LaTeX. But for everyday notes, documents, READMEs, and blog posts, Markdown is the most hassle‑free way to write.

You do not need to memorise all the syntax. Just remember `#` for headings, `**` for bold, `[link]()`, and `![image]()`. That is enough. Look up the rest when you need it.