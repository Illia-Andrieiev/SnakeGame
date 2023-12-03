#include<string>
#include<iostream>
#include<fstream>
#include<ctime>
#include "SnakeGame.h"
#include"PixelBox.h"
#include"libs/boost/ut.hpp"

// Constructors
SnakeGame::SnakeGame(int argc, char** argv, int boxSize) {
    // Initialization colours
    const unsigned char green[3] = { 50, 205, 50 };
    const unsigned char grey[3] = { 200, 200, 200 };
    unsigned char firstBoxColour[3] = { 0,0,139 };
    unsigned char secondBoxColour[3] = { 139, 0, 0 };
    unsigned char colourEnlarge[3] = { 255, 0, 0 };
    unsigned char colourShrink[3] = { 218, 112, 214 };
    setSnakeColour(firstBoxColour, secondBoxColour);
    setApplesColour(colourEnlarge, colourShrink);
    // Initialization game parametrs
    this->argc = argc;
    this->argv = argv;
    if (boxSize < 10) // min boxSize == 5
        this->boxSize = 5;
    else // max boxSize == 10
        this->boxSize = 10;
    maximumEnlargeApples = 10;
    maximumShrinkApples = 4;
    shrinkAppleLifeTime = 12000;
    enlargeAppleLifeTime = 9000;
    // Assign background and display
    background.assign(64, 64, 1, 3, 0).draw_rectangle(0, 0, W - 1, H - 1, green, 1.0f)
        .noise(10).resize(W, H + scoreTableH).blur(2).normalize(0, 128).draw_rectangle(0, 0, W-1, scoreTableH-1, grey, 1.0f);
    cimg_library::CImgDisplay displ(background, "* CImg-Snake *");
    this->disp = displ;
    readRecordsFromFile("Records.txt");
}
// Main game method
void SnakeGame::game() {
    /*
        Prepare for main loop
    */
    cimg_usage("A simple Snake game, using the CImg Library");
    const int delay = cimg_option("-s", 10, "Game speed (lower value means faster)");
    const int zoom = cimg_option("-z", 3, "Zoom factor");
    const bool full = cimg_option("-f", false, "Fullscreen mode");
    // Define game colors
    const unsigned char blue[] = { 128,200,255 }, grey[3] = { 200, 200, 200 }, black[] = { 0,0,0 },
        red[] = { 255,0,0 }, white[] = { 255,255,255 }, green[3] = { 50, 205, 50 };
    // Score board image
    cimg_library::CImg<unsigned char> score;
    score.assign(64, 64, 1, 3, 0).normalize(0, 128).
        draw_rectangle(0, 0, W, scoreTableH*3, grey, 1.0f).resize(W, scoreTableH);
    // Open display window
    if (zoom > 1) disp.resize(-100 * zoom, -100 * zoom);
    if (full) disp.toggle_fullscreen().display(background);
    // Create snake, score message and display game menu
    Snake snake(*this, static_cast<const int>(delay * 6.5), firstBoxColour, secondBoxColour, colourEnlarge, colourShrink, boxSize,9);
    std::string message;
    displayMenu();
    /*
        Start main game loop
    */
    while (!disp.is_closed()) {
        if (disp.is_keyESC()) {
            setNewRoundParametrs(snake, delay, 9);
        }
        // Display score
        message = "Score: " + std::to_string(snake.getScore());
        background.draw_image(0, 0, 0, 0, score);
        background.draw_text(W/5, 0,message.data(), black, 0, 1, 30).resize(-100, -100, 1, 3);
        // Move snake and check keys events
        bool isCollide = snake.move(moveDirection);
        if (disp.is_keyA()) { moveDirection = moveDirection == 1? 1:0; }
        else if (disp.is_keyD()) { moveDirection = moveDirection == 0 ? 0 : 1; }
        else if (disp.is_keyW()) { moveDirection = moveDirection == 3 ? 3 : 2; }
        else if (disp.is_keyS()) { moveDirection = moveDirection == 2 ? 2 : 3; }
        //// Check if round is over.
        if (isCollide) {
            // Draw red circle near sneak head
            for (int r = 0; r < 50; r += 3) background.draw_circle(snake.getXHeadPosition(), snake.getYHeadPosition(), r, red, r / 300.0f).display(disp.wait(20));
            for (int rr = 0; rr < 50; rr += 3)
                ((+background) *= (50 - rr) / 50.0f).draw_circle(snake.getXHeadPosition(), snake.getYHeadPosition(), (50 + rr),red, 1 / 6.0f).display(disp.wait(20));      
            setNewRoundParametrs(snake, delay, 9);
        }
    }
    printRecordsInFile("Records.txt");
}

