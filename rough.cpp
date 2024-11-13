#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

// ======================
// Question Class
// ======================
class Question {
    string questionText;
    string options[4];
    char correctAnswer;

public:
    // Load question details from file
    bool loadQuestion(ifstream &file) {
        // Clear any residual data in the question and options
        questionText.clear();
        for (int i = 0; i < 4; ++i) {
            options[i].clear();
        }

        // Read the question text
        if (!getline(file, questionText) || questionText.empty()) {
            return false;  // Stop loading if we reach end or encounter an empty line
        }

        // Read the four options
        for (int i = 0; i < 4; ++i) {
            if (!getline(file, options[i]) || options[i].empty()) {
                return false; // Stop loading if there's an issue with options
            }
        }

        // Read the correct answer
        if (!(file >> correctAnswer)) {
            return false;  // Stop loading if there's an issue with the correct answer
        }
        file.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore remaining characters in line

        return true; // Successfully loaded question
    }

    // Display the question and options to the player
    void display() const {
        cout << questionText << endl;
        for (int i = 0; i < 4; ++i) {
            cout << char('A' + i) << ". " << options[i] << endl;
        }
    }

    // Check if the provided answer is correct
    bool isCorrect(char answer) const {
        return answer == correctAnswer;
    }
};

// ======================
// Player Class
// ======================
class Player {
    string name;
    int score;

public:
    Player(const string &playerName) : name(playerName), score(0) {}

    void increaseScore() { score += 10; }
    int getScore() const { return score; }
    string getName() const { return name; }
};

// ======================
// Quiz Class
// ======================
class Quiz {
    vector<Question> questions;
    Player player;

public:
    Quiz(const string &playerName) : player(playerName) {}

    bool loadQuestions(const string &filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Could not open file: " << filename << endl;
            return false;
        }

        // Load questions from the file until the end is reached
        while (file) {
            Question q;
            if (q.loadQuestion(file)) {
                questions.push_back(q);
            } else {
                // If loading a question fails, break out to avoid partial data
                break;
            }
            file.ignore(numeric_limits<streamsize>::max(), '\n'); // Skip any extra blank lines
        }

        file.close();
        return !questions.empty(); // Return true if at least one question was loaded
    }

    void start() {
        cout << "Welcome, " << player.getName() << "! Starting the quiz...\n\n";

        char answer;
        for (const auto &q : questions) {
            q.display();
            cout << "Your answer (A, B, C, or D): ";
            cin >> answer;
            answer = toupper(answer);

            if (q.isCorrect(answer)) {
                cout << "Correct!\n";
                player.increaseScore();
            } else {
                cout << "Incorrect!\n";
            }
            cout << "\n";
        }

        cout << "Quiz Over! " << player.getName() << ", your final score is: " << player.getScore() << endl;
    }

    void saveScore(const string &filename) const {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << player.getName() << ": " << player.getScore() << endl;
            file.close();
        } else {
            cout << "Could not open file: " << filename << " to save score.\n";
        }
    }
};

// ======================
// Main Function
// ======================
int main() {
    string playerName;
    cout << "Enter your name: ";
    getline(cin, playerName);

    Quiz quiz(playerName);

    if (quiz.loadQuestions("questions.txt")) {
        quiz.start();
        quiz.saveScore("scores.txt");
    } else {
        cout << "Failed to load questions. Please check the file.\n";
    }

    return 0;
}
