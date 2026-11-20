# Section 0: Download and Introduction

> Actively being updated – here's a placeholder for now.  
> I'll upload a copy to GitHub, but you can also download it from here – just click the download button in the top navigation bar of the website.  
> Quack quack – just reserving a spot, though it's hardly necessary.  
> Regardless of where you download it, note that the software is licensed under [Apache 2.0](https://www.apache.org/licenses/LICENSE-2.0.html), the hardware and enclosure models under [CERN OHL v2 Permissive](https://ohwr.org/cern_ohl_p_v2.txt), and the documentation for this project under [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/legalcode).

## 0.1 Project Overview

This project will tackle hardware, software, and modeling together, with the goal of building a highly customizable DSP sound card based on the STM32G474CEx series MCU – highly customizable because, after all, you'll be writing the program yourself. I haven't finished working out the specific features yet, so this section is just a placeholder.

In this project, you'll learn the basics of HAL programming for ST microcontrollers, the fundamentals of C for embedded systems, some ideas about code optimization, some architectural thinking, RTOS basics, and hardware design basics. Modeling basics probably won't be covered, because I'm still learning that myself – so the model here will likely be rather rough.

Oh, right – you'll also pick up a bit of inspiration about audio DSP, but only a bit, since I'm hardly a professional.


## 0.2 Miscellaneous Notes

This chapter is expected to be very, very long. After all, it has to teach C as well, and embedded C makes heavy use of pointers – an abstract thing – and sometimes adopts hard-to-read constructs for optimization; real-time constraints are also an unavoidable concern. So I'll focus on the nature of pointers, their bounds, and common pitfalls; on the readability and maintainability of optimized code; and on coding habits and engineering trade-offs under real-time constraints – trying to explain the principles clearly while also giving practical, actionable advice. Of course, since I'm a learner too, I can't be as comprehensive as a traditional textbook. For PID control, digital signal processing, communication protocol stacks, RTOS kernel implementation, hardware timing and electromagnetic compatibility, my current level isn't enough to explain them thoroughly; I suggest studying the relevant monographs.

I'll use `STM32CubeCLT (including STLinkServer) + VSCode` as the toolchain for development and debugging. I'll explain how to set up this environment in Section 1.

Also deserving of a remark — and I wonder whether it is not my duty to remark upon it — is the circumstance that certain teams have, with respect to the development of ST microcontrollers, entertained ideas of a distinctly *original* character. They have seen fit to turn their backs upon the traditional `Keil μVersion 5` path; yet neither have they seen fit to embrace the more modern `STM32CubeCLT (including STLinkServer) + VSCode` toolchain; nor, indeed, have they condescended to adopt the more universal and more modern `VSCode + GCC + OpenOCD + GDB` arrangement. They have instead arrived — and one can only admire the journey, if not the destination — at the `Keil + VSCode + mingw-w64-ucrt-x86_64-gcc` toolchain, a combination whose precise merits it is somewhat beyond my present powers to enumerate. Readers may wish to note that this environment constitutes a textbook negative example: it remains, at bottom, `Keil` — an IDE whose debugging experience may charitably be described as an acquired taste — they having found its interface, shall we say, aesthetically unrewarding, and having therefore draped over it a `VSCode` skin, before selecting, with a confidence one can only envy, `mingw-w64-ucrt-x86_64-gcc` — an x86_64 build of mingw — to serve as IntelliSense. It is, I think, worth observing that mainstream ST microcontroller families are, generally speaking and as a broad rule, of the ARM persuasion — which renders this x86_64 build of mingw a choice that remains, to the impartial observer, magnificently inexplicable. What follows are some screenshots. **Readers are courteously advised not to imitate the bewildering operations therein depicted.**

![Confusing Env](/img/4.0.1-1.png)