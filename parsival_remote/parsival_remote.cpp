#include "wiiuse.h"
#include "Serial.h"
#include <memory>
#include <unordered_map>

#define MAX_WIIMOTES                1

std::shared_ptr<Serial> serial = nullptr;
std::unordered_map<std::string, uint8_t> commandNumber;

void handle_event(struct wiimote_t* wm) {
    std::cout << std::endl << std::endl << "--- EVENT [id " << wm->unid << "] ---" << std::endl;
    if (nullptr == serial || serial->error) {
        return;
    }

    if (IS_PRESSED(wm, WIIMOTE_BUTTON_A)) {
        std::cout << "A pressed" << std::endl;
        serial->writeByte(commandNumber["A"]);
    } if (IS_PRESSED(wm, WIIMOTE_BUTTON_B)) {
        std::cout << "B pressed" << std::endl;
        serial->writeByte(commandNumber["B"]);
    } if (IS_PRESSED(wm, WIIMOTE_BUTTON_UP)) {
        std::cout << "UP pressed" << std::endl;
        serial->writeByte(commandNumber["U"]);
    } if (IS_PRESSED(wm, WIIMOTE_BUTTON_DOWN))    {
        std::cout << "DOWN pressed" << std::endl;
        serial->writeByte(commandNumber["D"]);
    } if (IS_PRESSED(wm, WIIMOTE_BUTTON_LEFT))    {
        std::cout << "LEFT pressed" << std::endl;
        serial->writeByte(commandNumber["L"]);
    } if (IS_PRESSED(wm, WIIMOTE_BUTTON_RIGHT))    {
        std::cout << "RIGHT pressed" << std::endl;
        serial->writeByte(commandNumber["R"]);
    } if (IS_PRESSED(wm, WIIMOTE_BUTTON_MINUS))    {
        std::cout << "MINUS pressed" << std::endl;
        serial->writeByte(commandNumber["-"]);
    } if (IS_PRESSED(wm, WIIMOTE_BUTTON_PLUS))    {
        std::cout << "PLUS pressed" << std::endl;
        serial->writeByte(commandNumber["+"]);
    } if (IS_PRESSED(wm, WIIMOTE_BUTTON_ONE)) {
        std::cout << "ONE pressed" << std::endl;
        serial->writeByte(commandNumber["1"]);
    } if (IS_PRESSED(wm, WIIMOTE_BUTTON_TWO)) {
        std::cout << "TWO pressed" << std::endl;
        serial->writeByte(commandNumber["2"]);
    } if (IS_PRESSED(wm, WIIMOTE_BUTTON_HOME))    {
        std::cout << "HOME pressed" << std::endl;
        serial->writeByte(commandNumber["H"]);
    }

    if (wm->exp.type == EXP_NUNCHUK || wm->exp.type == EXP_MOTION_PLUS_NUNCHUK) {
        /* nunchuk */
        struct nunchuk_t* nc = (nunchuk_t*)&wm->exp.nunchuk;

        if (IS_PRESSED(nc, NUNCHUK_BUTTON_C)) {
            std::cout << "Nunchuk: C pressed" << std::endl;
            serial->writeByte(commandNumber["C"]);
        }
        if (IS_PRESSED(nc, NUNCHUK_BUTTON_Z)) {
            std::cout << "Nunchuk: Z pressed" << std::endl;
            serial->writeByte(commandNumber["Z"]);
        }

        bool leftTurn = nc->js.x < -0.5;
        bool rightTurn = nc->js.x > 0.5;
        bool nunchukUp = nc->js.y > 0.5;
        bool nunchukDown = nc->js.y < -0.5;

        if (leftTurn) {
            std::cout << "Nunchuk: Left pressed" << std::endl;
            serial->writeByte(commandNumber["NL"]);
        } else if (rightTurn) {
            std::cout << "Nunchuk: Right pressed" << std::endl;
            serial->writeByte(commandNumber["NR"]);
        } if (nunchukUp) {
            std::cout << "Nunchuk: Up pressed" << std::endl;
            serial->writeByte(commandNumber["NU"]);
        } else if (nunchukDown) {
            std::cout << "Nunchuk: Down pressed" << std::endl;
            serial->writeByte(commandNumber["ND"]);
        }
    }
}

