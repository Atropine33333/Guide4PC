# Appendix 10: Office

> This appendix covers the core features of Microsoft Office's three main components: Word, Excel, and PowerPoint. You'll rely on them throughout college for writing papers, delivering presentations, and working with data.  
> I'm focusing on Microsoft Office (specifically Office 365). WPS Office works similarly but has minor differences in behavior. I recommend Office 365—though if your instructor explicitly requires WPS for your thesis (which is rare), it's safer to follow that rule, because format handling between MS Office and WPS can be subtly, almost mysteriously, inconsistent. Opening an MS Office document in WPS (or vice versa) can shift margins, line spacing, or font rendering. Before submitting your final draft, always open it in the software your instructor uses and double‑check the layout.  
> In the next appendix I'll introduce LaTeX, which is even more powerful for academic typesetting—most universities provide a LaTeX thesis template anyway.

---

## Part 1: Word

Word is a document‑editing tool. You'll use it for papers, resumes, assignments, and more. Below are the features that actually matter.

### 1.1 Styles

A **Style** is a predefined bundle of formatting—font, size, bold/italic, color, paragraph spacing, and so on.

Many people write by selecting text and manually tweaking the font size, bolding, and line spacing as they go. That works for a one‑page memo, but for a thesis with dozens of pages and multiple heading levels, it's tedious and a nightmare to revise.

The right way: define your styles first, then apply them.

Word comes with built‑in styles like "Heading 1", "Heading 2", and "Normal". Right‑click any style → Modify to set its font, size, line spacing, and before/after paragraph spacing. Once you change a style definition, every piece of text using that style updates automatically.

**Benefits**:
- Consistent formatting everywhere—no more "this heading is Songti size 16, that one is Heiti size 15".
- Fast updates—change the style once, and the entire document reflects it.
- Enables automatic table of contents generation (via the References feature).

### 1.2 Page Breaks vs. Line Breaks

When you need to start a new page (e.g., each thesis chapter on a fresh page), many people hammer the Enter key until they reach the next page. That's fragile—any small edit earlier will break the layout.

The proper methods:

- **`Ctrl + Enter`** inserts a **Page Break**. Everything after the cursor moves to the next page. The break stays fixed regardless of edits above.
- **`Shift + Enter`** inserts a **Line Break** (not a paragraph break). Enter creates a new paragraph with spacing between them; `Shift + Enter` just moves to the next line without starting a new paragraph—useful for addresses, poems, or any place where you want a new line but no extra gap.

### 1.3 Layout

The **Layout** tab controls the overall page appearance:

- **Margins**: the blank space around your document. Theses usually have strict margin requirements (e.g., top 2.5cm, bottom 2.5cm, left 3cm, right 2cm). Use "Custom Margins" for precise values.
- **Orientation**: Portrait vs. Landscape. If a table is too wide to fit, you can make that single page landscape by choosing "Apply to this section" in Page Setup.
- **Columns**: split a page into two or more columns—handy for newsletters or pamphlets.

### 1.4 References

The **References** tab is your thesis‑writing powerhouse.

**Table of Contents**: As long as you've used heading styles (Heading 1, 2, etc.), click "Table of Contents" and Word generates one instantly. Page numbers update automatically when the document changes—just click "Update Table" if you add or remove headings.

**Footnotes and Endnotes**:
- Footnotes appear at the bottom of the current page. Insert a superscript number in the main text, and the note sits at the page bottom—great for supplemental comments.
- Endnotes appear at the end of the document—sometimes used for references.

**Citations & Bibliography**:
- Click "Insert Citation" → "Add New Source", fill in author, title, year, etc. Word formats the citation for you.
- At the end of your document, click "Bibliography" to auto‑generate the reference list.
- Different journals/schools require different styles (APA, MLA, Chicago, etc.)—just pick the one you need from the "Style" dropdown.

**Captions**: After inserting a figure, table, or equation, select it and click "Insert Caption" to add numbered labels (Figure 1, Table 1, etc.). Combine with "Cross‑reference" to write "as shown in Figure 1" in your text—clicking it jumps to the figure. If you insert a new figure in between, all numbers update automatically.

### 1.5 Review

The **Review** tab is heavily used during college—especially when your instructor marks up your paper.

**Comments**: Select some text and click "New Comment"—a bubble appears on the right. Instructors can write things like "logic unclear here" or "add a reference". After you address it, you can "Reply" or "Resolve" the comment.

**Track Changes**: When you turn this on, every edit (deletions, insertions, moves) appears in colored markup. Your instructor sees exactly what you changed, and you see their edits. The markup stays until you accept or reject each change.

**Compare**: You have two versions of a paper and want to know what's different? Click "Compare", select the original and the revised document, and Word generates a side‑by‑side comparison highlighting every change.

### 1.6 Important Settings in Word Options

Click File → Options to open the Word Options dialog. A few settings are worth knowing:

**General → User Interface Options**: Change the color scheme to "Dark Gray" or "Black" to reduce eye strain during long writing sessions.