/*
        PRIVATE METHODS
*/

// Set game parametrs for new round and display menu
void SnakeGame::setNewRoundParametrs(Snake& snake, int delay, int snakeLength) {
    const unsigned char grey[3] = { 200, 200, 200 }, green[3] = { 50, 205, 50 };
    addRecord(snake.getScore()); // Add score in records
    background.assign(64, 64, 1, 3, 0).draw_rectangle(0, 0, W - 1, H - 1, green, 1.0f) // Clear bg
        .noise(10).resize(W, H + scoreTableH).blur(2).normalize(0, 128).draw_rectangle(0, 0, W - 1, scoreTableH - 1, grey, 1.0f);
    displayMenu(); // Display menu
    snake.clear(static_cast<const int>(delay * 6.5), firstBoxColour, secondBoxColour, colourEnlarge, colourShrink, boxSize, 9); // Clear snake
}
// Set colours of Apples and Snake
void SnakeGame::setSnakeColour(unsigned char colour1[3], unsigned char colour2[3]) {
    for (int i = 0; i < 3; i++) {
        this->firstBoxColour[i] = colour1[i];
        this->secondBoxColour[i] = colour2[i];
    }
}
void SnakeGame::setApplesColour(unsigned char colourEnlarge[3], unsigned char colourShrink[3]) {
    for (int i = 0; i < 3; i++) {
        this->colourEnlarge[i] = colourEnlarge[i];
        this->colourShrink[i] = colourShrink[i];
    }
}
// Display main game menu
void SnakeGame::displayMenu() {
    const unsigned char black[] = { 0,0,0 },  white[] = { 255,255,255 };
    // Display a simple pre-round page
    cimg_library::CImg<unsigned char> logo, pressakey;
    logo.draw_text(0, 30, " CImg-Snake", white, 0, 1, 33).resize(-100, -100, 1, 3);
    cimg_library::CImg<unsigned char> tmp = (+background).draw_text(3, 0, "To Records press 'R'", black, 0, 1, 22).draw_image((W - logo.width()) / 2, (H - logo.height()) / 2 - 20,
        logo, logo.get_channel(0).dilate(6).normalize(0, 1));
    pressakey.draw_text(0, 0, "* Press a key to start round *", white);
    for (float i = 0; i < 1; i += 0.05f) 
        ((+tmp) *= i).display(disp.wait(20));
    disp.flush();
    // Cycle for display menu images with flashing text. If any key pressed close menu
    for (unsigned long t = 0; !disp.key() && !disp.is_closed(); ++t) {
        if (!(t % 10)) {
            if (t % 20) disp.display(tmp);
            else disp.display((+tmp).draw_image(W / 2 - 70, H / 2 + 50, pressakey, pressakey, 1, 255));
        }
        if (disp.wait(20).is_resized()) disp.resize(disp);
    }
    if (disp.is_keyR()) displayRecords(); // If pressed 'R' show records
    if (disp.is_keyESC()) disp.close(); // If pressed 'ESC' end game
}
// Read no more than maximumRecords records from file. Records in file sorted.
void SnakeGame::readRecordsFromFile(std::string fileName) {
    records.clear();
    std::ifstream file;
    file.open(fileName);
    if (file.is_open()) {
        std::string word = ""; // string var. to contain read note from file
        while (!file.eof() && records.size() < maximumRecords) {
            file >> word;
            records.push_back(std::stoi(word));
        }
    }
}
void SnakeGame::readRecordsFromFileTest() {
    using namespace boost::ut;
    "readRecordsFromFile"_test = [this] {
        records.clear();
        // Create a file with some records
        std::string fileName = "test_records.txt";
        std::ofstream file(fileName);
        file << "5\n4\n3";
        file.close();

        // Read records from file
        readRecordsFromFile(fileName);

        // Check if the records match the contents of the file
        expect(records == std::vector<int>{5, 4, 3}) << "Records do not match expected records";
        records.clear();
        };
}
// Print records in file
void SnakeGame::printRecordsInFile(std::string fileName) {
    std::ofstream file;
    file.open(fileName);
    if (file.is_open()) {
        for (int i = 0; i < records.size() - 1; i++) {
            file << records[i] << '\n';
        }
        file << records[records.size() - 1];
    }
}
void SnakeGame::printRecordsInFileTest() {
    using namespace boost::ut;
    "printRecordsInFile"_test = [this] {
        records.clear();
        // Add some records
        addRecord(5);
        addRecord(3);
        addRecord(4);
        addRecord(-553);
        // Print records to file
        std::string fileName = "test_records.txt";
        printRecordsInFile(fileName);

        // Read the file and check if it matches the records
        std::ifstream file(fileName);
        std::stringstream buffer;
        buffer << file.rdbuf();

        std::string expected = "5\n4\n3\n-553";  // This assumes that records are sorted in falling order
        expect(buffer.str() == expected) << "Records in file do not match expected records";
        records.clear();
        };
}
// Display records on screen
void SnakeGame::displayRecords(){
    const unsigned char black[] = { 0,0,0 }, white[] = { 255,255,255 }, green[3] = { 50, 205, 50 };
    // Display a records background
    cimg_library::CImg<unsigned char> record;
    record.assign(64, 64, 1, 3, 0).draw_rectangle(0, 0, W - 1, H - 1, green, 1.0f)
        .noise(10).resize(W, H + scoreTableH).blur(2).normalize(0, 128);
    record.draw_text(static_cast<const int>(W / 2.7), 0, "Records", black);
    
    for (float i = 0; i < 1; i += 0.05f)
        ((+record) *= i).display(disp.wait(20));
    disp.flush();
    int i = 0;
    for (unsigned long t = 0; !disp.key() && !disp.is_closed(); ++t) {
        if (!(t % 10)) {
            if (!(t % 20)){
                if (!(i >= records.size())) {
                    // Display every record in vector with delay
                    std::string text = std::to_string(i+1) + ") " + std::to_string(records[i]);
                    record.draw_text(W/10, 15*(i+1), text.data(), black);
                    ++i;
                }              
            }
            disp.display(record);
        }
        if (disp.wait(20).is_resized()) disp.resize(disp);
    }
    // If any key pressed return to menu
    if (disp.key()) displayMenu();
}
// Add score to records, if it big enough. Records vector must be sorted, and after processing vector also will be sorted.
void SnakeGame::addRecord(int rec) {
    // If records have maximum size, check if score big enough and add in correct position
    if (records.size() == maximumRecords) {
        int temp = records[0];
        for (int i = 0; i < records.size(); i++) {
            if (records[i] < rec){
                temp = records[i];
                records[i] = rec;
                rec = temp;
            }
        }
    }
    // Else simply add in correct position
    else {
        for (int i = 0; i < records.size(); i++) {
            if (records[i] < rec) {
                records.insert(records.begin() + i, rec);
                return;
            }
        }
        records.push_back(rec);
    }
}
void SnakeGame::addRecordTest() {
    using namespace boost::ut;
    "addRecord"_test = [this] {
        records.clear();
        // Test when records vector is not full
        addRecord(5);
        expect(records == std::vector<int>{5});

        addRecord(3);
        expect(records == std::vector<int>{5, 3});

        addRecord(4);
        expect(records == std::vector<int>{5, 4, 3});

        // Fill up to maximumRecords
        for (int i = 6; records.size() < maximumRecords; ++i) {
            addRecord(i);
        }

        // Test when records vector is full
        addRecord(2);  // Smaller than smallest record, should not be added
        expect(records.back() != 2_i);
        addRecord(500);  // Larger than largest record, should be added
        expect(records.front() == 500_i && records.size() == maximumRecords);
        records.clear();
    };
}
void SnakeGame::gameTest() {
    Snake snake(*this, 65, firstBoxColour, secondBoxColour, colourEnlarge, colourShrink, boxSize, 9);
    snake.snakeTest();
    addRecordTest();
    printRecordsInFileTest();
    readRecordsFromFileTest();
}