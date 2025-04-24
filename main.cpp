#include "include/Decoder.h"
#include "include/Encoder.h"
#include "include/String.h"


int main() {
    String toEncodeFileName;
    String encodedFileName;
    String decodedFileName;
    int mode;

    std::cout << "Please select the mode:\n0 - Encoder, 1 - Decoder, 2 - Both\n";
    std::cin >> mode;

    if (mode == 0) {
        std::cout << "Enter the path to file you want to encode:";
        std::cin >> toEncodeFileName;
        std::cout << "Enter the path to the encoded file:";
        std::cin >> encodedFileName;

        const Encoder encoder(toEncodeFileName, encodedFileName);
        encoder.encode();
    } else if (mode == 1) {
        std::cout << "Enter the path to the encoded file:";
        std::cin >> encodedFileName;
        std::cout << "Enter the path to the result file:";
        std::cin >> decodedFileName;

        const Decoder decoder(encodedFileName, decodedFileName);
        decoder.decode();
    } else if (mode == 2) {
        std::cout << "Enter the path to file you want to encode:";
        std::cin >> toEncodeFileName;
        std::cout << "Enter the path to the encoded file:";
        std::cin >> encodedFileName;
        std::cout << "Enter the path to the result file:";
        std::cin >> decodedFileName;

        const Encoder encoder(toEncodeFileName, encodedFileName);
        encoder.encode();
        const Decoder decoder(encodedFileName, decodedFileName);
        decoder.decode();
    } else {
        std::cout << "Selected mode is not available";
    }

    return 0;
}
