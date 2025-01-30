# Hash Function Project

This project implements a custom **hash function** optimized for word-based hashing using probability distributions.

## 📂 Project Structure
```
.
hash_function_project/
├── src/
│   ├── hash_function.c
│   ├── main.c
├── include/
│   ├── hash_function.h
├── Makefile
└── README.md
2 directories, 5 files
```
## 🚀 Features
- Efficient hashing based on **word length** and **letter frequency**.
- The statistics are taken from https://norvig.com/mayzner.html and https://en.wikipedia.org/wiki/Letter_frequency.
- Optimized for reducing collisions.
- Uses **global indexing** for faster lookup.


## 🚀 How the Hash Function Works

The hash function is **designed to efficiently hash words** by considering:
1. **Word Length**  
   - Words are first categorized by length using **word frequency statistics**.
2. **First Letter Probability**  
   - The first letter determines the **global index**, based on letter frequency.
3. **Second & Third Letter Influence**  
   - The second and third letters refine the **local index**, ensuring better distribution.

### **🔢 Hash Calculation Steps**
1. **Find word length category (`start_index`)**
2. **Calculate `global_chunk_index` using the first letter**
3. **Calculate `local_chunk_index` using the second and third letters**
4. **Final Hash Value** `hash = global_chunk_index * 25 + local_chunk_index`

### **🖥 Example of Hashing a Word**
#### **Word: "apple"**
1. Length → **5**  
2. First letter `'A'` → **global index: 3**  
3. Second letter `'P'`, Third letter `'P'` → **local index: 7**  
4. **Final Hash: `3 * 25 + 7 = 82`**


## 🛠 How to Build
```bash
mingw32-make

## 🏃 How to Run
bash
./hash_function.exe

## 📜 License
This project is licensed under the MIT License - see the LICENSE file for details.

## ✉️ Author: Xonstantin
