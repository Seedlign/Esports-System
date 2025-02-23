#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

struct Player {
    string PlayerName;
    string PlayerCity;
    string PlayerRegion;
    int PlayerKills;
    int PlayerDeaths;
    int PlayerTierPoints;

    double KDR() const {
        return (PlayerDeaths != 0) ? static_cast<double>(PlayerKills) / PlayerDeaths : 0.0;
    }
};

class PlayerScore {
public:
    vector<Player> ReadPlayersFromFile(const string& filename);
    void DisplaySortedPlayers(const vector<Player>& players);
    void QuickSort(vector<Player>& players, int left, int right);
    int Partition(vector<Player>& players, int left, int right);
    void MergeSortByRegionAndCity(vector<Player>& players, int left, int right);
    void MergeByRegionAndCity(vector<Player>& players, int left, int mid, int right);
    void MergeSortByTierPoints(vector<Player>& players, int left, int right);
    void MergeByTierPoints(vector<Player>& players, int left, int mid, int right);
    void SearchPlayerByUsername(vector<Player>& players);
    int BinarySearchByUsername(const vector<Player>& players, const string& username);
    void AddPlayerToFile(const string& filename);
    void DeletePlayerFromFile(const string& filename, const string& username);
    void DisplayMenu();
};

vector<Player> PlayerScore::ReadPlayersFromFile(const string& filename) {
    ifstream file(filename);
    vector<Player> players;
    string line;

    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return players;
    }

    bool isHeader = true;
    while (getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }

        stringstream ss(line);
        Player p;
        string kills, deaths, tierPoints;

        getline(ss, p.PlayerName, '|');
        getline(ss, kills, '|');
        getline(ss, deaths, '|');
        getline(ss, p.PlayerCity, '|');
        getline(ss, p.PlayerRegion, '|');
        getline(ss, tierPoints, '|');

        p.PlayerKills = stoi(kills);
        p.PlayerDeaths = stoi(deaths);
        p.PlayerTierPoints = stoi(tierPoints);
        players.push_back(p);
    }
    file.close();
    return players;
}

void PlayerScore::DisplaySortedPlayers(const vector<Player>& players) {
    cout << left << setw(20) << "USERNAME"
         << setw(10) << "KILLS"
         << setw(10) << "DEATHS"
         << setw(20) << "CITY"
         << setw(30) << "REGION"
         << setw(15) << "TIER POINTS"
         << setw(10) << "KD RATIO" << endl;
    cout << string(115, '-') << endl;
    for (const auto& player : players) {
        cout << left << setw(20) << player.PlayerName
             << setw(10) << player.PlayerKills
             << setw(10) << player.PlayerDeaths
             << setw(20) << player.PlayerCity
             << setw(30) << player.PlayerRegion
             << setw(15) << player.PlayerTierPoints
             << setw(10) << fixed << setprecision(2) << player.KDR() << endl;
    }
}

int PlayerScore::Partition(vector<Player>& players, int left, int right) {
    Player pivot = players[left];
    int i = left + 1, j = right;

    while (true) {
        while (i <= j && players[i].KDR() > pivot.KDR()) i++;
        while (i <= j && players[j].KDR() < pivot.KDR()) j--;
        if (i >= j) break;
        swap(players[i], players[j]);
    }
    swap(players[left], players[j]);
    return j;
}

void PlayerScore::QuickSort(vector<Player>& players, int left, int right) {
    if (left < right) {
        int pi = Partition(players, left, right);
        QuickSort(players, left, pi - 1);
        QuickSort(players, pi + 1, right);
    }
}