**Display**:
- "Always show these formatting marks on screen": I recommend checking "Tabs", "Spaces", and "Paragraph marks". You'll see gray dots (spaces) and pilcrow signs (paragraph breaks) that help debug misalignment.
- **Never change the document page color**: Some people set a green or dark background for comfort, but your final reader (instructor, journal reviewer) sees plain white paper. If you pick light‑colored text on a dark background, it may become invisible on white. Keep the default, or at least check how it looks before printing/submitting.

**Save**:
- "Save AutoRecover information": defaults to 10 minutes. For critical documents, you might shorten it to 5.
- "Default file format": choose `Word Document (*.docx)`. `.doc` is the older format with limited compatibility.
- **Embed fonts**: if you use a non‑standard font, recipients without it will see a fallback font and the layout may break. Check "Embed fonts in the file". Note that some fonts have license restrictions that prevent embedding.

**Advanced**:
- **Image fidelity**: Word compresses inserted images by default to save space. If you need high‑quality figures (e.g., for a printed thesis), go to "Image Size and Quality" and set "Default resolution" to "High fidelity".
- **Cut, copy, and paste**: When pasting from a webpage, you often bring along unwanted formatting (background colors, weird fonts). Under "Pasting from other programs", choose "Keep Text Only" so you get clean plain text.
- **Show document content**: you'll find options like "Show background colors and images in Print Layout view".

**Add‑ins**: these extend Word's capabilities—for example, Zotero (reference manager) installs an add‑in here so you can insert citations directly. If an add‑in slows Word down or causes errors, you can disable it here.

### 1.7 DOTM Templates

**.dotm** is the Word template format that supports macros. Normal documents are `.docx`, templates are `.dotx`, and macro‑enabled templates are `.dotm`.

A **template** is like a mold. Create a resume template with margins, styles, headers, etc., save it as `.dotx`. Every time you double‑click it, Word generates a new document that inherits all settings—the original template remains untouched.

Some universities provide a thesis `.dotm` template with pre‑set formatting and maybe automation scripts (e.g., one‑click figure number checks). If it's `.dotm`, you'll be prompted to "Enable Macros" when opening—you need to allow that to use the full functionality.

**Where to place templates**: Word looks for templates in `C:\Users\YourUsername\Documents\Custom Office Templates`. Put yours there, then they appear under File → New → Personal.

---

### Word Further Reading

#### Extra 1: Styles and Table of Contents linkage

When generating a TOC, Word only picks up "Heading 1", "Heading 2", and "Heading 3" by default. If you use custom styles (e.g., "Level 1 Heading"), you must manually map them via References → Table of Contents → Custom Table of Contents → Options → assign the appropriate outline level.

#### Extra 2: Section Breaks

A **Section Break** is more powerful than a page break. After inserting one, each section can have independent margins, headers/footers, and page orientation. Example: your preface and TOC use Roman numerals, while the main body uses Arabic numerals—you need a section break between them.

How to insert: Layout → Breaks → Next Page (Section Break).

#### Extra 3: Document Page Color

The "Never change the document page color" option in Word Options is aimed at people who write in dark mode. Windows 11's dark mode can make Word appear black‑on‑white, but the actual document background remains white. If someone changes the font color to a light shade while in dark mode, it may become unreadable on a white background for the instructor. Staying with the default is safest.

---

## Part 2: Excel

Excel is a spreadsheet tool for handling data, performing calculations, and creating charts. Many people just type into cells and dread formulas. This section doesn't dive into complex functions—only the ones you'll actually use in college.

### 2.1 Basic Operations

A **Cell** is the smallest unit, identified by column letter and row number—e.g., `B3` means column 2, row 3.

A **Range** is a contiguous block of cells, denoted by the top‑left and bottom‑right cells with a colon—e.g., `A1:C10` covers from A1 to C10.

**Common shortcuts**:

| Action | Method |
|--------|--------|
| Select entire row | Click the row number |
| Select entire column | Click the column header |
| Select entire sheet | Click the triangle at the top‑left (intersection of A and 1) |
| Fill same content | Select a cell, drag the small square (fill handle) at its bottom‑right |
| Fill sequence | Hold `Ctrl` while dragging the fill handle (or enter two values so Excel detects the pattern) |
| Auto‑fit column width / row height | Double‑click the separator between column/row headers |

### 2.2 Formulas and Functions

Formulas start with `=`. For example, type `=1+2` in a cell and press Enter—it shows `3`.

**Frequently used functions**:

| Function | Purpose | Example |
|----------|---------|---------|
| `SUM` | Sum of numbers | `=SUM(A1:A10)` |
| `AVERAGE` | Average | `=AVERAGE(B2:B20)` |
| `COUNT` | Count numeric cells | `=COUNT(C1:C100)` |
| `COUNTA` | Count non‑empty cells | `=COUNTA(D:D)` |
| `MAX` / `MIN` | Maximum / minimum | `=MAX(E2:E50)` |
| `IF` | Conditional logic | `=IF(F2>=60, "Pass", "Fail")` |

