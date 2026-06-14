# Chapter Zero: Introduction

## 0.1 The Appendix and the Main Book Are Different Kinds of Things

This book is split into two major parts: the **Appendix** and the **Main Book**. Their goals, writing styles, and examples are not the same.

### 0.1.1 What the Appendix Looks Like

The Appendix is everything you just finished reading. Here's what defines it:

- **It teaches you how to do things**: how to partition a drive, disable BitLocker, change NTP servers, pick browser extensions. This is hands‑on guidance.
- **It focuses on daily usage**: it addresses problems you might actually run into while using your computer – things like "it's frozen" or "I don't know where to change that setting".
- **It uses Windows 11 for examples**: all screenshots, commands, and path formats assume you're on Windows 11.
- **Anyone can read it**: you don't need to know how to program, or even be sure you want to study CS. As long as you use a computer, some part of the Appendix will be useful to you.

The Appendix is meant to be a **reference manual** you keep within arm's reach.

### 0.1.2 What the Main Book Looks Like

The Main Book is what you're about to read. Here's what defines it:

- **It explains why things work**: why can a compiler turn source code into an executable? Why does memory have the stack and the heap? The Appendix doesn't go deep on questions like these; the Main Book does.
- **It walks you through writing code**: the Main Book contains a lot of code. You'll need to type it yourself, compile it, modify it, and run it. Just reading along won't help.
- **It uses WSL2 Ubuntu for examples**: all commands, compilation instructions, and path formats assume you're working inside Ubuntu 24.04 on WSL2.
- **It's aimed primarily at CS/EE students**: if you've already decided on (or are seriously considering) a CS or EE major, the Main Book is written for you. If you choose a different path, the Main Book can still deepen your understanding of how computers work – but you should be aware that it won't always hold your hand.

The Main Book's goal is to **help you move from "knowing how" to "understanding why"**. Once you understand the principles, many of the "how‑to" questions answer themselves.

---

## 0.2 How I Write the Main Book

The Appendix and the Main Book are written differently. I want to make those differences clear upfront so you can read more smoothly.

### 0.2.1 Very Few Analogies

The Appendix uses some analogies – for example, comparing memory vs. disk to a cutting board vs. a refrigerator, or explaining swap space using a kitchen counter. Those analogies work well in the Appendix because its goal is to give you a quick mental picture.

The Main Book avoids analogies as much as possible. The reason is simple:

- An analogy tries to explain something unfamiliar using something you already know. But many concepts in computing don't have real‑world equivalents. A pointer is a pointer – it holds a memory address. A file descriptor is just an integer that points to an entry inside the kernel's table. Forcing an analogy often leads to more confusion than clarity.
- Eventually, you'll need to get comfortable with these concepts on their own terms. Analogies are training wheels – if you keep using them, you won't move very fast.

So the Main Book gets more direct. That doesn't mean deliberately difficult – it just means fewer detours.

### 0.2.2 There Will Be a Lot of Terminology

Every section introduces new terms. There's no way around it – learning any craft requires learning its jargon.

The **Quick Reference Table** at the beginning of each section is designed to help you handle this:

- Glance at it before you start reading, so you know what the section will cover.
- When you forget what a term means while reading, flip back to the table.
- After finishing the section, use it as a review checklist to see whether you've actually remembered the terms.
- If you run into a term you don't understand, copy it straight into a search engine or an AI assistant – those terms are already the best keywords to search for.

Don't expect to remember every term on the first pass. Learning programming and low‑level concepts is a spiral process – you'll encounter these words again and again, and each time you'll remember them a little better.

### 0.2.3 You Have to Run Things Yourself

Every claim in the Main Book can be verified with code or commands.

If you only read and never type, you'll end up with a vague impression. That vague impression won't help you one bit when you run into compilation errors, segmentation faults, or link failures. What *will* help is the muscle memory you build by typing, running, modifying, and debugging examples yourself.

So all code examples in the Main Book assume you'll execute them inside your own WSL2 environment. Some examples are deliberately broken – you'll need to read the error messages, understand why they fail, and then fix them.

---

## 0.3 How to Read This Book

### 0.3.1 If You're a New CS/EE Student

Read in order. You must complete the validation checklist at the end of each section. If a validation step fails, don't skip it – stop and figure out why. The process of troubleshooting is itself a learning experience.

If you encounter a concept that was mentioned in the Appendix (like environment variables, PATH, or swap space), feel free to flip back to the Appendix for a refresher. However, the Appendix's explanations tend toward "rough idea", while the Main Book is more precise – when in doubt, trust the Main Book.

### 0.3.2 If You're Not Planning to Study CS

You can skip some of the very low‑level chapters (like the ones on process scheduling or memory paging later on). But I strongly recommend reading the chapters on compilers, build systems, Git, and debuggers. Those topics aren't tied to any particular major – anyone who does creative work on a computer will benefit from them.

### 0.3.3 About AI

Of course, you're welcome to use AI as a learning aid – but AI cannot replace your own brain. Claude Code can produce beautiful code in ten minutes. You get an application out of it, but that's all. Genuine learning requires your own gradual trial and error. Even if you use agents like Claude Code, I hope you'll read the code they generate and actually understand it.

You can ask an AI what a certain term means, how to interpret a piece of syntax, or what might be causing a particular error message. AIs make mistakes. The final judge is what you see when you actually run your code.

---

## 0.4 Moving On to Chapter One

Ready? Let's begin.