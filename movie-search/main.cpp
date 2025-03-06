#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <chrono>
#include "Movie.h"

using namespace std;

//Quick Sort and Merge Sort pseudocode referenced from DSA Discussion 8 slides
int partition(vector<Movie> &uniqueMovies, int low, int high) {
    int pivot = uniqueMovies[low].getSimilarity();
    int up = low;
    int down = high;

    while (up < down) {
        for (int i = up; i < high; i++) {
            if (uniqueMovies[up].getSimilarity() < pivot)
                break;
            up++;
        }

        for (int i = high; i > low; i--) {
            if (uniqueMovies[down].getSimilarity() > pivot)
                break;
            down--;
        }

        if (up < down) {
            uniqueMovies[up].swapMovies(uniqueMovies[down]);
        }
    }

    uniqueMovies[low].swapMovies(uniqueMovies[down]);
    return down;
}

void quickSort(vector<Movie> &uniqueMovies, int low, int high){
    if(low < high){
        int pivot = partition(uniqueMovies, low, high);
        quickSort(uniqueMovies, low, pivot - 1);
        quickSort(uniqueMovies, pivot + 1, high);
    }
}

void merge(vector<Movie> &movies, int left, int mid, int right) {
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    vector<Movie> leftM, rightM;
    leftM.reserve(leftSize);
    rightM.reserve(rightSize);
    for (int i = 0; i < leftSize; i++)
        leftM.push_back(movies[left + i]);
    for (int i = 0; i < rightSize; i++)
        rightM.push_back(movies[mid + 1 + i]);

    int leftPtr = 0;
    int rightPtr = 0;
    int mergedPtr = left;

    while (leftPtr < leftSize && rightPtr < rightSize){
        if (leftM[leftPtr].getSimilarity() > rightM[rightPtr].getSimilarity()) {
            movies[mergedPtr] = leftM[leftPtr];
            leftPtr++;
        }

        else {
            movies[mergedPtr] = rightM[rightPtr];
            rightPtr++;
        }

        mergedPtr++;
    }

    while (leftPtr < leftSize) {
        movies[mergedPtr] = leftM[leftPtr];
        mergedPtr++;
        leftPtr++;
    }

    while (rightPtr < rightSize) {
        movies[mergedPtr] = rightM[rightPtr];
        mergedPtr++;
        rightPtr++;
    }
}

void mergeSort(vector<Movie> &movies, int start, int end){
    if(start < end){
        int middle = start + (end-start)/2;
        mergeSort(movies, start, middle);
        mergeSort(movies, middle+1, end);
        merge(movies, start, middle, end);
    }
}

int main() {
    //Read in data from file
    ifstream file("movieReviews/movielens.csv", ios::in);
    int id, year;
    string title, genre;
    float rating;
    string line;

    //Map to organize unique movies their average ratings
    //Key = Movie title, pair = frequency and Movie
    unordered_map<string, pair<int, Movie*>> organize;

    //Vector of movies to sort
    vector<Movie> uniqueMovies;

    //Skip header row in database
    getline(file, line);
    string curr;

    if (file.is_open()) {
        for (int i = 0; i < 100000; i++) {
            getline(file, line);
            stringstream s(line);
            getline(s, curr, ',');
            getline(s, curr, ',');
            id = stoi(curr);
            getline(s, curr, ',');
            title = curr;
            if (title[0] == '"') {
                string p2;
                getline(s, p2, '"');
                title = title.substr(1, title.size() - 1) + "," + p2.substr(0, p2.size());
                getline(s, curr, ',');
            }
            getline(s, curr, ',');
            year = stoi(curr);
            getline(s, curr, ',');
            genre = curr;
            getline(s, curr, ',');
            getline(s, curr, ',');
            rating = stof(curr);
            getline(s, curr, ',');

            //If the movie is already in the map
            if (organize.count(title) == 1) {
                organize[title].first++;
                organize[title].second->setRating(organize[title].second->getRating() + rating);
            } else {
                Movie *n = new Movie(id, title, year, genre, rating);
                pair<int, Movie *> newMovie = {1, n};
                organize[title] = newMovie;
                uniqueMovies.push_back(*n);
            }
        }
    }
    file.close();

    //Take user input
    string movieInput;
    string numOutputs;
    string sort;
    cout << "Enter a movie title:";
    getline(cin, movieInput, '\n');
    while (organize.count(movieInput) == 0){
        cout << "Error: invalid movie name, enter another movie title:";
        getline(cin, movieInput, '\n');
    }

    cout << "Enter the number of similar movies (1 to 1000):";
    getline(cin, numOutputs, '\n');
    while (stoi(numOutputs)>1000 || stoi(numOutputs)<1){
        cout << "Error, enter another number (1 to 1000):";
        getline(cin, numOutputs, '\n');
    }

    cout << "Enter 1 to sort by Merge Sort, or 2 to sort by Quick Sort:";
    getline(cin, sort, '\n');
    while (sort != "1" && sort != "2"){
        cout << "Error, enter another number, 1 for Merge Sort, 2 for Quick Sort:";
        getline(cin, sort, '\n');
    }

    //Calculate average rating and similarity score for each movie
    for (auto it = uniqueMovies.begin(); it < uniqueMovies.end(); it++) {
        it->setRating(organize[it->getTitle()].second->getRating() / organize[it->getTitle()].first);
        it->calcSimilarity(*organize[movieInput].second);
    }

    auto begin = std::chrono::high_resolution_clock::now();

    //Sort vector of movies by similarity score
    if(sort == "1") {
        mergeSort(uniqueMovies, 0, uniqueMovies.size() - 1);
    } else {
        quickSort(uniqueMovies, 0, uniqueMovies.size()-1);
    }

    auto end = chrono::high_resolution_clock::now();

    auto dur = chrono::duration_cast<chrono::microseconds>(end - begin);

    // Displaying the elapsed time
    std::cout << "Elapsed Time: " << dur.count() << " ms";

    //Print most similar movies
    cout << "\n";
    cout << "Movie Title - Similarity Score Out of 100" << endl;
    cout << "*****************************************" << endl;
    for(int i = 0; i < stoi(numOutputs); i++)
        cout << uniqueMovies[i].getTitle() << " - " << uniqueMovies[i].getSimilarity() << endl;

    return 0;
}