void PlayerScore::MergeByRegionAndCity(vector<Player>& players, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Player> leftArr(n1);
    vector<Player> rightArr(n2);

    for (int i = 0; i < n1; i++)
        leftArr[i] = players[left + i];

    for (int i = 0; i < n2; i++)
        rightArr[i] = players[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (leftArr[i].PlayerRegion < rightArr[j].PlayerRegion ||
           (leftArr[i].PlayerRegion == rightArr[j].PlayerRegion && leftArr[i].PlayerCity < rightArr[j].PlayerCity)) {
            players[k] = leftArr[i];
            i++;
        } else {
            players[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        players[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2) {
        players[k] = rightArr[j];
        j++;
        k++;
    }
}

void PlayerScore::MergeSortByRegionAndCity(vector<Player>& players, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        MergeSortByRegionAndCity(players, left, mid);
        MergeSortByRegionAndCity(players, mid + 1, right);
        MergeByRegionAndCity(players, left, mid, right);
    }
}

void PlayerScore::MergeByTierPoints(vector<Player>& players, int left, int mid, int right) {
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    vector<Player> leftPlayers(leftSize);
    vector<Player> rightPlayers(rightSize);

    for (int i = 0; i < leftSize; i++)
        leftPlayers[i] = players[left + i];
    for (int i = 0; i < rightSize; i++)
        rightPlayers[i] = players[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < leftSize && j < rightSize) {
        int leftTierGroup = (leftPlayers[i].PlayerTierPoints < 1000) ? 0 :
                            (leftPlayers[i].PlayerTierPoints <= 2500) ? 1 : 2;

        int rightTierGroup = (rightPlayers[j].PlayerTierPoints < 1000) ? 0 :
                             (rightPlayers[j].PlayerTierPoints <= 2500) ? 1 : 2;

        if (leftTierGroup < rightTierGroup) {
            players[k] = leftPlayers[i++];
        } else if (leftTierGroup > rightTierGroup) {
            players[k] = rightPlayers[j++];
        } else {
            if (leftPlayers[i].PlayerTierPoints <= rightPlayers[j].PlayerTierPoints) {
                players[k] = leftPlayers[i++];
            } else {
                players[k] = rightPlayers[j++];
            }
        }
        k++;
    }

    while (i < leftSize) {
        players[k++] = leftPlayers[i++];
    }
    while (j < rightSize) {
        players[k++] = rightPlayers[j++];
    }
}

void PlayerScore::MergeSortByTierPoints(vector<Player>& players, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        MergeSortByTierPoints(players, left, mid);
        MergeSortByTierPoints(players, mid + 1, right);
        MergeByTierPoints(players, left, mid, right);
    }
}

int PlayerScore::BinarySearchByUsername(const vector<Player>& players, const string& username) {
    int left = 0, right = players.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (players[mid].PlayerName == username) {
            return mid;
        } else if (players[mid].PlayerName < username) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

void PlayerScore::SearchPlayerByUsername(vector<Player>& players) {
    string username;
    cout << "Enter username to search: ";
    cin >> username;

    sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
        return a.PlayerName < b.PlayerName;
    });

    int index = BinarySearchByUsername(players, username);

    if (index != -1) {
        const Player& player = players[index];
        cout << "\nPlayer Found!" << endl;
        cout << left << setw(20) << "USERNAME"
             << setw(10) << "KILLS"
             << setw(10) << "DEATHS"
             << setw(20) << "CITY"
             << setw(30) << "REGION"
             << setw(15) << "TIER POINTS"
             << setw(10) << "KD RATIO" << endl;
        cout << string(115, '-') << endl;
        cout << left << setw(20) << player.PlayerName
             << setw(10) << player.PlayerKills
             << setw(10) << player.PlayerDeaths
             << setw(20) << player.PlayerCity
             << setw(30) << player.PlayerRegion
             << setw(15) << player.PlayerTierPoints
             << setw(10) << fixed << setprecision(2) << player.KDR() << endl;
    } else {
        cout << "Player not found!" << endl;
    }
}

