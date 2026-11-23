# Appendix 23: Git

> This appendix introduces the basics of Git. Git is a version control tool that records every change you make to your files, allowing you to return to any previous version at any time. Whether you are writing a thesis, code, or any kind of document, Git saves you from the endless cycle of "final.doc", "final_v2.doc", "final_v2_really.doc", and "final_v2_really_this_time_for_real.doc".

## 23.1 Why Use Git?

When writing a thesis, you have probably experienced something like this:

```
thesis_draft.docx
thesis_revision1.docx
thesis_revision2.docx
thesis_supervisor_feedback.docx
thesis_final.docx
thesis_final_really_final.docx
```

Then your supervisor says: "I think the structure of the first draft was better – can we go back to that?" You are left searching through a dozen files, afraid to delete the wrong one.

Git solves this. It records every change, and you can revert to any historical version at any time. You only ever need one file – no more creating multiple copies.

**What Git gives you**:
- Every change is logged; you can always see what was modified and when.
- Reverting to any previous version is just one command away.
- You can create "branches" to experiment safely, without affecting your main version.
- When collaborating with others, no one accidentally overwrites someone else's work.

## 23.2 Installing Git

**Windows**: Visit [`git-scm.com/download/win`](https://git-scm.com/install/windows), download the installer, and follow the default options. After installation, search for "Git Bash" in the Start menu – if a command‑line window opens, you are good to go.

**macOS**: Open Terminal and type `git --version`. If Git is not installed, macOS will prompt you to install the Command Line Tools – follow the instructions.

**Linux (Ubuntu/Debian)**: `sudo apt install git`

After installation, open a terminal (Git Bash on Windows, Terminal on macOS/Linux) and set your name and email (these are just for tracking who made changes; you can use anything):

```bash
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

## 23.3 Initialising a Repository (`git init`)

Git turns a folder into a **repository**, storing all version history inside that folder.

**How to do it**:

1. In File Explorer, create a dedicated folder for your thesis, e.g., `D:\thesis`.
2. Right‑click the folder → select "Git Bash Here" (if you have Git installed).
3. In the terminal, type:

```bash
git init
```

You will see a message like `Initialized empty Git repository in D:/thesis/.git/`. That `.git` folder is where Git keeps all its history. **Do not touch it**.

From now on, any changes you make inside the `thesis` folder can be recorded by Git.

## 23.4 Your First Commit

A "commit" is a snapshot of all your current files, saved into Git's history.

**Scenario**: You have just finished the first chapter of your thesis and want to save a checkpoint.

**Steps**:

1. Add the file to the "staging area" – this tells Git "I want to save these files":

```bash
git add chapter1.docx
```

To add all files in the current folder at once:

```bash
git add .
```

2. Commit with a message (use `-m` to add a short description of what you changed):

```bash
git commit -m "Finished first draft of Chapter 1"
```

Now your first chapter has a version record. Whenever you reach a milestone, repeat `git add .` + `git commit -m "description"`.

**Viewing history**:

```bash
git log
```

This lists all commits, each with a hash (like `a1b2c3d`), author, date, and commit message.

## 23.5 Checking the Status

At any time, type:

```bash
git status
```

This tells you:
- Red files: modified but not yet `add`ed.
- Green files: added but not yet `commit`ted.
- Clean working directory: all changes are committed.

Get into the habit of running `git status` often – you will always know exactly where you stand.

## 23.6 Branches: Experiment Without Fear

Branches are Git's most powerful feature. Think of them as parallel universes.

**Scenario**: Your supervisor says, "Try restructuring Chapter 3, but I am not sure if it will work – just give it a try." You do not want to break your existing version, but you want to experiment.

**How to do it**:

1. Create a new branch from the current version:

```bash
git branch experimental-changes
```

This creates a branch called "experimental-changes" that is identical to your current main branch (often called `master` or `main`).

2. Switch to the new branch:

```bash
git checkout experimental-changes
```

Or do both in one command:

```bash
git checkout -b experimental-changes
```

3. Make any changes you like and commit them freely. Even if you mess up, your main branch remains untouched.

**To switch back to the main branch**:

```bash
git checkout master   # or git checkout main
```

Your files will revert to the main branch state; all experimental changes are safely stored in the "experimental-changes" branch.

**List all branches**:

```bash
git branch
```

The branch with a `*` next to it is the one you are currently on.

## 23.7 Merging: Bringing Experimental Work Back

If your experimental changes turned out well and your supervisor approves, you can merge them back into the main branch.

**How to do it**:

1. Switch back to the main branch:

```bash
git checkout master
```

2. Perform the merge:

```bash
git merge experimental-changes
```

Git will incorporate all the changes from the "experimental-changes" branch into the main branch. If both branches changed the same line of the same file, Git will flag a **conflict**. You will need to resolve it manually – Git marks the conflicting sections in the file with `<<<<<<<`, `=======`, and `>>>>>>>`. Choose which version to keep, delete the markers, then save, add, and commit.

**If the experiment fails**: Just delete the branch – the main branch is unaffected:

```bash
git branch -d experimental-changes
```

## 23.8 Real‑World Scenario: Your Supervisor Edits Your Thesis

Your supervisor says, "I made some edits to Chapter 3" and sends you a revised file. You overwrite your old file with the new one. How can Git show you exactly what your supervisor changed?

**How to do it**:

1. Before overwriting, ensure all your changes are committed (`git status` should be clean).
2. Replace the old file with the new one your supervisor sent.
3. Use `git diff` to see the differences:

```bash
git diff
```

This shows every line that changed – green for additions, red for deletions.

4. After reviewing, commit the changes:

```bash
git add .
git commit -m "Incorporated supervisor's edits to Chapter 3"
```

**If your supervisor sends multiple revisions**: Repeat the steps each time. Git preserves the history of every version, so you can always go back.

## 23.9 Going Back in Time: What If You Regret a Change?

**Scenario**: You have been working for three days and realise the direction is wrong. You want to go back to the version from three days ago.

1. View the history and find the hash of the version you want (the first few characters are enough):

```bash
git log
```

2. Go back to that version:

```bash
git checkout a1b2c3d   # replace with the hash you found
```

Your files will revert to that state. If you want to continue working from there, create a new branch.

**For a more drastic "go back"** (discard all subsequent changes):

```bash
git reset --hard a1b2c3d
```

**⚠️ Warning**: `git reset --hard` permanently deletes all later changes. Think twice before using it.

## 23.10 Quick Command Reference

| Command | What it does |
|---------|---------------|
| `git init` | Initialise a new repository |
| `git add filename` | Add a file to the staging area |
| `git add .` | Add all changes to the staging area |
| `git commit -m "message"` | Commit the staged changes |
| `git status` | Show the current status |
| `git log` | Show the commit history |
| `git log --oneline` | Show a concise history (one line per commit) |
| `git diff` | Show unstaged changes |
| `git diff --staged` | Show staged but uncommitted changes |
| `git branch` | List all branches |
| `git branch branch-name` | Create a new branch |
| `git checkout branch-name` | Switch to a branch |
| `git checkout -b branch-name` | Create and switch to a new branch |
| `git merge branch-name` | Merge the specified branch into the current branch |
| `git branch -d branch-name` | Delete a branch |
| `git checkout hash` | Switch to a specific historical version |
| `git reset --hard hash` | Permanently revert to a specific historical version |

---

# Extended Reading

> The following sections cover more advanced Git usage. Come back when you need them.

## Extended 1: The `.gitignore` File

Some files should not be tracked by Git – for example, temporary build files, system caches, or large binaries. Create a `.gitignore` file in the root of your repository and list the files or patterns you want to ignore:

```
*.tmp
*.log
.DS_Store
node_modules/
```

Git will automatically ignore these files and they will not appear in `git status`.

## Extended 2: Amending the Last Commit

If you just committed but realise you made a typo in the message or forgot to include a file:

```bash
git commit --amend -m "New commit message"
```

This modifies the most recent commit without creating a new one.

## Extended 3: Stashing Unfinished Work (`git stash`)

You are in the middle of some changes, but you need to urgently switch to another branch to fix a bug – and your current work is not ready for committing yet:

```bash
git stash          # Save your current changes, leaving a clean working directory
git checkout other-branch
# ... fix the bug ...
git checkout original-branch
git stash pop      # Restore your previous changes
```