**Important: Relative vs. Absolute References**

In a formula, `A1` is a relative reference—dragging it down changes to `A2`, `A3`, etc. `$A$1` is absolute—it stays fixed. Mixed references like `$A1` (column fixed, row variable) and `A$1` (row fixed, column variable) have their uses.

Press `F4` to cycle through the four reference modes quickly.

**Example**: Calculate each sale's percentage of total sales. Assume sales figures in `B2:B10`, total in `B11`. The percentage formula is `=B2/$B$11`—lock `$B$11` so it doesn't change when you drag down.

### 2.3 Lookup and Reference Functions

These are Excel's power tools for finding data based on a key.

**VLOOKUP (vertical lookup)**

Syntax: `=VLOOKUP(what_to_find, where_to_look, column_number, 0)`

Example: You have a student score table—column A is student ID, B is name, C is score. To find the score for a given ID:
`=VLOOKUP(F2, A:C, 3, 0)`
- F2: the ID to search for
- A:C: search within columns A through C (the first column must be the ID column)
- 3: return the value from the 3rd column (C) of the matching row
- 0: exact match (do not omit this)

**Caveats**:
- The lookup value must be in the first column of the lookup range.
- VLOOKUP only searches left‑to‑right; you cannot return values from columns to the left of the lookup column.
- Always use `0` (or `FALSE`) for exact match—don't leave it out.

**XLOOKUP (Office 365 only)**

`XLOOKUP` is an upgraded version that solves many VLOOKUP limitations. Simpler syntax, can look left, and you can specify a custom "not found" message.

Syntax: `=XLOOKUP(what_to_find, where_to_look, return_column, "not found")`

Example: `=XLOOKUP(F2, A:A, C:C, "No such ID")`

**Version awareness**:
- Office 2021 and Office 365 support `XLOOKUP`, `FILTER`, `SORT`, and other new functions.
- Office 2019 and older do not—if you send a file with `XLOOKUP` to someone on an older version, they'll see `#NAME?` errors.

**INDEX + MATCH (universal combination, works in all versions)**

`INDEX` returns the value at a given row and column within a range. `MATCH` finds the position of a value in a list. Combined, they enable two‑way lookups.

Example: Find a score based on student name and subject name:
`=INDEX(score_range, MATCH(name, name_column, 0), MATCH(subject, subject_row, 0))`

### 2.4 Text and Date Handling

**Text functions**:

| Function | Purpose | Example |
|----------|---------|---------|
| `LEFT(text, n)` | Take first n characters | `=LEFT("2024001", 4)` → "2024" |
| `RIGHT(text, n)` | Take last n characters | `=RIGHT(A2, 6)` |
| `MID(text, start, n)` | Extract n characters starting at position | `=MID("2024-09-01", 6, 2)` → "09" |
| `&` operator | Concatenate text | `=B2 & " " & C2` |
| `TEXT(value, format)` | Format a number or date | `=TEXT(A2, "yyyy-mm-dd")` |

**Date functions**:

| Function | Purpose | Example |
|----------|---------|---------|
| `TODAY()` | Current date (auto‑updates daily) | |
| `NOW()` | Current date and time | |
| `YEAR(date)` | Extract year | `=YEAR(A2)` |
| `MONTH(date)` | Extract month | |
| `DAY(date)` | Extract day | |
| `DATEDIF(start, end, unit)` | Difference between two dates | `=DATEDIF(A2, TODAY(), "y")` calculates age |

### 2.5 Conditional Statistics

| Function | Purpose | Example |
|----------|---------|---------|
| `COUNTIF(range, condition)` | Count cells meeting a condition | `=COUNTIF(B:B, ">=60")` |
| `SUMIF(range, condition, sum_range)` | Sum cells meeting a condition | `=SUMIF(A:A, "Sales", B:B)` |
| `AVERAGEIF(range, condition, avg_range)` | Average cells meeting a condition | `=AVERAGEIF(C:C, ">0", C:C)` |

### 2.6 Pivot Tables

A **Pivot Table** is Excel's most powerful data analysis tool. With a few clicks, you can summarize thousands of rows.

**Scenario**: You have a school‑wide student score table with columns "Class", "Name", "Subject", and "Score"—5,000 rows. You want the average score per class, the highest score per subject, or a cross‑tabulation of class vs. subject. A pivot table does that in seconds.

**Steps**:
1. Select the entire data range.
2. Click Insert → PivotTable.
3. In the popup, choose "New Worksheet".
4. In the PivotTable Fields panel, drag fields into areas:
   - **Rows**: what to group by (e.g., "Class")
   - **Columns**: what to cross‑tabulate (e.g., "Subject")
   - **Values**: what to calculate (e.g., "Score"—default is Sum; click "Value Field Settings" to change to Average or Count)

### 2.7 Charts

Select your data → Insert → Chart. Common types:
- **Column chart**: compare values across categories.
- **Line chart**: show trends over time.
- **Pie chart**: show proportions of a whole (keep it under 5 slices, or it becomes cluttered).

