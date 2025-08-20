# 🔐 Enigma Machine Simulator

*A Modern Take on a Historic Cipher Device*

---

## ✨ Project Overview

Welcome to the **Enigma Machine Simulator**!
This is a **C++ application** that faithfully recreates the mechanics of the famous **Enigma machine**.

By using a system of configurable **rotors**, a **plugboard**, and a **reflector**, this program can transform plain text into a scrambled cipher and back again.

Think of it as a **historical encryption gadget, powered by code!** 💻

---

## ⚙️ Key Features

* **Customizable Rotors**: Choose rotor types and arrange them in any order for unique encryption.
* **Stepping Mechanism**: Rotors advance automatically with each character, making encryption complex and secure.
* **Plugboard (Steckerbrett)**: Swap pairs of letters to add another encryption layer.
* **Reflectors**: Bounce signals back through the rotors, ensuring different outputs every time.

---

## 📂 Project Structure

```
📦 enigma_app
 ┣ 📂 bin
 ┃ ┗ 📂 Debug
 ┃ ┃ ┗ 📜 enigma_app.exe        # Compiled program
 ┣ 📂 obj
 ┃ ┗ 📂 Debug
 ┃ ┃ ┗ 📜 main.o                # Intermediate build files
 ┣ 📜 decrypt                   # Output: decrypted message
 ┣ 📜 dlog                      # Output: decryption log
 ┣ 📜 elog                      # Output: encryption log
 ┣ 📜 encrypt                   # Output: encrypted message
 ┣ 📜 esetup                    # Input: machine settings
 ┣ 📜 main.cpp                  # Main source code
 ┗ 📜 plain                     # Input: plaintext message
```

---

## 🚀 How to Run the Program

### 1️⃣ Configure Your Machine

Create a file named **`esetup`** and define your machine’s settings:

* **Line 1 (Plugboard):** Pairs of letters to swap

  ```
  ag bs
  ```
* **Lines 2-x (Rotors):** Rotor type, position, window setting

  ```
  31a
  ```
* **Final Line (Reflector):** Reflector type

  ```
  t
  ```

### 2️⃣ Prepare Your Message

Place your message inside a file named **`plain`**. The program reads this file for encryption.

### 3️⃣ Compile and Execute

Open your terminal in the project folder:

```bash
# Compile
g++ main.cpp -o enigma_app

# Run
./enigma_app
```

### 4️⃣ Check the Results

After running, you’ll find these new files in your directory:

* **encrypt** → Encrypted message
* **decrypt** → Decrypted message
* **elog** → Encryption log
* **dlog** → Decryption log

---

## 💡 Future Ideas

* 📱 **Graphical User Interface (GUI):** Full app with a visual layout for machine setup.
* 🌐 **Web-Based Version:** Port logic to JavaScript for an interactive website.
* 🧩 **More Rotors:** Add authentic Enigma rotor wirings for greater accuracy.
* 📜 **Extensive Documentation:** A detailed guide on Enigma’s history & inner workings.

---

## 🏷️ Topics

`c++` `cryptography` `encryption` `cipher` `enigma-machine`  `simulator`