short any_wiimote_connected(wiimote** wm, int wiimotes) {
    int i;
    if (!wm) {
        return 0;
    }

    for (i = 0; i < wiimotes; i++) {
        if (wm[i] && WIIMOTE_IS_CONNECTED(wm[i])) {
            return 1;
        }
    }

    return 0;
}

int main(int argc, char** argv) {
    commandNumber.insert(std::make_pair<std::string, uint8_t >("U", 0));
    commandNumber.insert(std::make_pair<std::string, uint8_t >("D", 1));
    commandNumber.insert(std::make_pair<std::string, uint8_t >("L", 2));
    commandNumber.insert(std::make_pair<std::string, uint8_t >("R", 3));
    commandNumber.insert(std::make_pair<std::string, uint8_t >("NU", 4));
    commandNumber.insert(std::make_pair<std::string, uint8_t >("ND", 5));
    commandNumber.insert(std::make_pair<std::string, uint8_t >("NL", 6));
    commandNumber.insert(std::make_pair<std::string, uint8_t >("NR", 7));
    commandNumber.insert(std::make_pair<std::string, uint8_t >("A", 8));
    commandNumber.insert(std::make_pair<std::string, uint8_t >("B", 9));
    commandNumber.insert(std::make_pair<std::string, uint8_t >("C", 10));
    commandNumber.insert(std::make_pair<std::string, uint8_t >("Z", 11));
    commandNumber.insert(std::make_pair<std::string, uint8_t >("1", 12));
    commandNumber.insert(std::make_pair<std::string, uint8_t >("2", 13));
    commandNumber.insert(std::make_pair<std::string, uint8_t >("-", 14));
    commandNumber.insert(std::make_pair<std::string, uint8_t >("+", 15));
    commandNumber.insert(std::make_pair<std::string, uint8_t >("H", 16));

    serial = std::make_shared<Serial>("/dev/cu.usbserial-A8004xcb");
    if (!serial || serial->error) {
        std::cerr << "Error opening serial." << std::endl;
        return -1;
    }
    std::cout << "serial connection with robot opened" << std::endl;

    wiimote** wiimotes =  wiiuse_init(MAX_WIIMOTES);

    int found = wiiuse_find(wiimotes, MAX_WIIMOTES, 3);
    if (!found) {
        std::cerr << "No wiimotes found." << std::endl;
        return 0;
    }

    int connected = wiiuse_connect(wiimotes, MAX_WIIMOTES);
    if (connected) {
        std::cout << "Connected to " << connected << " wiimotes (of " << found << " found)." << std::endl;
    } else {
        std::cerr << "Failed to connect to any wiimote." << std::endl;
        return 0;
    }

    wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);
    wiiuse_rumble(wiimotes[0], 1);
#ifndef WIIUSE_WIN32
    usleep(200000);
#else
    Sleep(200);
#endif
    wiiuse_rumble(wiimotes[0], 0);
    wiiuse_motion_sensing(wiimotes[0], 0);
    wiiuse_set_ir(wiimotes[0], 0);
    wiiuse_set_motion_plus(wiimotes[0], 0);

    while (any_wiimote_connected(wiimotes, MAX_WIIMOTES)) {
        if (wiiuse_poll(wiimotes, MAX_WIIMOTES)) {
            for (int i = 0; i < MAX_WIIMOTES; ++i) {
                switch (wiimotes[i]->event) {
                    case WIIUSE_EVENT:
                        handle_event(wiimotes[i]);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    wiiuse_cleanup(wiimotes, MAX_WIIMOTES);

    return 0;
}
