# Order Execution and Management System for Deribit Test

## Objective
A high-performance C++ system for order execution and management on the Deribit Test platform. This system facilitates trading of Spot, Futures, and Options, providing real-time market data updates and extensive order management capabilities.

---

## Features

### Order Management
- **Place Order**: Execute new trades on supported instruments.
- **Cancel Order**: Cancel open orders.
- **Modify Order**: Edit existing orders.
- **Get Orderbook**: Fetch the current orderbook data for selected symbols.
- **View Current Positions**: Display active positions for all instruments.

### Real-Time Market Data
- **WebSocket Integration**:
  - Implement WebSocket server functionality.
  - Allow clients to subscribe to symbols.
  - Stream continuous orderbook updates for subscribed symbols.

### Market Coverage
- **Instruments**: Spot, Futures, and Options.
- **Scope**: All supported symbols on the Deribit Test platform.

---

## Project Structure

```plaintext
Order Execution and Management System for Deribit Test  
│   .gitignore                              // Files to be ignored by Git
│   README.md                               // Project documentation
│   xmake.lua                               // Build configuration file
│
└───project                                 // Project source code
    │   .clang-format                       // Clang formatting configuration
    │
    ├───include                             // Header files
    │   └───order-management                // Order management system
    │           access-token-handler.hpp    // Access token handler
    │           benchmarking.hpp            // Benchmarking utilities
    │           config.hpp                  // Configuration settings
    │           core.hpp                    // Core system utilities
    │           deribit-ws-client.hpp       // Deribit WebSocket client
    │           json-map.hpp                // JSON typedefs and utilities
    │           notification-system.hpp     // Notification system
    │           order-book.hpp              // Order book data structure
    │           order-desc.hpp              // Order description
    │           position.hpp                // Position data structure
    │           trade-system.hpp            // Trade system
    │
    ├───src                                 // Source files 
    │   ├───notification-cli                // Notification system CLI
    │   │       main.cpp                    // Main entry point
    │   │       notification.cpp            // Notification system
    │   │       notification.hpp            // Notification system header
    │   │
    │   ├───order-management                // Order management system
    │   │   │   access-token-handler.cpp    // Access token handler
    │   │   │   deribit-ws-client.cpp       // Deribit WebSocket client
    │   │   │   json-map.cpp                // JSON typedefs and utilities
    │   │   │   notification-system.cpp     // Notification system
    │   │   │   trade-system.cpp            // Trade system
    │   │   │
    │   │   └───mappers                     // Mapper classes
    │   │           auth-mapper.hpp         // Authentication mapper
    │   │           cancel-order-mapper.hpp // Cancel order mapper
    │   │           common-json-map.hpp     // Common JSON map utilities
    │   │           modify-order-mapper.hpp // Modify order mapper
    │   │           order-book-mapper.hpp   // Order book mapper
    │   │           place-order-mapper.hpp  // Place order mapper
    │   │           position-mapper.hpp     // Position mapper
    │   │           subscription-mapper.hpp // Subscription mapper
    │   │
    │   └───trade-system-cli                // Trade system CLI
    │           cli.cancel-order.cpp        // Cancel order CLI
    │           cli.cpp                     // CLI utilities 
    │           cli.get-order-book.cpp      // Get order book CLI
    │           cli.hpp                     // CLI header
    │           cli.modify-order.cpp        // Modify order CLI
    │           cli.place-order.cpp         // Place order CLI
    │           cli.position.cpp            // Position CLI
    │           main.cpp                    // Main entry point
    │
    └───tests                               // Unit tests
        └───order-management                // Order management system tests
                latency-tests.cpp           // Latency tests
```
## Setup Instructions

### Prerequisites
- **A C++ compiler** supporting C++23 (e.g., GCC, Clang, or MSVC).
- **An active account** on Deribit Test.
- **API keys** generated from the Deribit Test platform.
- **XMake** for building the project.

### Steps to Run

1. **Clone the Repository**  
  Clone this repository to your local machine:
    ```bash
    git clone <repository-url>
    ```

2. **Configure API Keys**
Open the file `project/include/order-management/config.hpp` and add your client ID and client secret generated from Deribit Test.
Example:
```C++
inline constexpr const char* DeribitClientId     = "your_client_id";
inline constexpr const char* DeribitClientSecret = "your_client_secret";
```
4. **Compile the Project**
Use the following command to build the project:
```bash
xmake build
```

5. **Run the Application**
Execute the compiled file:
```bash
xmake run notification-cli
xmake run trade-system-cli
```
