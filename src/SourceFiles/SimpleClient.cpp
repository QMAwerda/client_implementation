// #include "olc_net.hpp"
// #include <iostream>

// enum class CustomMsgTypes : uint32_t {
//   ServerAccept,
//   ServerDeny,
//   ServerPing,
//   MessageAll,
//   ServerMessage,
// };

// class CustomClient : public olc::net::client_interface<CustomMsgTypes> {
// public:
//   void PingServer() {
//     olc::net::message<CustomMsgTypes> msg;
//     msg.header.id = CustomMsgTypes::ServerPing;

//     // Caution with this...
//     std::chrono::system_clock::time_point timeNow =
//         std::chrono::system_clock::now();

//     msg << timeNow;
//     Send(msg);
//   }

//   void MessageAll() {
//     olc::net::message<CustomMsgTypes> msg;
//     msg.header.id = CustomMsgTypes::MessageAll;
//     Send(msg);
//   }
// };

// int main() {
//   CustomClient c;
//   c.Connect("127.0.0.1", 60000);

//   bool key[3] = {false, false, false};
//   bool old_key[3] = {false, false, false};

//   bool bQuit = false;
//   while (!bQuit) {
//     if (GetForegroundWindow() == GetConsoleWindow()) {
//       key[0] = GetAsyncKeyState('1') & 0x8000;
//       key[1] = GetAsyncKeyState('2') & 0x8000;
//       key[2] = GetAsyncKeyState('3') & 0x8000;
//     }

//     if (key[0] && !old_key[0])
//       c.PingServer();
//     if (key[1] && !old_key[1])
//       c.MessageAll();
//     if (key[2] && !old_key[2])
//       bQuit = true;

//     for (int i = 0; i < 3; i++)
//       old_key[i] = key[i];

//     if (c.IsConnected()) {
//       if (!c.Incoming().empty()) {

//         auto msg = c.Incoming().pop_front().msg;

//         switch (msg.header.id) {
//         case CustomMsgTypes::ServerAccept: {
//           // Server has responded to a ping request
//           std::cout << "Server Accepted Connection\n";
//         } break;

//         case CustomMsgTypes::ServerPing: {
//           // Server has responded to a ping request
//           std::chrono::system_clock::time_point timeNow =
//               std::chrono::system_clock::now();
//           std::chrono::system_clock::time_point timeThen;
//           msg >> timeThen;
//           std::cout << "Ping: "
//                     << std::chrono::duration<double>(timeNow -
//                     timeThen).count()
//                     << "\n";
//         } break;

//         case CustomMsgTypes::ServerMessage: {
//           // Server has responded to a ping request
//           uint32_t clientID;
//           msg >> clientID;
//           std::cout << "Hello from [" << clientID << "]\n";
//         } break;
//         }
//       }
//     } else {
//       std::cout << "Server Down\n";
//       bQuit = true;
//     }
//   }

//   return 0;
// }

#include "olc_net.hpp"
#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

enum class CustomMsgTypes : uint32_t {
  ServerAccept,
  ServerDeny,
  ServerPing,
  MessageAll,
  ServerMessage,
};

class CustomClient : public olc::net::client_interface<CustomMsgTypes> {
public:
  void PingServer() {
    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::ServerPing;

    // Caution with this...
    std::chrono::system_clock::time_point timeNow =
        std::chrono::system_clock::now();

    msg << timeNow;
    Send(msg);
  }

  void MessageAll() {
    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::MessageAll;
    Send(msg);
  }
};

int main() {
  CustomClient c;
  c.Connect("127.0.0.1", 60000);

  bool key[3] = {false, false, false};

  bool bQuit = false;
  struct termios newTerminalSetting; // structures with terminal settings
  int symbol_code;
  newTerminalSetting.c_lflag &=
      ~(ICANON | ECHO); // binary AND with (ICANON + ECHO) - off
                        // (canon)string input and echo input
  // Set new terminal settings
  tcsetattr(STDIN_FILENO, TCSANOW, &newTerminalSetting);
  while (!bQuit) {

    symbol_code = getchar(); // Read sybmol with new terminal settings

    key[0] = symbol_code == 49;
    key[1] = symbol_code == 50;
    key[2] = symbol_code == 51;

    if (key[0]) {
      c.PingServer(); // send ping to server
    }
    if (key[1])
      c.MessageAll();
    if (key[2])
      bQuit = true;

    if (c.IsConnected()) {
      if (!c.Incoming().empty()) {

        auto msg = c.Incoming().pop_front().msg;

        switch (msg.header.id) {
        case CustomMsgTypes::ServerAccept: {
          // Server has responded to a ping request
          std::cout << "Server Accepted Connection\n";
        } break;

        case CustomMsgTypes::ServerPing: {
          // Server has responded to a ping request
          std::chrono::system_clock::time_point timeNow =
              std::chrono::system_clock::now();
          std::chrono::system_clock::time_point timeThen;
          msg >> timeThen;
          std::cout << "Ping: "
                    << std::chrono::duration<double>(timeNow - timeThen).count()
                    << "\n";
        } break;

        case CustomMsgTypes::ServerMessage: {
          // Server has responded to a ping request
          uint32_t clientID;
          msg >> clientID;
          std::cout << "Hello from [" << clientID << "]\n";
        } break;
        }
      }
    } else {
      std::cout << "Server Down\n";
      bQuit = true;
    }
  }

  return 0;
}