Add titles, data labels, and axis descriptions. Right‑click the chart to change chart type, palette, add trendlines, etc.

### 2.8 Conditional Formatting

**Conditional Formatting** automatically colors cells based on their values.

Select your data → Home → Conditional Formatting. Useful rules:
- Highlight cells rules: mark values greater than a threshold in red, duplicates in yellow.
- Color scales: use color intensity to represent value (green = high, red = low).
- Data bars: draw a bar inside the cell proportional to the value—no extra space needed.
- Icon sets: use traffic lights or arrows to indicate performance.

### 2.9 Data Validation

**Data Validation** restricts what can be entered in a cell, preventing errors.

Select a cell → Data → Data Validation → Settings → "Allow" choose "List", then enter options in "Source" (comma‑separated, e.g., `Yes,No`). The cell becomes a dropdown menu.

**Common constraints**:
- List → dropdown menu
- Whole number → restrict numeric range (e.g., 0–100)
- Date → restrict date range
- Text length → restrict character count (e.g., phone number must be 11 digits)

### 2.10 Shortcuts

| Shortcut | Action |
|----------|--------|
| `Ctrl + Arrow` | Jump to edge of data region |
| `Ctrl + Shift + Arrow` | Select contiguous region |
| `Alt + =` | Auto‑sum (inserts SUM formula instantly) |
| `F4` | Repeat last action (e.g., insert row, apply format) |
| `F2` | Edit current cell (no double‑click) |
| `Ctrl + D` | Fill down (copy from cell above) |
| `Ctrl + R` | Fill right |
| `Ctrl + 1` | Open Format Cells dialog |
| `Ctrl + Shift + L` | Toggle filters |

---

### Excel Further Reading

#### Extra 1: Function differences between Office 2021 and Office 365

| Function | Office 2021 | Office 365 | Notes |
|----------|-------------|------------|-------|
| `XLOOKUP` | ✅ | ✅ | Supported in 2021 as well |
| `FILTER` | ❌ | ✅ | 365 only—auto‑filters and returns results |
| `SORT` | ❌ | ✅ | 365 only—auto‑sorts |
| `UNIQUE` | ❌ | ✅ | 365 only—extracts unique values |
| `TEXTJOIN` | ✅ | ✅ | Concatenates with a delimiter |
| `LET` | ❌ | ✅ | 365 only—defines variables to simplify formulas |
| `LAMBDA` | ❌ | ✅ | 365 only—creates custom functions |

If your school provides an Office 365 account, you have access to all of these. If you bought a perpetual‑license Office 2021, support for new functions is limited.

#### Extra 2: `FILTER` function example (Office 365)

`=FILTER(A:C, B:B="Sales", "No results")`—returns all rows from columns A–C where column B equals "Sales". No manual filtering or copying needed.

#### Extra 3: Excel Tables

Select your data and press `Ctrl + T` to convert a normal range into an **Excel Table**. Benefits:
- New rows automatically inherit formulas and formatting.
- Column headers come with built‑in filter buttons.
- Formulas use structured references like `=[@Sales]`, which are more readable.

---

## Part 3: PowerPoint

PowerPoint is a presentation tool. You'll use it for college reports, course presentations, thesis defenses, research talks, and project showcases. Many people think PPT is just "putting text on slides", but it's really about information design—how to organize content, emphasize key points, and guide your audience. This section covers the essentials and practical tips.

---

### 3.1 Where to Start

#### 3.1.1 What Goes on a Slide

For a slide that conveys a complete idea, include at least:

1. **A title**: the core question or conclusion for that page—<u>not</u> something vague like "Overview" or "Part 1".
2. **Body**: supporting evidence or bullet points, preferably <u>in short phrases rather than full sentences</u>.
3. **Notes/footer** (optional): data sources, page numbers, dates—adds clarity.

> **Important**: Don't copy‑paste entire paragraphs from a Word document onto a slide—it creates a painful reading experience. In Word, you need complete sentences to explain. In PPT, the audience reads the slide while *you* speak, so the on‑screen content should be concise—keywords and short phrases. **One slide = one core idea.** If it doesn't fit, move it to the next slide or delete it.

> —Nobody wants to read a wall of text projected on a screen.

![](/img/3.1-1.png)

#### 3.1.2 Information Hierarchy

Your slide should have a clear visual priority that guides viewers from most important to supporting details. Unlike Word, where linear reading is typical, PPT benefits from a visible structural anchor. You can place the current section title in a <u>corner</u> of each slide to keep the audience oriented.

**General guidelines**:
1. Font size directly signals importance. Avoid having multiple headings at nearly the same size on one page.
2. Use bold/color only for the most critical words—**<u>never bold entire paragraphs</u>**.
3. In body text, use short sentences and bullet points for easy scanning.

![](/img/3.1.2-1.png)

---

### 3.1.3 Basic Operations

For Office 2016 and newer, the layout is largely consistent. Here are the core areas you'll use for 90% of everyday tasks.

**Interface**