void PlayerScore::AddPlayerToFile(const string& filename) {
    ofstream file(filename, ios::app);
    if (!file) {
        cerr << "Error opening file!" << endl;
        return;
    }

    Player newPlayer;

    // Function to handle input with error checking
    auto getInput = [](const string& prompt, auto& value) {
        while (true) {
            cout << prompt;
            cin >> value;
            if (cin.fail()) {
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                cout << "Invalid input. Please try again.\n";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                break;
            }
        }
    };

    getInput("Enter Username: ", newPlayer.PlayerName);
    getInput("Enter Kills: ", newPlayer.PlayerKills);
    getInput("Enter Deaths: ", newPlayer.PlayerDeaths);
    getInput("Enter City: ", newPlayer.PlayerCity);
    getInput("Enter Region: ", newPlayer.PlayerRegion);
    getInput("Enter Tier Points: ", newPlayer.PlayerTierPoints);

    file << newPlayer.PlayerName << "|" << newPlayer.PlayerKills << "|"
         << newPlayer.PlayerDeaths << "|" << newPlayer.PlayerCity << "|"
         << newPlayer.PlayerRegion << "|" << newPlayer.PlayerTierPoints << endl;

    file.close();
    cout << "Player added successfully!\n";
}
void PlayerScore::DeletePlayerFromFile(const string& filename, const string& username) {
    vector<Player> players = ReadPlayersFromFile(filename);
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file!" << endl;
        return;
    }

    bool found = false;
    file << "USERNAME|KILLS|DEATHS|CITY|REGION|TIER POINTS\n";
    for (const auto& player : players) {
        if (player.PlayerName != username) {
            file << player.PlayerName << "|" << player.PlayerKills << "|"
                 << player.PlayerDeaths << "|" << player.PlayerCity << "|"
                 << player.PlayerRegion << "|" << player.PlayerTierPoints << endl;
        } else {
            found = true;
        }
    }

    file.close();
    if (found) {
        cout << "Player deleted successfully!\n";
    } else {
        cout << "Player not found.\n";
    }
}

void PlayerScore::DisplayMenu() {
    vector<Player> players = ReadPlayersFromFile("Players_200.txt");
    int choice;

    while (true) {
        cout << "\n=====================================" << endl;
        cout << "         PLAYER MANAGEMENT MENU      " << endl;
        cout << "=====================================" << endl;
        cout << "[1] Display Players" << endl;
        cout << "[2] Quick Sort (K/D Ratio)" << endl;
        cout << "[3] Merge Sort Menu" << endl;
        cout << "[4] Search for a Player" << endl;
        cout << "[5] Add a New Player" << endl;
        cout << "[6] Delete a Player" << endl;
        cout << "[7] Exit" << endl;
        cout << "-------------------------------------" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number between 1 and 7.\n";
            continue;
        }

        switch (choice) {
            case 1:
                DisplaySortedPlayers(players);
                break;
            case 2:
                QuickSort(players, 0, players.size() - 1);
                cout << "Players sorted by KD Ratio." << endl;
                break;
            case 3: {
                int sortChoice;
                while (true) {
                    cout << "\nMerge Sort Menu:" << endl;
                    cout << "1. Sort by Region & City" << endl;
                    cout << "2. Sort by Tier Points" << endl;
                    cout << "Enter choice: ";

                    if (!(cin >> sortChoice)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input! Please enter 1 or 2.\n";
                        continue;
                    }

                    if (sortChoice == 1) {
                        MergeSortByRegionAndCity(players, 0, players.size() - 1);
                        cout << "Players sorted by Region & City." << endl;
                        break;
                    } else if (sortChoice == 2) {
                        MergeSortByTierPoints(players, 0, players.size() - 1);
                        cout << "Players sorted by Tier Points (0-1000, 1001-2500, 2500+)." << endl;
                        break;
                    } else {
                        cout << "Invalid choice. Please enter 1 or 2.\n";
                    }
                }
                break;
            }
            case 4:
                SearchPlayerByUsername(players);
                break;
            case 5:
                AddPlayerToFile("Players_200.txt");
                players = ReadPlayersFromFile("Players_200.txt");
                break;
            case 6: {
                string username;
                while (true) {
                    cout << "Enter username to delete: ";
                    cin >> username;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input! Please enter a valid username.\n";
                    } else {
                        break;
                    }
                }

                DeletePlayerFromFile("Players_200.txt", username);
                players = ReadPlayersFromFile("Players_200.txt");
                break;
            }
            case 7:
                cout << "Exiting..." << endl;
                return;
            default:
                cout << "Invalid choice, try again." << endl;
        }
    }
}

int main() {
    PlayerScore score;
    score.DisplayMenu();
    return 0;
}
