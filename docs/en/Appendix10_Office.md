# Appendix 10: Office

> This appendix introduces the core features of Microsoft Office’s three main components (Word, Excel, PowerPoint). You will rely on them throughout university for writing papers, creating presentations, and processing data.  
> I will focus on Microsoft Office, using Office 365 as an example. WPS is similar but has minor differences. I recommend using Office 365. Of course, if your instructor specifically requires WPS for your thesis – which is rare – you should follow that rule, because formatting differences between MS Office and WPS can be somewhat unpredictable. Margins, line spacing, and font rendering may shift when opening an MS Office document in WPS or vice versa. Before submitting your final draft, open it with the software your instructor uses to double‑check the formatting.  
> In the next appendix I will introduce `LaTeX`, which is even more powerful for thesis typesetting. Most universities provide a LaTeX template for graduation theses.

---

## Part 1: Word

Word is a document editing tool. You will use it to write papers, create resumes, and complete assignments. The following sections cover the features you will actually use.

### 1.1 Styles

A **Style** is a pre‑defined set of formatting options – font, size, bold, colour, paragraph spacing, and more.

Many people write documents by: selecting text → manually adjusting font size → manually making it bold → manually tweaking line spacing. For a thesis dozens of pages long with several heading levels, doing this manually is slow and a nightmare to update.

The right way: define styles first, then apply them.

Word comes with built‑in styles like “Heading 1”, “Heading 2”, and “Normal”. Right‑click a style → Modify to set its font, size, line spacing, and spacing before/after paragraphs. After you modify a style, every paragraph using that style updates automatically.

**Benefits**:
- Consistent formatting – no accidental mixing of fonts and sizes
- Fast to change: just modify the style definition, and the whole document updates
- Works with the References tools to auto‑generate a table of contents

### 1.2 Page Breaks and Line Breaks

When writing a thesis, you often need to start a new page (for example, each chapter begins on a fresh page). Many people mash the Enter key until they reach the next page. The problem: if you add or remove content earlier, everything after goes out of place.

The right way:

- **`Ctrl + Enter`**: Insert a page break. Everything after the cursor moves to the next page. No matter how you edit the content before it, the page break stays put.
- **`Shift + Enter`**: Insert a line break without starting a new paragraph. Pressing Enter alone creates a new paragraph, which adds spacing between lines. `Shift + Enter` just moves to the next line without extra spacing. Useful for addresses, poems, or any place where you want a new line but not a paragraph gap.

### 1.3 Layout

The **Layout** tab controls the overall appearance of the page:

- **Margins**: The blank space around your document. Theses usually have specific margin requirements (e.g., top 2.5 cm, bottom 2.5 cm, left 3 cm, right 2 cm). Use “Custom Margins” to set exact values.
- **Orientation**: Portrait (vertical) and Landscape (horizontal). If a table is too wide to fit, you can set only that page to Landscape (choose “Apply to this section” in Page Setup).
- **Columns**: Split a page into multiple columns. Useful for newsletters or pamphlets.

### 1.4 References

The **References** tab is the core toolset for academic writing.

**Table of Contents**: As long as you have used styles (Heading 1, Heading 2, etc.), clicking “Table of Contents” generates one automatically. Page numbers update as you edit. If you add or remove headings, click “Update Table”.

**Footnotes and Endnotes**:
- Footnote: Appears at the bottom of the same page. The reference mark goes in the text, the note appears at the page bottom. Good for supplementary comments.
- Endnote: Appears at the end of the document. Often used for references (some citation styles use endnotes).

**Citations & Bibliography**:
- Click “Insert Citation” → “Add New Source”, fill in author, title, year, etc. Word automatically formats the citation for you.
- At the end of your document, click “Bibliography” to generate a reference list automatically.
- Different journals and universities require different citation styles (APA, MLA, Chicago, etc.). Choose from the “Style” dropdown.

**Captions**: After inserting an image, table, or equation, select it and click “Insert Caption”. This numbers your figures (Figure 1, Figure 2, Table 1, Table 2). Combined with “Cross‑reference”, you can write “as shown in Figure 1” in your text and make it clickable. If you insert a new figure in the middle, all later numbers update automatically.

