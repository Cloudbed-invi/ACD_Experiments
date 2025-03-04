# Flex & Bison Setup and Usage Guide

This guide explains how to install GCC using MinGW and set up WinFlexBison (Flex & Bison) on Windows. It also provides instructions on how to clone the repository and compile/execute your Flex and Bison programs, which are located in your GitHub repository under **ACD_Experiments\Experiment5\flex** and **ACD_Experiments\Experiment5\yacc**.

---

## 1. Cloning the Repository

1. **Install Git (if not already installed):**
   - Download Git from [git-scm.com](https://git-scm.com/) and install it.

2. **Clone the Repository:**
   - Open a Command Prompt or Git Bash and run:
     ```sh
     git clone https://github.com/Cloudbed-invi/ACD_Experiments.git
     ```
   - This will create a folder named `ACD_Experiments` in your current directory.

---

## 2. Installing GCC Using MinGW

1. **Download MinGW:**
   - Go to the [MinGW-w64 project page](https://sourceforge.net/projects/mingw/).
   - Download the latest version of MinGW-w64 suitable for your system (32-bit or 64-bit).

2. **Install MinGW:**
   - Run the installer and follow the on-screen instructions.
   - By default, MinGW will be installed in a directory like:
     ```
     C:\MinGW\ or C:\MinGW-w64\
     ```

3. **Set the PATH Environment Variable:**
   - Open **System Properties**:
     - Right-click on **This PC** (or **My Computer**) and choose **Properties**.
     - Click on **Advanced system settings**.
     - Click on the **Environment Variables** button.
   - Under **System variables**, find the variable named **Path**, select it, and click **Edit**.
   - Add the path to the `bin` directory of your MinGW installation (e.g., `C:\MinGW\bin` or `C:\MinGW-w64\bin`).
   - Click **OK** to close the dialogs.
   - **Important:** Close and reopen any terminal/Command Prompt windows so that the new PATH is applied.

4. **Verify Installation:**
   - Open a new Command Prompt and type:
     ```sh
     gcc --version
     ```
   - You should see version information for GCC.

---

## 3. Installing Flex and Bison (WinFlexBison)

1. **Download WinFlexBison:**
   - Visit the [WinFlexBison releases page](https://github.com/lexxmark/winflexbison/releases).
   - Download the latest release (e.g., `win_flex_bison-2.5.25.zip`).

2. **Extract the Files:**
   - Extract the ZIP file to a folder on your C: drive. For example:
     ```
     C:\win_flex_bison-2.5.25
     ```

3. **Add WinFlexBison to PATH:**
   - Open **Environment Variables** (see above).
   - Under **System variables**, edit the **Path** variable.
   - Add the path to the WinFlexBison folder. For example:
     ```
     C:\win_flex_bison-2.5.25
     ```
   - Click **OK** to save your changes.
   - **Important:** Close and reopen your terminal/Command Prompt so the new PATH takes effect.

4. **Verify Installation:**
   - Open a new Command Prompt and run:
     ```sh
     win_flex --version
     win_bison --version
     ```
   - Both commands should display version information for WinFlexBison.

---

## 4. Compiling and Running Flex/Bison Programs

Your repository structure is as follows:
- **ACD_Experiments\Experiment5\flex** – Contains your Flex source file (`lexer.l`).
- **ACD_Experiments\Experiment5\yacc** – Contains your Bison source file (`parser.y`) and a copy of the Flex file (`lexer.l`) that works with it.

### **A. Compiling the Flex Code (Optional)**

If you want to compile the Flex file from the **flex** folder:

1. **Navigate to the Flex Directory:**
   ```sh
   cd ACD_Experiments\Experiment5\flex
   ```

2. **Run Flex:**
   ```sh
   win_flex lexer.l
   ```
   - This generates a file called `lex.yy.c` in the same folder.

### **B. Compiling the Bison Code (Recommended)**

It is recommended to compile the complete parser using the files in the **yacc** folder.

1. **Navigate to the Yacc Directory:**
   ```sh
   cd ACD_Experiments\Experiment5\yacc
   ```

2. **Run Bison:**
   ```sh
   win_bison -d parser.y
   ```
   - This generates two files:
     - `parser.tab.c` (the parser source code)
     - `parser.tab.h` (the token definitions)

   **Note:** If the Flex file in this folder (`lexer.l`) includes `#include "y.tab.h"`, change it to `#include "parser.tab.h"` or rename `parser.tab.h` to `y.tab.h`.

3. **Run Flex in the Yacc Directory:**
   ```sh
   win_flex lexer.l
   ```
   - This will generate `lex.yy.c` in the **yacc** folder.

### **C. Compiling the Generated Files with GCC**

1. **Compile the Program:**
   - In the **ACD_Experiments\Experiment5\yacc** directory (which now contains both `parser.tab.c` and `lex.yy.c`), run:
     ```sh
     gcc -o my_parser parser.tab.c lex.yy.c -lm
     ```
   - This creates an executable named `my_parser`.

2. **Run the Executable:**
   ```sh
   my_parser
   ```
   - Follow the on-screen instructions for your program.

---

## 5. Summary

- **GCC via MinGW:** Install MinGW and add its `bin` directory to your PATH.
- **WinFlexBison:** Download from GitHub, extract to `C:\win_flex_bison-2.5.25`, and add its directory to your PATH.
- **Repository:** Clone the repository:
  ```sh
  git clone https://github.com/Cloudbed-invi/ACD_Experiments.git
  ```
- **Flex (in flex folder):** Optionally, run `win_flex lexer.l` in **ACD_Experiments\Experiment5\flex** to generate `lex.yy.c`.
- **Bison (in yacc folder):** Run `win_bison -d parser.y` in **ACD_Experiments\Experiment5\yacc** to generate `parser.tab.c` and `parser.tab.h`, then run `win_flex lexer.l` there to generate `lex.yy.c`.
- **Compile:** In **ACD_Experiments\Experiment5\yacc**, compile the files using:
  ```sh
  gcc -o my_parser parser.tab.c lex.yy.c -lm
  ```
- **Run:** Execute your program with:
  ```sh
  my_parser
  ```
