# 第2节：Ubuntu环境配置

## 术语快查表

| 术语 | 简单解释 |
|------|----------|
| 编译器 | 把源代码转换成机器可以执行的程序的工具 |
| GCC | GNU Compiler Collection，Linux下最常用的C/C++编译器 |
| Clang | LLVM项目的C/C++编译器，报错信息比GCC更友好 |
| 解释器 | 逐行执行代码的程序，不需要提前编译成机器码 |
| pip | Python的包管理工具，用来安装第三方库 |
| 虚拟环境 | 隔离的Python环境，不同项目可以用不同版本的库，互不干扰 |
| venv | Python自带的虚拟环境管理工具 |
| Shell | 命令行解释器，你输入命令它就执行 |
| Bash | 最常用的Shell程序，Ubuntu默认就用它 |
| Meson | 构建系统，用来配置和编译大型项目 |
| Ruby | 一种编程语言，某些工具（比如Jekyll）依赖它 |

---

## 2.1 目录结构：把代码放在该放的地方

在Windows上，你的文档、下载、桌面都在`C:\Users\你的用户名\`下面。WSL里也有一个对应的位置：`/home/你的用户名/`。

建议在这个目录下新建一个文件夹，专门存放所有课程项目和个人代码。我习惯叫它`project`。你可以叫别的名字，但后面的讲解会默认使用`project`这个名字。

创建一个`project`目录：

```bash
cd ~
mkdir project
```

`~`是当前用户home目录的简写。上面两条命令等价于：

```bash
mkdir /home/你的用户名/project
```

以后所有代码项目都放在`~/project/`下面。每个项目单独建一个子目录。比如：

```
~/project/
├── os-lab-1/
├── compiler-assignment/
├── personal-website/
└── meson-demo/
```

这样做的原因很简单：当你以后需要备份、打包、或者用Git管理代码时，只需要操作这一个文件夹。它和系统文件分开了，不会不小心删掉系统关键文件，也不会被系统更新影响。

---

## 2.2 编译器：GCC和Clang

Linux下写C/C++，编译器有两个主流选择：GCC和Clang。两个都装上，不冲突。

### 2.2.1 安装GCC

GCC是Linux生态里最老牌的编译器。Ubuntu默认不带，需要手动安装：

```bash
sudo apt update
sudo apt install gcc g++
```

`gcc`是C编译器，`g++`是C++编译器。两个是独立的程序，通常一起装。

验证安装：

```bash
gcc --version
g++ --version
```

第一条显示GCC版本信息，第二条显示G++版本信息。

### 2.2.2 安装Clang

Clang是LLVM项目的C/C++编译器。和GCC相比，它的报错信息通常更清晰、更准确，对新手更友好。

```bash
sudo apt install clang
```

验证：

```bash
clang --version
```

### 2.2.3 GCC和Clang的区别（简版）

你现在不需要深入了解两者的实现差异。只需要知道三件事：

- 绝大多数代码在两个编译器上都能编译通过。
- 有些课程或项目会指定用某一个（比如操作系统实验有时要求GCC）。
- 两个都装不占多少空间，用到哪个就用哪个。

日常写作业，用Clang起步更舒服，因为它的报错信息更容易看懂。

---

## 2.3 Python环境：pip和venv

Ubuntu 24.04预装了Python 3。打开终端输入`python3 --version`就能看到。

但预装版本通常不带pip（包管理工具），也没有默认配置虚拟环境。需要手动配置。

### 2.3.1 安装pip

```bash
sudo apt install python3-pip
```

验证：

```bash
pip3 --version
```

注意命令是`pip3`不是`pip`。

### 2.3.2 为什么要用虚拟环境

Python有一个特点：所有通过pip安装的第三方库，默认装到系统的Python目录下。这意味着：

- 项目A需要`requests 2.31.0`，项目B需要`requests 2.20.0`。两个项目各自需要不同版本，但你只能装一个。
- 装多了之后，系统Python目录变得乱七八糟，你自己都分不清哪些库是哪个项目用的。
- 想删掉某个库，又怕删了别的项目依赖的东西。

虚拟环境解决的就是这个问题。

**虚拟环境是什么**：在一个独立目录里，复制一份Python解释器和pip，然后所有安装的库都放在这个目录里，和系统Python隔离。每个项目有自己的虚拟环境，里面装的库互不干扰。

你可以这样理解：系统Python是一间公共厨房，所有人都在里面做饭，共用调料，最后乱成一团。虚拟环境是每个人自己带了一个便携料理台，自己用自己的调料，用完收走，不影响别人。

### 2.3.3 安装venv

安装 venv

Ubuntu 将 Python 的 venv 模块拆分到了单独的软件包中，需要手动安装：

```bash
sudo apt install python3-venv
```

安装完成后，就可以正常创建虚拟环境了。

创建一个虚拟环境：

```bash
python3 -m venv ~/project/myproject/venv
```

这会在`~/project/myproject/venv/`目录下创建一套独立的Python环境。

激活虚拟环境：

```bash
source ~/project/myproject/venv/bin/activate
```

激活后，终端提示符前面会出现`(venv)`字样。此时执行`pip install`安装的库只会装到这个虚拟环境里。

退出虚拟环境：

```bash
deactivate
```

更常用的做法是：每个项目根目录下放一个`venv`文件夹，然后在项目根目录执行`source venv/bin/activate`激活。

### 2.3.4 一个注意事项

`venv`只管理Python库。如果你需要不同版本的Python本身（比如项目要求Python 3.10，系统装的是3.12），venv做不到。那是pyenv或者其他工具的事情，以后再说。

---

## 2.4 Bash基础

你现在已经打开了Ubuntu终端，正在和Bash打交道。Bash是你输入命令之后接收和解释这些命令的程序。

下面列出最常用的几条，如果还不太熟悉可以跟着敲一遍。注意每条命令后面都有简短的注释。

### 2.4.1 文件和目录操作

```bash
pwd                     # 显示当前所在目录的完整路径
ls                      # 列出当前目录下的文件和文件夹
ls -la                  # 列出所有文件（包括隐藏的），显示详细信息
cd 目录名               # 切换到指定目录
cd ..                   # 回到上级目录
cd ~                    # 回到自己的home目录
mkdir 目录名            # 新建一个目录
rmdir 目录名            # 删除空目录
rm 文件名               # 删除文件
rm -rf 目录名           # 强制删除目录及里面所有内容（小心用）
cp 源文件 目标位置      # 复制文件
mv 源文件 目标位置      # 移动文件（也可以用来改名）
```

### 2.4.2 查看和编辑文件

```bash
cat 文件名              # 把文件内容打印到屏幕上
less 文件名             # 分页查看大文件，按q退出
head -n 10 文件名       # 查看文件前10行
tail -n 10 文件名       # 查看文件最后10行
nano 文件名             # 用nano编辑器打开文件
code 文件名             # 用VS Code打开文件（如果装了Remote-WSL）
```

### 2.4.3 权限相关

```bash
chmod +x 文件名         # 给文件添加可执行权限
sudo 命令               # 以管理员身份执行命令（需要输入密码）
```

### 2.4.4 通配符和自动补全

按`Tab`键可以自动补全命令和文件名。输入`cd ~/pro`然后按`Tab`，如果`~/project`存在，它会自动补全成`cd ~/project`。

`*`是通配符，匹配任意字符串。比如`ls *.c`会列出所有以`.c`结尾的文件。

### 2.4.5 历史记录

按上下方向键可以翻出之前输入过的命令。输入`history`可以看到完整的命令历史列表。

这些命令不需要背。用多了自然就记住了。遇到不记得的，用`--help`参数查看帮助，比如`ls --help`。

---

## 2.5 Meson：构建系统的安装与验证

附录已经安装过Meson，但当时只是为了验证工具链存在。这里做同样的验证，确保环境配置完整。

确认Meson已安装：

```bash
meson --version
```

如果提示命令未找到，执行：

```bash
sudo apt install meson ninja-build
```

Meson本身是一个Python程序（用pip也可以装），但用apt装更省事，依赖关系由系统包管理器处理。

验证Ninja（Meson的后端构建工具）：

```bash
ninja --version
```

如果你决定以后用Meson管理项目，到那个时候再系统学习。现在只要知道：`meson setup builddir`生成构建文件，`meson compile -C builddir`执行编译。第二章开始会用到。

---

## 2.6 Ruby：某些工具的前置依赖

不是所有CS学生都需要Ruby。但如果你以后要做这些事情，Ruby就绕不开：

- 用Jekyll搭建个人博客（GitHub Pages默认支持Jekyll）
- 写Chef或Puppet的自动化脚本
- 维护Ruby on Rails项目（选了相关课程或实习的话）

安装：

```bash
sudo apt install ruby ruby-dev
```

验证：

```bash
ruby --version
```

暂时不需要学Ruby语法。装好就行。

---

## 2.7 本节验证动作

完成以下检查，确认所有工具就绪：

1. **GCC**：`gcc --version`正常输出，不报错
2. **Clang**：`clang --version`正常输出
3. **pip**：`pip3 --version`正常输出
4. **venv**：执行`python3 -m venv /tmp/test-venv`，然后`ls /tmp/test-venv/bin/`能看到`activate`文件。最后`rm -rf /tmp/test-venv`清理掉
5. **Meson**：`meson --version`正常输出
6. **Ninja**：`ninja --version`正常输出
7. **Ruby**：`ruby --version`正常输出

---

## 2.8 可能遇到的问题

**`sudo apt update`报错“无法解析域名”**
：网络问题。检查WSL能不能ping通外网。如果`.wslconfig`配好了还是不行，试试临时关掉代理工具。

**`pip3`安装的包在命令行找不到**
：pip安装的可执行文件放在`~/.local/bin/`目录下。如果这个目录不在PATH里，执行`echo 'export PATH=$PATH:~/.local/bin' >> ~/.bashrc && source ~/.bashrc`。

**Meson和Ninja版本过旧**
：Ubuntu 24.04自带的版本通常够用。如果之后遇到某些项目要求更新版本，可以通过pip安装`pip3 install meson`，然后确保`~/.local/bin`在PATH前面（比系统路径优先）。

---