When you create a new presentation, you'll see a dense ribbon at the top with tabs like "File", "Home", etc.—this is the **ribbon**, where most slide operations live.

On the left, a thumbnail strip shows each slide. Here you can:
1. Right‑click <u>a slide</u> to copy or delete it.
2. Right‑click <u>empty space</u> to insert a new slide or add a **section**.
3. <u>Drag</u> slides to reorder them.

The center area is your editing workspace for the current slide.

> You might see a vertical toolbar on the right—this could be OfficePLUS, a Microsoft‑sponsored add‑in for the Chinese market. If you find it intrusive, you can disable it via Add‑ins.

---

**The most common operation: Insert**

Almost everything you add to a slide lives under the **Insert** tab.

1. **Insert a text box**: Insert → Text Box → choose "Horizontal" or "Vertical", then drag on the canvas to create a box and type. **This is one of the most frequent actions**—aside from title and content placeholders, you'll often need additional text boxes for custom content. <u>Also, put each distinct piece of text in its own text box—it makes positioning much easier later.</u>
2. **Insert a picture**: Insert → Pictures, **or simply drag an image file directly into the PowerPoint window**. To resize without distortion, <u>hold `Shift` while dragging a corner handle</u>.
![](/img/3.1.3-1.png)
3. **Insert a shape**: Insert → Shapes—choose arrows, rectangles, circles, lines, etc. Drag on the canvas to draw. After drawing, you can change fill color, outline, and even double‑click the shape to add text inside.
4. **Insert a table**: Insert → Table—drag in the grid to select row/column count, or click "Insert Table" to enter numbers manually. You can also **copy a table from Excel and paste it here**.
5. **Insert a chart**: Insert → Chart—choose column, line, pie, etc. A mini Excel sheet pops up—enter your data there, and the chart updates live.

> At the far right of the Insert tab, you'll find "Header & Footer"—use it to add slide numbers, dates, or footer text consistently across all slides. Much faster than doing it manually per slide, and you won't miss any. You can also do this via the Master (see 3.2).

---

**The most common operation: Modify**

Click any element on a slide (text box, picture, shape, chart) to select it—then you can modify it.