### 1.5 Review

Features in the **Review** tab are frequently used – especially when your instructor marks up your paper.

**Comments**: Select some text, click “New Comment”, and a bubble appears on the right side. Instructors write feedback like “this logic is unclear” or “add a reference”. Students read the comments, make changes, and can “Reply” or “Resolve” the comment.

**Track Changes**: When you turn on Track Changes, every edit (deletion, addition, move) is shown as a coloured mark. Your instructor can see exactly what you changed, and you can see what they changed. The marks disappear only after you Accept or Reject the revision.

**Compare**: Submitted two versions of your paper and cannot remember what you changed? Click “Compare”, choose the original and the modified document, and Word generates a comparison highlighting every difference.

### 1.6 Important Settings in Word Options

Click “File” → “Options”. Here are a few settings worth knowing:

**General → User Interface options**: Change the colour scheme to “Dark Gray” or “Black”. Easier on the eyes during long writing sessions.

**Display**:
- “Always show these formatting marks on the screen”: Recommend turning on “Tab characters”, “Spaces”, and “Paragraph marks”. You will see grey dots (spaces) and弯 arrow symbols (paragraphs). When alignment looks wrong, these marks help you diagnose the problem.
- **Never change document page colour**: Some people set Word’s background to eye‑friendly green or dark mode. But your readers (instructors, journal reviewers) see the default white background and black text. If you change font colours while using a coloured background (e.g., light grey), those colours may become invisible on white. Stay with the default, or at least check how it looks on white.

**Save**:
- “Save AutoRecover information”: Default is 10 minutes. For important documents, set it shorter – say, 5 minutes.
- “Default file format”: Choose `Word Document (*.docx)`. The older `.doc` format has poorer compatibility.
- “Embed fonts”: If you use a non‑standard font (not pre‑installed on Windows), someone else’s computer will substitute a different font, likely breaking your layout. Check “Embed fonts in the file” to package the font inside the document. Note: some fonts have licensing restrictions that prevent embedding.

**Advanced**:
- **Image fidelity**: By default, Word compresses inserted images to reduce file size. If you need to print or submit a high‑quality document (e.g., a thesis with high‑resolution figures), go to “Image Size and Quality” and change “Default resolution” to “High fidelity”.
- **Cut, copy, and paste**: When you paste text from a webpage, it often brings along formatting (background colours, weird fonts). Under “Pasting from other programs”, choose “Keep Text Only”. Only the plain text will be pasted.
- Other display options exist under “Show document content” (e.g., “Show background colors and images in Print Layout view”).

**Add‑ins**: Used to extend Word’s functionality. For example, Zotero (a reference manager) installs an add‑in here so you can insert citations directly from within Word. If an add‑in slows down Word or causes errors, you can disable it here.

### 1.7 DOTM Templates

**.dotm** is Word’s macro‑enabled template file format. Normal documents are `.docx`, templates are `.dotx`, and macro‑enabled templates are `.dotm`.

A **Template** is like a mould. You create a resume template – set margins, styles, headers/footers – and save it as `.dotx`. Every time you double‑click that template file, it generates a new document inheriting all those settings, while the original template remains unchanged.

Some universities provide a graduation thesis template as a `.dotm` file. It contains pre‑set formatting and may include automation scripts (e.g., one‑click checking of figure/table numbering). If the file is `.dotm`, Word will prompt you to “Enable Macros” – you need to allow this to use all features.

**Where to put templates**: The default location is `C:\Users\YourUserName\Documents\Custom Office Templates`. After placing templates there, they appear under “File” → “New” → “Personal”.

---

### Word Extended Reading

#### Extended 1: Linking Styles to Table of Contents

When generating a table of contents, Word by default only looks for “Heading 1”, “Heading 2”, and “Heading 3”. If you use custom styles (e.g., “Chapter Title”), go to “References” → “Table of Contents” → “Custom Table of Contents” → “Options” to manually assign heading levels.

#### Extended 2: Section Breaks

A **Section Break** is more powerful than a simple page break. After inserting a section break, each section can have its own margins, headers/footers, and page orientation. For example, the preface and table of contents might use Roman numeral page numbers, while the main body uses Arabic numerals. A section break in between makes this possible.

