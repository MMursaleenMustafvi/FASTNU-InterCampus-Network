# FAST-NU Inter-Campus Communication System  
A real-time communication system connecting all FAST-NU campuses using C++ TCP and UDP socket programming.

---

## 🚀 Overview
This project implements a **central server** that allows multiple campuses (Lahore, Karachi, Islamabad, Peshawar, CFD, Multan) to communicate securely and efficiently.  
It supports **real-time messaging**, **broadcasts**, **authentication**, and **UDP heartbeat status tracking**.

---

## 🏗️ System Architecture

### **1. TCP Server**
- Accepts client connections  
- Verifies campus credentials  
- Prevents duplicate campus logins  
- Routes messages between campuses  
- Handles admin broadcasts  

### **2. UDP Heartbeat Server**
- Receives heartbeat packets every 5 seconds  
- Monitors online campus presence  
- Lightweight and non-blocking  

### **3. Client**
- Connects via TCP  
- Sends direct messages like:  
  ```
  Karachi:Hello from Lahore!
  ```
- Sends heartbeats using UDP  
- Displays formatted messages, system notifications & server broadcasts

---

## ⚡ Features
- ✔ **Multi-threaded server**—each TCP connection handled by a dedicated thread  
- ✔ **Secure campus authentication**  
- ✔ **Direct campus-to-campus messaging**  
- ✔ **Server-side broadcast support**  
- ✔ **UDP heartbeat monitoring**  
- ✔ **Colored CLI with formatted messages**  
- ✔ **Graceful connection closing**  

---

## 🔧 Technologies Used
- C++17  
- POSIX Sockets (TCP + UDP)  
- Multithreading (`std::thread`)  
- Mutex locks for synchronization  
- ANSI escape codes for UI  

---

## 📂 Project Structure
```
/server
   server.cpp

/client
   client.cpp
```

---

## ▶️ How to Run

### **Start Server**
```
g++ server.cpp -pthread -o ser
./ser
```

### **Start Client**
```
g++ client.cpp -pthread -o cli
./cli
```

---

## 📡 Message Format
```
TargetCampus:Your message here
```

Example:
```
Islamabad:Meeting at 3 PM
```

---

## 🖋️ Author
**Muhammad Mursaleen Mustafvi**
FAST-National University of Computer and Emerging Sciences  
2024–2025

---



## ⭐ Feedback & Contributions  
Pull requests are welcome!  
If you have suggestions for improvements or additional features, feel free to open an issue.