1. **Edit text**: click inside a text box, cursor appears—type or delete. Select the text, then use the **Home** tab to change **font**, **size**, **bold/italic/underline**, **color**, etc. Remember: font size is a direct way to show hierarchy—title, body, and auxiliary text should have clear size tiers. <u>Oh, and avoid the built‑in **WordArt** effects—they rarely look professional.</u>
2. **Edit a picture**: double‑click the image to enter the **Picture Format** tab. Here you can adjust **brightness/contrast**, **crop** (including crop to shape), **remove background** (Office 2016+ can do one‑click background removal—we'll cover it in 3.4.2), and add borders/shadows. All image adjustments live here.
3. **Edit a shape**: select a shape → **Shape Format** tab → change **fill color**, **outline color/thickness**, **shape effects** (shadow, glow, etc.). If a shape is blocking text, right‑click → "Send to Back" or "Bring to Front" (or move step‑by‑step) to adjust layering.
4. **Move and resize**: drag an element to reposition. Drag <u>corner</u> handles to <u>scale proportionally (hold `Shift`)</u>; drag <u>edge</u> handles to stretch in one direction (use sparingly—it often distorts).

> **Undo**: Click the **curved arrow** at the top‑left, or press **Ctrl + Z**, to step back. PPT supports multiple undo levels—experiment freely.

---

**View modes**

Besides the default "Normal" view (editing area on top, thumbnails on left), PPT offers other views accessible from the bottom‑right:

- **Normal view**: everyday editing—most used.
- **Slide Sorter**: thumbnails tiled, great for reordering, bulk deletion, or copying slides. Hold Ctrl to select multiple slides, then apply "Hide Slide" or delete in bulk.
- **Reading View**: like a slideshow but in a window—quick preview of the finished look without going full‑screen.

---

**Slide Show**

To present, press `F5` (start from beginning) or `Shift + F5` (start from current slide). Press `ESC` to exit.

---

**Saving your file**

<u>This is the most important yet most overlooked step.</u>

- After creating a new file, press `Ctrl + S` or click the floppy‑disk icon at top‑left to save.
- Build a habit: **<u>hit Ctrl + S every few minutes</u>**. PPT can crash—unsaved changes are gone forever.
- The default format is `.pptx`—the standard. If your recipient uses an older Office version (pre‑2007), you might need to save as `.ppt` (compatibility mode), but nowadays almost everyone supports `.pptx`.
- On first save, confirm the "Save as type" dropdown says "PowerPoint Presentation (*.pptx)", not template or show format.
- You can enable `AutoSave`, but earlier versions of Office may require the file to be stored in OneDrive for that to work. WPS Office users can enable it without that restriction.

---

Once you master these basics, you can start building a full presentation. Next, use the **Master** and **Layout** system to enforce consistency and avoid manual duplication.

### 3.2 The Slide Master

This is a crucial PowerPoint feature that most beginners never touch. It's like **Styles in Word**: change it once, and every slide using that layout updates instantly.

#### What Is the Slide Master

The Slide Master is the underlying "blueprint" for each slide. Anything you add or change on the Master automatically appears on all slides using that layout. Use it to control:

- Fonts (what font and size for headings vs. body)
- Placement of title and content placeholders
- Page numbers and dates
- Fixed elements (e.g., school logo, institution name)
- Background color or image

![](/img/3.2-1.png)

#### A Practical Example

Suppose you're making a 20‑slide class presentation and manually insert the school logo in the top‑right corner of each slide. Later, your instructor says to move it to the bottom‑left—now you have to edit all 20 slides manually, risking missing one. Using the Master, you insert the logo once in the top‑level Master, and every slide reflects the change instantly.

**How to do it**:
1. View → Slide Master.
2. The left pane shows a set of Master and layout thumbnails. The topmost (largest) controls all layouts.
3. On a specific layout (e.g., "Title and Content"), add your logo, adjust fonts, etc. After closing the Master view, all slides using that layout update automatically.

**Best practice**: Set up your Master *before* you start creating content—fonts, colors, logo placement. If you already have dozens of slides with manual overrides, changing the Master later can cause conflicts and messy results.

---

### 3.3 Themes and Layouts

#### Themes

A **Theme** is a coordinated visual package—font combinations, color palettes, background styles. In the **Design** tab, you can change the entire presentation's look with one click.

- **Office built‑in themes**: usable, but many are overly flashy and not suitable for academic presentations.
- **Custom themes**: it's better to build one based on your university's official colors (from the logo). Go to Design → Variants → Colors → Customize Colors, set Accent 1–6 to your preferred palette—then all charts and shapes in the presentation will follow these colors.

**Recommendation**: <u>Avoid downloading fancy commercial templates with heavy backgrounds and decorative graphics.</u> For academic talks and defenses, keep the background clean and text legible—no distractions.

#### Layouts

Layouts are predefined page structures under the Master. They tell PowerPoint "where the title goes, where the body goes, and whether there's a picture placeholder."

Common layouts:
- **Title Slide**: for the cover—large title and subtitle only.
- **Title and Content**: the workhorse content page—title plus body/chart area.
- **Section Header**: for chapter transitions.
- **Blank**: fully custom—good for full‑page images or your own diagrams.
- **Comparison**: two content areas side‑by‑side for contrast.

> **Tip**: Don't start every slide from "Blank" and manually add text boxes. Instead, when inserting a new slide, **choose a suitable layout and just fill in the content**. This keeps positions consistent and makes future Master‑based changes much easier.

---

### 3.4 Working with Pictures and Shapes

#### 3.4.1 Key Points

- **Maintain proportions**: When resizing, hold **Shift** and drag a corner handle to keep the original aspect ratio. **Never stretch one‑dimensionally**—faces get squashed, charts become distorted. <u>A slightly smaller image is fine; a deformed one ruins the layout.</u>
- **Pictures should serve your point**: Don't add random decoration. Every image should have a reason—to prove a conclusion, show a result, or illustrate a process.

#### 3.4.2 Cropping and Background Removal

PowerPoint's built‑in cropping is more powerful than you might think:

- **Crop to ratio**: Select the image → Picture Format → Crop → Aspect Ratio—quickly crop to 1:1, 16:9, etc., to make multiple images uniform.
- **Crop to shape**: Crop into circles, rounded rectangles, etc.—great for team photos or icons.
- **Remove Background**: Select the image → Picture Format → Remove Background. <u>For simple backgrounds, you don't even need Photoshop.</u>

<u>Double‑clicking an image</u> automatically takes you to the <u>Picture Format tab</u>—all cropping and adjustment tools are there.

#### 3.4.3 SmartArt and Shapes

**SmartArt** turns boring bullet lists into visual diagrams with one click:

- Process (sequential steps with arrows)
- Hierarchy (organizational charts)
- Cycle (closed‑loop processes)
- Relationship (side‑by‑side, radial, Venn, etc.)

**Pro tip**: Select an existing bullet list, then go to Home → Convert to SmartArt and pick a graphic—it's faster than starting from scratch.

**Caution**: Don't overuse SmartArt just because it's shiny. Many default styles come with <u>garish 3D effects and extra lines</u> that clutter the slide. Stick with "simple" style SmartArt, remove unnecessary shadows and bevels—clear communication wins.

---

### 3.5 Charts and Data Presentation

#### 3.5.1 Inserting a Chart

PPT charts embed a mini Excel spreadsheet. When you insert a chart, a data sheet pops up—edit the numbers there and the chart updates live.

Supported types: column, line, pie, scatter, area, etc. Choose based on what you want to convey:
- Compare sizes → column chart
- Show trends → line chart
- Show proportions → pie chart (<u>keep it under 5 slices</u>)

#### 3.5.2 Chart Design Principles

**Common pitfall**: Copy‑paste a chart directly from Excel into PPT—it brings along cluttered gridlines, multi‑color legends, and tiny decimal numbers. Projected on a big screen, the back row can't read a thing.

**What we usually do**:
- **Remove clutter**: gridlines, duplicate axis titles—strip them out. Keep only essential data labels.
- **Highlight key data**: color the most important bar differently (e.g., grey for all classes, red for your own class). Keep others muted—contrast draws attention.
- **Eliminate legends when possible**: if there's only one data series, you don't need a legend—delete it so the audience sees the trend directly.
- **Make fonts big enough**: axis numbers and labels <u>should be no smaller than 12pt</u>.

**Principle**: Think from the audience's perspective—"What do I want them to understand from this slide?" Then use color, annotations, and magnification to guide their eyes. Don't just dump raw data and call it done.

---

### 3.6 Animations and Transitions
#### 3.6.1 Animations

PowerPoint's animation system is surprisingly capable—modern hardware handles it smoothly, so "laggy PPT" is often a myth.

The simplest use is to control **progressive disclosure**—reveal one bullet point per click so the audience doesn't read ahead. Click → appear → you explain → next click.

If you <u>don't want to spend much time on animations</u> but still want some liveliness:
- Fade
- Wipe (left‑to‑right or bottom‑to‑top)
- Appear

**Animations to avoid**:
- Bounce, spin, spiral fly‑in—they make academic presentations look cheap.
- <u>Don't animate every single element on a slide.</u>

**How to set**: select an element → Animations tab → pick an entrance effect. Use the **Animation Pane** on the right to adjust order and trigger ("On Click" is the most common).

> **Remember**: Animation serves your talk, not to show off how fancy your PPT can be. If you're replaying an animation just for the cool effect, it probably should be removed.

If <u>you want to go deeper into PowerPoint animations</u>, we'll cover that in 3.6.3 to 3.6.5.

#### 3.6.2 Slide Transitions

Transitions are the effects when moving from one slide to the next.

If you don't want to invest much time:
- **Recommended**: Fade, Push (clean and professional).
- **Avoid**: Airplane, Ripple, Origami—overly complex. Stick with one subtle transition throughout.
- You can also set all to "None" and only use "Fade" once for section breaks—gives nice rhythm.

For <u>a deeper dive into transitions</u>, see 3.6.6.

#### 3.6.3 Animation Speed (for PowerPoint, not WPS)

This and the next two subsections assume you're using **PowerPoint** (not WPS Presentation) and <u>want to dig into the animation details</u>.

In the **Animation Pane** (Animations tab > Animation Pane), you can see all animations on the current slide. Double‑click one to open its detailed settings—you'll find "Smooth Start", "Smooth End", and "Bounce End" sliders. Adjusting these creates smoother acceleration/deceleration curves, as shown:

![](/img/3.6.3-1.gif)

As for duration, 0.5–1 second is a good range for most element entrances/exits.

#### 3.6.4 Animation Timing

Open the **Animation Pane**—you'll see a timeline of all animations ordered by click triggers. The logic is straightforward (example):

![](/img/3.6.4-1.png)

You can also double‑click an animation, switch to the "Timing" tab, and set "Repeat" to loop it.

**Multiple animations on one element**: select the element, then in the Animations tab click "Add Animation" to append more—you'll see them stacked in the Animation Pane. This allows effects like moving and rotating simultaneously.

**Special animations**: Audio and video playback/stop are also treated as animations—you can edit them in the Animation Pane to start or pause at specific times. For audio, after selecting the audio icon, go to the **Playback** tab and enable "Play across slides" so it continues when you advance.

#### 3.6.5 Animation Triggers

Select an element, then below the Animation Pane button you should see a **"Trigger"** button. In the dropdown under "On Click of", you can choose another element—meaning the animation plays when that specific element is clicked. Combined with 3.6.4, you can trigger multiple animations in sequence.

#### 3.6.6 Page Transitions (continued)

PowerPoint's "Transition" refers to the effect applied when *entering* the current slide, not when leaving it.

**Morph Transition**

Microsoft 365 (or Office 2016+) provides **Morph**. It works by detecting the same object on two slides with different positions, sizes, or rotations—then automatically interpolates the movement.

**Practical uses**:
- Zoom from a thumbnail to a full‑scale image.
- Animate chart elements smoothly between data sets.
- Move a highlight box gracefully across a process diagram.
- ...

To use: duplicate a slide, modify the object on the second slide, then apply Transition → Morph to the second slide.

---

### 3.7 Alignment and Layout Techniques

This is the **fastest and most effective way to improve your PPT's visual quality**. Many messy presentations aren't due to ugly colors or missing animations—they're caused by misaligned elements and uneven spacing.

#### 3.7.1 Alignment Basics

Select multiple objects (text boxes, images, shapes), then under Picture Format or Shape Format → Align:
- Left / Center / Right align
- Top / Middle / Bottom align

You can also enable **Gridlines and Guides** (View → check "Guides") to drag out reference lines. "Looks about aligned" is usually not aligned—use the tools.

#### 3.7.2 Distribution Basics

When you have three icons or text boxes that need equal horizontal spacing, dragging manually never gets it right. Select them, then use "Distribute Horizontally" or "Distribute Vertically" under Align—spacing becomes exactly equal.

**Note**: Align first, then distribute—doing it in reverse can mess things up.

#### 3.7.3 Grouping

Group multiple elements (e.g., an icon and its label) so you can move and position them as one unit. Select them and press `Ctrl + G`. To edit an individual element later, click inside the group—or press `Ctrl + Shift + G` to ungroup.

#### 3.7.4 Spacing (optional deep dive)

If you want fine‑tuned layout details, read on—otherwise skip to 3.8.

Spacing greatly affects visual comfort. Generally, for a top‑left title, make the <u>left margin slightly smaller than the top margin</u>—it stabilises the page:

![](/img/3.7.4-1.png)

For content centered on the slide, positioning it slightly above the geometric center makes it feel more balanced:

![](/img/3.7.4-2.png)

If you use multiple stacked rounded rectangles, try to match their corner radii:

![](/img/3.7.4-3.png)

---

### 3.8 Presentation and Delivery Tips

#### 3.8.1 Presenter View

Using **Presenter View** is a must‑have skill for college defenses and talks. When connected to a projector, your laptop shows a special interface while the audience sees the full‑screen slide.

You can see:
- The current slide
- A preview of the next slide (helps with transitions)
- Your **notes** (visible only to you—great for talk prompts)
- A timer

Enable it: Slide Show → check "Use Presenter View". If you have a second display, it usually activates automatically.

> **Newbie tip**: Always test Presenter View on the classroom computer before your defense. Older projectors may mirror your screen directly, exposing your notes to the audience—you'll need to set Windows display mode to "Extend" to avoid that.

#### 3.8.2 Rehearse Timings

If you need to speak without notes or have a strict time limit (e.g., a 5‑minute competition talk), use Slide Show → Rehearse Timings. Run through your presentation naturally—PPT records your time per slide and total duration. You can later save these timings for auto‑advance.

---

### 3.9 Exporting and Compatibility

#### Save Formats

- **`.pptx`**: default—use this for everyday work.
- **`.ppt`**: legacy format (Office 97‑2003)—**not recommended**.
- **`.pdf`**: if you only need to share the presentation for viewing (not editing), **export to PDF**—it guarantees layout fidelity across any device.

#### Font Issues

> **The classic disaster**: You carefully tuned fonts and layout on your own machine. On the classroom computer, everything turns to宋体 (SimSun) and the formatting falls apart.

**Why**: The classroom computer doesn't have your custom font installed.

**Solutions**:
1. File → Options → Save.
2. Check "Embed fonts in the file".
3. Choose "Embed only the characters used in the presentation" (smaller file) or "Embed all characters" (if the recipient needs to edit).

**Caveat**: Some fonts are licensed and cannot be embedded—you'll get an error. In that case, select the text box, right‑click "Save as Picture" or "Convert to Shape"—the text becomes an image that preserves the look without relying on the font.

#### Video and Audio

If you insert a video, make sure to copy the video file along with your PPT, or test whether embedding works. Recommendations:
- Use MP4 format for best compatibility.
- Do a full playback test on the target computer beforehand—check both picture and sound.

---

### 3.10 Closing Thought: Shift Your Mindset

Move from "this slide looks empty, let me add some decoration" to "what format will help my audience understand my point fastest?"

> **PPT is not about showing how much you know—it's about helping others understand your perspective.**

In fact, if you can strip away the redundant and highlight the essential, your slides already beat the majority.

---

## Extra: VBA Macros

**VBA (Visual Basic for Applications)** is Office's built‑in programming language. You can write scripts to automate repetitive tasks.

**Open the VBA editor**: `Alt + F11`

### What VBA Can Do

**Batch process Word documents**: Change the margins of every Word file in a folder to 2.5cm all at once. Manually, you'd open each document and click through dozens of dialogs—with a VBA script, one click does hundreds.

**Excel automation**: Merge data from 12 monthly spreadsheets into one master sheet. Write a loop that opens each file, copies data, and pastes into the summary.

**PowerPoint batch operations**: Add a company logo to the top‑right corner of all 200 slides.

### Recording Macros

The easiest way: click the "Developer" tab → "Record Macro" → perform your steps (e.g., set margins, change fonts) → "Stop Recording". Excel automatically converts your actions into VBA code. You can replay it anytime.

**Note**: Recorded macros generate verbose code (it records every click, including unnecessary ones). Still, it's a great learning tool—record something, inspect the generated code, and then tweak it.

### Security

`.docm`, `.xlsm`, `.pptm` are macro‑enabled files. Office will prompt "This file contains macros" when you open them. Only enable macros from trusted sources (e.g., school‑issued, colleagues). Macros can delete files or format drives—they're essentially small executable programs.

**Viewing and editing macros**: `Alt + F11` opens the VBA editor—you can see all modules and code. Even if you're not a programmer, you can often recognize basic logic (like "open", "copy", "paste" in plain English).