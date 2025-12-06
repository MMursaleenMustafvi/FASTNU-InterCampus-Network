# Building-a-Multi-Campus-Communication-System-with-C-C-Sockets-and-Cisco-Packet-Tracer

Modern universities often operate across multiple cities, creating the need for seamless communication between campuses. To simulate this concept, I built a **nationwide multi-campus communication system** that combines **network design** and **application development** into one integrated project.


## 🌐 Part 1 — Designing the WAN Across Pakistan

The goal was to connect six FAST-NUCES campuses through a reliable and scalable WAN. I used **Cisco Packet Tracer** to design a mesh-based backbone topology ensuring redundancy and high availability.

### Key Highlights:

* **Mesh Topology:** Multiple redundant paths between backbone routers
* **Dynamic Routing (RIP v2):** Automatic propagation of routes across campuses
* **DHCP:** Automated IP distribution within each campus
* **Department-Level Network Segmentation:** Faculty, Admin, Student Labs, Server Rooms
* **Serial  Links:** Simulated real-world WAN connectivity

Each campus operated as a combination of VLANs and subnets, uniquely addressable within the IP hierarchy.

---

## 💻 Part 2 — Implementing the Multi-Campus Messaging System in C/C++

To simulate communication between campuses, I built a multi-client system using **TCP and UDP sockets**.

### ✔ TCP (Reliable)

Used for:

* Campus Client authentication
* Direct messages between departments across campuses

### ✔ UDP (Connectionless)

Used for:

* Heartbeat status packets
* System-wide announcements
* Low-overhead campus presence updates

---

## 🏗 System Architecture

### Central Server (Islamabad)

* Accepts multiple TCP clients
* Validates campus credentials
* Routes inter-campus messages
* Displays real-time heartbeats from all campuses
* Provides an admin console for broadcasts

### Campus Clients (LHR, KHI, PSH, CFD, MULTAN)

* Connect to the server using TCP
* Send & receive cross-campus messages
* Periodically send UDP “online” signals
* Provide a simple console UI for department users

---

## 🧪 Testing & Validation

* Inter-campus routing verified using `show ip route`
* DHCP pools correctly assigned IPs across VLANs
* Message routing confirmed through TCP relay
* UDP broadcasts received campus-wide

---

## 🎓 What I Learned

* Designing multi-site WANs
* Implementing hybrid TCP/UDP architectures
* Managing concurrent TCP clients using threads
* Benefits & tradeoffs of connection-oriented vs connectionless protocols
* Practical system modeling that applies to real universities, enterprises, or government institutions

---

## 👀 Final Thoughts

This project blends **networking, distributed systems, and system design**, making it perfect for students or professionals looking to strengthen both their networking and programming skills.

If you'd like to explore or reuse the project, you can check the GitHub repository in the description.

..