How to insert: Layout → Breaks → Next Page (this is a section break).

#### Extended 3: Document Page Colour

The “Never change document page colour” setting in Word Options exists precisely because of dark mode. Windows 11’s dark mode affects Office – Word may show black background with white text. However, the actual document background is still white. If someone changes font colours to something light (e.g., light grey) while using dark mode, those colours may become illegible when the document is opened on a white background (as your instructor likely will). Keeping the defaults is safest.

---

## Part 2: Excel

Excel is a spreadsheet tool for working with data, performing calculations, and creating charts. Many people only type into cells and feel intimidated by formulas. This section covers only what you will actually use during university – no overly complex functions.

### 2.1 Basic Operations

A **Cell** is the smallest unit in a spreadsheet, identified by its column letter and row number – for example, `B3` means column 2, row 3.

A **Range** is a contiguous block of cells, written as the top‑left cell and bottom‑right cell separated by a colon. For example, `A1:C10` covers all cells from A1 through C10.

**Common shortcuts**:

| Action | Method |
|--------|--------|
| Select an entire row | Click the row number |
| Select an entire column | Click the column letter |
| Select the whole sheet | Click the triangle at the top‑left (between A and 1) |
| Fill the same content downward | Select the cell, drag the small square at the bottom‑right (fill handle) |
| Fill a sequence | Hold `Ctrl` while dragging the fill handle (or enter two values so Excel detects the pattern) |
| Auto‑fit column width / row height | Double‑click the divider between column letters (or row numbers) |

### 2.2 Formulas and Functions

Formulas begin with `=`. For example, typing `=1+2` into a cell will display `3` after you press Enter.

**Common functions**

| Function | Purpose | Example |
|----------|---------|---------|
| `SUM` | Adds numbers | `=SUM(A1:A10)` |
| `AVERAGE` | Average | `=AVERAGE(B2:B20)` |
| `COUNT` | Counts numeric cells | `=COUNT(C1:C100)` |
| `COUNTA` | Counts non‑empty cells | `=COUNTA(D:D)` |
| `MAX` / `MIN` | Largest / smallest value | `=MAX(E2:E50)` |
| `IF` | Conditional logic | `=IF(F2>=60, "Pass", "Fail")` |

**Important concept: Relative vs Absolute Reference**

In a formula, `A1` is a relative reference. When you drag the formula down, it automatically becomes `A2`, `A3`, etc. `$A$1` is an absolute reference – it stays the same when dragged. There are also mixed references: `$A1` (column fixed, row flexible) and `A$1` (row fixed, column flexible).

Press `F4` to quickly cycle through these four reference modes.

**Example**: Calculate each sale’s percentage of total sales. Assume sales figures are in `B2:B10` and the total is in `B11`. The formula for the first row is `=B2/$B$11`. The `$B$11` is locked, so when you drag the formula down, the total cell never changes.

### 2.3 Lookup & Reference Functions

This is Excel’s core power: finding data from one table based on a value in another.

**VLOOKUP (Vertical Lookup)**

Syntax: `=VLOOKUP(what_to_find, where_to_look, column_number, 0)`

Example: You have a student grade table – column A is student ID, column B is name, column C is grade. You want to find the grade for a given ID:
`=VLOOKUP(F2, A:C, 3, 0)`
- F2: the student ID to look for
- A:C: look within columns A through C (the first column of this range must contain the ID)
- 3: once found, return the value from the 3rd column of the range (column C, the grade)
- 0: exact match

**Important notes**:
- The lookup value must be in the first column of the lookup range
- `VLOOKUP` only searches left‑to‑right; it cannot return values from columns to the left of the lookup column
- Always use `0` (or `FALSE`) for exact match – do not omit it

**XLOOKUP (Office 365 only)**

`XLOOKUP` is an improved version that fixes many of `VLOOKUP`’s limitations. The syntax is simpler, it can look left, and you can specify a custom “not found” message.

Syntax: `=XLOOKUP(what_to_find, where_to_look, column_to_return, "not found")`

Example: `=XLOOKUP(F2, A:A, C:C, "ID not found")`

