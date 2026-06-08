# Warehouse Navigation & Fleet Management System

A C++ application designed to orchestrate warehouse operations by connecting order processing, automated inventory discovery, robot fleet coordination, and real-time telemetry routing.

---

## Core Modules & Data Structures

### 1. Order Management (Queue)

* Handles incoming requests sequentially using a First-In, First-Out (FIFO) linked list structure.
* Tracks active order fulfillment states and maintains an archival history of completed requests.

### 2. Robot Assignment (Circular Queue)

* Distributes fulfillment tasks among a fleet of delivery units using a circular queue implementation.
* Controls work distributions, system logging, operational summaries, and status variations including maintenance cycles.

### 3. Warehouse Inventory (Binary Search Tree)

* Organizes stock records within a structured data architecture sorted by unique tracking identifiers.
* Supports lookups by inventory identifier or descriptive labels to pinpoint stock storage locations.

### 4. Robot Navigation Tracking (Stack)

* Captures real-time directional steering coordinates inside a dynamic stack arrangement.
* Computes safe physical rollback pathways, intercepts unexpected blockages, and reverses tracking arrays to guide units back to home base stations.

---

## Processing Pipeline

```
[Order Enqueued] 
       │
       ▼
[Order Dequeued] ──> [Search Inventory by Name (BST)] ──> [Extract Stock Location]
                                                                  │
                                                                  ▼
[Populate Telemetry Stack] <── [Assign Task to Next Robot] <──────┘

```

1. **Extraction**: An item request gets pulled from the scheduling queue.
2. **Localization**: The inventory system scans for the requested name to pull its location details.
3. **Allocation**: An available delivery unit accepts the assignment description.
4. **Telemetry**: A navigation profile opens up, establishing tracking points out to the designated warehouse sector.

---

## File Architecture

* `main.cpp`: Orchestrates interface flows and binds the system modules together.
* `OrderManager.hpp` / `.cpp`: Manages customer queues and historical processing logs.
* `RobotAssignment.hpp` / `.cpp`: Governs machine allocations and device scheduling constraints.
* `BST.h` / `.cpp`: Implements search logic for stock items.
* `ItemNode.h`: Layout definition for nodes containing stock descriptions.
* `RobotNavigation.hpp` / `.cpp`: Handles path histories and movement logging.

---

## Setup & Compilation

Compile the codebase using any standard modern C++ compiler:

```bash
g++ -std=c++17 main.cpp OrderManager.cpp RobotAssignment.cpp BST.cpp RobotNavigation.cpp -o WarehouseSystem
./WarehouseSystem

```
