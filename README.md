## 
![banner](https://dummyimage.com/900x200/000/00ffea&text=clean+-+File+Organizer)

# 🧹 clean

A fast, cross-platform terminal tool that automatically sorts and
organizes your files into neat, structured folders.\
Built for Windows and Linux using C++, designed for anyone who hates
messy directories.

------------------------------------------------------------------------

## 🚀 Features

-   📁 **Sort by file type** --- images, videos, audio, docs, archives,
    etc\
-   🏷️ **Sort by file name patterns** --- useful for downloaded media\
-   ⚡ **Fast and lightweight** (pure C++)\
-   🖥️ **Runs in the terminal** (Linux + Windows)\
-   🎯 **Zero setup** --- just run and watch your folder get clean\
-   🧠 **Beginner-friendly command structure**

------------------------------------------------------------------------

## 🔧 How It Works (In Simple Terms)

clean scans a folder, reads every file, figures out what type it is, and
moves it into a matching organized directory.

Example:

    /Downloads
       ├── photo1.png  → /Downloads/Images/
       ├── beat.mp3    → /Downloads/Audio/
       ├── movie.mkv   → /Downloads/Videos/
       └── project.pdf → /Downloads/Documents/

It's basically that "Clean your room!" energy --- but for your PC.

------------------------------------------------------------------------

## 🛠️ Installation

### **Clone the repo**

``` bash
git clone https://github.com/Sammiemacha/clean.git
cd clean
```

### **Build (Linux)**

``` bash
g++ -std=c++17 -O2 -Wall -Wextra -Iinclude -o build/clean src/*.cpp src/clean/*.cpp
```

### **Build (Windows / MinGW)**

``` bash
g++ src/main.cpp src/Header.cpp src/FileTypes.cpp src/clean/CleanByType.cpp src/clean/CleanByName.cpp -Iinclude -o build/clean.exe
```

------------------------------------------------------------------------

## 🧪 Usage

Run the tool:

``` bash
./clean 
```

Example:

``` bash
./clean
```

On Windows:

``` bash
clean.exe 
```

------------------------------------------------------------------------

## ⚙️ Roadmap

-   [ ] Add multi-threading for massive directories\
-   [ ] Add a "simulation mode" (preview sorting without moving files)
-   [ ] Add feature to surport command line arguments

------------------------------------------------------------------------

## 🤝 Contributing

Pull requests are welcome.\
If you find bugs or want new features, open an issue.

------------------------------------------------------------------------

## 🔥 Final Note

clean started as a simple, personal tool to tidy messy folders --- now it's
evolving into a full command-line organizer.\
More features coming soon.

> **"Clean code. Clean folders. Clean mind."**