**Version differences**:
- Office 2021 and Office 365 support `XLOOKUP`, `FILTER`, `SORT`, and other new functions
- Office 2019 and older do not. If you send someone an older‑version Excel file containing `XLOOKUP`, they will see a `#NAME?` error

**INDEX + MATCH (universal, works in all versions)**

`INDEX` returns the value at a given row and column within a range. `MATCH` finds the position of a value within a list. Together they can perform two‑way lookups.

Example: Find a grade based on student name and subject name:
`=INDEX(grade_range, MATCH(name, name_column, 0), MATCH(subject, subject_row, 0))`

### 2.4 Text and Date Functions

**Text functions**

| Function | Purpose | Example |
|----------|---------|---------|
| `LEFT(text, n)` | Takes n characters from the left | `=LEFT("2024001", 4)` → "2024" |
| `RIGHT(text, n)` | Takes n characters from the right | `=RIGHT(A2, 6)` |
| `MID(text, start, count)` | Takes middle characters | `=MID("2024-09-01", 6, 2)` → "09" |
| `&` operator | Concatenates text | `=B2 & " " & C2` |
| `TEXT(value, format)` | Formats numbers or dates | `=TEXT(A2, "yyyy-mm-dd")` |

**Date functions**

| Function | Purpose | Example |
|----------|---------|---------|
| `TODAY()` | Current date | Updates automatically each day |
| `NOW()` | Current date and time | |
| `YEAR(date)` | Extracts the year | `=YEAR(A2)` |
| `MONTH(date)` | Extracts the month | |
| `DAY(date)` | Extracts the day | |
| `DATEDIF(start, end, unit)` | Difference between two dates | `=DATEDIF(A2, TODAY(), "y")` calculates age |

### 2.5 Conditional Statistics

| Function | Purpose | Example |
|----------|---------|---------|
| `COUNTIF(range, condition)` | Counts cells meeting a condition | `=COUNTIF(B:B, ">=60")` |
| `SUMIF(condition_range, condition, sum_range)` | Sums cells meeting a condition | `=SUMIF(A:A, "Sales", B:B)` |
| `AVERAGEIF(condition_range, condition, average_range)` | Averages cells meeting a condition | `=AVERAGEIF(C:C, ">0", C:C)` |

### 2.6 Pivot Tables

A **Pivot Table** is the most powerful data analysis tool in Excel. With a few clicks, you can generate summary reports from thousands of rows of data.

**Scenario**: You have a school‑wide grade table with columns “Class”, “Student Name”, “Subject”, “Grade” – 5,000 rows total. You want to know the average grade per class, the highest grade per subject, or the distribution of grades by class and subject. A pivot table does this in seconds.

**Steps**:
1. Select the entire data range
2. Click “Insert” → “PivotTable”
3. In the pop‑up, choose “New Worksheet”
4. In the “PivotTable Fields” panel, drag fields into different areas:
   - **Rows**: what you want to group by (e.g., “Class”)
   - **Columns**: what you want to cross‑tabulate with (e.g., “Subject”)
   - **Values**: what you want to calculate (e.g., “Grade”). By default it sums; click “Value Field Settings” to change to “Average” or “Count”

### 2.7 Creating Charts

Select your data → “Insert” → “Chart”. Common types:
- **Column chart**: Compare values across categories
- **Line chart**: Show trends over time
- **Pie chart**: Show proportions of a whole (use with at most 5 slices, otherwise it gets messy)

Consider adding titles, data labels, and axis descriptions. Right‑click on the chart to change chart type, colour palette, add trendlines, etc.

### 2.8 Conditional Formatting

**Conditional Formatting** makes cells automatically change colour based on their values.

Select your data → “Home” → “Conditional Formatting”. Useful rules:
- Highlight cells rules: highlight values above a threshold in red, duplicate values in yellow
- Colour scales: shade cells from green (high) to red (low)
- Data bars: draw a bar inside each cell whose length represents the value – no extra space needed
- Icon sets: use traffic lights or arrows to indicate high/medium/low

### 2.9 Data Validation

**Data Validation** restricts what can be typed into a cell, preventing errors.

Select a cell → “Data” → “Data Validation” → “Settings” → “Allow” choose “List”. In “Source”, type your options separated by commas (e.g., `Yes,No`). That cell now becomes a dropdown menu.

**Common restrictions**:
- List → dropdown menu
- Whole number → limit to a range (e.g., 0 to 100)
- Date → restrict to a date range
- Text length → limit number of characters (e.g., phone number must be 11 digits)

### 2.10 Shortcuts

| Shortcut | Effect |
|----------|--------|
| `Ctrl + arrow key` | Jump to the edge of the data region |
| `Ctrl + Shift + arrow key` | Select the entire contiguous region |
| `Alt + =` | AutoSum (inserts a SUM formula) |
| `F4` | Repeat the last action (e.g., repeat inserting a row, repeat formatting) |
| `F2` | Edit the current cell (instead of double‑clicking) |
| `Ctrl + D` | Fill down (copy from the cell above) |
| `Ctrl + R` | Fill right |
| `Ctrl + 1` | Open the Format Cells dialog |
| `Ctrl + Shift + L` | Turn filters on/off |

---

### Excel Extended Reading

#### Extended 1: Function Differences – Office 2021 vs. Office 365

| Function | Office 2021 | Office 365 | Notes |
|----------|-------------|------------|-------|
| `XLOOKUP` | ✅ | ✅ | Also available in 2021 |
| `FILTER` | ❌ | ✅ | 365 only – auto‑filters and returns results |
| `SORT` | ❌ | ✅ | 365 only – sorts automatically |
| `UNIQUE` | ❌ | ✅ | 365 only – extracts unique values |
| `TEXTJOIN` | ✅ | ✅ | Joins text with a delimiter |
| `LET` | ❌ | ✅ | 365 only – assign names to variables inside formulas |
| `LAMBDA` | ❌ | ✅ | 365 only – create custom functions |

If you use an Office 365 account provided by your school, you have all of these. If you bought a perpetual‑license version like Office 2021, support for new functions is limited.

#### Extended 2: `FILTER` Function Example (Office 365)

`=FILTER(A:C, B:B="Sales", "No results")` – returns all rows from columns A through C where column B equals “Sales”. No manual filtering or copying needed.

#### Extended 3: Excel Tables

Select your data and press `Ctrl + T` to convert a normal range into an **Excel Table**. Benefits:
- When you add new data, formulas and formatting automatically extend
- Column headers automatically have filter buttons
- Formulas use structured references like `=[@Sales]`, which are much more readable than `=D2`

---

## Part 3: PowerPoint

> I would like someone more experienced than me to write this part. For now, it is left intentionally blank.

---

## Extended: VBA Macros

**VBA (Visual Basic for Applications)** is the programming language built into Office. You can write scripts to automate repetitive tasks.

**Open the VBA editor**: `Alt + F11`

### What VBA Can Do

**Batch processing Word documents**: Change the margins of every Word document in a folder to 2.5 cm. Doing this manually means opening each document and clicking many times. With a short VBA script, you can process hundreds of documents with one click.

**Excel automation**: Combine 12 monthly data sheets into one master sheet. Write a loop that automatically opens each file, copies the data, and pastes it into a summary sheet.

**PowerPoint batch operations**: Add a company logo to the top‑right corner of every slide in a 200‑slide deck.

### Recording Macros

The easiest method: Go to the “Developer” tab → “Record Macro” → perform your actions (e.g., set margins, change fonts) → “Stop Recording”. Excel automatically translates your actions into VBA code. You can then replay that macro with a single click.

**Note**: Recorded macros generate code that is verbose (it records every single step, including some you might not need). But it is a great way to learn – record an action, look at the generated code, then start tweaking it.

### Security

`.docm`, `.xlsm`, and `.pptm` are macro‑enabled files. When you open such a file, Office warns you that “the file contains macros”. Only open macro‑enabled files from trusted sources (e.g., your school, a colleague). Never open them from unknown origins – macros can delete files, format your hard drive, or do other harmful things. Macros are essentially small executable programs.

**Viewing and editing macros**: Press `Alt + F11` to open the VBA editor, where you can see all modules and code. You do not need to be a programmer to gain something – being able to recognise basic logic words like “Open”, “Copy”, “Paste” is already useful.